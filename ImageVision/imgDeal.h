#pragma once
#include "source.h"

#ifndef __exdll__
#define __exdll__

#include "deeplearning\deeplearning.h"


#define EXP 2.7182818284590452353602874 

#define _EXPORTDLL
#ifdef _EXPORTDLL
#define IMGDEAL_LIBAPI extern"C" __declspec(dllexport)  
#define IMAGE_API __declspec(dllexport) 
#else
#define IMGDEAL_LIBAPI extern"C"__declspec(dllimport)
#define IMAGE_API __declspec(dllimport) 
#endif

struct blob
{
	int x;
	int y;
	double sigma;
	int intensity;
};

IMGDEAL_LIBAPI double sigmoid(double x);
IMGDEAL_LIBAPI double Tanh(double z);
IMGDEAL_LIBAPI void exColor_Invert(Mat& src); //颜色倒置
IMGDEAL_LIBAPI void	exdrawArrow(Mat& img, Point pStart, Point pEnd, int len, int alpha, Scalar& color, int thickness, int lineType);//画箭头
IMGDEAL_LIBAPI void find_gravity_center(const Mat& mat, Point &p);//找到白色点的重心
IMGDEAL_LIBAPI void Histogram(const Mat& src, Mat& dst);	//图像均值化
IMGDEAL_LIBAPI void calcGradientFeat(const Mat& imgSrc, vector<float>& feat);

IMGDEAL_LIBAPI float sumMatValue(const Mat& image);
IMGDEAL_LIBAPI int get_OTSU_value(const Mat& src);

IMGDEAL_LIBAPI void CreateDoubleMatrix(double **&matrix, CvSize size);
IMGDEAL_LIBAPI int CreateGeoMatchModel(const Mat& templateArr, double maxContrast, double minContrast);
IMGDEAL_LIBAPI void swapValue(uchar* a, uchar* b);			//交换值 C语言，不能用&a






/*
IMAGE_API Mat create_LOG(int size, double sigma);
IMAGE_API vector<double> create_sigma(double start, double step, double end);
IMAGE_API vector<Mat> create_scale_space(Mat &im_in, vector<double> &sigma);
IMAGE_API vector<struct blob> detect_blobs(Mat &im_in, double thresh, vector<double> et, int padding = 10);
IMAGE_API vector<struct blob> prune_blobs(vector<struct blob> blobs_in);
*/
//将上面的函数缩写成一个
IMAGE_API inline vector<struct blob> get_blobs(Mat& src, double thresh = 0.2, double start = 1.0, double step = 0.2, double end = 3.0);

//IMAGE_API void calcGradientFeat(const Mat& imgSrc, vector<float>& feat);


#endif
