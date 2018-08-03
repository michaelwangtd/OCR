#include<opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

#include "digitalimgpro.h"
#include "utils.h"

/*
opencv��̬ѧ����������

//���ʹ���
dilate(bin_img, sp_img, core);

//��ʴ������԰�ɫ������
erode(bin_img, sp_img, core);

//�߼���̬ѧ--������
morphologyEx(bin_img, sp_img, MORPH_OPEN, core);

//�߼���̬ѧ--������
morphologyEx(bin_img, sp_img, MORPH_CLOSE, core);
*/




/*
	����ַ�ͶӰ�ָ�ģ��
	input��candidate pic region
	output��vector<Mat> single charss
*/
vector<Mat> getCharsFromHorizontalVerticalSeg(Mat candImg, int hbbw = 0, int vbbw = 0) {
	/*
		��ˮƽͶӰ����ֱͶӰ
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

	//ˮƽͶӰ
	horizontalProjection(candImg, hp, oheight, owidth);

	//ˮƽ�ü�
	hedImg = horizontalClip(candImg, hp, hbbw, oheight, owidth);
	
	hdheight = hedImg.rows;
	hdwidth = hedImg.cols*hedImg.channels();

	vp = new int[hdwidth];
	memset(vp, 0, hdwidth * 4);

	//��ֱͶӰ
	verticalProjection(hedImg, vp, hdheight, hdwidth);

	//��ֱ�ָ�
	verticalClip(chars, hedImg, vp, hdheight, hdwidth);

	return chars;
}


vector<Mat> getCharsFromVerticalSeg(Mat candImg, int hbbw = 0, int vbbw = 0) {
	/*
		ֻ���д�ֱͶӰ
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

	//��ֱͶӰ
	verticalProjection(hedImg, vp, hdheight, hdwidth);

	//��ֱ�ָ�
	verticalClip(chars, hedImg, vp, hdheight, hdwidth);

	return chars;
}



/*
	ֱ����Ժ�ѡ�������ͼ����ȥ�룩
*/
Mat digitsProcessCandidateRegion(Mat &img) {
	Mat gray_img;
	Mat bin_img;
	Mat sp_img;
	//��������Ӧ��ֵ�����ص���ֵ
	double thre;

	cvtColor(img, gray_img, COLOR_BGR2GRAY);

	thre = threshold(gray_img, bin_img, 0, 255, CV_THRESH_OTSU);

	return bin_img;
}