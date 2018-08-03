#include <iostream>
#include <opencv2\opencv.hpp>
#include <map>

using namespace std;
using namespace cv;

#include "controller.h"
#include "utils.h"
#include "dnls.h"
#include "digitalimgpro.h"
#include "alg.h"

/*
	���̴����
*/

//////////////////////////��target region 10λ��ĸ���ַָ�ɵ����ַ�///////////////////////////
////////////////////////����ѵ������/////////////////////////
void targetRegion2SingleCharsProcess() {

	string indir = "E:\\cv\\color\\xxx\\xxx\\xxx";
	string outdir = "E:\\cv\\color\\xxx\\xxx\\xxx\\xxx";


	Mat src, houghImg, bin_img;
	vector<Mat> chars;
	vector<string> fpaths;

	//��ȡ��Ŀ¼�������ļ�·��
	getFileNames(indir, fpaths);

	int startIdx = indir.length() + 1;	//�����и��ָ���\\Ҫ��1
	int n = 3;

	for (int i = 0; i < fpaths.size();i++) {
		string fpath = fpaths.at(i);
		string firstName = fpath.substr(startIdx, n);

		src = imread(fpath);

		houghImg = tiltBalanceHough(src);

		chars = getCharsFromHorizontalVerticalSeg(houghImg);
		//cout << "chars size:" << chars.size() << endl;

		for (int i = 0; i < chars.size(); i++) {
			Mat singleImg = chars.at(i);

			//ͼ����һ��
			//���������Ż�,��ʱ�����ǹ̶���
			singleImg = resizeImg(singleImg, 15, 25);
			//cout << "height: " << singleImg.rows << " width: " << singleImg.cols << endl;
			
			string outName = firstName + "_" + to_string(i + 1) + ".png";
			cout << singleImg.size() << " " << outName << endl;
			string outfpath = outdir + "//" + outName;
			imwrite(outfpath, singleImg);
		}
	}
}
//////////////////////////��target region 10λ��ĸ���ַָ�ɵ����ַ�///////////////////////////


///////////////////////////////////////���ļ�������ѵ������//////////////////////////////////////////
vector<Mat> createTrainData() {
	/*
		��E:\cv\color\prefix_num\03-singlepool\classifybase
		���ݼ�������ѵ������X,Y
	*/
	vector<Mat> trainData;
	//���������
	Mat X, Y;
	vector<string> dirnames;

	string basepath = "E:\\cv\\color\\xxx\\xxx\\xxx";
	getDirNames(basepath, dirnames);

	for (int i = 0; i < dirnames.size();i++) {
		//��Ŀ¼����
		string dirname = dirnames.at(i);
		//int label = atoi(dirname.c_str());

		string subdir = basepath + "\\" + dirname;
		vector<string> subfpaths;

		getFileNames(subdir, subfpaths);

		//cout << subfpaths.size() << endl;
		for (int j = 0; j < subfpaths.size(); j++) {
			string sfp = subfpaths.at(j);
			Mat x = imread(sfp);	//��ط�Ϊ3ͨ��ͼ
			cvtColor(x, x, CV_BGR2GRAY);	//xΪ�Ҷ�ͼ
			X.push_back(x.reshape(0, 1));
			Y.push_back(i);
			//cout << label << endl;
		}
	}
	trainData.push_back(X);
	trainData.push_back(Y);

	/*for (int k = 0; k < Y.rows; k++) {
		cout << Y.row(k) << endl;
	}
	cout << "X size:" << X.size() << endl;
	cout << "Y size" << Y.size() << endl;*/
	return trainData;
}
///////////////////////////////////////���ļ�������ѵ������//////////////////////////////////////////


/////////////////////////����ѵ������ѵ��ģ�Ͳ�����ģ�;��Ȳ���//////////////////////////////
map<int, string> trans;

void getTransMap(map<int, string> &trans) {
	trans[0] = "0";
	trans[1] = "1";
	trans[2] = "2";
	trans[3] = "3";
	trans[4] = "4";
	trans[5] = "5";
	trans[6] = "6";
	trans[7] = "7";
	trans[8] = "8";
	trans[9] = "9";
	trans[10] = "a";
	trans[11] = "b";
	trans[12] = "c";
	trans[13] = "d";
	trans[14] = "e";
	trans[15] = "f";
	trans[16] = "g";
	trans[17] = "h";
	trans[18] = "j";
	trans[19] = "l";
	trans[20] = "m";
	trans[21] = "n";
	trans[22] = "p";
	trans[23] = "q";
	trans[24] = "r";
	trans[25] = "s";
	trans[26] = "u";
	trans[27] = "w";
	trans[28] = "x";
	trans[29] = "y";
	trans[30] = "z";
}


void knnModel(Ptr<KNearest> &knn) {
	vector<Mat> trainData;
	Mat X, Y;

	trainData = createTrainData();
	X = trainData[0];
	Y = trainData[1];
	/*cout << X.size() << " " << Y.size() << endl;
	cout << X.type() << " " << Y.type() << endl;*/

	X.convertTo(X, CV_32F);
	
	knn = KNN(X, Y, 6);
	
}

//Ŀ��ͼ���Ԥ�⺯��
vector<double> predictChars(Mat img, Ptr<KNearest> &knn) {
	Mat houghImg;
	vector<Mat> chars;

	vector<double> rst;
	
	//��������ͼƬ�ǲ�ɫĿ������ͼ���
	houghImg = tiltBalanceHough(img);

	chars = getCharsFromHorizontalVerticalSeg(houghImg);
	//cout << "chars size:" << chars.size() << endl;

	for (int i = 0; i < chars.size(); i++) {
		Mat singleImg = chars.at(i);

		/*cout << singleImg.channels() << endl;
		show_img(singleImg);*/

		//ͼ����һ��
		singleImg = resizeImg(singleImg, 15, 25).reshape(0, 1);
		//cout << "height: " << singleImg.rows << " width: " << singleImg.cols << endl;
		singleImg.convertTo(singleImg, CV_32F);
		
		//knn
		float pd_num = knn->predict(singleImg);
		rst.push_back(pd_num);
	}

	/*for (int j = 0; j < rst.size();j++) {
		cout << " " << rst.at(j);
	}
	cout << endl;
	show_img(img);*/

	return rst;
}


