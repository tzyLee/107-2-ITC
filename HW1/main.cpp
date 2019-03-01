#include <string>
#include "BMPImg.h"

int main(int argc, char* argv[]) {
    // main function must not change
	BMPImg imgrabbit("./img/rabbit.bmp");
	imgrabbit.rotate();
	imgrabbit.storePic("./output/rabbit_ans.bmp");

	BMPImg imgflower("./img/flower.bmp");
	imgflower.PrewittFilter();
	imgflower.storePic("./output/flower_ans.bmp");

	return 0;
}


