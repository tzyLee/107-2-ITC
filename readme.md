# Simple BMP reader

## Requirements
- BMP files
  | Header           | Value |
  | ---------------- | ----- |
  | Plane            | 1     |
  | Bits per pixel   | 24    |
  | Compression      | 0     |
  | Used Colors      | 0     |
  | Important Colors | 0     |
- Machine

  std::fstream reads in little endian

  Memory alignment is power of 2, and is greater than 1