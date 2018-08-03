#ifndef _ALG_H_
#define _ALG_H_

#include<iostream>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

void knnPrepareTrainPredict();

Ptr<KNearest> KNN(Mat &x_train, Mat &y_train, int k = 1);

#endif