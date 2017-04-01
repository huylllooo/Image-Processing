#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
using namespace cv;
using namespace std;

// 1-3 HSV CONVERSION
/* HSV to RGB
	http://www.rapidtables.com/convert/color/hsv-to-rgb.htm
*/
// Find maximum
uchar findMax(uchar a, uchar b, uchar c) {
	uchar max = a;
	if (b > max)
		max = b;
	if (c > max)
		max = c;
	return max;
}

// Find minimum
uchar findMin(uchar a, uchar b, uchar c) {
	int min = a;
	if (b < min)
		min = b;
	if (c < min)
		min = c;
	return min;
}

// Calculate Hue
int Hue(uchar r, uchar g, uchar b, uchar max, uchar min) {
	int h;
	if (max == min)
		h = 0;
	else if (r == max)
		h =60 * (g - b) / (max - min);
	else if (g == max)
		h = 60 * (2 + (b - r) / (max - min));
	else if (b == max)
		h = 60 * (4 + (r - g) / (max - min));

	return h % 360;
}

// Calculate saturation
double Sat(uchar max, uchar min) {
	if (max == min)
		return 0;
	else
		return (double)(max - min) / max;
}

int main(int argc, char** argv)
{
	// Print out usage
	if (argc != 3) {
		cout << "Usage: " << argv[0] << "[input] [output]" << endl;
		waitKey(0);
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
	uchar R, G, B, max, min;
	int H;
	double S, V;
	double C, X, m, r, g, b;
	// Create output image (8 bit depth, 3 channel)
	Mat output(in.rows, in.cols, CV_8UC3);

	for (int y = 0; y < in.rows; y++) {
		for (int x = 0; x < in.cols; x++) {
			B = channel[0].at<uchar>(y, x);
			G = channel[1].at<uchar>(y, x);
			R = channel[2].at<uchar>(y, x);
			max = findMax(R, G, B);
			min = findMin(R, G, B);

			// Calculate HSV
			H = Hue(R, G, B, max, min);
			S = Sat(max, min);
			V = max * 1.0 / 255;
			// Add 180 to Hue
			if (H > 180)
				H -= 180;
			else
				H += 180;

			// Convert back to RGB
			C = V * S;
			m = V - C;
			X = C * (1 - abs((H / 60) % 2 - 1));
			if (H < 60) {
				r = C;
				g = X;
				b = 0;
			}
			else if (H < 120) {
				r = X;
				g = C;
				b = 0;
			}
			else if (H < 180) {
				r = 0;
				g = C;
				b = X;
			}
			else if (H < 240) {
				r = 0;
				g = X;
				b = C;
			}
			else if (H < 300) {
				r = X;
				g = 0;
				b = C;
			}
			else if (H < 360) {
				r = C;
				g = 0;
				b = X;
			}
			channel[0].at<uchar>(y, x) = (b + m) * 255;
			channel[1].at<uchar>(y, x) = (g + m) * 255;
			channel[2].at<uchar>(y, x) = (r + m) * 255;
		}
	}
	// Merge 3 channels
	merge(channel, 3, output);
	// Display output image
	imshow("HSV", output);

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
	//destroyAllWindows;
	return 0;
}