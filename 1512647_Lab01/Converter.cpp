#include "Converter.h"


int Converter::GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage) {
	if (!sourceImage.data) {
		return 0;
	}
	// tạo một tra cứu màu dựa vào mức xám
	int T[5][3] = { {0, 0, 0},	//dark
				{101, 67, 33 },	// brown
				{255, 105, 180},	// pink
				{255, 255, 255}, 	// white
				{255, 255, 255 } };
	int range[] = { 0, 50, 120, 160, 255 };
	int cols = sourceImage.cols;
	int rows = sourceImage.rows;
	
	//destinationImage.create(rows, cols, CV_8UC1);
	
	for (int y = 0; y < rows; ++y) {
		const unsigned char* data = sourceImage.ptr<uchar>(y);
		unsigned char *data2 = destinationImage.ptr<uchar>(y);

		for (int x = 0; x < cols; ++x) {
			int gray = *data; data++;

			for (int i = 4;  i >= 0; --i) {
				if (gray >= range[i]) {
					*data2 = T[i][2]; data2++;
					*data2 = T[i][1]; data2++;
					*data2 = T[i][0]; data2++;
					break;
				}
			}

		}
	}

	
	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", destinationImage);
	waitKey(0);
	return 1;
}

int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type) {
	switch (type)
	{
	case 1: {
		return GrayScale2RGB(sourceImage, destinationImage);
		
		break;
	}
	default:
		break;
	}
	return 0;
}

Converter::Converter() {

}

Converter::~Converter() {

}