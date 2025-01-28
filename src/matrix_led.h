#ifndef MATRIX_LED_H
#define MATRIX_LED_H

#include <stdint.h>

#define LED_COUNT 25
#define LED_PIN 7

// Definição de pixel GRB
typedef struct {
    uint8_t G, R, B;
} npLED_t;

// Declaração do buffer de pixels que formam a matriz.
extern npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
extern PIO np_pio;
extern uint sm;

// Funções de controle dos LEDs
void npInit(uint pin);
void npSetLED(uint index, uint8_t r, uint8_t g, uint8_t b);
void npClear(void);
void npWrite(void);
void applyBrightnessToMatrix(int matriz[5][5][3], float scale);
int getIndex(int x, int y);
void updateMatrix(int matriz[5][5][3]);

#endif // MATRIX_LED_H
