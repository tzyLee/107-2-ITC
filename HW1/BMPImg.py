from struct import Struct, calcsize
from math import hypot
from array import array

class BMPHead:
    HEADER_SIZE = [2,4,4,4,4,4,4,2,2,4,4,4,4,4,4]
    __slots__ = ("Identifier", "FileSize", "Reserved", "BitmapDataOffset",
               "BitmapHeaderSize", "Width", "Height", "Planes",
               "BitsPerPixel", "Compression", "BitmapDataSize", "H_Resolution",
               "V_Resolution", "UsedColors", "ImportantColors")
    struct = Struct(''.join('H' if i == 2 else 'I' for i in HEADER_SIZE[1:]))

    def load(self, file):
        assert 'b' in file.mode and file.readable(), "File is not opened in binary mode or not readable"
        self.Identifier = file.read(2).decode('ascii')
        res = self.struct.unpack(file.read(sum(self.HEADER_SIZE) - 2))
        for key, value in zip(self.__slots__[1:], res):
            setattr(self, key, value)
    
    def save(self, file):
        assert 'b' in file.mode and file.writable(), "File is not opened in binary mode or not writable"
        file.write(self.Identifier.encode('ascii'))
        file.write(self.struct.pack(*(getattr(self, i) for i in self.__slots__[1:])))
    
    def __iter__(self):
        return map(lambda i: (i, getattr(self, i)), self.__slots__)

class PixelArray:
    def __init__(self, array, width=None):
        self.array = array
        self.width = width
    
    def __getitem__(self, index):
        if type(index) is tuple:
            ret = 3*(self.width*index[0] + index[1])
            if len(index) == 3:
                ret += index[2]
            return self.array[ret]
        else:
            return self.array[index]

    def __setitem__(self, index, value):
        if type(index) is tuple:
            ret = 3*(self.width*index[0] + index[1])
            if len(index) == 3:
                ret += index[2]
            self.array[ret] = value
        else:
            self.array[index] = value
class BMPImg:
    def __init__(self):
        self.header = BMPHead()
        self.data = PixelArray(array('B'))
    
    def loadPic(self, picPath):
        with open(picPath, 'rb') as pic:
            self.header.load(pic)
            self.data.width = self.header.Width
            dataSize = self.header.Width * self.header.Height * (self.header.BitsPerPixel // 8)
            self.data.array.fromfile(pic, dataSize)
    
    def getBytesPerPixel(self):
        return self.header.BitsPerPixel // 8
    
    def printHeader(self):
        for key, value in self.header:
            print(key, ": ", value, sep='')

    def storePic(self, outputPath):
        with open(outputPath, 'wb') as pic:
            self.header.save(pic)
            self.data.array.tofile(pic)
        print("--- Store Picture ---")
        
    def rotate(self):
        print("--- Rotate ---")
        arr = [self.data[j, i, k] for i in range(self.header.Width) for j in reversed(range(self.header.Height)) for k in range(3)]
        temp = PixelArray(array('B', arr), self.header.Width)
        self.data = temp
        self.header.Width, self.header.Height = self.header.Height, self.header.Width
        
    def RGB2Y(self):
        print("--- RGB to Y ---")
        for i in range(len(self.data.array)//3):
            self.data[3*i] = self.data[3*i+1] = self.data[3*i+2] = \
                int(0.299*self.data[3*i+2] + 0.587*self.data[3*i+1] + 0.114*self.data[3*i])
    
    def PrewittFilter(self):
        """Convert image to grayscale"""
        self.RGB2Y()
        print("--- PrewittFilter ---")
        temp = PixelArray(array('B', b'\xFF'*len(self.data.array)), self.header.Width)
        data = self.data
        for i in range(1, self.header.Height-1):
            for j in range(1, self.header.Width-1):
                g_x = data[i-1, j+1] + data[i, j+1] + data[i+1, j+1] - data[i-1, j-1] - data[i, j-1] - data[i+1, j-1]
                g_y = data[i-1, j-1] + data[i-1, j] + data[i-1, j+1] - data[i+1, j-1] - data[i+1, j] - data[i+1, j+1]
                temp[i, j, 0] = temp[i, j, 1] = temp[i, j, 2] = min(round(hypot(g_x, g_y)), 255)
        self.data = temp
