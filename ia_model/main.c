// Executar no Arduino IDE com as dependências instaladas e com o arquivo ia_model.h no mesmo diretório

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "ia_model.h"

// Mapeamento dos sensores
#define ADC1_0 36 // Sensor de Temperatura
#define ADC1_3 39 // Sensor de Umidade
#define ADC1_6 34 // Sensor de Gás
#define ADC1_7 35 // Sensor de Luz
#define BUTTON 21

// Token do seu bot
const char* botToken = "7515864309:AAH6vmNAB6jFyaUwMjr-88lPAAXQrh07ZIo";

// Wi-Fi
const char* ssid = "Galaxy A51 430B";
const char* password = "ladygaga123";

// TLS e Bot
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);
unsigned long lastTimeBotRan = 0;
const unsigned long botInterval = 1000; // 1s

// Limites reais dos sensores
#define MAX_TEMPERATURE_C 50.0f
#define MAX_GAS_PPM       217.79f
#define MAX_LIGHT_CD      1086.46f

// Controle de leitura
unsigned long lastSensorRead = 0;
const unsigned long interval = 500;

volatile bool buttonPressed = false;
bool systemOn = true;

// Prototipação 
void IRAM_ATTR handleButtonInterrupt();
void connectWiFi();
void handleTelegramMessages();

// IA
float relu(float x) { return x > 0 ? x : 0; }
float sigmoid(float x) { return 1.0f / (1.0f + expf(-x)); }

float model_predict(const float x[INPUT_SIZE]) {
  float hidden[HIDDEN_SIZE];
  for (int i = 0; i < HIDDEN_SIZE; ++i) {
    hidden[i] = b1[i];
    for (int j = 0; j < INPUT_SIZE; ++j) {
      hidden[i] += x[j] * W1[j][i];
    }
    hidden[i] = relu(hidden[i]);
  }

  float output = b2;
  for (int i = 0; i < HIDDEN_SIZE; ++i) {
    output += hidden[i] * W2[i];
  }
  return sigmoid(output);
}

void normalize_readings(float input[INPUT_SIZE]) {
  input[0] /= MAX_TEMPERATURE_READING;
  input[1] /= 100.0f;
  input[2] /= MAX_GAS_READING;
  input[3] /= MAX_LIGHT_READING;
}

// Setup 
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), handleButtonInterrupt, FALLING);
  connectWiFi();
  client.setInsecure();
}

// Loop principal
void loop() {
  unsigned long currentTime = millis();

  if (millis() - lastTimeBotRan > botInterval) {
    handleTelegramMessages();
    lastTimeBotRan = millis();
  }

  if (currentTime - lastSensorRead >= interval) {
    lastSensorRead = currentTime;

    Serial.println("============================================");

    if (systemOn) {
      int rawTemp = analogRead(ADC1_0);
      int rawHum  = analogRead(ADC1_3);
      int rawGas  = analogRead(ADC1_6);
      int rawLux  = analogRead(ADC1_7);

      float temp = ((float)rawTemp / 4095.0f) * MAX_TEMPERATURE_C;
      float hum  = ((float)rawHum / 4095.0f) * 100.0f;
      float gas  = ((float)rawGas / 4095.0f) * MAX_GAS_PPM;
      float lux  = ((float)rawLux / 4095.0f) * MAX_LIGHT_CD;

      Serial.printf("Temperatura: %.2f °C\n", temp);
      Serial.printf("Umidade: %.2f %%\n", hum);
      Serial.printf("Gás: %.2f ppm\n", gas);
      Serial.printf("Luminosidade: %.2f cd\n", lux);

      float input[INPUT_SIZE] = { temp, hum, gas, lux };
      normalize_readings(input);
      float chance = model_predict(input);

      Serial.printf("Chance de vida: %.2f%%\n", chance * 100);
      if (chance >= 0.70)
        Serial.println("Status do planeta: Propício à vida ✅");
      else if (chance >= 0.5)
        Serial.println("Status do planeta: Condição Moderada 🟨");
      else
        Serial.println("Status do planeta: Ambiente Hostil ❌");

      Serial.print("Status Wi-Fi: ");
      Serial.println(WiFi.status() == WL_CONNECTED ? "Conectado" : "Desconectado");
    } else {
      Serial.println("Sensores desativados.");
    }

    Serial.print("Status do Sistema: ");
    Serial.println(systemOn ? "Ligado" : "Desligado");
  }

  if (buttonPressed) {
    buttonPressed = false;
    systemOn = !systemOn;
  }
}

