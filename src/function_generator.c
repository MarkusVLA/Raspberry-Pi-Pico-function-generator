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
#define ADC_RANGE_VOLTS 1.6f
#define MAX_FREQUENCY 100000.0f
#define MIN_FREQUENCY 1.0f

const double PI = 3.14159265358979323846;

uint8_t sine_wave[SAMPLES_PER_CYCLE];

void generate_sine_wave() {
    for (uint i = 0; i < SAMPLES_PER_CYCLE; i++) {
        sine_wave[i] = (uint8_t)((sin(i * (2.0 * PI / SAMPLES_PER_CYCLE)) + 1.0) * 127.5);
    }
}

void set_dac_value(uint8_t value) {
    for (uint i = 0; i < DAC_RESOLUTION; i++) {
        gpio_put(R2R_PIN_START + i, (value >> i) & 1);
    }
}

float read_adc() {
    uint32_t adc_value = adc_read();
    float voltage = (float)adc_value / 4095.0f * 3.3f; // Convert to voltage (0-3.3V)
    float normalized_value = voltage / ADC_RANGE_VOLTS;
    if (normalized_value > 1.0f) {
        normalized_value = 1.0f;
    }
    return normalized_value;
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

    while (1) {
        uint8_t *waveform = sine_wave;
        float adc_value = read_adc();
        float frequency = MIN_FREQUENCY + (MAX_FREQUENCY - MIN_FREQUENCY) * adc_value;
        float delay_us = (1e6 * (1 / frequency)) / SAMPLES_PER_CYCLE;

        for (uint i = 0; i < SAMPLES_PER_CYCLE; i++) {
            set_dac_value(waveform[i]);
            busy_wait_us(delay_us);
        }
    }

    return 0;
}
