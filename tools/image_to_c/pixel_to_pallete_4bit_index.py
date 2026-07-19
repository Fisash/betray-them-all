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

row_bytes = (width + 1) // 2

print(f"const unsigned char {name}[{height}][{row_bytes}] = {{")
for y in range(height):
    indices = []
    for x in range(width):
        r, g, b, _ = image.getpixel((x, y))
        indices.append(closest_index(r, g, b))
    
    packed = []
    for i in range(0, len(indices), 2):
        low = indices[i] & 0x0F
        high = indices[i+1] & 0x0F if i+1 < len(indices) else 0
        packed.append(f"0x{(high << 4) | low:02X}")
    
    print("    {", ", ".join(packed), end=" }")
    if y < height - 1:
        print(",")
    else:
        print()
print("};")
