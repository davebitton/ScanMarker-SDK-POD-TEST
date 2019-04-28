//#include "StdAfx.h"
#include <stdio.h>
#include "OneLine_4.h"
#include "xvbmp.h"
//#include "..\SplineInterpolation\SplineInterpolation.h"
#define APP_ENGINE_LNG_KOREAN					232
#define APP_ENGINE_LNG_JAPANESE 		278
#define APP_ENGINE_LNG_CHINESE_TRADITIONAL		221
#define APP_ENGINE_LNG_CHINESE_SIMPLIFIED		222
#define APP_ENGINE_LNG_VERTICAL_CHINESE_TRADITIONAL		364
#define APP_ENGINE_LNG_VERTICAL_CHINESE_SIMPLIFIED		365
#define APP_ENGINE_LNG_VERTICAL_JAPANESE 				366
//#define APP_ENGINE_LNG_CMC7					371
#define APP_ENGINE_LNG_HEBREW					230
#define NUMBER_WORDS_IN_STEP 1
#define NUMBER_WORDS_IN_CALC 0
#define NUMBER_WORDS_IN_ITALIC 3
#define TextColor 255
#define APP_ENGINE_LNG_CMC7  		332
#define White 255
#define Black 0
#define _MIDDLE_ROW_ 64
#define _BEGIN_X_ 5
#define _FIRST_WORD_SCANNED_BAD_SPACE_ 200
bool isAsialanguage(int l)
{
	bool result = false;

	switch(l)
	{
		case APP_ENGINE_LNG_KOREAN:
		case APP_ENGINE_LNG_JAPANESE:
		//case APP_ENGINE_LNG_CHINESE_TRADITIONAL:
		//case APP_ENGINE_LNG_CHINESE_SIMPLIFIED:
		case APP_ENGINE_LNG_VERTICAL_CHINESE_TRADITIONAL:
		case APP_ENGINE_LNG_VERTICAL_CHINESE_SIMPLIFIED:
		{
			result = true;
			break;

		}

		default :
		{
			result = false;
		}
	}
	return result;
}
void COneLine_4::PrintDebugStr(char* str, bool append)
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
COneLine_4::COneLine_4(void)
{
	_UP_EDGE_  = 3;
	_DOWN_EDGE_  = 126;
	m_width = 0;
	m_height = 0;
	m_pDataIn = NULL;
	m_pDataOut = NULL;
	m_pWorkData = NULL;
	m_pWorkData1 = NULL;
	m_pWorkData2 = NULL;
	m_pWorkData3 = NULL;
	m_pWorkData4 = NULL;
	m_pWorkData5 = NULL;
	m_pWorkData6 = NULL;
	m_pWorkData7 = NULL;
	m_pDataIn3 = NULL;
	m_pDataOut1 = NULL;

	m_isChineseVertical = false;
	m_firstWordBad = false;
	
	
}
 COneLine_4::~COneLine_4()
{
	if (m_pWorkData)
	{
		delete[] m_pWorkData;
		m_pWorkData = NULL;
	}
	if (m_pWorkData1)
	{
		delete[] m_pWorkData1;
		m_pWorkData1= NULL;
	}
	if (m_pWorkData2)
	{
		delete[] m_pWorkData2;
		m_pWorkData2= NULL;
	}
	if (m_pWorkData3)
	{
		delete[] m_pWorkData3;
		m_pWorkData3= NULL;
	}
	if (m_pWorkData4)
	{
		delete[] m_pWorkData4;
		m_pWorkData4= NULL;
	}
	if (m_pWorkData5)
	{
		delete[] m_pWorkData5;
		m_pWorkData5= NULL;
	}
	if (m_pWorkData6)
	{
		delete[] m_pWorkData6;
		m_pWorkData6 = NULL;
	}
	if (m_pWorkData7 != NULL)
	{
		delete[] m_pWorkData7;
		m_pWorkData7= NULL;
	}
	
	if (m_pDataIn3)
	{
		delete[] m_pDataIn3;
		m_pDataIn3= NULL;
	}

	if (m_pDataOut1 != NULL)
	{
		delete[] m_pDataOut1;
		m_pDataOut1= NULL;
	}
	
}
void COneLine_4::AllocateMemory()
{
	if (m_pDataOut1 != NULL)
	{
		delete[] m_pDataOut1;
		m_pDataOut1= NULL;
	}
	m_pDataOut1 = new byte[(m_width+600)*m_height*3];
	memset(m_pDataOut1, 0, (m_width+600)*m_height*3*sizeof(byte));
	

	if (m_pWorkData!= NULL)
	{
		delete[] m_pWorkData;
		m_pWorkData = NULL;
	}
	m_pWorkData = new int[(m_width+600)*m_height*3];
	memset(m_pWorkData, 0, (m_width+600)*m_height*3*sizeof(int));

	
	if (m_pWorkData1)
	{
		delete[] m_pWorkData1;
		m_pWorkData1= NULL;
	}
	m_pWorkData1 = new int[(m_width+600)*m_height*3*2];
	memset(m_pWorkData1, 0, (m_width+600)*m_height*2*3*sizeof(int));
	if (m_pWorkData2)
	{
		delete[] m_pWorkData2;
		m_pWorkData2= NULL;
	}
	m_pWorkData2 = new int[(m_width+600)*m_height*3];
	memset(m_pWorkData2, 0, (m_width+600)*m_height*3*sizeof(int));
	if (m_pWorkData3)
	{
		delete[] m_pWorkData3;
		m_pWorkData3= NULL;
	}
	m_pWorkData3 = new int[(m_width+600)*m_height*3];
	memset(m_pWorkData3, 0, (m_width+600)*m_height*3*sizeof(int));
	if (m_pWorkData4)
	{
		delete[] m_pWorkData4;
		m_pWorkData4= NULL;
	}
	m_pWorkData4= new int[(m_width+600)*m_height*3];
	memset(m_pWorkData4, 0, (m_width+600)*m_height*3*sizeof(int));

	if (m_pWorkData5)
	{
		delete[] m_pWorkData5;
		m_pWorkData5= NULL;
	}
	m_pWorkData5= new int[(m_width+600)*m_height*3];
	memset(m_pWorkData5, 0, (m_width+600)*m_height*3*sizeof(int));

	if (m_pWorkData6)
	{
		delete[] m_pWorkData6;
		m_pWorkData6 = NULL;
	}
	m_pWorkData6 = new int[(m_width+600)*m_height*3];
	memset(m_pWorkData6, 0, (m_width+600)*m_height*3*sizeof(int));


	if (m_pWorkData7 != NULL)
	{
		delete[] m_pWorkData7;
		m_pWorkData7= NULL;
	}
	m_pWorkData7 = new int [m_width];
	memset(m_pWorkData7, 0, m_width*sizeof(int));
	


	if (m_pDataIn3)
	{
		delete[] m_pDataIn3;
		m_pDataIn3= NULL;
	}
	m_pDataIn3= new byte[(m_width+600)*m_height*3];
	memset(m_pDataIn3, 0, (m_width+600)*m_height*3*sizeof(byte));


	

	m_outputSnakes = NULL;
	m_maxNumberOutputSnakes = 0;;
	



	
	
}

void COneLine_4::Init(int width, int height, byte* pDataIn, byte* pDataOut, byte* pDataOut1)
{
	m_width = width;
	m_height = height;
	m_pDataIn = pDataIn;
	m_pDataOut = pDataOut;
//	m_pDataOut1 = pDataOut1;
	memset(m_pDataOut, Black, m_width*m_height*3*sizeof(byte));
	//AllocateMemory();
	m_isInit = true;
	AllocateMemory();

}
void COneLine_4::ReadCfgFile(char* dir)
{
	return;
	char fileName[255];
	//sprintf(fileName, "%s//%s", dir, "text_is_out.cfg");
	sprintf(fileName, "china_compress_big.cfg");
	FILE * fff = fopen(fileName, "rt");
	if (!fff) return;

	char str[255];
	while (fgets(str, 254, fff))
	{
		if (str[0] == '#') continue;

		
/*		if ( strstr(str, "_HALF_NUMBER_CHANGES_TO_ANALIZE_"))
		{
			sscanf(strstr(str, "_HALF_NUMBER_CHANGES_TO_ANALIZE_") + strlen("_HALF_NUMBER_CHANGES_TO_ANALIZE_"), "%ld", &_HALF_NUMBER_CHANGES_TO_ANALIZE_);
		}
		if ( strstr(str, "_MAX_NUMBER_BAD_COLUMNS_ "))
		{
			sscanf(strstr(str, "_MAX_NUMBER_BAD_COLUMNS_ ") + strlen("_MAX_NUMBER_BAD_COLUMNS_ "), "%ld", &_MAX_NUMBER_BAD_COLUMNS_ );
		}
		if ( strstr(str, "_MIN_RATIO_BLACK_TO_WHITE_"))
		{
			sscanf(strstr(str, "_MIN_RATIO_BLACK_TO_WHITE_") + strlen("_MIN_RATIO_BLACK_TO_WHITE_"), "%ld", &_MIN_RATIO_BLACK_TO_WHITE_);
		}
		if ( strstr(str, "_MIN_LENGTH_OF_BLACK_"))
		{
			sscanf(strstr(str, "_MIN_LENGTH_OF_BLACK_") + strlen("_MIN_LENGTH_OF_BLACK_"), "%ld", &_MIN_LENGTH_OF_BLACK_);
		}
*/
	}
	

	fclose(fff);
}
void COneLine_4::CalculateBodyHeight()
{
	int heights[128];
	for ( int h = 0; h < 128; h++)
	{
		heights[h] = 0;
	}
	for ( int l = 0; l < m_numberLetters; l++)
	{
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if ( h > 0 && h < 127)
		{
			heights[h] ++;
		}
	}
	m_bodyHeight = 0;
	int count = 0;
	for ( int h = 20; h < 80; h++)
		count += heights[h];
	int half = count *4/ 5;
	int hBody = 0;
	count = 0;
	for ( int h = 20; h < 80; h++)
	{
		count += heights[h];
		if (count > half)
		{
			m_bodyHeight = h;
			break;
		}
	}
}
void COneLine_4::SetParameters(int numberChanges, int goodNumberWhitesPerChanges, int goodPerBadColumn, int badsPerColumnPercent, int numberBadsForFail)
{
	/*m_numberChanges = numberChanges;
	m_goodNumberWhitesPerChanges = goodNumberWhitesPerChanges;
	m_goodPerBadColumn = goodPerBadColumn;
	m_badsPerColumnPercent= badsPerColumnPercent;
	m_numberBadsForFail = numberBadsForFail;
	*/
	_MAX_NUMBER_BAD_COLUMNS_ = 1000;
	_MIN_RATIO_BLACK_TO_WHITE_ = 3;
	_MIN_LENGTH_OF_BLACK_ = 200;
	//AllocateMemory();

}
void COneLine_4::RemoveColumnsInBeginAndEnd()
{
	//return;
	int maxHeightOfColumn = 0;
	for (int i = 200; i < m_width-200; i++)
	{
		int j1 = m_height-1;
		int j2 = 0;
		for (int j = 10; j < m_height-10; j++)
		{
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				if (j < j1) j1 = j;
				if (j > j2) j2 = j;
			}
		}

		int h = j2 - j1;
		if (h > maxHeightOfColumn)
			maxHeightOfColumn = h;
	}

	if (maxHeightOfColumn < 85) return;
	int widthCheck = 1200;
	if (widthCheck > m_width - 2)
		widthCheck = m_width - 2;
	{
		int firstWIdeSpaceColumn = 0;
		int widthWideColumn = 0;
		for (int i = 0; i < widthCheck; i++)
		{
			bool isWhiteColumn = true;
			for (int j = 0; j < m_height; j++)
			{
				if (m_pDataIn[j*m_width+i] == TextColor)
					isWhiteColumn = false;
			}

			if (isWhiteColumn)
			{
				widthWideColumn++;
				if (widthWideColumn > 70)
				{
					firstWIdeSpaceColumn = i - widthWideColumn;
					break;
				}
			}
			else
			{
				widthWideColumn = 0;
			}

		}
		if (firstWIdeSpaceColumn < 500)
		{
			for (int i = 0; i < firstWIdeSpaceColumn+1; i++)
			{
				m_pWorkData7[i] = 1;
			}
		}
	}
	{
		int lastWIdeSpaceColumn =  m_width-1;
		int widthWideColumn = 0;
		for (int i = 0; i < widthCheck; i++)
		{
			bool isWhiteColumn = true;
			for (int j = 0; j < m_height; j++)
			{
				if (m_pDataIn[j*m_width+m_width-i-1] == TextColor)
					isWhiteColumn = false;
			}

			if (isWhiteColumn)
			{
				widthWideColumn++;
				if (widthWideColumn > 70)
				{
					lastWIdeSpaceColumn = m_width - i - 1 + widthWideColumn;
					break;
				}
			}
			else
			{
				widthWideColumn = 0;
			}
		}
		if (m_width - lastWIdeSpaceColumn < 500)
		{
			for (int i = lastWIdeSpaceColumn; i < m_width-1; i++)
			{
				m_pWorkData7[i] = 1;
			}
		}
		
	}
}
void COneLine_4::Calculate( bool isTable, int language)
{ 
	/*RemoveHorizontalLines();
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pDataIn[j*m_width+i] == 127)
			{
				m_pWorkData[j*m_width + i +0] = TextColor;
				m_pDataIn[j*m_width + i] = 255 - TextColor;
			}
		}
	}
	*/


	RemoveColumnsInBeginAndEnd();
	for ( int i = m_width - 5; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataIn[j*m_width+i] = 255  -TextColor;
		}
		
	}

		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pDataOut1[j*3*m_width+i*3]  = m_pDataIn[j*m_width+i] ;
			}
		}
	m_language = language;
	if ( m_language == 371)
	{
		FillChars(m_pDataIn, m_pDataIn3);
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pDataIn[j*m_width+i] = m_pDataIn3[j*m_width*3+i*3+0];
			}
		}
#ifdef _PRINT_DEBUG_BMP_

		FILE * fp = fopen("C://Temp//oneLineFill.bmp", "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataIn3,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
#endif			
	}
	else
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pDataIn3[j*m_width*3+i*3+0] = m_pDataIn[j*m_width+i];
				m_pDataIn3[j*m_width*3+i*3+1] = m_pDataIn[j*m_width+i];
				m_pDataIn3[j*m_width*3+i*3+2] = m_pDataIn[j*m_width+i];
			}
		}
	}

	if (!m_outputSnakes || m_maxNumberOutputSnakes == 0)
		isTable = false;
	for ( int j = 0; j < m_height; j++)
	{
		int numberColorColumns = 0;
		for ( int i = _BEGIN_X_; i < m_width; i++)
		{
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				numberColorColumns++;
			}
		}
		if (numberColorColumns > m_width - 10 - _BEGIN_X_)
		{
			for ( int i = _BEGIN_X_; i < m_width; i++)
			{	
				m_pDataIn[j*m_width+i] = 255 - TextColor;
			}
		}
	}

    for ( int i = 0; i <_BEGIN_X_ ; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataIn[j*m_width+i] = 255 - TextColor;
		}
	}
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pDataOut1[j*3*m_width+i*3]  = m_pDataIn[j*m_width+i] ;
			}
		}
	}
	if (isTable)
	{
		//Remove TAble lines
		RemoveTableHorizontalLines();
		RemoveTableVerticalLines();
		
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (m_pDataIn[j*m_width+i] == 127)
					m_pDataIn[j*m_width+i] = 255 - TextColor;
			}
		}
	}
	else
	{
		RemoveTableVerticalLines();
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (m_pDataIn[j*m_width+i] == 127)
				{
					m_pDataIn[j*m_width+i] = 255 - TextColor;
				}
			}
		}
	}
	bool found = false;
	for ( int i = _BEGIN_X_; i < m_width; i++)
	{
		for ( int j = _UP_EDGE_; j < _DOWN_EDGE_; j++)
		{
			if ( m_pDataIn[j*m_width + i] == TextColor)
			{
				m_begin_x = i;
				found = true;
				break;
			}
		}
		if ( found ) break;
	}
	found = false;
	for ( int i = m_width-1; i >= 0; i--)
	{
		for ( int j = _UP_EDGE_; j < _DOWN_EDGE_; j++)
		{
			if ( m_pDataIn[j*m_width + i] == TextColor)
			{
				m_end_x = i;
				found = true;
				break;
			}
		}
		if ( found ) break;
	}


	m_begin_y = _UP_EDGE_;
	m_end_y = _DOWN_EDGE_;



	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j< m_height; j++)
		{
			m_pDataOut[j*m_width*3 + i*3 +0] = 255-TextColor;
			m_pDataOut[j*m_width*3 + i*3 +1] = 255-TextColor;
			m_pDataOut[j*m_width*3 + i*3 +2] = 255-TextColor;
		}
	}



		//BuildNahash1();
	CalculateLetters();
	
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pDataOut1[j*3*m_width+i*3]  = m_pDataIn[j*m_width+i] ;
			}
		}
#ifdef _PRINT_DEBUG_BMP_

		FILE * fp = fopen("C://Temp//oneLineFill3.bmp", "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataOut1,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
#endif
	}

	CalculateBodyHeight();
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pDataOut1[j*3*m_width+i*3]  = m_pDataIn[j*m_width+i] ;
			}
		}
#ifdef _PRINT_DEBUG_BMP_

		FILE * fp = fopen("C://Temp//oneLineFill4.bmp", "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataOut1,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
#endif
	}
	BuildNahashim();
#ifdef _PRINT_DEBUG_BMP_
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j< m_height; j++)
		{
		//	if ( m_pWorkData1[j*m_width*3 + i*3 + 0] < 0) continue;
			if ( m_pWorkData1[j*m_width*3 + i*3 + 1] < 0) continue;
			int r = 0;
			int g = 0;
			int b = 0;
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				int snake = m_pWorkData1[j*m_width*3 + i*3 + 1];
				r = (snake*97 ) % 255;
				g = (snake*81) % 255;
				b = (snake*117) % 255;
			}
			m_pDataOut[j*m_width*3 + i*3 +0] = r;
			m_pDataOut[j*m_width*3 + i*3 +1] = g;
			m_pDataOut[j*m_width*3 + i*3 +2] = b;
			/*
		    m_pDataOut[j*m_width*3 + i*3 +0] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 +1] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn[j*m_width+i];
	
			*/
		}
	
	}
	{
		FILE* fp = fopen("C://Temp//oneLineFill41.bmp", "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}
	for ( int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
		    m_pDataOut[j*m_width*3 + i*3 +0] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 +1] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn[j*m_width+i];
		}
	}	
#endif
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pDataOut1[j*3*m_width+i*3]  = m_pDataIn[j*m_width+i] ;
			}
		}
