#ifndef __BMPIMG_H__
#define __BMPIMG_H__

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

static uint_fast8_t headerSize[] = {4,4,4, 4,4,4,2,2,4,4,4,4,4,4};
static const std::array<const char*, 14> headerInfo = { "FileSize", "Reserved", "BitmapDataOffset",
                                                        "BitmapHeaderSize", "Width", "Height", "Planes",
                                                        "BitsPerPixel", "Compression", "BitmapDataSize", "H_Resolution",
                                                        "V_Resolution", "UsedColors", "ImportantColors"};


class BMPHead { // Works for memory alignment i, i == 2 ** n, n > 1 (such that the memory layout of this class is continuous)
    friend class BMPImg; // Not storing BM, 1. This class is intended for BMP files, 2. Memory alignment
    uint32_t FileSize, Reserved, BitmapDataOffset;

    uint32_t BitmapHeaderSize, Width, Height;
    uint16_t Planes, BitsPerPixel;
    uint32_t Compression, BitmapDataSize, H_Resolution, V_Resolution, UsedColors, ImportantColors;
    bool load(std::istream& pic) { // Assume file is in binary mode
        char Identifier[2];
        pic.read(Identifier, 2);
        if(std::strcmp("BM", Identifier) != 0) // Check for file type
            return false;
        return static_cast<bool>(pic.read(reinterpret_cast<char *>(this), sizeof(BMPHead)));
    }
    bool save(std::ostream& file) { // Assume file is in binary mode
        file << "BM";
        return static_cast<bool>(file.write(reinterpret_cast<char *>(this), sizeof(BMPHead)));
    }
    friend std::ostream& operator<<(std::ostream& os, const BMPHead& header) {
        os << "Identifier:BM\n";
        uint_fast8_t* infoSize = headerSize;
        uint8_t* temp = reinterpret_cast<uint8_t *>(const_cast<BMPHead *>(&header)); // Move 1 byte when +1
        for(const auto& info : headerInfo) {
            os << info << ':';
            switch(*(infoSize++)) {
                case 4:
                    std::cout << *reinterpret_cast<uint32_t *>(temp);
                    temp += 4;
                    break;
                case 2:
                default:
                    std::cout << *reinterpret_cast<uint16_t *>(temp);
                    temp += 2;
                    break;
            }
            os.put('\n');
        }
        return os.flush();
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
    BMPImg(const std::string& picPath) { loadPic(picPath); }
    ~BMPImg() { delete[] data; }

    void copyHead(const BMPImg& ori) {
        header = ori.header;
        if(data) // Prevent potential memory leak
            delete[] data;
        data = new unsigned char[getPxlNum() * getBytesPerPixel()];
    }

    bool loadPic(const std::string& picPath) {
        std::ifstream pic(picPath.c_str(), std::ios::in | std::ios::binary);
        if(!pic || !header.load(pic))
            return false;
        auto offset = header.BitmapDataOffset;
        if(offset > sizeof(BMPHead) + 2) { // +2 for "BM"
            pic.seekg(offset - sizeof(BMPHead) - 2, std::ios_base::cur);
        }
        int dataSize = getPxlNum() * getBytesPerPixel();
        data = new unsigned char[dataSize];
        pic.read(reinterpret_cast<char *>(data), dataSize);
        pic.close();
        return true;
    }

    void printHeader() const { std::cout << header; }
    
    bool storePic(const std::string& outPath) {
        std::ofstream picOut(outPath.c_str(), std::ofstream::out | std::ios::binary);
        header.save(picOut);
        picOut.write(reinterpret_cast<char *>(data), getPxlNum() * getBytesPerPixel());
        picOut.close();
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
        std::swap(header.H_Resolution, header.V_Resolution);
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
        // Convert image to grayscale
        RGB2Y();
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
