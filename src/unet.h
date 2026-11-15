#include <Arduino.h>
#include <I2S.h>

#define CLOCKSPEED  196500

// I2S clocks output use pio0 block, two pins each for BCLK and LRCLK
#define I2S_PIO_CLKS pio0
#define I2S_CLKS_PIN 20 // 2 pins starting point (BCLK first)

// I2S data outputs use pio1 block, one pin each for DATA(DOUT)
#define I2S_PIO_DOUT pio1
#define I2S1_PIN 15
#define I2S2_PIN 16
#define I2S3_PIN 17
#define I2S4_PIN 18


#define AUDIV 8                     // Audio divider for pio timing 

extern void core1_entry(void);//main process for core 1, defined in "core1.c"
extern volatile bool i2s_ready;