#ifndef LEDS_H
#define LEDS_H

#define NUM_LEDS 10

void initialize_leds(void);
void set_color(int led_index, int red, int green, int blue);
void send_buffer(void);

#endif