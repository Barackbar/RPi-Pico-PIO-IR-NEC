/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: John David Sprunger
 * Date: 4/20/2021
 */
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
// Our assembled program:
#include "ir_nec.pio.h"

int main() {
  PIO pio = pio0;
  uint offset = pio_add_program(pio, &ir_nec_tx_program);
  uint sm = pio_claim_unused_sm(pio, true);

  // Set the data pin to pull down
  gpio_pull_down(22);

  // Set the clock divider so that it runs at 4*38222Hz
  float div = (float)clock_get_hz(clk_sys) / (float)(4 * 38000);

  ir_nec_tx_program_init(pio, sm, offset, 22, div);

  while (true) {
    // Just sending the Vizio volume up command every second
    ir_nec_send(pio, sm, 0xDF20BF40);
    sleep_ms(1000);
  }
}
