#include<iostream>
#include<string>
#include<opencv2\opencv.hpp>
#include<vector>
#include<cv.h>
#include<core/core.hpp>    
#include<highgui/highgui.hpp> 
#include <fstream>
using namespace std;
using namespace cv;

#include "utils.h"
#include "dnls.h"
#include "digitalimgpro.h"
#include "controller.h"
#include "process.h"

string test_odir = "E:/cv/color/rst/";
string ttpath = "E:/cv/color/test/xxx.png";

void test_new_imgprocess() {
	string fpath = "E:/cv/color/test/xxx.png";
	Mat img, bin_img, cand_img, tar_img;

	//读取图片
	img = imread(fpath);
	show_img(img);
	
	//去噪处理
	bin_img = filterGrayThresholdShape(img);
	show_img(bin_img);
	//候选区域裁剪
	cand_img = candidateRegion(bin_img, 3, 4, 2);
	show_img(cand_img);
	//投影直方图分割目标区域
	tar_img = targetRegion(cand_img, 2);
	show_img(tar_img);
}


void test_imgopen_size() {
	string fpath = "E:/cv/color/test/xxx.png";
	Mat img = imread(fpath);
	cout << "img size: " << img.size() << endl;
}


void test_getTransMap() {
	map<int, string> trans;
	getTransMap(trans);
	cout << "trans size: " << trans.size() << endl;
	cout << trans[10] << endl;
}


void test_getFileNames() {
	string fdir = "E:/cv/color/test/xxx.png";
	vector<string> names;
	getFileNames(fdir, names);
	for (int i = 0; i < names.size(); i++) {
		cout << names.at(i) << endl;
	}
}


void test_getDirNames() {
	string fdir = "E:/cv/color/test/xxx.png";
	vector<string> names;
	getDirNames(fdir, names);
	cout << names.size() << endl;
	for (int i = 0; i < names.size(); i++) {
		cout << names.at(i) << endl;
	}
}

void test_getFilesRecursion() {
	char * indir = "E:/cv/color/test/xxx.png";
	vector<string> files;

	//getFilesRecursion(indir, files);
	getFileNames(indir, files);
	
	//getFiles(indir, files);
	cout << files.size() << endl;
	for (int i = 0; i < files.size();i++) {
		cout << files.at(i) << endl;
	}
}


void test_tiltBalanceHough() {
	Mat img, dst;
	Mat rst;
	img = imread(ttpath);
	dst = tiltBalanceHough(img,1);
	show_img(dst);
}



void test_resize() {
	Mat img, dst;
	Mat rst;
	img = imread(ttpath);
	show_img(img);
	dst = resizeImg(img, 110,64);
	show_img(dst);
	mergeImg(rst, img, dst);
	show_img(rst);
}



void test_degree() {
	Mat img, dst;
	
	img = imread(ttpath);
	show_img(img);
	double degree = calcDegree(img,1);
	cout << "degree: " << degree << endl;

	rotateImage(img, dst, degree);
	show_img(dst);

}



void test_denoiselocationseg(){
	Mat img;
	vector<Mat> chars;

	img = imread(ttpath);
	img = digitsProcessCandidateRegion(img);
	chars = getCharsFromHorizontalVerticalSeg(img, 1, 0);

	//show_img(img);
}


void test_temp1() {
	cout << "ans: " << sizeof(char*) << endl;
}


void test_binProcess() {
	Mat img, gray_img, bin_img;
	string ttpath = "E:/cv/color/test/xxx.png";
	img = imread(ttpath);

	cvtColor(img, gray_img, COLOR_BGR2GRAY);

	double threval = threshold(gray_img, bin_img, 0, 255, CV_THRESH_OTSU);

	show_img(gray_img);
}


void test_Mat_reshape_pushback() {
	Mat rst;
	Mat r1,r2;
	Mat m1(2, 3, CV_8UC1, 1);
	r1 = m1.reshape(0, 1);
	Mat m2(2, 3, CV_8UC1, 2);
	r2 = m2.reshape(0, 1);
	rst.push_back(r1);
	rst.push_back(r2);
	cout << rst << endl;
}