vector<double> predictCharsFromBinTarImg(Mat img, Ptr<KNearest> &knn) {
	Mat houghImg;
	vector<Mat> chars;

	vector<double> rst;

	//��������ͼƬ�ǲ�ɫĿ������ͼ���
	houghImg = tiltBalanceHough(img);

	chars = getCharsFromHorizontalVerticalSeg(houghImg);
	//cout << "chars size:" << chars.size() << endl;

	for (int i = 0; i < chars.size(); i++) {
		Mat singleImg = chars.at(i);

		/*cout << singleImg.channels() << endl;
		show_img(singleImg);*/

		//ͼ����һ��
		singleImg = resizeImg(singleImg, 15, 25).reshape(0, 1);
		//cout << "height: " << singleImg.rows << " width: " << singleImg.cols << endl;
		singleImg.convertTo(singleImg, CV_32F);

		//knn
		float pd_num = knn->predict(singleImg);
		rst.push_back(pd_num);
	}

	/*for (int j = 0; j < rst.size();j++) {
	cout << " " << rst.at(j);
	}
	cout << endl;
	show_img(img);*/

	return rst;
}


vector<double> predictCharsAndVerticalSeg(Mat img, Ptr<KNearest> &knn) {
	Mat houghImg;
	vector<Mat> chars;

	vector<double> rst;

	//��������ǻ�����б���������ﲻ������б����
	//houghImg = tiltBalanceHough(img);

	//chars = getCharsFromHorizontalVerticalSeg(houghImg);
	chars = getCharsFromVerticalSeg(img);
	//cout << "chars size:" << chars.size() << endl;

	for (int i = 0; i < chars.size(); i++) {
		Mat singleImg = chars.at(i);

		/*cout << singleImg.channels() << endl;
		show_img(singleImg);*/

		//ͼ����һ��
		singleImg = resizeImg(singleImg, 15, 25).reshape(0, 1);
		//cout << "height: " << singleImg.rows << " width: " << singleImg.cols << endl;
		singleImg.convertTo(singleImg, CV_32F);

		//knn
		float pd_num = knn->predict(singleImg);
		rst.push_back(pd_num);
	}

	/*for (int j = 0; j < rst.size();j++) {
	cout << " " << rst.at(j);
	}
	cout << endl;
	show_img(img);*/

	return rst;
}



void getTestDataAndAccuracyTest() {
	/*
		��ȡѵ�����ݽ����㷨���Ȳ���
		��ȡĿ��������ͨ��ͼ�񣬽���ocrʶ��
	*/

	//string tdir = "E:\\cv\\color\\xxx\\xxx\\xxx";
	string tdir = "E:\\cv\\color\\xxx\\xxx\\xxx";

	vector<string> fnames;
	Ptr<KNearest> knn;

	getFileNames(tdir, fnames);

	knnModel(knn);

	getTransMap(trans);

	//cout << "fname size: " << fnames.size() << endl;
	
	for (int i = 0; i < fnames.size();i++) {
		string fpath = fnames.at(i);
		vector<double> subrst;

		//Mat tarImg = localCandidateRegion(timg);
		//Mat tarImg = imgProcess(timg);

		//�����ȡ��ͼ���ǽ�ȡ�õ�target region��ͼ���
		Mat timg = imread(fpath);

		//ocr
		subrst = predictChars(timg, knn);

		for (int j = 0; j < subrst.size();j++) {
			cout << " " << trans[(int)subrst.at(j)];
		}
		cout << endl;
		show_img(timg);

	}
	
}


void getWholePicAndAccuracyTest() {
	/*
		��ȡ����ͼƬ�����㷨���Ȳ���
		��ȡ����rmbͼƬ����ocrʶ��
		��ʾ�㷨��������
	*/

	//string tdir = "E:\\cv\\color\\xxx\\xxx\\xxx";
	string tdir = "E:\\cv\\color\\xxx\\xxx\\xxx\\";

	vector<string> fnames;
	Ptr<KNearest> knn;

	getFileNames(tdir, fnames);

	//����knnģ��
	knnModel(knn);

	//�ַ�ʶ����ӳ��
	getTransMap(trans);

	//cout << "fname size: " << fnames.size() << endl;

	for (int i = 0; i < fnames.size(); i++) {
		string fpath = fnames.at(i);
		vector<double> subrst;

		//�����ȡ��ͼ����ȫ����ͼ
		Mat oimg = imread(fpath);
		
		//oimg = resizeImg(oimg, 600, 300);
		
		//from origin rmb img to target bin img
		Mat timg = origin2TargetImg(oimg);
		show_img(timg);
		//ocr
		/*subrst = predictChars(timg, knn);*/
		//����������Ƕ�ֵĿ������ͼ��
		subrst = predictCharsAndVerticalSeg(timg, knn);
		cout << "subrst: " << subrst.size() << endl;

		for (int j = 0; j < subrst.size(); j++) {
			cout << " " << trans[(int)subrst.at(j)];
		}
		cout << endl;
		show_img(oimg);
	}
}
/////////////////////////����ѵ������ѵ��ģ�Ͳ�����ģ�;��Ȳ���//////////////////////////////