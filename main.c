#include <stdio.h>
#include "pico/stdlib.h"
#include "include/leds.h"
#include "include/keyboard.h"
#include "include/key_c.h"

int main()
{
    stdio_init_all();
    initialize_leds();
    keyboard_init();

    printf("Sistema iniciado\n");

    while (true)
    {
        char key = keyboard_read();
        if (key)
        {
            switch (key)
            {
            case 'C':
                handle_key_c();
                break;
            default:
                printf("Tecla %c não atribuída.\n", key);
                break;
            }
        }
        sleep_ms(100);
    }

    return 0;
}
