#include "StdAfx.h"
#include <stdio.h>
#include "InOut.h"
#ifdef _SCANMARKER_WINDOWS_
#else
#include "Include/types_hebrewdll.h"
#endif
//#include "..\SplineInterpolation\SplineInterpolation.h"
#define SPACE_LEFT_DATA_IN 0
#define SPACE_UP_DATA_IN 0
#define SPACE_BOTTOM_DATA_IN 0


//#define TextColor 255

#define White 255
#define Black 0

int m_spaceLeft = SPACE_LEFT_DATA_IN;
int m_spaceUp = SPACE_UP_DATA_IN;
int m_spaceBottom = SPACE_BOTTOM_DATA_IN;

int GetDataInValue(void* dataIn, int dataInMode,  int ii, int j, int width,  int height, int* pShifts)
{		int value = -10000;
		int i = ii;
		if (j < 0) j = 0;
		if (j >= height-1) j = height-1;
		if (pShifts)
		{
			i += pShifts[j];
		}
		if (i < 0) i = 0;
		if (i >= width-1 ) i = width-1;

		if (dataInMode == 0)
		{
			value = ((byte*)dataIn)[3*j*width+i*3];
		}
		else if (dataInMode == 1)
		{
			value = ((byte*)dataIn)[j*width+i];
		}
		else if (dataInMode== 2)
		{
			value = ((int*)dataIn)[j*width+i];
		}
		else if (dataInMode== 3)
		{
			value = ((int*)dataIn)[j*3*width+i*3 + 0];
		}

		return value;
}
void CopyData(void* dataIn, int dataInMode,  
			 void* dataOut, int dataOutMode, 
			 int i1, int i2, int j1, int j2, 
			 int width,  int height)
{
	for ( int i = i1; i < i2; i++)
	{
		for ( int j = j1; j < j2; j++)
		{
			int value = GetDataInValue(dataIn, dataInMode,  i, j, width,  height);
			SetDataValue(dataOut, dataOutMode, i, j, value, width,  height);
		}
	}
}

void SetDataValue(void* dataOut, int dataOutMode, int i, int j, int value, int width,  int height)
{
	if ( (i < 0) || (i >= width) || (j < 0) || ( j >= height)) return;
	if (dataOutMode == 0)
	{
		((byte*)dataOut)[3*j*width+3*(i)+0] = value;
		((byte*)dataOut)[3*j*width+3*(i)+1] = value;
		((byte*)dataOut)[3*j*width+3*(i)+2] = value;
		
	}
	else if (dataOutMode == 1)
	{
		((byte*)dataOut)[j*width+i] = value;
	}
	else if (dataOutMode == 2)
	{
		((int*)dataOut)[j*width+i] = value;
	}
	else if (dataOutMode == 3)
	{
		((int*)dataOut)[3*j*width+3*(i)+0] = value;
		((int*)dataOut)[3*j*width+3*(i)+1] = value;
		((int*)dataOut)[3*j*width+3*(i)+2] = value;
	}
}

bool IsText(int value, int iTextColor)
{
	if (value == iTextColor)
	{
		return true;
	}
	else
		return false;
}