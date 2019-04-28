#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "Include/types_hebrewdll.h"
#endif
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#pragma once
#define MAX_NUMBER_WORDS 200
#define MAX_NUMBER_HEBREW_STEPS 200
class HebrewItalic
{
public:
	HebrewItalic(void);
	~HebrewItalic(void);

	int m_width;
	int m_height;
	int m_output_width;
	int m_output_width_x2;
	int m_output_height;
	byte *m_pDataIn;
	byte *m_pDataOut;
	byte * m_pWorkData;
	byte * m_pWorkData1;
	byte * m_pWorkData2;
	int * m_pWorkDataShifts[MAX_NUMBER_HEBREW_STEPS];
	
	float* m_pInterpolationOutput;
	float* m_pInterpolationInput;
	int m_beginWords[MAX_NUMBER_WORDS];
	int m_endWords[MAX_NUMBER_WORDS];
	int m_beginWordsCopy[MAX_NUMBER_WORDS];
	int m_endWordsCopy[MAX_NUMBER_WORDS];
	int m_beginWordsOut[MAX_NUMBER_WORDS];
	int m_endWordsOut[MAX_NUMBER_WORDS];
	int m_numberWords;
	byte *m_pBody;
	int *m_body;
	int *m_bodyHeight;
	int m_aveBodyHeight;
	int *m_pChangesCoord[1000];
	int m_activeChangesIndex;
	int m_begin_x;
	int m_end_x;
	int m_begin_y;
	int m_end_y;
	int m_begin_y_1;
	int m_end_y_1;
	int m_italicStep;
	bool m_notToDoItalic;
	int m_language;

	int s_maxHeightSquareArr[MAX_NUMBER_WORDS];
	
	float		 m_heightSquare[MAX_NUMBER_WORDS][MAX_NUMBER_HEBREW_STEPS];
	int s_maxHeightSquareMarkArr[MAX_NUMBER_WORDS];
	char m_dirname[256];
	char m_lettersFileName[MAX_NUMBER_WORDS][30];
	int m_MAX_HEBREW_S;
	int _NUMBER_CHANGES_TO_ANALYZE_;
	int _HALF_NUMBER_CHANGES_TO_ANALIZE_;
	int m_dataInMode;
	int m_dataOutMode;
	void Init(int width, int height, int outputWidth,int outputHeight,byte* pDataIn, byte* pDataOut);
	void ReadCfgFile(char* dir);
	void DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue = -10000, float maxValue=-10000);
	int Calculate();
	void AllocateMemory();
	void FindLetters(void* dataIn, int dataInMode,  int iTextColor, int*  pDataInShifte);
	void CalculateBody();
	void CreateLetterItalicFile(int w);
	void StepItalic(void* dataIn, int dataInMode, void* pWorkData, int workDataMode, int beginWord, int endWord, int h1, int h2, int nSteps, int* &pWorkDataShifts);
	float CalculateHeightSquare(void* dataIn, int dataInMode, int w, int iTextColor, int jStep);
	void CalculateOne(int letter);
	void LoadInputFile(char* fileName);
	void CalculateHeightSquareMark(int w);
	void PrintDebugStr(char* str, bool append=true);
	void MakeLinesThinner();

};
