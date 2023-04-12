#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define DAC_RESOLUTION 8
#define R2R_PIN_START 0
#define ADC_PIN 26
#define SAMPLES_PER_CYCLE 128
#define MAX_FREQUENCY 20000.0
#define MIN_FREQUENCY 100.0

const uint MAX_VALUE = pow(2,8) - 1;

const double PI = 3.14159265358979323846;

uint8_t sine_wave[SAMPLES_PER_CYCLE];
uint8_t ramp[SAMPLES_PER_CYCLE];

uint8_t custom_function[SAMPLES_PER_CYCLE];

void generate_sine_wave() {
    for (uint i = 0; i < SAMPLES_PER_CYCLE; i++) {
        sine_wave[i] = (uint8_t)((sin(i * (2.0 * PI / SAMPLES_PER_CYCLE)) + 1.0) * 127.5);
    }
}

void generate_ramp(){
    for (int i = 0; i < SAMPLES_PER_CYCLE; i++){
        ramp[i] = (uint8_t) MAX_VALUE/SAMPLES_PER_CYCLE * i;
    }
}

void mix_functions(uint8_t * array_1, uint8_t * array_2){
    //Get array length
    for (int i = 0; i < SAMPLES_PER_CYCLE; i++){
        custom_function[i] = array_1[i] / 2 + array_2[i] / 2;
    }
}

void set_dac_value(uint8_t value) {
    for (int bit = 0; bit < DAC_RESOLUTION; bit++) {
        if ((value >> bit) & 0x01) {
            sio_hw->gpio_set = (1 << bit);
        } else {
            sio_hw->gpio_clr = (1 << bit);
        }
    }
}

float read_adc() {
    uint32_t adc_value = adc_read();
    return (float)adc_value / 4095.0f;
}

int main() {
    stdio_init_all();

    // Configure the GPIO pins as outputs
    for (uint i = 0; i < DAC_RESOLUTION; i++) {
        gpio_init(R2R_PIN_START + i);
        gpio_set_dir(R2R_PIN_START + i, GPIO_OUT);
    }

    // Initialize and configure the ADC
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);

    generate_sine_wave();
    generate_ramp();
    uint8_t *waveform = sine_wave;

    uint i;

    while (1) {

        float adc_value = read_adc();  // [0 , 1]
        float frequency = (uint16_t)MIN_FREQUENCY + (MAX_FREQUENCY - MIN_FREQUENCY) * adc_value;
        float delay_us = (1e6 * (1 / frequency)) / SAMPLES_PER_CYCLE;

        for (i = 0; i < SAMPLES_PER_CYCLE; i++) {
            set_dac_value(waveform[i]);
            busy_wait_us(delay_us);
        }
    }

    return 0;
}