#ifdef _PRINT_DEBUG_BMP_

		FILE * fp = fopen("C://Temp//oneLineFill5.bmp", "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataOut1,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
#endif
	}
	if (!isTable)
	{

		int n = ChooseNahash();	
#ifdef _PRINT_DEBUG_BMP_

		{
			for ( int i = 0; i < m_width; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					m_pDataOut1[j*3*m_width+i*3]  = m_pDataIn[j*m_width+i] ;
				}
			}

			FILE * fp = fopen("C://Temp//oneLineFill60.bmp", "wb");
			if (fp)
			{
				WriteBMP(fp,m_pDataOut1,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
				fclose(fp);
			}
		}
#endif
		//if (m_language == APP_ENGINE_LNG_JAPANESE && m_isChineseVertical)
		{
			AddToChosenSnake(n);
#ifdef _PRINT_DEBUG_BMP_

			{
				for (int i = 0; i < m_width; i++)
				{
					for (int j = 0; j < m_height; j++)
					{
						m_pDataOut1[j * 3 * m_width + i * 3] = m_pDataIn[j*m_width + i];
					}
				}

				FILE * fp = fopen("C://Temp//oneLineFill61.bmp", "wb");
				if (fp)
				{
					WriteBMP(fp, m_pDataOut1, PIC24, m_width, m_height, NULL, NULL, NULL, 8, F_FULLCOLOR);
					fclose(fp);
				}
			}

#endif
		}
		/*{
			FILE* fp = fopen("H://Temp//debug2.bmp", "wb");
			if (fp)
			{
				WriteBMP(fp,m_pDataIn3,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
				fclose(fp);
			}
		
		}*/
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j< m_height; j++)
			{
				m_pDataOut[j*m_width*3 + i*3 +0] = 0;
				m_pDataOut[j*m_width*3 + i*3 +1] = 0;
				m_pDataOut[j*m_width*3 + i*3 +2] = 0;
			
				if ( m_pWorkData1[j*m_width*3 + i*3 + 1] != n) continue;
			
				m_pDataOut[j*m_width*3 + i*3 +0] = m_pDataIn3[j*m_width*3+i*3+2];
				m_pDataOut[j*m_width*3 + i*3 +1] = m_pDataIn3[j*m_width*3+i*3+2];
				m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn3[j*m_width*3+i*3+2];
			}
		}

		TestForFirstWordScannedBad();
		int firstLetter_i1 = m_width-1;
		int firstLetter_i2 =  m_width-1;
		int firstLetter_j1 = -1;
		int firstLetter_j2 = -1;
		int lastLetter_i1 = -1;
		int lastLetter_i2 = -1;
		int lastLetter_j1 = -1;
		int lastLetter_j2 = -1;
		for ( int l = 0; l < m_numberLetters; l++)
		{
			if (m_letterEdges[l][0] == -1) continue;
			if ( m_nahashim[l] != n) continue;
			int i1 = m_letterEdges[l][0];
			int i2 = m_letterEdges[l][2];
			int j1 = m_letterEdges[l][1];
			int j2 = m_letterEdges[l][3];
			if (firstLetter_i1 > i1)
			{
				firstLetter_i1 = i1;
				firstLetter_i2 = i2;
				firstLetter_j1 = j1;
				firstLetter_j2 = j2;
			}
			if (lastLetter_i2 < i2)
			{
				lastLetter_i1 = i1;
				lastLetter_i2 = i2;
				lastLetter_j1 = j1;
				lastLetter_j2 = j2;
			}
			for ( int i = i1; i <= i2; i++)
			{
				for ( int j = j1; j <= j2; j++)
				{
					m_pDataOut[j*m_width*3 + i*3 +0] = m_pDataIn3[j*m_width*3+i*3+2];
					m_pDataOut[j*m_width*3 + i*3 +1] = m_pDataIn3[j*m_width*3+i*3+2];
					m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn3[j*m_width*3+i*3+2];
				}
			}


		}
#ifdef _PRINT_DEBUG_BMP_

		{
			FILE* fp = fopen("C://Temp//oneFill6.bmp", "wb");
			if (fp)
			{
				WriteBMP(fp,m_pDataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
				fclose(fp);
			}
		}
#endif		

		if (/*!isAsialanguage(m_language ) &&*/ m_language != APP_ENGINE_LNG_CMC7)
		{
			RemoveGarbageAtBeginAndFinish();
			{
				int widthFirstLetter = 30;

				if (m_begin_x_1 > 0)
				{
					firstLetter_j1 = m_height - 1;
					firstLetter_j2 = 0;
					for ( int i = m_begin_x_1; i < m_begin_x_1 + 60; i++)
					{
						if ( i >= m_width-1 || i <= 0) continue; 
						for ( int j = 0; j < m_height; j++)
						{
							if (m_pDataOut[j*m_width*3 + i*3 +0] == TextColor)
							{
								if (firstLetter_j1 > j) 
									firstLetter_j1 = j;
								if (firstLetter_j2 < j) 
									firstLetter_j2 = j;
							}
						}
					}
				}
				else
				{
					m_begin_x_1 = firstLetter_i1;
				}

				/*for ( int i = m_begin_x_1 - widthFirstLetter; i <= m_begin_x_1; i++)
				{
					if ( i >= m_width-1 || i <= 0) continue; 
					for ( int j = firstLetter_j1-5; j <= firstLetter_j2+5; j++)
					{
						if ( j >= m_height-1 || j <= 0) continue; 
						m_pDataOut[j*m_width*3 + i*3 +0] = m_pDataIn3[j*m_width*3+i*3+2];
						m_pDataOut[j*m_width*3 + i*3 +1] = m_pDataIn3[j*m_width*3+i*3+2];
						m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn3[j*m_width*3+i*3+2];
					}
				}*/
			
			}
			{
			}
			{
				int widthLastLetter = 30;
				
				if (m_end_x_1 < m_width-1)
				{
					lastLetter_j1 = m_height - 1;
					lastLetter_j2 = 0;
					for ( int i = m_end_x_1-60; i < m_end_x_1; i++)
					{
						if ( i >= m_width-1 || i <= 0) continue; 
						for ( int j = 0; j < m_height; j++)
						{
							if (m_pDataOut[j*m_width*3 + i*3 +0] == TextColor)
							{
								if (lastLetter_j1 > j) 
									lastLetter_j1 = j;
								if (lastLetter_j2 < j) 
									lastLetter_j2 = j;
							}
						}
					}
				}
				else
				{
					m_end_x_1 = lastLetter_i1;
				}
				/*
				for ( int i = m_end_x_1; i <= m_end_x_1+widthLastLetter; i++)
				{
					if ( i >= m_width-1 || i <= 0) continue; 
					for ( int j = lastLetter_j1-5; j <= lastLetter_j2+5; j++)
					{
						if ( j >= m_height-1 || j <= 0) continue; 
						m_pDataOut[j*m_width*3 + i*3 +0] = m_pDataIn3[j*m_width*3+i*3+2];
						m_pDataOut[j*m_width*3 + i*3 +1] = m_pDataIn3[j*m_width*3+i*3+2];
						m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn3[j*m_width*3+i*3+2];
					}
				}*/
			}
		}
		RemoveHorizontalLines();
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (m_pDataIn[j*m_width+i] == 127)
				{
					m_pDataOut[j*m_width*3 + i*3 +0] = 255 - TextColor;
					m_pDataOut[j*m_width*3 + i*3 +1] = 255 - TextColor;
					m_pDataOut[j*m_width*3 + i*3 +2] = 255 - TextColor;
				//	m_pDataIn3[j*m_width*3+i*3+0] = 255 - TextColor;
				//	m_pDataIn3[j*m_width*3+i*3+1] = 255 - TextColor;
				//	m_pDataIn3[j*m_width*3+i*3+2] = 255 - TextColor;
				}
			}
		}
		for ( int i = 0; i < m_width; i++)
		{
			if (m_pWorkData7[i])
			{
				for (int j = 0; j < m_height; j++)
				{
					m_pDataOut[j*m_width*3 + i*3 +0] = 255 - TextColor;
					m_pDataOut[j*m_width*3 + i*3 +1] = 255 - TextColor;
					m_pDataOut[j*m_width*3 + i*3 +2] = 255 - TextColor;
				}
			}
		}
		return;
	}
	if (!isTable) 
		return;
	int numberBorderPixels = 0;
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if ( m_pWorkData6[j*m_width+i*3+i*3+1] > 0)
			{
				numberBorderPixels++;
			}
		}
	}
	if (numberBorderPixels > m_width*2)
		ChooseLineFromNahashim_1();
	else
		ChooseLineFromNahashim();
	//RemoveTableLinesSnakes();

	for ( int i = m_begin_x_1; i < m_end_x_1; i++)
	{
		for ( int j = 0; j< m_height; j++)
		{
			if ( m_pWorkData1[j*m_width*3 + i*3 + 0] < 0) continue;
			if ( m_pWorkData1[j*m_width*3 + i*3 + 1] < 0) continue;
			/*int r = 0;
			int g = 0;
			int b = 0;
			if (m_pDataIn[j*m_width+i] == 0)
			{
				int snake = m_pWorkData1[j*m_width*3 + i*3 + 1];
				r = (snake*97 ) % 255;
				g = (snake*81) % 255;
				b = (snake*117) % 255;
			}
			m_pDataOut[j*m_width*3 + i*3 +0] = r;
			m_pDataOut[j*m_width*3 + i*3 +1] = g;
			m_pDataOut[j*m_width*3 + i*3 +2] = b;
			*/
		    m_pDataOut[j*m_width*3 + i*3 +0] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 +1] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn[j*m_width+i];
	
			
		}
	}
