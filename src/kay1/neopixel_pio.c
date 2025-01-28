#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "build/ws2818b.pio.h"

#define LED_COUNT 25
#define LED_PIN 7

// Pinos para o teclado matricial
#define ROWS 4
#define COLS 4
uint row_pins[ROWS] = {16, 17, 18, 19}; // Pinos das linhas (R1-R4)
uint col_pins[COLS] = {20, 28, 8, 9};  // Pinos das colunas (C1-C4)

// Estrutura do pixel GRB
struct pixel_t {
  uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

npLED_t leds[LED_COUNT];
PIO np_pio;
uint sm;
volatile bool animation_enabled = false;

void npInit(uint pin) {
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true);
  }
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i) {
    npSetLED(i, 0, 0, 0);
  }
}

void npWrite() {
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100);
}

int getIndex(int x, int y) {
  if (y % 2 == 0) {
    return 24 - (y * 5 + x);
  } else {
    return 24 - (y * 5 + (4 - x));
  }
}

void keyboard_init() {
  for (int i = 0; i < ROWS; i++) {
    gpio_init(row_pins[i]);
    gpio_set_dir(row_pins[i], GPIO_OUT);
    gpio_put(row_pins[i], 1);
  }
  for (int j = 0; j < COLS; j++) {
    gpio_init(col_pins[j]);
    gpio_set_dir(col_pins[j], GPIO_IN);
    gpio_pull_up(col_pins[j]);
  }
}

char read_keypad() {
  for (int i = 0; i < ROWS; i++) {
    gpio_put(row_pins[i], 0);
    for (int j = 0; j < COLS; j++) {
      if (gpio_get(col_pins[j]) == 0) {
        gpio_put(row_pins[i], 1);
        sleep_ms(50); // Debounce
        return (i * 4 + j + 1); // Retorna o número da tecla
      }
    }
    gpio_put(row_pins[i], 1);
  }
  return 0; // Nenhuma tecla pressionada
}

void animate() {
  int matriz[5][5][3] = {
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0} },
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} }
   
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  };
    npWrite();
    sleep_ms(2000);
    npClear();
  int matriz2[5][5][3] = {
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 165, 0} },
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} }
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz2[coluna][linha][0], matriz2[coluna][linha][1], matriz2[coluna][linha][2]);
    }
   };
    npWrite();
    sleep_ms(2000);
    npClear();
  int matriz3[5][5][3] = {
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0} },
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0}}
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz3[coluna][linha][0], matriz3[coluna][linha][1], matriz3[coluna][linha][2]);
    }
   };
   npWrite();
   sleep_ms(2000);
   npClear();
  int matriz4[5][5][3] = {
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} },
    { {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0} },
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} }
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz4[coluna][linha][0], matriz4[coluna][linha][1], matriz4[coluna][linha][2]);
    }
   };
   npWrite();
   sleep_ms(2000);
   npClear();
  int matriz5[5][5][3] = {
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} },
    { {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0} },
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} }
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz5[coluna][linha][0], matriz5[coluna][linha][1], matriz5[coluna][linha][2]);
    }
   };
   npWrite();
   sleep_ms(2000);
   npClear();
   int matriz6[5][5][3] = {
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} },
    { {255, 165, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 165, 0} },
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} }
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz6[coluna][linha][0], matriz6[coluna][linha][1], matriz6[coluna][linha][2]);
    }
   };
   npWrite();
   sleep_ms(2000);
   npClear();
   int matriz7[5][5][3] = {
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} },
    { {255, 165, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 165, 0}, {255, 0, 0}, {255, 165, 0}, {255, 165, 0} },
    { {255, 165, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 165, 0} },
    { {0, 0, 0}, {255, 165, 0}, {255, 165, 0}, {255, 165, 0}, {0, 0, 0} }
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz7[coluna][linha][0], matriz7[coluna][linha][1], matriz7[coluna][linha][2]);
    }
   };
   npWrite();
   sleep_ms(2000);
   npClear();
  int matriz8[5][5][3] = {
    { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} } ,
    { {0, 0, 0}, {0, 0, 0}, {255, 255, 255}, {0, 0, 0}, {0, 0, 0} },
    { {0, 0, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {0, 0, 0} },
    { {0, 0, 0}, {0, 0, 0}, {255, 255, 255}, {0, 0, 0}, {0, 0, 0} },
    { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz8[coluna][linha][0], matriz8[coluna][linha][1], matriz8[coluna][linha][2]);
    }
   };
   npWrite();
   sleep_ms(350);
   npClear();
  int matriz9[5][5][3] = {   
    {{0, 0, 0}, {0, 0, 0}, {255, 255, 255}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {0, 0, 0}},
    {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
    {{0, 0, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 255, 255}, {0, 0, 0}, {0, 0, 0}}
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
    int posicao = getIndex(linha, coluna);
    npSetLED(posicao, matriz9[coluna][linha][0], matriz9[coluna][linha][1], matriz9[coluna][linha][2]);
    }
   };
   npWrite();
   sleep_ms(350);
   npClear();
  int matriz10[5][5][3] = {   
    {{0, 0, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {0, 0, 0}},
    {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
    {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
    {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
    {{0, 0, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {0, 0, 0}}
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz10[coluna][linha][0], matriz10[coluna][linha][1], matriz10[coluna][linha][2]);
    }
   };
   npWrite();
   sleep_ms(350);
   npClear();
  int matriz11[5][5][3] = {   
    {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
    {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
    {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
    {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
    {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}}
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz11[coluna][linha][0], matriz11[coluna][linha][1], matriz11[coluna][linha][2]);
    }
   };
   npWrite();
   sleep_ms(350);
   npClear();
  int matriz12[5][5][3] = {       
    {{0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}},
    {{0, 255, 0}, {255, 22, 255}, {0, 255, 0}, {255, 22, 255}, {0, 255, 0}},
    {{0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}}
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz12[coluna][linha][0], matriz12[coluna][linha][1], matriz12[coluna][linha][2]);
    }
   };
   npWrite();
   sleep_ms(1500);
   npClear();
  int matriz13[5][5][3] = {       
    {{0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}},
    {{0, 255, 0}, {255, 0, 0}, {0, 255, 0}, {255, 0, 0}, {0, 255, 0}},
    {{0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}}
    };
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz13[coluna][linha][0], matriz13[coluna][linha][1], matriz13[coluna][linha][2]);
    }
  };
   npWrite();
   sleep_ms(5000);
   npClear();
}

int main() {
  stdio_init_all();
  npInit(LED_PIN);
  npClear();
  keyboard_init();

  while (true) {
    char key = read_keypad();
    if (key == 1) { // Tecla "1"
      animation_enabled = !animation_enabled;
      while (read_keypad() == 1); // Aguarda a tecla ser liberada
      sleep_ms(300); // Debounce adicional
    }

    if (animation_enabled) {
      animate();
    } else {
      npClear();
      npWrite();
    }
  }
}
