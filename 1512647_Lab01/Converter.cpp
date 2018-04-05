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

// matrix transform sử dụng cho việc chuyển đổi hsv sang rgb và ngược lại
float m[3][3] = { { sqrt(3) / 3, sqrt(3) / 3 , sqrt(3) / 3 },
					{ 0, 1 / sqrt(2), -1 / sqrt(2) },
					{ 2 / sqrt(6), -1 / sqrt(6), -1 / sqrt(6) } };


/*
Hàm chuyển đổi không gian màu của ảnh từ GrayScale sang RGB
sourceImage: ảnh input
destinationImage: ảnh output
Hàm trả về
0: nếu chuyển thành công
1: nếu chuyển thất bại (không đọc được ảnh input,...)
*/
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

/*
Hàm chuyển đổi không gian màu của ảnh
sourceImage: ảnh input
destinationImage: ảnh output cùng kích thước, cùng loại với ảnh input
type: loại chuyển đổi
0,1: chuyển từ RGB sang GrayScale và ngược lại
2,3: chuyển từ RGB sang HSV và ngược lại
Hàm trả về
0: nếu chuyển thành công
1: nếu chuyển thất bại (không đọc được ảnh input hay type không chính xác,...)
*/
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


/*
Hàm chuyển đổi không gian màu của ảnh từ RGB sang HSV
sourceImage: ảnh input
destinationImage: ảnh output
Hàm trả về
0: nếu chuyển thành công
1: nếu chuyển thất bại (không đọc được ảnh input,...)
*/
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

			float b = (float)(*data / 255.0);  data++;
			float g = (float)(*data / 255.0);  data++;
			float r = (float)(*data / 255.0);  data++;


			float cmax = max(max(b, g), r);
			float cmin = min(min(g, b), r);
			float delta = cmax - cmin;

			float tmp = 0;
			float h, s, v;
			v = cmax;
			if (delta < 0.0000001)
				return 0; // loi

			if (cmax > 0) {
				s = delta / cmax;
			}
			else {
				s = 0.0;
				return 0; // loi
			}

			if (r >= cmax) {
				h = (g - b) / delta;
			}
			else if (g >= cmax) {
				h = 2.0 + (b - r) / delta;
			}else
				h = 4.0 + (r - g) / delta;

			h *= 60;

			if (h < 0.0)
				h += 360;

			data1[0] = (uchar)((float)(h / 360) * 100);
			data1[1] = (uchar)(s * 100);
			data1[2] = (uchar)(v * 100);

			//cout << h << " " << s*100 << " " << v << endl;
			data1 += 3;
			}
	}

	imwrite("hsvImage.jpg", destinationImage);
	cout << "ket qua duoc luu o file hsvImage.jpg" << endl;
	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", sourceImage);
	waitKey(0);
	return 1;

}

/*
Hàm chuyển đổi không gian màu của ảnh từ HSV sang RGB
sourceImage: ảnh input
destinationImage: ảnh output
Hàm trả về
0: nếu chuyển thành công
1: nếu chuyển thất bại (không đọc được ảnh input,...)
*/
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

			float h = (float)(*data/100.0); data++;
			float s = (float)(*data / 100.0); data++;
			float v = (float)(*data/100.0);  data++;
			h = h*360.0;
		
			if (s <= 0.0) {
				data1[0] = (uchar)v;
				data1[1] = (uchar)v;
				data1[1] = (uchar)v;
				data1 += 3;
				continue;
			}

			int r, g, b;
			float hh, p, q, t, ff;
			int i;
			hh = h;
			if (hh >= 360.0) hh = 0.0;
			hh /= 60.0;
			i = (int)hh;
			ff = hh - i;
			p = v * (1.0 - s);
			q = v * (1.0 - (s * ff));
			t = v * (1.0 - (s * (1.0 - ff)));

			p *= 255;
			q *= 255;
			t *= 255;
			v *= 255;
			switch (i) {
			case 0: r = v; g = t; b = p;
				break;
			case 1: r = q; g = v; b = p;
				break;
			case 2: r = p; g = v;b = t;
				break;
			case 3:r = p;g = q;b = v;
				break;
			case 4:r = t;g = p;b = v;
				break;
			case 5:
			default:r = v;g = p;b = q;
				break;
		
			}

			data1[0] = b;
			data1[1] = g;
			data1[2] = r;
	
			data1 += 3;
		}
	}

	namedWindow("show", WINDOW_AUTOSIZE);
	imshow("show", destinationImage);
	waitKey(0);
	return 1;
}

const float cr = 0.299, cg = 0.587, cb = 0.114;
// công thức chuyển một rgb -> gray
// input : giá trị r, g, b của pixel
// ouput : giá trị xám
int formular1(int r, int g, int b) {
	float fr = r / 255.0;
	float fg = g / 255.0;
	float fb = b / 255.0;
	return (int)((cr  * fr + cg * fg + cb * fb)*255.0);
}

/*
Hàm chuyển đổi không gian màu của ảnh từ RGB sang GrayScale
sourceImage: ảnh input
destinationImage: ảnh output
Hàm trả về
0: nếu chuyển thành công
1: nếu chuyển thất bại (không đọc được ảnh input,...)
*/
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