#ifdef false //_PRINT_DEBUG_BMP_
	{
		FILE* fp = fopen("C://Temp//oneLineFill62.bmp", "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}
#endif
	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		m_snakePresents[n] = false;
	}

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int n = m_pWorkData1[j*m_width*3 + i*3 + 0];
			if ( n >= 0)
				m_snakePresents[n] = true;
		}
	}

	int numberOutputSnakes = 0;
	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		if ( !m_snakePresents[n]) continue;

		int minX = m_width;
		int maxX = 0;

		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j< m_height; j++)
			{
				if ( m_pWorkData1[j*m_width*3 + i*3 + 0] != n) continue;
				if ( i < minX )
					minX = i;
				if ( i > maxX)
					maxX = i;
			}
		}
		m_outputSnakes[numberOutputSnakes*2+0] = minX;
		m_outputSnakes[numberOutputSnakes*2+1] = maxX;
		numberOutputSnakes++;
		if (numberOutputSnakes >= m_maxNumberOutputSnakes) break;

	}
 
	for ( int im = 0; im < numberOutputSnakes; im++)
	{
		for ( int im1 = im+1; im1 < numberOutputSnakes; im1++)
		{
			if ( m_outputSnakes[im1*2+0] < m_outputSnakes[im*2+0])
			{
				int tmp = m_outputSnakes[im1*2+0];
				m_outputSnakes[im*2+0] = tmp;
				m_outputSnakes[im1*2+0] = m_outputSnakes[im*2+0];

				tmp = m_outputSnakes[im1*2+1];
				m_outputSnakes[im*2+1] = tmp;
				m_outputSnakes[im1*2+1] = m_outputSnakes[im*2+1];
			}
		}
	}
	int count = 0;
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pWorkData5[j*m_width+i] == 127)
				count++;
		}
	}
	if ( count > m_height*2)
	{
		for ( int im = 0; im < numberOutputSnakes-1; im++)
		{
			int i1 = m_outputSnakes[(im)*2+1];
			int i2 = m_outputSnakes[(im+1)*2+0];
			if (i1 == -1 || i2 == -1) continue;
			int count = 0;
			for ( int i = i1; i < i2; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					if (m_pWorkData5[j*m_width+i] == 127)
						count++;
				}
			}
			if (count > 90)
			{
				m_outputSnakes[(im)*2+1] =  m_outputSnakes[(im+1)*2+1];
				m_outputSnakes[(im+1)*2+0] = -1;
				m_outputSnakes[(im+1)*2+1] = -1;
			}
		}
	}
	else
	{
		int spaces[_MAX_NUMBER_NAHASHIM_];
		int spacesSort[_MAX_NUMBER_NAHASHIM_];
		for (int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
		{
			spaces[n] = 0;
			spacesSort[n] = 0;
		}
		for ( int im = 0; im < numberOutputSnakes-1; im++)
		{
			spaces[im] = m_outputSnakes[(im+1)*2+0] - m_outputSnakes[(im)*2+1] ;
			spacesSort[im] = spaces[im];
		}
		for (  int im = 0; im < numberOutputSnakes-1; im++)
		{
			for (  int im1 = 0; im1 < numberOutputSnakes-2; im1++)
			{
				if (spacesSort[im] < spacesSort[im1])
				{
					int tmp = spacesSort[im];
					spacesSort[im]  = spacesSort[im1];
					spacesSort[im1] = tmp;
				}
			}
		}
		int minSpace = 0;
		for ( int im = 0; im < numberOutputSnakes-1; im++)
		{
			if (spacesSort[im] > 3*spacesSort[im+1])
			{
				minSpace = 3*spacesSort[im+1];
				break;
			}
		}

		for ( int im = 0; im < numberOutputSnakes-1; im++)
		{
			if ( spaces[im] < minSpace)
			{
				m_outputSnakes[(im)*2+1] =  m_outputSnakes[(im+1)*2+1];
				m_outputSnakes[(im+1)*2+0] = -1;
				m_outputSnakes[(im+1)*2+1] = -1;
			}
		}
	}
	bool found1 = true;
	while (found1)
	{
		found1 = false;
		int im_delete = -1;
		for ( int im = 0; im < numberOutputSnakes; im++)
		{
			if (m_outputSnakes[(im)*2+0] == -1 && m_outputSnakes[(im)*2+1] == -1)
			{
				found1 = true;
				im_delete = im;
				break;
			}
		}
		if (!found1) break;
		for ( int im = im_delete; im < numberOutputSnakes-1; im++)
		{
			m_outputSnakes[(im)*2+0] =  m_outputSnakes[(im+1)*2+0];
			m_outputSnakes[(im)*2+1] =  m_outputSnakes[(im+1)*2+1];
		}
		m_outputSnakes[(numberOutputSnakes-1)*2+0] = -1;
		m_outputSnakes[(numberOutputSnakes-1)*2+1] = -1;
		numberOutputSnakes--;
	}
	return;
	
		{
			/*FILE* fp = fopen("C://Temp//debug2.bmp", "wb");
			if (fp)
			{
				WriteBMP(fp,m_pDataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
				fclose(fp);
			}
			}*/
		}





}
void COneLine_4::TestForFirstWordScannedBad()
{
	m_firstWordBad = false;
	for (int i = 0; i < _FIRST_WORD_SCANNED_BAD_SPACE_;i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pDataOut[j*m_width*3 + i*3 +0] == TextColor)
			{
				m_firstWordBad = true;
				break;
			}
		}

	}
}
void COneLine_4::RemoveGarbageAtBeginAndFinish()
{

	
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 4; j < m_height - 4; j++)
		{
			m_pDataOut1[j*m_width*3+i*3+0] = m_pDataIn[j*m_width+i];

		}
	}
	/*FILE* fp = fopen("C://Temp//debug2.bmp", "wb");
	if (fp)
	{
		WriteBMP(fp,m_pDataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
		fclose(fp);
	}*/
	
	int spaces[300][6];
	int spaces1[300][4];
	int numberSpaces = 0;
	//int s1 = -1; 
	//int s2 = -1;
	bool isSpace = false;
	m_begin_x_1 = 0;
	m_end_x_1 = m_width-1;
	int i;









	int i1= -1;
	for ( int i = 6; i < m_width-10; i++)
	{
		i1 = i;
		bool columnSpace = true;
		for (int j = 0; j < m_height; j++)
		{
			if (m_pDataOut[j*m_width*3+i*3] == TextColor)
			{
				columnSpace = false;
				break;
			}
		}
		if (columnSpace &&  (!isSpace))
		{
			isSpace = true;
			//s1 = i;
			spaces[numberSpaces][0] = i;
		}
		else if (!columnSpace &&  (isSpace))
		{
			isSpace = false;
			//s2 = i;
			


			spaces[numberSpaces++][1] = i;// s2 - s1;
			if (numberSpaces >= 299)
				break;
		}
	}
	if (isSpace && i1 >= m_width-20)
	{
		spaces[numberSpaces++][1] = m_width-10;// s2 - s1;
	}

	
	for ( int s = 0; s < numberSpaces;s++)
	{
		{
			int i1 = spaces[s][0] - 150;
			int i2 = spaces[s][0];
			if ( i1 < 1) i1 = 1;
			if (i2 > m_width-1) i2 = m_width-1;

			int numberChanges = 0;
			int w = 0;
			for ( int ii = i1; ii < i2; ii++)
			{
				bool isSpaceColumn = true;
				for ( int j = 0; j < m_height; j++)
				{
					if ( m_pDataOut[j*m_width*3+ii*3] != m_pDataOut[j*m_width*3+(ii-1)*3])
					//if ( m_pDataOut1[j*m_width+ii] != m_pDataOut1[j*m_width+(ii-1)])
					{
						numberChanges++;
					}
					if ( m_pDataOut[j*m_width*3+ii*3]  == TextColor)
						isSpaceColumn = false;
						
				}
				if (!isSpaceColumn)
				{
					w++;
	}
			}
			spaces[s][2] = numberChanges;
			spaces[s][3] = w;
		}

		{
			int i1 = spaces[s][1] ;
			int i2 = spaces[s][1] + 150;
			if ( i1 < 1) i1 = 1;
			if (i2 > m_width-1) i2 = m_width-1;

			int numberChanges = 0;
			int w = 0;
			for ( int ii = i1; ii < i2; ii++)
			{
				bool isSpaceColumn = true;
				for ( int j = 0; j < m_height; j++)
				{
					if ( m_pDataOut[j*m_width*3+ii*3] != m_pDataOut[j*m_width*3+(ii-1)*3])
					//if ( m_pDataOut1[j*m_width+ii] != m_pDataOut1[j*m_width+(ii-1)])
			{
						numberChanges++;
					}
					if ( m_pDataOut[j*m_width*3+ii*3]  == TextColor)
						isSpaceColumn = false;
					
				}
				if (!isSpaceColumn)
				{
					w++;
		}
	}
			spaces[s][4] = numberChanges;
			spaces[s][5] = w;
		}


	}

	for ( int s = 0; s < numberSpaces;s++)
	{
		{
			int i1 = spaces[s][1] - 150;
			int i2 = spaces[s][1];
			if ( i1 < 1) i1 = 1;
			if (i2 > m_width-1) i2 = m_width-1;

			int numberChanges = 0;
			int w = 0;
			for ( int ii = i1; ii < i2; ii++)
		{
				bool isSpaceColumn = true;
			for ( int j = 0; j < m_height; j++)
			{
					if ( m_pDataIn[j*m_width+ii] != m_pDataIn[j*m_width+(ii-1)])
					//if ( m_pDataOut1[j*m_width+ii] != m_pDataOut1[j*m_width+(ii-1)])
					{
						numberChanges++;
					}
					if ( m_pDataIn[j*m_width+ii]  == TextColor)
						isSpaceColumn = false;
						
				}
				if (!isSpaceColumn)
				{
					w++;
				}
			}
			spaces1[s][0] = numberChanges;
			spaces1[s][1] = w;
		}

		{
			int i1 = spaces[s][0] ;
			int i2 = spaces[s][0] + 150;
			if ( i1 < 1) i1 = 1;
			if (i2 > m_width-1) i2 = m_width-1;

			int numberChanges = 0;
			int w = 0;
			for ( int ii = i1; ii < i2; ii++)
			{
				bool isSpaceColumn = true;
				for ( int j = 0; j < m_height; j++)
				{
					if ( m_pDataIn[j*m_width+ii] != m_pDataIn[j*m_width+(ii-1)])
					//if ( m_pDataOut1[j*m_width+ii] != m_pDataOut1[j*m_width+(ii-1)])
					{
						numberChanges++;
					}
					if ( m_pDataIn[j*m_width+ii]  == TextColor)
						isSpaceColumn = false;
					
				}
				if (!isSpaceColumn)
				{
					w++;
				}
			}
			spaces1[s][2] = numberChanges;
			spaces1[s][3] = w;
		}


	}


	int ratioThreshold = m_bodyHeight*18/10;
	if (numberSpaces <= 1) return;
	int s_begin = 0;
	
	float maxRatio = 0;
	int s_begin_max = -1;
	int numberApproved = 0;
	int s_begin_1 = -1;
	int s_begin_2 = -1;
	int s_begin_3 = -1;
	int s_begin_4 = -1;
	for ( int s = s_begin; s < s_begin+4/*numberSpaces*/; s++)
	{
		//if (spaces[s][2] < 40) continue;
		float widthSpaces = spaces[s][1] - spaces[s][0];
		float ratio = 100;
		if (spaces[s][5] > 0)
			ratio = /*widthSpaces **/ (float)spaces[s][4] / (float) spaces[s][5];
		if ((ratio > 2.0 && widthSpaces > 15 )  ||
			(ratio > 1.5 && widthSpaces > 20 ) ||
			(ratio > 1.0 && widthSpaces > 25) ||
			(ratio > 0.7 && widthSpaces > 38 )  ||
			(ratio > 0.5 && widthSpaces > 65 )||
			(ratio > 0.3 && widthSpaces > 80 )||
			(widthSpaces > 100))
		//if (ratio > ratioThreshold /*&& ratio > maxRatio*/)
		{
			m_begin_x_1 = spaces[s][1] + 1;
			s_begin_max = s;
			maxRatio = ratio;
			if (s_begin_1 == -1)
				s_begin_1 = s;
			numberApproved++;
			//break;
		}
	}
	if (numberApproved == 0 || ((s_begin_max >= 0)&&(spaces[s_begin_max][0] > m_width/2))
		|| s_begin_max >= numberSpaces -1)
	{
		for ( int s = s_begin; s < s_begin+4/*numberSpaces*/; s++)
		{
			//if (spaces[s][2] < 40) continue;
			float widthSpaces = spaces[s][1] - spaces[s][0];
			float ratio = 1000;
			if (spaces[s][5] > 0)
				ratio = /*widthSpaces **/ (float)spaces[s][4] / (float) spaces[s][5];
			if ((ratio > 0.4 && widthSpaces > 50 ) ||
				(ratio > 0.6 && widthSpaces > 20 ) ||
				(ratio > 1.5 && widthSpaces > 15) || 
				(ratio > 3.5 && widthSpaces > 10))				
			//if (ratio > ratioThreshold /*&& ratio > maxRatio*/)
			{
				m_begin_x_1 = spaces[s][1] + 1;
				s_begin_max = s;
				maxRatio = ratio;
				s_begin_2 = s;
				break;
			}
		}
	}
	if (numberApproved > 1)
	{
		for ( int s = s_begin; s < s_begin+4/*numberSpaces*/; s++)
		{
			//if (spaces[s][2] < 40) continue;
			float widthSpaces = spaces[s][1] - spaces[s][0];
			float ratioLeft = 100;
			float ratioRight = 100;
			float ratioLeftAll = 100;
			if (spaces[s][3] > 0)
				ratioLeft = /*widthSpaces **/ (float)spaces[s][2] / (float) spaces[s][3];
			if (spaces[s][5] > 0)
				ratioRight = /*widthSpaces **/ (float)spaces[s][4] / (float) spaces[s][5];
			if (spaces1[s][1] > 0)
				ratioLeftAll = /*widthSpaces **/ (float)spaces1[s][0] / (float) spaces1[s][1];
			if ((ratioRight > 1.0 && widthSpaces > 60 && ratioLeft > 0.25 && ratioLeftAll > 0.4) ||
				(ratioRight > 1.5 && widthSpaces > 20 && ratioLeft > 0.3 && ratioLeftAll > 0.4) ||
				(ratioRight > 1.0 && widthSpaces > 25 && ratioLeft > 0.55 && ratioLeftAll > 0.4) ||
				(ratioRight > 0.7 && widthSpaces > 38 && ratioLeft > 0.55 && ratioLeftAll > 0.4 ) ||
				(ratioRight > 2.0 && widthSpaces > 15  && ratioLeft > 1.0 && ratioLeftAll > 1.0) ||
				( /*ratioLeft >= 99 &&*/ widthSpaces > 50 && ratioLeftAll > 0.4) ||
				(widthSpaces > 400))
			//if (ratio > ratioThreshold /*&& ratio > maxRatio*/)
			{
				m_begin_x_1 = spaces[s][1] + 1;
				s_begin_max = s;
				s_begin_3 = s;
				maxRatio = ratioRight;
				numberApproved++;
				break;
			}
		}
	}
	
	/*if (numberSpaces > s_begin_max+1 )
	{
		int i1 = spaces[s_begin_max][1];
		int i2 = spaces[s_begin_max+1][0];
		int j1 = m_height-1;
		int j2 = 0;
		for ( int ii = i1; ii <= i2; ii++)
		{
			for ( int j = 5; j < m_height-5; j++)
			{
				if (m_pDataOut[j*m_width*3+ii*3] != m_pDataOut[(j-1)*m_width*3+ii*3])
				{
					if (j1 > j) j1 = j;
					if ( j2 < j) j2 = j;
				}
			}
		}
		if  (j2-j1 < m_bodyHeight/2+2)// && i2-i1 < 10)
		{
			s_begin_max = s_begin_max+1;
			s_begin_4 = s_begin_max;
			m_begin_x_1 = spaces[s_begin_max][1] + 1;
		}
	}*/
	
	/*if (numberSpaces > s_max+1 && existAtTheBegin)
	{
		int widthSpaces1 = spaces[s_max][1] - spaces[s_max][0];
		float ratio1 = widthSpaces1 * (float)spaces[s_max][2] / (float) spaces[s_max][3];
		int widthSpaces2 = spaces[s_max+1][1] - spaces[s_max+1][0];
		float ratio2 = widthSpaces2 * (float)spaces[s_max+1][2] / (float) spaces[s_max+1][3];
		if (ratio1 > ratioThreshold && ratio2 > ratioThreshold)
		{
			s_max = s_max+1;
			m_begin_x_1 = spaces[s_max][1] + 1;
		}
	}*/
	maxRatio = 0;
	int s_end_max = -1;
	int s_end_1 = -1;
	int s_end_2 = -1;
	int s_end_3 = -1;
	int s_end_4 = -1;
	int s_end = numberSpaces - 1;
	numberApproved = 0;
	for ( int s = s_end; s > s_end-4; s--)
	{
		//if (spaces[s][2] < 40) continue;
		int widthSpaces = spaces[s][1] - spaces[s][0];
		float ratio = 100;
		if (spaces[s][3] > 0)
			ratio = /*widthSpaces **/ (float)spaces[s][2] / (float) spaces[s][3];
		if ((ratio > 2.0 && widthSpaces > 15 ) ||
			(ratio > 1.5&& widthSpaces > 20 )  ||
			(ratio > 1.0 && widthSpaces > 25)||
			(ratio > 0.7 && widthSpaces > 38 )  ||
			(ratio > 0.5 && widthSpaces > 65 ) ||
			(ratio > 0.3 && widthSpaces > 80 )||
			(widthSpaces > 100))
		//if (ratio > ratioThreshold /*&& ratio > maxRatio*/)

		{
			m_end_x_1 = spaces[s][0] - 1;
			maxRatio = ratio;
			s_end_max = s;
			if (s_end_1 == -1)
				s_end_1 = s;
			numberApproved++;
			//break;
		}
	}
	if (numberApproved == 0 || ((s_end_max >= 0) && (spaces[s_end_max][0] < m_width/2)) || s_end_max <= 0)
	{
		for ( int s = s_end; s > s_end-4; s--)
		{
			//if (spaces[s][2] < 40) continue;
			int widthSpaces = spaces[s][1] - spaces[s][0];
			float ratio = 1000;
			if (spaces[s][3] > 0)
				ratio = /*widthSpaces **/ (float)spaces[s][2] / (float) spaces[s][3];
			if ((ratio > 0.4 && widthSpaces > 50 ) ||
				(ratio > 0.6 && widthSpaces > 20) ||
				(ratio > 1.5 && widthSpaces > 15)|| 
				(ratio > 3.5 && widthSpaces > 10))
			//if (ratio > ratioThreshold /*&& ratio > maxRatio*/)

			{
				m_end_x_1 = spaces[s][0] - 1;
				maxRatio = ratio;
				s_end_max = s;
				s_end_2 = s;
				break;
			}
		}
	}
	if ( numberApproved > 1)
	{
		for ( int s = s_end; s > s_end-4; s--)
		{
			//if (spaces[s][2] < 40) continue;
			int widthSpaces = spaces[s][1] - spaces[s][0];
			float ratioLeft = 100;
			float ratioRight = 100;
			float ratioRightAll = 100;
			if (spaces1[s][3] > 0)
				ratioRightAll = /*widthSpaces **/ (float)spaces1[s][2] / (float) spaces1[s][3];
			if (spaces[s][3] > 0)
				ratioLeft = /*widthSpaces **/ (float)spaces[s][2] / (float) spaces[s][3];
			if (spaces[s][5] > 0)
				ratioRight = /*widthSpaces **/ (float)spaces[s][4] / (float) spaces[s][5];
			if ((ratioLeft > 1.0 && widthSpaces > 60 && ratioRight > 0.25 && ratioRightAll > 0.4) ||
				(ratioLeft > 1.5 && widthSpaces > 20 && ratioRight > 0.3 && ratioRightAll > 0.4)  ||
				(ratioLeft > 1.0 && widthSpaces > 25 && ratioRight > 0.55 && ratioRightAll > 0.4)||
				(ratioLeft > 0.7 && widthSpaces > 40  && ratioRight > 0.55 && ratioRightAll > 0.4) ||
				(ratioLeft > 2.0 && widthSpaces > 15  && ratioRight > 1.0 && ratioRightAll > 1.0) ||
				( /*ratioRight >= 99 && */widthSpaces > 50 && ratioRightAll > 0.3 ) ||
				(widthSpaces > 400 ))
			//if (ratio > ratioThreshold /*&& ratio > maxRatio*/)

			{
				m_end_x_1 = spaces[s][0] - 1;
				maxRatio = ratioLeft;
				s_end_max = s;
				s_end_3 = s;
				numberApproved++;
				break;
			}
		}
	}
	/*if (0 <= s_end_max-1 )
	{
		int i1 = spaces[s_end_max-1][1];
		int i2 = spaces[s_end_max][0];
		int j1 = m_height-1;
		int j2 = 0;
		for ( int ii = i1; ii <= i2; ii++)
		{
			for ( int j = 5; j < m_height-5; j++)
			{
				if (m_pDataOut[j*m_width*3+ii*3+0] != m_pDataOut[(j-1)*m_width*3+ii*3])
				{
					if (j1 > j) j1 = j;
					if ( j2 < j) j2 = j;
				}
			}
		}
		if  (j2-j1 < m_bodyHeight/2+2)// && i2-i1 < 10)
		{
			s_end_max = s_end_max-1;
			s_end_4 = s_end_max;
			m_end_x_1 = spaces[s_end_max][0] - 1;
		}
	*/
	/*if (0 <= s_max-1 && existAtTheEnd)
	{
		int widthSpaces1 = spaces[s_max][1] - spaces[s_max][0];
		float ratio1 = widthSpaces1 * (float)spaces[s_max][2] / (float) spaces[s_max][3];
		int widthSpaces2 = spaces[s_max-1][1] - spaces[s_max-1][0];
		float ratio2 = widthSpaces2 * (float)spaces[s_max-1][2] / (float) spaces[s_max-1][3];
		if (ratio1 > ratioThreshold && ratio2 > ratioThreshold)
		{
			s_max = s_max-1;
			m_end_x_1 = spaces[s_max][0] - 1;
		}
	}*/
	if (s_begin_max >= s_end_max && s_begin_max != -1 && s_end_max != -1)
	{
		int max_s[4];
		max_s[0] = s_begin_1;
		max_s[1] = s_begin_2;
		max_s[2] = s_begin_3;
		max_s[3] = s_begin_4;
		for ( int k = 0; k < 4; k++)
		{
			for ( int k1 = k+1; k < 4; k++)
			{
				if (max_s[k] > max_s[k1])
				{
					int tmp = max_s[k];
					max_s[k] = max_s[k1];
					max_s[k1] = tmp;
				}
			}
		}

		int k= 0;
		while (k < 4)
		{
			if ( max_s[k] >= 0)
			{
				s_begin_max = max_s[k];
				break;
			}
			k++;
		}
		
		if (s_begin_max >= 0)
		m_begin_x_1 = spaces[s_begin_max][0] - 1;
		else
			m_begin_x_1 = 0;

			
		max_s[0] = s_end_1;
		max_s[1] = s_end_2;
		max_s[2] = s_end_3;
		max_s[3] = s_end_4;
		for ( int k = 0; k < 4; k++)
		{
			for ( int k1 = k+1; k < 4; k++)
			{
				if (max_s[k] > max_s[k1])
				{
					int tmp = max_s[k];
					max_s[k] = max_s[k1];
					max_s[k1] = tmp;
				}
			}
		}
		k= 3;
		while (k >= 0)
		{
			if ( max_s[k] >= 0)
			{
				s_end_max = max_s[k];
				break;
			}
			k--;
		}
		
		if (s_end_max >= 0)
		m_end_x_1 = spaces[s_end_max][0] - 1;
		else
			m_end_x_1 = m_width-1;
	}

	if (s_begin_max >= s_end_max && s_begin_max != -1 && s_end_max != -1)
	{
		s_begin_max = -1;
		s_end_max = -1;
	}
	if ( s_begin_max == -1)
		m_begin_x_1 = 0;
	if (s_end_max == -1)
		m_end_x_1 = m_width;
	
	if ( m_begin_x_1 == -1)
		m_begin_x_1 = 0;
	if (m_end_x_1 == -1)
		m_end_x_1 = m_width;


	for ( int i = 0; i < m_begin_x_1; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_width*3+i*3+0] = 255 - TextColor;
			m_pDataOut[j*m_width*3+i*3+1] = 255 - TextColor;
			m_pDataOut[j*m_width*3+i*3+2] = 255 - TextColor;
		}
	}
	for ( int i = m_end_x_1; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_width*3+i*3+0] = 255 - TextColor;
			m_pDataOut[j*m_width*3+i*3+1] = 255 - TextColor;
			m_pDataOut[j*m_width*3+i*3+2] = 255 - TextColor;
		}
	}
	return;
}
	

	

void COneLine_4::AddToChosenSnake(int choosenSnake)
{
/*	int begin_x = m_width-1;
	int end_x = 0;
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height;j++)
		{
			if (m_pDataIn3[j*m_width*3+i*3+2] == TextColor)
			{
				if ( begin_x > i) begin_x = i;
				if (end_x < i ) end_x = i;
			}
		}
	}
	*/

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j< m_height; j++)
		{
			if ( m_pWorkData1[j*m_width*3 + i*3 + 1] < 0) continue;
			int r = 0;
			int g = 0;
			int b = 0;
			if (m_pDataIn[j*m_width+i] == 0)
			{
				int snake = m_pWorkData1[j*m_width*3 + i*3 + 1];
				r = (snake*97 ) % 255;
				g = (snake*81) % 255;
				b = (snake*117) % 255;
			}
			m_pDataOut[j*m_width*3 + i*3 +0] = r;
			m_pDataOut[j*m_width*3 + i*3 +1] = g;
			m_pDataOut[j*m_width*3 + i*3 +2] = b;
			
		    //m_pDataOut[j*m_width*3 + i*3 +0] = m_pDataIn[j*m_width+i];
			//m_pDataOut[j*m_width*3 + i*3 +1] = m_pDataIn[j*m_width+i];
			//m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn[j*m_width+i];
	
			
		}
	}
	/*{
			FILE* fp = fopen("C://programdata//irispen/Snake_debug2.bmp", "wb");
			if (fp)
			{
				WriteBMP(fp,m_pDataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
				fclose(fp);
		}
	}*/




	int beginSnake[_MAX_NUMBER_NAHASHIM_];
	int endSnake[_MAX_NUMBER_NAHASHIM_];
	
	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		beginSnake[n] = m_width-1;
		endSnake[n] = 0;
	}

	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		for ( int j = _UP_EDGE_; j <= _DOWN_EDGE_; j++)
		{
			int n = m_pWorkData1[j*m_width*3+i*3+1];
			if (n < 0) continue;
			if (beginSnake[n] > i) beginSnake[n] = i;
			if (endSnake[n] < i) endSnake[n] = i;
		}
	}


	int count = 0;
	bool tryToUnify = true;
	while (count <= 10 && tryToUnify)
	{
		count++;
		tryToUnify = false;

		int beginChoosenSnake = m_width-1;
		int endChoosenSnake = 0;

		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height;j++)
			{
				if (m_pWorkData1[j*m_width*3 + i*3 + 1] == choosenSnake)
				{
					if ( beginChoosenSnake > i) beginChoosenSnake = i;
					if (endChoosenSnake < i ) endChoosenSnake = i;
				}
			}
		}
