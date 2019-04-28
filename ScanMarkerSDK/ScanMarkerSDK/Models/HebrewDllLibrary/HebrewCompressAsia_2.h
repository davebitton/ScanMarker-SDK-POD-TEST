#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "Include/types_hebrewdll.h"
#endif
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#pragma once

#define BgColor 0
#define TextColor 255
#define _MAX_IMAGE_HEIGHT_  600
#define _MAX_NUMBER_LETTERS_ 200
class CCheckFonts;

class CHebrewCompressAsia_2
{
public:
	CHebrewCompressAsia_2(void);
	~CHebrewCompressAsia_2(void);

	void Init(bool corierActive, int width, int height, int outputWidth,byte* pDataIn, byte* pDataOut, byte* pDataIn3=NULL);
	void ReadCfgFile(char* dir);
	void WriteDebugStr(char * str, bool isAppend = true);
	int Calculate(void *job=NULL, int* d=NULL, int* o=NULL);
	void AllocateMemory();
	void CalculateImageBoundaries();
	void DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue = -10000, float maxValue=-10000);
	void Compress(int isTempOutput, int flag = 0);
	void CalculateWidthLines();
	void CalculateWidthLinesKorean();
	void CalculateWidthPerChange(int window, int step);
	void FixByWidthWordKorean();
	void FixByWidthWordChina();
	void FixByWidthWordJapaneseVertical();
	void FixByWidthWordJapaneseHorizontal();
	void FixWidthByColorChange();
	float CalculateFactorForWordJapanese(int w);
	void CalculateLetterEdgesVerticalAsia();
	void CalculateCompressRatio();
	//void FixInterpolationOutput(int beginWord, int endWord, int minJ, int maxJ, float ratio);
	int m_wordEdges[100][4];
	int m_letterEdges[_MAX_NUMBER_LETTERS_][4];
	float m_letterWidthLines[_MAX_NUMBER_LETTERS_];
	int m_numberLetters;
	int m_pWorkLData[50];

	bool m_isDuringFix;
	
	int m_width;
	int m_height;
	int m_output_width;
	int m_begin_x;
	int m_end_x;
	int m_begin_y;
	int m_end_y;
	byte *m_pDataIn;
	byte* m_pDataOut;
	byte *m_pDataIn3;
	byte *m_pWorkData;
	byte *m_pWorkData1;
	float *m_pWorkData2;
	byte *m_pWorkData3;
	bool *m_isEmptyColumn;
	int *m_pIndexData;
	int * m_pOppositeIndex;
	float * m_widthPerChange;
	float * m_widthPerChange_1;
	float* m_pInterpolationOutput; 
	float* m_pInterpolationInput;
	float* m_pInterpolationInput1;
	float* m_pInterpolationOutput1;
	float  m_additionalFactor;
	float *m_widthHorizontal;
	float* m_widthVertical;
	char m_debugFileBasicName[1000];
	bool m_isVertical;
	int m_language;
	char b[1024];


};
