#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace cv;
using namespace std;

// 4 HISTOGRAM INTERSECTION

// Calculate histogram of a single image, save result into array h
void calculateHis(Mat img, unsigned __int16 h[]) {
	Mat channel[3];
	split(img, channel);
	int r, g, b;

	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			r = (int)channel[2].at<uchar>(y, x);
			g = (int)channel[1].at<uchar>(y, x);
			b = (int)channel[0].at<uchar>(y, x);

			// Red
			if (r > 191)
				h[3]++;
			else if (r > 127)
				h[2]++;
			else if (r > 63)
				h[1]++;
			else if (r > 0)
				h[0]++;
			// Green
			if (g > 191)
				h[7]++;
			else if (g > 127)
				h[6]++;
			else if (g > 63)
				h[5]++;
			else if (g > 0)
				h[4]++;
			//Blue
			if (b > 191)
				h[11]++;
			else if (b > 127)
				h[10]++;
			else if (b > 63)
				h[9]++;
			else if (b > 0)
				h[8]++;
		}
	}
}

// Calculate D
unsigned __int32 calculateD(unsigned __int16 a[], unsigned __int16 b[]) {
	unsigned __int32 sum = 0;
	for (int i = 0; i < 12; i++) {
		sum += (a[i] < b[i]) ? a[i] : b[i];
	}
	return sum;
}

int main(int argc, char** argv)
{
	// Print out usage
	if (argc != 4) {
		cout << "Usage: " << argv[0] << "[input] [output]" << endl;
		return 0;
	}

	// 2D array to save histogram data of all 15 images
	unsigned __int16 CI[15][12];
	for (int x = 0; x < 15; x++) {
		for (int y = 0; y < 12; y++) {
			CI[x][y] = 0;
		}
	}
	// Read the image data and store it in 'in'
	for (int x = 0; x < 15; x++) {
		Mat in = imread("img/"+to_string(x)+".jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
		// Check whether the image is loaded or not
		if (in.empty()) {
			cout << "Error: Image cannot be loaded!" << endl;
			return -1;
		}
		calculateHis(in, CI[x]);
	}

	int max[2]; // store index of output images
	max[0] = 0;
	max[1] = 1;

	unsigned __int32 D_list[15]; // Store D of each image

	int input = atoi(argv[1]);
	D_list[input] = 0;
	for (int x = 0; x < 15; x++) {
		if (x != input)
		D_list[x] = calculateD(CI[input], CI[x]);
	}

	// max[0] has the biggest value of D, then max[1]
	if (D_list[max[0]] < D_list[max[1]]) {
		max[0] = 1;
		max[1] = 0;
	}
	// Find 2 images that have the biggest D
	for (int x = 2; x < 15; x++) {
		if (D_list[x] >= D_list[max[0]]) {
			max[1] = max[0]; // replace current 2nd max by 1st max
			max[0] = x;		// save new max
		}
		else if (D_list[x] >= D_list[max[1]]) {
			max[1] = x; // replace current 2nd max by new 2nd max
		}
	}
	// Load output image and display
	Mat out0 = imread("img/" + to_string(max[0]) + ".jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	Mat out1 = imread("img/" + to_string(max[1]) + ".jpg", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);


	imshow("Output Image 1", out0);

	imshow("Output Image 2", out1);

	waitKey(0);
	destroyAllWindows;
	return 0;
}