/*
		for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
		{
			if (n == choosenSnake) continue;
			if ( beginSnake[n] > endSnake[n]) continue;
			int beginSnakeCommon1 = -1;
			int endSnakeCommon2 = -1;
			int n_snakeBegin = -1;
			int n_snakeEnd = -1;

			
			if (abs(beginChoosenSnake - endSnake[n]) < 200)
			{
				beginSnakeCommon1 = beginChoosenSnake;
				endSnakeCommon2 = endSnake[n];
				n_snakeBegin = choosenSnake;
				n_snakeEnd = n;
			}
			else if (abs(endChoosenSnake - beginSnake[n]) < 200)
			{
				beginSnakeCommon1 = beginSnake[n];
				endSnakeCommon2 = endChoosenSnake;
				n_snakeBegin = n;
				n_snakeEnd = choosenSnake;
			}
			else 
				continue;


			int j1_1 = m_height-1;
			int j2_1 = 0;
			for ( int i = beginSnakeCommon1; i < beginSnakeCommon1+ 200; i++)
			{
				if ( i >= m_width - 5) continue;
				for ( int j = _UP_EDGE_; j <= _DOWN_EDGE_; j++)
				{
					if (m_pWorkData1[j*m_width*3+i*3+1] == n_snakeBegin)
					{
						if ( j1_1 > j) j1_1 = j;
						if ( j2_1 < j) j2_1 = j;
					}
				}
			}
			int j1_2 = m_height-1;
			int j2_2 = 0;
			for ( int i = endSnakeCommon2-200; i < endSnakeCommon2; i++)
			{
				if ( i < 5) continue;
				for ( int j = _UP_EDGE_; j <= _DOWN_EDGE_; j++)
				{
					if (m_pWorkData1[j*m_width*3+i*3+1] == n_snakeEnd)
					{
						if ( j1_2 > j) j1_2 = j;
						if ( j2_2 < j) j2_2 = j;
					}
				}
			}
			int j1Common = (j1_1 > j1_2) ? j1_1 : j1_2;
			int j2Common = (j2_1 < j2_2) ? j2_1 : j2_2;
			bool toUnify = false;
			if (j2Common - j1Common > 10)
			{
				toUnify = true;
			}
			if ( !toUnify) 
				continue;

			for ( int i = beginSnake[n]; i <= endSnake[n]; i++)
			{
				for (int j = 0; j < m_height; j++)
				{
					if ( m_pWorkData1[j*m_width*3+i*3+1] == n)
						m_pWorkData1[j*m_width*3+i*3+1] = choosenSnake;
				}
			}
			for ( int l = 0; l  < _MAX_NUMBER_LETTERS_; l++)
			{
				if ( m_nahashim[l] == n)
					m_nahashim[l] = choosenSnake;
			}

			beginSnake[n] = m_width-1;
			endSnake[n] = 0;
			tryToUnify = true;
			break;
		}
	

*/

		{
			int jChoosenLeft_1 = m_height-1;
			int jChoosenLeft_2 = 0;
			for ( int i= beginChoosenSnake; i <= beginChoosenSnake+100; i++)
			{
				if ( i >= m_width-5) continue;
				for ( int j = _UP_EDGE_; j <= _DOWN_EDGE_; j++)
				{
					if (m_pWorkData1[j*m_width*3+i*3+1] == choosenSnake)
					{
						if ( jChoosenLeft_1 > j) jChoosenLeft_1 = j;
						if ( jChoosenLeft_2 < j) jChoosenLeft_2 = j;
					}
				}
			}
			int snakeToUnifyWith = -1;
			int distLeft = beginChoosenSnake - m_begin_x;
			for ( int i= beginChoosenSnake; i >= beginChoosenSnake-distLeft; i--)
			{
				if ( i <= 0) continue;
				int delta = (jChoosenLeft_2 - jChoosenLeft_1) / 7 * 2; //33% of line height in given point
				for ( int j = jChoosenLeft_1+delta; j <= jChoosenLeft_2-delta; j++)
				{
					if (m_pWorkData1[j*m_width*3+i*3+1] !=-1 && m_pWorkData1[j*m_width*3+i*3+1] != choosenSnake)
					{
						snakeToUnifyWith = m_pWorkData1[j*m_width*3+i*3+1];
						break;
					}
				}
				if (snakeToUnifyWith != -1)
					break;

			}
			if (snakeToUnifyWith != -1)
			{
				for ( int i = 0; i <= m_width; i++)
					{
						for (int j = 0; j < m_height; j++)
						{
							if ( m_pWorkData1[j*m_width*3+i*3+1] == snakeToUnifyWith)
								m_pWorkData1[j*m_width*3+i*3+1] = choosenSnake;
						}
					}
					for ( int l = 0; l  < _MAX_NUMBER_LETTERS_; l++)
					{
						if ( m_nahashim[l] == snakeToUnifyWith)
							m_nahashim[l] = choosenSnake;
					}

					beginSnake[snakeToUnifyWith] = m_width-1;
					endSnake[snakeToUnifyWith] = 0;
					tryToUnify = true;
					//break;
			}
		}

		{
			int jChoosenRight_1 = m_height-1;
			int jChoosenRight_2 = 0;
			for ( int i= endChoosenSnake-100; i <= endChoosenSnake; i++)
			{
				if ( i <= 0) continue;
				for ( int j = _UP_EDGE_; j <= _DOWN_EDGE_; j++)
				{
					if (m_pWorkData1[j*m_width*3+i*3+1] == choosenSnake)
					{
						if ( jChoosenRight_1 > j) jChoosenRight_1 = j;
						if ( jChoosenRight_2 < j) jChoosenRight_2 = j;
					}
				}
			}
			int snakeToUnifyWith = -1;
			int distRight = m_end_x - endChoosenSnake;;
			for ( int i= endChoosenSnake; i <= endChoosenSnake+distRight; i++)
			{
				if ( i >= m_width-5) continue;
				int delta = (jChoosenRight_2 - jChoosenRight_1) /7 * 2; //33% of line height
				for ( int j = jChoosenRight_1+delta; j <= jChoosenRight_2-delta; j++)
				{
					if (m_pWorkData1[j*m_width*3+i*3+1] !=-1 && m_pWorkData1[j*m_width*3+i*3+1] != choosenSnake)
					{
						snakeToUnifyWith = m_pWorkData1[j*m_width*3+i*3+1];
						break;
					}
				}
				if (snakeToUnifyWith != -1)
					break;

			}
			if (snakeToUnifyWith != -1)
			{
				for ( int i = 0; i <= m_width; i++)
					{
						for (int j = 0; j < m_height; j++)
						{
							if ( m_pWorkData1[j*m_width*3+i*3+1] == snakeToUnifyWith)
								m_pWorkData1[j*m_width*3+i*3+1] = choosenSnake;
						}
					}
					for ( int l = 0; l  < _MAX_NUMBER_LETTERS_; l++)
					{
						if ( m_nahashim[l] == snakeToUnifyWith)
							m_nahashim[l] = choosenSnake;
					}

					beginSnake[snakeToUnifyWith] = m_width-1;
					endSnake[snakeToUnifyWith] = 0;
					tryToUnify = true;
					//break;
			}
		}
	}


}
#define _UP_MIDDLE_BOUND_ 43
#define _DOWN_MIDDLE_BOUND_ 87
#define _SIZE_MIDDLE_ 44

void COneLine_4::ChooseLineFromNahashim_1()
{
	for ( int s = 0; s < _MAX_NUMBER_NAHASHIM_; s++)
	{
		m_nahashimEdgesUpDown[s][0] = m_height-1;
		m_nahashimEdgesUpDown[s][1] = 0;
	}
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int s = m_pWorkData1[j*m_width*3 + i*3 + 0];
			if ( s < 0 || s >= _MAX_NUMBER_NAHASHIM_) continue; 
			if ( j < m_nahashimEdgesUpDown[s][0]) 
				m_nahashimEdgesUpDown[s][0] = j;
			if ( j > m_nahashimEdgesUpDown[s][1]) 
				m_nahashimEdgesUpDown[s][1] = j;

		}
	}
	
	
	/*for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 + 0] = -1;
		}
	}*/
			
	int i = m_begin_x;
	int firstSnake = -1;
	int i1 = m_width; 
	int i2 = 0;
	int j1 = m_height; 
	int j2 = 0;
	while ( i < m_end_x)
	{
		while (i < m_end_x)
		{
			for ( int j = _UP_MIDDLE_BOUND_; j < _DOWN_MIDDLE_BOUND_; j++)
			{
				if ( m_pWorkData1[j*m_width*3+i*3+1] >= 0 && m_pDataIn[j*m_width+i] != 127)
				{
					firstSnake  = m_pWorkData1[j*m_width*3+i*3+1];
					break;
				}
			}
			if ( firstSnake >= 0) break;
			i++;
		}
	

		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if ( m_pWorkData1[j*m_width*3+i*3+1] == firstSnake )
				{
					if ( i1 > i) i1 = i;
					if ( i2 < i) i2 = i;
					if ( j1 > j) j1 = j;
					if ( j2 < j) j2 = j;
				}
			}
		}
		if ( i2 -i1 < 30 || j2-j1 < 10)
		{
			i = i2+1;
			firstSnake = -1;
			i1 = m_width; 
			i2 = 0;
			j1 = m_height; 
			j2 = 0;
		}
		else
			break;
	}
	if ( i >= m_end_x) return;
	int snakesUp[10] = {-1, -1,-1, -1, -1, -1, -1, -1, -1, -1};
	int snakesDown[10] = {-1, -1,-1, -1, -1, -1, -1, -1, -1, -1};

	
	for ( int i = i1; i < i2; i++)
	{
		int j = j1;
		int edgeIndex = m_pWorkData6[j*m_width*3 + i*3 + 1];
		while (edgeIndex <= 0)
		{
			j--;
			if ( j <=0) break;
			edgeIndex = m_pWorkData6[j*m_width*3 + i*3 + 1];
		}
		if ( edgeIndex > 0)
		{
			for ( int s = 0; s < 10; s++)
			{
				if ( snakesUp[s] == -1)
				{
					snakesUp[s] = edgeIndex;
					break;
				}
				if (snakesUp[s] == edgeIndex)
					break;
			}
		}



		j = j2;
		edgeIndex = m_pWorkData6[j*m_width*3 + i*3 + 1];
		while (edgeIndex <= 0)
		{
			j++;
			if ( j >= m_height-1) break;
			edgeIndex = m_pWorkData6[j*m_width*3 + i*3 + 1];
		}
		if ( edgeIndex > 0)
		{
			for ( int s = 0; s < 10; s++)
			{
				if ( snakesDown[s] == -1)
				{
					snakesDown[s] = edgeIndex;
					break;
				}
				if (snakesDown[s] == edgeIndex)
					break;
			}
		}
	}

	
	//int snakesChoosen[100];

	/*for ( int s = 0; s < 100; s++)
		snakesChoosen[s] = -1;
*/

	for ( int i = m_begin_x; i < m_end_x; i++)
	{

		int j1 = 0;
		for ( int j = 1; j < m_height; j++)
		{
			if ( m_pWorkData6 [j*m_width*3 + i*3 + 0] != 127) continue;
			int currentEdge= m_pWorkData6 [j*m_width*3 + i*3 + 1];
			for ( int s = 0; s < 10; s++)
			{
				if (currentEdge == snakesUp[s])
				{
					j1 = j;
					break;
				}
			}
			if ( j1 > 0) break;
		}
		//if ( j1 == 0) continue;


		int j2 = m_height -1;
		for ( int j = m_height-1; j >= 0; j--)
		{
			if ( m_pWorkData6 [j*m_width*3 + i*3 + 0] != 127) continue;
			int currentEdge= m_pWorkData6 [j*m_width*3 + i*3 + 1];
			for ( int s = 0; s < 10; s++)
			{
				if (currentEdge == snakesDown[s])
				{
					j2 = j;
					break;
				}
			}
			if ( j2 < m_height-1) break;
		}
		//if ( j2 == m_height-1) continue;

		if ( j1 == 0 && j2 == m_height-1)
		{
			int found_s[10] = {-1,-1,-1,-1,-1,-1, -1, -1, -1, -1};
			for ( int j = 0; j < m_height; j++)
			{
				int s = m_pWorkData1[j*m_width*3 + i*3 + 0];
				if ( s < 0 || s >= _MAX_NUMBER_NAHASHIM_) continue;
				for ( int ss = 0; ss < 10; ss++)
				{
					if ( found_s[ss] == -1)
					{
						found_s[ss] = s;
						break;
					}
					if (found_s[ss] == s)
						break;
				}
			}
			int s_found = -1;
			if (found_s[0] == -1) continue;
			if (found_s[1] == -1) s_found = found_s[0];
			if (s_found == -1)
			{
				for ( int ss = 0; ss < 10; ss++)
				{
					if (found_s[ss] == -1) break;
					int sss = found_s[ss];
					if (m_nahashimEdgesUpDown[sss][1] > 60 && m_nahashimEdgesUpDown[sss][0] < 60)
					{
						s_found = sss;
					}
				}
			}
			if (s_found == -1)
			{
				for ( int ss = 0; ss < 10; ss++)
				{
					if (found_s[ss] == -1) break;
					int sss = found_s[ss];
					if (m_nahashimEdgesUpDown[sss][1] > 40 && m_nahashimEdgesUpDown[sss][0] < 80)
					{
						s_found = sss;
					}
				}
			}
			if (s_found == -1)
			{
				for ( int ss = 0; ss < 10; ss++)
				{
					if (found_s[ss] == -1) break;
					int sss = found_s[ss];
					if (m_nahashimEdgesUpDown[sss][1] > 20 && m_nahashimEdgesUpDown[sss][0] < 100)
					{
						s_found = sss;
					}
				}
			}
			if (s_found == -1)
				continue;
			for ( int j = 0; j < m_height; j++)
			{
				int s = m_pWorkData1[j*m_width*3 + i*3 + 0];
				if ( s < 0 || s >= _MAX_NUMBER_NAHASHIM_) continue;
				if ( s_found != s) continue;
				m_pWorkData1[j*m_width*3 + i*3 + 0] = -1000;
			}
			continue;
		}
		for ( int j = j1; j < j2; j++)
		{
			if ( m_pWorkData6 [j*m_width*3 + i*3 + 0] == 127) continue;
			if (m_pWorkData1[j*m_width*3 + i*3 + 1] < 0) continue;
			m_pWorkData1[j*m_width*3 + i*3 + 0] = -1000;
			/*if (m_pWorkData1[j*m_width*3+i*3+1] >= 0)
			{
				int tableSnake = m_pWorkData1[j*m_width*3+i*3+1];
				for ( int s = 0; s < 100; s++)
				{
					if (snakesChoosen[s] == -1)
					{
						snakesChoosen[s] = tableSnake;
						break;
					}
					if (snakesChoosen[s] == tableSnake)
					{
						break;
					}
				}

			}*/
		}
	}
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
	
			if (m_pWorkData1[j*m_width*3 + i*3 + 0] != -1000)
			{
				m_pWorkData1[j*m_width*3 + i*3 + 0] = -1;
			}
			else
			{
				m_pWorkData1[j*m_width*3 + i*3 + 0] = 1;
			}
		}
	}
	/*for (int s = 0; s < 100; s++)
	{
		if (snakesChoosen[s] == -1) break;
		int snakeChoosen = snakesChoosen[s];
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (m_pWorkData1[j*m_width*3 + i*3 + 1] == snakeChoosen)
				{
					m_pWorkData1[j*m_width*3 + i*3 + 0] = snakeChoosen;
				}
			}
		}
	}*/
}
void COneLine_4::ChooseLineFromNahashim()
{


	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 + 0] = -1;
		}
	}

	// copy all snakes presented in the middle pass to the array m_pWorkData1[j*m_width*3 + i*3 + 0]
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = _UP_MIDDLE_BOUND_; j < _DOWN_MIDDLE_BOUND_; j++)
		{
			if (m_pWorkData1[j*m_width*3 + i*3 + 1] >= 0)
			{
				int new_n = m_pWorkData1[j*m_width*3 + i*3 + 1];
				if (m_pWorkData1[j*m_width*3 + i*3 + 0] == new_n) continue;
				for ( int jj = 0; jj < m_height; jj++)
				{
					if (m_pWorkData1[jj*m_width*3 + i*3 + 1] == new_n) 
					{
						m_pWorkData1[jj*m_width*3 + i*3 + 0] = new_n;
					}
				}
			}
		}
	}

	for ( int i = 0; i < m_width; i++)
	{
		int tableNahashimInMiddle[_SIZE_MIDDLE_];
		int numberPixelsInMiddle[_SIZE_MIDDLE_ ];
		int numberTableNahashimInMiddle = 0;
		for ( int n = 0; n < _SIZE_MIDDLE_; n++)
		{
			tableNahashimInMiddle[n] = 0;
			numberPixelsInMiddle[n] = 0;
		}
		// count number snakes presents in the middle of current column
		for ( int j = _UP_MIDDLE_BOUND_; j < _DOWN_MIDDLE_BOUND_; j++)
		{
			if (m_pWorkData1[j*m_width*3 + i*3 + 0] != -1)
			{
				int n_newNahash = m_pWorkData1[j*m_width*3 + i*3 + 0];
				bool newNahash =  true;
				for (int n = 0; n < numberTableNahashimInMiddle; n++)
				{
					if (tableNahashimInMiddle[n] == n_newNahash)
					{
						newNahash = false;
						break;
					}
				}
				if (newNahash)
				{
					tableNahashimInMiddle[numberTableNahashimInMiddle++] = n_newNahash;
				}
			}
		} 
		// if only one snake is presents in the middle of current column - it is ok. We can continue;
		if (numberTableNahashimInMiddle <= 1) continue;
		
		// if more than one snake presents in the middle pass of the column - we should keep only the snake that 
		// has the most presentation on the middle


		// then we count number of pixels in the middle pass occupyied by the each snake in the middle pass
		for (int n = 0; n < numberTableNahashimInMiddle; n++)
		{
			int nahash = tableNahashimInMiddle[n];
			for ( int ii = 0; ii < m_width; ii++)
			{
				for ( int jj = _UP_MIDDLE_BOUND_; jj < _DOWN_MIDDLE_BOUND_; jj++)
				{
					if (m_pWorkData1[jj*m_width*3 + ii*3 + 0] == nahash)
						numberPixelsInMiddle[n]++;
				}
			}
		}
		// find snake that ocuupies the most number of pixels in the snake
		int maxPixelsNahash = -1;
		int maxPixels = 0;
		for (int n = 0; n < numberTableNahashimInMiddle; n++)
		{
			if (numberPixelsInMiddle[n] > maxPixels)
			{
				maxPixels = numberPixelsInMiddle[n];
				maxPixelsNahash = n;
			}
		}
		// remove other snakes from the field
		for (int n = 0; n < numberTableNahashimInMiddle; n++)
		{
			if (maxPixelsNahash == n) continue;
			int nahash = tableNahashimInMiddle[n];
			for ( int ii = 0; ii < m_width; ii++)
			{
				//for ( int jj = _UP_MIDDLE_BOUND_; jj < _DOWN_MIDDLE_BOUND_; jj++)
				for ( int jj = 0; jj < m_height; jj++)
				{
					if (m_pWorkData1[jj*m_width*3 + ii*3 + 0] == nahash)
						m_pWorkData1[jj*m_width*3 + ii*3 + 0] = -1;
				}
			}
		}
	}
}

