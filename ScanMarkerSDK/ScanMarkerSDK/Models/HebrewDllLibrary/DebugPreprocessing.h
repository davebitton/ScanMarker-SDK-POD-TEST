#pragma once
#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "Include/types_hebrewdll.h"
#endif


enum Preprocessing_Type
{
	NO_PREPROCESSING = 0,
	DOUBLE_THICK = 1
};


class CDebugPreprocessing
{
public:
	CDebugPreprocessing(void);
	~CDebugPreprocessing(void);


	Preprocessing_Type m_preprocessingType;
	byte* m_pDataIn;
	byte* m_pDataOut;
	int m_width;
	int m_height;
	int m_output_width;
	


	void Calculate();
	void DoubleThickPreprocessing();
	void NoPreprocessing();
	void Init(int width, int height, byte* pDataIn, int output_width, byte* pDataOut);



};
