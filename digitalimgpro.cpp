#include <opencv2\opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

#include "utils.h"

#define ERROR 0.0

/*
	���������Եĺ������βξ���д����ֵ
*/

Mat horizontalClip(Mat &img, int *&hp, int &bw, int &height, int &width);
void horizontalProjection(Mat &img, int *&p, int &height, int &width);


Mat pixelFilp(Mat img, int &height, int &width) {
	/*
		����ֵ��ת
		ע�⣺width = img.cols * img.channels()
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
		ͼ���������
		ע�⣺����ָ��interpolationֵΪINTER_LINEAR
		INTER_NEAREST - ����ڲ�ֵ
		INTER_LINEAR - ���Բ�ֵ��Ĭ�ϣ�
		INTER_AREA - �����ֵ
		INTER_CUBIC - ����������ֵ
		INTER_LANCZOS4 - Lanczos��ֵ
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

	//��ֵ�˲�
	medianBlur(img, mb_img, 3);

	//�Ҷȴ���
	cvtColor(img, gray_img, COLOR_BGR2GRAY);
	cvtColor(mb_img, gray_bm_img, COLOR_BGR2GRAY);

	//����Ӧ��ֵ��
	thre = threshold(gray_img, bin_img, 0, 255, CV_THRESH_OTSU);

	//��̬ѧ����
	Mat core = getStructuringElement(MORPH_RECT, Size(1, 1));
	morphologyEx(bin_img, sp_img, MORPH_OPEN, core);

	return sp_img;
}


Mat candidateRegion(Mat &img, int xcn, int ycn, int np) {
	/**
	��ͼƬ���Ͻ�Ϊ����ԭ��
	�������Ϊx���������ϱ�����Ϊy��������
	int xcn,ycn���ָ�����
	xcn��x�� cut num��np��noise pixels
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
	bw��bounding width
	*/
	Mat rst;

	int height = img.rows, width = img.cols;

	//��ʼ�����鳤��Ϊimg.rows������ʼ��Ϊ0
	int *hp = new int[height];
	memset(hp, 0, height * 4);

	//����ͳ������ֵ��ˮƽͶӰ
	horizontalProjection(img, hp, height, width);

	/*for (int i = 0; i < height;i++) {
	cout << *hp << endl;
	hp++;
	}*/

	//���вü�
	rst = horizontalClip(img, hp, bw, height, width);

	return rst;
}



Mat imgProcess(string &fpath) {
	Mat img, bin_img, cand_img, tar_img;

	//��ȡͼƬ
	img = imread(fpath);

	//ȥ�봦��
	bin_img = filterGrayThresholdShape(img);

	//��ѡ����ü�
	cand_img = candidateRegion(bin_img, 3, 4, 2);

	//ͶӰֱ��ͼ�ָ�Ŀ������
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
		��λ target location
		1 ��ѡ����ü�
		2 ͶӰֱ��ͼ�ָ�Ŀ������
	*/

	Mat bin_img, cand_img, tar_img;

	//ȥ�봦��
	bin_img = filterGrayThresholdShape(img);
	
	//��ѡ����ü�
	cand_img = candidateRegion(bin_img, 3, 4, 2);
	
	//ͶӰֱ��ͼ�ָ�Ŀ������
	tar_img = targetRegion(cand_img, 2);

	//cout << "img channels: " << tar_img.channels() << endl;

	return tar_img;
}




/////////////////////////////ֱ��ͼͶӰ////////////////////////////////
Mat horizontalProjection_backup(Mat &img, int *&p, int height, int width) {
	/**
		������ͳ�ƽ��������ָ��p�в�����
		ע�⣺
		1 �β�������ָ��p��һ�����á���int *&p
		2 ����ֵ��Ҫ���з�ת
	**/

	//����ֵ��ת
	Mat filp_img = pixelFilp(img, height, width);

	//ͳ�������������
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
	������ͳ�ƽ��������ָ��p�в�����
	ע�⣺
	1 �β�������ָ��p��һ�����á���int *&p
	**/

	//ͳ�������������
	int pn;
	for (int i = 0; i < height; i++) {
		pn = 0;
		for (int j = 0; j < width; j++) {
			//opencv����ֵҪȡ��
			pn += 255 - img.at<uchar>(i, j);
		}
		*(p + i) = pn;
	}
}

Mat horizontalClip(Mat &img, int *&hp, int &bw, int &height, int &width) {
	/*
	ˮƽ�ü�
	����hp����ֵ���ü���һ������
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

	//�����ü����ص�
	startp -= bw;	//�ߵ����ص�����
	endp += bw;
	clip_width = endp - startp;
	//ע���ʼ�������
	x0 = startp;
	y0 = 0;

	/*cout << "x0:" << x0 << " y0:" << y0 << " clip_width:" << clip_width << " width:" << width << endl;*/

	return imgClipper(img, x0, y0, clip_width, width);
}


void verticalProjection(Mat &img, int *&vp, int &height, int &width) {
	/*
		��ֱͶӰ
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
		��ֱͶӰ
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
		//�ڽ����ַ��ָ�ǰ�����ж�
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
/////////////////////////////ֱ��ͼͶӰ////////////////////////////////


/////////////////////��б����������任��////////////////////
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

	//����ֱ�߼�ⲿ�ֿ����޸�
	int thre = 300; //��ֵ�趨���ߵ����޷����ֱ�ߣ���ֵ����ֱ��̫�࣬�ٶȺ���
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
		cout << "û�м�⵽ֱ��:HoughLines " << endl;
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
	//�����ά��ת�ķ���任����
	Mat affineMrt = getRotationMatrix2D(center, degree, 1);
	//����任������ɫ���Ϊ��ɫ
	warpAffine(src, dst, affineMrt, Size(length, length), 1, 0, Scalar(255, 255, 255));
}
/////////////////////��б����������任��////////////////////