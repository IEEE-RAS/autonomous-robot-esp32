import logging
import socket
import json
import threading
from flask import Flask, request, jsonify, send_from_directory, make_response
from flask_cors import CORS
from datetime import datetime
import sqlite3
import os
import cv2 


logging.basicConfig(level=logging.INFO, format='[%(levelname)s] %(asctime)s - %(message)s')

DB_NAME = 'planet_exploration.db'
PHOTOS_DIR = 'C:/Users/naila/Documents/GitHub/autonomous-robot-esp32photos' # so p teste 
DATA_PORT = 9998
COMMAND_PORT = 9999
FLASK_PORT = 5001

class Broker:
    def __init__(self, data_port, command_port):
        self.ipHost = '0.0.0.0'
        self.data_port = data_port
        self.command_port = command_port
        self.devices = {}   
        self.datas = {}    
        self.lock = threading.Lock()
        self.init_database()
        # self.last_photo_path = None # Não eh mais necessário 
        self.setup_data_server()
        self.setup_command_server()

    def init_database(self):
        """Inicializa o banco de dados SQLite e cria a tabela se não existir."""
        conn = sqlite3.connect(DB_NAME, check_same_thread=False)
        cursor = conn.cursor()
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS sensor_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                device_name TEXT NOT NULL,
                timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
                temperature REAL,
                humidity REAL,
                gas REAL,
                light REAL,
                life_probability REAL,
                terrain_status TEXT,
                photo_url TEXT DEFAULT NULL -- NOVA COLUNA para o URL da foto
            )
        ''')
        conn.commit()
        conn.close()
        logging.info(f"Banco de dados SQLite '{DB_NAME}' inicializado.")
        os.makedirs(PHOTOS_DIR, exist_ok=True)
        logging.info(f"Diretório de fotos '{PHOTOS_DIR}' verificado/criado.")

    def store_sensor_data(self, device_name, data, photo_url=None):
        """Armazena os dados dos sensores no banco de dados SQLite."""
        try:
            conn = sqlite3.connect(DB_NAME, check_same_thread=False)
            cursor = conn.cursor()
            cursor.execute('''
                INSERT INTO sensor_data (device_name, temperature, humidity, gas, light, life_probability, terrain_status, photo_url)
                VALUES (?, ?, ?, ?, ?, ?, ?, ?)
            ''', (
                device_name,
                data.get('temp'),
                data.get('hum'),
                data.get('gas'),
                data.get('lux'),
                data.get('life_chance'),
                data.get('terrain_status'),
                photo_url 
            ))
            conn.commit()
            conn.close()
        except Exception as e:
            logging.error(f"Erro ao armazenar dados no banco de dados: {e}")

    def setup_data_server(self):
        """Configura o servidor UDP para receber dados dos dispositivos."""
        self.data_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.data_sock.bind((self.ipHost, self.data_port))
        logging.info(f"Ouvindo os dados UDP na porta {self.data_port}")
        threading.Thread(target=self.process_data, daemon=True).start()

    def setup_command_server(self):
        """Configura o servidor TCP para registro e comandos."""
        self.command_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.command_socket.bind((self.ipHost, self.command_port))
        self.command_socket.listen(6)
        logging.info(f"Ouvindo os comandos TCP na porta {self.command_port}")
        threading.Thread(target=self.start_listening_for_devices, daemon=True).start()

    def process_data(self):
        """Processa pacotes UDP, armazena os dados na memória e no DB."""
        while True:
            try:
                data, sender_address = self.data_sock.recvfrom(4096)
                message = json.loads(data.decode())
                device_name = message.get('source')

                if not device_name:
                    continue

                with self.lock:
                    self.datas[device_name] = message
                    
                    self.store_sensor_data(device_name, message.get('data', {}))

            except json.JSONDecodeError:
                logging.warning(f"Recebida mensagem UDP mal formatada.")
            except Exception as e:
                logging.error(f"Erro no process_data: {e}")

    def manage_device_connection(self, connection, sender_address):
        """Gerencia a conexão TCP de um dispositivo (registro e recebimento de comandos)."""
        registered_name = None
        with connection:
            logging.info(f"Nova conexão TCP de: {sender_address}")
            try:
                while True: 
                    data = connection.recv(1024).decode()
                    if not data: 
                        break
                    
                    message = json.loads(data)
                    
                    if message.get("type") == "register":
                        device_name = message.get("name")
                        if device_name:
                            with self.lock:
                                self.devices[device_name] = connection
                                registered_name = device_name
                            logging.info(f"Dispositivo '{device_name}' registrado via TCP.")
                    elif message.get("type") == "command": 
                        command_type = message.get("command_type")
                        if command_type == "take_photo":
                            logging.info(f"Comando 'take_photo' recebido do ESP32 '{registered_name}'.")
                            self.take_photo(registered_name) # Passa device_name
                        else:
                            logging.warning(f"Comando desconhecido do ESP32: {command_type}")

            except (ConnectionResetError, json.JSONDecodeError, socket.timeout):
                pass 
            except Exception as e:
                logging.error(f"Erro em manage_device_connection: {e}")
            finally:
                if registered_name:
                    with self.lock:
                        if registered_name in self.devices:
                            del self.devices[registered_name]
                    logging.info(f"Dispositivo '{registered_name}' desconectado.")

    def start_listening_for_devices(self):
        """Inicia a thread para gerenciar conexões com os dispositivos."""
        while True:
            connection, sender_address = self.command_socket.accept()
            connection.settimeout(300) 
            threading.Thread(target=self.manage_device_connection, args=(connection, sender_address), daemon=True).start()

    def take_photo(self, device_name):
        """Captura uma foto da webcam, a salva e registra o caminho no DB."""
        cap = cv2.VideoCapture(0) # Tenta abrir a primeira webcam disponível

        if not cap.isOpened():
            logging.error("Não foi possível abrir a webcam. Verifique se ela está conectada e não está em uso.")
            return

        ret, frame = cap.read() # Captura um frame

        if not ret:
            logging.error("Não foi possível capturar o frame da webcam.")
            cap.release()
            return

        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        photo_filename = f"photo_{timestamp}.jpg"
        photo_path = os.path.join(PHOTOS_DIR, photo_filename)
        photo_url_for_frontend = f"/photos/{photo_filename}" 

        success = cv2.imwrite(photo_path, frame)
        if success:
            logging.info(f"Foto salva em: {photo_path}")
            self.update_latest_sensor_data_with_photo_url(device_name, photo_url_for_frontend)
        else:
            logging.error(f"Falha ao salvar a foto em: {photo_path}")

        cap.release() # Libera a webcam

    def update_latest_sensor_data_with_photo_url(self, device_name, photo_url):
        """Atualiza o último registro de sensor para um dispositivo com o URL da foto."""
        try:
            conn = sqlite3.connect(DB_NAME, check_same_thread=False)
            cursor = conn.cursor()
            cursor.execute('''
                UPDATE sensor_data
                SET photo_url = ?
                WHERE id = (
                    SELECT id FROM sensor_data
                    WHERE device_name = ?
                    ORDER BY timestamp DESC
                    LIMIT 1
                )
            ''', (photo_url, device_name))
            conn.commit()
            conn.close()
            if cursor.rowcount > 0:
                logging.info(f"URL da foto '{photo_url}' atualizado no último registro de '{device_name}'.")
            else:
                logging.warning(f"Não foi encontrado registro para atualizar com a foto para '{device_name}'.")
        except Exception as e:
            logging.error(f"Erro ao atualizar URL da foto no banco de dados: {e}")


app = Flask(__name__)
CORS(app)
broker = Broker(DATA_PORT, COMMAND_PORT)


@app.route('/', methods=['GET'])
def index():
    return jsonify({"status": "broker_online", "version": "1.2", "message": "Robo Explorador API"})

@app.route('/devices', methods=['GET'])
def get_devices():
    return jsonify(list(broker.datas.keys()))

@app.route('/devices/<device_name>/data', methods=['GET'])
def get_device_data_api(device_name):
    with broker.lock:
        latest_data = broker.datas.get(device_name)
    
    response = jsonify(latest_data if latest_data else {"status": "waiting_for_data", "message": f"Dispositivo '{device_name}' conectado, aguardando dados."})
    response.status_code = 200 if latest_data or device_name in broker.devices else 404
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Pragma"] = "no-cache"
    response.headers["Expires"] = "0"
    return response


@app.route('/devices/<device_name>/command', methods=['POST'])
def send_command_api(device_name):
    with broker.lock:
        connection = broker.devices.get(device_name)
    
    if not connection:
        return jsonify({'error': f"Dispositivo '{device_name}' não está conectado para receber comandos."}), 404

    command_data = request.json
    if not command_data or 'command_type' not in command_data:
        return jsonify({'error': 'Payload do comando inválido. Esperado {"command_type": "seu_comando"}'}), 400
    
    try:
        command_payload = json.dumps({"command": command_data['command_type']})
        connection.sendall((command_payload + '\n').encode()) 
        logging.info(f"Comando '{command_payload}' enviado para o dispositivo: '{device_name}'")
        return jsonify({'message': f"Comando '{command_data['command_type']}' enviado."}), 200
    except Exception as e:
        logging.error(f"Erro ao enviar comando para '{device_name}': {e}")
        return jsonify({'error': 'Falha ao enviar comando para o dispositivo.'}), 500

# aq eh a rota p ele obter a foto por meio do bd 
@app.route('/photos/latest', methods=['GET'])
def get_latest_photo_url():
    with broker.lock:
        conn = sqlite3.connect(DB_NAME, check_same_thread=False)
        cursor = conn.cursor()
        cursor.execute("SELECT photo_url FROM sensor_data WHERE photo_url IS NOT NULL ORDER BY timestamp DESC LIMIT 1")
        result = cursor.fetchone()
        conn.close()

        if result and result[0]:
            photo_url = result[0] 
            response = jsonify({"photo_url": photo_url})
            response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
            response.headers["Pragma"] = "no-cache"
            response.headers["Expires"] = "0"
            return response
        else:
            response = jsonify({"error": "Nenhuma foto disponível."})
            response.status_code = 404
            response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
            response.headers["Pragma"] = "no-cache"
            response.headers["Expires"] = "0"
            return response


@app.route('/photos/<filename>')
def serve_photo(filename):
    """Serve um arquivo de foto do diretório PHOTOS_DIR."""
    response = make_response(send_from_directory(PHOTOS_DIR, filename))
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Pragma"] = "no-cache"
    response.headers["Expires"] = "0"
    return response

if __name__ == "__main__":
    host_ip = socket.gethostbyname(socket.gethostname())
    logging.info(f"Flask rodando em http://{host_ip}:{FLASK_PORT}")
    app.run(host='0.0.0.0', port=FLASK_PORT)
