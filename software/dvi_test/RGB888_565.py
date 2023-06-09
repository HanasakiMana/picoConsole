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



assert len(palette_bin) <= 4096

with open('reisa.pic', 'wb') as f:
    f.write(img_width.to_bytes(2))
    f.write(img_height.to_bytes(2))

    f.write(int(len(palette_bin)/2 - 1).to_bytes(1)) # length of palette
    f.write(palette_bin)
    
    f.write(block_size.to_bytes(1))
    f.write(total_frames.to_bytes(1))

    blocks_of_x = int(img_height/block_size)
    blocks_of_y = int(img_width/block_size)
    assert blocks_of_x * blocks_of_y == len(bitmap_bin)

    f.write(blocks_of_x.to_bytes(1)) # number of blocks
    f.write(blocks_of_y.to_bytes(1))
    
    for block in bitmap_bin:
        f.write(block)