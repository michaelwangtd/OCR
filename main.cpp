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
		���д�ֱͶӰ���ٽ����ַ��ָ�
		imgClipper()��x0��height��ֵ�ǹ̶���
		Ĭ��֮ǰ���������ˮƽ�ָ�
	*/

	int height = img.rows, width = img.cols*img.channels();

	//���崹ֱͶӰ����ָ��
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
		//�ڽ����ַ��ָ�ǰ�����ж�
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


//����ȫ�ֱ������ļ�Ŀ¼·��
string fpath = "X:/cv/color/xxx.png";

int main() {

	/*
		1 �ַ�ȥ�롢�ָ�
	*/
	//Mat img;
	//vector<Mat> chrVec;

	//img = imgProcess(fpath);
	//
	////�ַ��ָ�
	//charSegment(chrVec,img);

	//for (int i = 0; i < chrVec.size();i++) {
	//	show_img(chrVec.at(i));
	//	imwrite(odir + to_string(i) + ".jpg", chrVec.at(i));
	//}


	/*
		2 ����ģ��ʶ���ַ�opencv��д�ַ�
	*/
	//knnPrepareTrainPredict();


	/*
		3 ��б����������任��
	*/
	/*Mat img = imread(fpath);
	Mat rstImg = tiltBalanceHough(img);*/


	/*
		4 ��target region 10λ��ĸ���ַָ�ɵ����ַ�
		  ����ģ��ѵ������
		  �������б��
	*/
	//targetRegion2SingleCharsProcess();


	/*
		����ѵ������
	*/
	//createTrainData();


	/*
	��ȡѵ�����ݽ����㷨���Ȳ���
	*/
	//getTestDataAndAccuracyTest();

	//getWholePicAndAccuracyTest();

	return 0;
}




