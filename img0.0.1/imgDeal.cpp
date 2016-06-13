//#define BUILD_IMAGEDEAL_DLL
#include "imgDeal.h"

inline Mat create_LOG(int size, double sigma);
inline vector<double> create_sigma(double start, double step, double end);
inline vector<Mat> create_scale_space(Mat &im_in, vector<double> &sigma);
inline vector<struct blob> detect_blobs(Mat &im_in, double thresh, vector<double> et, int padding = 10);
inline vector<struct blob> prune_blobs(vector<struct blob> blobs_in);









//self use
float calculateAccuracyPercent(const Mat &original, const Mat &predicted)					//测试精度
{
	return 100 * (float)countNonZero(original == predicted) / predicted.rows;
}

//self use end
void exColor_Invert(Mat& src)
{
	if (src.empty()) return;
	
	const int width = (int)src.cols;
	const int height = (int)src.rows;
	const int widthStep = (int)src.step;
	uchar* idata = src.data;

	int test = 0;

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < widthStep; w++)
		{
			
			(unsigned char)idata[w] = 255 - (unsigned char)idata[w];
			
		
		}
		idata += widthStep;
	}

}

void exdrawArrow(Mat& img, Point pStart, Point pEnd, int len, int alpha, Scalar& color, int thickness, int lineType)
{
	const double PI = 3.1415926;
	Point arrow;
	double angle = atan2((double)(pStart.y - pEnd.y), (double)(pStart.x - pEnd.x));
	line(img, pStart, pEnd, color, thickness, lineType); 
	arrow.x = (int)(pEnd.x + len * cos(angle + PI * alpha / 180));
	arrow.y = (int)(pEnd.y + len * sin(angle + PI * alpha / 180));
	line(img, pEnd, arrow, color, thickness, lineType);
	arrow.x = (int)(pEnd.x + len * cos(angle - PI * alpha / 180));
	arrow.y = (int)(pEnd.y + len * sin(angle - PI * alpha / 180));
	line(img, pEnd, arrow, color, thickness, lineType);
	/**/
	return;
}

void find_gravity_center(const Mat& src, Point &p)
{
	if (src.empty()) return;

	const int width = (int)src.cols;
	const int height = (int)src.rows;
	const int widthStep = (int)src.step;
	uchar* idata = src.data;
	long x = 0, y = 0, sum = 1;

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < widthStep; w++)
		{
			if (255 == idata[w])
			{
				x += w;
				y += h;
				sum += 1;
			}
		}
		idata += widthStep;
	}

	p.x = (int)(x / sum);
	p.y = (int)(y / sum);
}

void emerge_rows(const Mat& src1, const Mat& src2, Mat& dst)
{
	if (src1.empty() ||
		src2.empty() ||
		src1.cols != src2.cols ||
		src1.type() != src2.type()
		)
	{
		return;
	}

	Mat tmp((src1.cols), (src2.rows + src1.rows), src1.type(), Scalar(0, 0, 0));

	const int width = (int)src1.cols;
	const int height1 = (int)src1.rows;
	const int height2 = (int)src2.rows;
	const int widthStep = (int)src1.step;
	uchar* idata1 = src1.data;
	uchar* idata2 = src2.data;
	uchar* ddata = tmp.data;

	for (int h = 0; h < height1; h++)
	{
		for (int w = 0; w < widthStep; w++)
		{
			ddata[w] = idata1[w];
		}
		idata1 += widthStep;
		ddata  += widthStep;
	}


	for (int h = 0; h < height2; h++)
	{
		for (int w = 0; w < widthStep; w++)
		{
			ddata[w] = idata2[w];
		}
		idata2 += widthStep;
		ddata += widthStep;
	}
	dst = tmp.clone();
}

double sigmoid(double x)
{
	return (1 / (1 + pow(EXP, 0 - x)));
}

double Tanh(double z)
{
	double x = pow(EXP, 2 * z);
	return (x - 1) / (x + 1);
}

double covariance(void)
{
	return 1.0;
}