void COneLine_4::RemoveTableLinesSnakes()
{
	// remove snakes having max height (of each column ) < 12 pixels - this is horizontal line.
	// and shakes having height > 70 pixels and max width (of each column ) < 12 pixels
	
	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		m_snakePresents[n] = false;
		m_snakeMaxHeightByEachColumn[n] = 0;
		m_snakeMaxWidthByEachRaw[n] = 0;
		m_snakeHeightByAllColumns[n] = 0;
	}

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int n = m_pWorkData1[j*m_width*3 + i*3 + 0];
			if ( n >= 0)
				m_snakePresents[n] = true;
		}
	}


	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		if ( !m_snakePresents[n]) continue;

		int minYInAllColumns = m_height;
		int maxYInAllColumns = 0;
		for ( int i = 0; i < m_width; i++)
		{
			int minYInEachColumn = m_height;
			int maxYInEachColumn = 0;
			for ( int j = 0; j < m_height; j++)
			{
				if (m_pWorkData1[j*m_width*3 + i*3 + 0] != n) continue;
				
				if (j < minYInAllColumns) 
					minYInAllColumns = j;
				if (j > maxYInAllColumns) 
					maxYInAllColumns = j;

		
				if (j < minYInEachColumn) 
					minYInEachColumn = j;
				if (j > maxYInEachColumn) 
					maxYInEachColumn= j;
			}
			int h = maxYInEachColumn - minYInEachColumn;
			if ( h > m_snakeMaxHeightByEachColumn[n])
				m_snakeMaxHeightByEachColumn[n] = h;
		}
		m_snakeHeightByAllColumns[n] =  maxYInAllColumns - minYInAllColumns;
	
	
	
		for ( int j = 0; j < m_height; j++)
		{
			
			int minXInEachRaw= m_width;
			int maxXInEachRaw = 0;

			for ( int i = 0; i < m_width; i++)
			{
				if (m_pWorkData1[j*m_width*3 + i*3 + 0] != n) continue;
				if (i < minXInEachRaw) 
					minXInEachRaw= i;
				if (i > maxXInEachRaw) 
					maxXInEachRaw = i;
			}
			int w = maxXInEachRaw - minXInEachRaw;
			if ( w > m_snakeMaxWidthByEachRaw[n])
			{
				m_snakeMaxWidthByEachRaw[n] = w;
			}
		}
	
		// eliminate horizontal snakes (max height by each column < 15)
		if (m_snakeMaxHeightByEachColumn[n] < 15)
		{
			for ( int i = 0; i < m_width; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					if (m_pWorkData1[j*m_width*3 + i*3 + 0] != n) continue;
					m_pWorkData1[j*m_width*3 + i*3 + 0] = -1;
				}
			}
		}

		//eliminate vertical snakes ( max Height By All Columns > 70 && max width by each raw < 15)
		if (m_snakeHeightByAllColumns[n] > 90 && m_snakeMaxWidthByEachRaw[n] < 15)
		{
			for ( int i = 0; i < m_width; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					if (m_pWorkData1[j*m_width*3 + i*3 + 0] != n) continue;
					m_pWorkData1[j*m_width*3 + i*3 + 0] = -1;
				}
			}
		}
	}





}
void  COneLine_4::RemoveHorizontalLines()
{
	int begin_x = m_width-1;
	int end_x = 0;
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			if (m_pDataIn[j*m_width+i] == TextColor/* || m_pDataIn[j*m_width+i] == 127*/)
			{
				if ( i < begin_x) begin_x = i;
				if (i > end_x) end_x = i;
			}
		}
	}
	begin_x -= 5;
	if (begin_x < 0) begin_x = 0;
	for ( int j = 10; j < m_height; j++)
	{
		int minWidth = 100;
			
		for ( int i = begin_x; i < m_width; i+=minWidth)
		{
			int countText = 0;
			int countBG = 0;
			int maxCountText = 0;
			minWidth = 0;
			int minMaavarim = 12;
			int nMaavarimInRaw[128];
			for ( int jj = 0; jj < m_height; jj++)
				nMaavarimInRaw[jj] = 0;
			bool found = false;
			for ( int ii = i; ii < m_width; ii++)
			{
				minWidth++;
				for ( int jj = 0; jj < m_height; jj++)
				{
					if ((m_pDataIn[jj*m_width+ii] == TextColor/* || m_pDataIn[jj*m_width+ii] == 127*/) && m_pDataIn[jj*m_width+ii+1] == 0)
					{
						nMaavarimInRaw[jj]++;
						if (minMaavarim < nMaavarimInRaw[jj])
						{
							found = true;
							break;
							
						}
					}
				}
				if ( found) 
					break;
			}
			
			for ( int ii = i; ii < i + minWidth; ii++)
			{
				bool isTextPixel = false;
				for ( int jj = j - 10; jj < j; jj++)
				{
					if (m_pDataIn[jj*m_width+ii] == TextColor/* || m_pDataIn[jj*m_width+ii] == 127*/)
					{
						isTextPixel = true;
						break;
					}
				}
				if (isTextPixel)
				{
					countText++;
					if (countText > maxCountText)
					{
						maxCountText = countText;
					}
					countBG = 0;
				}
				else
				{
					countBG++;
					if (countBG > 2)
					{
						countText = 0;
						countBG = 0;
					}
				}
			}
			if (maxCountText > minWidth - 20)
			{ 
				/*
				int numberMaavarim = 0;
				for ( int ii = i; ii < i + minWidth-10; ii++)
				{
					for ( int jj = j - 10; jj < j; jj++)
					{
						int blackWhite = 0;
						if (m_pDataIn[jj*m_width+ii] == TextColor || m_pDataIn[jj*m_width+ii] == 127)
						{
							blackWhite++;
						}
						if (m_pDataIn[jj*m_width+ii] == 255-TextColor )
						{
							blackWhite--;
						}
						if (m_pDataIn[jj*m_width+ii+1] == TextColor || m_pDataIn[jj*m_width+ii+1] == 127)
						{
							blackWhite++;
						}
						if (m_pDataIn[jj*m_width+ii+1] == 255-TextColor )
						{
							blackWhite--;
						}
						if (blackWhite == 0)
							numberMaavarim++;
					}
				}
				if (numberMaavarim > 22)
					continue;
					*/
				int numberMaavarimUp = 0;
				int numberMaavarimDown = 0;
				for ( int ii = i; ii < i + minWidth-1; ii++)
				{
					for ( int jj = j+2 ; jj < m_height; jj++)
					{
						if (m_pDataIn[jj*m_width+ii] != m_pDataIn[jj*m_width+ii+1])
							numberMaavarimDown++;
					}
					for ( int jj = j-12 ; jj >= 0; jj--)
					{
						if (m_pDataIn[jj*m_width+ii] != m_pDataIn[jj*m_width+ii+1])
							numberMaavarimUp++;
					}
				}
				for ( int ii = i; ii < i + minWidth; ii++)
				{
					int step = 0;
					if (numberMaavarimUp > numberMaavarimDown)
					{
						for ( int jj = j ; jj >= j-10; jj--)
						{
							if (m_pDataIn[jj*m_width+ii] == 255 - TextColor && step == 1)
								break;
							if (m_pDataIn[jj*m_width+ii] == TextColor /*|| m_pDataIn[jj*m_width+ii] == 127*/)
							{
								step = 1;
								m_pDataOut[jj*3*m_width+3*ii] = 127;
							}
						}
					}
					else
					{
						for ( int jj = j-10 ; jj <= j; jj++)
						{
							if (m_pDataIn[jj*m_width+ii] == 255 - TextColor && step == 1)
								break;
							if (m_pDataIn[jj*m_width+ii] == TextColor || m_pDataIn[jj*m_width+ii] == 127)
							{
								step = 1;
								m_pDataOut[jj*3*m_width+3*ii] = 127;
							}
						}
					}
				}
			}
		}	
		/*
		int i = end_x;
		if (i < 0) i = 0;
		int countText = 0;
		int countBG = 0;
		int maxCountText = 0;
		for ( int ii = i; ii < i + 200; ii++)
		{
			bool isTextPixel = false;
			for ( int jj = j - 10; jj < j; jj++)
			{
				if (m_pDataIn[jj*m_width+ii] == TextColor || m_pDataIn[jj*m_width+ii] == 127)
				{
					isTextPixel = true;
					break;
				}
			}
			if (isTextPixel)
			{
				countText++;
				if (countText > maxCountText)
				{
					maxCountText = countText;
				}
				countBG = 0;
			}
			else
			{
				countBG++;
				if (countBG > 2)
				{
					countText = 0;
					countBG = 0;
				}
			}
		}
		if (maxCountText > 180)
		{ 
			int numberMaavarimUp = 0;
			int numberMaavarimDown = 0;
			for ( int ii = i; ii < i + 199; ii++)
			{
				for ( int jj = j+2 ; jj < m_height; jj++)
				{
					if (m_pDataIn[jj*m_width+ii] != m_pDataIn[jj*m_width+ii+1])
						numberMaavarimDown++;
				}
				for ( int jj = j-12 ; jj >= 0; jj--)
				{
					if (m_pDataIn[jj*m_width+ii] != m_pDataIn[jj*m_width+ii+1])
						numberMaavarimUp++;
				}
			}
			for ( int ii = i; ii < i + 200; ii++)
			{
				int step = 0;
				if (numberMaavarimUp > numberMaavarimDown)
				{
					for ( int jj = j ; jj >= j-10; jj--)
					{
						if (m_pDataIn[jj*m_width+ii] == 255 - TextColor && step == 1)
							break;
						if (m_pDataIn[jj*m_width+ii] == TextColor || m_pDataIn[jj*m_width+ii] == 127)
						{
							step = 1;
							m_pDataIn[jj*m_width+ii] = 127;
						}
					}
				}
				else
				{
					for ( int jj = j-10 ; jj <= j; jj++)
					{
						if (m_pDataIn[jj*m_width+ii] == 255 - TextColor && step == 1)
							break;
						if (m_pDataIn[jj*m_width+ii] == TextColor || m_pDataIn[jj*m_width+ii] == 127)
						{
							step = 1;
							m_pDataIn[jj*m_width+ii] = 127;
						}
					}
				}
			}
		}
		*/
	}
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pDataOut[j*m_width*3 + i * 3] == 127)
			{
				m_pDataIn[j*m_width+i] = 255 - TextColor;
				m_pDataOut[j*m_width*3 + i * 3] == 255 - TextColor;
			}
			
		}
	}

}

void  COneLine_4::RemoveTableHorizontalLines()
{
	
	for ( int j = 10; j < m_height; j++)
	{
		for ( int i = 0; i < m_width-200; i+=50)
		{
			int countText = 0;
			int countBG = 0;
			int maxCountText = 0;
			for ( int ii = i; ii < i + 200; ii++)
			{
				bool isTextPixel = false;
				for ( int jj = j - 10; jj < j; jj++)
				{
					if (m_pDataIn[jj*m_width+ii] == TextColor || m_pDataIn[jj*m_width+ii] == 127)
					{
						isTextPixel = true;
						break;
					}
				}
				if (isTextPixel)
				{
					countText++;
					if (countText > maxCountText)
					{
						maxCountText = countText;
					}
					countBG = 0;
				}
				else
				{
					countBG++;
					if (countBG > 2)
					{
						countText = 0;
						countBG = 0;
					}
				}
			}
			if (maxCountText > 100)
			{ 
				for ( int jj = j - 5; jj < j; jj++)
				{
					for ( int ii = i; ii < i + 100; ii++)
					{
						if (m_pDataIn[jj*m_width+ii] == TextColor)
						{
							m_pDataIn[jj*m_width+ii] = 127;
							m_pWorkData6[jj*m_width*3 + ii*3 + 0] = 127;
						}
					}
				}
			}
		}
	}
	
	int iTable = 1;
	for ( int i = 10; i < m_width-10; i+= 10)
	{
		int i1 = i - 10;
		int i2 = i+10;
		for ( int j = 5; j < m_height-10; j+=5)		
		{
			int j1 = j -5; 
			int j2 = j+5;

			int currentITable = 0;
			for ( int ii = i1; ii < i2; ii++)
			{
				for ( int jj = j1; jj < j2+5; jj++)
				{
					if ( m_pWorkData6[jj*m_width*3 + ii*3 + 0] == 127)
					{
						if ( m_pWorkData6[jj*m_width*3 + ii*3 + 1] > 0 )
						{
							currentITable = m_pWorkData6[jj*m_width*3 + ii*3 + 1];
							break;
						}
					}
				}
				if (currentITable > 0) break;
			}
			for ( int ii = i1; ii < i2; ii++)
			{
				for ( int jj = j1; jj < j2; jj++)
				{
					if ( m_pWorkData6[jj*m_width*3 + ii*3 + 0] == 127)
					{
						if (currentITable == 0) 
							currentITable  = iTable++;
						m_pWorkData6[jj*m_width*3 + ii*3 + 1] = currentITable;
					}
				}
			}
		}
	}
}
void COneLine_4::RemoveTableVerticalLines()
{
	for ( int i = 18; i < m_width; i++)
	{
		int countText = 0;
		int countBG = 0;
		int maxCountText = 0;
		for ( int j = 0; j < m_height; j++)
		{
			bool isTextPixel = false;
			for ( int ii = i - 18; ii < i-2; ii++)
			{
				if (m_pDataIn[j*m_width+ii] == TextColor || m_pDataIn[j*m_width+ii] == 127)
				{
					isTextPixel = true;
					break;
				}
			}
			if (isTextPixel)
			{
				countText++;
				if (countText > maxCountText)
				{
					maxCountText = countText;
				}
				countBG = 0;
			}
			else
			{
				countBG++;
				if (countBG > 0)
				{
					countText = 0;
					countBG = 0;
				}
			}
		}
		int limit = 70;
		if (isAsialanguage(m_language ))
			limit = 90;
		if (maxCountText > limit)
		{
			for ( int ii = i - 18; ii < i; ii++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					if (m_pDataIn[j*m_width+ii] == TextColor)
					{
						m_pDataIn[j*m_width+ii] = 127;
					}
					if (m_pDataIn[j*m_width+ii] == 127)
					{
						m_pWorkData5[j*m_width+ii] = 127;
					}

				}
			}
		}
	}
	
}
/*
void COneLine_4::BuildNahash1()
{


	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 + 1] = 0;
			m_pWorkData1[j*m_width*3 + i*3 + 0] = 0;
			m_pWorkData1[j*m_width*3 + i*3 + 2] = 0;
		}
	}
	
	int i_begin = 2;
	int j_begin = 5;

	while (i_begin < m_width-10)
	{
		bool found = false;
		for ( ;i_begin < m_width; i_begin++)
		{
			for ( j_begin = 5; j_begin < m_height; j_begin++)
			{
				if ((m_pDataIn[j_begin*m_width + i_begin] == TextColor) && (m_pWorkData1[(j_begin)*m_width*3 + (i_begin)*3 + 2] == 0)) 
				{
					found = true;
					break;
				}
			}
			if (found) break;
		}
		if ( i_begin >= m_width - 10) break;

		int i_end = i_begin;
		int j_end = j_begin;
		int count = 1;
		m_pWorkData1[(j_begin)*m_width*3 + (i_begin)*3 + 2] = 1;
		while (count > 0)
		{
			count = 0;
			for ( int i = i_begin; i <= i_end; i++)
			{
				for ( int j = j_begin; j <= j_end; j++)
				{
					//if (m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] == 2) continue;
					if (m_pDataIn[(j)*m_width + i] != TextColor) continue;
					if (m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] != 1) continue;
									
					if ( i < 2) continue;
					if ( i >= m_width - 2) continue;
					if ( j < 5) continue;
					if ( j >= m_height - 2) continue;
					for ( int ii = i-1; ii <= i+1; ii++)
					{
						for ( int jj = j-1; jj <= j+1; jj++)
						{
							if ( ii == i && jj == j) continue;
							if (m_pDataIn[(jj)*m_width + ii] == TextColor) 
							{
								if (m_pWorkData1[(jj)*m_width*3 + (ii)*3 + 2] == 0)
								{
									m_pWorkData1[(jj)*m_width*3 + (ii)*3 + 2] = 1;
									count++;
									if (i_begin > ii) i_begin = ii;
									if ( i_end < ii) i_end = ii;
									if (j_begin > jj) j_begin = jj;
									if ( j_end < jj) j_end = jj;
								}
							}
						}
					}
					m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] = 2;
				}
			}
		}
		for ( int i = i_begin; i <= i_end ; i++)
		{
			for ( int j = j_begin; j <= j_end; j++)
			{
				m_pWorkData1[j*m_width*3 + i*3 + 1] = 100;
			}
		}
		//i_begin = i_end;
		j_begin = j_end;

	}


}
*/

