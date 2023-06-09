#include <PicoDVI.h>
#include <SPI.h>

#define LED_0 16
#define LED_1 17
#define LED_2 18
#define LED_RED 19

#define SD_CS 1
#define SD_MISO 4
#define SD_MOSI 3
#define SD_SCK 2
#define SD_CD 0

#define SRAM_CS 13
#define SRAM_MISO 12
#define SRAM_MOSI 15
#define SRAM_SCK 14



static const struct dvi_serialiser_cfg pico_console_cfg = {
	.pio = pio0,
	.sm_tmds = {0, 1, 2},
	.pins_tmds = {24, 26, 28},
	.pins_clk = 22,
	.invert_diffpairs = true
};