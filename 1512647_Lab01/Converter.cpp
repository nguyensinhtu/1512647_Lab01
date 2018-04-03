#include "Converter.h"

// kiểm tra một ảnh có phải là ảnh xám
// input : ảnh cần kiểm tra
// output : trả về true nếu là ảnh xám
//			trả về false ngược lại
bool IsGrayScale(const Mat &image) {
	if (image.type() == CV_8UC1)
		return true;
	return false;
}

// matrix transform
float m[3][3] = { { sqrt(3) / 3, sqrt(3) / 3 , sqrt(3) / 3 },
					{ 0, 1 / sqrt(2), -1 / sqrt(2) },
					{ 2 / sqrt(6), -1 / sqrt(6), -1 / sqrt(6) } };

int Converter::GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage) {
	if (!sourceImage.data) {
		return 0;
	}

	if (!IsGrayScale(sourceImage)) {
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
	
	destinationImage.create(rows, cols, CV_8UC3);
	
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

	imwrite("RGBImage.jpg", destinationImage);
	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", destinationImage);
	waitKey(0);
	return 1;
}

int Converter::Convert(const Mat& sourceImage, Mat& destinationImage, int type) {
	switch (type)
	{
	case 0: {
		return RGB2GrayScale(sourceImage, destinationImage);
	}
	case 1: {
		return GrayScale2RGB(sourceImage, destinationImage);
		break;
	}
	case 2: {
		return RGB2HSV(sourceImage, destinationImage);
		break;
	}
	case 3: {
		return HSV2RGB(sourceImage, destinationImage);
		break;
	}
	default:
		cout << "type khong dung" << endl;
		break;
	}
	return 0;
}

int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage) {
	if (!sourceImage.data) {
		return 0;
	}

	int width = sourceImage.cols;
	int height = sourceImage.rows;

	destinationImage.create(height, width, CV_8UC3);

	//V = m[0][0] * ;
	for (int y = 0; y < height; ++y) {
		const unsigned char *data = sourceImage.ptr<uchar>(y);
		 unsigned char * data1 = destinationImage.ptr<uchar>(y);

		for (int x = 0; x < width; ++x) {
			int b = *data; data++;
			int g = *data; data++;
			int r = *data; data++;
			float V = m[0][0] * r + m[0][1] * g + m[0][2] * b;
			float V1 = m[1][0] * r + m[1][1] * g + m[1][2] * b;
			float V2 = m[2][0] * r + m[2][1] * g + m[2][2] * b;

			
			*data1 = round(atan(V2 / V1));  data1++;
			*data1 = round(sqrt(pow(V1, 2) + pow(V2, 2)));  data1++;
			*data1 = round(V);  data1++;
			}
	}

	imwrite("hsvImage.jpg", destinationImage);
	return 1;

}

int Converter::HSV2RGB(const Mat& sourceImage, Mat& destinationImage) {
	if (!sourceImage.data) {
		return 0;
	}

	int width = sourceImage.cols;
	int height = sourceImage.rows;

	destinationImage.create(height, width, CV_8UC3);

	for (int y = 0; y < height; ++y) {
		unsigned char *data1 = destinationImage.ptr<uchar>(y);
		const uchar * data = sourceImage.ptr<uchar>(y);
		for (int x = 0; x < width; ++x) {
			int H = *data; data++;
			int S = *data; data++;
			int V = *data; data++;

			cout << H << " " << S << " " << V << endl;
			float V1 = S*cos(H);
			float V2 = S*sin(H);

			int b = round(m[0][0] * V + m[1][0] * V1 + m[2][0] * V2);
			int g = round(m[0][1] * V + m[1][1] * V1 + m[2][1] * V2);
			int r = round(m[0][2] * V + m[1][2] * V1 + m[2][2] * V2);

			*data1 = b; ++data1;
			*data1 = g; ++data1;
			*data1 = r; ++data1;
		}
	}

	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", destinationImage);
	waitKey(0);
}

const float cr = 0.299, cg = 0.587, cb = 0.114;
// công thức chuyển một rgb -> gray
int formular1(int r, int g, int b) {
	float fr = r / 255.0;
	float fg = g / 255.0;
	float fb = b / 255.0;
	return (int)((cr  * fr + cg * fg + cb * fb)*255.0);
}

int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage){
	if (!sourceImage.data) {
		return 0;
	}

	if (IsGrayScale(sourceImage)) {
		return 0;
	}

	int width = sourceImage.rows;
	int height = sourceImage.cols;
	destinationImage.create(height, width, CV_8UC1);

	for (int y = 0; y < width; ++y) {
		const unsigned char* data = sourceImage.ptr<uchar>(y);
		unsigned char *data1 = destinationImage.ptr<uchar>(y);
		for (int x = 0; x < height; ++x) {
			*data1 = formular1(*data++, *data++, *data++); data1++;
		}
	}

	imwrite("GrayImage.jpg", destinationImage);
	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", destinationImage);
	waitKey(0);
	return 1;
}


Converter::Converter() {

}

Converter::~Converter() {

}