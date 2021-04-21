/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
// Our assembled program:
// Our assembled program:
#include "ir_nec.pio.h"

int main() {
  PIO pio = pio0;
  uint offset = pio_add_program(pio, &ir_nec_program);
  uint sm = pio_claim_unused_sm(pio, true);

  // Set the data pin to pull down
  gpio_pull_down(22);

  // Set the clock divider so that it runs at 4*38222Hz
  float div = (float)clock_get_hz(clk_sys) / (float)(4 * 38222);
  // Calculate the 9ms header pulse
  uint32_t header_pulse_loops = 0.009*38222;
  // Calculate the 4.5ms header delay
  uint32_t header_delay_loops = 0.0045*38222;

  ir_nec_program_init(pio, sm, offset, 22, div);

  // The state machine is now running. Any value we push to its TX FIFO will
  // appear on the LED pin.
  while (true) {
    // Blink

    // Push in the header pulse length in cycles...
    pio_sm_put_blocking(pio, sm, header_pulse_loops);
    // And the header delay length in cycles...
    pio_sm_put_blocking(pio, sm, header_delay_loops);
    // And finally the data.
    // TODO: put an actual value that was gotten from an external source,
    //       like over an I2C channel or something
    // Vizio Vol up = 1101 1111 0010 0000 1011 1111 0100 0000 = DF20BF40
    // What it's sending = 0000 0010 1111 1101 0000 0100 1111 101?
    // It's backwards, lul, because out shift register is shifting right
    // Also forgot to add ending burst
    pio_sm_put_blocking(pio, sm, 0xDF20BF40);
    
    sleep_ms(1000);
  }
}
