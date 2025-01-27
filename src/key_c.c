#include "../include/key_c.h"
#include "../include/leds.h"
#include <stdio.h>

#define RED_INTENSITY 204 // 80% da luminosidade máxima (255 * 0.8)

void handle_key_c(void) {
    printf("Tecla C pressionada: Acendendo LEDs em vermelho (80%% intensidade)\n");

    for (int i = 0; i < NUM_LEDS; i++) {
        set_color(i, RED_INTENSITY, 0, 0);
    }

    send_buffer();
}

void handle_key_star(void) {
    printf("Tecla * pressionada: Retornando ao main.c\n");
}
