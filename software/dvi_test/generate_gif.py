from PIL import Image, ImageSequence

img_width: int = 0
img_height: int = 0
total_frames: int = 1
block_size = 16
palette_bin = b''
bitmap_bin = []

img_list = []

with Image.open('background.gif') as img_group:
    for img in ImageSequence.Iterator(img_group):
       img_list.append(img.copy().crop((80, 30, 400, 270)))

img_list[0].save("bg_cropped.gif", save_all=True, append_images=img_list[1:])

