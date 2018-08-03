#include<iostream>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;

void camera_showImg() {
	/*
		opencv调用摄像头的例子
	*/
	Mat edges;

	/*
	0：笔记本内置摄像头
	1：usb驱动摄像头
	*/
	VideoCapture capture(0);
	if (!capture.isOpened()) {
		cout << "the video is not open" << endl;
	}

	while (1) {
		Mat frame;
		capture >> frame;
		if (frame.empty()) {
			cout << "no captured frame" << endl;
		}
		else {
			//frame进行处理
			//灰度处理
			cvtColor(frame, edges, CV_BGR2GRAY);

			//均值滤波
			//blur(edges,edges,Size(7,7));

			//边缘检测
			//Canny(edges, edges, 0, 30, 3);

			imshow("img", frame);
		}
		waitKey(30);
	}
}