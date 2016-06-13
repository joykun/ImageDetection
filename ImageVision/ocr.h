

//#include "stdafx.h"
//#include <leptonica\allheaders.h>  
#include <baseapi.h>
#include <iostream>
#include <stdlib.h>

#pragma comment(lib,"tesseract\\lib\\x64\\libtesseract304.lib")

#include <vector>
#include <iostream>
using namespace std;


#ifndef __OCR_DEAL__
#define __OCR_DEAL__

class Ocr
{
public:
	Ocr();
	virtual ~Ocr();

	tesseract::TessBaseAPI ocr;


	int WhatKind;
	std::string m_string;


};

#endif