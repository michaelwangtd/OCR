#include<iostream>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;

void camera_showImg() {
	/*
		opencv��������ͷ������
	*/
	Mat edges;

	/*
	0���ʼǱ���������ͷ
	1��usb��������ͷ
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
			//frame���д���
			//�Ҷȴ���
			cvtColor(frame, edges, CV_BGR2GRAY);

			//��ֵ�˲�
			//blur(edges,edges,Size(7,7));

			//��Ե���
			//Canny(edges, edges, 0, 30, 3);

			imshow("img", frame);
		}
		waitKey(30);
	}
}