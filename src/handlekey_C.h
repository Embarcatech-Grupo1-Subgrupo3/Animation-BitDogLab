#include "src/matrix_led.h"
#include "pico/stdlib.h"

// tudo vermelho
int tecla_C[5][5][3] = {
    {{238, 0, 0}, {238, 0, 0}, {238, 0, 0}, {238, 0, 0}, {238, 0, 0}},
    {{238, 0, 0}, {238, 0, 0}, {238, 0, 0}, {238, 0, 0}, {238, 0, 0}},
    {{238, 0, 0}, {238, 0, 0}, {238, 0, 0}, {238, 0, 0}, {238, 0, 0}},
    {{238, 0, 0}, {238, 0, 0}, {238, 0, 0}, {238, 0, 0}, {238, 0, 0}},
    {{238, 0, 0}, {238, 0, 0}, {238, 0, 0}, {238, 0, 0}, {238, 0, 0}}};

void handle_keyC()
{
    npClear();
    updateMatrix(tecla_C);
}