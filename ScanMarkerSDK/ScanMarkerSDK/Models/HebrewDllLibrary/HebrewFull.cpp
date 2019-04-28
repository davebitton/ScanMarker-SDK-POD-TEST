#include "StdAfx.h"
#include "xv.h"
#include <stdio.h>
#include <string.h>
#include "xvbmp.h"
#include "HebrewCompressAsia_2.h"
#include "HebrewCompressChina_1.h"
#include "HebrewCompress_1.h"
#include "HebrewCompressHebrew_1.h"
///#include "HebrewCompressKorean_1.h"
//#include "Straight.h"
//#include "Straight_LH.h"
#include "StraightWithPolynom1.h"
#include "HebrewItalic.h"
#include "HebrewFull.h"
#include "CheckFonts.h"
#include "DebugPreprocessing.h"

#ifdef _SCANMARKER_WINDOWS_
#include <shlobj.h>
#endif
/*
#include "OneLine.h"
#include "OneLine_1.h"
#include "OneLine_2.h"
*/
#include "Table.h"

#define TextColor 255
#define BgColor 0
#define APP_ENGINE_LNG_HEBREW		230
#define APP_ENGINE_LNG_ARABIC 		351
#define APP_ENGINE_LNG_JAPANESE 	278
#define APP_ENGINE_LNG_KOREAN		232	
#define APP_ENGINE_LNG_CHINESE_TRADITIONAL		221
#define APP_ENGINE_LNG_CHINESE_SIMPLIFIED		222

void HebrewFull::PrintDebugStr(char* str, bool append)
{
	return;
	FILE * fff = NULL;
	if (append)
		fff = fopen("C://Temp//debug.txt", "at");
	else
		fff = fopen("C://Temp//debug.txt", "wt");

	if (!fff) return;
	fprintf(fff, "\n%s", str);
	fclose(fff);
}
void HebrewFull::PrintTimeStr(char* str, int ms, bool append)
{
	return;
	FILE * fff = NULL;
	//if (append)
		fff = fopen("C:\\Temp\\TextImageTime.txt", "at");
	//else
		//fff = fopen("C://Temp//debug5.txt", "wt");

	if (!fff) return;
	fprintf(fff, "\n%s %d", str, ms);
	fclose(fff);
}
HebrewFull::HebrewFull(void)
{
	m_width = 0;
	m_height = 0;
	m_widthRaw = 0;
	m_heightRaw = 0;
	m_dataRaw = NULL;
	m_dataIn = NULL;
	m_dataOut = NULL;
	m_dataInMode = 1;
	m_dataOutMode = 3;
	m_isDebugMode = true;
	sprintf(m_debugFileBasicName , "C:\\Temp\\UnknownHebrew_");
	m_directionFromLeftToRight = true;
	m_heightOfOutputBody = 44;
	m_language_In = 0;
	m_compressFactor_In = 1.0;
	m_isTableWithLines = false;
}
HebrewFull::~HebrewFull()
{
	m_width = 0;
	m_height = 0;
	m_dataIn = NULL;
	m_dataOut = NULL;
	m_dataInMode = 1;
	m_dataOutMode = 3;
	m_isDebugMode = true;
	sprintf(m_debugFileBasicName , "C:\\Temp\\UnknownChina_");
	m_dirName[0] = 0;

}
int HebrewFull::Init(bool corierActive, int width, int height, byte* pDataIn, byte* pDataOut,byte* pDataOut1, bool debugMode,
				char* dirName, char* debugFileBasicName, bool directionFromLeftToRight)
{
	m_corierActive = corierActive;
	m_directionFromLeftToRight = directionFromLeftToRight;
	m_width = width;
	m_height = height;
	m_dataIn = pDataIn;
	m_dataOut = pDataOut;
	m_dataOut_1 = pDataOut1;
	m_dataInMode = 1;
	m_dataOutMode = 3;
	m_isDebugMode = debugMode;
	sprintf(m_debugFileBasicName, "%s_hebrewFull_", debugFileBasicName);
	strncpy(m_dirName, dirName, 999);

	int numberWhite = 0;
	int numberBlack = 0;

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if ( m_dataIn[j*m_width+i] == 0)
			{
				numberBlack++;
			}
			else
			{
				numberWhite++;
			}
		}
	}

	if ( numberBlack == 0 || numberWhite == 0) return -1000;

    m_isWhiteBackground = false;
	if ( numberWhite > numberBlack)
	{
		m_isWhiteBackground = true;
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_dataIn[j*m_width+i] = 255 - m_dataIn[j*m_width+i];
			}
		}
	}

	return 0;

}
unsigned int getMillisecond() 
{
#ifdef _SCANMARKER_WINDOWS_
     __int64 freq, time, time_milli;
     unsigned int milliseconds;

     // The QueryPerformanceFrequency function retrieves the   frequency of the high-resolution performance counter, if one exists. The frequency cannot change while the system is running
     QueryPerformanceFrequency((LARGE_INTEGER*)&freq);  
 
       // The QueryPerformanceCounter function retrieves the current  value of the high-resolution performance counter
     QueryPerformanceCounter((LARGE_INTEGER*)&time);
    
     time_milli = ((time) * 1000) / freq;
     milliseconds = (unsigned int)(time_milli & 0xffffffff);
     return milliseconds;
#else
    return 0;
#endif
} 
void HebrewFull::UnSplit()
{
	int histogram[300];
	for ( int j = 0; j < m_height; j++)
	{
		histogram[j] = 0;
	}

	int jLine = 0;
	for ( int j = 5; j < m_height-5; j++)
	{
		histogram[j] = 0;
		for ( int i = 0; i < m_width; i++)
		{
			if ( m_dataIn[j*m_width+i] == 255)
			{
				histogram[j]++;
			}
		}
	}
	for ( int j = 5; j < m_height-5; j++)
	{
		if ((histogram[j] > m_width - 8) &&
			(histogram[j-1] < m_width - 4) &&
			(histogram[j+1] < m_width - 4) )
		{
			if ( jLine == 0)
			{
				jLine = j;
			}
			else
			{
				jLine = -1;
			}
		}
	}
	if ( jLine > 0)
	{
		int j1 = -1;
		for ( int j = jLine+1; j < m_height; j++)
		{
			j1++; 
			for ( int i = 0; i < m_width; i++)
			{
				m_dataOut[j1*m_width+i] = m_dataIn[j*m_width+i];
			}
		}
		for ( int j = 0; j <= jLine; j++)
		{

			j1++; 
			for ( int i = 0; i < m_width; i++)
			{
				m_dataOut[j1*m_width+i] = m_dataIn[j*m_width+i];
			}
		}
		/*for ( int j = m_height - 4; j < m_height; j++)
		{
			//j1++;
			for ( int i = 0; i < m_width; i++)
			{
					m_dataOut[j*m_width+i] = 255;
			}
		}
		/*int j = m_height - 4;
		for ( int i = 0; i < m_width; i++)
		{
			m_dataOut[j*m_width+i] = m_dataIn[j*m_width+i];
		}*/
/*		int j1 = -1;
		for ( int j = 0; j < 3; j++)
		{
			j1++;
			for ( int i = 0; i < m_width; i++)
			{
					m_dataOut[j1*m_width+i] = 255;
			}
		}
*/
		for ( int j = 0; j < m_height; j++)
		{
			for ( int i = 0; i < m_width; i++)
			{
				m_dataIn[j*m_width+i] = m_dataOut[j*m_width+i];
				m_dataOut[j*m_width+i] = 0;
			}
		}

	}



}
void HebrewFull::KoreanThick()
{
	int heights[128];
	for ( int j = 0; j < 128; j++)
		heights[j]++;
	for ( int i = 0; i < m_width; i++)
	{

		bool isText = false;
		int startText = -1;
		for ( int j = 5; j < 123; j++)
		{
			if ( !isText && m_dataIn[m_width*j+i] == TextColor)
			{
				isText = true;
				startText = j;
			}
			if ( isText && m_dataIn[m_width*j+i] != TextColor)
			{
				isText = false;
				int h = j - startText;
				startText = -1;
				if ( h <= 0 || h >= 125) continue;
				heights[h]++;
			}
		}
		
	}

	int max_h = 0;
	int max_h_count = 0;
	for ( int h = 2; h < 10; h++)
	{
		if ( /*heights[h-1] + */heights[h]/* + heights[h+1]*/ > max_h_count)
		{
			max_h_count = /*heights[h-1]*/ + heights[h] /*+ heights[h+1]*/;
			max_h = h;
		}
	}

	if ( max_h <= 3)
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if ( m_dataIn[m_width*j+i] == TextColor)
				{
					m_dataOut[j*m_width+i] = TextColor;
					m_dataOut[(j-1)*m_width+i] = TextColor;
					m_dataOut[(j)*m_width+i-1] = TextColor;
					m_dataOut[(j-1)*m_width+i-1] = TextColor;
				}
			}
		}
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_dataIn[m_width*j+i] = m_dataOut[m_width*j+i];
				m_dataOut[m_width*j+i] = 0;
			}
		}
	}

}
void HebrewFull::OneLineTry(int widthHist)
{
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_dataOut[3*m_width*j + i*3 + 0] = m_dataIn[m_width*j+i];
			m_dataOut[3*m_width*j + i*3 + 1] = m_dataIn[m_width*j+i];
			m_dataOut[3*m_width*j + i*3 + 2] = m_dataIn[m_width*j+i];

		}
	}
	
	for ( int i = 0; i < m_width - widthHist; i+=widthHist)
	{
		int histogram[128];
		for ( int j = 0; j < m_height; j++)
		{
			histogram[j] = 0;
		}
		for ( int j = 0; j < m_height; j++)
		{
			m_dataOut[3*m_width*j + i*3 + 0] = 127;
			for ( int ii = 0; ii < widthHist;ii++)
			{
				histogram[j] += (m_dataIn[m_width*j+i+ii] == TextColor) ? 1 : 0;
			}
			for ( int ii = 0; ii < histogram[j]; ii++)
			{
				m_dataOut[3*m_width*j + (i+ii)*3 + 1] = 127;
			}
		}
	}

}
int HebrewFull::Calculate( bool isOldStraight, int* param)
{
	//isOldStraight = false;
    /*--shimon
	HANDLE myself = GetCurrentProcess();
	int priority = GetPriorityClass(myself);
	int priority1 = HIGH_PRIORITY_CLASS;
	SetPriorityClass(myself, priority1);
	priority1 = GetPriorityClass(myself);
     shimon--*/
	int time_begin = getMillisecond();
	
	byte* output = m_dataOut ;
	byte* output2 = m_dataOut_1;
	int result = -2000;
try
{
	char fnOut[1000];

 /*  for (int i = 0; i < m_width; i++)
	   for ( int j = 0; j < m_height; j++)
			m_dataIn[m_width*j+i] = 255 - m_dataIn[m_width*j+i];
*/
	//m_isDebugMode = false;
	if (m_isDebugMode)
	{
		sprintf(fnOut, "%s_input.bmp", m_debugFileBasicName);
		for (int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_dataOut[m_width*j*3+i*3+0] = m_dataIn[m_width*j+i];
				m_dataOut[m_width*j*3+i*3+1] = m_dataIn[m_width*j+i];
				m_dataOut[m_width*j*3+i*3+2] = m_dataIn[m_width*j+i];
			}
		}
		/*FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}*/
		for (int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_dataOut[m_width*j*3+i*3+0] = 0;
				m_dataOut[m_width*j*3+i*3+1] = 0;
				m_dataOut[m_width*j*3+i*3+2] = 0;
			}
		}
	}


