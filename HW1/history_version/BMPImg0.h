#ifndef __BMPIMG_H__
#define __BMPIMG_H__

#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include <bitset>
#include<cmath>

static const int headerNum = 15;
static const int headerSize[headerNum] = {2,4,4,4, 4,4,4,2,2,4,4,4,4,4,4};
static const std::string headerInfo[headerNum]
    = {"Identifier","FileSize","Reserved","BitmapDataOffset",
       "BitmapHeaderSize","Width","Height","Planes",
       "BitsPerPixel","Compression","BitmapDataSize","H_Resolution",
       "V_Resolution","UsedColors","ImportantColors"
      };

class BMPHead {
private:
    char Identifier[2];
    unsigned int    FileSize;
    unsigned int    Reserved;
    unsigned int    BitmapDataOffset;

    unsigned int    BitmapHeaderSize;
    unsigned int    Width;
    unsigned int    Height;
    unsigned short  Planes;
    unsigned short  BitsPerPixel;
    unsigned int    Compression;
    unsigned int    BitmapDataSize;
    unsigned int    H_Resolution;
    unsigned int    V_Resolution;
    unsigned int    UsedColors;
    unsigned int    ImportantColors;
public:
    friend class BMPImg;
    void* pFlag(const int i)const { //return pointers of flags according to its order(int)
        switch(i) {
        default:
        case  0:
            return (void*)Identifier;
        case  1:
            return (void*)&FileSize;
        case  2:
            return (void*)&Reserved;
        case  3:
            return (void*)&BitmapDataOffset;
        case  4:
            return (void*)&BitmapHeaderSize;
        case  5:
            return (void*)&Width;
        case  6:
            return (void*)&Height;
        case  7:
            return (void*)&Planes;
        case  8:
            return (void*)&BitsPerPixel;
        case  9:
            return (void*)&Compression;
        case 10:
            return (void*)&BitmapDataSize;
        case 11:
            return (void*)&H_Resolution;
        case 12:
            return (void*)&V_Resolution;
        case 13:
            return (void*)&UsedColors;
        case 14:
            return (void*)&ImportantColors;
        }
    }
};
class BMPImg {
private:
    BMPHead header;
    int width, height, BPP;
    bool GreyScale;
    unsigned char*** sobelData;
public:
    unsigned char*** data;

    int getWidth()const {
        return header.Width;
    }
    int getHeight()const {
        return header.Height;
    }
    int getPxlNum()const {
        return header.Width * header.Height;
    }
    int getBytesPerPixel()const {
        return header.BitsPerPixel/8;
    }

    BMPImg() {}
    BMPImg(const std::string& picPath) {
        width = 0;
        height = 0;
        BPP = 0;
        sobelData = NULL;
        GreyScale = false;
        if(!loadPic(picPath))
            std::cout<<"Error: Not a BMP File\n";
    }
    ~BMPImg() {
        for(int i=0; i<height; i++) {
            for(int j=0; j<width; j++) {
                delete [] data[i][j];
            }
            delete [] data[i];
        }
        delete [] data;
        if(sobelData != NULL) {
            for(int i=0; i<height; i++) {
                for(int j=0; j<width; j++) {
                    delete [] sobelData[i][j];
                }
                delete [] sobelData[i];
            }
            delete [] sobelData;
        }
    }