void COneLine_4::CalculateLetters()
{


	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 + 1] = 0;
			m_pWorkData1[j*m_width*3 + i*3 + 0] = 0;
			m_pWorkData1[j*m_width*3 + i*3 + 2] = 0;

			m_pWorkData2[j*m_width*3 + i*3 + 1] = 0;
			m_pWorkData2[j*m_width*3 + i*3 + 0] = 0;
			m_pWorkData2[j*m_width*3 + i*3 + 2] = 0;
		}
	}
	
	int i_begin = m_begin_x-1;
	int j_begin = 5;

	int i_letter = 0;

	while (i_begin < m_end_x)
	{
		bool found = false;
		for ( ;i_begin < m_end_x+1; i_begin++)
		{
			for ( j_begin = m_begin_y; j_begin < m_end_y+1; j_begin++)
			{
				if ((m_pDataIn[j_begin*m_width + i_begin] == TextColor) && (m_pWorkData1[(j_begin)*m_width*3 + (i_begin)*3 + 2] == 0)) 
				{
					found = true;
					break;
				}
			}
			if (found) break;
		}
		if ( i_begin >= m_end_x) break;

		int i_end = i_begin;
		int j_end = j_begin;
		int count = 1;
		m_pWorkData1[(j_begin)*m_width*3 + (i_begin)*3 + 2] = 1;
		while (count > 0)
		{
			count = 0;
			for ( int i = i_begin; i <= i_end; i++)
			{
				for ( int j = j_begin; j <= j_end; j++)
				{
					//if (m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] == 2) continue;
					if (m_pDataIn[(j)*m_width + i] != TextColor) continue;
					if (m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] != 1) continue;
															
					if ( i < 2) continue;
					if ( i >= m_width - 2) continue;
					if ( j < 5) continue;
					if ( j >= m_height - 2) continue;
					for ( int ii = i-1; ii <= i+1; ii++)
					{
						for ( int jj = j-1; jj <= j+1; jj++)
						{
							if ( ii == i && jj == j) continue;
							if (m_pDataIn[(jj)*m_width + ii] == TextColor) 
							{
								if (m_pWorkData1[(jj)*m_width*3 + (ii)*3 + 2] == 0)
								{
									m_pWorkData1[(jj)*m_width*3 + (ii)*3 + 2] = 1;
									m_pWorkData2[(jj)*m_width*3 + (ii)*3 + 2] = 1;
									count++;
									if (i_begin > ii) i_begin = ii;
									if ( i_end < ii) i_end = ii;
									if (j_begin > jj) j_begin = jj;
									if ( j_end < jj) j_end = jj;
								}
							}
						}
					}
					m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] = 2;
				}
			}
		}
		/*
		for ( int i = i_begin; i <= i_end ; i++)
		{
			for ( int j = j_begin; j <= j_end; j++)
			{
				m_pWorkData1[j*m_width*3 + i*3 + 1] = 100;
			}
		}*/
		int w = i_end - i_begin;
		int h = j_end - j_begin;
		if (w*h <= 4) continue;



	if (i_end - i_begin > 400)
	{
		int numberChanges = 0;
		
		for ( int  i = i_begin; i <= i_end; i++)
		{
			for ( int j = j_begin-1; j <= j_end+1; j++)
			{
				if ( j < 0) continue;
				if (j >= m_height-2) continue;
				if ( m_pDataIn[(j+1)*m_width+i] != m_pDataIn[j*m_width+i])
				{
					numberChanges++;
				}
			}
		}
		int w = i_end - i_begin;
		/*if ( numberChanges <= w*2+5)
		{
			for ( int  i = i_begin; i <= i_end; i++)
			{
				for ( int j = j_begin; j <= j_end; j++)
				{
					if (m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] == 2)
					{
						m_pDataIn[j*m_width+i] = 255 - TextColor;
						m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] = 0;
					}
					
				}
			}
			continue;
		}*/ // DINA garbage at the end
	}




		int numberLetters = 0;
		//if (!isAsialanguage(m_language ))
			numberLetters = CheckConnectedLetters(i_begin, i_end, j_begin, j_end);

		if ( numberLetters > 1)
		{
			int w = (i_end - i_begin) / numberLetters;
			for ( int ll = 0; ll < numberLetters; ll++)
			{
				int i1 = i_begin + ll*w;
				int i2 = i1 + w;
				int j1 = m_height;
				int j2 = 0;
				for ( int j = j_begin; j < j_end; j++)
				{
					for ( int i = i1; i < i2; i++)
					{
						if ( m_pDataIn[j*m_width+i] == TextColor && m_pWorkData2[(j)*m_width*3 + (i)*3 + 2] == 1)
						{
							if ( j1 > j) j1 = j;
							if (j2 < j) j2 = j;
						}
					}
				}
				m_letterEdges[i_letter][0] = i1;
				m_letterEdges[i_letter][1] = j1;
				m_letterEdges[i_letter][2] = i2;
				m_letterEdges[i_letter][3] = j2;
				for ( int i = i1; i <= i2; i++)
				{
					if ( m_pDataIn[j1* m_width + i] == TextColor)
					{
						m_letterPointUp[i_letter][0] = i;
						m_letterPointUp[i_letter][1] = j1;
					}
					if ( m_pDataIn[j2* m_width + i] == TextColor)
					{
						m_letterPointDown[i_letter][0] = i;
						m_letterPointDown[i_letter][1] = j2;
					}
				}
				m_letterHeight[i_letter] = j2- j1+ 1;;
				
				if ( i_letter < _MAX_NUMBER_LETTERS_-1 )
					i_letter++;

			}
			for ( int iii = i_begin; iii <= i_end; iii++)
			{
				for ( int jjj = j_begin; jjj <= j_end; jjj++)
				{
					m_pWorkData2[(jjj)*m_width*3 + (iii)*3 + 2] = 0;
				}
			}

			continue;
		}


		m_letterEdges[i_letter][0] = i_begin;
		m_letterEdges[i_letter][1] = j_begin;
		m_letterEdges[i_letter][2] = i_end;
		m_letterEdges[i_letter][3] = j_end;
		for ( int i = i_begin; i <= i_end ; i++)
		{
			if ( m_pDataIn[j_begin * m_width + i] == TextColor)
			{
				m_letterPointUp[i_letter][0] = i;
				m_letterPointUp[i_letter][1] = j_begin;
			}
			if ( m_pDataIn[j_end * m_width + i] == TextColor)
			{
				m_letterPointDown[i_letter][0] = i;
				m_letterPointDown[i_letter][1] = j_end;
			}
		}
		m_letterHeight[i_letter] = j_end - j_begin + 1;;
		 
		if ( i_letter < _MAX_NUMBER_LETTERS_-1 )
			i_letter++;
		//i_begin = i_end;
		//j_begin = j_end;
		for ( int iii = i_begin; iii <= i_end; iii++)
		{
			for ( int jjj = j_begin; jjj <= j_end; jjj++)
			{
				m_pWorkData2[(jjj)*m_width*3 + (iii)*3 + 2] = 0;
			}
		}



	}
m_numberLetters = i_letter;
		
}

