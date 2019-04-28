#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "Include/types_hebrewdll.h"
#endif
#include <stdio.h>
#include <math.h>
#include <ctype.h>

class OneLine
{
public:

	int m_width;
	int m_height;
	int m_halfWidth;
	int m_halfHeight;
	byte *m_pDataIn;
	byte *m_pDataStraight;
	byte *m_pDataItalic;
	byte *m_pDataCompressed;
	byte *m_pDataOut;
	byte *m_pDataOut1;
	byte *m_pDataOutCopy;
	byte *m_pDataInCopy;
	int m_start_x;
	int prev_i2;


	int m_numberChanges;
	int m_goodNumberWhitesPerChanges;
	int m_numberChangesFound;
	int m_goodPerBadColumn;
	int m_badsPerColumnPercent;
	int m_numberBadsForFail;
	float *m_pInterpolationInput;
	float *m_pInterpolationOutput;
	int *m_numberChangesInRow[2];
	
	int m_bodyHeight;
	int m_newFrom;
	int m_widthOfInterest;

	bool m_isStart;
	int _UP_EDGE_;
	int _DOWN_EDGE_;
	bool m_result;
	int m_numberBadColumns;

	OneLine();
	~OneLine();
	void Calculate();
	void Init(int width, int height, byte* pDataIn, byte* pDataOut, byte* pDataOut1);
	void SetParameters(int numberChanges, int goodNumberWhitesPerChanges, int goodPerBadColumn, int badsPerColumnPercent, int numberBadsForFail);
	void ReadCfgFile(char* dir);
	int CalculateWidthOfInterest();

	int _HALF_NUMBER_CHANGES_TO_ANALIZE_;
	int _MAX_NUMBER_BAD_COLUMNS_;
	int _MIN_RATIO_BLACK_TO_WHITE_;
	int _MIN_LENGTH_OF_BLACK_;
private:
	int *m_pChangesCoord[1000];
	int *m_pNumberWhite[1000];
	int *m_pNumberBlack[1000];
	bool *m_isFull[1000];
	bool m_isInit;
	bool m_isFullInfo;
	int m_activeChangesIndex;
	int m_j;
	int m_histogram[128];
	bool m_isFirstHistogram[3];
	bool m_isHistogramExists[3];
	int m_middle[3];
	int m_up[3];
	int m_down[3];
	int m_maxHeight[3];
	float m_ratio_min;
	float m_ratio_max;
	int * m_pWorkData;
	int * m_pWorkData1;
	int * m_pWorkData2;


	int m_prev_up[3];
	int m_prev_down[3];
	

	int m_From[3];
	int m_To[3];

	int m_endWord;
	int m_beginWord;
	int *m_verticalHistogram;

	
	void AllocateMemory();
	void ChangeFound(int jj, bool isLast);
	void CalculateColor();
	void SummaryByColumns(int cSrc, int cDst1, int cDst2);
	void CalculateNumberGoodsBads(int i, int &numberGoods, int &numberBads);
	void CalculateColumnColors(int i, int &numberWhites, int &numberBlacks,  int &numberChanges);
	void ColorSpread(int jj, int j_step, int cDst, int cSrc);
	//void SpreadAround(int i, int j, int cSrc, int cDest);
	void CalculateHistogram(int jj, int k);
	//void CalculateVerticalHistogram(byte* data,  int start, int finish, int width, int* verticalHistogram);
	void CalculateVerticalHistogram(void* data, int dataMode, int start, int finish,  int width, int* verticalHistogram, int iTextColor);
	//void CalculateVerticalHistogram(byte* data);
	void FillFromHistogram(int jj, int i1, int i2, int k);
	void SmoothCompressCurve();
	void DrawOutput(void* values, int valuesType, int red, int green, int blue);
	void StraightTextLine(byte* m_pDataOut,void* m_pInterpolationOutput, int outputMode);
	void FindWords();
	void FindNextWord(int *verticalHistogram, int & beginWord, int & endWord, void  *dataIn = 0, int dataMode = 0, int iTextColor = 0, int * ph1 = 0, int * ph2 = 0);
	//void FindNextWord(int *verticalHistogram, int & beginWord, int & endWord);
	void ProcessItalic(void* dataIn, int dataInMode,  void* dataOut, int dataOutMode, int iTextColor);
public:
	int CalculateBodyHeight();
	void PrintDebugStr(char* str, bool append=true);
};
