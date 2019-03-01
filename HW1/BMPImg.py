HEADER_NUM = 15;
HEADER_SIZE = [2,4,4,4,4,4,4,2,2,4,4,4,4,4,4];
HEADER_INFO = ["Identifier","FileSize","Reserved","BitmapDataOffset",
               "BitmapHeaderSize","Width","Height","Planes",
               "BitsPerPixel","Compression","BitmapDataSize","H_Resolution",
               "V_Resolution","UsedColors","ImportantColors"];

class BMPHead:
    def __init__(self):
        self.Identifier = '';
        self.FileSize = 0;
        self.Reserved = 0;
        self.BitmapDataOffset = 0;
        self.BitmapHeaderSize = 0;
        self.Width = 0;
        self.Height = 0;
        self.Planes = 0;
        self.BitsPerPixel = 0;
        self.Compression = 0;
        self.BitmapDataSize = 0;
        self.H_Resolution = 0;
        self.V_Resolution = 0;
        self.UsedColors = 0;
        self.ImportantColors = 0;


class BMPImg:
    def __init__(self):
        self.header = BMPHead();
        self.data = "";
    
    def loadPic(self, picPath):
        pic = open(picPath, 'rb');
        
        for i in range(HEADER_NUM):
            a = pic.read(HEADER_SIZE[i]);
            self.header.setValue(i, a);
    
        dataSize =     self.getPxlNum() * self.getBytesPerPixel();
        self.data = pic.read(int(dataSize));

        pic.close();
            
    def getWidth(self):
        return self.header.getWidth();
    
    def getHeight(self):
        return self.header.getHeight();
    
    def getPxlNum(self):
        return self.header.getWidth() * self.header.getHeight();
    
    def getBytesPerPixel(self):
        return self.header.getBitsPerPixel() / 8;
    
    def printHeader(self):
        getDict = {
            "Identifier": self.header.Identifier,
            "FileSize": self.header.FileSize,
            "Reserved": self.header.Reserved,
            "BitmapDataOffset": self.header.BitmapDataOffset,
            "BitmapHeaderSize": self.header.BitmapHeaderSize,
            "Width": self.header.Width,
            "Height": self.header.Height,
            "Planes": self.header.Planes,
            "BitsPerPixel": self.header.BitsPerPixel,
            "Compression": self.header.Compression,
            "BitmapDataSize": self.header.BitmapDataSize,
            "H_Resolution": self.header.H_Resolution,
            "V_Resolution": self.header.V_Resolution,
            "UsedColors": self.header.UsedColors,
            "ImportantColors": self.header.ImportantColors
        }
        
        for h in HEADER_INFO:
            print(h+":", getDict[h]);

    def storePic(self, outputPath):
        getDict = {
            "Identifier": self.header.Identifier,
            "FileSize": self.header.FileSize,
            "Reserved": self.header.Reserved,
            "BitmapDataOffset": self.header.BitmapDataOffset,
            "BitmapHeaderSize": self.header.BitmapHeaderSize,
            "Width": self.header.Width,
            "Height": self.header.Height,
            "Planes": self.header.Planes,
            "BitsPerPixel": self.header.BitsPerPixel,
            "Compression": self.header.Compression,
            "BitmapDataSize": self.header.BitmapDataSize,
            "H_Resolution": self.header.H_Resolution,
            "V_Resolution": self.header.V_Resolution,
            "UsedColors": self.header.UsedColors,
            "ImportantColors": self.header.ImportantColors
        }
        
        self.header.updateGetDict();
        pic = open(outputPath, 'wb');
        for h in HEADER_INFO:
            pic.write(getDict[h]);
        pic.write(self.data);
        pic.close();
        print("--- Store Picture ---")

        
    def rotate(self):
        print("--- rotate ---")
        #TODO
        
    def RGB2Y(self):
        print("--- RGB to Y ---")
        #TODO
    
    def PrewittFilter(self):
        
        #Convert image to grayscale
        self.RGB2Y();
        print("--- PrewittFilter ---")
    #TODO: bonus