int COneLine_4::CheckConnectedLetters( int i1, int i2, int j1, int j2)
{
	int maxChanges = 0;
	for ( int j = j1; j < j2; j++)
	{
		int nChanges = 0;
		for ( int i = i1; i <= i2; i++)
		{
			if (m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width + i-1]) 
				nChanges++;
		}
		if (nChanges > maxChanges) maxChanges = nChanges;
	}

	int w = i2 - i1;
	if ( !w) w = 1;
	if (maxChanges < 6) return 1;
	int n = maxChanges / 4;
	if (isAsialanguage(m_language ))
		n = maxChanges / 6;
	if (n == 0) n = 1;
	//int oneChangeWidth = w / maxChanges;
	int oneLetterWidth = w / n;
	if (oneLetterWidth > 15 ) return n;
	if (w < 100) return 1;
	return (w/40);

}
void COneLine_4::UnifyLettersKorean()
{
	//UnifyLetters();
	//return;
	for ( int l = 0; l < m_numberLetters; l++)
	{
		for ( int l2 = 0; l2 < m_numberLetters; l2++)
		{
			if ( l == l2) continue;
			int i1 = m_letterEdges[l][0]; 
			int i2 = m_letterEdges[l][2]; 
			int j1 = m_letterEdges[l][1]; 
			int j2 = m_letterEdges[l][3]; 
			
			int ii1 = m_letterEdges[l2][0]; 
			int ii2 = m_letterEdges[l2][2]; 
			int jj1 = m_letterEdges[l2][1]; 
			int jj2 = m_letterEdges[l2][3];

			if (i1 == -1 || ii1 == -1) continue;

			if ( i1 > ii2 || i2 < ii1) continue;

			bool isUnufy = false;
			if (j1 - jj2 < 10 && j1 > jj2)
				isUnufy = true;
			if (jj1 - j2 < 10 && jj1 > j2)
				isUnufy = true;
			if (j1 > jj1 && j1 < jj2)
					isUnufy = true;
			if (jj1 > j1 && jj1 < j2)
					isUnufy = true;
			
			if (!isUnufy) continue;



			int jj_1 = jj1;
			int jj_2 = jj2;

			if ( jj_1 > m_letterEdges[l][1]) jj_1 = m_letterEdges[l][1];
			if ( jj_2 < m_letterEdges[l][3]) jj_2 = m_letterEdges[l][2];
			//if (jj_2 - jj_1 >= m_max_letters_height*3/2) continue;





			if ( ii1 < i1) m_letterEdges[l][0] = ii1;
			if ( jj1 < m_letterEdges[l][1]) m_letterEdges[l][1] = jj1;
			if ( ii2 > i2) m_letterEdges[l][2] = ii2;
			if ( jj2 > m_letterEdges[l][3]) m_letterEdges[l][3] = jj2;
			ii1 = m_letterEdges[l][0]; 
			ii2 = m_letterEdges[l][2]; 
			jj1 = m_letterEdges[l][1]; 
			jj2 = m_letterEdges[l][3]; 

			int h = m_letterEdges[l][3] - m_letterEdges[l][1];
			//if ( h < 8) continue;
				

			for ( int ii = ii1; ii <= ii2; ii++)
			{
				for ( int jj = jj1; jj < jj2; jj++)
				{
					m_pWorkData1[jj*m_width*3+ii*3+0] = l;
				}
			}
			m_letterEdges[l2][0] = -1; 
			m_letterEdges[l2][2] = -1; 
			m_letterEdges[l2][1] = -1; 
			m_letterEdges[l2][3] = -1; 
		}
	}
}
void COneLine_4::UnifyLetters()
{
	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		int width_letter = m_letterEdges[l][2] - m_letterEdges[l][0];

		if (width_letter > (m_end_x - m_begin_x)/2) continue;
		

		int i1 = m_letterEdges[l][0]; 
		int i2 = m_letterEdges[l][2]; 
		int j1 = m_letterEdges[l][1]; 
		int j2 = m_letterEdges[l][3]; 
		int j_begin = j1-8; 
		int j_end = j2 + 8;
		if (j_begin < 0) j_begin = 0;
		if (j_end >= m_height - 1) j_end = m_height - 1;
		int l2 = -1;

				
		for ( int i = i1; i < i2; i++)
		{
			for ( int j = j_begin; j <= j_end; j++)
			{
				//if ( j >= m_letterEdges[l][1] && j <= m_letterEdges[l][3]) continue;
				int l2 = m_pWorkData1[j*m_width*3+i*3+0];
				if (l2 == -1 || l2 == l)
					continue;
				if (m_pDataIn[j*m_width+i] != TextColor) continue;
				int ii1 = m_letterEdges[l2][0]; 
				int ii2 = m_letterEdges[l2][2]; 
				int jj1 = m_letterEdges[l2][1]; 
				int jj2 = m_letterEdges[l2][3]; 
					
				int jjj1, jjj2;
				if (j < m_letterEdges[l][1])
				{
					jjj1 =  m_letterEdges[l][1]; 
					jjj2 =  jjj1+2; 
				}
				else if (j > m_letterEdges[l][3])
				{
					jjj2 =  m_letterEdges[l][3]; 
					jjj1 =  jjj2-2; 
				}
				else continue;
				bool unify = false;
		
				for ( int ii = i -3; ii < i+3; ii++)
				{
					for ( int jj = jjj1; jj <= jjj2; jj++)
					{
						if (m_pWorkData1[jj*m_width*3+ii*3+0] == l && m_pDataIn[jj*m_width+ii] == TextColor) 
						{
							unify = true;
							break;
						}
						//if (m_pWorkData1[jj*m_width*3+ii*3+0] == l2 && m_pDataIn[jj*m_width+ii] == TextColor) exist_2 = true;
					}
					if (unify) break;
				}



				if (!unify) continue;
				

				int jj_1 = jj1;
				int jj_2 = jj2;

				if ( jj_1 > m_letterEdges[l][1]) jj_1 = m_letterEdges[l][1];
				if ( jj_2 < m_letterEdges[l][3]) jj_2 = m_letterEdges[l][2];
				/*if (m_language == APP_ENGINE_LNG_KOREAN)
				{
					if (jj_2 - jj_1 >= m_max_letters_height*3/2) continue;
				}
				else*/
				{
					if (jj_2 - jj_1 >= m_max_letters_height) continue;
				}
				





				if ( ii1 < i1) m_letterEdges[l][0] = ii1;
				if ( jj1 < m_letterEdges[l][1]) m_letterEdges[l][1] = jj1;
				if ( ii2 > i2) m_letterEdges[l][2] = ii2;
				if ( jj2 > m_letterEdges[l][3]) m_letterEdges[l][3] = jj2;
				ii1 = m_letterEdges[l][0]; 
				ii2 = m_letterEdges[l][2]; 
				jj1 = m_letterEdges[l][1]; 
				jj2 = m_letterEdges[l][3]; 

				int h = m_letterEdges[l][3] - m_letterEdges[l][1];
				//if ( h < 8) continue;
				

				for ( int ii = ii1; ii <= ii2; ii++)
				{
					for ( int jj = jj1; jj < jj2; jj++)
					{
						m_pWorkData1[jj*m_width*3+ii*3+0] = l;
					}
				}
				m_letterEdges[l2][0] = -1; 
				m_letterEdges[l2][2] = -1; 
				m_letterEdges[l2][1] = -1; 
				m_letterEdges[l2][3] = -1; 
			}
		}
	}
}
void COneLine_4::BuildNahashim()
{
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 + 0] = -1;
			m_pWorkData1[j*m_width*3 + i*3 + 1] = -1;
			m_pWorkData1[j*m_width*3 + i*3 + 2] = -1;

		}
	}

	int width_letters[_MAX_NUMBER_LETTERS_];
	int height_letters[_MAX_NUMBER_LETTERS_];
	int width_letters_1[_MAX_NUMBER_LETTERS_];
	int height_letters_1[_MAX_NUMBER_LETTERS_];
	for ( int l = 0; l < _MAX_NUMBER_LETTERS_; l++)
	{
		width_letters[l] = 0;
		height_letters[l] = 0;
		width_letters_1[l] = 0;
		height_letters_1[l] = 0;
	}
	
	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][3] > _DOWN_EDGE_ - 3) continue;
		if (m_letterEdges[l][1] < _UP_EDGE_ + 3) continue;
		int w = m_letterEdges[l][2] - m_letterEdges[l][0];
		width_letters[l] = w;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		height_letters[l] = h;
	}
	/*int ll = 0;
	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (width_letters_1[l] > 0 && height_letters_1[l] > 0)
		{
			width_letters[ll] = width_letters_1[l];
			height_letters[ll] = height_letters_1[l];
			ll++;
		}
	}
*/
	int numberLetters = m_numberLetters;
	for ( int l = 0; l < numberLetters-1; l++)
	{
		for ( int l1 = l; l1 < numberLetters; l1++)
		{
			if ( width_letters[l] > width_letters[l1])
			{
				int temp = width_letters[l];
				width_letters[l] = width_letters[l1];
				width_letters[l1] = temp;
			}
		}
	}
	for ( int l = 0; l < numberLetters-1; l++)
	{
		for ( int l1 = l; l1 < numberLetters; l1++)
		{
			if ( height_letters[l] > height_letters[l1])
			{
				int temp = height_letters[l];
				height_letters[l] = height_letters[l1];
				height_letters[l1] = temp;
			}
		}
	}

	int first_h = 0;
	for	( int l = 0; l < numberLetters-1; l++)
	{
		if (height_letters[l] > 0)
		{
			first_h = l;
			break;
		}
	}
	int n3_4 = ((numberLetters - first_h) * 3 / 4) + first_h;
	int n_2 = ((numberLetters - first_h) / 2) + first_h;
	int n5_6 = ((numberLetters - first_h) *5/6) + first_h;
	int n2_3 = ((numberLetters - first_h) *2/3) + first_h;

	m_max_letters_height = height_letters[n3_4] * 5/3;
	//Dina 060313 m_min_letters_height = height_letters[n_2] *4/ 5;
	m_min_letters_height = 4;
	if (height_letters[n3_4] < 25)
	{
		m_max_letters_height = height_letters[n5_6] * 2;
	}
	int min_letters_width =  width_letters[n3_4];
	if (min_letters_width < 30) min_letters_width = 30;

	m_max_search_width = 6* width_letters[n2_3];
 
	for ( int l = 0; l < m_numberLetters; l++)
	{
		int width_letter = m_letterEdges[l][2] - m_letterEdges[l][0];

		if (width_letter > (m_end_x - m_begin_x)/2) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		//if ( h < 8) continue;
		if ( h < m_min_letters_height) continue;
		if (!isAsialanguage(m_language ))
		{
			if ( h >= m_max_letters_height) continue;
		}
		
		for ( int i = m_letterEdges[l][0]; i < m_letterEdges[l][2]; i++)
		{
			for ( int j =  m_letterEdges[l][1]; j < m_letterEdges[l][3]; j++)
			{
				m_pWorkData1[j*m_width*3+i*3+0] = l;
			}
		}
	}


	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if ( !isAsialanguage(m_language ))
		{
			if ( h >= m_max_letters_height) continue;
		}
		if ( h < m_min_letters_height) continue;
		
		int i1 = m_letterEdges[l][0];
		int i2 = m_letterEdges[l][2];
		int j1 = m_letterEdges[l][1];
		int j2 = m_letterEdges[l][3];



		int l2 = -1;
		int ii1 = -1;
		int ii2 = -1;
		int jj1 = -1;
		int jj2 = -1;
		
		bool unify = false;

		for ( int i = i1; i <= i2; i++)
		{
			for ( int j = j1; j <= j2; j++)
			{
				
				l2 = m_pWorkData1[j*m_width*3+i*3+0];
				if ( l2 == l || l2 == -1) continue;
				int h2 = m_letterEdges[l2][3] - m_letterEdges[l2][1];
				if ( !isAsialanguage(m_language ))
				{
					if ( h2 >= m_max_letters_height) continue;
				}
				if ( h < m_min_letters_height) continue;
		
				ii1 = m_letterEdges[l2][0];
				ii2 = m_letterEdges[l2][2];
				jj1 = m_letterEdges[l2][1];
				jj2 = m_letterEdges[l2][3];
				int iii1 = (ii1 > i1) ? ii1 : i1;
				int iii2 = (ii2 < i2) ? ii2 : i2;
				int jjj1 = (jj1 > j1) ? jj1 : j1;
				int jjj2 = (jj2 < j2) ? jj2 : j2;
				/*int s = (i2 - i1)*(j2-j1);
				int ss = (ii2 - ii1)*(jj2 - jj1);
				int common_s = (iii2 - iii1) * ( jjj2 - jjj1);
				*/
				int h = j2 - j1;
				int hh = jj2 - jj1;
				int common_h = jjj2 - jjj1;
				int w = i2 - i1;
				int ww = ii2 - ii1;
				int common_w = iii2 - iii1;
				if ((common_h > h / 3 && common_h > hh/3) && ( common_w > w / 3 || common_w > ww / 3))
				{
					unify = true;
					break;
				}
			}
			if (unify) break;
		}
		if (!unify) continue;
		
		for ( int ii = ii1; ii <= ii2; ii++)
		{
			for ( int jj = jj1; jj <= jj2; jj++)
			{
				m_pWorkData1[jj*m_width*3+ii*3+0] = l;
			}
		}
		if ( ii1 < i1) m_letterEdges[l][0] = ii1;
		if ( ii2 > i2) m_letterEdges[l][2] = ii2;
		if ( jj1 < j1) m_letterEdges[l][1] = jj1;
		if ( jj2 > j2) m_letterEdges[l][3] = jj2;
		m_letterEdges[l2][0] = -1; 
		m_letterEdges[l2][2] = -1; 
		m_letterEdges[l2][1] = -1; 
		m_letterEdges[l2][3] = -1; 
	}










	if ( isAsialanguage(m_language ))
	{
		UnifyLettersKorean();
	}
	else
	{
		UnifyLetters();
	}	

	
	for (int l = 0; l < _MAX_NUMBER_LETTERS_; l++)
	{
		m_chainToRight[l][0] = -1;
		m_chainToRight[l][1] = -1;
		m_chainToRight[l][2] = -1;
		m_chainToLeft[l][0] = -1;
		m_chainToLeft[l][1] = -1;
		m_chainToLeft[l][2] = -1;
		m_nahashim[l] = -1;
		for ( int k = 0; k < 20; k++)
		{
			m_connectedLettersToRight[l][k][0] = -1;
			m_connectedLettersToRight[l][k][1] = 0;
			m_connectedLettersToLeft[l][k][0] = -1;
			m_connectedLettersToLeft[l][k][1] = 0;
		}
	}
	
	int j_connected[128];
	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if (  !isAsialanguage(m_language ))
		{
			if ( h >= m_max_letters_height) continue;
		}
		if ( h < m_min_letters_height) continue;
		
		for ( int j = 0; j < 128; j++)
			j_connected[j] = -1;

		int i_end = m_letterEdges[l][0]-1;
		int i_begin = i_end - m_max_search_width;
		if ( i_begin <= 0) i_begin = 0;

		for ( int j =  m_letterEdges[l][1]; j <= m_letterEdges[l][3]; j++)
		{
			for ( int i = i_end; i >= i_begin; i--)
			{
				int l2 = m_pWorkData1[j*m_width*3+i*3+0];
				if (l2 < 0 || l2 == l) continue;
				int h = m_letterEdges[l2][3] - m_letterEdges[l2][1];
				if (  !isAsialanguage(m_language ))
				{
					if ( h >= m_max_letters_height) continue;
				}	
				if ( h < m_min_letters_height) continue;
		
				j_connected[j] = l2;
				break;
				
			}
		}


		int ll = 0;
		for ( int j =  m_letterEdges[l][1]; j < m_letterEdges[l][3]; j++)
		{
			int l2 = j_connected[j];
			if (l2 == -1 || l2 == l) continue;
			int found = false;
			for ( int ll1 = 0; ll1 < ll; ll1++)
			{
				if ( m_connectedLettersToRight[l][ll1][0] == l2)
				{
					m_connectedLettersToRight[l][ll1][1]++;
					found = true;
					break;
				}
			}
			if (!found)
			{
				m_connectedLettersToRight[l][ll][0] = l2;
				m_connectedLettersToRight[l][ll][1] = 1;
				if ( ll < 19) 
				ll++;
			}
		}
		
	}
	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if (  !isAsialanguage(m_language ))
		{
			if ( h >= m_max_letters_height) continue;
		}
		if ( h < m_min_letters_height) continue;
		
		int ll1 = 0;
		for ( int ll = 0; ll < 20; ll++)
		{
			int l2 = m_connectedLettersToRight[l][ll][0];
			if (l2 == -1) break;


			ll1 = 0;
			while (m_connectedLettersToLeft[l2][ll1][0] >= 0 && ll1 < 19) ll1++;
			m_connectedLettersToLeft[l2][ll1][0] = l;
			m_connectedLettersToLeft[l2][ll1][1] = m_connectedLettersToRight[l][ll][1];
		}

	}


	for (int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if (  !isAsialanguage(m_language ))
		{
			if ( h >= m_max_letters_height) continue;
		}
		if ( h < m_min_letters_height) continue;
		int max_2 = 0;
		int minDist_1  = 10000;
		//int numberMax = 0;
		int max_ll_1 = 0;
		int max_1 = 0;
		//int numberMax = 0;
		int max_ll_2 = 0;
		int minDist_2 = 0;
		for ( int ll = 0; ll < 20; ll++)
		{
			if (m_connectedLettersToLeft[l][ll][0] < 0) break;
			int l2 = m_connectedLettersToLeft[l][ll][0];
			int d1 = abs(m_letterEdges[l][0] - m_letterEdges[l2][2]);
			int d2 = abs(m_letterEdges[l][2] - m_letterEdges[l2][0]);
			int dist = (d1 < d2) ? d1 : d2;
			int nc = m_connectedLettersToLeft[l][ll][1];
			if (nc > 2)
			{
				/*numberMax++;*/
				if (dist < minDist_1)
				{
					max_ll_1 = ll;
					minDist_1 = dist;
					max_1 = nc;
				}
				if (max_2 < nc)
				{
					max_2 = nc;
					max_ll_2 = ll;
					minDist_2 = dist;
				}
			}
		}
		//if ( numberMax == 1)
		{
			int distDiff = minDist_2 - minDist_1;
			if (distDiff > m_max_search_width/6)
				m_chainToLeft[l][0] = m_connectedLettersToLeft[l][max_ll_1][0];
			else
				m_chainToLeft[l][0] = m_connectedLettersToLeft[l][max_ll_2][0];
		}

	}


	for (int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if (  !isAsialanguage(m_language ))
		{
			if ( h >= m_max_letters_height) continue;
		}
		if ( h < m_min_letters_height) continue;
		/*		int max = 0;
		int numberMax = 0;
		int minDist = 10000;
		int max_ll = 0;
*/
		int max_2 = 0;
		int minDist_1  = 10000;
		//int numberMax = 0;
		int max_ll_1 = 0;
		int max_1 = 0;
		//int numberMax = 0;
		int max_ll_2 = 0;
		int minDist_2 = 0;
		for ( int ll = 0; ll < 20; ll++)
		{
			if (m_connectedLettersToRight[l][ll][0] < 0 ) break;
			int nc = m_connectedLettersToRight[l][ll][1];
			int l2 = m_connectedLettersToRight[l][ll][0];
			int d1 = abs(m_letterEdges[l][0] - m_letterEdges[l2][2]);
			int d2 = abs(m_letterEdges[l][2] - m_letterEdges[l2][0]);
			int dist = (d1 < d2) ? d1 : d2;
/*			if (nc > 8)
			{
				
				numberMax++;
				if (max < nc)
				{
					max = nc;
					max_ll = ll;
				}
				numberMax++;
				if (dist < minDist)
				{
					max_ll = ll;
					minDist = dist;
				}

			}
*/
			if (nc > 2)
			{
				/*numberMax++;*/
				if (dist < minDist_1)
				{
					max_ll_1 = ll;
					minDist_1 = dist;
					minDist_1 = dist;
					max_1 = nc;
				}
				if (max_2 < nc)
				{
					max_2 = nc;
					max_ll_2 = ll;
					minDist_2 = dist;
				}
			}

		}
		//if ( numberMax == 1)
		{
//			m_chainToRight[l][0] = m_connectedLettersToRight[l][max_ll][0];
			int distDiff = minDist_2 - minDist_1;
			if (distDiff > m_max_search_width/6)
				m_chainToRight[l][0] = m_connectedLettersToRight[l][max_ll_1][0];
			else
				m_chainToRight[l][0] = m_connectedLettersToRight[l][max_ll_2][0];

		}
		/*else if ( numberMax > 1)
		{
			if 

		}*/

	}


	int doubleLetters[20];
	for ( int l = 0; l < m_numberLetters-1; l++)
	{
		if (m_chainToRight[l][0] == -1) continue;
		for ( int l1 = l+1; l1 < m_numberLetters; l1++)
		{
			if (m_chainToRight[l1][0] == -1) continue;
			if ( m_chainToRight[l][0] == m_chainToRight[l1][0])
			{
				int l2 = m_chainToRight[l][0];
				int l3 = m_chainToLeft[l2][0];
			
				int h_l = m_letterEdges[l][3] - m_letterEdges[l][1];
				int h_l1 = m_letterEdges[l1][3] - m_letterEdges[l1][1];
				int h_l_0 = m_letterEdges[m_chainToRight[l][0]][3] - m_letterEdges[m_chainToRight[l][0]][1];
				int h_l1_0 = m_letterEdges[m_chainToRight[l1][0]][3] - m_letterEdges[m_chainToRight[l1][0]][1];
				int h_max = h_l;
				if (h_max < h_l1) h_max = h_l1;
				if (h_max < h_l_0) h_max = h_l_0;
				if (h_max < h_l1_0) h_max = h_l1_0;


				int j1 = m_letterEdges[l][1];
				if ( j1 <  m_letterEdges[l1][1]) j1 = m_letterEdges[l1][1];
				if ( j1 <  m_letterEdges[m_chainToRight[l][0]][1]) j1 = m_letterEdges[m_chainToRight[l][0]][1];
				if ( j1 <  m_letterEdges[m_chainToRight[l1][0]][1]) j1 = m_letterEdges[m_chainToRight[l1][0]][1];

				int j2 = m_letterEdges[l][3];
				if ( j2 >  m_letterEdges[l1][3]) j1 = m_letterEdges[l1][3];
				if ( j2 >  m_letterEdges[m_chainToRight[l][0]][3]) j1 = m_letterEdges[m_chainToRight[l][0]][3];
				if ( j2 >  m_letterEdges[m_chainToRight[l1][0]][3]) j1 = m_letterEdges[m_chainToRight[l1][0]][3];

				int h_common = j2 - j1;
				if ( h_common > h_max / 3)
				{
					m_chainToRight[l1][0] = l;
				}
				else
				{
					if (l3 == l) m_chainToRight[l1][0] = -1;
					if ( l3 == l1) m_chainToRight[l][0] = -1;
				}
			}
		}

	}
	for ( int l = 0; l < m_numberLetters; l++)
		m_chainToRight[l][1] = -1;

	for ( int l = 0; l < m_numberLetters; l++)
	{
		int l2 = m_chainToRight[l][0];
		if (l2 == -1) continue;
		m_chainToRight[l2][1] = l;
	}
	
	int nahash = 0;

	int lll = -1;
	while (true)
	{
		bool found = false;
		int first_l = -1;
		while ( lll < m_numberLetters-1)
		{
			lll++;
			if (m_chainToRight[lll][1] == -1)
			{
				if (m_letterEdges[lll][0] == -1) continue;
				int h = m_letterEdges[lll][3] - m_letterEdges[lll][1];
				if ( !isAsialanguage(m_language ))
				{
					if ( h >= m_max_letters_height) continue;
				}
				first_l = lll;
				found = true;
				break;
			}
			
		}
		if (!found) break;
		nahash++;
		int ll = first_l;
		m_nahashim[ll] = nahash;
		while (m_chainToRight[ll][0] != -1)
		{
			ll = m_chainToRight[ll][0];
			m_nahashim[ll] = nahash;
			
		} 

	}


	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		if (m_nahashim[l] == -1) continue;
		int width_letter = m_letterEdges[l][2] - m_letterEdges[l][0];

		if (width_letter > (m_end_x - m_begin_x)/2) continue;
		
		for ( int i = m_letterEdges[l][0]; i < m_letterEdges[l][2]; i++)
		{
			for ( int j =  m_letterEdges[l][1]; j < m_letterEdges[l][3]; j++)
			{
				m_pWorkData1[j*m_width*3+i*3+1] = m_nahashim[l];
			}
		}
	
	}

	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		int width_letter = m_letterEdges[l][2] - m_letterEdges[l][0];

		if (width_letter > (m_end_x - m_begin_x)/2) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if ( h < m_max_letters_height) continue;
		for ( int i = m_letterEdges[l][0]; i < m_letterEdges[l][2]; i++)
		{
			for ( int j =  m_letterEdges[l][1]; j < m_letterEdges[l][3]; j++)
			{
				if (m_pWorkData1[j*m_width*3+i*3+0] != -1) continue;
				m_pWorkData1[j*m_width*3+i*3+0] = l;
				m_pWorkData1[j*m_width*3+i*3+1] = -2;
			}
		}
	
	}

	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] < 0) continue;
		int width_letter = m_letterEdges[l][2] - m_letterEdges[l][0];

		if (width_letter > (m_end_x - m_begin_x)/2) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if ( h >= 8) continue;
		//if ( h >= 8) continue;
		
		for ( int i = m_letterEdges[l][0]; i < m_letterEdges[l][2]; i++)
		{
			for ( int j =  m_letterEdges[l][1]; j < m_letterEdges[l][3]; j++)
			{
				if (m_pWorkData1[j*m_width*3+i*3+0] != -1) continue;
				m_pWorkData1[j*m_width*3+i*3+0] = l;
				m_pWorkData1[j*m_width*3+i*3+1] = -5;
			}
		}
	}

	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] < 0) continue;
		int width_letter = m_letterEdges[l][2] - m_letterEdges[l][0];

		if (width_letter > (m_end_x - m_begin_x)/2) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if ( h < 8) continue;
		if ( h > m_min_letters_height) continue;
		//if ( h >= 8) continue;
		
		for ( int i = m_letterEdges[l][0]; i < m_letterEdges[l][2]; i++)
		{
			for ( int j =  m_letterEdges[l][1]; j < m_letterEdges[l][3]; j++)
			{
				if (m_pWorkData1[j*m_width*3+i*3+0] != -1) continue;
				m_pWorkData1[j*m_width*3+i*3+0] = l;
				m_pWorkData1[j*m_width*3+i*3+1] = -6;
			}
		}
	}





	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if (  !isAsialanguage(m_language ))
		{
			if ( h >= m_max_letters_height) continue;
		}
		if ( h < m_min_letters_height) continue;
		
		int i1 = m_letterEdges[l][0];
		int i2 = m_letterEdges[l][2];
		int j1 = m_letterEdges[l][1];
		int j2 = m_letterEdges[l][3];

		int nahashim1[10];
		int nahashim2[10];

		for ( int n = 0; n < 10; n++)
		{
			nahashim1[n] = -1;
			nahashim2[n] = -1;
		}
		int nn1 = 0;
		int nn2 = 0;
		int mainNahash = m_nahashim[l];
		if (mainNahash == -1) continue;
		for ( int i = i1; i <= i2; i++)
		{
			for ( int j = j1; j <= j2; j++)
			{
				int nahash = m_pWorkData1[j*m_width*3+i*3+1];
				
				if ( nahash < 0 ) continue;
				if ( nahash == mainNahash) continue;
				bool found = false;
				for ( int n = 0; n < nn1; n++)
				{
					if ( nahashim1[n] == nahash)
					{
						found = true;
						break;
					}
				}
				if (found) continue;
				for ( int n = 0; n < nn2; n++)
				{
					if ( nahashim2[n] == nahash)
					{
						found = true;
						break;
					}
				}
				if (found) continue;
				
				if ( !found )
				{
					int l2 = m_pWorkData1[j*m_width*3+i*3+0];
					int h2 = m_letterEdges[l2][3] - m_letterEdges[l2][1];
					//if ( h2 >= m_max_letters_height) continue;
					if ( !isAsialanguage(m_language ))
					{
						if ( h2 >= m_max_letters_height) continue;
					}
					int ii1 = m_letterEdges[l2][0];
					int ii2 = m_letterEdges[l2][2];
					int jj1 = m_letterEdges[l2][1];
					int jj2 = m_letterEdges[l2][3];
					int iii1 = (ii1 > i1) ? ii1 : i1;
					int iii2 = (ii2 < i2) ? ii2 : i2;
					int jjj1 = (jj1 > j1) ? jj1 : j1;
					int jjj2 = (jj2 < j2) ? jj2 : j2;
					/*int s = (i2 - i1)*(j2-j1);
					int ss = (ii2 - ii1)*(jj2 - jj1);
					int common_s = (iii2 - iii1) * ( jjj2 - jjj1);
					*/
					int h = j2 - j1;
					int hh = jj2 - jj1;
					int common_h = jjj2 - jjj1;
					int w = i2 - i1;
					int ww = ii2 - ii1;
					int common_w = iii2 - iii1;
					if ((common_h < h / 3/* || common_w < w / 3*/) && (common_h < hh/3 /*|| common_w < ww / 3*/))
					{
						nahashim1[nn1] = nahash;
						if ( nn1 < 9) nn1++;
					}
					else
					{
						nahashim2[nn2] = nahash;
						if ( nn2 < 9) nn2++;
					}
				}
			}
		}
		if ( nn2 > 0)
		{
			for ( int n = 0; n < nn2; n++)
			{
				int n1 = nahashim2[n];
				for ( int i = 0; i < m_width; i++)
				{
					for ( int j = 0; j < m_height; j++)
					{
						if ( m_pWorkData1[j*m_width*3+i*3+1] == n1)
							m_pWorkData1[j*m_width*3+i*3+1] = mainNahash;
					}
				}

				for ( int nnn = 0; nnn < _MAX_NUMBER_LETTERS_; nnn++)
				{
					if (m_nahashim[nnn] == n1)
						m_nahashim[nnn] = mainNahash;
				}
			}
		}
	}

	
    AddSpecialLetters(m_max_letters_height/5);
	UnifyNahashim();

	
}



