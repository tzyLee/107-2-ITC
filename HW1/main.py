import BMPImg;

def main():
    # mian function must not change
	bmpimg = BMPImg.BMPImg();
	bmpimg.loadPic('img/rabbit.bmp');
	bmpimg.rotate();
	bmpimg.storePic('output/rabbit_ans.bmp');

    bmpimg = BMPImg.BMPImg();
    bmpimg.loadPic('img/flower.bmp');
    bmpimg.PrewittFilter();
    bmpimg.storePic('output/flower_ans.bmp');


if __name__ == '__main__':
	main();
