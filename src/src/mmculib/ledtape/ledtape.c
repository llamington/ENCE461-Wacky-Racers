#include "delay.h"
#include "pio.h"

// TPERIOD is manually tuned to generate the correct 800 kHz waveforms
// The sum of the delays + the time it takes to set the PIO pin is about 1.25 uS
// The high period is the most critical. This timing is tuned for the best compromise.
#define TPERIOD (0.32)

__attribute__((optimize(2)))
__always_inline__ static void
ledtape_write_byte(pio_t pin, uint8_t byte)
{
    int j;
    uint8_t bit;

    for (j = 0; j < 8; j++)
    {
        // MSB first
        bit = (byte & 0x80);

        pio_output_high(pin);
        DELAY_US(TPERIOD);
        if (!bit)
            pio_output_low(pin);
        DELAY_US(TPERIOD);
        if (bit)
            pio_output_low(pin);
        DELAY_US(TPERIOD);

        byte <<= 1;
    }
}

__attribute__((optimize(2))) void ledtape_write(pio_t pin, uint8_t *buffer, uint16_t size)
{
    int i;

    // Send reset code
    pio_config_set(pin, PIO_OUTPUT_LOW);
    DELAY_US(100);

    // The data order is R G B per LED
    for (i = 0; i < size; i++)
    {
        ledtape_write_byte(pin, buffer[i]);
    }
}
