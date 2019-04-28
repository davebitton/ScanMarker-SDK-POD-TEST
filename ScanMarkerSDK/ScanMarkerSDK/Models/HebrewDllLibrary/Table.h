#pragma once
#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#endif
class Table
{
public:
	Table(void);
	~Table(void);

	byte* m_pDataIn;
	byte* m_pDataRaw;
	byte* m_pDataOut;
	
	int m_width;
	int m_height;
	int m_outputWidth;
	int m_outputHeight;

	int m_begin_x;
	int m_end_x;
	int m_begin_y;
	int m_end_y;

	byte* m_pWorkData1;
	byte* m_pWorkData2;
	int* m_StraightLineByX;
	int* m_StraightLineByY;
	bool m_italicSuccess;

	bool m_isDebugMode;
	char m_debugFileBasicName[1000];
	char m_dirName[1000];
	void Init(int width, int height, int outputWidth,byte* pDataIn, byte* pDAtaRaw, byte* pDataOut);
	void AllocateMemory();
	void Calculate();
	void CalculateImageBoundaries();
	void FindNearest(int begin_x, int numberChanges, int & end_x);
	void FillSnake();
	void FixDatainFromRaw(int addHeight, int addWidth);
	void Straight();
	void Italic();
	int CheckForClick();

};

