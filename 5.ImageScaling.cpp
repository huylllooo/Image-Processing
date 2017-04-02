#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;

// 5-1 IMAGE ENLARGING

int main(int argc, char** argv)
{
	// Print out usage
	if (argc != 4) {
		cout << "Usage: " << argv[0] << "[input] [output1] [output2]" << endl;
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
	Mat channel[3], outChannel[3];
	split(in, channel);

	// Create output image (8 bit depth, 3 channels)
	Mat output(in.rows * 2, in.cols * 2, CV_8UC3);
	split(output, outChannel);

	// Reduce the colorspace
	for (int y = in.rows-1; y > 0; y--) {
		for (int x = in.cols-1; x > 0 ; x--) {
			outChannel[0].at<uchar>(y * 2, x * 2) = channel[0].at<uchar>(y, x);
			outChannel[1].at<uchar>(y * 2, x * 2) = channel[1].at<uchar>(y, x);
			outChannel[2].at<uchar>(y * 2, x * 2) = channel[2].at<uchar>(y, x);
		}
	}
	
	for (int y = 0; y < output.rows -1; y+=2) {
		for (int x = 0; x < output.cols-1; x+=2) {
			// Pixel at x+1,y
			outChannel[0].at<uchar>(y, x + 1) = outChannel[0].at<uchar>(y, x);
			outChannel[1].at<uchar>(y, x + 1) = outChannel[1].at<uchar>(y, x);
			outChannel[2].at<uchar>(y, x + 1) = outChannel[2].at<uchar>(y, x);

			// Pixel at x,y+1
			outChannel[0].at<uchar>(y + 1, x) = outChannel[0].at<uchar>(y, x);
			outChannel[1].at<uchar>(y + 1, x) = outChannel[1].at<uchar>(y, x);
			outChannel[2].at<uchar>(y + 1, x) = outChannel[2].at<uchar>(y, x);

			// Pixel at x+1,y+1
			outChannel[0].at<uchar>(y + 1, x + 1) = outChannel[0].at<uchar>(y, x);
			outChannel[1].at<uchar>(y + 1, x + 1) = outChannel[1].at<uchar>(y, x);
			outChannel[2].at<uchar>(y + 1, x + 1) = outChannel[2].at<uchar>(y, x);
		}
	}

	// Merge 3 channels
	merge(outChannel, 3, output);
	// Display output image
	imshow("Output1", output);

	// Specify compression quality
	vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(100);

	// Write img to file
	bool saveImg = imwrite(argv[2], output, params);

	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}

	/*-----------------OUTPUT 2------------------*/
	split(output, outChannel);

	for (int y = 0; y < output.rows - 1; y += 2) {
		for (int x = 1; x < output.cols; x += 2) {
			if (x == output.cols - 1) {
				outChannel[0].at<uchar>(y, x) = outChannel[0].at<uchar>(y, x-1);
				outChannel[1].at<uchar>(y, x) = outChannel[1].at<uchar>(y, x-1);
				outChannel[2].at<uchar>(y, x) = outChannel[2].at<uchar>(y, x-1);
			}
			else {
				outChannel[0].at<uchar>(y, x) = outChannel[0].at<uchar>(y, x + 1) / 2 + outChannel[0].at<uchar>(y, x - 1) / 2;
				outChannel[1].at<uchar>(y, x) = outChannel[1].at<uchar>(y, x + 1) / 2 + outChannel[1].at<uchar>(y, x - 1) / 2;
				outChannel[2].at<uchar>(y, x) = outChannel[2].at<uchar>(y, x + 1) / 2 + outChannel[2].at<uchar>(y, x - 1) / 2;
			}
		}
	}
	for (int x = 0; x < output.cols - 1; x += 2) {
		for (int y = 1; y < output.rows; y += 2) {
			if (y == output.rows - 1) {
				outChannel[0].at<uchar>(y, x) = outChannel[0].at<uchar>(y - 1, x);
				outChannel[1].at<uchar>(y, x) = outChannel[1].at<uchar>(y - 1, x);
				outChannel[2].at<uchar>(y, x) = outChannel[2].at<uchar>(y - 1, x);
			}
			else {
				outChannel[0].at<uchar>(y, x) = outChannel[0].at<uchar>(y + 1, x) / 2 + outChannel[0].at<uchar>(y - 1, x) / 2;
				outChannel[1].at<uchar>(y, x) = outChannel[1].at<uchar>(y + 1, x) / 2 + outChannel[1].at<uchar>(y - 1, x) / 2;
				outChannel[2].at<uchar>(y, x) = outChannel[2].at<uchar>(y + 1, x) / 2 + outChannel[2].at<uchar>(y - 1, x) / 2;
			}
		}
	}

	// Merge 3 channels
	merge(outChannel, 3, output);
	// Display output image
	imshow("Output2", output);

	// Specify compression quality
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(100);


	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}
	waitKey(0);
	//destroyAllWindows;
	return 0;
}