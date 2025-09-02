// adc_dma_diag.ino — Minimal DMA ADC diagnostic for RP2350B
// - No dynamic allocation, no String
// - Prints raw 12-bit ADC readings (0..4095) for GPIO 40..47 in round-robin

// Your header expects PWM_RESOLUTION; define a sane default if not provided.
#ifndef PWM_RESOLUTION
#define PWM_RESOLUTION 4096u
#endif

#include <Arduino.h>
#include "ADCless.h"   // <- put your header file name here

// Print every N milliseconds
static const uint32_t kPrintIntervalMs = 200;
static uint32_t s_lastPrintMs = 0;

static inline void print_line(const char* s) {
  Serial.println(s);
}

static void print_header_once() {
  print_line("");
  print_line("[ADC DMA DIAG]");
  print_line("Channels: 8 (GPIO 40..47)  |  Values: 0..4095  |  Round-robin DMA");
  print_line("--------------------------------------------------------------------");
  print_line("ch0\tch1\tch2\tch3\tch4\tch5\tch6\tch7");
}

void setup() {
  // Serial at a common speed; wait briefly for host to attach
  Serial.begin(115200);
  uint32_t start_ms = millis();
  while (!Serial && (millis() - start_ms) < 2000) { /* wait up to 2s */ }

  print_header_once();

  // Kick off your ADC+DMA engine
  configureADC_DMA();
}

void loop() {
  const uint32_t now = millis();
  if ((now - s_lastPrintMs) >= kPrintIntervalMs) {
    s_lastPrintMs = now;

    // Read the most recent round of samples.
    // Note: DMA is continuously overwriting adc_results_buf[].
    // Reading 16-bit values is atomic on this MCU, so this is fine for diagnostics.
    for (uint8_t i = 0; i < NUM_ADC_INPUTS; ++i) {
      uint16_t v = adc_results_buf[i];
      Serial.print(v);
      if (i < (NUM_ADC_INPUTS - 1)) Serial.print('\t');
    }
    Serial.print('\n');
  }
}
