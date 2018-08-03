#ifndef _DNLS_H_
#define _DNLD_H_

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

vector<Mat> getCharsFromHorizontalVerticalSeg(Mat candImg, int hbbw = 0, int vbbw = 0);

vector<Mat> getCharsFromVerticalSeg(Mat candImg, int hbbw = 0, int vbbw = 0);

Mat digitsProcessCandidateRegion(Mat &img);

#endif