vector<double> create_sigma(double start, double step, double end)
{
	vector<double> sigma;
	while (start <= end + 1e-8){
		double s = exp(start);
		sigma.push_back(s);
		start += step;
	}
	return sigma;
}

Mat create_LOG(int size, double sigma)
{
	Mat H(size, size, CV_64F);
	int cx = (size - 1) / 2;
	int cy = (size - 1) / 2;
	double sum = 0;
	for (int i = 0; i < H.rows; i++)
	{
		for (int j = 0; j < H.cols; j++)
		{
			int nx = i - cx;
			int ny = j - cy;
			double s = -1 / (3.1415926 * sigma*sigma*sigma*sigma);
			s = s* (1 - (nx*nx + ny*ny) / (2 * sigma*sigma));
			s = s*exp(-(nx*nx + ny*ny) / (2 * sigma*sigma));
			sum += s;
			H.at<double>(i, j) = s;
		}
	}

	double mean = sum / (size*size);

	for (int i = 0; i < H.rows; i++)
	{
		for (int j = 0; j < H.cols; j++)
		{
			H.at<double>(i, j) -= mean;
		}
	}

	return H;
}

//
vector<Mat> create_scale_space(Mat &im_in, vector<double> &sigma)
{
	vector<Mat> scale_space;

	for (int i = 0; i < sigma.size(); i++)
	{

		int n = ceil(sigma[i] * 3) * 2 + 1;
		Mat LOG = create_LOG(n, sigma[i]);
		Mat dst;

		filter2D(im_in.mul(sigma[i] * sigma[i]), dst, -1, LOG, Point(-1, -1));
		scale_space.push_back(dst);
	}

	return scale_space;
}

//找到blob
vector<struct blob> detect_blobs(Mat &im_in, double thresh, vector<double> et, int padding)
{
	vector<struct blob> blobs;
	Mat addborder, norm;
	copyMakeBorder(im_in, addborder, padding, padding, padding, padding, BORDER_CONSTANT, Scalar(255));
	normalize(addborder, norm, 1, 0, NORM_MINMAX, CV_64F);
	vector<Mat> all_ims = create_scale_space(norm, et);

	for (int i = 0; i < et.size(); i++)
	{
		Mat grayscale, mx;
		normalize(all_ims[i], grayscale, 0, 255, NORM_MINMAX, CV_8U, Mat());
		Mat structedElement(3, 3, CV_8U, Scalar(1));
		dilate(grayscale, mx, structedElement);
		grayscale = (grayscale == mx) & (all_ims[i] > thresh);	   //取大于threshold并且是周围最大
		for (int j = 0; j < norm.rows; j++)
		{
			for (int k = 0; k < norm.cols; k++)
			{
				if (grayscale.at<unsigned char>(j, k) > 0)
				{
					struct blob b;
					b.x = j - padding;
					b.y = k - padding;
					b.sigma = et[i];
					b.intensity = all_ims[i].at<double>(j, k);
					blobs.push_back(b);
				}
			}
		}
	}
	return blobs;
}

//去掉重复的blob
vector<struct blob> prune_blobs(vector<struct blob> blobs_in)
{
	vector<bool>keep(blobs_in.size(), true);

	for (int i = 0; i < blobs_in.size(); i++)
	{
		for (int j = 0; j < blobs_in.size(); j++)
		{
			if ((i == j) || (keep[i] == false) || (keep[j] == false))
				continue;

			struct blob blobi = blobs_in[i];
			struct blob blobj = blobs_in[j];

			int xi = blobi.x;
			int yi = blobi.y;
			double ri = blobi.sigma;
			int xj = blobj.x;
			int yj = blobj.y;
			double rj = blobj.sigma;

			double d = sqrt((xj - xi)*(xj - xi) + (yj - yi)*(yj - yi));
			double rirj = ri + rj;
			double overlap_percent = rirj / d;
			if (overlap_percent > 2.0)
			{
				if (blobi.intensity > blobj.intensity)
				{
					keep[i] = true;
					keep[j] = false;
				}
				else {
					keep[i] = false;
					keep[j] = true;
				}
			}
		}
	}
	vector<struct blob> blobs_out;

	for (int i = 0; i < blobs_in.size(); i++)
		if (keep[i])
			blobs_out.push_back(blobs_in[i]);

	return blobs_out;
}


