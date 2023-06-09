#include <SD.h>

uint16_t read_uint16(File *file) {
  uint8_t temp[2];
  file->read(temp, 2);
  return (uint16_t)temp[0] << 8 | temp[1];
}


int read_block(File *file, uint8_t block_size, uint8_t* block_bin) {
  for (int i=0; i < block_size*block_size; i++) {
    block_bin[i] = file->read();
  }
  
  return 1;
}

int read_palette(File *file, uint16_t size, uint16_t* palette) {
  for (int i=0; i < size; i++) {
    palette[i] = read_uint16(file);
  }
  return 1;
}
