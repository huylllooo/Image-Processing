#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;

// 1-2 Binarization

int main(int argc, char** argv)
{
	// Print out usage
	if (argc != 4) {
		cout << "Usage: " << argv[0] << "[input] [output] [threshold]" << endl;
		return 0;
	}

	uchar pivot = atoi(argv[3]);

	// Read the image data and store it in 'in', 
	// grayscale: image depth = 8 bits, 1 channel
	Mat in = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	// Check whether the image is loaded or not
	if (in.empty()) {
		cout << "Error: Image cannot be loaded!" << endl;
		return -1;
	}

	// Display the original grayscale image
	imshow("Original", in);

	// Calculate new pixel value based on threshold 'pivot'
	for (int y = 0; y < in.rows; y++) {
		for (int x = 0; x < in.cols; x++) {
			if (in.at<uchar>(y, x) < pivot)
				in.at<uchar>(y, x) = 0;
			else
				in.at<uchar>(y, x) = 255;
		}
	}

	// Display output image
	imshow("Grayscale", in);

	// Specify compression quality
	vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(100);

	// Write img to file
	bool saveImg = imwrite(argv[2], in, params);

	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}

	waitKey(0);
	destroyAllWindows;
	return 0;
}