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
		cout << "Usage: " << argv[0] << "[input] [scale] [output]" << endl;
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

	int scale = atoi(argv[2]);
	// Create output image (8 bit depth, 3 channels)
	Mat output(in.rows / scale, in.cols / scale , CV_8UC3);
	split(output, outChannel);

	// Initialize output pixels
	for (int y = 0; y < output.rows; y += 1) {
		for (int x = 0; x < output.cols; x += 1) {
			int r=0, g=0, b=0;	// avoid overloading
			for (int i = 0; i < scale; i++) {
				for (int j = 0; j < scale; j++) {
					b += channel[0].at<uchar>(y * scale + i, x * scale + j);
					g += channel[1].at<uchar>(y * scale + i, x * scale + j);
					r += channel[2].at<uchar>(y * scale + i, x * scale + j);
				}
			}
			outChannel[0].at<uchar>(y, x) = b / scale / scale;
			outChannel[1].at<uchar>(y, x) = g / scale / scale;
			outChannel[2].at<uchar>(y, x) = r / scale / scale;
			
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
	bool saveImg = imwrite(argv[3], output, params);

	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}

	waitKey(0);
	//destroyAllWindows;
	return 0;
}