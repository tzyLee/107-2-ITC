#ifndef __BMPIMG_H__
#define __BMPIMG_H__

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

static const int headerNum = 15;
static const int headerSize[headerNum] = {2,4,4,4,4,4,4,2,2,4,4,4,4,4,4};
static const string headerInfo[headerNum] = { "Identifier", "FileSize",
        "Reserved", "BitmapDataOffset", "BitmapHeaderSize", "Width", "Height",
        "Planes", "BitsPerPixel", "Compression", "BitmapDataSize",
        "H_Resolution", "V_Resolution", "UsedColors", "ImportantColors" };

class BMPHead {
private:
    char Identifier[2];
    unsigned int FileSize;
    unsigned int Reserved;
    unsigned int BitmapDataOffset;
    unsigned int BitmapHeaderSize;
    unsigned int Width;
    unsigned int Height;
    unsigned short Planes;
    unsigned short BitsPerPixel;
    unsigned int Compression;
    unsigned int BitmapDataSize;
    unsigned int H_Resolution;
    unsigned int V_Resolution;
    unsigned int UsedColors;
    unsigned int ImportantColors;
    
public:
    friend class BMPImg;
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
public:
    unsigned char* data;

    int getWidth() const { return header.Width; }
    int getHeight() const { return header.Height; }
    int getPxlNum() const { return header.Width * header.Height; }
    int getBytesPerPixel() const { return header.BitsPerPixel / 8; }

    BMPImg() = delete;
    BMPImg(string picPath) { loadPic(picPath); }
    ~BMPImg() { delete[] data; }

    void copyHead(const BMPImg& ori) {
        header = ori.header;
        if(data)
            delete[] data;
        data = new unsigned char[getPxlNum() * getBytesPerPixel()];
    }

    bool loadPic(string picPath) {
        std::ifstream pic(picPath.c_str(), std::ios::in | std::ios::binary);
        for (int i = 0; i < headerNum; ++i) {
            pic.read(reinterpret_cast<char *>(header.pFlag(i)), headerSize[i]);
        }
        int dataSize = getPxlNum() * getBytesPerPixel();
        data = new unsigned char[dataSize];
        pic.read(reinterpret_cast<char *>(data), dataSize);
        pic.close();
        return true;
    }

    void printHeader() const {
        for (int i = 0; i < headerNum; ++i) {
            cout << headerInfo[i] + ":";

            if (i == 0) {
                cout << header.Identifier[0] << header.Identifier[1];
            } else {
                if (headerSize[i] == 4)
                    cout << *((unsigned int*) (header.pFlag(i)));
                else
                    cout << *((unsigned short*) (header.pFlag(i)));
            }
            cout << endl;
        }
    }

    
    bool storePic(string outPath) {
        std::ofstream picOut(outPath.c_str(), std::ofstream::out | std::ios::binary);
        for (int i = 0; i < headerNum; ++i) {
            picOut.write(reinterpret_cast<char *>(header.pFlag(i)), headerSize[i]);
        }
        picOut.write(reinterpret_cast<char *>(data), getPxlNum() * getBytesPerPixel());
        picOut.close();
        return true;
    }

    bool rotate() {
        unsigned char* temp = new unsigned char [getPxlNum() * getBytesPerPixel()];
        int index = 0;
        for(int i=0; i<header.Width; ++i)
            for(int j=header.Height-1; j>=0; --j) {
                temp[index++] = data[3 * (header.Width * j + i) + 0];
                temp[index++] = data[3 * (header.Width * j + i) + 1];
                temp[index++] = data[3 * (header.Width * j + i) + 2];
            }
        delete[] data;
        data = temp;
        std::swap(header.Width, header.Height);
        std::swap(header.H_Resolution, header.V_Resolution);
        return true;
    }
    
    bool RGB2Y() {
        auto BPP = getBytesPerPixel();
        for(unsigned i=0; i<getPxlNum()*getBytesPerPixel(); i += BPP) {
            unsigned char* pixelValue = &data[i];
            unsigned char greyValue = 0.3*pixelValue[2] + 0.59*pixelValue[1] + 0.11*pixelValue[0];
            pixelValue[0] = pixelValue[1] = pixelValue[2] = greyValue;
        }
        return true;
    }

    bool PrewittFilter() {
        // Convert image to grayscale
        RGB2Y();
        unsigned char* temp = new unsigned char [getPxlNum() * getBytesPerPixel()];
        for(int i=0; i<header.Width; ++i) {
            temp[3 * i] = temp[3 * i + 1] = temp[3 * i + 2] = 255;
            temp[3 * (header.Width * (header.Height - 1) + i)] = temp[3 * (header.Width * (header.Height - 1) + i) + 1] = temp[3 * (header.Width * (header.Height - 1) + i) + 2] = 255;
        }
        for(int i=1; i<header.Height; ++i) {
            temp[3 * (header.Width * i)] = temp[3 * (header.Width * i) + 1] = temp[3 * (header.Width * i) + 2] = 255; // First column
            temp[3 * (header.Width * i + header.Width-1)] = temp[3 * (header.Width * i + header.Width-1) + 1] = temp[3 * (header.Width * i + header.Width-1) + 2] = 255; // Last column
        }
        for(int i=1; i<header.Height-1; ++i)
            for(int j=1; j<header.Width-1; ++j) {
                double g_x = data[3 * (header.Width * (i-1) + (j+1))] + data[3 * (header.Width * i + (j+1))] + data[3 * (header.Width * (i+1) + (j+1))]
                           - data[3 * (header.Width * (i-1) + (j-1))] - data[3 * (header.Width * i + (j-1))] - data[3 * (header.Width * (i+1) + (j-1))];
                double g_y = data[3 * (header.Width * (i-1) + (j-1))] + data[3 * (header.Width * (i-1) + j)] + data[3 * (header.Width * (i-1) + (j+1))]
                           - data[3 * (header.Width * (i+1) + (j-1))] - data[3 * (header.Width * (i+1) + j)] - data[3 * (header.Width * (i+1) + (j+1))];
                temp[3 * (i*header.Width + j)] = temp[3 * (i*header.Width + j) + 1] = temp[3 * (i*header.Width + j) + 2] = std::hypot(g_x, g_y);
            }
        delete[] data;
        data = temp;
        return true;
    }

};

#endif // __BMPIMG_H__
