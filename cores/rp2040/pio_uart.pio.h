// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------ //
// pio_tx //
// ------ //

#define pio_tx_wrap_target 0
#define pio_tx_wrap 5
#define pio_tx_pio_version 0

static const uint16_t pio_tx_program_instructions[] = {
    //     .wrap_target
    0xe029, //  0: set    x, 9
    0x98a0, //  1: pull   block           side 1
    0x6001, //  2: out    pins, 1
    0xa046, //  3: mov    y, isr
    0x0084, //  4: jmp    y--, 4
    0x0042, //  5: jmp    x--, 2
    //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program pio_tx_program = {
    .instructions = pio_tx_program_instructions,
    .length = 6,
    .origin = -1,
    .pio_version = 0,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x0
#endif
};

static inline pio_sm_config pio_tx_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + pio_tx_wrap_target, offset + pio_tx_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}

static inline void pio_tx_program_init(PIO pio, uint sm, uint offset, uint pin_tx) {
    // Tell PIO to initially drive output-high on the selected pin, then map PIO
    // onto that pin with the IO muxes.
    pio_sm_set_pins_with_mask(pio, sm, 1u << pin_tx, 1u << pin_tx);
    pio_sm_set_pindirs_with_mask(pio, sm, 1u << pin_tx, 1u << pin_tx);
    pio_gpio_init(pio, pin_tx);
    pio_sm_config c = pio_tx_program_get_default_config(offset);
    // OUT shifts to right, no autopull
    sm_config_set_out_shift(&c, true, false, 32);
    // We are mapping both OUT and side-set to the same pin, because sometimes
    // we need to assert user data onto the pin (with OUT) and sometimes
    // assert constant values (start/stop bit)
    sm_config_set_out_pins(&c, pin_tx, 1);
    sm_config_set_sideset_pins(&c, pin_tx);
    // We only need TX, so get an 8-deep FIFO!
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    pio_sm_init(pio, sm, offset, &c);
}

#endif

// ------ //
// pio_rx //
// ------ //

#define pio_rx_wrap_target 0
#define pio_rx_wrap 6
#define pio_rx_pio_version 0

static const uint16_t pio_rx_program_instructions[] = {
    //     .wrap_target
    0xe032, //  0: set    x, 18
    0x2020, //  1: wait   0 pin, 0
    0xa047, //  2: mov    y, osr
    0x0083, //  3: jmp    y--, 3
    0x4001, //  4: in     pins, 1
    0x0042, //  5: jmp    x--, 2
    0x8020, //  6: push   block
    //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program pio_rx_program = {
    .instructions = pio_rx_program_instructions,
    .length = 7,
    .origin = -1,
    .pio_version = 0,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x0
#endif
};

static inline pio_sm_config pio_rx_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + pio_rx_wrap_target, offset + pio_rx_wrap);
    return c;
}

static inline void pio_rx_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, false);
    pio_gpio_init(pio, pin);
    gpio_pull_up(pin);
    pio_sm_config c = pio_rx_program_get_default_config(offset);
    sm_config_set_in_pins(&c, pin); // for WAIT, IN
    sm_config_set_jmp_pin(&c, pin); // for JMP
    // Shift to right, autopull disabled
    sm_config_set_in_shift(&c, true, false, 32);
    pio_sm_init(pio, sm, offset, &c);
}

#endif

