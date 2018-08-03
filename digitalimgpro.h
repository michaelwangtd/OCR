#ifndef _DIGITALIMGPRO_H_
#define _DIGITALIMGPRO_H_

#include <opencv2\opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;


Mat pixelFilp(Mat img, int &height, int &width);

void horizontalProjection(Mat &img, int *&p, int &height, int &width);
Mat horizontalClip(Mat &img, int *&hp, int &height, int &width, int &bw);

void verticalProjection(Mat &img, int *&vp, int &height, int &width);
void verticalClip(vector<Mat> &chars, Mat &img, int *&vp, int &height, int &width);

double calcDegree(Mat &src, int flag = 0);

void rotateImage(Mat &src, Mat &dst, double &degree);

Mat resizeImg(Mat &img, int height, int width);

Mat imgProcess(string &fpath);

Mat localCandidateRegion(string &fpath);

Mat filterGrayThresholdShape(Mat &img);

Mat candidateRegion(Mat &img, int xcn, int ycn, int np);

Mat targetRegion(Mat &img, int bw);

Mat origin2TargetImg(Mat oimg);

#endif