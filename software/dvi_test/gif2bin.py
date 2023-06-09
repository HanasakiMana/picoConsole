from PIL import Image, ImageSequence

img_width: int = 0
img_height: int = 0
total_frames: int = 0
block_size = 16
palette_list = []
palette_bin = b''
bitmap_bin = []

with Image.open('bg_cropped.gif') as img_f:
    img_width, img_height = img_f.size

    
    for img in ImageSequence.Iterator(img_f):
        img = img.convert("RGB")
        total_frames += 1
        print(f"frame: {total_frames}")


        assert img_height % block_size == 0
        assert img_width % block_size == 0
        for y in range(int(img.size[1]/block_size)):
            for x in range(int(img.size[0]/block_size)):
                block = b''
                for i in range(block_size):
                    for j in range(block_size):
                        pixel = img.getpixel((x * block_size + j, y * block_size + i))
                        if pixel not in palette_list:
                            palette_list.append(pixel)
                        # print(palette_list)
                        flag = 0
                        for k in range(len(palette_list)):
                            if pixel == palette_list[k]:
                                block += k.to_bytes(1)
                                flag = 1
                        if flag == 0:
                            print(pixel, x * block_size + j, y * block_size + i)
                assert len(block) == block_size * block_size
                bitmap_bin.append(block)

     


for i in range(len(palette_list)):
        r = palette_list[i][0] >> 3
        g = palette_list[i][1] >> 2
        b = palette_list[i][2] >> 3
        rgb = (r << 11) | (g << 5) | b;
        palette_bin += rgb.to_bytes(2)

assert len(palette_bin) <= 4096

with open('bg.pic', 'wb') as f:
    f.write(img_width.to_bytes(2))
    f.write(img_height.to_bytes(2))

    f.write(int(len(palette_bin)/2 - 1).to_bytes(1)) # length of palette
    f.write(palette_bin)
    
    f.write(block_size.to_bytes(1))
    f.write(total_frames.to_bytes(1))

    blocks_of_x = int(img_height/block_size)
    blocks_of_y = int(img_width/block_size)
    assert blocks_of_x * blocks_of_y * total_frames == len(bitmap_bin)

    f.write(blocks_of_x.to_bytes(1)) # number of blocks
    f.write(blocks_of_y.to_bytes(1))
    
    for block in bitmap_bin:
        f.write(block)