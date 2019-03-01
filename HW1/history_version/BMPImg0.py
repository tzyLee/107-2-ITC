import struct as struct
HEADER_INFO = {"Identifier": 2, "FileSize": 4, "Reserved": 4, "BitmapDataOffset": 4,
               "BitmapHeaderSize": 4, "Width": 4, "Height": 4, "Planes": 2,
               "BitsPerPixel": 2, "Compression": 4, "BitmapDataSize": 4, "H_Resolution": 4,
               "V_Resolution": 4, "UsedColors": 4, "ImportantColors": 4}


class BMPImg:
    def __init__(self, imagePath=''):
        self.header = {key: 0 for key in HEADER_INFO}
        self.data = []
        if imagePath:
            self.loadPic(imagePath)

    def loadPic(self, picPath):
        print("--- Load Picture ---")
        with open(picPath, 'rb') as f:
            for key, byte_count in HEADER_INFO.items():
                if key == "Identifier":
                    self.header[key] = f.read(byte_count).decode('ascii')
                    if self.header[key] != 'BM':
                        print("Error: Not an BMP File")
                        return False
                else:
                    self.header[key] = int.from_bytes(
                        f.read(byte_count), byteorder='little')
            self.data = [list(f.read(3)) for i in range(
                self.header["BitmapDataSize"] // self.getBytesPerPixel())]  # BGR
        print("--- Picture Loaded ---")

    def getBytesPerPixel(self):
        return self.header["BitsPerPixel"] // 8

    def printHeader(self):
        for h in self.header:
            print('{:17}:'.format(h), self.header[h])

    def storePic(self, outputPath):
        print("--- Store Picture ---")
        with open(outputPath, 'wb') as f:
            for key, value in self.header.items():
                if key == "Identifier":
                    f.write(bytes(value, encoding='ascii'))
                elif key == "Planes" or key == "BitsPerPixel":
                    f.write(struct.pack('H', value))
                else:
                    f.write(struct.pack('I', value))
            for i in self.data:
                f.write(bytes(i))
        print("--- Storing Completed ---")

    def RGB2Y(self):
        print("--- RGB to Y ---")
        self.data = [[int(0.299*self.data[i][2] + 0.587 * self.data[i]
                          [1] + 0.114*self.data[i][0])]*3 for i in range(len(self.data))]

    def SobelFilter(self):
        print("--- Sobel Edge ---")
        w = self.header["Width"]
        h = self.header["Height"]
        sobel_data = [int(((self.data[(i+1)*w+j-1][0] + 2*self.data[(i+1)*w+j][0] + self.data[(i+1)*w+j+1][0] -
                            self.data[(i-1)*w+j-1][0] - 2*self.data[(i-1)*w+j][0] - self.data[(i-1)*w+j+1][0])**2 +
                           (self.data[(i-1)*w+j-1][0] + 2*self.data[i*w+j-1][0] + self.data[(i+1)*w+j-1][0] -
                            self.data[(i-1)*w+j+1][0] - 2*self.data[i*w+j+1][0] - self.data[(i+1)*w+j+1][0]) ** 2)**0.5)
                      if i != 0 and i != h-1 and j != 0 and j != w-1 else 255 for i in range(h) for j in range(w)]
        self.data = [[i]*3 for i in map(lambda x:x % 256, sobel_data)]
