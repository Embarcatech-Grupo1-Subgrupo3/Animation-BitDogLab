#include "../include/keyboard.h"
#include <stdio.h>

void keyboard_init() {
    printf("Teclado inicializado.\n");
}

char keyboard_read() {
    printf("Tecla pressionada lida.\n");
    return 'C';
}