vector<struct blob> get_blobs(Mat& src, double thresh, double start, double step, double end)
{
	vector<double> sigma = create_sigma(start, step, end);
	vector<struct blob> blobs = detect_blobs(src, thresh, sigma);
	return prune_blobs(blobs);	
}

//MLP
void calcGradientFeat(const Mat& imgSrc, vector<float>& feat)
{
	float sumMatValue(const Mat& image);
	// 计算图像中像素灰度值总和 

	Mat image;
	cvtColor(imgSrc, image, CV_BGR2GRAY);
	resize(image, image, Size(8, 16));


	// 计算x方向和y方向上的滤波 
	float mask[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	Mat y_mask = Mat(3, 3, CV_32F, mask) / 8;
	Mat x_mask = y_mask.t();
	// 转置 
	Mat sobelX, sobelY;

	filter2D(image, sobelX, CV_32F, x_mask);
	filter2D(image, sobelY, CV_32F, y_mask);

	sobelX = abs(sobelX);
	sobelY = abs(sobelY);

	float totleValueX = sumMatValue(sobelX);
	float totleValueY = sumMatValue(sobelY);


	// 将图像划分为4*2共8个格子，计算每个格子里灰度值总和的百分比 
	for (int i = 0; i < image.rows; i = i + 4)
	{
		for (int j = 0; j < image.cols; j = j + 4)
		{
			Mat subImageX = sobelX(Rect(j, i, 4, 4));
			feat.push_back(sumMatValue(subImageX) / totleValueX);
			Mat subImageY = sobelY(Rect(j, i, 4, 4));
			feat.push_back(sumMatValue(subImageY) / totleValueY);
		}
	}
}

float sumMatValue(const Mat& image)
{
	float sumValue = 0;
	int r = image.rows;
	int c = image.cols;
	if (image.isContinuous())
	{
		c = r*c;
		r = 1;
	}
	for (int i = 0; i < r; i++)
	{
		const uchar* linePtr = image.ptr<uchar>(i);
		for (int j = 0; j < c; j++)
		{
			sumValue += linePtr[j];
		}
	}
	return sumValue;
}

void Histogram(const Mat& src, Mat& dst)
{
	dst = src.clone();
	vector<Mat> splitBGR(src.channels());
	//分割通道，存储到splitBGR中
	split(src, splitBGR);
	//对各个通道分别进行直方图均衡化
	for (int i = 0; i < src.channels(); i++)
		equalizeHist(splitBGR[i], splitBGR[i]);
	//合并通道
	merge(splitBGR, dst);
}

int get_OTSU_value(const Mat& src)
{
	const int GrayScale = 256;
	const int width = src.cols;
	const int height = src.rows;
	int pixelCount[GrayScale] = { 0 };
	double pixelPro[GrayScale] = { 0 };
	int i, j, pixelSum = width * height, threshold = 0;
	uchar* idata = src.data;  //指向像素数据的指针

	//统计灰度级中每个像素在整幅图像中的个数  
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[(int)idata[i * width + j]]++;  //将像素值作为计数数组的下标
		}
	}

	//计算每个像素在整幅图像中的比例  
	double maxPro = 0.0;
	int kk = 0;
	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (double)pixelCount[i] / pixelSum;
		if (pixelPro[i] > maxPro)
		{
			maxPro = pixelPro[i];
			kk = i;
		}
	}


	//遍历灰度级[0,255]  
	double w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < GrayScale; i++)     // i作为阈值
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < GrayScale; j++)
		{
			if (j <= i)   //背景部分  
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //前景部分  
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		deltaTmp = w0 * w1 * abs(u0 - u1);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}
	return threshold;
}







void CreateDoubleMatrix(double **&matrix, cv::Size size)
{
	matrix = new double*[size.height];
	for (int iInd = 0; iInd < size.height; iInd++)
		matrix[iInd] = new double[size.width];
}

