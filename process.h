#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

void targetRegion2SingleCharsProcess();

vector<Mat> createTrainData();

void knnModel(Ptr<KNearest> &knn);

void getTestDataAndAccuracyTest();

void getWholePicAndAccuracyTest();

void getTransMap(map<int, string> &trans);


#endif