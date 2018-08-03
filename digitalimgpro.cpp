#include <opencv2\opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

#include "utils.h"

#define ERROR 0.0

/*
	单个功能性的函数，形参尽量写引用值
*/

Mat horizontalClip(Mat &img, int *&hp, int &bw, int &height, int &width);
void horizontalProjection(Mat &img, int *&p, int &height, int &width);


Mat pixelFilp(Mat img, int &height, int &width) {
	/*
		像素值翻转
		注意：width = img.cols * img.channels()
	*/
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			//img.at<uchar>(i, j) = img.at<uchar>(i, j) > 126 ? 0 : 255;
			img.at<uchar>(i, j) = 255 - img.at<uchar>(i, j);
		}
	}
	return img;
}

Mat resizeImg(Mat &img, int width, int height) {
	/*
		图像放缩函数
		注意：这里指定interpolation值为INTER_LINEAR
		INTER_NEAREST - 最近邻插值
		INTER_LINEAR - 线性插值（默认）
		INTER_AREA - 区域插值
		INTER_CUBIC - 三次样条插值
		INTER_LANCZOS4 - Lanczos插值
	*/
	Mat reImg(height, width, img.type());
	resize(img, reImg, reImg.size(), 0, 0, INTER_LINEAR);
	return reImg;
}


Mat filterGrayThresholdShape(Mat &img) {
	Mat mb_img;
	Mat gray_img, gray_bm_img;
	Mat bin_img;
	Mat sp_img;

	double thre;

	//中值滤波
	medianBlur(img, mb_img, 3);

	//灰度处理
	cvtColor(img, gray_img, COLOR_BGR2GRAY);
	cvtColor(mb_img, gray_bm_img, COLOR_BGR2GRAY);

	//自适应二值化
	thre = threshold(gray_img, bin_img, 0, 255, CV_THRESH_OTSU);

	//形态学处理
	Mat core = getStructuringElement(MORPH_RECT, Size(1, 1));
	morphologyEx(bin_img, sp_img, MORPH_OPEN, core);

	return sp_img;
}


Mat candidateRegion(Mat &img, int xcn, int ycn, int np) {
	/**
	以图片左上角为坐标原点
	左边向下为x轴正方向，上边向右为y轴正方向
	int xcn,ycn：分割点个数
	xcn：x轴 cut num，np：noise pixels
	**/
	int x0, y0, h, w;
	int img_h = img.rows, img_w = img.cols;

	x0 = (int)floor(img_h / xcn) * 2;
	y0 = np;
	h = img_h - x0 - np;
	w = (int)round(img_w / ycn) - np;

	return imgClipper(img, x0, y0, h, w);

}

Mat targetRegion(Mat &img, int bw) {
	/*
	bw：bounding width
	*/
	Mat rst;

	int height = img.rows, width = img.cols;

	//初始化数组长度为img.rows，并初始化为0
	int *hp = new int[height];
	memset(hp, 0, height * 4);

	//按行统计像素值，水平投影
	horizontalProjection(img, hp, height, width);

	/*for (int i = 0; i < height;i++) {
	cout << *hp << endl;
	hp++;
	}*/

	//按行裁剪
	rst = horizontalClip(img, hp, bw, height, width);

	return rst;
}



Mat imgProcess(string &fpath) {
	Mat img, bin_img, cand_img, tar_img;

	//读取图片
	img = imread(fpath);

	//去噪处理
	bin_img = filterGrayThresholdShape(img);

	//候选区域裁剪
	cand_img = candidateRegion(bin_img, 3, 4, 2);

	//投影直方图分割目标区域
	tar_img = targetRegion(cand_img, 2);
	return tar_img;
}

Mat localCandidateRegion(string &fpath) {
	Mat img, candImg;

	img = imread(fpath);

	candImg = candidateRegion(img, 3, 4, 2);
	return candImg;
}


Mat origin2TargetImg(Mat img) {
	/*
		定位 target location
		1 候选区域裁剪
		2 投影直方图分割目标区域
	*/

	Mat bin_img, cand_img, tar_img;

	//去噪处理
	bin_img = filterGrayThresholdShape(img);
	
	//候选区域裁剪
	cand_img = candidateRegion(bin_img, 3, 4, 2);
	
	//投影直方图分割目标区域
	tar_img = targetRegion(cand_img, 2);

	//cout << "img channels: " << tar_img.channels() << endl;

	return tar_img;
}




/////////////////////////////直方图投影////////////////////////////////
Mat horizontalProjection_backup(Mat &img, int *&p, int height, int width) {
	/**
		函数的统计结果保存在指针p中并返回
		注意：
		1 形参声明【指针p的一个引用】：int *&p
		2 像素值需要进行翻转
	**/

	//像素值翻转
	Mat filp_img = pixelFilp(img, height, width);

	//统计像素填充数组
	int pn;
	for (int i = 0; i < height; i++) {
		pn = 0;
		for (int j = 0; j < width; j++) {
			pn += filp_img.at<uchar>(i, j);
		}
		*(p + i) = pn;
	}
	return filp_img;
}

void horizontalProjection(Mat &img, int *&p, int &height, int &width) {
	/**
	函数的统计结果保存在指针p中并返回
	注意：
	1 形参声明【指针p的一个引用】：int *&p
	**/

	//统计像素填充数组
	int pn;
	for (int i = 0; i < height; i++) {
		pn = 0;
		for (int j = 0; j < width; j++) {
			//opencv中数值要取反
			pn += 255 - img.at<uchar>(i, j);
		}
		*(p + i) = pn;
	}
}

