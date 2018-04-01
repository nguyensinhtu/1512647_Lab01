#include "Converter.h"
#include"ColorTransformer.h"

#include <iostream>

int main(int argc, char **argv) {
	int ch = 0, b = 0, res = 0;
	float c = 1;
	Converter *converter = new Converter();
	ColorTransformer *colorTransformer = new ColorTransformer();
	Mat dest;
	Mat image = imread(argv[2], CV_LOAD_IMAGE_ANYCOLOR);

	if (!image.data) {
		cout << "khong mo duoc anh" << endl;
		return -1;
	}

	if (strcmp(argv[1], "--rgb2gray") == 0) {
		ch = 0;
	}
	else if (strcmp(argv[1], "--gray2rgb") == 0) {
		ch = 1;
	}
	else if (strcmp(argv[1], "--rgb2hsv") == 0) {
		ch = 2;
	}
	else if (strcmp(argv[1], "--hsv2rgb") == 0) {
		ch = 3;
	}
	else if (strcmp(argv[1], "--bright") == 0) {
		ch = 4;
		stringstream ss(argv[3]); ss >> b;
		res = colorTransformer->ChangeBrighness(image, dest, b);
	}
	else if (strcmp(argv[1], "--contrast") == 0) {
		ch = 5;
		stringstream ss(argv[3]); ss >> c;
		res = colorTransformer->ChangeContrast(image, dest, c);
	}
	else if (strcmp(argv[1], "--hist") == 0) {
		ch = 6;
		res = colorTransformer->DrawHistogram(image, dest);
	}
	else if (strcmp(argv[1], "--equalhist") == 0) {
		ch = 7;
		res = colorTransformer->HistogramEqualization(image, dest);
	}
	else {
		cout << "not found : " << argv[1] << endl;
	}

	if (ch < 4) {
		res = converter->Convert(image, dest, ch);
	}

	if (res == 0) {
		cout << "loi chuyen doi" << endl;
	}


	image.release();
	dest.release();

	
	return 0;
}

ColorTransformer::ColorTransformer() {

}
ColorTransformer::~ColorTransformer() {

}

