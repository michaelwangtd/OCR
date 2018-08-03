#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

#include "digitalimgpro.h"
#include "dnls.h"
#include "utils.h"

/*
	系统调用的中上层
*/

Mat tiltBalanceHough(Mat &src, int flag = 0) {
	/*
		倾斜矫正（霍夫变换）
	*/
	double degree;
	Mat dst, binImg;

	//计算旋转角度
	degree = calcDegree(src,flag);

	binImg = digitsProcessCandidateRegion(src);

	if (degree == 0.0) {
		//show_img(binImg);
		return binImg;
	}

	//旋转图像
	rotateImage(binImg, dst, degree);
	return dst;
}

