#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"
#include "hardware/gpio.h"

// número total de LEDs
#define NUM_LEDS 25
#define LED_MATRIX_PIN 7

// configuração do teclado
#define ROWS 4
#define COLUMNS 4
  
int cycles=0;   

const unsigned int row_pins[ROWS] = {16, 17, 18, 19};     // pinos para linhas
const unsigned int column_pins[COLUMNS] = {20, 28, 8, 9}; // pinos para colunas

// mapeamento de teclas do teclado
const char key_map[ROWS][COLUMNS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// estrutura do pixel
struct led_color_t
{
    uint8_t g, r, b;
};
typedef struct led_color_t led_t;
led_t led_matrix[NUM_LEDS];

// variáveis do PIO
PIO pio_instance;
uint state_machine;

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

// inicializa os LEDs
void initialize_leds(uint pin)
{
    uint offset = pio_add_program(pio0, &ws2818b_program);
    pio_instance = pio0;
    state_machine = pio_claim_unused_sm(pio_instance, true);
    ws2818b_program_init(pio_instance, state_machine, offset, pin, 800000.f);

    for (uint i = 0; i < NUM_LEDS; ++i)
    {
        led_matrix[i].r = led_matrix[i].g = led_matrix[i].b = 0;
    }
}

// ajusta a cor do LED
void set_color(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
    led_matrix[index].r = r;
    led_matrix[index].g = g;
    led_matrix[index].b = b;
}

// limpa todos os LEDs
void clear_all()
{
    for (uint i = 0; i < NUM_LEDS; ++i)
    {
        set_color(i, 0, 0, 0);
    }
}

// envia os dados dos LEDs
void send_buffer()
{
    for (uint i = 0; i < NUM_LEDS; ++i)
    {
        pio_sm_put_blocking(pio_instance, state_machine, led_matrix[i].g);
        pio_sm_put_blocking(pio_instance, state_machine, led_matrix[i].r);
        pio_sm_put_blocking(pio_instance, state_machine, led_matrix[i].b);
    }
    sleep_us(100);
}

// animação de LEDs
void simple_animation()
{
    int cycles = 3;
    while (cycles > 0)
    {
        for (int row = 0; row < 5; row++)
        {
            clear_all();
            for (int i = row * 5; i < (row + 1) * 5; i++)
            {
                set_color(i, 255, 0, 255); // define os LEDs da linha atual como lilás
            }
            send_buffer();
            sleep_ms(200); // pausa de 200ms entre as linhas
        }
        cycles--;
    }
    clear_all();
    send_buffer();
}

// esta animacao começa no centro dos leds
// subdivide-se em dois, e seguem seu caminho até as extremidades, afastando um do outro
void animation_key2()
{
    int cycles = 3; // numero de ciclos da animação
    while (cycles > 0)
    {
        int center = NUM_LEDS / 2; // calcula o índice do LED central

        // inicializa os LEDs em camadas, do centro para as bordas
        for (int layer = 0; layer <= NUM_LEDS / 2; layer++)
        {
            clear_all(); // apaga todos os LEDs

            // acende os LEDs da camada atual
            for (int i = 0; i < NUM_LEDS; i++)
            {
                // verifica se o LED está na camada atual
                if (abs(i - center) == layer)
                {
                    set_color(i, 255, 0, 0); // define a cor vermelha para os LEDs da camada
                    if(read_key('A')){
                        cycles=0;
                        clear_all(); // apaga todos os LEDs ao final de cada ciclo
                        }
                }
            }

            send_buffer();
            sleep_ms(200);
        }

        clear_all(); // apaga todos os LEDs ao final de cada ciclo
        send_buffer();
        sleep_ms(500); // pausa antes de repetir o ciclo

        cycles--;
  }
}


// esta animacao começa no centro dos leds
// se expande para as bordas
void animation_key5()
{
    int cycles = 3; // numero de ciclos da animação
    while (cycles > 0)
    {
        // inicializa os LEDs em camadas, do centro para as bordas
        for (int colum = 0; colum < 5 ; colum++)
        {
            clear_all(); // apaga todos os LEDs

            // percorre todos os leds
            for (int i = 0; i <=NUM_LEDS; i++)
            {
                // se o resto da divisão de i pelo numero da coluna + 2 fpr 0, o led acende
                if (i % (colum+2) == 0)
                {
                    set_color(i, 0, 0, 255); // define a cor verde para os LEDs ativos
                    
                }
                
            }
            send_buffer();
            sleep_ms(2000);
        }

        clear_all(); // apaga todos os LEDs ao final de cada ciclo
        send_buffer();
        sleep_ms(500); // pausa antes de repetir o ciclo

        cycles--;
    }
} 

// altera os LEDs baseado na tecla pressionada
void handle_key(char key)
{
    clear_all();
    switch (key)
    {
    case 'A':
        clear_all(); // apaga todos os LEDs ao final de cada ciclo
        send_buffer();
        cycles=0;
        break;
    case 'B':
        printf("ligar LEDs azuis\n");
        for (int i = 0; i < NUM_LEDS; i++)
        {
            set_color(i, 0, 0, 255);
        }
        break;
    case 'C':
        printf("ligar LEDs vermelhos\n");
        for (int i = 0; i < NUM_LEDS; i++)
        {
            set_color(i, 255, 0, 0);
        }
        break;
    case 'D':
        printf("ligar LEDs verdes\n");
        for (int i = 0; i < NUM_LEDS; i++)
        {
            set_color(i, 0, 255, 0);
        }
        break;
    case '1':
        printf("LEDs lilás em ímpares\n");
        for (int i = 0; i < NUM_LEDS; i++)
        {
            if (i % 2 != 0)
                set_color(i, 255, 0, 255);
        }
        break;
    case '2':
        printf("Animação: Afastamento\n");
        animation_key2();
        break;
    case '3':
        printf("animação lilás\n");
        simple_animation();
        break;
    case '5':
        printf("animação lilás\n");
        animation_key5();
        break;
    default:
        break;
    }
    send_buffer();
}

// programa principal
int main()
{
    stdio_init_all();
    initialize_keyboard();
    initialize_leds(LED_MATRIX_PIN);

    printf("sistema iniciado\n");
    while (true)
    {
        char key = read_key(); // lê a tecla pressionada
        if (key)
        {
            handle_key(key); // executa a ação correspondente
        }
        sleep_ms(100);
    }

    return 0;
}
