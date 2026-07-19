import sys
from PIL import Image
from pathlib import Path
import math

if len(sys.argv) < 3:
    print("Usage: script.py <image> <palette>")
    sys.exit(1)

image_path = sys.argv[1]
palette_path = sys.argv[2]

with open(palette_path, 'r') as f:
    palette_hex = [line.strip() for line in f if line.strip()]
palette = [tuple(int(h[i:i+2], 16) for i in (0, 2, 4)) for h in palette_hex]

image = Image.open(image_path).convert('RGBA')
width, height = image.size
name = Path(image_path).stem

def closest_index(r, g, b):
    min_dist = float('inf')
    idx = 0
    for i, (pr, pg, pb) in enumerate(palette):
        dist = (r - pr)**2 + (g - pg)**2 + (b - pb)**2
        if dist < min_dist:
            min_dist = dist
            idx = i
    return idx

print(f"const unsigned char {name}[{height}][{width}] = {{")
for y in range(height):
    print("    {", end=" ")
    row = []
    for x in range(width):
        r, g, b, _ = image.getpixel((x, y))
        idx = closest_index(r, g, b)
        row.append(f"0x{idx:02X}")
    print(", ".join(row), end="")
    print(" }," if y < height - 1 else " }")
print("};")
