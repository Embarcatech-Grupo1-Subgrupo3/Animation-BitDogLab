#include <stdio.h>
#include "pico/stdlib.h"

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
void keyboard_init()
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
char keyboard_read()
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