/* 
* Modelo treinado para prever porcentagem de chance de vida em um planeta com base em leituras de sensores.
* O modelo é uma rede neural simples com uma camada oculta e uma camada de saída. A camada oculta usa a
* função de ativação ReLU e a camada de saída usa a função de ativação sigmoid.

* A rede foi treinada utilizando o Google Colab com os seguintes parâmetros:
* - Otimizador: Adam
* - Função de perda: Mean Squared Error (MSE)
* - Metricas: Mean Absolute Error (MAE)
* - Épocas: 500
* - Batch size: 32
*
* As leituras dos sensores (entradas) são normalizadas para garantir que os valores estejam na mesma escala.
*/


#ifndef IA_MODEL_H
#define IA_MODEL_H

#include <stdio.h>
#include <math.h>


#define INPUT_SIZE 4    // Número de entradas (sensores)
#define HIDDEN_SIZE 8   // Número de neurônios na camada oculta
#define OUTPUT_SIZE 1   // Número de saídas (chance de vida)

/* Valores para normalização das leituras*/
#define MAX_GAS_READING 217.79f
#define MAX_TEMPERATURE_READING 50.0f
#define MAX_LIGHT_READING 1086.46


/* Pesos e bias do modelo treinado*/

// Pesos e bias da camada oculta (Dense + ReLU)
// W1: Matriz de pesos da camada oculta
// b1: Vetor de bias da camada oculta
const float W1[INPUT_SIZE][HIDDEN_SIZE] = {
    {0.76127756, -1.14999, -1.3243964, 1.0532789, -0.5648892, -0.35137573, -0.02234721, -0.97573954},
    {0.0390515, 0.19636367, 0.04113916, -0.27672333, 1.0549195, 0.55996454, -0.11504948, 1.0107998},
    {0.0651774, 0.07326719, -0.0230303, -0.08702946, -0.38678792, -0.68653685, 0.03985898, 0.3901636},
    {-1.0542206, 0.10372138, -0.2699437, 0.5364545, -0.02500459, -0.50767237, 0.790702, -0.15410507}
};

const float b1[HIDDEN_SIZE] = { 0.23017338, 0.32451043, 0.7052842, -0.37595168, 0.09138247, 0.06862238, -0.2987081, 0.32811505 };

// Pesos e bias da camada de saída (Dense + Sigmoid)
// W2: Vetor de pesos da camada de saída
// b2: Escalar de bias da camada de saída
const float W2[HIDDEN_SIZE] = { -2.3351076, -1.8962342, -4.0220113, -0.7348212, 1.2856134, 1.095262, -1.9509647, 1.2919254};

const float b2 = 0.2667996;

/* Prototipo de funções*/
float relu(float x);
float sigmoid(float x);
float model_predict(const float x[INPUT_SIZE]);
void normalize_readings(float input[INPUT_SIZE]);

#endif // IA_MODEL_H