int CreateGeoMatchModel(const Mat& templateArr, double maxContrast, double minContrast)
{
	if (CV_8UC1 != templateArr.type())
	{
		return 0;
	}
#if 0

	Mat src = templateArr.clone();
	CvSize Ssize;

	// Convert IplImage to Matrix for integer operations
	//CvMat srcstub, *src = (CvMat*)templateArr;



	// set width and height
	Ssize.width = src.cols;
	Ssize.height = src.rows;
	modelHeight = src.rows;		//Save Template height
	modelWidth = src.cols;			//Save Template width

	noOfCordinates = 0;											//initialize
	if (cordinates) delete cordinates;
	cordinates = new Point[modelWidth *modelHeight];		//Allocate memory for coorinates of selected points in template image

	if (edgeMagnitude) delete edgeMagnitude;
	edgeMagnitude = new double[modelWidth *modelHeight];		//Allocate memory for edge magnitude for selected points

	if (edgeDerivativeX) delete edgeDerivativeX;
	edgeDerivativeX = new double[modelWidth *modelHeight];			//Allocate memory for edge X derivative for selected points

	if (edgeDerivativeY) delete edgeDerivativeY;
	edgeDerivativeY = new double[modelWidth *modelHeight];			////Allocate memory for edge Y derivative for selected points


	// Calculate gradient of Template
	Mat gx(Ssize.height, Ssize.width, CV_16SC1);		//create Matrix to store X derivative
	Mat gy(Ssize.height, Ssize.width, CV_16SC1);		//create Matrix to store Y derivative
	cvSobel(src, gx, 1, 0, 3);		//gradient in X direction			
	cvSobel(src, gy, 0, 1, 3);	//gradient in Y direction

	Mat nmsEdges(Ssize.height, Ssize.width, CV_32F);		//create Matrix to store Final nmsEdges
	const short* _sdx;
	const short* _sdy;
	double fdx, fdy;
	double MagG, DirG;
	double MaxGradient = -99999.99;
	double direction;

	int *orients = new int[Ssize.height *Ssize.width];
	int count = 0, i, j; // count variable;

	double **magMat;
	CreateDoubleMatrix(magMat, Ssize);

	for (i = 1; i < Ssize.height - 1; i++)
	{
		for (j = 1; j < Ssize.width - 1; j++)
		{
			_sdx = (short*)(gx->data.ptr + gx->step*i);
			_sdy = (short*)(gy->data.ptr + gy->step*i);
			fdx = _sdx[j]; fdy = _sdy[j];        // read x, y derivatives

			MagG = sqrt((float)(fdx*fdx) + (float)(fdy*fdy)); //Magnitude = Sqrt(gx^2 +gy^2)
			direction = cvFastArctan((float)fdy, (float)fdx);	 //Direction = invtan (Gy / Gx)
			magMat[i][j] = MagG;

			if (MagG > MaxGradient)
				MaxGradient = MagG; // get maximum gradient value for normalizing.


			// get closest angle from 0, 45, 90, 135 set
			if ((direction > 0 && direction < 22.5) || (direction >157.5 && direction < 202.5) || (direction>337.5 && direction < 360))
				direction = 0;
			else if ((direction > 22.5 && direction < 67.5) || (direction >202.5 && direction < 247.5))
				direction = 45;
			else if ((direction > 67.5 && direction < 112.5) || (direction>247.5 && direction < 292.5))
				direction = 90;
			else if ((direction > 112.5 && direction < 157.5) || (direction>292.5 && direction < 337.5))
				direction = 135;
			else
				direction = 0;

			orients[count] = (int)direction;
			count++;
		}
	}

	count = 0; // init count
	// non maximum suppression
	double leftPixel, rightPixel;

	for (i = 1; i < Ssize.height - 1; i++)
	{
		for (j = 1; j < Ssize.width - 1; j++)
		{
			switch (orients[count])
			{
			case 0:
				leftPixel = magMat[i][j - 1];
				rightPixel = magMat[i][j + 1];
				break;
			case 45:
				leftPixel = magMat[i - 1][j + 1];
				rightPixel = magMat[i + 1][j - 1];
				break;
			case 90:
				leftPixel = magMat[i - 1][j];
				rightPixel = magMat[i + 1][j];
				break;
			case 135:
				leftPixel = magMat[i - 1][j - 1];
				rightPixel = magMat[i + 1][j + 1];
				break;
			}
			// compare current pixels value with adjacent pixels
			if ((magMat[i][j] < leftPixel) || (magMat[i][j] < rightPixel))
				(nmsEdges->data.ptr + nmsEdges->step*i)[j] = 0;
			else
				(nmsEdges->data.ptr + nmsEdges->step*i)[j] = (uchar)(magMat[i][j] / MaxGradient * 255);

			count++;
		}
	}


	int RSum = 0, CSum = 0;
	int curX, curY;
	int flag = 1;

	//Hysterisis threshold
	for (i = 1; i < Ssize.height - 1; i++)
	{
		for (j = 1; j < Ssize.width; j++)
		{
			_sdx = (short*)(gx->data.ptr + gx->step*i);
			_sdy = (short*)(gy->data.ptr + gy->step*i);
			fdx = _sdx[j]; fdy = _sdy[j];

			MagG = sqrt(fdx*fdx + fdy*fdy); //Magnitude = Sqrt(gx^2 +gy^2)
			DirG = cvFastArctan((float)fdy, (float)fdx);	 //Direction = tan(y/x)

			////((uchar*)(imgGDir->imageData + imgGDir->widthStep*i))[j]= MagG;
			flag = 1;
			if (((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j]) < maxContrast)
			{
				if (((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j]) < minContrast)
				{

					(nmsEdges->data.ptr + nmsEdges->step*i)[j] = 0;
					flag = 0; // remove from edge
					////((uchar*)(imgGDir->imageData + imgGDir->widthStep*i))[j]=0;
				}
				else
				{   // if any of 8 neighboring pixel is not greater than max contraxt remove from edge
					if ((((double)((nmsEdges->data.ptr + nmsEdges->step*(i - 1)))[j - 1]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i - 1)))[j]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i - 1)))[j + 1]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j - 1]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j + 1]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i + 1)))[j - 1]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i + 1)))[j]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i + 1)))[j + 1]) < maxContrast))
					{
						(nmsEdges->data.ptr + nmsEdges->step*i)[j] = 0;
						flag = 0;
						////((uchar*)(imgGDir->imageData + imgGDir->widthStep*i))[j]=0;
					}
				}

			}

			// save selected edge information
			curX = i;	curY = j;
			if (flag != 0)
			{
				if (fdx != 0 || fdy != 0)
				{
					RSum = RSum + curX;	CSum = CSum + curY; // Row sum and column sum for center of gravity

					cordinates[noOfCordinates].x = curX;
					cordinates[noOfCordinates].y = curY;
					edgeDerivativeX[noOfCordinates] = fdx;
					edgeDerivativeY[noOfCordinates] = fdy;

					//handle divide by zero
					if (MagG != 0)
						edgeMagnitude[noOfCordinates] = 1 / MagG;  // gradient magnitude 
					else
						edgeMagnitude[noOfCordinates] = 0;

					noOfCordinates++;
				}
			}
		}
	}

	centerOfGravity.x = RSum / noOfCordinates; // center of gravity
	centerOfGravity.y = CSum / noOfCordinates;	// center of gravity

	// change coordinates to reflect center of gravity
	for (int m = 0; m < noOfCordinates; m++)
	{
		int temp;

		temp = cordinates[m].x;
		cordinates[m].x = temp - centerOfGravity.x;
		temp = cordinates[m].y;
		cordinates[m].y = temp - centerOfGravity.y;
	}

	////cvSaveImage("Edges.bmp",imgGDir);

	// free alocated memories
	delete[] orients;
	////cvReleaseImage(&imgGDir);
	cvReleaseMat(&gx);
	cvReleaseMat(&gy);
	cvReleaseMat(&nmsEdges);

	ReleaseDoubleMatrix(magMat, Ssize.height);

	modelDefined = true;


#endif
	return 0;

}




