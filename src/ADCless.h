#pragma once

#include <Arduino.h>
#include <hardware/adc.h>
#include <hardware/dma.h>
#include <hardware/regs/dreq.h>
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

#define NUM_ADC_INPUTS      8
#define ADCLESS_RP2350B
//#define ADCLESS_RP2040

#ifdef ADCLESS_RP2350B
    #define BASE_ADC_PIN 40
#else
    #define BASE_ADC_PIN 26
#endif

// DECLARATIONS only (using extern keyword)
extern volatile uint16_t adc_results_buf[NUM_ADC_INPUTS];
extern volatile uint16_t* adc_results_ptr[1];
extern uint adc_samp_chan, adc_ctrl_chan;

// Function declaration only
void configureADC_DMA();