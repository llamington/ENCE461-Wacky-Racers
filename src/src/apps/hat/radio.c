#include "radio.h"
#include "pio.h"
#include "nrf24.h"
#include "common.h"
#include "panic.h"

#define RADIO_ADDRESS 0xCA3F50468FLL
#define RADIO_PAYLOAD_SIZE 2

/** Randomly generated channels to reduce conflicts */
#define CH0_NUM 23
#define CH1_NUM 46
#define CH2_NUM 63
#define CH3_NUM 125

static spi_cfg_t spi_cfg =
    {
        .channel = 0,
        .clock_speed_kHz = 1000,
        .cs = RADIO_CS_PIO,
        .mode = SPI_MODE_0,
        .cs_mode = SPI_CS_MODE_FRAME,
        .bits = 8};

static spi_t spi;

static nrf24_t *nrf;

static uint8_t channel;

/** Initialises radio module */
void radio_init(void)
{
    pio_config_set(CH0_PIO, PIO_PULLUP);
    pio_config_set(CH1_PIO, PIO_PULLUP);
    pio_config_set(CH2_PIO, PIO_PULLUP);
    pio_config_set(CH3_PIO, PIO_PULLUP);

    channel = radio_get_channel();

    nrf24_cfg_t nrf24_cfg =
        {
            .channel = channel,
            .address = RADIO_ADDRESS,
            .payload_size = RADIO_PAYLOAD_SIZE,
            .ce_pio = RADIO_CE_PIO,
            .irq_pio = RADIO_IRQ_PIO,
        };

    spi = spi_init(&spi_cfg);
    if (!spi)
        panic(LED_ERROR_PIO, ERROR_CODE_SPI);

    nrf = nrf24_init(spi, &nrf24_cfg);
    if (!nrf)
        panic(LED_ERROR_PIO, ERROR_CODE_RADIO);
}

/** Returns the currently selected channel, defaulting to CH0 if none selected */
uint8_t radio_get_channel(void)
{
    if (!pio_input_get(CH1_PIO))
        return CH1_NUM;
    if (!pio_input_get(CH2_PIO))
        return CH2_NUM;
    if (!pio_input_get(CH3_PIO))
        return CH3_NUM;

    return CH0_NUM;
}

void radio_poll_channel(void)
{
    uint8_t curr_channel = radio_get_channel();

    if (curr_channel != channel)
    {
        bool success = nrf24_set_channel(nrf, curr_channel);
        if (!success)
            panic(LED_ERROR_PIO, ERROR_CODE_CH_SET);
    }

    channel = curr_channel;
}

void radio_transmit_direction(int8_t throttle, int8_t heading)
{
    char buffer[] = {throttle, heading};
    uint8_t bytes_transmitted = nrf24_write(nrf, buffer, RADIO_PAYLOAD_SIZE);
}

void radio_transmit_dastardly(void)
{
    char buffer[] = {-128, -128};
    uint8_t bytes_transmitted = nrf24_write(nrf, buffer, RADIO_PAYLOAD_SIZE);
}

bool radio_read_bumper(void)
{
    char buffer[RADIO_PAYLOAD_SIZE];
    uint8_t bytes_read = nrf24_read(nrf, buffer, RADIO_PAYLOAD_SIZE);
    return bytes_read == 0 ? false : true;
}

void radio_shutdown(void)
{
    nrf24_power_down(nrf);
    spi_shutdown(spi);
}
