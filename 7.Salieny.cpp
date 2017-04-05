#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;

// 7 SALIENCY
Mat calcSaliencyMap(const Mat image)
{

	const int NUM_SCALES = 7;
	vector<Mat> pyramidI(NUM_SCALES); //Luminance pyramid

	Mat scaled = image;
	for (int s = 0; s < NUM_SCALES; ++s) {

		// Luminance mapping
		vector<Mat > colors;
		split(scaled, colors);
		Mat  imageI = (colors[0] / 3 + colors[1] / 3 + colors[2] / 3) ;
		pyramidI[s] = imageI;
		pyrDown(scaled, scaled); // minimize image to 1/2
	}

	/* Calculate difference between 2 arbitrary luminance pyramid layers,
	*	save result to array pyramidD
	*	for every layer n, start from smallest layer to n+1 (0 is the biggest layer)
	*/
	vector<Mat> pyramidD(21);
	int k = 0;
	for (int s = 5; s >= 0; s--) {
		for (int i = 6; i > s; i--) {
			pyrUp(pyramidI[i], pyramidI[i]);		
			pyramidD[k++] = abs(pyramidI[s] - pyramidI[i]);
		}
	}

	// Scale up every layer in pyramidD
	int total = 20;
	for (int s = 6; s > 1; s--) {
		total -= s;
		for (int i = total; i >= 0; i--) {
			pyrUp(pyramidD[i], pyramidD[i]);
		}
	}

	return  pyramidD[0] + pyramidD[2] + pyramidD[5] + pyramidD[9] + pyramidD[14] + pyramidD[17];
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

	// Calculate saliency
	Mat saliency = calcSaliencyMap(in);
	imshow("Saliency", saliency);

	vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	params.push_back(100);

	// Write img to file
	bool saveImg = imwrite(argv[2], saliency, params);

	if (!saveImg) {
		cout << "ERROR : Failed to save the image" << endl;
	}

	waitKey(0);
	destroyAllWindows;
	return 0;
}