// Wi-Fi 
void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Interrupção do botão
void IRAM_ATTR handleButtonInterrupt() {
  static unsigned long lastInterruptTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastInterruptTime > 200) {
    lastInterruptTime = currentTime;
    buttonPressed = true;
  }
}

// Telegram 
void handleTelegramMessages() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  while (numNewMessages) {
    for (int i = 0; i < numNewMessages; i++) {
      String chat_id = bot.messages[i].chat_id;
      String text = bot.messages[i].text;

      if (text == "/start") {
        bot.sendMessage(chat_id,
          "🛡 *Orisa Online!*\n\n"
          "Sou sua IA de defesa ambiental.\n"
          "Estou aqui para monitorar o ambiente e garantir que tudo esteja sob controle. 💚\n\n"
          "📋 *Comandos disponíveis:*\n"
          "🔍 /status – Verificar estado do sistema\n"
          "📡 /sensores – Dados ambientais atuais\n"
          "🧭 /ajuda – Lista de comandos\n", 
          "Markdown");
      } 

      else if (text == "/status") {
        String status = systemOn ? "🟢 *Sistema:* Ligado" : "🔴 *Sistema:* Desligado";
        String wifiStatus = WiFi.status() == WL_CONNECTED
          ? "📶 *Wi-Fi:* Conectado\n🌐 *IP:* " + WiFi.localIP().toString()
          : "📶 *Wi-Fi:* Desconectado";

        bot.sendMessage(chat_id,
          "📊 *Status do Sistema*\n\n" +
          status + "\n" + wifiStatus, 
          "Markdown");
      }

      else if (text == "/sensores") {
        int rawTemp = analogRead(ADC1_0);
        int rawHum  = analogRead(ADC1_3);
        int rawGas  = analogRead(ADC1_6);
        int rawLux  = analogRead(ADC1_7);

        float temp = ((float)rawTemp / 4095.0f) * MAX_TEMPERATURE_C;
        float hum  = ((float)rawHum / 4095.0f) * 100.0f;
        float gas  = ((float)rawGas / 4095.0f) * MAX_GAS_PPM;
        float lux  = ((float)rawLux / 4095.0f) * MAX_LIGHT_CD;

        float input[INPUT_SIZE] = { temp, hum, gas, lux };
        normalize_readings(input);
        float chance = model_predict(input);

        String statusPlaneta;
        if (chance >= 0.70)
          statusPlaneta = "🟢 *Propício à vida* ✅";
        else if (chance >= 0.5)
          statusPlaneta = "🟡 *Condição Moderada* ⚠️";
        else
          statusPlaneta = "🔴 *Ambiente Hostil* ❌";

        String message = "📡 *Leituras Atuais dos Sensores:*\n\n";
        message += "🌡 *Temperatura:* " + String(temp, 2) + " °C\n";
        message += "💧 *Umidade:* " + String(hum, 2) + " %\n";
        message += "🧪 *Gás:* " + String(gas, 2) + " ppm\n";
        message += "💡 *Luminosidade:* " + String(lux, 2) + " cd\n\n";
        message += "🔬 *Chance de vida:* " + String(chance * 100, 2) + " %\n";
        message += "🛰 *Status do planeta:* " + statusPlaneta;



        bot.sendMessage(chat_id, message, "Markdown");
      }

      else if (text == "/ajuda") {
        bot.sendMessage(chat_id,
          "📘 *Ajuda Orisa*\n\n"
          "Use os comandos abaixo para interagir comigo:\n"
          "🔍 /status – Ver estado do sistema\n"
          "📡 /sensores – Ver leituras dos sensores\n"
          "🧭 /ajuda – Mostrar esta mensagem novamente\n", 
          "Markdown");
      }

      else {
        bot.sendMessage(chat_id,
          "❌ *Comando não reconhecido!*\n\n"
          "Use /ajuda para ver a lista de comandos disponíveis.", 
          "Markdown");
      }
    }
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
}
