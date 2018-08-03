#include<opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

#include "digitalimgpro.h"
#include "utils.h"

/*
opencv形态学处理函数整理

//膨胀处理
dilate(bin_img, sp_img, core);

//腐蚀处理（针对白色背景）
erode(bin_img, sp_img, core);

//高级形态学--开运算
morphologyEx(bin_img, sp_img, MORPH_OPEN, core);

//高级形态学--闭运算
morphologyEx(bin_img, sp_img, MORPH_CLOSE, core);
*/




/*
	设计字符投影分割模块
	input：candidate pic region
	output：vector<Mat> single charss
*/
vector<Mat> getCharsFromHorizontalVerticalSeg(Mat candImg, int hbbw = 0, int vbbw = 0) {
	/*
		先水平投影，后垂直投影
	*/
	vector<Mat> chars;
	Mat hedImg;

	int oheight, owidth;
	int hdheight, hdwidth;

	int *hp, *vp;

	oheight = candImg.rows;
	owidth = candImg.cols*candImg.channels();

	hp = new int[oheight];
	memset(hp, 0, oheight * 4);

	//水平投影
	horizontalProjection(candImg, hp, oheight, owidth);

	//水平裁剪
	hedImg = horizontalClip(candImg, hp, hbbw, oheight, owidth);
	
	hdheight = hedImg.rows;
	hdwidth = hedImg.cols*hedImg.channels();

	vp = new int[hdwidth];
	memset(vp, 0, hdwidth * 4);

	//垂直投影
	verticalProjection(hedImg, vp, hdheight, hdwidth);

	//垂直分割
	verticalClip(chars, hedImg, vp, hdheight, hdwidth);

	return chars;
}


vector<Mat> getCharsFromVerticalSeg(Mat candImg, int hbbw = 0, int vbbw = 0) {
	/*
		只进行垂直投影
	*/
	vector<Mat> chars;
	Mat hedImg;

	
	int hdheight, hdwidth;

	int *vp;

	hedImg = candImg;

	hdheight = hedImg.rows;
	hdwidth = hedImg.cols*hedImg.channels();

	vp = new int[hdwidth];
	memset(vp, 0, hdwidth * 4);

	//垂直投影
	verticalProjection(hedImg, vp, hdheight, hdwidth);

	//垂直分割
	verticalClip(chars, hedImg, vp, hdheight, hdwidth);

	return chars;
}



/*
	直接针对候选区域进行图像处理（去噪）
*/
Mat digitsProcessCandidateRegion(Mat &img) {
	Mat gray_img;
	Mat bin_img;
	Mat sp_img;
	//接受自适应二值化返回的阈值
	double thre;

	cvtColor(img, gray_img, COLOR_BGR2GRAY);

	thre = threshold(gray_img, bin_img, 0, 255, CV_THRESH_OTSU);

	return bin_img;
}