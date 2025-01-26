#ifndef LEDS_H
#define LEDS_H

#define LED_PIN_1 10
#define LED_PIN_2 11
#define LED_PIN_3 12 

void led_init();
void led_on(int led_number);
void led_off(int led_number);
void led_toggle(int led_number);

#endif