/*
	bool _KOREAN_THICK_ = false;
	int _KOREAN_THICK_HEIGHT_ = 44;
	bool _ONE_LINE_TRY_ = false;
	int _ONE_LINE_PARAMETER_ = 50;
	{
		FILE* fff = fopen("config/cfg.txt", "rt");
		{
			if ( fff)
			{
				char str[1000];
				while (fgets(str, 999, fff))
				{
					if (str[0] == '#') continue;
					if (strstr(str, "_KOREAN_THICK_"))
					{
						_KOREAN_THICK_ = true;
					}
					if (strstr(str, "_KOREAN_THICK_HEIGHT_"))
					{
						_KOREAN_THICK_ = true;
						sscanf(strstr(str, "_KOREAN_THICK_HEIGHT_") + strlen("_KOREAN_THICK_HEIGHT_"), "%ld", &_KOREAN_THICK_HEIGHT_);
					}
					if (strstr(str, "_ONE_LINE_TRY_"))
					{
						_ONE_LINE_TRY_ = true;
						sscanf(strstr(str, "_ONE_LINE_TRY_") + strlen("_ONE_LINE_TRY_"), "%ld", &_ONE_LINE_PARAMETER_);
					}
				}
				fclose(fff);
			}
		}
		if (_ONE_LINE_TRY_)
		{
			OneLineTry(_ONE_LINE_PARAMETER_);
		}
*/
	/*
		{
			sprintf(fnOut, "%s_oneLine_hist.bmp", m_debugFileBasicName);
			FILE* fp = fopen(fnOut, "wb");
			if (fp)
			{
				WriteBMP(fp,m_dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
				fclose(fp);
			}
			for (int i = 0; i < m_width; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					m_dataOut[m_width*j*3+i*3+0] = 0;
					m_dataOut[m_width*j*3+i*3+1] = 0;
					m_dataOut[m_width*j*3+i*3+2] = 0;
				}
			}
		}
		if (_KOREAN_THICK_)
		{
		//	KoreanThick();
		}
	}

*/


int oneLineExecTime = getMillisecond();
	
	if (!m_directionFromLeftToRight)
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_dataOut[(m_height -j)*m_width + (m_width - i)] = m_dataIn[j*m_width +i];
			}
		}
/*		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_dataIn[j*m_width +i] = m_dataOut[j*m_width +i];
			}
		}
*/
	} 
	else
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_dataOut[j*m_width + i] = m_dataIn[j*m_width +i];
			}
		}
	}

	int _DEBUG_COMPRESS_WIDTH_  = 50;
	bool _NEW_ONE_LINE_  = true;
	float _ONE_WORD_COMPRESS_FACTOR_  = 1.1;
/*
	if (_NEW_ONE_LINE_)
	{
		COneLine_1 hebrewOneLine;
		//fnOut = fnOutSrc +"_text.bmp";
		sprintf(fnOut, "%s_oneLine.bmp", m_debugFileBasicName);
		hebrewOneLine.Init(m_width, m_height, m_dataIn, m_dataOut, NULL);
		hebrewOneLine.SetParameters(1,25,20, 40, 1000);
		//hebrewOneLine.ReadCfgFile(m_dirName);
		hebrewOneLine.Calculate();
	}
	else
	{
		OneLine hebrewOneLine;
		//fnOut = fnOutSrc +"_text.bmp";
		sprintf(fnOut, "%s_oneLine.bmp", m_debugFileBasicName);
		hebrewOneLine.Init(m_width, m_height, m_dataIn, m_dataOut, NULL);
		hebrewOneLine.SetParameters(1,25,20, 40, 1000);
		//hebrewOneLine.ReadCfgFile(m_dirName);
		hebrewOneLine.Calculate();
	}





	oneLineExecTime = getMillisecond() - oneLineExecTime;


	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}

*/
	memset(m_dataIn, 0, (m_width*2 + 600)*m_height);
/*	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int value = m_dataOut[3*m_width*j + 3*i + 0];
			if (!m_directionFromLeftToRight)
			{
				value = m_dataOut[3*m_width*(m_height-j) + 3*(m_width-i) + 0];
			}
			m_dataIn[m_width*2*j + i] = value;
		}
	}
*/
	char filename[1000] = "";

#ifdef _DEBUG_PREPROCESSING_

	int _output_width = m_width*2;
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_dataIn[m_width*j + i] = m_dataOut[m_width*j + i];
		}
	}
	for ( int i = 0; i < _output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_dataOut[_output_width*3*j + i*3 + 0] = 0;
			m_dataOut[_output_width*3*j + i*3 + 1] = 0;
			m_dataOut[_output_width*3*j + i*3 + 2] = 0;
		}
	}
	CDebugPreprocessing debugPreprocessing;
	debugPreprocessing.Init(m_width, m_height, m_dataIn, _output_width, m_dataOut);
	debugPreprocessing.Calculate();

	sprintf(filename, "%s_preprocessing.bmp", m_debugFileBasicName);
	/*FILE* fp = fopen(filename, "wb");
	if (fp)
	{
		WriteBMP(fp,m_dataOut,PIC24,_output_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
		fclose(fp);
	}*/
    for ( int i = 0; i < _output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_dataIn[_output_width*j + i] = m_dataOut[_output_width*3*j + i*3+0];
		}
	}
	m_width*=2;

    for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_dataOut[m_width*j + i] = m_dataIn[m_width*j + i];
			m_dataIn[m_width*j + i] = 0;
		}
	}
 
#endif
    int newWidth1 = m_width*2;
    if (m_width < 1600)
        newWidth1 = m_width*3;
    if (m_width < 1000)
        newWidth1 = m_width*4;
    for ( int i = 0; i < newWidth1; i++)
    {
        for ( int j = 0; j < m_height; j++)
        {
            m_dataIn[newWidth1*j + i] = 0;
        }
    }
	if (!m_isTableWithLines)
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				int value = m_dataOut[m_width*j + i ];
				if (!m_directionFromLeftToRight)
				{
					value = m_dataOut[m_width*(m_height-j) + (m_width-i) ];
				}
                m_dataIn[newWidth1*j + i] = value;
			}
		}
		//m_width *=2;
        m_width = newWidth1;
	}
	else
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				int value = m_dataOut[m_width*j + i ];
				if (!m_directionFromLeftToRight)
				{
					value = m_dataOut[m_width*(m_height-j) + (m_width-i) ];
				}
				m_dataIn[m_width*j + i] = value;
			}
		}
	}

	int numberWhite = 0;
	int numberBlack = 0;

	int numberChanges = 0;
	int numberWhiteColumna = 0;
