#pragma once

#include <vector>
using namespace std;


#ifndef __MOTION_CARD__
#define __MOTION_CARD__
#include "bdaqctrl.h"
using namespace Automation::BDaq;




class Motion
{
public:
	Motion();
	~Motion();

public:
	bool Read_Input(short x);
	void Write_Output(short x, short LowOrHigh);
	bool Get_Output(short x);
	bool init();
	vector<string> motioncardlist;
	int CardSelect;
	int isInitOK;
	uint8 OldBit;
	uint8 NewBit;
	bool Triger();

	void Front();

	void Back();

	void Reset();

	int m_IportCount;
	int m_portNum0;
	byte* m_portData;


	InstantDiCtrl		*m_instantDiCtrl;
	InstantDoCtrl		*m_instantDoCtrl;
#ifdef _BDAQ_TYPES_DEFINED

#define Error_Code ErrorCode 
#endif
	
};



#endif



