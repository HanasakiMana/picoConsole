from PIL import Image, ImagePalette

img_width: int = 0
img_height: int = 0
total_frames: int = 1
block_size = 16
palette_bin = b''
bitmap_bin = []
with Image.open('reisa.png') as img:
    img_width, img_height = img.size
    img = img.convert('P')
    pal = img.getpalette()

    for i in range(len(pal) // 3):
        r = pal[i * 3 + 0] >> 3
        g = pal[i * 3 + 1] >> 2
        b = pal[i * 3 + 2] >> 3
        rgb = (r << 11) | (g << 5) | b;
        palette_bin += rgb.to_bytes(2)

    
    for y in range(int(img.size[1]/block_size)):
        for x in range(int(img.size[0]/block_size)):
            block = b''
            for i in range(16):
                for j in range(16):
                    block += img.getpixel((x * block_size + j, y * block_size + i)).to_bytes(1)
            bitmap_bin.append(block)
    print(len(bitmap_bin))


assert len(palette_bin) <= 4096

with open('reisa_bitmap.h', 'w') as f:
    f.write("const uint8_t reisa_bitmap[] = {")
    for block in bitmap_bin:
        len_block = 0
        for pixel in block:
            f.write("0x%02X, "%(int(pixel)))
            len_block += 1
    f.write("};")