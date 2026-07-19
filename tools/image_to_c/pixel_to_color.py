import sys
from PIL import Image
from pathlib import Path

if(len(sys.argv) < 2):
    print("Give path to image")
    sys.exit(0)

path = sys.argv[1];
image = Image.open(path).convert('RGBA')

name = Path(path).stem
width, height = image.size

print(f"const uint32_t {name}[{height}][{width}] = {{\n")

for y in range(height):
    print("    {")
    row_pixels = []
    for x in range(width):
        r, g, b, a = image.getpixel((x, y))
        pixel32 = (a << 24) | (r << 16) | (g << 8) | b
        row_pixels.append(f"0x{pixel32:08X}")
    print(", ".join(row_pixels))
    print("    },\n" if y < height - 1 else "    }\n")

print("};\n")