Mat horizontalClip(Mat &img, int *&hp, int &bw, int &height, int &width) {
	/*
	水平裁剪
	根据hp数组值，裁剪第一个区域
	*/
	int startp = 0, endp = 0;
	int clip_width;
	int x0, y0;

	for (int i = 0; i < height; i++) {
		if (startp > 0 && endp > startp) {
			break;
		}
		if (*(hp + i) != 0 && startp == endp) {
			startp = i;
		}
		if (*(hp + i) == 0 && startp > endp) {
			endp = i;
		}
	}

	//调整裁剪像素点
	startp -= bw;	//高的像素点上移
	endp += bw;
	clip_width = endp - startp;
	//注意初始点的设置
	x0 = startp;
	y0 = 0;

	/*cout << "x0:" << x0 << " y0:" << y0 << " clip_width:" << clip_width << " width:" << width << endl;*/

	return imgClipper(img, x0, y0, clip_width, width);
}


void verticalProjection(Mat &img, int *&vp, int &height, int &width) {
	/*
		垂直投影
	*/
	int pn;
	for (int i = 0; i < width; i++) {
		pn = 0;
		for (int j = 0; j < height; j++) {
			pn += 255 - img.at<uchar>(j, i);
		}
		*(vp + i) = pn;
	}
}

void verticalClip(vector<Mat> &chars, Mat &img, int *&vp, int &height, int &width) {
	/*
		Mat &img, int *&hp, int &height, int &width, int &bw
		垂直投影
	*/
	int startp = 0, endp = 0;
	bool inRegion = false;

	int dist;
	Mat block;
	int x0 = 0;

	for (int i = 0; i < width; i++) {
		if (!inRegion && *(vp + i) != 0) {
			inRegion = true;
			startp = i;
		}
		if (*(vp + i) == 0 && inRegion) {
			inRegion = false;
			endp = i;
		}
		//在进行字符分割前进行判断
		if (startp < endp && !inRegion) {
			dist = endp - startp;

			if (endp = width - 1 && dist < 3) {
				continue;
			}

			//cout << "x0:" << x0 << " y0:" << startp << " height:" << height << " width:" << dist << endl;
			block = imgClipper(img, x0, startp, height, dist);
			//show_img(block);
			chars.push_back(block);
		}
	}
}
/////////////////////////////直方图投影////////////////////////////////


/////////////////////倾斜矫正（霍夫变换）////////////////////
double degreeTrans(double theta) {
	double rst = theta / CV_PI * 180;
	return rst;
}

double calcDegree(Mat &src,int flag = 0) {
	Mat midImg, dstImg;
	vector<Vec2f> lines;

	cvtColor(src, midImg, CV_BGR2GRAY);

	Canny(midImg, midImg, 50, 200, 3);

	cvtColor(midImg, dstImg, CV_GRAY2BGR);

	//霍夫直线检测部分可以修改
	int thre = 300; //阈值设定过高导致无法检测直线，阈值过低直线太多，速度很慢
	//int thre = 50;
	HoughLines(midImg, lines, 1, CV_PI / 180, thre, 0, 0);

	if (!lines.size()) {
		thre -= 50;
		HoughLines(midImg, lines, 1, CV_PI / 180, thre, 0, 0);
	}
	if (!lines.size()) {
		thre -= 50;
		HoughLines(midImg, lines, 1, CV_PI / 180, thre, 0, 0);
	}
	if (!lines.size()) {
		thre -= 50;
		HoughLines(midImg, lines, 1, CV_PI / 180, thre, 0, 0);
	}
	if (!lines.size()) {
		thre -= 50;
		HoughLines(midImg, lines, 1, CV_PI / 180, thre, 0, 0);
	}
	if (!lines.size()) {
		thre -= 50;
		HoughLines(midImg, lines, 1, CV_PI / 180, thre, 0, 0);
	}
	if (!lines.size()) {
		cout << "没有检测到直线:HoughLines " << endl;
		return ERROR;
	}
	//cout << "HoughLines threshold: " << thre << endl;

	float sum = 0;

	for (int i = 0; i < lines.size(); i++) {
		float rho = lines[i][0];
		float theta = lines[i][1];
		Point pt1, pt2;

		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;

		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));

		sum += theta;

		line(dstImg, pt1, pt2, Scalar(55, 100, 195), 1, LINE_AA);
	}

	if (flag) {
		show_img(dstImg);
	}

	float avg = sum / lines.size();

	double angle = degreeTrans(avg) - 90;

	return angle;
}

void rotateImage(Mat &src, Mat &dst, double &degree) {
	Point2f center;
	center.x = float(src.cols / 2.0);
	center.y = float(src.rows / 2.0);
	int length = 0;
	length = sqrt(src.cols*src.cols + src.rows*src.rows);
	//计算二维旋转的仿射变换矩阵
	Mat affineMrt = getRotationMatrix2D(center, degree, 1);
	//仿射变换，背景色填充为白色
	warpAffine(src, dst, affineMrt, Size(length, length), 1, 0, Scalar(255, 255, 255));
}
/////////////////////倾斜矫正（霍夫变换）////////////////////