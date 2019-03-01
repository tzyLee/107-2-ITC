#include <string>
#include "BMPImg.h"

int main(int argc, char* argv[]) {
    // main function must not change
	BMPImg imgrabbit("./img/rabbit.bmp");
	imgrabbit.rotate();
	imgrabbit.storePic("./output/rabbit_ans.bmp");
	BMPImg aaa("./result.bmp");
	aaa.rotate();
	aaa.storePic("./result_rotated.bmp");

	BMPImg imgflower("./img/flower.bmp");
	imgflower.PrewittFilter();
	imgflower.storePic("./output/flower_ans.bmp");

	return 0;
}


template<size_t width, size_t height, class ValueType = unsigned char>
union PixelArray{
	ValueType flatten[width*height*3];
	ValueType data[width][height][3];
	ValueType transposed[height][width][3];
};