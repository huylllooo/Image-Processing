#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;

// 1-1 GRAYSCALE CONVERSION

int main(int argc, char** argv)
{
	// Print out usage
	if (argc != 3) {
		cout << "Usage: " << argv[0] << "[input] [output]" << endl;
		return 0;
	}

	// Read the image data and store it in 'in'
	Mat in = imread(argv[1], CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_ANYCOLOR);

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

	// Create output image (8 bit depth, 1 channel(grayscale))
	// 0.299 * R + 0.587 * G + 0.114 * B
	Mat output(in.rows, in.cols, CV_8UC1);
	output = 0.299 * channel[2] + 0.587 * channel[1] + 0.114 * channel[0];

	// Display output image
	imshow("Grayscale", output);

	// Specify compression quality
	vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(100);

	// Write img to file
	bool saveImg = imwrite(argv[2], output, params);

	if (!saveImg)	{
		cout << "ERROR : Failed to save the image" << endl;
	}

	waitKey(0);
	destroyAllWindows;
	return 0;
}