/*	for ( int i = 0; i < m_width; i++)
	{
		bool isWhiteColumn = false;
		for ( int j = 0; j < m_height; j++)
		{
			if ( m_dataIn[j*m_width+i] == 0)
			{
				numberBlack++;
			}
			else
			{
				numberWhite++;
				isWhiteColumn = true;
			}
			if ( m_dataIn[j*m_width+i] != m_dataIn[j*m_width+i-1])
			{
				numberChanges++;
			}
		}
		if (isWhiteColumn)
			numberWhiteColumna++;
	}
if ( numberWhiteColumna < 30 || (numberChanges < m_height)) 
		return -1000;
*/  

	int isClick = 0;
	for ( int j = 0; j < m_height; j++)
	{
		int numberChanges = 0;
		for ( int i = 1; i < m_width; i++)
		{
			if ( m_dataIn[j*m_width+i] != m_dataIn[j*m_width+i-1])
			{
				numberChanges++;
			}
		}
		if ( numberChanges > 2) isClick++;
	}
	if ( isClick < 20 && m_width < 400) return -1000;
	if ( m_width < 200) return -1000;
	


//return -1000;
	memset(m_dataOut, 0, m_width*m_height*3);

	
	

int straightExecTime= getMillisecond();

	sprintf(fnOut, "%s_straight.bmp", m_debugFileBasicName);
	
	FILE * fff = NULL;//fopen(fileNameShift, "wt");
	bool isEnlarged = false;
	
	byte * dataInBeforeStraight = new byte[ m_width*3*2*m_height];
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int value = m_dataIn[m_width*j + i];
			dataInBeforeStraight[m_width*3*2*j + i*3 + 0] = value;
			dataInBeforeStraight[m_width*3*2*j + i*3 + 1] = value;
			dataInBeforeStraight[m_width*3*2*j + i*3 + 2] = value;
		}
	}

	/*if (m_isDebugMode)
	{
		char fnOut1[1000];
		sprintf(fnOut1, "%sdataInBeforeStraight.bmp", m_debugFileBasicName);
		FILE* fp = fopen(fnOut1, "wb");
		if (fp)
		{
			WriteBMP(fp,dataInBeforeStraight,PIC24,m_width * 2,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}*/
	bool doNothing = false;
	if (m_language_In == 351) //not Arabic
	{
		doNothing = true;
	}
	bool isShortScan = false;
	if (CheckIfShortScan())
	{
		isShortScan = true;
		doNothing = true;
	}
	isShortScan = false;
	doNothing = false;
	
	Table m_tableProcessing;
	bool doItalic = true;
	if (!m_isTableWithLines)
	{
	//CStraightWithForie hebrewStraight;
		CStraightWithPolynom1 *hebrewStraight = new CStraightWithPolynom1;
		try
		{
			hebrewStraight->Init(m_width, m_height, m_width*2, m_dataIn, m_dataOut);
			hebrewStraight->m_pDataRaw = m_dataRaw;
			hebrewStraight->m_widthRaw = m_widthRaw;
			hebrewStraight->m_heightRaw = m_heightRaw;
			hebrewStraight->m_language = m_language_In;
			hebrewStraight->m_isVertical = m_isVertical;
			hebrewStraight->m_doNothing = doNothing;
			hebrewStraight->ReadCfgFile(m_dirName);
		//hebrewStraight._KOREAN_THICK_ = _KOREAN_THICK_;
			//hebrewStraight->m_heightOfOutputBody = m_heightOfOutputBody;
			hebrewStraight->m_aveHeightDefault = m_aveHeightDefault;
			if (m_isDebugMode)
			{
				sprintf(filename, "%s_straight_polynom.bmp", m_debugFileBasicName);
			}

		//isEnlarged = hebrewStraight.Calculate(false, filename);
			isEnlarged = hebrewStraight->Calculate( filename);
		}
		catch(...)
		{
			delete hebrewStraight;
			return 8;
		}

		delete hebrewStraight;
		if (fff)
		{
			fclose(fff);
		}
		memset(m_dataIn, 0, m_width*m_height);
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
			int value = dataInBeforeStraight[m_width*2*3*j + i*3 ];
			m_dataIn[m_width*j + i] = value;
			}
		}
	}
	else
	{
		strncpy(m_tableProcessing.m_debugFileBasicName, m_debugFileBasicName, 999);
		m_tableProcessing.m_isDebugMode = m_isDebugMode;
		m_tableProcessing.Init(m_width, m_height, m_width*2, m_dataIn, m_dataRaw, m_dataOut);
		m_tableProcessing.Calculate();
		doItalic = false;//!m_tableProcessing.m_italicSuccess;
	}
	

	if (m_language_In != 351) //not Arabic
	{
		m_width*= 2;

		straightExecTime= getMillisecond()  - straightExecTime;
	memset(m_dataIn, 0, m_width*m_height);
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				int value = m_dataOut[3*m_width*j + 3*i + 0];
				m_dataIn[m_width*j + i] = value;
			}
		}

		if (dataInBeforeStraight)
		{
			delete [] dataInBeforeStraight;
			dataInBeforeStraight = NULL;
		}


		if (m_isDebugMode)
		{
			FILE* fp = fopen(fnOut, "wb");
			if (fp)
			{
				WriteBMP(fp,m_dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
				fclose(fp);
			}
		}
	}
	
	int compressExecTime = getMillisecond();


