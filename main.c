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
#include "src/handlekey_1.h"
#include "src/handlekey_5.h"
#include "src/handlekey_A.h"
#include "src/handlekey_B.h"
#include "src/handlekey_C.h"
#include "src/handlekey_D.h"
#include "src/handlekey_#.h"
#include "src/handle_key2.h"
#include "src/handle_key3.h"
#include "src/handlekey_9.h"
#include "src/handlekey_6.h"
#include "src/handlekey_8.h"
#include "src/handlekey_7.h"

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

int main()
{
    stdio_init_all();
    npInit(LED_PIN);
    npClear();
    setup_audio();
    initialize_keyboard();

    float brightnessScale = 0.05;    // 5% de brilho
    float brightnessScale_20 = 0.2;  // 20% de brilho
    float brightnessScale_50 = 0.5;  // 50% de brilho
    float brightnessScale_80 = 0.8;  // 80% de brilho
    float brightnessScale_100 = 1.0; // 100% de brilho

    applyBrightnessToMatrix(LETRA_L, brightnessScale);
    applyBrightnessToMatrix(LETRA_E, brightnessScale);
    applyBrightnessToMatrix(LETRA_O, brightnessScale);
    applyBrightnessToMatrix(LETRA_N, brightnessScale);
    applyBrightnessToMatrix(LETRA_A, brightnessScale);
    applyBrightnessToMatrix(LETRA_R, brightnessScale);
    applyBrightnessToMatrix(LETRA_D, brightnessScale);
    applyBrightnessToMatrix(frame_1_1, brightnessScale);
    applyBrightnessToMatrix(frame_1_2, brightnessScale);
    applyBrightnessToMatrix(frame_1_3, brightnessScale);
    applyBrightnessToMatrix(frame_1_4, brightnessScale);
    applyBrightnessToMatrix(frame_1_5, brightnessScale);
    applyBrightnessToMatrix(tecla_B, brightnessScale_100);
    applyBrightnessToMatrix(tecla_C, brightnessScale_80);
    applyBrightnessToMatrix(tecla_D, brightnessScale_50);
    applyBrightnessToMatrix(tecla_, brightnessScale_20);

    while (true)
    {
        char key = read_key(); // lê a tecla pressionada

        switch (key)
        {
        case 'A':
            handle_keyA();
            break;
        case 'B':
            handle_keyB();
            break;
        case 'C':
            handle_keyC();
            break;
        case 'D':
            handle_keyD();
            break;
        case '#':
            handle_key();
            break;
        case '1':
            // Ação para a tecla 1
            for (int state = 0; state <= 4; state++)
            {
                handle_key1(state);
                sleep_ms(1000);
            }
            npClear();
            npWrite();
            break;
        case '2':
            // Ação para a tecla 2
            main_audio();
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
            for (int state = 0; state <= 6; state++)
            {
                handle_key3(state);
                sleep_ms(1000);
            }
            npClear();
            npWrite();
            break;
        case '4':
            // Ação para a tecla 4
            break;
        case '5':
            // Ação para a tecla 5
            for (int state = 0; state <= 4; state++)
            {
                handle_key5(state);
                sleep_ms(1000);
            }
            npClear();
            npWrite();
            break;
        case '6':
            for (int state = 0; state <= 4; state++)
            {
                handle_key6(state);
                sleep_ms(500);
            }
            npClear();
            npWrite();
            // Ação para a tecla 6
            break;
        case '7':
            // Ação para a tecla 7
            for (int state = 0; state <= 9; state++)
            {
                handle_key7(state);
                sleep_ms(500);
            }
            npClear();
            npWrite();
            break;
        case '8':
            // Ação para a tecla 8
            for (int state = 0; state <= 19; state++)
            {
                handle_key8(state);
                sleep_ms(1000);
            }
            npClear();
            npWrite();
            break;
        case '9':
            // Ação para a tecla 9
            for (int state = 0; state <= 12; state++)
            {
                handle_key9(state);
                sleep_ms(1000);
            }
            npClear();
            npWrite();
            // Ação para a tecla 9
            break;
        default:
            // Ação padrão para teclas desconhecidas
            break;
        }
        sleep_ms(100);
    }

    return 0;
}
