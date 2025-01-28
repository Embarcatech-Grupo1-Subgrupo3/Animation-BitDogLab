#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <stdlib.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "pico/binary_info.h"
#include "src/play_audio.h"
#include "src/matrix_led.h"
#include "src/letras.h"

// configuração do teclado
#define ROWS 4
#define COLUMNS 4

const unsigned int row_pins[ROWS] = {16, 17, 18, 19};     // pinos para linhas
const unsigned int column_pins[COLUMNS] = {20, 28, 8, 9}; // pinos para colunas

// mapeamento de teclas do teclado
const char key_map[ROWS][COLUMNS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// inicializa os pinos do teclado
void initialize_keyboard()
{
    for (int i = 0; i < ROWS; i++)
    {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_OUT);
        gpio_put(row_pins[i], 1); // estado inativo (alto)
    }

    for (int j = 0; j < COLUMNS; j++)
    {
        gpio_init(column_pins[j]);
        gpio_set_dir(column_pins[j], GPIO_IN);
        gpio_pull_up(column_pins[j]); // habilita pull-up
    }
}

// lê a tecla pressionada
char read_key()
{
    for (int row = 0; row < ROWS; row++)
    {
        gpio_put(row_pins[row], 0); // ativa a linha atual

        for (int col = 0; col < COLUMNS; col++)
        {
            if (!gpio_get(column_pins[col]))
            {                 // detecta tecla pressionada
                sleep_ms(50); // debounce simples
                while (!gpio_get(column_pins[col]))
                    ;
                gpio_put(row_pins[row], 1); // desativa a linha
                return key_map[row][col];   // retorna a tecla correspondente
            }
        }

        gpio_put(row_pins[row], 1); // desativa a linha
    }
    return '\0'; // nenhuma tecla pressionada
}

#define LED_COUNT 25
#define LED_PIN 7

void handle_key2(int state)
{
    npClear();
    switch (state)
    {
    case 0:
        updateMatrix(LETRA_L);
        break;
    case 1:
        updateMatrix(LETRA_E);
        break;
    case 2:
        updateMatrix(LETRA_O);
        break;
    case 3:
        updateMatrix(LETRA_N);
        break;
    case 4:
        updateMatrix(LETRA_A);
        break;
    case 5:
        updateMatrix(LETRA_R);
        break;
    case 6:
        updateMatrix(LETRA_D);
        break;
    case 7:
        updateMatrix(LETRA_O);
        break;
    }
}

int main()
{
    stdio_init_all();
    npInit(LED_PIN);
    npClear();
    setup_audio();
    initialize_keyboard();

    float brightnessScale = 0.05; // 5% de brilho

    applyBrightnessToMatrix(LETRA_L, brightnessScale);
    applyBrightnessToMatrix(LETRA_E, brightnessScale);
    applyBrightnessToMatrix(LETRA_O, brightnessScale);
    applyBrightnessToMatrix(LETRA_N, brightnessScale);
    applyBrightnessToMatrix(LETRA_A, brightnessScale);
    applyBrightnessToMatrix(LETRA_R, brightnessScale);
    applyBrightnessToMatrix(LETRA_D, brightnessScale);

    while (true)
    {
        char key = read_key(); // lê a tecla pressionada

        switch (key)
        {
        case 'A':
            npClear();
            npWrite();
            break;
        case 'B':
            // Ação para a tecla B
            break;
        case 'C':
            // Ação para a tecla C
            break;
        case 'D':
            // Ação para a tecla D
            break;
        case '1':
            // Ação para a tecla 1
            break;
        case '2':
            for (int state = 0; state <= 7; state++)
            {
                handle_key2(state);
                sleep_ms(1000);
            }
            npClear();
            npWrite();
            break;
        case '3':
            // Ação para a tecla 3
            break;
        case '4':
            // Ação para a tecla 4
            break;
        case '5':
            // Ação para a tecla 5
            break;
        case '6':
            // Ação para a tecla 6
            break;
        case '7':
            // Ação para a tecla 7
            break;
        case '8':
            // Ação para a tecla 8
            break;
        case '9':
            // Ação para a tecla 9
            break;
        default:
            // Ação padrão para teclas desconhecidas
            break;
        }
        sleep_ms(100);
    }

    // main_audio(); // Chame a função principal de áudio continuamente
    return 0;
}