void test_noisePixels() {
	Mat img, gray_img, bin_img;
	img = imread(ttpath);
	show_img(img);
	cvtColor(img, gray_img, CV_BGR2GRAY);
	threshold(gray_img, bin_img, 0, 255, CV_THRESH_OTSU);
	writeData(test_odir + "test_bin.txt", bin_img);
	show_img(bin_img);
}


void test_changeImg() {
	Mat img, gray_img, bin_img;
	Mat rst;

	img = imread(ttpath);
	cout << "RGB channels:" << img.channels() << endl;

	cvtColor(img, gray_img, COLOR_BGR2GRAY);
	cout << "Gray channels:" << gray_img.channels() << endl;

	double threval = threshold(gray_img, bin_img, 0, 255, CV_THRESH_OTSU);
	cout << "Bin channels:" << bin_img.channels() << endl;
	cout << "thre value:" << threval << endl;

	cout << "height:" << bin_img.rows << endl;
	cout << "width:" << bin_img.cols << endl;

	int height, width;
	height = bin_img.rows;
	//width = bin_img.cols;
	width = bin_img.cols*bin_img.channels();

	Mat changed_img = bin_img.clone();
	for (int i = 0; i < height;i++) {
		for (int j = 0; j < width;j++) {
			changed_img.at<uchar>(i, j) = 255 - changed_img.at<uchar>(i, j);
		}
	}
	
	mergeImg(rst, bin_img, changed_img);
	show_img(rst);
}


void foreachImg() {
	string tpath = "E:/cv/color/test/xxx.png";
	Mat img = imread(tpath);

	cout << img.size() << endl;
	cout << img.rows << endl;
	cout << img.cols << endl;

	int n;
	for (int i = 0; i < img.rows;i++) {
		for (int j = 0; j < img.cols;j++) {
			n = img.at<uchar>(i, j);
			cout << n;
		}
		cout << endl;
	}

}


void readMat() {
	string tpath = "E:/cv/color/test/xxx.png";
	Mat img = imread(tpath);
	int n;
	n = img.at<uchar>(1, 1);

	//这里说明凡是输出的值都要声明类型
	cout << img.at<uchar>(1, 1) << " ";


}

void test_arrleng() {
	int arr[10];
	int *p = arr;
	for (int i = 0; i < 10;i++ ) {
		arr[i] = i;
	}
	/*for (int j = 0; j < 10;j++ ) {
		cout << "arr" << j << "  " << arr[j] << endl;
	}*/
	int leng = sizeof(arr) / sizeof(arr[0]);
	cout << "leng arr:" << leng << endl;
	/*for (int i = 0;i<) {
	
	}*/
}

void test_sizeof() {
	int *p = new int[10];
	memset(p, 0, 10 * sizeof(int));

	cout << "sizeof int:" << sizeof(int) << endl;
	cout << "sizeof p:" << sizeof(p) << endl;
	cout << "sizeof p:" << sizeof(*p) << endl;
	cout << "sizeof p / sizeof int:" << sizeof(p) / sizeof(*p) << endl;
	/*
		这里的问题已经非常明显了，sizeof(p)所求的是指针的大小
		原本的问题只是想遍历数组，其实这里只需要在形参中传入数组长度
		不要纠结这个问题
	*/
	for (int i = 0; i < sizeof(p); i++) {
		cout << *p << endl;
		p++;
	}
}

void test_roi(Mat &img) {
	Mat roi_img;
	roi_img = img(Range(0,95), Range(0,100)).clone();
	show_img(roi_img);
}


void test_vect() {
	vector<Mat> roiList;
}

void test_binary() {
	string fpath = "E:/cv/color/test/xxx.png";

	Mat img = imread(fpath);
	Mat img_gray = img.clone();
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	Mat img_bin = img_gray.clone();
	threshold(img_gray, img_bin, 60, 255, CV_THRESH_BINARY);

	show_img(img_bin);
}