#include<iostream>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

#include "utils.h"

#define THRESHOLD 1;

////////////////////////////opencv digits.png+knn测试knn准确率////////////////////////////////
/*
	模型-knn：

	/opencv/sources/samples/data/digits.png
	1 图片大小：1000 * 2000
	  数字块：20 * 20
	  总共50*100个手写字符

	2 样本矩阵：5000 * 400

	3 digits.png截取手写数字块时按列截取

*/

vector<Mat> createTTData(int flag = 0) {
	/*
		训练数据从一个整幅图片中获得，不生成外部入口
	*/
	Mat img, gray;
	vector<Mat> dataStream;

	string fpath;
	string dpath = "C:/software/opencv/3.4.0/opencv/sources/samples/data/";
	string fname = "digits.png";
	fpath = dpath + fname;

	img = imread(fpath);
	cvtColor(img, gray, CV_BGR2GRAY);

	int b = 20; //50*100
	int h = gray.rows / b; //50
	int w = gray.cols / b; //100
	Mat X, Y;
	Mat temp;

	string odir = "E:/cv/color/hwm/";
	for (int i = 0; i < w; i++) {
		int offsetCol = i * b;
		for (int j = 0; j < h; j++) {
			int offsetRow = j * b;
			//这里的截取只需要标定左上角坐标点
			temp = imgClipper(gray, offsetRow, offsetCol, b, b);

			if (flag) {
				string ofpath = odir + to_string(i) + "_" + to_string(j) + ".jpg";
				imwrite(ofpath, temp);
				cout << i << "_" << j << endl;
			}

			X.push_back(temp.reshape(0, 1));
			Y.push_back((int)(floor(j / 5)));
		}
	}

	/*cout << X.size() << X.rows << " " << X.cols << endl;
	cout << Y.size() << Y.rows << " " << Y.cols << endl;*/

	dataStream.push_back(X);
	dataStream.push_back(Y);

	return dataStream;
}

Ptr<KNearest> KNN(Mat &x_train, Mat &y_train, int k = 1) {
	/*
		封装一个KNN算法，默认参数为最近邻k=1
	*/
	Ptr<TrainData> tData = TrainData::create(x_train, ROW_SAMPLE, y_train);
	Ptr<KNearest> knn = KNearest::create();
	knn->setDefaultK(k);
	knn->setIsClassifier(true);
	knn->train(tData);

	return knn;
}


void knnPrepareTrainPredict() {
	/*
		主程序，pipeline流程
	*/
	vector<Mat> traintestData;
	Mat x_train, y_train, x_test, y_test;

	Ptr<KNearest> knn;

	//准备数据
	traintestData = createTTData();

	Mat X = traintestData[0], Y = traintestData[1];

	/*cout << X.size() << " " << Y.size() << endl;
	cout << X.type() << " " << Y.type() << endl;*/
	

	X.convertTo(X, CV_32F);
	int divNum = 4000;
	x_train = imgClipper(X, 0, 0, divNum, X.cols);
	x_test = imgClipper(X, divNum, 0, X.rows - divNum, X.cols);
	y_train = imgClipper(Y, 0, 0, divNum, Y.cols);
	y_test = imgClipper(Y, divNum, 0, Y.rows - divNum, Y.cols);

	/*cout << "x_train: " << x_train.size() << endl;
	cout << "x_test: " << x_test.size() << endl;
	cout << "y_train: " << y_train.size() << endl;
	cout << "y_test: " << y_test.size() << endl;
	x_train: [400 x 4000]
	x_test : [400 x 1000]
	y_train : [1 x 4000]
	y_test : [1 x 1000]*/

	//训练模型
	knn = KNN(x_train,y_train,5);
	

	//预测数据
	int start = divNum;
	double testp = 0;

	for (int i = start; i < X.rows;i++) {
		Mat sample = X.row(i);
		float rst = knn->predict(sample);

		float count = abs(rst - Y.at<int>(i)) < 1.f ? 1.f : 0.f;

		testp += count;

		cout << i << " " << Y.at<int>(i) << "-" << rst << endl;
	}
	testp = testp / 1000;

	printf("accuracy：test = %.1f%%\n", testp*100.);

}
////////////////////////////opencv digits.png+knn测试knn准确率////////////////////////////////


