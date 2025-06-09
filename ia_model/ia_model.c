#include "ia_model.h"


/* Função de ativação ReLU */
float relu(float x) {
    return x > 0 ? x : 0;
}

/* Função de ativação sigmoid */
float sigmoid(float x) {
    return 1.0f / (1.0f + expf(-x));
}

// Função de predição do modelo (forward pass)
float model_predict(const float x[INPUT_SIZE]) {
    float hidden[HIDDEN_SIZE];

    // Camada oculta (Dense + ReLU)
    for (int i = 0; i < HIDDEN_SIZE; ++i) {
        hidden[i] = b1[i];
        for (int j = 0; j < INPUT_SIZE; ++j) {
            hidden[i] += x[j] * W1[j][i];
        }
        hidden[i] = relu(hidden[i]);
    }

    // Camada de saída (Dense + Sigmoid)
    float output = b2;
    for (int i = 0; i < HIDDEN_SIZE; ++i) {
        output += hidden[i] * W2[i];
    }
    output = sigmoid(output);

    return output;
}

/* Função para normalização das leituras dos sensores*/
void normalize_readings(float input[INPUT_SIZE]) { 
    input[0] /= MAX_TEMPERATURE_READING;  // Temperatura
    input[1] /= 100.0f;                   // Umidade
    input[2] /= MAX_GAS_READING;          // Gaa
    input[3] /= MAX_LIGHT_READING;       // Luz
}

/* Exemplo*/

int main() {
    // Substitua pelos valores reais dos sensores (normalizados conforme usado no treino)
    float readings[INPUT_SIZE] = {19.68, 63.17,51.77,585.95};
    normalize_readings(readings);
    float chance_vida = model_predict(readings);
    printf("Porcentagem de chance de vida: %.2f%%\n", chance_vida * 100.0f);
    return 0;
}