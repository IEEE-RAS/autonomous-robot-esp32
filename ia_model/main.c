// Executar no Arduino IDE com as dependências instaladas e com o arquivo ia_model.h no mesmo diretório

#include <WiFi.h>
#include "ia_model.h"  // Modelo IA gerado (header com externs e constantes)

// Mapeamento dos sensores
#define ADC1_0 36 // Sensor de Temperatura
#define ADC1_3 39 // Sensor de Umidade
#define ADC1_6 34 // Sensor de Gás
#define ADC1_7 35 // Sensor de Luz
#define BUTTON 21

// Valores reais máximos do seu dataset treinado
#define MAX_TEMPERATURE_C 50.0f     // Temperatura real (°C)
#define MAX_GAS_PPM       217.79f   // Gás real (ppm)
#define MAX_LIGHT_CD      1086.46f  // Luz real (cd)

// Wi-Fi
const char* ssid = "Galaxy A51 430B";
const char* password = "ladygaga123";

// Controle de leitura
unsigned long lastSensorRead = 0;
const unsigned long interval = 500; // Intervalo de leitura (0,5s)

// Controle de botão e estado do sistema
volatile bool buttonPressed = false;
bool systemOn = true;

// Prototipação
void IRAM_ATTR handleButtonInterrupt();
void connectWiFi();

// Implementações diretas da IA
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
  input[0] /= MAX_TEMPERATURE_READING;  // Temperatura
  input[1] /= 100.0f;                   // Umidade (convertida para 0-1 acima)
  input[2] /= MAX_GAS_READING;          // Gás
  input[3] /= MAX_LIGHT_READING;        // Luz
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), handleButtonInterrupt, FALLING);
  connectWiFi();
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastSensorRead >= interval) {
    lastSensorRead = currentTime;

    Serial.println("=============================================");

    if (systemOn) {
      int rawTemp = analogRead(ADC1_0);
      int rawHum  = analogRead(ADC1_3);
      int rawGas  = analogRead(ADC1_6);
      int rawLux  = analogRead(ADC1_7);

      // Conversões para valores reais:
      float temp = ((float)rawTemp / 4095.0f) * MAX_TEMPERATURE_C;
      float hum  = ((float)rawHum / 4095.0f) * 100.0f;
      float gas  = ((float)rawGas / 4095.0f) * MAX_GAS_PPM;
      float lux  = ((float)rawLux / 4095.0f) * MAX_LIGHT_CD;

      Serial.print("Temperatura: ");
      Serial.print(temp, 2);
      Serial.println(" °C");

      Serial.print("Umidade: ");
      Serial.print(hum, 2);
      Serial.println(" %");

      Serial.print("Gás: ");
      Serial.print(gas, 2);
      Serial.println(" ppm");

      Serial.print("Luminosidade: ");
      Serial.print(lux, 2);
      Serial.println(" cd");

      // Preparar vetor para normalização
      float input[INPUT_SIZE] = { temp, hum, gas, lux };
      normalize_readings(input);

      float chance = model_predict(input);
      Serial.print("Chance de vida: ");
      Serial.print(chance * 100.0f);
      Serial.println("%");

      if (chance >= 0.70) {
        Serial.println("Status do planeta: Propício à vida ✅");
      } else if (chance >= 0.5) {
        Serial.println("Status do planeta: Condição Moderada 🟨");
      } else {
        Serial.println("Status do planeta: Ambiente Hostil ❌");
      }

      Serial.print("Status Wi-Fi: ");
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Conectado - IP: ");
        Serial.println(WiFi.localIP());
      } else {
        Serial.println("Desconectado");
      }

    } else {
      Serial.println("Sensores desativados.");
      Serial.println("Status Wi-Fi: Desconectado");
    }

    Serial.print("Status do Sistema: ");
    Serial.println(systemOn ? "Ligado" : "Desligado");
  }

  if (buttonPressed) {
    buttonPressed = false;
    systemOn = !systemOn;
  }
}

void IRAM_ATTR handleButtonInterrupt() {
  static unsigned long lastInterruptTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastInterruptTime > 200) {
    lastInterruptTime = currentTime;
    buttonPressed = true;
  }
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 10000;
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    yield();
  }
  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }
}
