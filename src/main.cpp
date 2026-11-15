#include "unet.h"

//declarations:
void core1_entry(void);



// 48-step PCM wave lookup table (32-bit signed range)
const int32_t pcm_wavetable_48[48] = {
        0,       522921,  1041334, 1550853, 2047118, 2525946, 2983363, 3415538,
  3818867, 4190014, 4525883, 4823677, 5080838, 5295159, 5464790, 5588307,
  5664654, 5693175, 5673562, 5605850, 5490422, 5327914, 5119293, 4865842,
  4569145, 4230985, 3853335, 3438347, 2988349, 2505828, 1993410, 1453857,
   889988,  305591, -288591, -889988, -1453857,-1993410,-2505828,-2988349,
 -3438347,-3853335,-4230985,-4569145,-4865842,-5119293,-5327914,-5490422
};
// 24-step PCM wave lookup table (2 kHz at 48 kHz sample rate)
const int32_t pcm_wavetable_32[32] = {
          0,  1111413,  2154434,  3042443,  3699119,  4080838,  4166268,  3953716,
    3454914,  2701934,  1743114,   639255,  -511929, -1632791, -2645850, -3481283,
   -4080838, -4401214, -4420850, -4128314, -3540838, -2687114, -1621413,  -462921,
     711929,  1832791,  2845850,  3681283,  4280838,  4601214,  4620850,  4328314
};
uint32_t table_indexL = 0;
uint32_t table_indexR = 0;

void setup() {

    set_sys_clock_khz(CLOCKSPEED, false);

    multicore_launch_core1(core1_entry);


}

void loop() {
    if (!i2s_ready) return;
    int32_t raw_sampleL = pcm_wavetable_48[table_indexL];
    int32_t raw_sampleR = pcm_wavetable_32[table_indexR];
    table_indexL = (table_indexL + 1) % 48;
    table_indexR = (table_indexR + 1) % 32;

    int32_t scaled_sampleL = (raw_sampleL * 15) / 100;
    int32_t scaled_sampleR = (raw_sampleR * 15) / 100;

    pio_sm_put_blocking(I2S_PIO_DOUT, 0, scaled_sampleL); // Send sample for the left channel
    pio_sm_put_blocking(I2S_PIO_DOUT, 0, 0); // Send 0 for the right channel

}