#ifndef __BMPIMG_H__
#define __BMPIMG_H__

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

static const int headerNum = 15;
static const int headerSize[] = {2,4,4,4,4,4,4,2,2,4,4,4,4,4,4};
static const char* headerInfo[] = { "Identifier", "FileSize",
        "Reserved", "BitmapDataOffset", "BitmapHeaderSize", "Width", "Height",
        "Planes", "BitsPerPixel", "Compression", "BitmapDataSize",
        "H_Resolution", "V_Resolution", "UsedColors", "ImportantColors" };

class BMPHead {
    friend class BMPImg;
private:
    char Identifier[2];
    uint32_t FileSize;
    uint32_t Reserved;
    uint32_t BitmapDataOffset;
    uint32_t BitmapHeaderSize;
    uint32_t Width;
    uint32_t Height;
    uint16_t Planes;
    uint16_t BitsPerPixel;
    uint32_t Compression;
    uint32_t BitmapDataSize;
    uint32_t H_Resolution;
    uint32_t V_Resolution;
    uint32_t UsedColors;
    uint32_t ImportantColors;
public:
    void* pFlag(const int i) const { //return pointers of flags according to its order(int)
        switch (i) {
        default:
        case 0:
            return (void*) Identifier;
        case 1:
            return (void*) &FileSize;
        case 2:
            return (void*) &Reserved;
        case 3:
            return (void*) &BitmapDataOffset;
        case 4:
            return (void*) &BitmapHeaderSize;
        case 5:
            return (void*) &Width;
        case 6:
            return (void*) &Height;
        case 7:
            return (void*) &Planes;
        case 8:
            return (void*) &BitsPerPixel;
        case 9:
            return (void*) &Compression;
        case 10:
            return (void*) &BitmapDataSize;
        case 11:
            return (void*) &H_Resolution;
        case 12:
            return (void*) &V_Resolution;
        case 13:
            return (void*) &UsedColors;
        case 14:
            return (void*) &ImportantColors;
        }
    }
};

class BMPImg {
private:
    BMPHead header;
    inline unsigned _ind(unsigned h, unsigned w, unsigned c) const {
        return 3 * (header.Width * h + w) + c; // Only works for RGB[8, 8, 8]
    }
    inline unsigned _ind(unsigned h, unsigned w) const { return 3 * (header.Width * h + w); }
public:
    unsigned char* data;

    int getWidth() const { return header.Width; }
    int getHeight() const { return header.Height; }
    int getPxlNum() const { return header.Width * header.Height; }
    int getBytesPerPixel() const { return header.BitsPerPixel / 8; }

    BMPImg() = delete;
    BMPImg(const std::string& picPath) {
        loadPic(picPath);
    }
    ~BMPImg() {
        delete[] data;
    }

    void copyHead(const BMPImg& ori) {
        header = ori.header;
        if(data) // Prevent potential memory leak
            delete[] data;
        data = new unsigned char[getPxlNum() * getBytesPerPixel()];
    }

    bool loadPic(const std::string& picPath) {
        std::ifstream pic(picPath.c_str(), std::ios::in | std::ios::binary);
        if(!pic)
            return false;
        for (int i = 0; i < headerNum; ++i) {
            pic.read((char*) (header.pFlag(i)), headerSize[i]);
        }
        int dataSize = getPxlNum() * getBytesPerPixel();
        data = new unsigned char[dataSize];
        pic.read((char*) data, dataSize);
        return true;
    }

    void printHeader() const {
        for (int i = 0; i < headerNum; ++i) {
            std::cout << headerInfo[i] << ':';

            if (i == 0) {
                std::cout << header.Identifier[0] << header.Identifier[1];
            } else {
                if (headerSize[i] == 4)
                    std::cout << *((unsigned int*) (header.pFlag(i)));
                else
                    std::cout << *((unsigned short*) (header.pFlag(i)));
            }
            std::cout << std::endl;
        }
    }

    
    bool storePic(const std::string& outPath) {
        std::ofstream picOut(outPath.c_str(), std::ios::out | std::ios::binary);
        for (int i = 0; i < headerNum; ++i) {
            picOut.write((char*) (header.pFlag(i)), headerSize[i]);
        }
        picOut.write((char*) data, getPxlNum() * getBytesPerPixel());
        return true;
    }

    bool rotate() {
        unsigned char* temp = new unsigned char [getPxlNum() * getBytesPerPixel()];
        int index = 0;
        for(unsigned i=0; i<header.Width; ++i)
            for(int j=header.Height-1; j>=0; --j) {
                temp[index++] = data[_ind(j, i, 0)];
                temp[index++] = data[_ind(j, i, 1)];
                temp[index++] = data[_ind(j, i, 2)];
            }
        delete[] data;
        data = temp;
        std::swap(header.Width, header.Height);
        return true;
    }
    
    bool RGB2Y() {
        unsigned bpp = getBytesPerPixel(), len = getPxlNum() * bpp;
        for(unsigned i=0; i<len; i += bpp) {
            data[i] = data[i+1] = data[i+2] = (30*data[i+2] + 59*data[i+1] + 11*data[i])/100;
            // data[i] = data[i+1] = data[i+2] = (299*data[i+2] + 587*data[i+1] + 114*data[i])/1000;
        }
        return true;
    }

    bool PrewittFilter() {
        RGB2Y();
        //Convert image to grayscale
        unsigned char* temp = new unsigned char [getPxlNum() * getBytesPerPixel()];
        unsigned lastHeight = header.Height - 1, lastWidth = header.Width - 1;
        for(unsigned i=0; i<header.Width; ++i) {
            temp[3 * i] = temp[3 * i + 1] = temp[3 * i + 2] = 255;
            temp[_ind(lastHeight, i, 0)] = temp[_ind(lastHeight, i, 1)] = temp[_ind(lastHeight, i, 2)] = 255;
        }
        for(unsigned i=1; i<header.Height; ++i) {
            temp[3 * (header.Width * i)] = temp[3 * (header.Width * i) + 1] = temp[3 * (header.Width * i) + 2] = 255; // First column
            temp[_ind(i, lastWidth, 0)] = temp[_ind(i, lastWidth, 1)] = temp[_ind(i, lastWidth, 2)] = 255; // Last column
        }
        for(unsigned i=1; i<lastHeight; ++i)
            for(unsigned j=1; j<lastWidth; ++j) {
                double g_x = data[_ind(i-1, j+1)] + data[_ind(i, j+1)] + data[_ind(i+1, j+1)]
                           - data[_ind(i-1, j-1)] - data[_ind(i, j-1)] - data[_ind(i+1, j-1)];
                double g_y = data[_ind(i-1, j-1)] + data[_ind(i-1, j)] + data[_ind(i-1, j+1)]
                           - data[_ind(i+1, j-1)] - data[_ind(i+1, j)] - data[_ind(i+1, j+1)];
                temp[_ind(i, j, 0)] = temp[_ind(i, j, 1)] = temp[_ind(i, j, 2)] = std::min(std::hypot(g_x, g_y), 255.0);
            }
        delete[] data;
        data = temp;
        return true;
    }

};

#endif // __BMPIMG_H__