memset(m_dataOut, 0, m_width*m_height*3);
	WriteDebugStr("aaaaaa", false);
	CCheckFonts checkFonts;
	checkFonts.m_fontsToCheck = m_fonts;
	
	WriteDebugStr("bbbbbb");
	
	//byte* dataAfterStraight = NULL;
	//int widthAfterStraight = m_width;
	//int heightAfterStraight = m_height;
	void *pHebrewCompress = NULL;
	if ( m_language_In == APP_ENGINE_LNG_HEBREW) // Hebrew
	{
		/*
		 pHebrewCompress = (void*) new CHebrewCompressHebrew_1 ;
		CHebrewCompressHebrew_1 hebrewCompress = *(CHebrewCompressHebrew_1 *)pHebrewCompress;
		*/
		sprintf(fnOut, "%s_compress.bmp", m_debugFileBasicName);
		CHebrewCompressHebrew_1 *hebrewCompress = new CHebrewCompressHebrew_1;
		try
		{
			hebrewCompress->Init(m_corierActive, m_width, m_height, m_width, m_dataIn, m_dataOut);
			hebrewCompress->m_checkFonts = NULL;// &checkFonts;
			hebrewCompress->_DEBUG_COMPRESS_WIDTH_ = _DEBUG_COMPRESS_WIDTH_;
			hebrewCompress->_ONE_WORD_COMPRESS_FACTOR_ = _ONE_WORD_COMPRESS_FACTOR_;
			hebrewCompress->m_compressFactor_In = 0.0;//m_compressFactor_In;
			hebrewCompress->Calculate();
		}
		catch(...)
		{
			delete hebrewCompress;
			return 8;
		}
		delete hebrewCompress;

	} 
	else if (m_language_In == APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
			  m_language_In == APP_ENGINE_LNG_CHINESE_SIMPLIFIED	) // Hebrew
	{
	
		sprintf(fnOut, "%s_compress.bmp", m_debugFileBasicName);
		CHebrewCompressChina_1 *hebrewCompress = new CHebrewCompressChina_1;
		int res = 0;
		try
		{
			hebrewCompress->Init(m_corierActive, m_width, m_height, m_width, m_dataIn, m_dataOut);
			//hebrewCompress.m_compressFactor_In = m_compressFactor_In;
			strncpy(hebrewCompress->m_debugFileBasicName, m_debugFileBasicName,999);
			hebrewCompress->m_isVertical = m_isVertical;
			hebrewCompress->m_language = m_language_In;
			res = hebrewCompress->Calculate();
		}
		catch(...)
		{
			delete hebrewCompress;
			return 8;
		}
		delete hebrewCompress;
		if ( res == -1000)
			return -1000;
	}
	else if ( m_language_In == APP_ENGINE_LNG_JAPANESE ||
		      m_language_In == APP_ENGINE_LNG_KOREAN ) // Hebrew
	{
		/*
		 pHebrewCompress = (void*) new CHebrewCompressHebrew_1 ;
		CHebrewCompressHebrew_1 hebrewCompress = *(CHebrewCompressHebrew_1 *)pHebrewCompress;
		*/
/*		if (m_language_In == APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
			m_language_In == APP_ENGINE_LNG_CHINESE_SIMPLIFIED	) // Hebrew
		{
			dataAfterStraight = new byte[m_width*m_height];
			for ( int i = 0; i < m_width; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					dataAfterStraight[j*m_width+i] = m_dataIn[j*m_width+i];
				}
			}
			widthAfterStraight = m_width;
			heightAfterStraight = m_height;
		}
*/
		sprintf(fnOut, "%s_compress.bmp", m_debugFileBasicName);
		CHebrewCompressAsia_2 *hebrewCompress = new CHebrewCompressAsia_2;
		int res = 0;
		try
		{
			hebrewCompress->Init(m_corierActive, m_width, m_height, m_width, m_dataIn, m_dataOut);
		//hebrewCompress.m_compressFactor_In = m_compressFactor_In;
			strncpy(hebrewCompress->m_debugFileBasicName, m_debugFileBasicName,999);
			hebrewCompress->m_isVertical = m_isVertical;
			hebrewCompress->m_language = m_language_In;
			res = hebrewCompress->Calculate();
		}
		catch(...)
		{
			delete hebrewCompress;
			return 8;
		}
		delete hebrewCompress;
		if ( res == -1000)
			return -1000;
	}
	else
	{
		/* 
		pHebrewCompress = (void*) new CHebrewCompress_1;
		CHebrewCompress_1 hebrewCompress = *(CHebrewCompress_1 *)pHebrewCompress;
		*/
		sprintf(fnOut, "%s_compress.bmp", m_debugFileBasicName);
		WriteDebugStr("Before CHebrewCompress_1::Init");
		CHebrewCompress_1 *hebrewCompress = new CHebrewCompress_1;
		try
		{
			hebrewCompress->Init(m_corierActive, m_width, m_height, m_width, m_dataIn, m_dataOut);
		//hebrewCompress.m_checkFonts = &checkFonts; 
			WriteDebugStr("After CHebrewCompress_1::Init");
			hebrewCompress->_DEBUG_COMPRESS_WIDTH_ = _DEBUG_COMPRESS_WIDTH_;
			hebrewCompress->_ONE_WORD_COMPRESS_FACTOR_ = _ONE_WORD_COMPRESS_FACTOR_;
			hebrewCompress->m_compressFactor_In = m_compressFactor_In;
			hebrewCompress->m_language = m_language_In;
			WriteDebugStr("Before CHebrewCompress_1::Calculate");
			hebrewCompress->Calculate();
			WriteDebugStr("After CHebrewCompress_1::Calculate");
		}
		catch(...)
		{
			delete hebrewCompress;
			return 8;
		}
		delete hebrewCompress;
	}	
	compressExecTime= getMillisecond() - compressExecTime;

	
			
	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}


    int begin_x = 0;
	int end_x = m_width-1;
    int begin_y = 0;
	int end_y = m_height-1;
	for ( int j = 0; j < m_height; j++)
	{
		bool found = false;
		for ( int i = 52; i < m_width-50; i++)
		{
			if (m_dataOut[3*m_width*j + 3*i + 0] == TextColor)
			{
				if (!found)
				{
					begin_y = j;
					found = true;
				}
				break;
			}
		}
		
		if (found) break;
	}
	for ( int j = m_height-1; j >= 0; j--)
	{
		bool found = false;
		for ( int i = 52; i < m_width-50; i++)
		{
			if (m_dataOut[3*m_width*j + 3*i + 0] == TextColor)
			{
				if (!found)
				{
					end_y = j;
					found = true;
				}
				break;
			}
		}
		
		if (found) break;
	}




	for ( int i = 52; i < m_width-50; i++)
	{
		bool found = false;
		for ( int j = 8; j < m_height-8; j++)
		{
			if (m_dataOut[3*m_width*j + 3*i + 0] == TextColor)
			{
				if (!found)
				{
					begin_x = i;
					found = true;
				}
				break;
			}
		}
		
		if (found) break;
	}
	for ( int i = m_width-20; i >= 51; i--)
	{
		bool found = false;
		for ( int j = 8; j < m_height-8; j++)
		{
			if (m_dataOut[3*m_width*j + 3*i + 0] == TextColor)
			{
				end_x = i;
				found = true;
				break;
			}
		}
		if (found) break;
	}

	int begin = begin_x - 50;
	if (begin < 0 ) begin = 0;

	int newWidth = (end_x-begin) + 50 + 70;
	if (newWidth > m_width) newWidth = m_width;


	if ((end_y - begin_y < 10) || (end_x - begin_x < 30) )
	{
		int w = m_width;
		m_width = m_width_2 = end_x - begin_x + 200;
		m_height = m_height_2 = end_y - begin_y + 40;
		int indOutput_i = 100;
		for ( int i = begin_x; i <=  end_x; i++)
		{
			int indOutput_j = 15;
			for ( int j =  begin_y; j <= end_y; j++)
			{
				m_dataOut[indOutput_j*m_width*3 + indOutput_i*3 + 0] = m_dataOut[3*w*j + 3*i + 0];
				m_dataOut[indOutput_j*m_width*3 + indOutput_i*3 + 1] = m_dataOut[3*w*j + 3*i + 1];
				m_dataOut[indOutput_j*m_width*3 + indOutput_i*3 + 2] = m_dataOut[3*w*j + 3*i + 2];
				
				m_dataOut_1[indOutput_j*m_width*3 + indOutput_i*3 + 0] = m_dataOut[3*w*j + 3*i + 0];
				m_dataOut_1[indOutput_j*m_width*3 + indOutput_i*3 + 1] = m_dataOut[3*w*j + 3*i + 1];
				m_dataOut_1[indOutput_j*m_width*3 + indOutput_i*3 + 2] = m_dataOut[3*w*j + 3*i + 2];
				
				
				indOutput_j++;
			}
			indOutput_i++;
		}
		return 0;
	}

	begin_y-=15;
	end_y+=15;

	if (begin_y < 0) begin_y = 0;
	if (end_y >= m_height) end_y = m_height-1;
	int newHeight = end_y-begin_y;

	memset(m_dataIn, 0, m_width*m_height*3);
	for ( int i = begin; i < m_width; i++)
	{
		if (i >= newWidth+begin) break;
		for ( int j = begin_y; j < end_y; j++)
		{
			int indexDataOut = 3*m_width*j + 3*i;
			int indexDataIn = newWidth*(j - begin_y)*3 + (i-begin+70)*3;
			m_dataIn[indexDataIn+0] = m_dataOut[indexDataOut + 0];
			m_dataIn[indexDataIn+1] = m_dataOut[indexDataOut + 1];
			m_dataIn[indexDataIn+2] = m_dataOut[indexDataOut + 2];
		}
	}
	m_width = newWidth;
	m_height = newHeight;
	
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_dataOut[3*m_width*j + 3*i + 0] = m_dataIn[m_width*j*3 + i*3+0];
			m_dataOut[3*m_width*j + 3*i + 1] = m_dataIn[m_width*j*3 + i*3+1];
			m_dataOut[3*m_width*j + 3*i + 2] = m_dataIn[m_width*j*3 + i*3+2];
		}
	}
	
	
	
	
	
	sprintf(fnOut, "%s_italic_input.bmp", m_debugFileBasicName);
	/*{
	{
	
#ifdef _SCANMARKER_WINDOWS_
//		CCheckFonts checkFonts;
//		checkFonts.m_fontsToCheck = m_fonts;

		if (m_isDebugMode)
		{
			char fn[1000];
			//if (m_fonts)
			//	sprintf(fn, "%s//%s_debug_output.txt", m_fonts->m_fontsDirname, m_fonts->m_fontsFilename);
			//else
			char f[1024];
			SHGetSpecialFolderPath(0, f, CSIDL_COMMON_APPDATA, false);
			sprintf(fn, "%s//Scanmarker//Fonts/Fonts_Output_debug.txt", f);
			SYSTEMTIME t;
			GetSystemTime(&t);


			FILE * fff = fopen(fn, "at");
			if (fff)
			{
				char timeStr[1000];
				
				sprintf(timeStr, "%02d/%02d/%04d %02d:%02d:%02d:%03d  ", t.wDay , t.wMonth , t.wYear , t.wHour,t.wMinute , t.wSecond , t.wMilliseconds);
	     
				fprintf(fff, "%s %s   FontApplied %s factorApplied=%f widthVertical=%.2f   widthHorizontalLow = %.2f  widthHorizontalHigh = %.2f  factor = %.2f BgHoriz = %.2f\n", 
					timeStr, m_debugFileBasicName, 
					checkFonts.m_fontApplied, checkFonts.m_factorToApply, 
					checkFonts.m_widthVertical, checkFonts.m_widthHorizontalLow, checkFonts.m_widthHorizontalHigh, 
					(checkFonts.m_widthVertical / checkFonts.m_widthHorizontalLow), checkFonts.m_widthBgHorizontal);
				fclose(fff);
			}

			sprintf(fn, "%s//Scanmarker//Fonts//Tables/%s%02d_%02d_%04d_%02d_%02d_%02d_%03d", f,m_debugFileBasicName, t.wDay , t.wMonth , t.wYear , t.wHour,t.wMinute , t.wSecond , t.wMilliseconds);
			checkFonts.WriteTablesDebug(fn);


		}
#endif
	
	checkFonts.Release();
	}
*/			
	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}

