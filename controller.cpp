#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

#include "digitalimgpro.h"
#include "dnls.h"
#include "utils.h"

/*
	ϵͳ���õ����ϲ�
*/

Mat tiltBalanceHough(Mat &src, int flag = 0) {
	/*
		��б����������任��
	*/
	double degree;
	Mat dst, binImg;

	//������ת�Ƕ�
	degree = calcDegree(src,flag);

	binImg = digitsProcessCandidateRegion(src);

	if (degree == 0.0) {
		//show_img(binImg);
		return binImg;
	}

	//��תͼ��
	rotateImage(binImg, dst, degree);
	return dst;
}

