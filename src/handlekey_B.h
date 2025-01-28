#include "src/matrix_led.h"
#include "pico/stdlib.h"

// tudo azul
int tecla_B[5][5][3] = { 
    {{0, 0, 238}, {0, 0, 238}, {0, 0, 238}, {0, 0, 238}, {0, 0, 238}},
    {{0, 0, 238}, {0, 0, 238}, {0, 0, 238}, {0, 0, 238}, {0, 0, 238}},
    {{0, 0, 238}, {0, 0, 238}, {0, 0, 238}, {0, 0, 238}, {0, 0, 238}},
    {{0, 0, 238}, {0, 0, 238}, {0, 0, 238}, {0, 0, 238}, {0, 0, 238}},
    {{0, 0, 238}, {0, 0, 238}, {0, 0, 238}, {0, 0, 238}, {0, 0, 238}}};

void handle_keyB()
{
    npClear();
    updateMatrix(tecla_B);
            sleep_ms(1000);
            npClear();
            npWrite();
}