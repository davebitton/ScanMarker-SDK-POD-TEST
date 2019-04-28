#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "Include/types_hebrewdll.h"
#endif
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "Fonts.h"

#pragma once
#define TextColor 255

class CCheckFonts
{
public:
	int m_width;
	int m_height;
	float m_factorToApply;
	char m_fontApplied[100];
	byte* m_pDataIn;
	CFonts* m_fontsToCheck;

	float m_widthHorizontalArray[1000];
	float m_widthVerticalArray[1000];
	float m_widthBgHorizontalArray[1000];
	float m_widthHorizontalLow;
	float m_widthHorizontalHigh;
	float m_widthVertical;
	float m_widthBgHorizontal;

	int m_widthHorizontalLow_1;
	int m_widthHorizontalLow_2;
	int m_widthHorizontalHigh_1;
	int m_widthHorizontalHigh_2;
	int m_widthVertical_1;
	int m_widthVertical_2;
	int m_widthBgHorizontal_1;
	int m_widthBgHorizontal_2;

	void Init( int width, int height, byte* pDataIn);
	void Release();
	
    void Calculate();
	void WriteDebugStr(char * str, bool isAppend = true);
	void WriteTablesDebug(char* filename_basic);


	CCheckFonts(void);
	~CCheckFonts(void);
};
