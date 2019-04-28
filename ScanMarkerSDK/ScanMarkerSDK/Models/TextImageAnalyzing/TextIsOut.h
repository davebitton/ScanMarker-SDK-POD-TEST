#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "types.h"
#endif
#include <stdio.h>
#include <math.h>
#include <ctype.h>

class TextIsOut
{
public:

	int m_width;
	int m_height;
	byte *m_pDataIn;
	byte *m_pDataOut;
	byte *m_pDataOut1;
	byte *m_pWorkData;
	int m_numberChanges;
	int m_goodNumberWhitesPerChanges;
	int m_numberChangesFound;
	int m_goodPerBadColumn;
	int m_badsPerColumnPercent;
	int m_numberBadsForFail;
	int m_TextOut_first_i;
	int m_number_i;
	bool m_result;
	int m_numberBadColumns;
	bool m_isStart;

	TextIsOut();
	~TextIsOut();
	void CalculateTextIsOut();
	void Init(int width, int height, byte* pDataIn, byte* pDataOut, byte* pDataOut1);
	void SetParameters(int numberChanges, int goodNumberWhitesPerChanges, int goodPerBadColumn, int badsPerColumnPercent, int numberBadsForFail);
	void ReadCfgFile(char* dir);

	int _UP_EDGE_;
	int _DOWN_EDGE_;
	float _BAD_RESULT_BODY_PERCENT_;
	int _HALF_NUMBER_CHANGES_TO_ANALIZE_;
	int _MAX_NUMBER_BAD_COLUMNS_;
	int _MIN_RATIO_BLACK_TO_WHITE_;
	int _MIN_LENGTH_OF_BLACK_;
	int _SEARCH_START_MIDDLE_;
	int _SEARCH_START_RANGE_;

private:
	int *m_pChangesCoord[128];
	int *m_pNumberWhite[128];
	int *m_pNumberBlack[128];
	bool *m_isFull[128];
	bool m_isInit;
	bool m_isFullInfo;
	int m_activeChangesIndex;
	int m_j;
	int m_histogram[128];
	bool m_isFirstHistogram[3];
	int m_middle[3];
	int m_up[3];
	int m_down[3];
	int *m_numberChangesInRow[2];
	int m_bodyHeight;
	int m_From[3];
	int m_To[3];
	int m_aveHeight;
	int m_numberInAve;
	int m_widthOfInterest;
	


	void AllocateMemory();
	void ChangeFound(int jj, bool isLast);
	void CalculateColor();
	void SummaryByColumns(int cSrc, int cDst1, int cDst2);
	void CalculateNumberGoodsBads(int i, int &numberGoods, int &numberBads);
	void CalculateColumnColors(int i, int &numberWhites, int &numberBlacks,  int &numberChanges);
	void ColorSpread(int jj, int j_step, int cDst, int cSrc);
	//void SpreadAround(int i, int j, int cSrc, int cDest);
	void CalculateHistogram(int jj, int k);
	void FillFromHistogram(int jj, int i1, int i2, int k);
	int CalculateBodyHeight();
	int CalculateWidthOfInterest();


};
