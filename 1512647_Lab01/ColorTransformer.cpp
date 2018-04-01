#include "ColorTransformer.h"

int normalize(int b) {
	if (b > 255)
		return 255;
	if (b < 0)
		return 0;
	return b;
}

int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, uchar b) {
	if (!sourceImage.data) {
		return 0;
	}

	int width = sourceImage.cols;
	int height = sourceImage.rows;
	
	bool isGrayScale = false;
	if (sourceImage.type() == CV_8UC1)
		isGrayScale = true;

	destinationImage.create(height, width, sourceImage.type());
	if (!destinationImage.data) {
		return 0;
	}

	for (int y = 0; y < height; ++y) {
		const unsigned char* data = sourceImage.ptr<uchar>(y);
		unsigned char* data2 = destinationImage.ptr<uchar>(y);
		for (int x = 0; x < width; ++x) {
			if (!isGrayScale) {		
				*data2 = normalize((int)(*data + b));   data++; data2++;
				*data2 = normalize((int)(*data + b)); data++; data2++;
				*data2 = normalize((int)(*data + b)); data++; data2++;
			}
			else {
				*data2 = normalize((int)(*data + b)); data++; data2++;
			}
		}
	}

	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", destinationImage);
	waitKey(0);
	return 1;
}


int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c) {
	if (!sourceImage.data) {
		return 0;
	}

	int width = sourceImage.cols;
	int height = sourceImage.rows;

	bool isGrayScale = false;
	if (sourceImage.type() == CV_8UC1)
		isGrayScale = true;

	destinationImage.create(height, width, sourceImage.type());
	if (!destinationImage.data) {
		return 0;
	}

	for (int y = 0; y < height; ++y) {
		const unsigned char* data = sourceImage.ptr<uchar>(y);
		unsigned char* data2 = destinationImage.ptr<uchar>(y);
		for (int x = 0; x < width; ++x) {
			if (!isGrayScale) {
				*data2 = normalize((int)round(c*log(1 + *data)));   data++; data2++;
				*data2 = normalize((int)round(c*log(1 + *data))); data++; data2++;
				*data2 = normalize((int)round(c*log(1 + *data))); data++; data2++;
			}
			else {
				*data2 = normalize((int)round(c*log(1 + *data))); data++; data2++;
			}
		}
	}

	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", destinationImage);
	waitKey(0);
	return 1;
}


int ColorTransformer::CalcHistogram(const Mat& sourceImage, Mat& histogram) {

}