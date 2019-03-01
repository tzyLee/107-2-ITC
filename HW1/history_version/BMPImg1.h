#ifndef __BMPIMG_H__
#define __BMPIMG_H__

#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

static uint8_t headerSize[] = {4,4,4, 4,4,4,2,2,4,4,4,4,4,4};
static const std::array<const char*, 14> headerInfo = { "FileSize", "Reserved", "BitmapDataOffset",
                                                        "BitmapHeaderSize", "Width", "Height", "Planes",
                                                        "BitsPerPixel", "Compression", "BitmapDataSize", "H_Resolution",
                                                        "V_Resolution", "UsedColors", "ImportantColors"};

struct BMPHeader {
    uint32_t FileSize, Reserved, BitmapDataOffset;

    uint32_t BitmapHeaderSize, Width, Height;
    uint16_t Planes, BitsPerPixel;
    uint32_t Compression, BitmapDataSize, H_Resolution, V_Resolution, UsedColors, ImportantColors;
    bool load(std::istream& pic) {
        char Identifier[2];
        pic.read(Identifier, 2);
        if(std::strcmp("BM", Identifier) != 0)
            return false;
        return static_cast<bool>(pic.read(reinterpret_cast<char *>(this), sizeof(BMPHeader)));
    }
    bool save(std::ostream& file) {
        file << "BM";
        return static_cast<bool>(file.write(reinterpret_cast<char *>(this), sizeof(BMPHeader)));
    }
    friend std::ostream& operator<<(std::ostream& os, const BMPHeader& header) {
        os << std::setw(18) << "Identifier" << ": BM\n";
        uint8_t* infoSize = headerSize;
        uint8_t* temp = reinterpret_cast<uint8_t *>(const_cast<BMPHeader *>(&header));
        for(const auto& info : headerInfo) {
            os << std::setw(18) << info << ": ";
            switch(*(infoSize++)) {
                case 4:
                    std::cout << *reinterpret_cast<u_int32_t *>(temp);
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

struct PixelArray { // Only works for RGB[8,8,8]
    PixelArray() = delete;
    PixelArray(unsigned width, unsigned height, unsigned bytePerPixel)
    : _width(width), _height(height), _bytePerPixel(bytePerPixel),
      _data(new unsigned char[_width*_height*_bytePerPixel]) {}
    inline unsigned index(unsigned h_index, unsigned w_index, unsigned color) const {
        return _bytePerPixel * (_width * w_index + h_index) + color;
    }
    inline size_t size() const { return _width*_height*_bytePerPixel; }
    unsigned _width, _height, _bytePerPixel;
    std::unique_ptr<unsigned char[]> _data;
};

class BMPImg {
private:
    BMPHeader header;
    unsigned width, height, BPP;
    bool GreyScale;
    std::unique_ptr<PixelArray> sobelData, data;
public:
    int getWidth() const { return header.Width; }
    int getHeight() const { return header.Height; }
    int getPxlNum() const { return header.Width * header.Height; }
    int getBytesPerPixel() const { return header.BitsPerPixel/8; }

    BMPImg() = delete;
    BMPImg(const std::string& picPath)
    : width(0), height(0), BPP(0), GreyScale(false), sobelData(nullptr), data(nullptr){
        if(!loadPic(picPath))
            std::cerr << "Error: Not a BMP File\n";
    }

    bool loadPic(const std::string& picPath) {
        std::ifstream pic(picPath.c_str(), std::ios::in | std::ios::binary);
        if(!pic || !header.load(pic))
            return false;
        height = getHeight();
        width = getWidth();
        BPP = getBytesPerPixel();
        if(height>0 && width>0) {
            data.reset(new PixelArray(height, width, BPP));
            pic.read(reinterpret_cast<char *>(data->_data.get()), data->size()); // BGR
        }
        pic.close();
        return true;
    }

    void printHeader() const { std::cout << header; }
    bool RGB2Y() {
        for(unsigned i=0; i<data->size(); i += BPP) {
            unsigned char* pixelValue = &data->_data[i];
            unsigned char greyValue = 0.299*pixelValue[2] + 0.587*pixelValue[1] + 0.114*pixelValue[0];
            pixelValue[0] = pixelValue[1] = pixelValue[2] = greyValue;
        }
        GreyScale = true;
        return true;
    }

    bool SobelFilter() {
        if(!GreyScale)
            RGB2Y();
        sobelData.reset(new PixelArray(height, width, BPP));
        for(unsigned i=1; i<height-1; ++i) {
            for(unsigned j=1; j<width-1; ++j) {
                unsigned char* top = &data->_data[data->index(i-1, j, 0)],
                             * mid = &data->_data[data->index(i, j, 0)],
                             * btm = &data->_data[data->index(i+1, j, 0)];
                double G_x = *(top-BPP) + 2* *(mid-BPP) + *(btm-BPP) - top[BPP] - 2*mid[BPP] - btm[BPP];
                double G_y = *(top-BPP) + 2*top[0] + top[BPP] - *(btm-BPP) - 2*btm[0] - btm[BPP];
                unsigned sobelValue = std::sqrt(G_x*G_x + G_y*G_y);
                unsigned char* pixelValue = &sobelData->_data[data->index(i, j, 0)];
                for(unsigned k=0; k<BPP; k++)
                    pixelValue[k] = sobelValue;
            }
        }
        data.swap(sobelData); // Replace data with sobel'd data
        return true;
    }

    bool storePic(const std::string& outPath) {
        std::ofstream ofs(outPath, std::ofstream::out | std::ios::binary);
        header.save(ofs);
        ofs.write(reinterpret_cast<char *>(data->_data.get()), data->size());
        ofs.close();
        return true;
    }
};

#endif // __BMPIMG_H__