void COneLine_4::UnifyNahashim()
{
	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		m_nahashimEdgesRight[n][0] = 0;
		m_nahashimEdgesRight[n][1] = m_height;
		m_nahashimEdgesRight[n][2] = 0;
		m_nahashimEdgesLeft[n][0] = m_width;
		m_nahashimEdgesLeft[n][1] = m_height;
		m_nahashimEdgesLeft[n][2] = 0;
		m_secondaryNahashim[n] = -1;
	}

	//find left, up, down, right, up, down edges of nahashim
	int max_n = 0;
	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;

		int i1 = m_letterEdges[l][0];
		int i2 = m_letterEdges[l][2];
		int j1 = m_letterEdges[l][1];
		int j2 = m_letterEdges[l][3];
		int n = m_nahashim[l];
		if ( n < 0) continue;

		int oldLeft = m_nahashimEdgesLeft[n][0];
		int oldLeftUp = m_nahashimEdgesLeft[n][1];
		int oldLeftDown = m_nahashimEdgesLeft[n][2];

		int oldRight = m_nahashimEdgesRight[n][0];
		int oldRightUp = m_nahashimEdgesRight[n][1];
		int oldRightDown = m_nahashimEdgesRight[n][2];


		int newLeft = oldLeft;
		int newLeftUp = oldLeftUp;
		int newLeftDown = oldLeftDown;

		int newRight = oldRight;
		int newRightUp = oldRightUp;
		int newRightDown = oldRightDown;


		if (i1 < oldLeft+50)
		{
			if (i1 < oldLeft) newLeft = i1;

			if (i1 < (oldLeft - 50))
			{
				newLeftUp = j1;
				newLeftDown = j2;
			}
			else
			{
				if ( oldLeftUp > j1) newLeftUp = j1;
				if ( oldLeftDown < j2) newLeftDown = j2;

			}
		}





		if (i2 > oldRight  - 50)
		{
			if (i2 > oldRight) newRight = i2;

			if (i2 > (oldRight + 50))
			{
				newRightUp = j1;
				newRightDown = j2;
			}
			else
			{
				if ( oldRightUp > j1) newRightUp = j1;
				if ( oldRightDown < j2) newRightDown = j2;

			}
		}


		m_nahashimEdgesLeft[n][0] = newLeft;
		m_nahashimEdgesLeft[n][1] = newLeftUp;
		m_nahashimEdgesLeft[n][2] = newLeftDown;

		m_nahashimEdgesRight[n][0] = newRight;
		m_nahashimEdgesRight[n][1] = newRightUp;
		m_nahashimEdgesRight[n][2] = newRightDown;
		
		if ( n > max_n) max_n = n;

	}
	int count_n = max_n + 1;

	int sizeNearestNahashim = count_n*count_n*5;
	if (count_n <= 4)
		sizeNearestNahashim = 5*5*5;
	int *nearestNahashim = new int[sizeNearestNahashim];
	for ( int s = 0; s < sizeNearestNahashim; s++)
		nearestNahashim[s] = -1;
	int secondary_free = 0;
	for ( int n = 0; n <  count_n; n++)
	{
		if ( n >= _MAX_NUMBER_NAHASHIM_) continue;

		int left = m_nahashimEdgesRight[n][0];
		int leftUp = m_nahashimEdgesRight[n][1];
		int leftDown = m_nahashimEdgesRight[n][2];

		int nn = 0;
		for ( int n1 = 0; n1 < count_n; n1++)
		{
			if ( n1 >= _MAX_NUMBER_NAHASHIM_) continue;
			if ( n == n1) continue;

			int right = m_nahashimEdgesLeft[n1][0];
			int rightUp = m_nahashimEdgesLeft[n1][1];
			int rightDown = m_nahashimEdgesLeft[n1][2];

			int distance =  right - left;
			if ( distance < 0 || distance > 100) continue;

			int j1 = (rightUp > leftUp) ? rightUp : leftUp;
			int j2 = (rightDown < leftDown) ? rightDown : leftDown;
			int commonHeight = j2 - j1;
			if ( commonHeight < 10) continue;

			int n_secondary_free = m_secondaryNahashim[n];
			int n1_secondary_free = m_secondaryNahashim[n1];
	
			for ( int n3 = 0; n3 < count_n; n3++)
			{
				if (n_secondary_free >= 0 && m_secondaryNahashim[n3] == n_secondary_free)
				{
					 m_secondaryNahashim[n3] = secondary_free;
				}
				if (n1_secondary_free >= 0 && m_secondaryNahashim[n3] == n1_secondary_free)
				{
					 m_secondaryNahashim[n3] = secondary_free;
				}
			}


			m_secondaryNahashim[n] = secondary_free;
			m_secondaryNahashim[n1] = secondary_free;
			secondary_free++;
			
			nearestNahashim[n*count_n*5 + n1*5 + 0] = n1; // nahash n1 is nearest from the right to nahash n
			nearestNahashim[n*count_n*5 + n1*5 + 1]= distance; // distance X between right of nahash n & left of nahash n1
			nearestNahashim[n*count_n*5 + n1*5 + 2] = commonHeight; // coommon Y of nahashim n & n1
			nearestNahashim[n*count_n*5 + 0*5 + 3]++; // number nearest nahashim from right
			nearestNahashim[n1*count_n*5 + 1*5 + 3]++; // number nearest nahashim from left
			nearestNahashim[n*count_n*5 + 2*5 + 3] = n1; // one of ( or only one) nearest from right
			nearestNahashim[n1*count_n*5 + 3*5 + 3] = n; // one of ( or only one) nearest from left

		}
	}
	for ( int n = 0; n < count_n; n++)
	{
		if (m_secondaryNahashim[n] == -1)
		{
			m_secondaryNahashim[n] = secondary_free++;
		}
	}

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int n = m_pWorkData1[j*m_width*3+i*3+1];
			if ( n < 0 || n >= count_n) continue;
			int n_newNahash = m_secondaryNahashim[n];
			m_pWorkData1[j*m_width*3+i*3+1] = n_newNahash;
			
		}
	}
	for ( int l = 0; l < _MAX_NUMBER_LETTERS_; l++)
	{
		int n = m_nahashim[l];
		if ( n < 0 || n >= count_n) continue;
		int n_newNahash = m_secondaryNahashim[n];
		m_nahashim[l] = n_newNahash;
		
	}
		
	delete [] nearestNahashim;
}
void COneLine_4::AddSpecialLetters(int distance)
{
	while (true)
	{
		int i_start = -1, j_start = -1;
		bool found = false;
		for ( int i = m_begin_x; i < m_end_x; i++)
		{
			for ( int j = _UP_EDGE_; j <= _DOWN_EDGE_; j++)
			{
				if (m_pWorkData1[j*m_width*3+i*3+1] == -2)
				{
					found = true;
					i_start = i;
					j_start = j;
					break;
				}
			}
			if ( found) break;
		}
		if (!found) break;
		int l_current = m_pWorkData1[j_start*m_width*3+i_start*3+0];
			/*int nahashim[_MAX_NUMBER_NAHASHIM_];
		for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
		{
			nahashim[n] = 0;
		}*/
		for ( int j =  m_letterEdges[l_current][1]; j < m_letterEdges[l_current][3]; j++)
		{
						
			int n = -1;
			int i_end = m_letterEdges[l_current][0]-1;
			int i_begin = i_end - m_max_search_width;
			int ii = i_end;
			for ( int i = i_end; i >= i_begin; i--, ii++)
			{
				if (i > 0 && m_pWorkData1[j*m_width*3+i*3+1] >= 0)
				{
					n = m_pWorkData1[j*m_width*3+i*3+1];
					break;
				}
				if (ii < m_width-1 && m_pWorkData1[j*m_width*3+ii*3+1] >= 0)
				{
					n = m_pWorkData1[j*m_width*3+ii*3+1];
					break;
				}


			}
			if ( n >= 0)
			{
				for ( int i =  m_letterEdges[l_current][0]; i < m_letterEdges[l_current][2]; i++)
				{
					if (m_pWorkData1[j*m_width*3+i*3+1] != -2) continue;
					if (m_pWorkData1[j*m_width*3+i*3+0] != l_current) continue;
					m_pWorkData1[j*m_width*3+i*3+1] = n;
				}
			}
			else
			{
				for ( int i =  m_letterEdges[l_current][0]; i < m_letterEdges[l_current][2]; i++)
				{
					if (m_pWorkData1[j*m_width*3+i*3+1] != -2) continue;
					if (m_pWorkData1[j*m_width*3+i*3+0] != l_current) continue;
					m_pWorkData1[j*m_width*3+i*3+1] = -3;
				}
			}
		}
	}

	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		for ( int j = _UP_EDGE_; j <= _DOWN_EDGE_; j++)
		{
			if (m_pWorkData1[j*m_width*3+i*3+1] == -3  )
			{
				bool connected = false;
				int n = -1;
				for ( int dj = 0; dj < distance/2; dj++)
				{
					int jj = j - dj;
					if ( jj >= _UP_EDGE_) 
					{
						n = m_pWorkData1[jj*m_width*3+i*3+1];
						if (n >= 0  )
						{
							m_pWorkData1[j*m_width*3+i*3+1] = n;
							break;
						}
					}
					jj = j + dj;
					if ( jj <= _DOWN_EDGE_) 
					{
						n = m_pWorkData1[jj*m_width*3+i*3+1];
						if (n >= 0 )
						{
							m_pWorkData1[j*m_width*3+i*3+1] = n;
							break;
						}
					}

				}
				
			}
		}
	}

	for ( int l = 0; l < m_numberLetters; l++)
	{
		if ( m_letterEdges[l][0] == -1) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if ( h >= 8) continue;
		if (m_letterEdges[l][3] > _DOWN_EDGE_ - 1) continue;
		if (m_letterEdges[l][1] < _UP_EDGE_ +1) continue;
		
		int n = -1;
		int i1 = m_letterEdges[l][0];
		int i2 = m_letterEdges[l][2];
		int j1 = m_letterEdges[l][1];
		int j2 = m_letterEdges[l][3];

		int minDist = 10000;
			
		bool connected = false;
		for ( int i = i1; i <= i2; i++)
		{
			for (int dj = 1; dj < distance; dj++)
			{
				int jj = j2 + dj;
				if ( jj < m_height-1)
				{
					int nn = m_pWorkData1[jj*m_width*3+i*3+1];
					if (nn >= 0 )
					{
						if (dj < minDist) 
						{
							n = nn;						
							minDist = dj;
							connected = true;
						}
					}
				}
				jj = j1 - dj;
				if ( jj > 0)
				{
					int nn = m_pWorkData1[jj*m_width*3+i*3+1];
					if (nn >= 0)
					{
						if (dj < minDist) 
						{
							n  = nn;
							minDist = dj;
							connected = true;
						}
					}
				}
			}
		}
		if (!connected)
		{
			for ( int j = j1; j <= j2; j++)
			{
				for (int di = 1; di < m_max_search_width/4; di++)
				{
					int ii = i2 + di;
					if ( ii < m_width-1)
					{
						int nn = m_pWorkData1[j*m_width*3+ii*3+1];
						if (nn >= 0 && m_pDataIn[j*m_width+ii] == TextColor)
						{
							if (di < minDist) 
							{
								n = nn;
								minDist = di;
								connected = true;
							}
						}
					}
					ii = i1 - di;
					if ( ii > 0)
					{
						int nn = m_pWorkData1[j*m_width*3+ii*3+1];
						if (nn >= 0)
						{
							if (di < minDist && m_pDataIn[j*m_width+ii] == TextColor) 
							{
								n  = nn;
								minDist = di;
								connected = true;
							}
						}
					}
				}
			}

		}
		if (connected) 
		{
			for ( int ii = i1; ii <= i2; ii++)
			{
				for ( int jj = j1; jj <= j2; jj++)
				{
					if (m_pWorkData1[jj*m_width*3+ii*3+0] != l) continue;
					m_pWorkData1[jj*m_width*3+ii*3+1] = n;
				}
			}
		}
		
	}



	for ( int l = 0; l < m_numberLetters; l++)
	{
		if ( m_letterEdges[l][0] == -1) continue;
		int h = m_letterEdges[l][3] - m_letterEdges[l][1];
		if ( h < 8) continue;
		if (h >= m_min_letters_height) continue;

		int n = -1;
		int i1 = m_letterEdges[l][0];
		int i2 = m_letterEdges[l][2];
		int j1 = m_letterEdges[l][1];
		int j2 = m_letterEdges[l][3];

		int minDist = 10000;
			
		bool connected = false;
		for ( int j = j1; j <= j2; j++)
		{
			for (int di = 1; di < m_max_search_width/4; di++)
			{
				int ii = i2 + di;
				if ( ii < m_width-1)
				{
					int nn = m_pWorkData1[j*m_width*3+ii*3+1];
					if (nn >= 0 && m_pDataIn[j*m_width+ii] == TextColor)
					{
						if (di < minDist) 
						{
							n = nn;
							minDist = di;
							connected = true;
						}
					}
				}
				ii = i1 - di;
				if ( ii > 0)
				{
					int nn = m_pWorkData1[j*m_width*3+ii*3+1];
					if (nn >= 0)
					{
						if (di < minDist && m_pDataIn[j*m_width+ii] == TextColor) 
						{
							n  = nn;
							minDist = di;
							connected = true;
						}
					}
				}
			}
		}
		if (connected) 
		{
			for ( int ii = i1; ii <= i2; ii++)
			{
				for ( int jj = j1; jj <= j2; jj++)
				{
					m_pWorkData1[jj*m_width*3+ii*3+1] = n;
				}
			}
		}
		
	}
}

int COneLine_4::ChooseNahash()
{

	int nahash_x[_MAX_NUMBER_NAHASHIM_][2];
	//int nahash_y[_MAX_NUMBER_NAHASHIM_][2];
	int nahashStart_y[_MAX_NUMBER_NAHASHIM_][2];
	int numberOutOfLine[_MAX_NUMBER_NAHASHIM_][2];
	int nahash_height[_MAX_NUMBER_NAHASHIM_][2];
	
	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		nahash_x[n][0] = -1;
		nahash_x[n][1] = -1;
		//nahash_y[n][0] = -1;
		//nahash_y[n][1] = -1;
		nahashStart_y[n][0] = m_height+1;
		nahashStart_y[n][1] = -1;
		nahash_height[n][0] = m_height+1;
		nahash_height[n][1] = 0;
		numberOutOfLine[n][0] = 0;
		numberOutOfLine[n][1] = 0;
	}

	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		for ( int j = _UP_EDGE_; j <= _DOWN_EDGE_; j++)
		{
			int n = m_pWorkData1[j*m_width*3+i*3+1];
			if (n < 0) continue;
			if ( nahash_x[n][0] == -1) nahash_x[n][0] = i;
			nahash_x[n][1] = i;
			if ( nahash_height[n][0] > j) nahash_height[n][0] = j;
			if ( nahash_height[n][1] < j) nahash_height[n][1] = j;
			if (nahash_x[n][0] == i)
			{
				if ( nahashStart_y[n][0] > j) nahashStart_y[n][0] = j;
				if ( nahashStart_y[n][1] < j) nahashStart_y[n][1] = j;
			}
			if ( j <= _UP_EDGE_+1 ) numberOutOfLine[n][0]++;
			if ( j >= _DOWN_EDGE_-1) numberOutOfLine[n][1]++;
		}
	}

	int min_width_scan = (m_end_x - m_begin_x)/5;
	int MIDDLE_Y = m_height / 2;
	int minDist = m_height;
	int n_minDist = -1;
	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		if (nahashStart_y[n][0] >= 128) continue;
		int w = nahash_x[n][1] - nahash_x[n][0];
		if ( w < min_width_scan) continue;
		if ((nahash_height[n][1] - nahash_height[n][0]) < 15) continue;
		if (numberOutOfLine[n][0] > 40) continue;
		if (numberOutOfLine[n][1] > 40) continue;
		int middle = (nahashStart_y[n][0] + nahashStart_y[n][1]) / 2;
		int dist = abs( middle - MIDDLE_Y);
		if (dist < minDist)
		{
			n_minDist = n;
			minDist = dist;
		}
	}
	if ( n_minDist >= 0) return n_minDist;
	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		if (nahashStart_y[n][0] >= 128) continue;
		int w = nahash_x[n][1] - nahash_x[n][0];
		if ( w < min_width_scan) continue;
		if ((nahash_height[n][1] - nahash_height[n][0]) < 15) continue;
		int middle = (nahashStart_y[n][0] + nahashStart_y[n][1]) / 2;
		int dist = abs( middle - MIDDLE_Y);
		if (dist < minDist)
		{
			n_minDist = n;
			minDist = dist;
		}
	}
	
	if ( n_minDist >= 0) return n_minDist;
	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		if (nahashStart_y[n][0] >= 128) continue;
		if ((nahash_height[n][1] - nahash_height[n][0]) < 15) continue;
		int middle = (nahashStart_y[n][0] + nahashStart_y[n][1]) / 2;
		int dist = abs( middle - MIDDLE_Y);
		if (dist < minDist)
		{
			n_minDist = n;
			minDist = dist;
		}
	}
	
	if ( n_minDist >= 0) return n_minDist;
	
	if ( n_minDist >= 0) return n_minDist;
	for ( int n = 0; n < _MAX_NUMBER_NAHASHIM_; n++)
	{
		if (nahashStart_y[n][0] >= 128) continue;
		int middle = (nahashStart_y[n][0] + nahashStart_y[n][1]) / 2;
		int dist = abs( middle - MIDDLE_Y);
		if (dist < minDist)
		{
			n_minDist = n;
			minDist = dist;
		}
	}
	

	return n_minDist;
	
}
void COneLine_4::FillChars(byte* dataIn, byte* dataOut)
{
	int * spaces = new int[m_width*m_height];
	int * filled = new int[m_width*m_height];
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			spaces[j*m_width+i] = 0;
			filled[j*m_width+i] = 0;
		}
	}

	for ( int j = 0; j < m_height; j++)
	{
		bool isSpace = true;
		bool isText = true;
		int begin = 0;
		for ( int i = 1; i < m_width; i++)
		{
			if ((dataIn[j*m_width+i] == TextColor) && isSpace)
			{
					isSpace = false;
					isText = true;
					spaces[j*m_width + i-1] = i - begin;
					begin = i;
			}
			else if ((dataIn[j*m_width+i] != TextColor) && isText)
			{
					isSpace = true;
					isText = false;
					filled[j*m_width + i-1] = i - begin;
					filled[j*m_width + begin] = i - begin;
					begin = i;
			}
		}
	}

	for (int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			dataOut[j*m_width*3+i*3+0] = dataIn[j*m_width+i];
			dataOut[j*m_width*3+i*3+1] = dataIn[j*m_width+i];
			dataOut[j*m_width*3+i*3+2] = dataIn[j*m_width+i];
		}
	}

	for (int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int spaceWidth = spaces[j*m_width+i];
			if (spaceWidth <= 0) continue;
			int widthLineBefore = filled[j*m_width + i-spaceWidth];
			if ( i + spaceWidth >= m_width) continue;
			int widthLineAfter = filled[j*m_width + i+1];
			if ((spaceWidth < (widthLineBefore)*20/4) && (spaceWidth < (widthLineAfter)*20/4))
			{
				for ( int s = 0; s < spaceWidth; s++)
				{
					dataOut[j*m_width*3+(i-s)*3+0] = 255;
					dataOut[j*m_width*3+(i-s)*3+1] = 127;
				}
			}


		}
	}
	for (int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < 2; j++)
		{
			dataOut[j*m_width*3+(i)*3+0] = 0;
			dataOut[j*m_width*3+(i)*3+1] = 0;
			dataOut[j*m_width*3+(i)*3+2] = 0;
		}
	}
    delete [] spaces;
	delete [] filled;
	//FillLetters(dataOut);
}
void COneLine_4::FillLetters(byte* dataInOut)
{


	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 + 1] = 0;
			m_pWorkData1[j*m_width*3 + i*3 + 0] = 0;
			m_pWorkData1[j*m_width*3 + i*3 + 2] = 0;
		}
	}
	
	int i_begin = m_begin_x-1;
	int j_begin = 5;

	int i_letter = 0;

	while (i_begin < m_end_x)
	{
		bool found = false;
		for ( ;i_begin < m_end_x+1; i_begin++)
		{
			for ( j_begin = m_begin_y; j_begin < m_end_y+1; j_begin++)
			{
				if ((dataInOut[j_begin*m_width*3 + i_begin*3+0] == TextColor) && (m_pWorkData1[(j_begin)*m_width*3 + (i_begin)*3 + 2] == 0)) 
				{
					found = true;
					break;
				}
			}
			if (found) break;
		}
		if ( i_begin >= m_end_x) break;

		int i_end = i_begin;
		int j_end = j_begin;
		int count = 1;
		m_pWorkData1[(j_begin)*m_width*3 + (i_begin)*3 + 2] = 1;
		while (count > 0)
		{
			count = 0;
			for ( int i = i_begin; i <= i_end; i++)
			{
				for ( int j = j_begin; j <= j_end; j++)
				{
					//if (m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] == 2) continue;
					if (dataInOut[(j)*m_width*3 + i*3+0] != TextColor) continue;
					if (m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] != 1) continue;
															
					if ( i < 2) continue;
					if ( i >= m_width - 2) continue;
					if ( j < 5) continue;
					if ( j >= m_height - 2) continue;
					for ( int ii = i-1; ii <= i+1; ii++)
					{
						for ( int jj = j-1; jj <= j+1; jj++)
						{
							if ( ii == i && jj == j) continue;
							if (dataInOut[(jj)*m_width*3 + ii*3] == TextColor) 
							{
								if (m_pWorkData1[(jj)*m_width*3 + (ii)*3 + 2] == 0)
								{
									m_pWorkData1[(jj)*m_width*3 + (ii)*3 + 2] = 1;
									count++;
									if (i_begin > ii) i_begin = ii;
									if ( i_end < ii) i_end = ii;
									if (j_begin > jj) j_begin = jj;
									if ( j_end < jj) j_end = jj;
								}
							}
						}
					}
					m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] = 2;
				}
			}
		}

	/*	for ( int i = i_begin-3; i <= i_end+3; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if ( dataInOut[j*m_width*3+i*3+0] == TextColor)
				{
					if ( j < j_begin) j_begin = j;
					if ( j > j_end ) j_end = j;
					if ( i < i_begin) i_begin = i;
					if ( i > i_end ) i_end = i;

				}
			}
		}

*/

		for ( int i = i_begin; i <= i_end ; i++)
		{
			for ( int j = j_begin; j <= j_end; j++)
			{
				m_pWorkData1[j*m_width*3 + i*3 + 1] = 100;
				dataInOut[j*m_width*3+i*3+0] = 255;


			}
		}


	}
	//m_numberLetters = i_letter;
		
}
