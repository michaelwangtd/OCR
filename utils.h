#ifndef _UTILS_H_
#define _UTILS_H_

#include<opencv.hpp>

using namespace cv;

void show_img(Mat &img);

void size_img(Mat &img);

int writeData(string fileName, Mat& matData);

void mergeImg(Mat & dst, Mat &src1, Mat &src2);

Mat imgClipper(Mat img, int x0, int y0, int height, int width);

void getFileNames(string path, vector<string>&  files);

void getDirNames(string path, vector<string>&  files);


#endif