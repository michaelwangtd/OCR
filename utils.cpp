#include<iostream>
#include<opencv2\opencv.hpp>
#include<string>
#include<vector>
#include<cv.h>
#include<core/core.hpp>    
#include<highgui/highgui.hpp> 
#include <fstream>
#include <io.h>
using namespace cv;
using namespace std;


//显示图片
void show_img(Mat &img) {

	imshow("Test", img);

	waitKey(0);
}

//输出图片尺寸信息
void size_img(Mat &img) {

	cout << "size:" << img.size << endl;

	cout << "height（rows）:" << img.rows << " width（cols）:" << img.cols << endl;
}

//将图片像素写入到文本
int writeData(string fileName, Mat& matData)
{
	int retVal = 0;
	if (matData.empty())
	{
		cout << "矩阵为空" << endl;
		retVal = 1;
		return (retVal);
	}

	// 打开文件
	ofstream outFile(fileName.c_str(), ios_base::out);	//按新建或覆盖方式写入
	if (!outFile.is_open())
	{
		cout << "打开文件失败" << endl;
		retVal = -1;
		return (retVal);
	}

	// 写入数据
	for (int i = 0; i < matData.rows; i++)
	{
		uchar* pixelPtr = matData.ptr<uchar>(i);            //获取矩阵每行首地址指针  
		for (int j = 0; j < matData.cols*matData.channels(); j++)
		{
			int data = pixelPtr[j];
			outFile << data << "\t";
		}
		outFile << endl;
	}
	return (retVal);
}

//图像合并
void mergeImg(Mat & dst, Mat &src1, Mat &src2)
{
	int rows = src1.rows + 5 + src2.rows;
	int cols = src1.cols + 5 + src2.cols;
	CV_Assert(src1.type() == src2.type());
	dst.create(rows, cols, src1.type());
	src1.copyTo(dst(Rect(0, 0, src1.cols, src1.rows)));
	src2.copyTo(dst(Rect(src1.cols + 5, 0, src2.cols, src2.rows)));
}

//裁剪图片
Mat imgClipper(Mat img, int x0, int y0, int height, int width) {
	/**
	根据待裁剪区域左上角坐标(x0,y0)，和待裁剪区域height、width进行图像裁剪

	int x0 = 70, y0 = 145, h = 70, w = 160;
	Mat cli_img = imgClipper(img, x0,y0,h,w);
	show_img(cli_img);
	**/
	int x1, y1;
	x1 = x0 + height;
	y1 = y0 + width;
	Mat roi_img = img(Range(x0, x1), Range(y0, y1)).clone();
	return roi_img;
}



//递归方式获取文件夹下所有文件
//void getFilesRecursion(string rootPath, vector<string> &files) {
//	/*
//		string rootPath：根文件夹目录
//		vector<string> &files：获取完整路径
//		注意：这里有个递归的方式
//	*/
//
//	//文件句柄
//	long hFile = 0;
//	//存储文件信息的结构体,<io.h>定义好的结构体
//	struct _finddata_t fileinfo;
//	//存放路径
//	string p;
//
//	if ((hFile = _findfirst(p.assign(rootPath).append("\\*").c_str(), &fileinfo)) != -1) {
//		do {
//			//对应子文件夹的递归调用
//			if ((fileinfo.attrib & _A_SUBDIR)) {
//				//文件名不等于"."&&文件名不等于".."
//				//.表示当前目录
//				//..表示当前目录的父目录
//				//判断时，两者都要忽略，不然就无限递归跳不出去了！
//				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
//					getFilesRecursion(p.assign(rootPath).append("\\").append(fileinfo.name),files);
//				}
//			}
//			else {
//				files.push_back(p.assign(rootPath).append("\\").append(fileinfo.name));
//			}
//		} while (_findnext(hFile, &fileinfo) == 0);
//	}
//}


//void getFiles(string path, vector<string>& files)
//{
//	//文件句柄  
//	long   hFile = 0;
//	//文件信息，声明一个存储文件信息的结构体  
//	struct _finddata_t fileinfo;
//	string p;//字符串，存放路径
//	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
//	{
//		do
//		{
//			//如果是目录,迭代之（即文件夹内还有文件夹）  
//			if ((fileinfo.attrib &  _A_SUBDIR))
//			{
//				//文件名不等于"."&&文件名不等于".."
//				//.表示当前目录
//				//..表示当前目录的父目录
//				//判断时，两者都要忽略，不然就无限递归跳不出去了！
//				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
//					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
//			}
//			//如果不是,加入列表  
//			else
//			{
//				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
//			}
//		} while (_findnext(hFile, &fileinfo) == 0);
//		//_findclose函数结束查找
//		_findclose(hFile);
//	}
//}


//遍历文件夹下所有文件，使用于x64环境
void getDirNames(string path, vector<string>&  files)
{
	__int64  Handle;
	struct __finddata64_t  FileInfo;
	string p;
	
	if ((Handle = _findfirst64(p.assign(path).append("\\*").c_str(), &FileInfo)) != -1)
	{
		do {
			if ((FileInfo.attrib & _A_SUBDIR)) {
				if (strcmp(FileInfo.name,".")!=0 && strcmp(FileInfo.name,"..")!=0) {
					files.push_back(p.assign(FileInfo.name));
				}
			}
		} while (_findnext64(Handle, &FileInfo) == 0);
		_findclose(Handle);
	}
}

void getFileNames(string path, vector<string>&  files)
{
	__int64  Handle;
	struct __finddata64_t  FileInfo;
	string p;
	string suffix2 = "\\*";
	if ((Handle = _findfirst64(p.assign(path).append(suffix2).c_str(), &FileInfo)) != -1)
	{
		do {
			if (strcmp(FileInfo.name, ".") != 0 && strcmp(FileInfo.name, "..") != 0) {
				files.push_back(p.assign(path).append("\\").append(FileInfo.name));
			}
		} while (_findnext64(Handle, &FileInfo) == 0);
		_findclose(Handle);
	}
}

void getFileNames_backup(string path, vector<string>&  files, string suffix)
{
	/*
		作为上面getDirNames()和getFileNames()函数的原型函数备份
	*/
	__int64  Handle;
	struct __finddata64_t  FileInfo;
	string p;
	string suffix2 = "\\*." + suffix;
	if ((Handle = _findfirst64(p.assign(path).append(suffix2).c_str(), &FileInfo)) != -1)
	{
		do {
			files.push_back(p.assign(path).append("\\").append(FileInfo.name));
		} while (_findnext64(Handle, &FileInfo) == 0);
		_findclose(Handle);
	}
}



