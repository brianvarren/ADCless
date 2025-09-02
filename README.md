# ADCless Library

A high-performance, DMA-based ADC library for Raspberry Pi Pico and RP2350B microcontrollers using arduino-pico. This library provides continuous, round-robin sampling of multiple ADC channels with zero CPU overhead during acquisition.

## Features

- **Zero CPU overhead** - Uses DMA for continuous ADC sampling
- **Round-robin sampling** - Automatically cycles through all configured ADC channels
- **High-speed acquisition** - Runs ADC at maximum speed with configurable clock divider
- **Dual platform support** - Works on both RP2040 and RP2350B
- **Simple API** - Easy integration into existing Arduino projects
- **Memory aligned buffers** - Optimized for DMA performance

## Hardware Support

| Platform | ADC Pins | Base Pin |
|----------|----------|----------|
| RP2040   | GPIO 26-29 (4 channels max) | 26 |
| RP2350B  | GPIO 40-47 (8 channels) | 40 |

## Installation

1. Copy `ADCless.h` and `ADCless.cpp` to your Arduino sketch folder
2. Include the library in your main sketch: `#include "ADCless.h"`

## Basic Usage

```cpp
#include <Arduino.h>
#include "ADCless.h"

void setup() {
    Serial.begin(115200);
    
    // Initialize ADC with DMA
    configureADC_DMA();
}

void loop() {
    // Read raw ADC values (0-4095)
    for (int i = 0; i < NUM_ADC_INPUTS; i++) {
        uint16_t raw_value = adc_results_buf[i];
        Serial.print("Channel ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(raw_value);
    }
    
    delay(100);
}
```

## Advanced Usage with Helper Functions

For normalized floating-point values, you can create helper functions:

```cpp
// In your main sketch or separate files
float getADCValue(int channel) {
    return (float)adc_results_buf[channel] / 4095.0f;
}

void loop() {
    // Get normalized values (0.0 - 1.0)
    for (int i = 0; i < NUM_ADC_INPUTS; i++) {
        float voltage_ratio = getADCValue(i);
        Serial.print("Channel ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(voltage_ratio, 3);
    }
    delay(100);
}
```

## Configuration

### Platform Selection
Uncomment the appropriate platform define in `ADCless.h`:

```cpp
#define ADCLESS_RP2350B    // For RP2350B (8 channels, GPIO 40-47)
//#define ADCLESS_RP2040   // For RP2040 (4 channels, GPIO 26-29)
```

### Number of Channels
Modify `NUM_ADC_INPUTS` in `ADCless.h`:

```cpp
#define NUM_ADC_INPUTS 8  // Set to desired number of channels
```

**Note**: Ensure your hardware supports the number of channels you configure.

## API Reference

### Functions

#### `void configureADC_DMA()`
Initializes the ADC and DMA system. Call once in `setup()`.

- Configures GPIO pins for ADC input
- Sets up round-robin sampling
- Initializes DMA channels for continuous operation
- Starts ADC in free-running mode

### Variables

#### `volatile uint16_t adc_results_buf[NUM_ADC_INPUTS]`
Buffer containing the most recent ADC readings for each channel.

- **Type**: Array of 16-bit unsigned integers
- **Range**: 0-4095 (12-bit ADC resolution)
- **Access**: Read-only from user code
- **Thread Safety**: Atomic reads on RP2040/RP2350B

#### `volatile uint16_t* adc_results_ptr[1]`
Internal pointer used by DMA system. **Do not modify.**

#### `uint adc_samp_chan, adc_ctrl_chan`
DMA channel identifiers used internally. **Do not modify.**

## How It Works

The library uses a dual-DMA channel approach:

1. **Sample Channel** (`adc_samp_chan`):
   - Triggered by ADC DREQ (Data Request)
   - Transfers ADC FIFO data to result buffer
   - Chains to control channel when complete

2. **Control Channel** (`adc_ctrl_chan`):
   - Reconfigures and retriggers the sample channel
   - Enables continuous, circular buffer operation

This creates a completely autonomous ADC acquisition system that requires no CPU intervention after initialization.

## Performance Characteristics

- **Sampling Rate**: Limited by ADC clock divider (default: maximum speed)
- **Latency**: New readings available within microseconds
- **CPU Usage**: Zero during acquisition
- **Memory Usage**: `NUM_ADC_INPUTS * 2` bytes for result buffer