#ifdef _RESEARCH_
	char fnConfig[1000];
	char debugFilePathName[1000];
	strncpy(debugFilePathName, m_debugFileBasicName, 999);
	int l = strlen(debugFilePathName);
	while ( l > 0 && debugFilePathName[l] != '\\' && debugFilePathName[l] != '\/') l --;
	debugFilePathName[l] = 0;

	sprintf(fnConfig, "%s//config/hebrewdll_cfg.txt",debugFilePathName);
	FILE* fCfg = fopen(fnConfig, "rt");
	if (fCfg)
	{
		char str[1000];
		while (fgets(str,999,fCfg))
		{
			if (strstr(str, "_IGNORE_ITALIC_"))
				doItalic = false;
		}
		fclose(fCfg);
	}



#endif
	
	
	

	int italicExecTime= getMillisecond();
//	if (doItalic)
	{
		memset(m_dataOut, 0, m_width*m_height*3);
		HebrewItalic *hebrewItalic = new HebrewItalic;
		sprintf(fnOut, "%s_italic.bmp", m_debugFileBasicName);
		try
		{
			hebrewItalic->m_output_width = m_width;
			WriteDebugStr("Before hebrewItalic::Init");
			hebrewItalic->Init(m_width, m_height, m_width, m_height, m_dataIn, m_dataOut);
			WriteDebugStr("After hebrewItalic::Init");
			hebrewItalic->ReadCfgFile(m_dirName);
			if (m_italicStep == 0)
				hebrewItalic->m_notToDoItalic= true;
			hebrewItalic->m_language = m_language_In;
			WriteDebugStr("Before hebrewItalic::Calculate");
			result = hebrewItalic->Calculate();
			WriteDebugStr("After hebrewItalic::Calculate");
			m_italicStep = hebrewItalic->m_italicStep;
		}
		catch(...)
		{
			delete hebrewItalic;
			WriteDebugStr("123123123");
			return 8;
		}
		WriteDebugStr("234234234");
		delete hebrewItalic;
		WriteDebugStr("345345345");
	}
italicExecTime= getMillisecond() - italicExecTime;
if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}	
	/*
	if ((!m_isVertical) && (m_language_In == APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
			m_language_In == APP_ENGINE_LNG_CHINESE_SIMPLIFIED	)) // Hebrew
	{
		FixWidthChina(dataAfterStraight, widthAfterStraight, heightAfterStraight);
		delete [] dataAfterStraight;
		dataAfterStraight = NULL;
	}
	*/
	byte * temp_output = new byte[m_width*m_height*4*4*3];

