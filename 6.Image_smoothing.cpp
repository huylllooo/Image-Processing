#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;

// 6 SMOOTH IMAGES

// Calculate PSNR
double getPSNR(const Mat& I1, const Mat& I2)
{
	Mat s1;
	absdiff(I1, I2, s1);       // |I1 - I2|
	s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
	s1 = s1.mul(s1);           // |I1 - I2|^2

	Scalar s = sum(s1);         // sum elements per channel

	double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

	if (sse <= 1e-10) // for small values return zero
		return 0;
	else
	{
		double  mse = sse / (double)(I1.channels() * I1.total());
		double psnr = 10.0*log10((255 * 255) / mse);
		return psnr;
	}
}

int main(int argc, char** argv)
{
	// Print out usage
	if (argc != 2) {
		cout << "Usage: " << argv[0] << "[input]" << endl;
		return 0;
	}

	// Read the image data and store it in 'in'
	Mat in = imread(argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

	// Check whether the image is loaded or not
	if (in.empty()) {
		cout << "Error: Image cannot be loaded!" << endl;
		return -1;
	}

	// Display the original image
	imshow("Original", in);

	// Specify compression quality
	vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(100);

	Mat dst;
	//char zBuffer[35];

	/*-----Gaussian Filter 5x5-----*/
	int i = 5;
	// Copy text tp buffer
	//_snprintf_s(zBuffer, 35, "Gaussian Filter: %d x %d", i, i);

	// smooth the img using Gaussian kernel and save it to "dst"
	GaussianBlur(in, dst, Size(i, i), 0, 0);

	// put the text in the 'zBuffer' to the 'dst' img
	// putText(dst, zBuffer, Point(in.cols / 8, in.rows / 8), CV_FONT_HERSHEY_PLAIN, 1, Scalar(222, 222, 222), 1);

	// show the blurred img
	imshow("Gaussian 5 x 5", dst);

	// Write img to file
	bool saveImg = imwrite("img_out/G-5.jpg", dst, params);

	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}
	// calculate PSNR
	cout << "Gaussian 5 x 5: " << getPSNR(in, dst) << endl;

	/*-----Median Filter 5x5-----*/
	medianBlur(in, dst, i);
	imshow("Median 5 x 5", dst);
	saveImg = imwrite("img_out/M-5.jpg", dst, params);
	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}
	// calculate PSNR
	cout << "Median 5 x 5: " << getPSNR(in, dst) << endl;

	/*-----Gaussian Filter 15x15-----*/
	i = 15;
	GaussianBlur(in, dst, Size(i, i), 0, 0);
	imshow("Gaussian 15 x 15", dst);
	saveImg = imwrite("img_out/G-15.jpg", dst, params);
	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}
	// calculate PSNR
	cout << "Gaussian 15 x 15: " << getPSNR(in, dst) << endl;

	/*-----Median Filter 15x15-----*/
	medianBlur(in, dst, i);
	imshow("Median 15 x 15", dst);
	saveImg = imwrite("img_out/M-15.jpg", dst, params);
	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}
	// calculate PSNR
	cout << "Median 15 x 15: " << getPSNR(in, dst) << endl;
	waitKey(0);
	destroyAllWindows;
	return 0;
}