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

class CCheckFonts;

class CHebrewCompress_1
{
public:
	CHebrewCompress_1(void);
	~CHebrewCompress_1(void);

	void Init(bool corierActive, int width, int height, int outputWidth,byte* pDataIn, byte* pDataOut, byte* pDataIn3=NULL);
	void ReadCfgFile(char* dir);
	void DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue = -10000, float maxValue=-10000);
	void CalculateColorsToChngesRatio(int jj);
	void CalculateBody();
	void Calculate(void *job=NULL, int* d=NULL, int* o=NULL);
	void AllocateMemory();
	void CalculateImageBoundaries();
	void CalculateWidthPerChange(int window, int step);
	void CalculateWidthPerChanges1(int numberChanges, int step);
	void CalculateColorPerChanges();
	void Compress(int isTempOutput);
	void CalculateFactor_2(bool print = false);
	void AddWidthTCorierFont();
	void WriteDebugStr(char * str, bool isAppend = true);
	int m_pWorkLData[50];


	bool m_corierActive;
	int m_dataInMode;
	int m_width;
	int m_height;
	int m_output_width;
	byte *m_pDataIn;
	byte *m_pDataIn3;
	byte *m_pDataOut;
	byte *m_pWorkData;
	byte *m_pWorkData1;
	byte *m_pWorkData2;
	byte *m_pWorkData3;
	byte *m_pWorkData4;
	byte *m_pWorkData5;
	int *m_compressInterpolation;
	int *m_compressInterpolationOpposite;

	int m_numberChangesAllAve;
	int *m_pIndexData;
	byte *m_pBody;
	int *m_body;
	int *m_bodyHeight;
	float * m_ColorsPerChanges;
	float * m_widthPerChange;
	float * m_widthPerChange1;
	int *m_pChangesCoord[_MAX_IMAGE_HEIGHT_];
	int m_begin_x_1;
	int m_end_x_1;
	int m_begin_x;
	int m_end_x;
	int m_begin_y;
	int m_end_y;
	int m_To;
	float* m_pInterpolationOutput; 
	float* m_pInterpolationInput;
	float* m_pInterpolationInput1;
	float* m_pInterpolationOutput1;
	float CalculateFactor_3();
	CCheckFonts* m_checkFonts;
	float m_fontFactor;
	int m_numberTextColumns;
	int m_aveBodyHeight;
	float m_aveBgToTExtRatio;
	float m_aveAve;
	float m_minWidth;
	float m_maxWidth;

	float m_compressFactor_In;
	bool m_doubleTextPixels;

	int _REFERENCE_BODY_HEIGHT_;
	
	char b[1024];


	int _DEBUG_COMPRESS_WIDTH_;
    int _NUMBER_CHANGES_TO_ANALYZE_;
	int _UP_EDGE_;
	int _DOWN_EDGE_;
	float _ONE_WORD_COMPRESS_FACTOR_;
	bool m_isDoubleWidth;
	int m_language;

};
