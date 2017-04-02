#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <math.h>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;

// 3 HISTOGRAM

unsigned __int32 su(unsigned __int16 i[]) {
	unsigned __int32 ddd = 0;
	for (int j = 0; j < 12; j++) {
		ddd += i[j];
	}
	return ddd;
}
int main(int argc, char** argv)
{
	// Print out usage
	if (argc != 3) {
		cout << "Usage: " << argv[0] << "[input] [output]" << endl;
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

	// Split input image into 3 channels
	Mat channel[3];
	split(in, channel);

	unsigned __int16 i[12];
	for (int n = 0; n < 12; n++) {
		i[n] = 0;
	}
	int r, g, b;
	// Reduce the colorspace
	for (int y = 0; y < in.rows; y++) {
		for (int x = 0; x < in.cols; x++) {
			r = (int) channel[2].at<uchar>(y, x);
			g = (int) channel[1].at<uchar>(y, x);
			b = (int) channel[0].at<uchar>(y, x);

			//cout << r << " " << g << " " << b << endl;
			if (r > 191)
				i[3]++;
			else if (r > 127)
				i[2]++;
			else if (r > 63)
				i[1]++;
			else if (r > 0)
				i[0]++;

			if (g > 191)
				i[7]++;
			else if (g > 127)
				i[6]++;
			else if (g > 63)
				i[5]++;
			else if (g > 0)
				i[4]++;

			if (b > 191)
				i[11]++;
			else if (b > 127)
				i[10]++;
			else if (b > 63)
				i[9]++;
			else if (b > 0)
				i[8]++;
			
			
		}
	}
	cout << su(i) << " "<<in.cols * in.rows;

	ofstream test;
	test.clear();
	test.open("histogram.dat", ios::out);
	for (int k = 0; k < 12; k++) {
		test << (int)k << " " << i[k] << endl;
	}
	test.close();

	waitKey(0);
	destroyAllWindows;
	return 0;
}