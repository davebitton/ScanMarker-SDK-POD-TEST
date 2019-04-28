#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "types.h"
#endif
#include <stdio.h>
#include <math.h>
#include <ctype.h>

class SlowLength
{
public:

	int m_bodyHeight;
	int m_width;
	int m_height;
	byte *m_pDataIn;
	byte *m_pDataOut;
	int *m_pDataOutInt;
	
	byte *m_pDataOut1;
	int *m_pDataOutInt1;
	int *m_pDataOutInt2;
	//int m_numberChanges;
	//int m_goodNumberWhitesPerChanges;
	int m_numberChangesFound;
	int *m_numberChangesInRow[2];
	//int m_goodPerBadColumn;
	//int m_badsPerColumnPercent;
	//int m_numberBadsForFail;
	FILE* m_fStatistics;

	bool m_resultTooSlow;
	bool m_resultTooFast;
	int m_numberBadSlowColumns;
	int m_numberGoodsSlowColumns;
	int m_numberBadFastColumns;
	int m_numberGoodsFastColumns;
	
	SlowLength();
	~SlowLength();
	void CalculateSlow();
	void Init(int width, int height, byte* pDataIn, byte* pDataOut, byte* pDataOut1);
	void SetParameters(int numberChanges, int goodNumberWhitesPerChanges, int goodPerBadColumn, int badsPerColumnPercent, int numberBadsForFail, int letterHeight, FILE* fStatistics);

	void ReadCfgFile(char* dir);

	int _LETTER_HEIGHT_;
	int _HALF_NUMBER_CHANGES_TO_ANALIZE_;
	int _GOOD_NUMBER_WHITES_PER_CHANGES_;
	int _GOOD_PER_BAD_COLUMN_;
	int _BAD_PER_COLUMN_PERCENT_;
	int _MAX_NUMBER_BAD_COLUMNS_ ;
	int _RED_BAD_COLOR_THRESHOLD_1;
	int _RED_BAD_COLOR_THRESHOLD_2;
	int _GREEN_GOOD_COLOR_THRESHOLD_;
	int _MAX_NUMBER_GOOD_COLUMNS_;
	int _UP_EDGE_;
	int _DOWN_EDGE_;
	int _SLOW_MOTION_THRESHOLD_;
	int _FAST_MOTION_THRESHOLD_;
	bool _PRINT_DEBUG_;

private:
	int *m_pChangesCoord;
	int *m_pNumberWhite;
	int *m_pNumberBlack;
	bool m_isInit;
	bool m_isFullInfo;
	int m_activeChangesIndex;
	int m_j;
	int min[128];
	int indexMin[128];
	bool m_isOutput;
	bool m_isShortWord;
	int m_allnumberChanges;
	int m_begin_x;
	int m_end_x;

	void DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue, float maxValue);
	void AllocateMemory(bool);
	void ChangeFound();
	void CalculateColor();
	void SummaryByColumns(int index);
	void CalculateNumberGoodsBads(int i, int &numberGoods, int &numberBads);
	void CalculateColumnColors(int i, int &numberWhites, int &numberBlacks,  int &numberChanges);
	void FinishSummary();
public:
	int CalculateBodyHeight();
	int m_language;

};
