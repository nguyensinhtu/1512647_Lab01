#include "ColorTransformer.h"

// chuẩn hóa giá trị pixel về đoạn [0, 255]
int normalize(int b) {
	if (b > 255)
		return 255;
	if (b < 0)
		return 0;
	return b;
}

// chuyển đổi giá trị từ đoạn [a, b] sang đoạn [c, d]
float mapping(float x, float b, float d) {
	return x*(b / d);
}

// kiểm tra một ảnh có phải là ảnh xám
// input : ảnh cần kiểm tra
// output : trả về true nếu là ảnh xám
//			trả về false ngược lại
bool isGrayScale(const Mat &image) {
	if (image.type() == CV_8UC1)
		return true;
	return false;
}


// tìm giá trị lớn nhất trong ma trận 
float FindMax(const Mat& arr) {
	int width = arr.cols;
	int height = arr.rows;

	float Max = 0;
	for (int y = 0; y < height; ++y) {
		const float *data = arr.ptr<float>(y);
		for (int x = 0; x < width; ++x) {
			Max = max((float)(*data), Max); data++;
		}
	}
	return Max;
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

	imwrite("contrast.jpg", destinationImage);
	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", destinationImage);
	waitKey(0);
	return 1;
}


int ColorTransformer::CalcHistogram(const Mat& sourceImage, Mat& histogram) {
	int width = sourceImage.cols;
	int height = sourceImage.rows;

	if (!sourceImage.data) {
		return 0;
	}

	int k = 0;
	if (isGrayScale(sourceImage)) {
		for (int y = 0; y < height; ++y) {
			const unsigned char* data = sourceImage.ptr<uchar>(y);
			float *data2 = histogram.ptr<float>(0);
			for (int x = 0; x < width; ++x) {
				//cout << (int)*data << endl;
				k++;
				//histogram.at<float>(0, (int)*data) += 1; data++;
				*(data2 + (int)*data) += 1; data++;
			}
		}
		cout << "k : " << k << endl;
	}
	else {
		for (int y = 0; y < height; ++y) {
			const unsigned char* data = sourceImage.ptr<uchar>(y);
			float *b = histogram.ptr<float>(0);
			float *g = histogram.ptr<float>(1);
			float *r = histogram.ptr<float>(2);
			for (int x = 0; x < width; ++x) {
				*(b + (int)(*data)) += 1; data++;
				*(g + (int)(*data)) += 1; data++;
				*(r + (int)(*data)) += 1; data++;
			}
		}
	}
	return 1;
}


int ColorTransformer::DrawHistogram(const Mat& sourceImage, Mat& histImage) {

	if (!sourceImage.data)
		return 0;

	if (isGrayScale(sourceImage)) {
		Mat histogram(1, 256, CV_32FC1, Scalar::all(0));
		CalcHistogram(sourceImage, histogram);


		float Max = FindMax(histogram);
		float b = 500;
		float size = min(Max, 700.0f);
		
		Mat tmp(size, 300, CV_8UC1, Scalar::all(255));
		int width = histogram.cols;

		for (int x = 0; x < width; ++x) {
			line(tmp, Point(x + 1 + 10, size - 50), Point(x + 1 + 10, size - mapping((float)histogram.at<float>(0, x), b, Max) - 50), Scalar(0, 0, 0));
		}
		histImage = tmp.clone();
		tmp.release();
	}
	else {
		Mat histogram(3, 256, CV_32FC1, Scalar::all(0));
		CalcHistogram(sourceImage, histogram);

		float Max = FindMax(histogram);
		float b = 200;
		float size = min(Max, 300.0f);
		
		Mat tmp(size*2, 700, CV_8UC3, Scalar::all(255));

		// draw blue
		for (int x = 0; x < 256; ++x) {
			line(tmp, Point(x + 50 + 1, size - 50), Point(x + 50 + 1, size - mapping((float)histogram.at<float>(0, x),b,  Max) - 50), Scalar(128, 0, 0));
		}

		// draw green
		for (int x = 0; x < 256; ++x) {
			line(tmp, Point(x + 356 + 1, size - 50), Point(x + 356 + 1, size - mapping((float)histogram.at<float>(1, x),b,  Max) - 50), Scalar(0, 128, 0));
		}

		// draw red
		for (int x = 0; x < 256; ++x) {
			line(tmp, Point(x + 150 + 1, size*2 - 50), Point(x + 150 + 1, size*2 - mapping((float)histogram.at<float>(2, x),b,  Max) - 50), Scalar(0, 0, 180));
		}
		histImage = tmp.clone();
		tmp.release();
	}

	imwrite("histogram.jpg", histImage);
	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", histImage);
	waitKey(0);
	return 1;
}

int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage) {
	if (!sourceImage.data) {
		return 0;
	}

	int height = sourceImage.rows;
	int width = sourceImage.cols;
	destinationImage.create(height, width, sourceImage.type());
	//destinationImage = sourceImage.clone();
	double NumOfPixel = width*height;
	
	if (isGrayScale(sourceImage)) {
		Mat histogram(1, 256, CV_32FC1, Scalar::all(0.0));
		CalcHistogram(sourceImage, histogram);
	
		// tính lược đồ xám tích lũy
		float T[256] = { 0 };
		T[0] = (float)(histogram.at<float>(0, 0) / NumOfPixel);
		for (int x = 1; x < 256; ++x) {
			T[x] = (float)((histogram).at<float>(0, x) / NumOfPixel) + T[x - 1];
			//cout << (double)((histogram).at<float>(0, x) / NumOfPixel) << " - " <<  T[x] << endl;
		}

		// tinh gia trị pixel cho ảnh output
		for (int y = 0; y < height; ++y) {
			const unsigned char *data = sourceImage.ptr<uchar>(y);
			unsigned char *data1 = destinationImage.ptr<uchar>(y);
			for (int x = 0; x < width; ++x) {
				
				*data1 = round(255 * T[(int)(*data)]); data1++; data++;
			}
		}
	}
	else {
		Mat histogram(3, 256, CV_32FC1, Scalar::all(0.0));
		CalcHistogram(sourceImage, histogram);
	
		float T[3][256] = { {0} };
		T[0][0] = (float)histogram.at<float>(0, 0) / NumOfPixel;
		T[1][0] = (float)histogram.at<float>(1, 0) / NumOfPixel;
		T[2][0] = (float)histogram.at<float>(2, 0) / NumOfPixel;

		// tính lược đồ xám tích lũy
		for (int i = 1; i < 256; ++i) {
			T[0][i] = (float)histogram.at<float>(0, i) / NumOfPixel + T[0][i - 1];
			T[1][i] = (float)histogram.at<float>(0, i) / NumOfPixel + T[1][i - 1];
			T[2][i] = (float)histogram.at<float>(0, i) / NumOfPixel + T[2][i - 1];
		}

		for (int y = 0; y < height; ++y){
			const unsigned char * data = sourceImage.ptr<uchar>(y);
			unsigned char *data1 = destinationImage.ptr<uchar>(y);
			for (int x = 0; x < width; ++x) {
				int b = round((float)(255 * T[0][(int)*data])); data++;
				int g = round((float)(255 * T[1][(int)*data])); data++;
				int r = round((float)(255 * T[2][(int)*data])); data++;

				*data1 = b; data1++;
				*data1 = g; data1++;
				*data1 = r; data1++;
			}
		}
	}

	imwrite("equalization_Image.jpg", destinationImage);
	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", destinationImage);
	waitKey(0);
	return 1;
}