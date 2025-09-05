#pragma once

#define NUM_ADC_INPUTS      8

// Uncomment one depending on the target
#define ADCLESS_RP2350B
//#define ADCLESS_RP2040

#ifdef ADCLESS_RP2350B
    #define BASE_ADC_PIN 40
#else
    #define BASE_ADC_PIN 26
#endif

extern volatile uint16_t adc_results_buf[NUM_ADC_INPUTS];
extern volatile uint16_t* adc_results_ptr[1];
extern uint adc_samp_chan, adc_ctrl_chan;

void configureADC_DMA();