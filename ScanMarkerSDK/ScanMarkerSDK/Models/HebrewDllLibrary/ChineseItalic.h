#pragma once
#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "Include/types_hebrewdll.h"
#endif
#define MAX_NUMBER_WORDS 200
#define MAX_NUMBER_HEBREW_STEPS 256
class ChineseItalic
{
public:
	ChineseItalic(void);
	~ChineseItalic(void);


	int m_width;
	int m_height;
	
	int m_begin_x;
	int m_end_x;


	byte *m_pDataIn;
	byte* m_pWorkData;
	int * m_pItalicShifts[MAX_NUMBER_HEBREW_STEPS];
	int m_language;
	
	int m_beginLetters[MAX_NUMBER_WORDS];
	int m_endLetters[MAX_NUMBER_WORDS];
	
	int m_numberLetters;

	void Calculate();
	void AllocateMemory();
	void Init(int width, int height, byte* pDataIn);
	void CalculateItalicShifts( int h1, int h2, int &maxSteps);
	void FindPieceOfImageByNumberChangesinRaw(int numberChangesToFind, int &begin, int &end);
	void CalculateHeightsOfPieceimage(int begin, int end, int &h1, int &h2);
	void CalculateBeginEndOfWholeImage();

};

