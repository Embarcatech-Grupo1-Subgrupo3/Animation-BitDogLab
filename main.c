#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "include/leds.h"
#include "include/keyboard.h"

//#include "key_a.h"
//#include "key_b.h"
include "include/key_c.h"
//#include "key_d.h"
//#include "key_0.h"
//#include "key_1.h"
//#include "key_2.h"
//#include "key_3.h"
//#include "key_4.h"
//#include "key_5.h"
//#include "key_6.h"
//#include "key_7.h"
//#include "key_8.h"
//#include "key_9.h"

#define I2C_PORT i2c0
#define SDA_PIN 14
#define SCL_PIN 15
#define OLED_ADDRESS 0x3C 

ssd1306_t oled;

void initialize_oled() {
    i2c_init(I2C_PORT, 100000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    oled = ssd1306_create(I2C_PORT, OLED_ADDRESS, 128, 64);
    ssd1306_clear(&oled);
    ssd1306_draw_string(&oled, 0, 0, "Sistema iniciado", 1, SSD1306_COLOR_WHITE);
    ssd1306_show(&oled);
}

void display_message(const char *message) {
    ssd1306_clear(&oled);
    ssd1306_draw_string(&oled, 0, 0, message, 1, SSD1306_COLOR_WHITE);
    ssd1306_show(&oled);
}

int main() {
    stdio_init_all();
    initialize_keyboard();
    initialize_leds();
    initialize_oled();

    printf("Sistema iniciado\n");
    display_message("Sistema iniciado");

    while (true) {
        char key = read_key();
        if (key) {
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "Tecla: %c", key);
            printf("%s\n", buffer);
            display_message(buffer);

            switch (key) {
                //case 'A': handle_key_a(); break;
                //case 'B': handle_key_b(); break;
                case 'C': handle_key_c(); break;
                //case 'D': handle_key_d(); break;
                //case '0': handle_key_0(); break;
                //case '1': handle_key_1(); break;
                //case '2': handle_key_2(); break;
                //case '3': handle_key_3(); break;
                //case '4': handle_key_4(); break;
                //case '5': handle_key_5(); break;
                //case '6': handle_key_6(); break;
                //case '7': handle_key_7(); break;
                //case '8': handle_key_8(); break;
                //case '9': handle_key_9(); break;
                default:
                    printf("Tecla %c não atribuida.\n", key);
                    display_message("Tecla nao atribuida");
                    break;
            }
        }
        sleep_ms(100);
    }

    return 0;
}
