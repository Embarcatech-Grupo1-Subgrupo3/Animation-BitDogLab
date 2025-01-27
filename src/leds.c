#include "../include/leds.h"
#include <stdio.h>

#define NUM_LEDS 10

void initialize_leds(void) {
    printf("LEDs inicializados.\n");
}

void set_color(int led_index, int red, int green, int blue) {
    if (led_index < 0 || led_index >= NUM_LEDS) {
        printf("Indice do LED fora do intervalo: %d\n", led_index);
        return;
    }
    printf("LED %d configurado para R:%d G:%d B:%d\n", led_index, red, green, blue);
}

void send_buffer(void) {
    printf("Buffer de LEDs enviado.\n");
}