/*
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int value = m_dataIn[m_width*j + i];
			m_dataOut[j*m_width*3+i*3+0] = value;
			m_dataOut[j*m_width*3+i*3+1] = value;
			m_dataOut[j*m_width*3+i*3+2] = value;
		}
	}
*/	



	memcpy(temp_output, output, m_width*m_height*3*sizeof(byte));

	
	//memset(output, 0, output_width*output_height*3*sizeof(byte));

		for ( int i = 1; i < m_width-1; i++)
		{
			for ( int j = 1; j < m_height-1; j++)
			{
				int value = temp_output[j*m_width*3+i*3];
				value += temp_output[(j-1)*3*m_width+i*3-1*3];
				value += temp_output[(j-1)*3*m_width+i*3];
				value += temp_output[(j-1)*3*m_width+i*3+1*3];
				value += temp_output[j*3*m_width+i*3-1*3];
				value += temp_output[j*3*m_width+i*3+1*3];
				value += temp_output[(j+1)*3*m_width+i*3-1*3];
				value += temp_output[(j+1)*3*m_width+i*3];
				value += temp_output[(j+1)*3*m_width+i*3+1*3];
				bool correct = false;
				if ( value <= TextColor)// || value == 2*TextColor)
				{
					value = 255 - TextColor;
					correct = true;
				}
				else if ( value >= TextColor*8)// || value == 2*TextColor)
				{
					value =  TextColor;
					correct = true;
				}
				else
					value = temp_output[j*m_width*3+i*3];
				if (m_language_In == APP_ENGINE_LNG_ARABIC) // Arabic
					correct = false;
/*					for ( int ii = -1; ii <= 1; ii++)
					{
						for ( int jj = -1; jj <=1; jj++)
						{
								m_pDataOut[(j+jj)*m_output_width*3+(i+ii)*3+0] = value;
								m_pDataOut[(j+jj)*m_output_width*3+(i+ii)*3+1] = value;
								m_pDataOut[(j+jj)*m_output_width*3+(i+ii)*3+2] = value;
						}
					}
*/
				if ( m_language_In == APP_ENGINE_LNG_KOREAN ||
					m_language_In == APP_ENGINE_LNG_HEBREW ||
					m_language_In ==APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
					m_language_In == APP_ENGINE_LNG_CHINESE_SIMPLIFIED ||
					m_language_In == APP_ENGINE_LNG_JAPANESE )
					correct = false;
				if (correct)
				{
					for ( int k = 0; k < 3; k++)
					{
						output[(j)*m_width*3+(i)*3+k] = value;
						output[(j)*m_width*3+(i-1)*3+k] = value;
						output[(j)*m_width*3+(i+1)*3+k] = value;
						output[(j-1)*m_width*3+(i)*3+k] = value;
						output[(j+1)*m_width*3+(i)*3+k] = value;
						
					}
				}
			}
		}
		if (m_isDebugMode)
		{
			FILE* fp = fopen(fnOut, "wb");
			if (fp)
			{
				WriteBMP(fp,output,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
				fclose(fp);
			}
		}	

		/*FILE *fp3 = fopen(fnOut, "wb");
		WriteBMP(fp3,output,PIC24,pinfo.w,pinfo.h,NULL,NULL, NULL,8,F_FULLCOLOR);
		fclose(fp3);
		*/
		int output_width = m_width;
		int output_height = m_height;





		memcpy(temp_output, output, m_width*m_height*3*sizeof(byte));
		memset(output2, 0, m_width*m_height*3*sizeof(byte));

		int new_begin_y = -1;
		for ( int j = 0; j < m_height; j++)
		{
			for ( int i = 0; i < m_width; i++)
			{
				if (temp_output[m_width*j*3+i*3+0]== TextColor)
				{
					new_begin_y = j;
					break;
				}
			}
			if (new_begin_y > -1) break;
		}

		int new_end_y = -1;
		for ( int j = m_height-1; j >= 0; j--)
		{
			for ( int i = 0; i < m_width; i++)
			{
				if (temp_output[m_width*j*3+i*3+0] == TextColor)
				{
					new_end_y = j;
					break;
				}
			}
			if (new_end_y > -1) break;
		}
		int output_width2= 0;
		int output_height2= 0;
	   if (isEnlarged)
		{
			output_height2= output_height;
			output_width2= output_width;;
			memcpy(output2, output, output_height*output_width*3*sizeof(byte));
            m_width_2 = output_width2;
            m_height_2 = output_height2;


		}
		else
		{
			
			
		
			
			float factor = 2.0;//6.0/4.0;
			output_height2 = (new_end_y - new_begin_y)*factor + 20;
			output_width2 = m_width*2;
            m_width_2 = output_width2;
            m_height_2 = output_height2;
			int _LOW_Y_1 = 10;//(2*m_height - (new_end_y - new_begin_y)*7/4)/2;
			if (_LOW_Y_1 < 0) _LOW_Y_1 = 0;
			if (new_end_y <= new_begin_y)
				return 0;
			int ii = 0;
			float f_jj, f_ii;
			int int_ii, int_jj;
			f_ii = factor;
			for ( int i = 0; i < m_width; i++)
			{
				if ( i < 0) continue;
				if ( (float)i*factor > output_width2) continue;
				int jj = 0;
				int_ii = (int)(f_ii+0.5);
				f_ii -= (float)int_ii;

				f_jj =factor;
				for ( int j1 = new_begin_y; j1 <= new_end_y; j1++)
				{	
					int_jj = (int)(f_jj+0.5);
					f_jj -= (float) int_jj;
					if ((jj + _LOW_Y_1) >= output_height2) continue;
					for ( int ki = 0; ki < int_ii; ki++)
					{
						for ( int kj = 0; kj < int_jj; kj++)
						{

							output2[(jj + kj + _LOW_Y_1)*output_width2*3 + (ii+ki)*3+0]  = temp_output[j1*m_width*3+ i*3 + 0];
							output2[(jj + kj + _LOW_Y_1)*output_width2*3 + (ii+ki)*3+1]  = temp_output[j1*m_width*3+ i*3 + 0];
							output2[(jj + kj + _LOW_Y_1)*output_width2*3 + (ii+ki)*3+2]  = temp_output[j1*m_width*3+ i*3 + 0];
							
						}
						
					}
					jj+=int_jj;
					f_jj += factor;
				}
				ii+=int_ii;
				f_ii += factor;
			}
		}
		//output_width = newWidth1;
		//output_height = newHeight1;

		//output_height = newHeight1;
		/*FILE *fp2 = fopen(fnOut, "wb");
		WriteBMP(fp2,output,PIC24,output_width,output_height,NULL,NULL, NULL,8,F_FULLCOLOR);
		fclose(fp2);
		{
		FILE *fp2 = fopen(fnOut, "wb");
		WriteBMP(fp2,output2,PIC24,output_width2,output_height2,NULL,NULL, NULL,8,F_FULLCOLOR);
		fclose(fp2);
		}*/
		int output_width1 = output_width;
		int output_height1 = output_height;
		for ( int k = 0; k < 1; k++)
		{
			if ( k == 0) 
			{
				output = m_dataOut;
				output_width = output_width1;
				output_height = output_height1;
			}
			if ( k == 1)
			{
				output = m_dataOut_1;
				output_width = output_width2;
				output_height = output_height2;
			}
			if (m_language_In != APP_ENGINE_LNG_KOREAN &&
				m_language_In !=APP_ENGINE_LNG_CHINESE_TRADITIONAL	&&
				m_language_In != APP_ENGINE_LNG_CHINESE_SIMPLIFIED &&
				m_language_In != APP_ENGINE_LNG_JAPANESE )// &&  (m_language_In != APP_ENGINE_LNG_HEBREW))
			{
				float threshold = 0.0;
				if (m_language_In == APP_ENGINE_LNG_HEBREW)
					 threshold = ThresholdByLinesThickness();
			memcpy(temp_output, output, output_width*output_height*3*sizeof(byte));
			memset(output, 0, output_width*output_height*3*sizeof(byte));
			for ( int i = 1; i < output_width-2; i++)
			{
				for ( int j = 1; j < output_height-2; j++)
				{
					byte value1 = temp_output[j*3*output_width+i*3];
					int value = temp_output[j*3*output_width+i*3];
					value += temp_output[(j-1)*3*output_width+i*3-1*3];
					value += temp_output[(j-1)*3*output_width+i*3];
					value += temp_output[(j-1)*3*output_width+i*3+1*3];
					value += temp_output[j*3*output_width+i*3-1*3];
					value += temp_output[j*3*output_width+i*3+1*3];
					value += temp_output[(j+1)*3*output_width+i*3-1*3];
					value += temp_output[(j+1)*3*output_width+i*3];
					value += temp_output[(j+1)*3*output_width+i*3+1*3];
					//if ( value < 256) value = 0;
					//value /= 7;
						if (true) //(m_language_In != APP_ENGINE_LNG_ARABIC &&
					{
							//value /= 9;
							if (m_language_In ==APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
								m_language_In == APP_ENGINE_LNG_CHINESE_SIMPLIFIED ||
								m_language_In == APP_ENGINE_LNG_JAPANESE 
								)
							{
								value /= 9;
								//if (value1 == 255) value = 255;
								value = value1;
							}
							else
							{
								value /= 9;
							if (value < 83) value = 0;
							if (value > 255) value = 255;
								value = value1;
						}
					}
					else
					{
						///if (value >= 255*8) value = 255;
						//else
						{
							value = value1;
							//if (value < 120) value = 0;
							if (value > 255) value = 255;
						}
						
					}
					/*if (k == 0)
					{
						output[j*output_width*3+i*3+0] = value;
						output[j*output_width*3+i*3+1] = value;
						output[j*output_width*3+i*3+2] = value;
					}
					else*/
					{
						output[j*output_width*3+i*3+0] = value;
						output[j*output_width*3+i*3+1] = value;
						output[j*output_width*3+i*3+2] = value;
					}

				}
			}
			}
		}
		{
			output = m_dataOut_1;
			int width = output_width2;
			int height = output_height2;
			memcpy(temp_output, output, width*height*3*sizeof(byte));
			memset(output, 0, width*height*3*sizeof(byte));
			int new_begin_y = -1;
			for ( int j = 0; j < height; j++)
			{
				for ( int i = 0; i < width; i++)
				{
					if (temp_output[width*j*3+i*3+0]== TextColor)
					{
						new_begin_y = j;
						break;
					}
				}
				if (new_begin_y > -1) break;
			}

			int new_end_y = -1;
			for ( int j = height-1; j >= 0; j--)
			{
				for ( int i = 0; i < width; i++)
				{
					if (temp_output[width*j*3+i*3+0] == TextColor)
					{
						new_end_y = j;
						break;
					}
				}
				if (new_end_y > -1) break;
			}

			int new_begin_x = -1;
			for ( int i = 0; i < width; i++)
			{
				for ( int j = 0; j < height; j++)
				{
					if (temp_output[width*j*3+i*3+0]== TextColor)
					{
						new_begin_x = i;
						break;
					}
				}
				if (new_begin_x > -1) break;
			}


			int new_end_x = -1;
			for ( int i = width-1; i >=0; i--)
			{
				for ( int j = 0; j < height; j++)
				{
					if (temp_output[width*j*3+i*3+0]== TextColor)
					{
						new_end_x= i;
						break;
					}
				}
				if (new_end_x > -1) break;
			}

			int newWidth = new_end_x - new_begin_x + 200;
			int newHeight = new_end_y - new_begin_y + 40;

			for ( int i = new_begin_x; i <= new_end_x; i++)
			{
				for ( int j = new_begin_y; j < new_end_y; j++)
				{
					int j1 = j + 15 - new_begin_y;
					int i1 = i + 170 - new_begin_x;
					
					byte value = temp_output[width*j*3+i*3+0];
					output[newWidth*3*j1+i1*3 + 0] = value;
					output[newWidth*3*j1+i1*3 + 1] = value;
					output[newWidth*3*j1+i1*3 + 2] = value;
				}
			}

			m_width_2 = newWidth;
			m_height_2 = newHeight;


		}



		delete [] temp_output;
	














#ifdef _SCANMARKER_WINDOWS_

	if (m_isDebugMode)
	{
		int allExecTime = getMillisecond() - time_begin;
		FILE *fff1 = fopen("C:\\Temp\\TextImageTime.txt", "at");
		if ( fff1)
		{
			SYSTEMTIME currentTime;
			GetSystemTime(&currentTime);
			fprintf(fff1, "\n\n\n China Full New Run At %ld//%ld//%ld %ld:%ld:%ld", 
				currentTime.wDay, currentTime.wMonth, currentTime.wYear, 
				currentTime.wHour, currentTime.wMinute, currentTime.wSecond );

/*		char priorityStr[1000],priorityStr1[1000];
		switch (priority)
		{
			case REALTIME_PRIORITY_CLASS:
			{
				strcpy(priorityStr, "REALTIME_PRIORITY_CLASS");
				break;
			}
			case HIGH_PRIORITY_CLASS:
			{
				strcpy(priorityStr, "HIGH_PRIORITY_CLASS");
				break;
			}
			case ABOVE_NORMAL_PRIORITY_CLASS:
			{
				strcpy(priorityStr, "ABOVE_NORMAL_PRIORITY_CLASS");
				break;
			}
			case NORMAL_PRIORITY_CLASS:
			{
				strcpy(priorityStr, "NORMAL_PRIORITY_CLASS");
				break;
			}
			case BELOW_NORMAL_PRIORITY_CLASS:
			{
				strcpy(priorityStr, "BELOW_NORMAL_PRIORITY_CLASS");
				break;
			}
			case IDLE_PRIORITY_CLASS:
			{
				strcpy(priorityStr, "IDLE_PRIORITY_CLASS");
				break;
			}
			default :
			{
				strcpy(priorityStr, "UNKNOWN");
				break;
			}



		}
		switch (priority1)
		{
			case REALTIME_PRIORITY_CLASS:
			{
				strcpy(priorityStr1, "REALTIME_PRIORITY_CLASS");
				break;
			}
			case HIGH_PRIORITY_CLASS:
			{
				strcpy(priorityStr1, "HIGH_PRIORITY_CLASS");
				break;
			}
			case ABOVE_NORMAL_PRIORITY_CLASS:
			{
				strcpy(priorityStr1, "ABOVE_NORMAL_PRIORITY_CLASS");
				break;
			}
			case NORMAL_PRIORITY_CLASS:
			{
				strcpy(priorityStr1, "NORMAL_PRIORITY_CLASS");
				break;
			}
			case BELOW_NORMAL_PRIORITY_CLASS:
			{
				strcpy(priorityStr1, "BELOW_NORMAL_PRIORITY_CLASS");
				break;
			}
			case IDLE_PRIORITY_CLASS:
			{
				strcpy(priorityStr1, "IDLE_PRIORITY_CLASS");
				break;
			}
			default :
			{
				strcpy(priorityStr1, "UNKNOWN");
				break;
			}



		}
*/

			if (allExecTime == 0)
				allExecTime = 1;
//			fprintf(fff1, "\n priority = %s(%ld) new %s(%ld)", priorityStr,priority, priorityStr1,priority1);

			fprintf(fff1, "\n allExecTime = %ld %ldsec", 
				allExecTime, allExecTime/1000);
				
			fprintf(fff1, "\n	oneLine Time= %ld %ldsec (%ld\%ld)", 
				oneLineExecTime, oneLineExecTime/1000,  oneLineExecTime*100/allExecTime);

			fprintf(fff1, "\n	straight Time= %ld %ldsec (%ld\%ld)", 
				straightExecTime, straightExecTime/1000,  straightExecTime*100/allExecTime);

			fprintf(fff1, "\n	compress Time= %ld %ldsec (%ld\%ld)", 
				compressExecTime, compressExecTime/1000,  compressExecTime*100/allExecTime);

			fprintf(fff1, "\n	italic Time= %ld %ldsec (%ld\%ld)", 
				italicExecTime, italicExecTime/1000,  italicExecTime*100/allExecTime);

			fclose(fff1);
		}
		
	}
#endif
}
catch(...)
{
	return -2000;
}
    return 1;
}
float  HebrewFull::ThresholdByLinesThickness()
{
	int begin_x = m_width-1;
	int end_x = 0;
	int begin_y = m_height-1;
	int end_y = 0;
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height;j++)
		{
			if (m_dataOut[j*m_width*3+i*3] != TextColor)
				continue;
			if (begin_x > i) begin_x = i;
			if (end_x < i) end_x = i;
			if (begin_y > j) begin_y = j;
			if (end_y < j) end_y = j;
		}
	}
	int widths[100];
	for ( int w = 0; w < 100; w++)
	{
		widths[w] = 0;
	}
	for ( int j = begin_y; j <= end_y; j++)
	{
		int i1 = 0; 
		int i2 = 0;
		bool isText = false;
		for ( int i = begin_x-1; i < end_x+1; i++)
		{
			if ( i >= m_width) continue;
			if (i < 0) continue;
			if (m_dataOut[j*m_width*3+i*3] == TextColor && !isText)
			{
				i1 = i;
				isText = true;
			}
			if (m_dataOut[j*m_width*3+i*3] != TextColor && isText)
			{
				i2 = i;
				isText = false;
				int w = i2 -i1;
				if ( w > 0 && w < 100)
				{
					widths[w]++;
				}
			}
		}
	}
	int w_maxCount = 0;
	float maxCount = 0;
	for ( int w = 0; w < 15; w++)
	{
		if (widths[w]+widths[w+1] > maxCount)
		{
			maxCount = widths[w]+widths[w+1];
			w_maxCount = w;
		}
	}
	float w_result = (float)(widths[w_maxCount]*w_maxCount+widths[w_maxCount+1]*(w_maxCount+1)) / (float)(widths[w_maxCount]+widths[w_maxCount+1]);
	return  w_result;
}
void HebrewFull::FixWidthChina(byte *dataAfterStraight, int widthAfterStraight, int heightAfterStraight  )
{
	/*for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_dataIn[j*m_width+i] = m_dataOut[j*m_width*3+i*3+0];
		}
	}
	memset(m_dataOut, 0, m_width*m_height*3);
		
	float ratio = CalculateLettersRatio();
	
	float factor = 1.0;
	/*if ((ratio < 0.97 || ratio > 1.15) && m_isVertical)
	{
		factor = 1.06/ratio;
	}* /
	if ((ratio < 0.96 || ratio > 1.06) && m_isVertical)
	{
		factor = 1.02/ratio;
	}
	else if ((ratio < 0.92 || ratio > 1.08) && (!m_isVertical))
	{
		factor = 1.0/ratio;
	}
	else
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_dataOut[j*m_width*3+i*3+0] = m_dataIn[j*m_width+i];
				m_dataOut[j*m_width*3+i*3+1] = m_dataIn[j*m_width+i];
				m_dataOut[j*m_width*3+i*3+2] = m_dataIn[j*m_width+i];
			}
		}
		
		return;
	}
	char fnOut[1000];
	sprintf(fnOut, "%s_compressFix.bmp", m_debugFileBasicName);
	m_width = widthAfterStraight;
	m_height = heightAfterStraight;
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_dataIn[j*m_width+i] = dataAfterStraight[j*m_width+i];
		}
	}
		
	CHebrewCompressAsia_1 hebrewCompress;
	hebrewCompress.Init(m_corierActive, m_width, m_height, m_width, m_dataIn, m_dataOut);
	//hebrewCompress.m_compressFactor_In = m_compressFactor_In;
	strncpy(hebrewCompress.m_debugFileBasicName, m_debugFileBasicName,999);
	hebrewCompress.m_isVertical = m_isVertical;
	hebrewCompress.m_language = m_language_In;
	hebrewCompress.m_additionalFactor = factor;
	hebrewCompress.Calculate();
 
		//checkFonts.Release();
	
	
			
	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}





    int begin_x = 0;
	int end_x = m_width-1;
    int begin_y = 0;
	int end_y = m_height-1;
	for ( int j = 0; j < m_height; j++)
	{
		bool found = false;
		for ( int i = 52; i < m_width-50; i++)
		{
			if (m_dataOut[3*m_width*j + 3*i + 0] == TextColor)
			{
				if (!found)
				{
					begin_y = j;
					found = true;
				}
				break;
			}
		}
		
		if (found) break;
	}
	for ( int j = m_height-1; j >= 0; j--)
	{
		bool found = false;
		for ( int i = 52; i < m_width-50; i++)
		{
			if (m_dataOut[3*m_width*j + 3*i + 0] == TextColor)
			{
				if (!found)
				{
					end_y = j;
					found = true;
				}
				break;
			}
		}
		
		if (found) break;
	}




	for ( int i = 52; i < m_width-50; i++)
	{
		bool found = false;
		for ( int j = 8; j < m_height-8; j++)
		{
			if (m_dataOut[3*m_width*j + 3*i + 0] == TextColor)
			{
				if (!found)
				{
					begin_x = i;
					found = true;
				}
				break;
			}
		}
		
		if (found) break;
	}
	for ( int i = m_width-20; i >= 51; i--)
	{
		bool found = false;
		for ( int j = 8; j < m_height-8; j++)
		{
			if (m_dataOut[3*m_width*j + 3*i + 0] == TextColor)
			{
				end_x = i;
				found = true;
				break;
			}
		}
		if (found) break;
	}

	int begin = begin_x - 50;
	if (begin < 0 ) begin = 0;

	int newWidth = (end_x-begin) + 50 + 70;
	if (newWidth > m_width) newWidth = m_width;


	begin_y-=5;
	end_y+=5;

	if (begin_y < 0) begin_y = 0;
	if (end_y >= m_height) end_y = m_height-1;
	int newHeight = end_y-begin_y;

	memset(m_dataIn, 0, m_width*m_height);
	for ( int i = begin; i < m_width; i++)
	{
		if (i >= newWidth+begin) break;
		for ( int j = begin_y; j < end_y; j++)
		{
			int value = m_dataOut[3*m_width*j + 3*i + 0];
			m_dataIn[newWidth*(j - begin_y) + i-begin+70] = value;
		}
	}
	m_width = newWidth;
	m_height = newHeight;
	
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_dataOut[3*m_width*j + 3*i + 0] = m_dataIn[m_width*j + i];
			m_dataOut[3*m_width*j + 3*i + 1] = m_dataIn[m_width*j + i];
			m_dataOut[3*m_width*j + 3*i + 2] = m_dataIn[m_width*j + i];
		}
	}
	
	

	
	
	sprintf(fnOut, "%s_italic_fix_input.bmp", m_debugFileBasicName);

	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}

	
	
	
	
	
	int italicExecTime= getMillisecond1();
	memset(m_dataOut, 0, m_width*m_height*3);
	HebrewItalic hebrewItalic;
	sprintf(fnOut, "%s_italic_fix.bmp", m_debugFileBasicName);

	hebrewItalic.m_output_width = m_width;
	hebrewItalic.Init(m_width, m_height, m_width, m_height, m_dataIn, m_dataOut);
	hebrewItalic.ReadCfgFile(m_dirName);
	if (m_italicStep == 0)
		hebrewItalic.m_notToDoItalic= true;
	hebrewItalic.Calculate();
	m_italicStep = hebrewItalic.m_italicStep;
	italicExecTime= getMillisecond1() - italicExecTime;
	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}

/*	int indOutput = 0;
	float nc;
	int ncInt;
	int prevNc = 0;
	for ( int i = 0; i <= m_width; i++)
	{
		if ( i >= m_width) break;
		if (indOutput >= m_width) 
			break;
		int k = i;
		
		float value;// = (value2+value2) /  2;
		value = factor;
		float value2 = value;
		nc+= value2;
		ncInt = (int) nc;
		int add = ncInt - prevNc;
		if (add > 3)
		{
			prevNc = ncInt;
			add = 0;
		}
		for ( int k = 0; k < add; k++)
		{
			for ( int j = 0; j < m_height; j++)
			{
					m_dataOut [j*m_width*3+ indOutput*3 +0] = m_dataIn[j*m_width + i];
					m_dataOut [j*m_width*3+ indOutput*3 +1] = m_dataIn[j*m_width + i];
					m_dataOut [j*m_width*3+ indOutput*3 +2] = m_dataIn[j*m_width + i];
			}
			indOutput++;
			if (indOutput >= m_width) break;
		}
		if (add)
			prevNc = ncInt;
	
	}
	*/
}
float HebrewFull:: CalculateLettersRatio()
{
	int begin_x = m_width-1;
	int begin_y = m_height - 1;
	int end_x = 0;
	int end_y = 0;
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if ( m_dataIn[j*m_width+i] == TextColor)
			{
				if (begin_x > i) begin_x = i;
				if (end_x < i) end_x = i;
				if (begin_y > j) begin_y = j;
				if (end_y < j) end_y = j;
			}
		}
	}


	int letters[200][4];

	int l = 0;
	bool searchForEndLetter = false;
	bool searchForBeginLetter = true;
	for ( int i = begin_x; i < end_x; i++)
	{
		bool isColumnEmpty = true;
		for ( int j = begin_y; j < end_y; j++)
		{
			if ( m_dataIn[j*m_width+i] == TextColor)
			{
				isColumnEmpty = false;
				break;
			}
		}
		if (!isColumnEmpty && searchForBeginLetter)
		{
			letters[l][0] = i;
			searchForBeginLetter = false;
			searchForEndLetter = true;
			continue;
		}
		if (isColumnEmpty && searchForEndLetter)
		{
			letters[l][2] = i;
			searchForBeginLetter = true;
			searchForEndLetter = false;
			if (l >= 199) break;
			l++;
			continue;

		}
	}
	int numberLetters = l;
	for ( int l = 0; l < numberLetters; l++)
	{
		int i1 = letters[l][0];
		int i2 = letters[l][2];
		int j1 = end_y;
		int j2 = begin_y;
		for ( int i = i1; i < i2; i++)
		{
			for ( int j = begin_y; j < end_y; j++)
			{
				if ( m_dataIn[j*m_width+i] == TextColor)
				{
					if ( j1 > j ) j1 = j;
					if ( j2 < j) j2 = j;
				}
			}
		}
		letters[l][1] = j1;
		letters[l][3] = j2;
	}

	float ratio[200];
	for ( int l = 0; l < numberLetters; l++)
	{
		ratio[l] = (float) (letters[l][2] - letters[l][0]) / (float) (letters[l][3] - letters[l][1]);
	}

	for ( int k = 0; k < numberLetters-1; k++)
	{
		for ( int kk = 1; kk < numberLetters; kk++)
		{
			if (ratio[k] > ratio[kk])
			{
				float temp = ratio[k] ;
				ratio[k] = ratio[kk];
				ratio[kk] = temp;
			}
		}
	}


	return ratio[numberLetters/2];


}
void HebrewFull::WriteDebugStr(char * str, bool isAppend)
{
return;
	FILE * fff = NULL;
	if (isAppend)
		fff = fopen("C://Temp//HebrewFullDebug.txt", "at");
	else
		fff = fopen("C://Temp//HebrewFullDebug.txt", "wt");

	if (fff)
	{
		fprintf(fff, "\n%s", str);
		fclose(fff);
	}
}
void HebrewFull::CropImage(byte* dataIn, byte* dataOut, int width, int height, int &newWidth, int &newHeight)
{
   int begin_x = 0;
	int end_x = width-1;
    int begin_y = 0;
	int end_y = height-1;
	for ( int j = 0; j < height; j++)
	{
		bool found = false;
		for ( int i = 0; i < width; i++)
		{
			if (dataIn[m_width*j + i + 0] == TextColor)
			{
				if (!found)
				{
					begin_y = j;
					found = true;
				}
				break;
			}
		}
		
		if (found) break;
	}
	for ( int j = height-1; j >= 0; j--)
	{
		bool found = false;
		for ( int i = 0; i < width; i++)
		{
			if (dataIn[width*j + 3*i + 0] == TextColor)
			{
				if (!found)
				{
					end_y = j;
					found = true;
				}
				break;
			}
		}
		
		if (found) break;
	}




	for ( int i = 0; i < width; i++)
	{
		bool found = false;
		for ( int j = 0; j < height; j++)
		{
			if (dataIn[width*j + i + 0] == TextColor)
			{
				if (!found)
				{
					begin_x = i;
					found = true;
				}
				break;
			}
		}
		
		if (found) break;
	}
	for ( int i = width; i >= 0; i--)
	{
		bool found = false;
		for ( int j = 0; j < height; j++)
		{
			if (dataIn[width*j + i + 0] == TextColor)
			{
				end_x = i;
				found = true;
				break;
			}
		}
		if (found) break;
	}

	int begin = begin_x - 30;
	if (begin < 0 ) begin = 0;

	newWidth = (end_x-begin) + 30;
	if (newWidth > width) newWidth = width;


	if (end_y - begin_y < 10) return;
	if (end_x - begin_x < 30) return;

	begin_y-=10;
	end_y+=10;

	if (begin_y < 0) begin_y = 0;
	if (end_y >= height) end_y = height-1;
	newHeight = end_y-begin_y;

	memset(dataOut, 0, width*height);
	for ( int i = begin; i < end_x+50; i++)
	{
		if (i >= newWidth+begin) break;
		for ( int j = begin_y; j < end_y; j++)
		{
			int value = dataIn[width*j + i + 0];
			dataOut[newWidth*(j - begin_y) + i-begin] = value;
		}
	}
			
}
 
