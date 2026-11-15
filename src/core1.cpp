#include "unet.pio.h"
#include "unet.h"

volatile bool i2s_ready = false;

void I2S_DOUT_pio_init(PIO pio, uint sm, uint pin, uint offset) {
    pio_sm_config c = i2s_dout_program_get_default_config(offset);
    pio_gpio_init(pio, pin);
    pio_gpio_init(pio, 25);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    sm_config_set_clkdiv_int_frac(&c, AUDIV, 0);
    sm_config_set_out_pins(&c, pin, 1);
    sm_config_set_in_pins(&c, 25); // Tell data SM where to look for the sync signal
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    sm_config_set_out_shift(&c, false, false, 32);
    pio_sm_init(pio, sm, offset, &c);
}

void I2S_CLK_pio_init(PIO pio, uint sm, uint pin, uint offset) {
    pio_sm_config c = i2s_clk_program_get_default_config(offset);
    pio_gpio_init(pio, pin);
    pio_gpio_init(pio, pin+1);
    pio_gpio_init(pio, pin-15); //sync pin for i2s_dout program
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 2, true);
    pio_sm_set_consecutive_pindirs(pio, sm, pin-15, 1, true);
    sm_config_set_clkdiv_int_frac(&c, AUDIV, 0);
    sm_config_set_set_pins(&c, pin-15, 1);
    sm_config_set_sideset_pins(&c, pin);
    pio_sm_init(pio, sm, offset, &c);
}

void core1_entry(void){



    uint i2s_dout_program_offset{static_cast<uint>(pio_add_program(I2S_PIO_DOUT, &i2s_dout_program))};
    uint i2s_clk_program_offset{static_cast<uint>(pio_add_program(I2S_PIO_CLKS, &i2s_clk_program))};

    I2S_DOUT_pio_init(I2S_PIO_DOUT, 0, I2S1_PIN, i2s_dout_program_offset); 
    //I2S_DOUT_pio_init(I2S_PIO_DOUT, 1, I2S2_PIN, i2s_dout_program_offset); 
    //I2S_DOUT_pio_init(I2S_PIO_DOUT, 2, I2S3_PIN, i2s_dout_program_offset); 
    //I2S_DOUT_pio_init(I2S_PIO_DOUT, 3, I2S4_PIN, i2s_dout_program_offset);

    pio_sm_put_blocking(I2S_PIO_DOUT, 0, 0);
    pio_sm_put_blocking(I2S_PIO_DOUT, 0, 0);


    sleep_ms(200);


    //================================================================
   // TO-DO: Process first samples before turning on I2S state machines
    //================================================================

    pio_enable_sm_mask_in_sync(I2S_PIO_DOUT, 0x1);

    uint i2s_clk_sm = pio_claim_unused_sm(I2S_PIO_CLKS, false);
    I2S_CLK_pio_init(I2S_PIO_CLKS, i2s_clk_sm, I2S_CLKS_PIN, i2s_clk_program_offset);
    pio_set_sm_mask_enabled(I2S_PIO_CLKS, 1u << i2s_clk_sm, true);

    i2s_ready = true;
    while(true){
    }
}

