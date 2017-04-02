#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;

// 8 BLUR BACKGROUND

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

	// define bounding rectangle (foreground)
	Rect rectangle(in.cols / 5, in.rows / 8, in.cols * 3 / 5, in.rows / 1.5);

	// segmentation
	Mat result;
	// the models
	Mat bgModel, fgModel;
	// GrabCut segmentation
	grabCut(in, result, rectangle, bgModel, fgModel, 5, GC_INIT_WITH_RECT);

	compare(result, GC_PR_FGD, result, CMP_EQ);

	// Create output image (8 bit depth, 3 channels)
	Mat background(in.rows, in.cols, CV_8UC3);
	GaussianBlur(in, background, Size(17, 17), 0, 0);

	in.copyTo(background, result);
	// Display output image
	imshow("Bokeh", background);

	// Specify compression quality
	vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(100);

	// Write img to file
	bool saveImg = imwrite(argv[2], background, params);

	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}

	waitKey(0);
	destroyAllWindows;
	return 0;
}