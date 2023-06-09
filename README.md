# picoConsole
A single-board "game console" with DVI output powered by RP2040.

## pin defination
* SD Card (SPI0)
  * GPIO0 Card Detect
  * GPIO1 Chip Select/CSn
  * GPIO2 CLK/SCLK
  * GPIO3 DI/MOSI/TX
  * GPIO4 DO/MISO/RX

* GPIO5-10 Reserved

* SPI SRAM (SPI1)
  * GPIO12 SO/MISO/RX
  * GPIO13 SI/MOSI/TX
  * GPIO14 SCLK
  * GPIO15 Chip Select/CSn

* Debug LEDs
  * GPIO16 LED_2
  * GPIO17 LED_1
  * GPIO18 LED_0
  * GPIO19 LED_RED

* HDMI EDID (I2C0)
  * GPIO20 SDA
  * GPIO21 SCL

* DVI Output
  * GPIO22 TMDS CLK M
  * GPIO23 TMDS CLK P
  * GPIO24 TMDS D0 M
  * GPIO25 TMDS D0 P
  * GPIO26 TMDS D1 M
  * GPIO27 TMDS D1 P
  * GPIO28 TMDS D2 M
  * GPIO29 TMDS D2 P