    bool loadPic(const std::string& picPath) {
        std::ifstream ifs(picPath.c_str(), std::ios::in | std::ios::binary);
        unsigned int holder=ifs.get(), value=0, power=1;
        if(holder == 'B') {
            header.Identifier[0] = holder;
            holder = ifs.get();
            if(holder == 'M')
                header.Identifier[1] = holder;
            else
                return false;
        } else
            return false;
        for(int i=1; i<headerNum; i++) {
            for(int j=0; j<headerSize[i]; j++) {
                holder = ifs.get();
                value += holder * power;
                power *= 256;
            }
            power=1;
            if(i!=7 && i!=8)
                *(unsigned int*)header.pFlag(i) = value;
            else *(unsigned short*)header.pFlag(i) = value;
            value = 0;
        }
        height = getHeight();
        width = getWidth();
        BPP = getBytesPerPixel();
        if(height>0 && width>0) {
            data = new unsigned char**[height];
            for(int i=0; i<height; i++) {
                data[i] = new unsigned char*[width];
                for(int j=0; j<width; j++) {
                    data[i][j] = new unsigned char[BPP];
                }
            }
        }
        for(int i=0; i<height; i++) {
            for(int j=0; j<width; j++) {
                for(int k=2; k>=0; k--) {
                    holder = ifs.get();
                    data[i][j][k] = holder;//012 for RGB
                }
            }
        }
        return true;
    }
    void printHeader()const {
        for(int i=0; i<headerNum; ++i) {
            std::cout<<std::setw(18)<<headerInfo[i]+": ";

            if(i==0) {
                std::cout<< header.Identifier[0] << header.Identifier[1];
            } else {
                if(headerSize[i]==4)
                    std::cout<< *(  (unsigned int*)(header.pFlag(i))  );
                else
                    std::cout<< *(  (unsigned short*)(header.pFlag(i))  );
            }
            std::cout<< std::endl;
        }
    }
    bool RGB2Y() {
        unsigned int greyValue=0;
        for(int i=0; i<height; i++) {
            for(int j=0; j<width; j++) {
                greyValue = static_cast<int>(0.299*data[i][j][0] + 0.587*data[i][j][1] + 0.114*data[i][j][2]);
                for(int k=0; k<BPP; k++) {
                    data[i][j][k] = greyValue;
                }
            }
        }
        GreyScale = true;
        return true;
    }
    bool SobelFilter() {
        if(!GreyScale) RGB2Y();
        sobelData = new unsigned char**[height];
        for(int i=0; i<height; i++) {
            sobelData[i] = new unsigned char*[width];
            for(int j=0; j<width; j++) {
                sobelData[i][j] = new unsigned char[BPP];
            }
        }
        for(int i=1; i<height-1; i++) {
            for(int j=1; j<width-1; j++) {
                double G_x = data[i-1][j-1][0] + 2*data[i][j-1][0] + data[i+1][j-1][0] - data[i-1][j+1][0] - 2*data[i][j+1][0] - data[i+1][j+1][0];
                double G_y = data[i-1][j-1][0] + 2*data[i-1][j][0] + data[i-1][j+1][0] - data[i+1][j-1][0] - 2*data[i+1][j][0] - data[i+1][j+1][0];
                unsigned int sobelValue = static_cast<int>(std::sqrt(G_x*G_x + G_y*G_y));
                for(int k=0; k<BPP; k++) {
                    sobelData[i][j][k] = sobelValue;
                }
            }
        }
        for(int i=1; i<height-1; i++) {
            for(int j=0; j<BPP; j++) {
                sobelData[i][0][j] = 255;
                sobelData[i][width-1][j] = 255;
            }
        }
        for(int i=1; i<width-1; i++) {
            for(int j=0; j<BPP; j++) {
                sobelData[0][i][j] = 255;
                sobelData[height-1][i][j] = 255;
            }
        }
        for(int i=0; i<height; i++) {
            for(int j=0; j<width; j++) {
                for(int k=0; k<BPP; k++) {
                    data[i][j][k] = sobelData[i][j][k];
                }
            }
        }
        return true;
    }
    bool storePic(const std::string& outPath) {
        std::ofstream ofs(outPath.c_str(), std::ofstream::out | std::ios::binary);
        unsigned int outputByte = 0;
        for(int i=0; i<headerNum; i++) {
            unsigned int value = *(unsigned int*)header.pFlag(i);
            for(int j=0; j<headerSize[i]; j++) {
                outputByte = value%256;
                ofs<<(unsigned char)outputByte;
                value /= 256;
            }
        }
        for(int i=0; i<height; i++) {
            for(int j=0; j<width; j++) {
                for(int k=BPP-1; k>=0; k--) {
                    ofs<<(unsigned char)data[i][j][k];
                }
            }
        }
        ofs.close();
        return true;
    }
};

#endif // __BMPIMG_H__
