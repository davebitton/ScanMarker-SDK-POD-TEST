#ifndef _INPUT_H_
#define _INPUT_H_
#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#endif

#include <stdio.h>
#include <math.h>
#include <ctype.h>

int GetDataInValue(void* dataIn, int dataInMode,  int i, int j, int width,  int height, int* pShifts = NULL);
void SetDataValue(void* dataOut, int dataOutMode, int i, int j, int value, int width,  int height);
bool IsText(int value, int iTextColor);
void CopyData(void* dataIn, int dataInMode,  
			 void* dataOut, int dataOutMode, 
			 int i1, int i2, int j1, int j2, 
			 int width,  int height);
extern int m_spaceLeft;
extern int m_spaceUp;
extern int m_spaceBottom;



#endif