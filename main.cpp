#include <iostream>
#include <opencv2\opencv.hpp>
#include <string>
#include <cmath>
using namespace std;
using namespace cv;

#include"test.h"
#include"utils.h"
#include"camera.h"
#include"alg.h"
#include "digitalimgpro.h"
#include "dnls.h"
#include "process.h"
#include "controller.h"


void charSegment(vector<Mat> &vec, Mat &img) {
	/*
		进行垂直投影后再进行字符分割
		imgClipper()中x0和height数值是固定的
		默认之前步骤进行了水平分割
	*/

	int height = img.rows, width = img.cols*img.channels();

	//定义垂直投影数组指针
	int *vp = new int[width];
	memset(vp, 0, width * 4);

	verticalProjection(img, vp, height, width);

	int startp = 0, endp = 0;
	bool inRegion = false;

	int dist;
	Mat block;
	int x0 = 0;
	for (int i = 0; i < width;i++) {
		if (!inRegion && *(vp+i) != 0) {
			inRegion = true;
			startp = i;
		}
		if (*(vp+i) == 0 && inRegion) {
			inRegion = false;
			endp = i;
		}
		//在进行字符分割前进行判断
		if (startp < endp && !inRegion) {
			dist = endp - startp;

			if (endp = width - 1 && dist < 3) {
				continue;
			}

			cout << "x0:" << x0 << " y0:" << startp << " height:" << height << " width:" << dist << endl;
			block = imgClipper(img, x0, startp, height, dist);
			//show_img(block);
			vec.push_back(block);
		}
	}
}


//定义全局变量的文件目录路径
string fpath = "X:/cv/color/xxx.png";

int main() {

	/*
		1 字符去噪、分割
	*/
	//Mat img;
	//vector<Mat> chrVec;

	//img = imgProcess(fpath);
	//
	////字符分割
	//charSegment(chrVec,img);

	//for (int i = 0; i < chrVec.size();i++) {
	//	show_img(chrVec.at(i));
	//	imwrite(odir + to_string(i) + ".jpg", chrVec.at(i));
	//}


	/*
		2 利用模型识别字符opencv手写字符
	*/
	//knnPrepareTrainPredict();


	/*
		3 倾斜矫正（霍夫变换）
	*/
	/*Mat img = imread(fpath);
	Mat rstImg = tiltBalanceHough(img);*/


	/*
		4 将target region 10位字母数字分割成单个字符
		  生成模型训练数据
		  后续进行标记
	*/
	//targetRegion2SingleCharsProcess();


	/*
		生成训练数据
	*/
	//createTrainData();


	/*
	获取训练数据进行算法精度测试
	*/
	//getTestDataAndAccuracyTest();

	//getWholePicAndAccuracyTest();

	return 0;
}




