#include <PicoDVI.h>
#include <SPI.h>
#include <SD.h>

#include "iomap.h"
#include "dataProcess.h"


//#include "sprites.h"
//#include "reisa_bitmap.h"



// DVI output with both front and back framebuffer
//DVIGFX8 display(DVI_RES_320x240p60, true, pico_console_cfg);
DVIGFX8 display(DVI_RES_320x240p60, false, pico_console_cfg);

#define SCK_MHZ(mhz) (1000000UL*(mhz))

uint16_t img_width;
uint16_t img_height;

uint16_t palette_size;

uint8_t block_size;
uint8_t total_frames;
uint8_t blocks_num_x;
uint8_t blocks_num_y;

uint32_t pos = 0;

// commands for LY68L6400 SPI SRAM
#define RD  0x03   // read (maxFreq@33MHz)
#define FRD  0x0B  // fast read (maxFreq@144MHz)
#define WR  0x02   // write
#define RSE  0x66  // reset enable
#define RS  0x99   // reset
#define LEN  0xC0  // toggle the mode how sram wrap data(Linear Burst or Wrap 32)
#define RDID  0x9F // read EID of sram

uint8_t transferBuffer[256];


void setup() {
  // initialize LEDs
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // initialize display output
  // show red if unable initialize framebuffer
  if (!display.begin()) {
    digitalWrite(LED_RED, HIGH);
  }
  
  // initialize SPI0 and SD card
  pinMode(SD_CD, INPUT);
  if (digitalRead(SD_CD) == HIGH) {
    digitalWrite(LED_RED, HIGH);
  } else {
    digitalWrite(LED_1, HIGH);
    SPI.setTX(SD_MOSI);
    SPI.setRX(SD_MISO);
    SPI.setSCK(SD_SCK);
    SD.begin(SD_CS, SCK_MHZ(50), SPI);
  }

  // initialize SPI1 for SPI SRAM
  pinMode(SRAM_CS, OUTPUT);
  digitalWrite(SRAM_CS, HIGH);
  SPI1.setCS(SRAM_CS);
  SPI1.setTX(SRAM_MOSI);
  SPI1.setRX(SRAM_MISO);
  SPI1.setSCK(SRAM_SCK);

  SPI1.begin();
  SPI1.beginTransaction(SPISettings(SCK_MHZ(100), MSBFIRST, SPI_MODE0));

  // initialize SPI SRAM
  readID(SPI1);
  //toggleBurst(SPI1); // change data burst type to Wrap 32 for higher clock



  // read picture from sd card
  File file_bin = SD.open("bg.pic", FILE_READ);


  img_width = read_uint16(&file_bin);
  img_height = read_uint16(&file_bin);

  palette_size = file_bin.read() + 1;

  uint16_t palette[palette_size];
  read_palette(&file_bin, palette_size, palette);
  memcpy(display.getPalette(), palette, sizeof palette);
  //display.swap(false, true);
  display.fillScreen(0);

  block_size = file_bin.read();
  total_frames = file_bin.read();
  blocks_num_x = file_bin.read();
  blocks_num_y = file_bin.read();
  pos = file_bin.position();

  // copy blocks from SD card to SRAM
  uint32_t sram_pos = 0; // skip the first page
  digitalWrite(LED_RED, HIGH);
  for (int frame=0; frame < total_frames; frame++) {
    for (int y=0; y < 15; y++) {
      for (int x=0; x < 20; x++) {
        read_block(&file_bin, block_size, transferBuffer);
        sram_write(SPI1, sram_pos);
        sram_pos += 256;
      }
    }
  }
  digitalWrite(LED_RED, LOW);



  while (1) {
    int swap_flag = 0;
    sram_pos = 0;
    for (int frame=0; frame < total_frames; frame++) {
      for (int y=0; y < 15; y++) {
        for (int x=0; x < 20; x++) {
          sram_fast_read(SPI1, sram_pos);
          sram_pos += 256;
          display.drawGrayscaleBitmap(x*block_size, y*block_size, transferBuffer, block_size, block_size);
        }
      }
      //display.swap();
      digitalWrite(LED_2, swap_flag);
      swap_flag = swap_flag^0x01;
    }
    file_bin.seek(pos);
  }
}

void readID(SPIClassRP2040 SPI) {
  digitalWrite(SRAM_CS, LOW);
  SPI.transfer(RDID);
  SPI.transfer(0); // waiting 24 cycles (part 12.4 in datasheet)
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(transferBuffer, 9);
  if (transferBuffer[0] != 0x0D) {
    digitalWrite(LED_RED, HIGH);
  }
  if (transferBuffer[1] != 0x5D) {
    digitalWrite(LED_0, HIGH);
  }
  digitalWrite(SRAM_CS, HIGH);
}

void toggleBurst(SPIClassRP2040 SPI) {
  digitalWrite(SRAM_CS, LOW);
  SPI.transfer(LEN);
  digitalWrite(SRAM_CS, HIGH);
}

void sram_write(SPIClassRP2040 SPI, uint32_t address) {
  digitalWrite(SRAM_CS, LOW);

  SPI.transfer(WR);
  // split address in 3 parts
  SPI.transfer((address >> 16) & 0xFF);
  SPI.transfer((address >> 8) & 0xFF);
  SPI.transfer(address & 0xFF);
  for (int i=0; i < sizeof transferBuffer; i++) {
    SPI.transfer(transferBuffer[i]);
  }
  
  
  digitalWrite(SRAM_CS, HIGH);


}

void sram_fast_read(SPIClassRP2040 SPI, uint32_t address) {
  digitalWrite(SRAM_CS, LOW);

  SPI.transfer(FRD);
  // split address in 3 parts
  SPI.transfer((address >> 16) & 0xFF);
  SPI.transfer((address >> 8) & 0xFF);
  SPI.transfer(address & 0xFF);
  // wait 8 cycles of clock
  SPI.transfer(0);


  SPI.transfer(transferBuffer, 256);

  digitalWrite(SRAM_CS, HIGH);
}

void loop() {

}