void HebrewFull::EnlargeImage(byte* dataIn, byte* dataOut, int width, int height,int &newWidth, int &newHeight)
{

		int output_height = height*2;
		int output_width = width*2;
        
		memset(dataOut, 0, output_width*output_height);
		//int _LOW_Y_1 = 10;//(2*m_height - (new_end_y - new_begin_y)*7/4)/2;
		//if (_LOW_Y_1 < 0) _LOW_Y_1 = 0;
		
		int ii = 0;
		for ( int i = 0; i < width; i++)
		{
			int ni = 2;
			if ( i % 8 == 0) ni = 1;
			if ( i <= 0 ) continue;
			if ( ii > output_width) continue;
			int jj = 0;
			for ( int j1 = 0; j1 <= height; j1++)
			{
				int nj = 2;
				if ( j1 % 8 == 0) nj = 1;
				
				//if ( j*2 > m_height + _LOW_Y_1) continue;
				//int j = j1 - new_begin_y;
				if ((jj) >= output_height) continue;
				for ( int ki = 0; ki < ni; ki++)
				{
					for ( int kj = 0; kj < nj; kj++)
					{

						dataOut[(jj + kj)*output_width + (ii+ki)+0]  = dataIn[j1*width+ i ];
						//output2[(jj + kj + _LOW_Y_1)*output_width*3 + (ii+ki)*3+1]  = temp_output[j1*m_width*3+ i*3 + 0];
						//output2[(jj + kj + _LOW_Y_1)*output_width*3 + (ii+ki)*3+2]  = temp_output[j1*m_width*3+ i*3 + 0];
						
					}
					
				}
				jj+=nj;
			}
			ii+=ni;
		}
		newHeight = output_height;
        newWidth = output_width;

}

bool HebrewFull::CheckIfShortScan()
{
	int numberChangesInRaw[128];
	for ( int j = 4; j < m_height; j++)
	{
		numberChangesInRaw[j] = 0;
		for ( int i = 0; i < m_width-2; i++)
		{
			if ( m_dataIn[j*m_width+i] != m_dataIn[j*m_width+i+1])
			{
				numberChangesInRaw[j]++;
			}
		}
	}
	for ( int k = 0; k < m_height-1; k++)
	{
		for ( int k1 = k+1; k1 < m_height;k1++)
		{
			if (numberChangesInRaw[k1] > numberChangesInRaw[k])
			{
				int temp = numberChangesInRaw[k1];
				numberChangesInRaw[k1] = numberChangesInRaw[k];
				numberChangesInRaw[k] = temp;
			}
		}

	}
 
	if ( numberChangesInRaw[5] > 40/*20*/) return false;
	else return true;
}

