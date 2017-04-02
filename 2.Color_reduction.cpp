#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;

// 2 COLOR REDUCTION

// Calculate new pixel value
uchar colorReduce(uchar in) {
	if (in > 191)
		return 224;
	else if (in > 127)
		return 160;
	else if (in > 63)
		return 96;
	else
		return 32;
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

	// Create output image (8 bit depth, 3 channels)
	Mat output(in.rows, in.cols, CV_8UC3);

	// Reduce the colorspace
	for (int y = 0; y < in.rows; y++) {
		for (int x = 0; x < in.cols; x++) {
			channel[0].at<uchar>(y, x) = colorReduce(channel[0].at<uchar>(y, x));
			channel[1].at<uchar>(y, x) = colorReduce(channel[1].at<uchar>(y, x));
			channel[2].at<uchar>(y, x) = colorReduce(channel[2].at<uchar>(y, x));
		}
	}


	// Merge 3 channels
	merge(channel, 3, output);
	// Display output image
	imshow("Color Reduced", output);

	// Specify compression quality
	vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(100);

	// Write img to file
	bool saveImg = imwrite(argv[2], output, params);

	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}

	waitKey(0);
	destroyAllWindows;
	return 0;
}