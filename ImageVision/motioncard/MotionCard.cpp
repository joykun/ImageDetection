#pragma once
#include "MotionCard.h"







Motion::~Motion()
{
}


Motion::Motion()
	:CardSelect(0)
	, isInitOK(false)
	, OldBit(0)
	, NewBit(0)
{
}


bool Motion::init()
{
	int cardno = 0;
	int tmp = 0;

	m_instantDiCtrl = AdxInstantDiCtrlCreate();
	m_instantDoCtrl = AdxInstantDoCtrlCreate();
	if ((m_instantDiCtrl != NULL) && (NULL != m_instantDoCtrl))
	{
		cardno++;
		isInitOK = true;

		int deviceNum = 0;
		DeviceInformation devInfo(deviceNum);

		ErrorCode errorCode = m_instantDiCtrl->setSelectedDevice(devInfo);
		if (errorCode != Success)
		{
			//CheckError(errorCode);
			return false;
		}
		m_instantDiCtrl->getSelectedDevice(devInfo);
		errorCode = m_instantDoCtrl->setSelectedDevice(devInfo);
		if (errorCode != Success)
			return false;//CheckError(errorCode);
		m_instantDoCtrl->getSelectedDevice(devInfo);

		m_IportCount = m_instantDiCtrl->getFeatures()->getPortCount();
		m_portData = new byte[m_IportCount];
		memset(m_portData, 0, m_IportCount);


		int portcount = m_instantDoCtrl->getFeatures()->getPortCount();
		errorCode = m_instantDiCtrl->setSelectedDevice(devInfo);
		if (errorCode != Success)
		{
			return false;
		}
		m_instantDiCtrl->getSelectedDevice(devInfo);
		errorCode = m_instantDoCtrl->setSelectedDevice(devInfo);
		if (errorCode != Success)
		{
			return false;
		}
		m_instantDoCtrl->getSelectedDevice(devInfo);
	}
	else
	{
		isInitOK = false;
		return false;
	}
	if (0 == cardno) return false;
	else return true;
}

bool Motion::Read_Input(short x)
{
	if (!isInitOK) return false;
	ErrorCode errorCode;
	uint8 outportValue;
	errorCode = m_instantDoCtrl->Read(0, x, &outportValue);
	return !outportValue;
}

void Motion::Write_Output(short x, short LowOrHigh)
{
	//OutStatusToPort(0,0x02);正面
	//OutStatusToPort(0,0x04);反面
	if (!isInitOK) return;
	uint32 port = (uint32)x;
	uint8 bValue = (uint8)LowOrHigh;
	ErrorCode errorCode = m_instantDoCtrl->Write(port, bValue);
	
}

bool Motion::Get_Output(short x)
{
	if (!isInitOK) return false;
	return false;
	/*
	ErrorCode errorCode = m_instantDoCtrl->Write(x, LowOrHigh);
	CheckError(errorCode);
	*/
}

bool Motion::Triger()
{
	if (!isInitOK) return false;

	int portcount = m_instantDoCtrl->getFeatures()->getPortCount();

	m_instantDiCtrl->Read(0,4, &NewBit);

	if ((NewBit != OldBit) && NewBit)
	{
		OldBit = NewBit;
		return true;
	}
	else
	{
		OldBit = NewBit;
		return false;
	}
}

void Motion::Front()
{
	if (!isInitOK) return;
	ErrorCode errorCode = m_instantDoCtrl->Write(0, 0x02);
}


void Motion::Back()
{
	if (!isInitOK) return;
	ErrorCode errorCode = m_instantDoCtrl->Write(0, 0x04);

}

void Motion::Reset()
{
	if (!isInitOK) return;
	ErrorCode errorCode = m_instantDoCtrl->Write(0, 0x00);
}