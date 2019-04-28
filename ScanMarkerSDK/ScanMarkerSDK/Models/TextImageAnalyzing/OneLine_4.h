#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "types.h"
#endif
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define _MAX_NUMBER_NAHASHIM_ 1000
#define _MAX_NUMBER_LETTERS_ 1000
#define APP_ENGINE_LNG_JAPANESE 		278
class COneLine_4
{
public:

	int m_width;
	int m_width_600;
	int m_height;
	int m_halfWidth;
	int m_halfHeight;
	byte *m_pDataIn;
	byte *m_pDataIn3;
	byte *m_pDataOut;
	byte *m_pDataOut1;
	bool m_isInit;
	int _UP_EDGE_;
	int _DOWN_EDGE_;
	int m_language;
	bool m_isChineseVertical;
	bool m_firstWordBad;
	
	COneLine_4();
	~COneLine_4();
	void Init(int width, int height, byte* pDataIn, byte* pDataOut, byte* pDataOut1);
	void SetParameters(int numberChanges, int goodNumberWhitesPerChanges, int goodPerBadColumn, int badsPerColumnPercent, int numberBadsForFail);
	void ReadCfgFile(char* dir);
	int CalculateWidthOfInterest();
	void Calculate( bool isTable, int language);
	void UnifyNahashim();
	void UnifyLetters();
	void UnifyLettersKorean();
	void ChooseLineFromNahashim();
	void ChooseLineFromNahashim_1();
	void RemoveTableLinesSnakes();
	void RemoveTableVerticalLines();
	void RemoveTableHorizontalLines();
	void RemoveHorizontalLines();
	void FillLetters(byte* dataIn);
	void AddToChosenSnake(int choosenSnake);
	void RemoveGarbageAtBeginAndFinish();
	void TestForFirstWordScannedBad();

	int _HALF_NUMBER_CHANGES_TO_ANALIZE_;
	int _MAX_NUMBER_BAD_COLUMNS_;
	int _MIN_RATIO_BLACK_TO_WHITE_;
	int _MIN_LENGTH_OF_BLACK_;
private:
	int * m_pWorkData;
	int * m_pWorkData1;
	int * m_pWorkData2;
	int * m_pWorkData3;
	int * m_pWorkData4;
	int * m_pWorkData5;
	int * m_pWorkData6;
	int *m_pWorkData7;
	int m_connectedLettersToRight[_MAX_NUMBER_LETTERS_][20][2];
	int m_connectedLettersToLeft[_MAX_NUMBER_LETTERS_][20][2];
	int m_chainToRight[_MAX_NUMBER_LETTERS_][3];
	int m_chainToLeft[_MAX_NUMBER_LETTERS_][3];
	int m_nahashim[_MAX_NUMBER_LETTERS_];
	int m_nahashimEdgesLeft[_MAX_NUMBER_NAHASHIM_][3];
	int m_nahashimEdgesRight[_MAX_NUMBER_NAHASHIM_][3];
	int m_nahashimEdgesUpDown[_MAX_NUMBER_NAHASHIM_][2];
	int m_secondaryNahashim[_MAX_NUMBER_NAHASHIM_];
	int m_tableNahashim[_MAX_NUMBER_NAHASHIM_];
	bool m_snakePresents[_MAX_NUMBER_NAHASHIM_];
	int m_snakeMaxHeightByEachColumn[_MAX_NUMBER_NAHASHIM_];
	int m_snakeMaxWidthByEachRaw[_MAX_NUMBER_NAHASHIM_];
	int m_snakeHeightByAllColumns[_MAX_NUMBER_NAHASHIM_];
public:
	int *m_outputSnakes;
	int m_maxNumberOutputSnakes;
	int m_bodyHeight;
private:	
	int m_begin_x;
	int m_end_x;
	int m_begin_x_1;
	int m_end_x_1;
	int m_max_letters_height;
	int m_begin_y;
	int m_end_y;

	int m_numberLetters;
	void AllocateMemory();
	
	int m_letterEdges[_MAX_NUMBER_LETTERS_][4];
	int m_letterPointUp[_MAX_NUMBER_LETTERS_][2];
	int m_letterPointDown[_MAX_NUMBER_LETTERS_][2];
	int m_letterHeight[_MAX_NUMBER_LETTERS_];
	int m_max_search_width;
	int m_min_letters_height;
	
	void CalculateBodyHeight();
	void CalculateLetters();
	void CalculateAsiaLetters();
	void FindNextAsiaLetters(int &indLetter, int &i_begin, int i);
	void BuildNahashim();
	void AddSpecialLetters(int distance);
	int ChooseNahash();
	int CheckConnectedLetters( int i1, int i2, int j1, int j2);
	void FillChars(byte* dataIn, byte* dataOut);
	void RemoveColumnsInBeginAndEnd();


	void PrintDebugStr(char* str, bool append=true);
};
