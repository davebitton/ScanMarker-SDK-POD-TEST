#include "StdAfx.h"
#include "HebrewItalic.h"
#include "FindWords.h"
#include "InOut.h"
#ifdef _SCANMARKER_WINDOWS_
#include "xv.h";
#include "xvbmp.h";
#else
#include "bmp/xv.h"
#include "bmp/xvbmp.h"
#endif

//#include "..\SplineInterpolation\SplineInterpolation.h"
#define White 255
#define Black 0

#define TextColor 255 
#define APP_ENGINE_LNG_HEBREW		230
void HebrewItalic::PrintDebugStr(char* str, bool append)
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

HebrewItalic::HebrewItalic(void)
{
	_NUMBER_CHANGES_TO_ANALYZE_ = 2;
	m_MAX_HEBREW_S = 0;
	_HALF_NUMBER_CHANGES_TO_ANALIZE_ = 2;
	for ( int j = 0; j < 1000; j++)
	{
		m_pChangesCoord[j] = NULL;
	}
	m_language = 0;
	m_width = 0;
	m_height = 0;
	m_pDataIn = NULL;
	m_pDataOut = NULL;
	m_pInterpolationOutput= NULL;
	m_pInterpolationInput= NULL;
	for ( int w = 0; w < MAX_NUMBER_WORDS; w++)
		m_lettersFileName[w][0] = 0;
	m_pBody = NULL;
	m_bodyHeight = NULL;
	m_body = NULL;
	m_activeChangesIndex = 0;
	m_begin_x = 0;
	m_end_x = 0;
	m_begin_y = 0;
	m_end_y = 0;
	m_pWorkData = NULL;
	m_pWorkData1 = NULL;
	m_pWorkData2 = NULL;
	for ( int s = 0; s < MAX_NUMBER_HEBREW_STEPS; s++)
		m_pWorkDataShifts[s] = NULL;
	
	for ( int w = 0; w < m_numberWords; w++)
		s_maxHeightSquareMarkArr[w] = -1;

	int w = 25;
	strcpy(m_lettersFileName[w--], "alef.bmp");
	strcpy(m_lettersFileName[w--], "bet.bmp");
	strcpy(m_lettersFileName[w--], "gimel.bmp");
	strcpy(m_lettersFileName[w--], "dalet.bmp");
	strcpy(m_lettersFileName[w--], "hei.bmp");
	strcpy(m_lettersFileName[w--], "vav.bmp");
	strcpy(m_lettersFileName[w--], "zain.bmp");
	strcpy(m_lettersFileName[w--], "het.bmp");
	strcpy(m_lettersFileName[w--], "tet.bmp");
	strcpy(m_lettersFileName[w--], "iud.bmp");
	strcpy(m_lettersFileName[w--], "kaf.bmp");
	strcpy(m_lettersFileName[w--], "kafsofit.bmp");
	strcpy(m_lettersFileName[w--], "lamed.bmp");
	strcpy(m_lettersFileName[w--], "mem.bmp");
	strcpy(m_lettersFileName[w--], "memsofit.bmp");
	strcpy(m_lettersFileName[w--], "nun.bmp");
	strcpy(m_lettersFileName[w--], "nunsofit.bmp");
	strcpy(m_lettersFileName[w--], "sameh.bmp");
	strcpy(m_lettersFileName[w--], "ain.bmp");
	strcpy(m_lettersFileName[w--], "pei.bmp");
	strcpy(m_lettersFileName[w--], "peisofit.bmp");
	strcpy(m_lettersFileName[w--], "tsadik.bmp");
	strcpy(m_lettersFileName[w--], "tsadiksofit.bmp");
	strcpy(m_lettersFileName[w--], "reish.bmp");
	strcpy(m_lettersFileName[w--], "shin.bmp");
	strcpy(m_lettersFileName[w--], "tav.bmp");
	m_italicStep = -1000;
	m_notToDoItalic = false;
}
 HebrewItalic::~HebrewItalic()
{
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pChangesCoord[j] != NULL)
			{
				delete[] m_pChangesCoord[j];
				m_pChangesCoord[j] = NULL;
			}
		}
		if (m_pInterpolationOutput)
		{
			delete[] m_pInterpolationOutput;
			m_pInterpolationOutput= NULL;
		}
		if (m_pInterpolationInput)
		{
			delete[] m_pInterpolationInput;
			m_pInterpolationInput= NULL;
		}
		if (m_pBody!= NULL)
		{
			delete[] m_pBody;
			m_pBody = NULL;
		}
		if (m_body!= NULL)
		{
			delete[] m_body;
			m_body = NULL;
		}
		if (m_bodyHeight != NULL)
		{
			delete[] m_bodyHeight;
			m_bodyHeight = NULL;
		}
		if (m_pWorkData != NULL)
		{
			delete[] m_pWorkData;
			m_pWorkData = NULL;
		}
		if (m_pWorkData1 != NULL)
		{
			delete[] m_pWorkData1;
			m_pWorkData1 = NULL;
		}

		if (m_pWorkData2 != NULL)
		{
			delete[] m_pWorkData2;
			m_pWorkData2 = NULL;
		}
		


		
		for ( int s = 0; s < MAX_NUMBER_HEBREW_STEPS; s++)
		{
			if (m_pWorkDataShifts[s] != NULL)
			{
				delete[] m_pWorkDataShifts[s];
				m_pWorkDataShifts[s] = NULL;
			}

		}

		
}
void HebrewItalic::AllocateMemory()
{
		m_begin_x = 0;
		m_end_x = m_width - 1;;
		m_begin_y = 0;
		m_end_y = m_height-1;


		m_activeChangesIndex = _HALF_NUMBER_CHANGES_TO_ANALIZE_*2+1;
		if (m_pInterpolationOutput)
		{
			delete[] m_pInterpolationOutput;
			m_pInterpolationOutput= NULL;
		}
		m_pInterpolationOutput= new float[m_output_width_x2];
		for ( int i = 0; i < m_output_width_x2; i++)
		m_pInterpolationOutput[i] = -10000.0;

		for ( int j = 0; j < m_height; j++)
		{
			if (m_pChangesCoord[j] != NULL)
			{
				delete[] m_pChangesCoord[j];
				m_pChangesCoord[j] = NULL;
			}
			
			m_pChangesCoord[j] = new int[m_activeChangesIndex+1];
		}
		if (m_pWorkData!= NULL)
		{
			delete[] m_pWorkData;
			m_pWorkData = NULL;
		}
		m_pWorkData = new byte[m_output_width_x2*m_height*3];
		memset(m_pWorkData, 255-TextColor, m_output_width_x2*m_height*3*sizeof(byte));

		if (m_pWorkData1!= NULL)
		{
			delete[] m_pWorkData1;
			m_pWorkData1 = NULL;
		}
		m_pWorkData1 = new byte[m_output_width_x2*m_height*3];
		memset(m_pWorkData1, 255-TextColor, m_output_width_x2*m_height*3*sizeof(byte));

		if (m_pWorkData2 != NULL)
		{
			delete[] m_pWorkData2;
			m_pWorkData2 = NULL;
		}
		m_pWorkData2 = new byte[m_output_width_x2*m_height*3];
		memset(m_pWorkData2 , 255-TextColor, m_output_width_x2*m_height*3*sizeof(byte));




		for ( int s = 0; s < MAX_NUMBER_HEBREW_STEPS; s++)
		{
			if (m_pWorkDataShifts[s] != NULL)
			{
				delete[] m_pWorkDataShifts[s];
				m_pWorkDataShifts[s] = NULL;
			}

		}


		if (m_pInterpolationInput)
		{
			delete[] m_pInterpolationInput;
			m_pInterpolationInput= NULL;
		}
		m_pInterpolationInput= new float[m_output_width_x2];
		for ( int i = 0; i < m_output_width_x2; i++)
		m_pInterpolationInput[i] = 0.0;
		if (m_pBody!= NULL)
		{
			delete[] m_pBody;
			m_pBody = NULL;
		}
		m_pBody = new byte[m_width*m_height];
		if (m_body!= NULL)
		{
			delete[] m_body;
			m_body = NULL;
		}
		m_body= new int[m_width];
		if (m_bodyHeight != NULL)
		{
			delete[] m_bodyHeight;
			m_bodyHeight = NULL;
		}
		m_bodyHeight = new int[m_width];

	
}


void HebrewItalic::LoadInputFile(char* fileName)
{
	PICINFO pinfo;
			
	LoadBMP(fileName , &pinfo);
			

	byte * input= m_pDataIn;//new byte[pinfo.w*pinfo.h];
	memset(input, 0,  sizeof(byte)*pinfo.w*pinfo.h);
	int s = 0;
	if (pinfo.type == PIC32)
	{
		s = 3;
	}
	else if (pinfo.type == PIC24)
	{
		s = 3;
	}
	else if ((pinfo.type == PIC8) && (pinfo.biBitCount == 1))
	{
		s = 0;
	}
	else if (pinfo.type == PIC8) 
	{
		s = 1;
	}
	if (s == 0)
	{
		for ( int i1 = 0; i1 < pinfo.w; i1++)
		{
			for ( int j1 = 0; j1 < pinfo.h; j1++)
			{
				int r = pinfo.pic[j1*pinfo.w + i1+0];
				int color = 0;
				if (r == 1) 
					color = 255;
				else 
					color = 0;
				input[j1*pinfo.w + i1]  = color;
			}
		}
	}
	else if (s == 1)
	{
		for ( int i1 = 0; i1 < pinfo.w; i1++)
		{
			for ( int j1 = 0; j1 < pinfo.h; j1++)
			{
				int r = pinfo.pic[j1*pinfo.w*s + i1*s+0];
				int color = 0;
				if (r < 250) 
					color = 0;
				else 
					color = 255;
				input[j1*pinfo.w + i1]  = color;
			}
		}
	}
	else
	{
		for ( int i1 = 0; i1 < pinfo.w; i1++)
		{
			for ( int j1 = 0; j1 < pinfo.h; j1++)
			{
				int r = pinfo.pic[j1*pinfo.w*s + i1*s+0];
				int g = pinfo.pic[j1*pinfo.w*s + i1*s+1];
				int b = pinfo.pic[j1*pinfo.w*s + i1*s+2];
				int color = 0;
				if ((r < 50) && (g < 50) && (b < 50))
					color = 0;
				else 
					color = 255;
				input[j1*pinfo.w + i1]  = color;
			}
		}
	}
}





void HebrewItalic::Init(int width, int height, int outputWidth, int outputHeight, byte* pDataIn, byte* pDataOut)
{
	m_width = width;
	m_output_width = outputWidth;
	m_output_width_x2 = outputWidth*2;
	m_output_height = outputHeight;
	m_height = height;
	m_pDataIn = pDataIn;
	m_pDataOut = pDataOut;
	m_dataInMode = 0;//1;
	m_dataOutMode = 0;
	AllocateMemory();


}
void HebrewItalic::ReadCfgFile(char* dir)
{
	return;
	char fileName[255];
	sprintf(fileName, "%s//%s", dir, "text_is_out.cfg");
	//sprintf(fileName, "text_is_out.cfg");
	FILE * fff = fopen(fileName, "rt");
	if (!fff) return;

	char str[255];
	while (fgets(str, 254, fff))
	{
		if (str[0] == '#') continue;

	
		/*if ( strstr(str, "_UP_EDGE_"))
		{
			sscanf(strstr(str, "_UP_EDGE_") + strlen("_UP_EDGE_"), "%ld", &_UP_EDGE_);
		}
		if ( strstr(str, "_DOWN_EDGE_"))
		{
			sscanf(strstr(str, "_DOWN_EDGE_") + strlen("_DOWN_EDGE_"), "%ld", &_DOWN_EDGE_);
		}
		if ( strstr(str, "_DOWN_EDGE_"))
		{
			sscanf(strstr(str, "_DOWN_EDGE_") + strlen("_DOWN_EDGE_"), "%ld", &_DOWN_EDGE_);
		}
		if ( strstr(str, "_NUMBER_CHANGES_TO_ANALYZE_"))
		{
			sscanf(strstr(str, "_NUMBER_CHANGES_TO_ANALYZE_") + strlen("_NUMBER_CHANGES_TO_ANALYZE_"), "%f", &_NUMBER_CHANGES_TO_ANALYZE_);
		}*/
	}
	fclose(fff);
	
}
void HebrewItalic::FindLetters(void* dataIn, int dataInMode,  int iTextColor, int*  pDataInShifte)
{
	for ( int ww = 0; ww < MAX_NUMBER_WORDS; ww++)
	{
		m_beginWords[ww] = -1;
		m_endWords[ww] = -1;;
	}
	m_numberWords = 0;
	
	int start = m_begin_x-5;
	int finish = m_end_x+5;
	if (start < 0) start = 0;
	if ( finish >= m_width) finish = m_width-1;
	FindWords findWords(m_width, m_height, dataIn, m_dataInMode, iTextColor);
	findWords.CalculateVerticalHistogram(start, finish, NULL);
	findWords.FindBigSpaces( start,  finish);
	int w = 0;
	findWords.m_beginWord = 6;
	findWords.m_endWord = 6;
	start -=10;
	if (start < 0) start = 0;
	while (start < finish)
	{
		if (w >= MAX_NUMBER_WORDS-2)
		{
			m_numberWords = w-2;
			return;
		}
		/*if (m_aveBodyHeight < 50)
			findWords.FindNextWord(start, finish);
		else*/
			findWords.FindNextWordBySpaceSize(start, finish);
			if (findWords.m_beginWord < 0)
			{
				m_numberWords = w;
				m_beginWords[m_numberWords] = -9999;
				m_endWords[m_numberWords] = -9999;
				return;
			}   
            if ((findWords.m_endWord - findWords.m_beginWord) < 100)
			{
				m_beginWords[w] = findWords.m_beginWord;
				start = findWords.m_endWord + 2;
				int temp_endWord = findWords.m_endWord;
				findWords.FindNextWordBySpaceSize(start, finish);
				if (findWords.m_beginWord < 0)
				{
					m_endWords[w] = temp_endWord;
				}
			
			}
			else
			{
				m_beginWords[w] = findWords.m_beginWord;
			}
		if (findWords.m_beginWord != -1)
			m_endWords[w] = findWords.m_endWord;
		w++;
		if (findWords.m_endWord < 0) break;
		start = findWords.m_endWord + 2;
	}

	m_numberWords = w;
}
int HebrewItalic::Calculate()
{
	//for ( int l = 0; l < 26; l++)
	{
	//	AllocateMemory();
		CalculateOne(0);
	
		if ( m_begin_x >= m_end_x) return 0;
return 1;

		memset(m_pWorkData1, 0, m_output_width*m_output_height*3);
		for ( int i = 0; i < m_output_width; i++)
		{
			for ( int j = m_begin_y-2; j < m_end_y+2; j++)
			{
				if ( (j <= 0) || (j >= m_height)) continue;
				m_pWorkData1[(j)*m_output_width+(i)] = m_pWorkData2[j*m_output_width_x2+i];
			}
		}

	/*	for ( int i = 0; i < m_output_width; i++)
		{
			for ( int j = m_begin_y-2; j < m_end_y-2; j++)
			{
				int value = m_pWorkData2[j*m_output_width_x2+i];
				value += m_pWorkData2[(j-1)*m_output_width_x2+i-1];
				value += m_pWorkData2[(j-1)*m_output_width_x2+i];
				value += m_pWorkData2[(j-1)*m_output_width_x2+i+1];
				value += m_pWorkData2[j*m_output_width_x2+i-1];
				value += m_pWorkData2[j*m_output_width_x2+i+1];
				value += m_pWorkData2[(j+1)*m_output_width_x2+i-1];
				value += m_pWorkData2[(j+1)*m_output_width_x2+i];
				value += m_pWorkData2[(j+1)*m_output_width_x2+i+1];
				bool correct = false;
				value = 0;
				if ( value == TextColor)// || value == 2*TextColor)
				{
					value = 255 - TextColor;
					correct = true;
				}
				if ( value == TextColor*8)// || value == 2*TextColor)
				{
					value =  TextColor;
					correct = true;
				}

/*					for ( int ii = -1; ii <= 1; ii++)
					{
						for ( int jj = -1; jj <=1; jj++)
						{
								m_pDataOut[(j+jj)*m_output_width*3+(i+ii)*3+0] = value;
								m_pDataOut[(j+jj)*m_output_width*3+(i+ii)*3+1] = value;
								m_pDataOut[(j+jj)*m_output_width*3+(i+ii)*3+2] = value;
						}
					}
* / 
				if (correct)
				{
					for ( int k = 0; k < 3; k++)
					{
						m_pWorkData1[(j)*m_output_width*k+(i)*k+0] = value;
						m_pWorkData1[(j)*m_output_width*k+(i-1)*k+0] = value;
						m_pWorkData1[(j)*m_output_width*k+(i+1)*k+0] = value;
						m_pWorkData1[(j-1)*m_output_width*k+(i)*k+0] = value;
						m_pWorkData1[(j+1)*m_output_width*k+(i)*k+0] = value;
						
					}
				}
			}
		}
		for ( int i = 0; i < m_output_width; i++)
		{
			for ( int j = m_begin_y-2; j < m_end_y-2; j++)
			{
				int value = m_pWorkData1[j*m_output_width+i];
				value += m_pWorkData1[(j-1)*m_output_width+i-1];
				value += m_pWorkData1[(j-1)*m_output_width+i];
				value += m_pWorkData1[(j-1)*m_output_width+i+1];
				value += m_pWorkData1[j*m_output_width+i-1];
				value += m_pWorkData1[j*m_output_width+i+1];
				value += m_pWorkData1[(j+1)*m_output_width+i-1];
				value += m_pWorkData1[(j+1)*m_output_width+i];
				value += m_pWorkData1[(j+1)*m_output_width+i+1];
				if ( value < 2) value = 0;
				value /= 9;
		
				m_pDataOut[j*m_output_width*3+i*3+0] = value;
				m_pDataOut[j*m_output_width*3+i*3+1] = value;
				m_pDataOut[j*m_output_width*3+i*3+2] = value;
			}
		}
		
*/
	}
	return 1;
}

void HebrewItalic::CalculateOne(int letter)
{
	//int t1, t2, t3, t4, t5, t6, t7;
/*	char fileNameIn[256];
	sprintf(fileNameIn, "%s//%s", m_dirname, m_lettersFileName[letter]);

	LoadInputFile(fileNameIn);
*/

	m_begin_x = -1;
	m_end_x = -1;
	m_begin_y = -1;
	m_end_y = -1;
//extern unsigned int getMillisecond(); 
	//int tStart = getMillisecond();


	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int value = m_pDataIn[j*m_width*3+i*3+0];
			if (value == TextColor)
			{
				if (m_begin_x == -1) m_begin_x = i;
				if (m_begin_y == -1) m_begin_y = j;
				if (i < m_begin_x) m_begin_x = i;
				if ( j < m_begin_y)  m_begin_y = j;
				if ( i > m_end_x) m_end_x = i;
				if ( j > m_end_y) m_end_y = j;
			}
		}
	}
	if ( m_begin_x >= m_end_x) return;
	{
		for (int i = 0; i <= m_width; i++)
		{
			for ( int j = 0; j < m_height ; j++)
			{
				m_pWorkData1[j*m_width*3+i*3+0] = m_pDataIn[j*m_width*3+i*3+0];
				m_pWorkData1[j*m_width*3+i*3+1] = m_pDataIn[j*m_width*3+i*3+1];
				m_pWorkData1[j*m_width*3+i*3+2] = m_pDataIn[j*m_width*3+i*3+2];
			}
		}
	
		for (int i = 0; i <= m_width; i++)
		{
			for ( int j = 0; j < m_height ; j++)
			{
				m_pWorkData1[j*m_width*3+i*3+0] = 255 - TextColor;
				m_pWorkData1[j*m_width*3+i*3+1] = 255 - TextColor;
				m_pWorkData1[j*m_width*3+i*3+2] = 255 - TextColor;
			}
		}
	}
	FindLetters(m_pDataIn, m_dataInMode,  White, NULL);
	int nw = 0;
	while (m_beginWords[nw] >= 0 && m_endWords[nw] < m_width && m_beginWords[nw] < m_endWords[nw])
		nw++;
	m_numberWords = nw;
	
//t3 = getMillisecond();
	CalculateBody();
//t4 = getMillisecond();


	int indOutput = 0;
	for ( int w = 0; w < m_numberWords; w++)
	{
		int bw = m_beginWords[w];
		int ew = m_endWords[w];
		m_beginWordsCopy[w] = m_beginWords[w];
		m_endWordsCopy[w] = m_endWords[w];

		int bw1 = indOutput;
		m_beginWordsOut[w] = bw1;
		int ew1 = indOutput + 2*m_MAX_HEBREW_S + ew - bw;
		m_endWordsOut[w] = ew1;
		
		indOutput = ew1 + 10;
		m_beginWords[w] = bw1;
		m_endWords[w] = ew1;
		
	}
	nw = 0;
	while (m_beginWords[nw] >= 0 && m_endWords[nw] < m_output_width_x2 && m_beginWords[nw] < m_endWords[nw])
		nw++;
	m_numberWords = nw;
	for ( int w = m_numberWords; w < MAX_NUMBER_WORDS; w++)
	{
		m_beginWords[w] = -1;
		m_endWords[w] = -1;
		m_beginWordsCopy[w] = -1;
		m_endWordsCopy[w] = -1;
		m_beginWordsOut[w] = -1;
		m_endWordsOut[w] = -1;
	}
	//memset(m_pDataOut, 255-TextColor, sizeof(byte)*m_output_width*m_height*3);
for ( int w = 0; w < m_numberWords; w++)
	{
		int bw = m_beginWordsCopy[w];
		int ew = m_endWordsCopy[w];

		int bw1 = m_beginWordsOut[w];
		int ew1 = m_endWordsOut[w];
		
		int shift = ((ew1-bw1)  - (ew-bw))/2;

		indOutput = bw1 + shift;
		m_beginWords[w] = indOutput;

		if (indOutput < 0 || indOutput > m_output_width_x2)
			indOutput = 0;
		memset(m_pWorkData, 255-TextColor, m_output_width_x2*m_height*sizeof(byte)*3);
		for (int i = bw; i <= ew; i++)
		{
            if ( i < 0 ) continue;
			for ( int j = m_begin_y-1; j<  m_end_y+1; j++)
			{
				if ( j < 0) continue;
				m_pWorkData[j*m_output_width_x2*3+indOutput*3+0] = m_pDataIn[j*m_width*3+i*3+0];
				m_pWorkData[j*m_output_width_x2*3+indOutput*3+1] = m_pDataIn[j*m_width*3+i*3+1];
				m_pWorkData[j*m_output_width_x2*3+indOutput*3+2] = m_pDataIn[j*m_width*3+i*3+2];
				m_pWorkData1[j*m_output_width_x2*3+indOutput*3+0] = m_pDataIn[j*m_width*3+i*3+0];
				m_pWorkData1[j*m_output_width_x2*3+indOutput*3+1] = m_pDataIn[j*m_width*3+i*3+1];
				m_pWorkData1[j*m_output_width_x2*3+indOutput*3+2] = m_pDataIn[j*m_width*3+i*3+2];
			}
			indOutput++;
		}
		m_endWords[w] = indOutput;
	//}
	/*{
		char fileNameOut[1024];
		sprintf(fileNameOut, "C:\\Temp\\work.bmp");
		FILE *fp = fopen(fileNameOut, "wb");
		
		WriteBMP(fp,m_pWorkData1,PIC24,m_output_width_x2,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
		fclose(fp);
	}
	*/
		int flag = 0;
	//t5 = getMillisecond();

for ( int s = -m_MAX_HEBREW_S; s <= m_MAX_HEBREW_S; s++)
	{
		flag = 1-flag;
		int jStep = s + m_MAX_HEBREW_S;
		StepItalic(m_pWorkData, m_dataInMode, NULL, 2, 0, m_output_width_x2, m_begin_y, m_end_y, s, m_pWorkDataShifts[jStep]);
		//StepItalic(m_pWorkData, m_dataInMode, NULL, 2, 0, m_width, m_begin_y, m_end_y, s, m_pWorkDataShifts[jStep]);

		int indOutput = 0;
		/*for ( int i = 0; i <= m_output_width; i++)
		{
			if ( i <= 0 ) continue;
			if ( i >= m_output_width) break;
			for ( int j = 0; j < m_height; j++)
			{
				int value = GetDataInValue(m_pWorkData, m_dataInMode,  i, j, m_output_width, m_height, m_pWorkDataShifts[jStep]);
				SetDataValue(m_pWorkData1, m_dataInMode, indOutput, j, value, m_output_width,  m_height);
			}
			if ( indOutput < m_output_width-1)
				indOutput++;
		}*/
	//	for ( int w = 0; w < m_numberWords; w++)
	//	{
			//if (flag)
			///	m_heightSquare[w][jStep] = 0;
			//else
		m_heightSquare[w][jStep] = CalculateHeightSquare(m_pWorkData,  m_dataInMode, w, TextColor, jStep);
			int len = m_endWords[w] - m_beginWords[w];
			int drawLen = 2*m_MAX_HEBREW_S+1;
			int shift = (len - drawLen) / 2;
			int ind = m_beginWords[w]+shift+s+m_MAX_HEBREW_S;
			if (ind < 0 || ind >= m_output_width_x2 ) continue;
			m_pInterpolationOutput[m_beginWords[w]+shift+s+m_MAX_HEBREW_S] = m_heightSquare[w][jStep]; 
		}
	}

	for (int i = 0; i < m_output_width_x2; i++)
	{
		for ( int j = 0; j <  m_height; j++)
		{
			m_pWorkData1[j*m_output_width_x2*3+i*3+0] = 255-TextColor;
			m_pWorkData1[j*m_output_width_x2*3+i*3+1] = 255-TextColor;
			m_pWorkData1[j*m_output_width_x2*3+i*3+2] = 255-TextColor;
		}
	}
	for ( int w = 0; w < m_numberWords; w++)
	{
		for ( int s = -m_MAX_HEBREW_S+4; s <= m_MAX_HEBREW_S-4; s++)
		{
			int jStep = s + m_MAX_HEBREW_S;
			int hS = m_heightSquare[w][jStep];
			if ( hS < 0.0001) continue;
			hS += m_heightSquare[w][jStep-4];
			hS += m_heightSquare[w][jStep-3];
			hS += m_heightSquare[w][jStep-2];
			hS += m_heightSquare[w][jStep-1];
			hS += m_heightSquare[w][jStep+1];
			hS += m_heightSquare[w][jStep+2];
			hS += m_heightSquare[w][jStep+3];
			hS += m_heightSquare[w][jStep+4];
			hS /= 9.0;
			m_heightSquare[w+m_numberWords][jStep] = hS;
			
		}
		for ( int s = -m_MAX_HEBREW_S; s <= m_MAX_HEBREW_S; s++)
		{
			int jStep = s + m_MAX_HEBREW_S;
			m_heightSquare[w][jStep] = m_heightSquare[w+m_numberWords][jStep];
		}
		
	}
	for ( int w = 0; w < m_numberWords; w++)
	{
		int maxHeightSquare = 0;
		int s_maxHeightSquare = 0;

		for ( int s = -m_MAX_HEBREW_S+4; s <= m_MAX_HEBREW_S-4; s++)
		//for ( int s = -m_MAX_HEBREW_S; s <= m_MAX_HEBREW_S; s++)
		{
			int jStep = s + m_MAX_HEBREW_S;
			/*int hS = (m_heightSquare[w][jStep-2] +m_heightSquare[w][jStep-1] +
					m_heightSquare[w][jStep]+
					m_heightSquare[w][jStep+1] + m_heightSquare[w][jStep+2])/5.0;*/
			/*int hS = (m_heightSquare[w][jStep-1] +
					m_heightSquare[w][jStep]+
					m_heightSquare[w][jStep+1])/3.0;
			*/
			int hS = m_heightSquare[w][jStep];
			if ( hS < 0.0001) continue;
			/*hS += m_heightSquare[w][jStep-3];
			hS += m_heightSquare[w][jStep-2];
			hS += m_heightSquare[w][jStep-1];
			hS += m_heightSquare[w][jStep+1];
			hS += m_heightSquare[w][jStep+2];
			hS += m_heightSquare[w][jStep+3];*/
			//hS += m_heightSquare[w][jStep-2];
			//hS += m_heightSquare[w][jStep+2];

			//int hS = m_heightSquare[w][jStep];
			if ( maxHeightSquare < hS)
			{
				maxHeightSquare = hS;
				s_maxHeightSquare = s;
			}
		}
		int maxValue = 0;
		int maxStep = s_maxHeightSquare;
		for ( int s = s_maxHeightSquare - 4; s <= s_maxHeightSquare+4; s++)
		{
			int jStep = s + m_MAX_HEBREW_S;
			if (maxValue < m_heightSquare[w][jStep])
			{
				maxValue = m_heightSquare[w][jStep];
				maxStep = s;
			}
		}
		s_maxHeightSquareArr[w] = s_maxHeightSquare; 
		//CalculateHeightSquareMark(w);
	}
//	t6 = getMillisecond();
memset(m_pWorkData, 0, sizeof(byte)*m_output_width_x2*m_height*3);
	DrawOutput(m_pInterpolationOutput, 2, 0, 255, 0);
	memset(m_pDataOut, 0, sizeof(byte)*m_output_width*m_height*3);
	memset(m_pDataOut, 255-TextColor, sizeof(byte)*m_output_width*m_height*3);
	int prev_end = 0;
	indOutput = 0;
	int stepsSortedIndexes[MAX_NUMBER_HEBREW_STEPS];
	for ( int w = 0; w < m_numberWords; w++)
		stepsSortedIndexes[w] = w;
	PrintDebugStr("eeeeeeeeeccccc");
for ( int w = 0; w < m_numberWords; w++)
	{
		for ( int w1 = w+1; w1 < m_numberWords; w1++)
		{
			if (s_maxHeightSquareArr[stepsSortedIndexes[w]] < s_maxHeightSquareArr[stepsSortedIndexes[w1]])
			{
				int temp = stepsSortedIndexes[w];
				stepsSortedIndexes[w] = stepsSortedIndexes[w1];
				stepsSortedIndexes[w1] = temp;
			}	
		}
	}
	if ( m_numberWords >= 4)
	{
		int s1 = s_maxHeightSquareArr[stepsSortedIndexes[0]];
		int s2 = s_maxHeightSquareArr[stepsSortedIndexes[1]];
		if (abs(s1 - s2) > 4)
		{
			s_maxHeightSquareArr[stepsSortedIndexes[0]] = s2;
		}
		s1 = s_maxHeightSquareArr[stepsSortedIndexes[m_numberWords-1]];
		s2 = s_maxHeightSquareArr[stepsSortedIndexes[m_numberWords-2]];
		if (abs(s1 - s2) > 4)
		{
			s_maxHeightSquareArr[stepsSortedIndexes[m_numberWords-1]] = s2;
		}
	}

	int prev_bw = 0;
	int prev_ew = 0;
	int jStepPrev = -1;
	int jStepPrevPrev = -1;
	int jStepPrevApply = -1;
	for ( int ww = 0; ww < m_numberWords; ww++)
	{

		int s = s_maxHeightSquareArr[ww];
		if (s < -9999)
		{
			s = 0;
			int i = 1;
			int count = 0;
			int s_ave = 0;
			while(i <= 4)
			{
				if (ww - i >= 0)
				{
					int s1 = s_maxHeightSquareArr[ww-i];
					if ( s1 > -9999)
					{
						s_ave += s1;
						count++;
					}
				}
				if ( ww+i < m_numberWords)
				{
					int s1 = s_maxHeightSquareArr[ww+i];
					if ( s1 > -9999)
					{
						s_ave += s1;
						count++;
					}
				}
				if (count > 0) break;
				i++;
			}
			if (count)
			{
				s = s_ave/count;
			}
			else
				s = 0;
			
		}
		if ( s < -m_MAX_HEBREW_S || s > m_MAX_HEBREW_S ) continue;
		//if (m_numberWords == 1 )//&& ( s < -((float)m_MAX_HEBREW_S)/0.7*0.3 || s > ((float)m_MAX_HEBREW_S)/0.7*0.5 ))
		//	s = 0;
		if (m_notToDoItalic)
		{
			m_italicStep = s;
			s = 0;
		}
		int jStep = s + m_MAX_HEBREW_S;

		if (ww == 0) 
		{
			jStepPrevApply = jStep;
			jStepPrev = jStep;
			jStepPrevPrev = jStep;
			//continue;
		}
		int jStepPrevApply1 = (jStep + jStepPrev + jStepPrevPrev ) / 3;
		jStepPrevPrev = jStepPrev;
		jStepPrev = jStep;
		//jStep = jStepPrevApply;
		jStepPrevApply = jStepPrevApply1;


		

		int bw = m_beginWordsCopy[ww];
		int ew = m_endWordsCopy[ww];

		int next_bw = m_beginWordsCopy[ww+1];

 		/*bw -= (bw - prev_ew)/2;

		if (next_bw >= 0)
		{
			ew += (m_beginWordsCopy[ww] < 0) ? 5 : (next_bw - ew) / 2;
		}
		else
		{
			ew += 100;
			if ( ew > m_width) ew = m_width-1;
		}
		*/
		//prev_bw = m_beginWordsCopy[ww];
		//prev_ew = m_endWordsCopy[ww];
		
		int bw1 = m_beginWordsOut[ww];
		int ew1 = m_endWordsOut[ww];
		


		int shift = ((ew1-bw1)  - (ew-bw))/2;

		//indOutput = bw1 + shift;
		//indOutput += bw - prev_end-1;// + ((s>0)?s:0);
		prev_end = ew;// + ((s < 0) ? (-s) : 0);
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pWorkData1[j*m_width*3+i*3+0] = 255 - TextColor;
				m_pWorkData1[j*m_width*3+i*3+1] = 255 - TextColor;
				m_pWorkData1[j*m_width*3+i*3+2] = 255 - TextColor;
			}
		}
		for ( int i = bw; i < ew; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pWorkData1[j*m_width*3+i*3+0] = m_pDataIn[j*m_width*3+i*3+0];;
				m_pWorkData1[j*m_width*3+i*3+1] = m_pDataIn[j*m_width*3+i*3+1];;
				m_pWorkData1[j*m_width*3+i*3+2] = m_pDataIn[j*m_width*3+i*3+2];;
			}
		}
		prev_ew = ew;
		for (int i = bw-m_MAX_HEBREW_S; i <= ew+m_MAX_HEBREW_S; i++)
		{
			bool active = false;
			for ( int j = m_begin_y; j<= m_end_y; j++)
			{
				if ( j < 0 || j >= m_height-1) continue;
				int jStep1 = jStep;//+2;
				if (jStep1 > m_MAX_HEBREW_S*2-3) 
					jStep1 = m_MAX_HEBREW_S*2-3;
				int ii = i - m_pWorkDataShifts[jStep1][j];
				if ( ii > ew) continue;
				if ( ii < bw) continue;
				//m_pWorkData2[j*m_output_width_x2+indOutput] = m_pDataIn[j*m_width+ii];
				active = true;
				m_pDataOut[j*m_output_width*3+indOutput*3+0] = m_pWorkData1[j*m_width*3+ii*3+0];
				m_pDataOut[j*m_output_width*3+indOutput*3+1] = m_pWorkData1[j*m_width*3+ii*3+1];
				m_pDataOut[j*m_output_width*3+indOutput*3+2] = m_pWorkData1[j*m_width*3+ii*3+2];

			}
			if (!active) continue;
			if (indOutput < m_output_width- 1)
				indOutput++;
		}
	}

	/*
	for ( int i = 0; i < m_output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[(j+m_height)*m_output_width*3 + i*3 + 0] = m_pWorkData[j*m_output_width*3+i*3+0];
			m_pDataOut[(j+m_height)*m_output_width*3 + i*3 + 1] = m_pWorkData[j*m_output_width*3+i*3+1];
			m_pDataOut[(j+m_height)*m_output_width*3 + i*3 + 2] = m_pWorkData[j*m_output_width*3+i*3+2];
		}
	}
*/
	/*
	for ( int w = 0; w < 26;/*m_numberWords;* / w++)
	{
		CreateLetterItalicFile(w);
	}*/
	/*t7 = getMillisecond();
	t1-=tStart;
	t2-=tStart;
	t3-=tStart;
	t4-=tStart;
	t5-=tStart;
	t6-=tStart;
	t7-=tStart;
*/
	/*bool isDebugMode = true;
	if (isDebugMode)
	{
		FILE *fff1 = fopen("C:\\Temp\\TextImageTime.txt", "at");
		if ( fff1)
		{
			SYSTEMTIME currentTime;
			GetSystemTime(&currentTime);
			fprintf(fff1, "\n\n\n China Full New Run At %ld//%ld//%ld %ld:%ld:%ld", 
				currentTime.wDay, currentTime.wMonth, currentTime.wYear, 
				currentTime.wHour, currentTime.wMinute, currentTime.wSecond );
			
			fprintf(fff1, "\n  t2 = %ld\n t3 = %ld\n t4 = %ld\n  t6 = %ld\n t7 = %ld\n",
				 t2, t3, t4,  t6, t7);
		}
	}*/


}
void HebrewItalic::CalculateHeightSquareMark(int w)
{
	int sMax = s_maxHeightSquareArr[w];
	int jStepMax = sMax + m_MAX_HEBREW_S;
	int heightSquareMax = m_heightSquare[w][jStepMax];
	int mark = 0;
	float diff = 0;
	int numberInCalc = 8;
	int count = 0;
	for ( int i = 0; i <= m_MAX_HEBREW_S*2/3; i++)
	{
		{
			int s = sMax + i;
			int jStep= s+ m_MAX_HEBREW_S;
			while (m_heightSquare[w][jStep] < 0.001) 
			{
				s++;
				jStep= s+ m_MAX_HEBREW_S;
				if ( s >= m_MAX_HEBREW_S) break;
			}
			if ( s >= m_MAX_HEBREW_S) continue;

			int s1 = s+1;
			int jStep1= s1+ m_MAX_HEBREW_S;
			while (m_heightSquare[w][jStep1] < 0.001) 
			{
				s1++;
				jStep1= s1+ m_MAX_HEBREW_S;
				if ( s1 >= m_MAX_HEBREW_S) break;
			}
			if ( s1 >= m_MAX_HEBREW_S) continue;
			if (jStep >= 0 && jStep1 <= 2*m_MAX_HEBREW_S)
			{
				float heightSquare= m_heightSquare[w][jStep];
				float heightSquare1= m_heightSquare[w][jStep1];
				if (heightSquare1 > 0.0001 && heightSquare > 0.0001)
				{

					float value = (heightSquare - heightSquare1)*abs(heightSquare - heightSquare1);
					float value1 = (heightSquare1*heightSquare1);
					float value3 = value / value1;
					diff += value3;
				//	diff += abs(heightSquareMax - heightSquare)*(heightSquareMax - heightSquare);
					count++;
				}
			}
		}
	}
	for ( int i = 0; i <= m_MAX_HEBREW_S*2/3; i++)
	{
		{
			int s = sMax - i;
			int jStep= s+ m_MAX_HEBREW_S;
			while (m_heightSquare[w][jStep] < 0.001)  
			{
				s--;
				jStep= s+ m_MAX_HEBREW_S;
				if ( s <= -m_MAX_HEBREW_S) break;
			}
			if ( s <= -m_MAX_HEBREW_S) continue;
			
			int s1 = s-1;
			int jStep1= s1+ m_MAX_HEBREW_S;
			while (m_heightSquare[w][jStep1] < 0.001) 
			{
				s1--;
				jStep1= s1+ m_MAX_HEBREW_S;
				if ( s1 <= -m_MAX_HEBREW_S) break;
			}
			if ( s1 <= -m_MAX_HEBREW_S) continue;
			/*int s = sMax - i;
			int jStep= s+ m_MAX_HEBREW_S;
			int s1 = s -1;
			int jStep1= s1+ m_MAX_HEBREW_S;*/
			if (jStep1 >= 0 && jStep <= 2*m_MAX_HEBREW_S)
			{
				float heightSquare= m_heightSquare[w][jStep];
				float heightSquare1= m_heightSquare[w][jStep1];
				if (heightSquare1 > 0.0001 && heightSquare > 0.0001)
				{
/*					int sign = ((heightSquare1 - heightSquare) > 0) ?  -1 : 1;
					float value = (heightSquare - heightSquare1)*abs(heightSquare - heightSquare1);
					value /= (float) (heightSquare1*heightSquare1);
					diff += value;
*/
					float value = (heightSquare - heightSquare1)*abs(heightSquare - heightSquare1);
					float value1 = (heightSquare1*heightSquare1);
					float value3 = value / value1;
					diff += value3;

						;
				//	diff += (heightSquareMax - heightSquare)*(heightSquareMax - heightSquare);
					count++;
				}
			}
		}
	}
	int word_width = m_endWords[w] - m_beginWords[w];
	if (!count) count = 1;
	float diff1 = (float) diff / (float) count;
//	s_maxHeightSquareMarkArr[w][0] = diff1;
	count = m_MAX_HEBREW_S*4/3;
	s_maxHeightSquareMarkArr[w] = diff*100000.0/count;//*10000.0;///(float)(count*word_width);
	/*if (s_maxHeightSquareMarkArr[w] < 15.0)
	{
		s_maxHeightSquareArr[w] = -10000;
	}
	*/
	if (word_width < m_aveBodyHeight*2.0)
	{
		s_maxHeightSquareArr[w] = -10000;
	}

}

float HebrewItalic::CalculateHeightSquare(void* dataIn, int dataInMode, int w, int iTextColor, int jStep)
{
	float word_height_square = 0;
	int numberTexts= 0;
	int numberLowTexts = 0;
	int numberTextColumns = 0;
	float numberTextPixels = 0;
	int h_col = 0;
	int begin_text = 0;
	bool insideText = false;
	bool isTextColumn = false;
	int i;	
	int value;
	int prevValue_h;
	int nextValue_h;
	int prevValue_w;
	int nextValue_w;
	int nLines = 0;
	byte* dataInByte = (byte*) dataIn;
	int s = jStep - m_MAX_HEBREW_S;

/*	int h1=m_height, h2 = 0;
	for ( int i = start; i < finish; i++)
	{
		for ( int j = m_begin_y; j < m_end_y; j++)
		{
			int value = GetDataInValue(dataIn, dataInMode,  i, j, m_width, m_height);//, pShifts);
			if (IsText(value,iTextColor))
			{
				if ( h1 > j) h1 = j;
				if ( h2 < j) h2 = j;
			}
		}

	}
*/
/*	int heightSquareTrhesold = (h2-h1) / 4;
	int one_height = m_end_y - m_begin_y;

	float heightThreshold = 0.25*(float) one_height;
*/
	int word_height = m_end_y - m_begin_y;// m_h2[w] - m_h1[w];
	float heightThreshold = 0.4*(float) m_aveBodyHeight;
	int i1 = m_beginWords[w];//[m_MAX_HEBREW_S];
	int i2 = m_endWords[w];//[m_MAX_HEBREW_S];
	int s1 = m_MAX_HEBREW_S;
	int iii;
	long index1;
	long index_prev1;
	long index_next;
	int valueTemp;
	int nChanges = 0;
	int* pShifts = m_pWorkDataShifts[jStep]; 
		/*for ( int s = 0; s < m_MAX_HEBREW_S; s++)
	{
		s1 = s+m_MAX_HEBREW_S;
		i1 = m_beginWords[w][s1];
		i2 = m_endWords[w][s1];
		if (i1 > 0 && i2 > 0) break;

		s1 = -s+m_MAX_HEBREW_S;
		i1 = m_beginWords[w][s1];
		i2 = m_endWords[w][s1];
		if (i1 > 0 && i2 > 0) break;
	}*/
	int start = m_beginWords[w];//[jStep];
	int finish = m_endWords[w];//[jStep]
	if (i1 < 0 || i2 < 0) return -1;;
	byte bgColor = 255 - iTextColor;
	start -= m_MAX_HEBREW_S;
	finish += m_MAX_HEBREW_S;
/*	for ( int i = 0; i < m_output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[m_output_width*j*3+i*3+0] = bgColor;
			m_pDataOut[m_output_width*j*3+i*3+1] = bgColor;
			m_pDataOut[m_output_width*j*3+i*3+2] = bgColor;
		}
	}
	for ( int i = start-2; i < finish+2; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[m_output_width*j*3+i*3+0] = dataInByte[j*m_output_width_x2+(i-pShifts[j])];
			m_pDataOut[m_output_width*j*3+i*3+1] = dataInByte[j*m_output_width_x2+(i-pShifts[j])];
			m_pDataOut[m_output_width*j*3+i*3+2] = dataInByte[j*m_output_width_x2+(i-pShifts[j])];
		}
	}

	{
		char fileNameOut[1024];
		sprintf(fileNameOut, "C:\\Temp\\work.bmp");
		FILE *fp = fopen(fileNameOut, "wb");
		
		WriteBMP(fp,m_pDataOut,PIC24,m_output_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
		fclose(fp);
	}*/
	//int s = 0;
	{
		
		//int start = m_beginWords[w];//[jStep];
		//int finish = m_endWords[w];//[jStep]
		/*;
		for ( int s = -m_MAX_HEBREW_S; s <= m_MAX_HEBREW_S; s++)
		{
			int jStep = s + m_MAX_HEBREW_S;
			m_heightSquare[w][jStep] = -1;
		}*/
		//if ( start == -1 || finish == -1) continue;
		word_height_square = 0;
		int lengthHighLines = 0;
		int lengthAllLines = 0;
		float word_height = 0;
		if (m_begin_y < 2) start = 2;
		if (m_end_y >= m_height-2) m_end_y= m_height-3;
		if (start < 2) start = 2;
		if (finish >= m_output_width_x2 -2) finish = m_output_width_x2-3;
		/*int step = 10;
		if (m_end_y - m_begin_y > 45)
		{
			step = 10;
		}*/
		//start -= m_MAX_HEBREW_S;
		//finish += m_MAX_HEBREW_S;
		for ( int ii = start-2; ii < finish+2; ii++)
		{
			if ( ii <= 1 || finish >= m_output_width_x2-50) continue;
			begin_text = 0;
			insideText = false;
			isTextColumn = false;
			//int h_col = 0;
     				
			for ( int j = m_begin_y-1; j <= m_end_y+1; j++)
			{
				//i = ii;
	/*			if (pShifts)
				{
					i += pShifts[j];
				}
	*/
				if (j < 1 || j >= m_height-2) continue;
				if (ii < 1 || ii >= m_output_width_x2-2) continue;
				/*value = GetDataInValue(dataIn, dataInMode,  i, j, m_output_width, m_height, m_pWorkDataShifts[jStep]);
				int valueTemp = GetDataInValue(m_pDataOut, dataInMode,  i, j, m_output_width, m_height, m_pWorkDataShifts[jStep]);
				prevValue_h = GetDataInValue(dataIn, dataInMode,  i, j-1, m_output_width, m_height, m_pWorkDataShifts[jStep]);
*/

				index1 = (j*m_output_width_x2+(ii-pShifts[j]))*3+0;
				index_prev1 = ((j-1)*m_output_width_x2+(ii-pShifts[j-1]))*3+0;
				//index_next = (j+1)*m_output_width_x2+(ii-pShifts[j+1]);
				if (index1 < 0 || index_prev1 < 0)// || 
					//index1 >= m_output_width_x2 || index_prev1 >= m_output_width_x2) 
					continue;
				value = dataInByte[index1];
				valueTemp = m_pDataOut[index1];
				prevValue_h = dataInByte[index_prev1];
				//nextValue_h = dataInByte[index_next];
		//			prevValue_w = GetDataInValue(dataIn, dataInMode,  i-1, j, m_width, m_height, pShifts);
	//			nextValue_w = GetDataInValue(dataIn, dataInMode,  i+1, j, m_width, m_height, pShifts);

	/*			if (IsText(value,iTextColor))
				{
					isTextColumn = true;
					numberTextPixels++;
				}
	*/
				if (/*(valueTemp==iTextColor) && */(value==iTextColor) && (prevValue_h!=iTextColor)&&(!insideText))
				{
					begin_text = j;
					insideText = true;
					nChanges++;
					//h_col = 0;
				}
				/*
				if (insideText && IsText(value,iTextColor)  &&
					((!IsText(prevValue_w,iTextColor)) || (!IsText(nextValue_w,iTextColor))))
				{
					h_col++;
				}
				*/
				//if (((!IsText(value,iTextColor)) || (!IsText(valueTemp,iTextColor)) ) && IsText(prevValue_h,iTextColor) && insideText)
				if (((value != iTextColor) /*|| (valueTemp!=iTextColor) */) && (prevValue_h==iTextColor)  &&insideText)
				{
					h_col =  j - begin_text;
					/*if (h_col > heightThreshold)
					{
						lengthHighLines +=h_col;
					}
					if (h_col > heightThreshold) //temporary
						lengthAllLines+=h_col;*/					
					//if (h_col > m_mfVerticalThick*2)
					//if (h_col > 6)
					
					//if (h_col > heightThreshold) //temporary
					{
						word_height_square += h_col*h_col;//*h_col;
						//nLines++;
						//word_height+=h_col;
						//numberTexts++;
					}
					//numberTexts++;
					begin_text = 0;
					insideText = false;
				}
			}
	/*		if (isTextColumn)
			{
				numberTextColumns++;
			}
	*/
		}
		//if (!word_height) word_height = 1;
//		m_heightSquare[w]/*[jStep]*/[0] = word_height_square;//*nLines/word_height;
		//if (lengthHighLines > 0)
		///	m_heightSquare[w]/*[jStep]*/[1] = lengthHighLines*100/lengthAllLines;
	
	}
	if (!nLines) nLines = 1;
	if (!word_height) word_height = 1;
	return word_height_square;///nLines*nLines;//*nLines;//word_height*word_height;

//	memset(m_pDataOut, 0, m_output_width*m_height*3*sizeof(byte));

	//trustLevel = word_height_square / (numberTextPixels*numberTextPixels);
	//trustLevel = (float)numberLowTexts *100.0 / (float)numberTexts;
//	numberTextColumns = (numberTextColumns ==0)? 1: numberTextColumns;

//	trustLevel /= (float) one_height;
	//word_height_square /= (float) numberTextColumns;
	//return word_height_square;///(finish-start);//*numberTexts*numberTexts;//(numberTextColumns*numberTextColumns);//*numberTexts;//*numberTexts;///numberTextColumns;
	//return numberTextColumns;

}

void HebrewItalic::DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue, float maxValue)
{
	float ratio_min = 10000000;
	float ratio_max = 0;
	for ( int i = 0; i < m_output_width_x2; i++)
	{
		float value;
		if (valuesType == 1)
		{
			value = ((int*)values)[i];
		}
		else if (valuesType ==2)
		{
			value = ((float*)values)[i];
		}


		if (value  < -1000)
		{
			continue;
		}
		
		if (value > ratio_max)
		{
			ratio_max = value;
		}
		if (value < ratio_min)
		{
			ratio_min = value;
		}
	}


	if ( minValue > -9999)
		ratio_min = minValue;

	if ( maxValue > -9999)
		ratio_max = maxValue;


	float delta = ratio_max - ratio_min;
	
	float show_ratio = 1.0;
	if (delta  > 0)
		show_ratio = (float)(m_height - 10) / delta;

	for ( int i = 0; i < m_output_width_x2; i++)
	{
		float value;
		if (valuesType == 1)
		{
			value = ((int*)values)[i];
		}
		else if (valuesType ==2)
		{
			value = ((float*)values)[i];
		}

		if (value  < -1000)
		{
			continue;
		}
		
		int j = (int) (m_height - 1 - show_ratio * (value - ratio_min)) ;
		if ( j > m_height - 1) j = m_height - 1;
		if ( j < 0) j = 0;
/*
		if ( red != -1)
			m_pDataOut[j*m_output_width*3 + i*3+0]  = red;

		if ( green != -1)
			m_pDataOut[j*m_output_width*3 + i*3+1]  = green;

		if ( blue != -1)
			m_pDataOut[j*m_output_width*3 + i*3+2]  = blue;
*/
		if ( red != -1)
			m_pWorkData[j*m_output_width_x2*3 + i*3+0]  = red;

		if ( green != -1)
			m_pWorkData[j*m_output_width_x2*3 + i*3+1]  = green;

		if ( blue != -1)
			m_pWorkData[j*m_output_width_x2*3 + i*3+2]  = blue;

	}

}
void HebrewItalic::CreateLetterItalicFile(int w)
{
	memset(m_pWorkData, 255-TextColor, sizeof(byte)*m_output_width_x2*m_height*3);
	memset(m_pDataOut, 255-TextColor, sizeof(byte)*m_output_width*m_height*3);

	for ( int i = m_beginWords[w]; i <= m_endWords[w]; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData[j*m_width+i] = m_pDataIn[j*m_width+i];
		}
	}

	int indOutput = 0;
	for ( int s = -m_MAX_HEBREW_S; s <= m_MAX_HEBREW_S; s+=2)
	{
		int jStep = s + m_MAX_HEBREW_S;
		
		int begin_x = m_beginWords[w] - abs(s);
		int end_x = m_endWords[w] + abs(s);
		StepItalic(m_pDataIn, m_dataInMode, NULL, 2, begin_x, end_x, m_begin_y, m_end_y, s, m_pWorkDataShifts[jStep]);
		for ( int i = begin_x; i <= end_x; i++)
		{
			if ( i <= 0 ) continue;
			if ( i >= m_width) break;
			for ( int j = 0; j < m_height; j++)
			{
				int value = GetDataInValue(m_pWorkData, m_dataInMode,  i, j, m_output_width_x2, m_height, m_pWorkDataShifts[jStep]);
				SetDataValue(m_pDataOut, m_dataOutMode, indOutput, j, value, m_output_width,  m_height);
			}
			if ( indOutput < m_output_width-1)
				indOutput++;
		}
	
		if ( indOutput < m_output_width-10)
			indOutput += 10;
		
	}

	char fileNameOut[256];
	sprintf(fileNameOut, "%s//%s", m_dirname, m_lettersFileName[w]);
	/*FILE *fp = fopen(fileNameOut, "wb");
	
	WriteBMP(fp,m_pDataOut,PIC24,m_output_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
	fclose(fp);
	*/

}

void HebrewItalic::CalculateBody()
{
	for (  int jj = 0; jj < m_height; jj++)
	{
		for ( int index = 0; index <= _NUMBER_CHANGES_TO_ANALYZE_; index++)
		{
			m_pChangesCoord[jj][index] = 0;
		}
	}
	memset(m_pBody, 0, m_width*m_height*sizeof(byte));
	
	for ( int i = m_begin_x-200; i< m_end_x+200; i++)
	{
		if ( i < 0) continue;
		if ( i >= m_width) continue;
		for (  int jj = m_begin_y; jj <= m_end_y; jj++)
		{
			if ( jj < 0 ) continue;
			if (jj >= m_height) continue;
			byte color = m_pDataIn[jj*m_width*3+i*3+0];
			byte prevColor = m_pDataIn[jj*m_width*3+(i-1)*3+0];
			byte prevprevColor = m_pDataIn[jj*m_width*3+(i-2)*3+0];


			if ((color == TextColor) && (prevColor != TextColor)&&(prevprevColor != TextColor))
			{
				bool goToNextPixel = false;
				m_pChangesCoord[jj][_NUMBER_CHANGES_TO_ANALYZE_] = i;
				if ((i -  m_pChangesCoord[jj][0]) < _NUMBER_CHANGES_TO_ANALYZE_*150)
				{
					for ( int ii = m_pChangesCoord[jj][0]; ii < m_pChangesCoord[jj][_NUMBER_CHANGES_TO_ANALYZE_]; ii++)
					{
						m_pBody[jj*m_width+ii] = 1;
					}
				}
				for ( int index = 1; index <= _NUMBER_CHANGES_TO_ANALYZE_; index++)
				{
					m_pChangesCoord[jj][index-1] = m_pChangesCoord[jj][index];
				}
				m_pChangesCoord[jj][_NUMBER_CHANGES_TO_ANALYZE_] = 0;

			}
		}
	}

	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		m_body[i] = 0;
		m_bodyHeight[i] = 0;
		for ( int j = m_begin_y; j <= m_end_y; j++)
		{
			if (m_pBody[j*m_width+i] ) 
			{
				m_body[i] = j;
				break;
			}
		}
		for ( int j = m_end_y; j >= m_begin_y; j--)
		{
			if (m_pBody[j*m_width+i]) 
			{
				m_bodyHeight[i] = j - m_body[i];
				break;
			}
		}
/*		for ( int j = m_body[i]; j <= m_body[i] + m_bodyHeight[i]; j++)
		{
			m_pDataOut[j*m_output_width*3 + i*3+0]  = m_pDataIn[j*m_width+ i];
			m_pDataOut[j*m_output_width*3 + i*3+1]  = m_pDataIn[j*m_width+ i];
			m_pDataOut[j*m_output_width*3 + i*3+2]  = m_pDataIn[j*m_width+ i];

		}

		for ( int j = m_body[i]; j <= m_body[i] + m_bodyHeight[i]; j++)
		{
			m_pDataOut[j*m_output_width*3 + i*3+0]  = 255;

		}*/
	}
	int ave = 0;
	int n = 0;
	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		if ( m_bodyHeight[i] > 0) 
		{
			ave += m_bodyHeight[i];
			n++;
		}
	}
	if (n)
		m_aveBodyHeight = ave/ n;
	m_MAX_HEBREW_S = m_aveBodyHeight*0.7;
	if (m_MAX_HEBREW_S > 30 || m_MAX_HEBREW_S < 10) m_MAX_HEBREW_S = 30;
	 ave = 0;
	int count = 0;
	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		if (m_body[i] > 0) 
		{
			ave += m_body[i];
			count++;
		}
	}
	//if (m_MAX_HEBREW_S > 25) m_MAX_HEBREW_S = 25;
	//m_begin_y_1 = ave/count;
	//m_end_y_1 = m_begin_y_1 + m_aveBodyHeight;

}

void HebrewItalic::StepItalic(void* dataIn, int dataInMode, void* pWorkData, int workDataMode, int beginWord, int endWord, int h1, int h2, int nSteps, int* &pWorkDataShifts)
{
	if (pWorkDataShifts) return;

	pWorkDataShifts = new int[m_height];
	memset(pWorkDataShifts, 0, m_height*sizeof(int));
	float sColumns = h2 - h1+1;
	if (nSteps != 0)
		sColumns = (float)(h2 - h1+1)/ (float) nSteps;
	sColumns = fabs(sColumns);
	float i_s_Columns = 0;
	int j1 = h2;
	int j2;
	//int *pDataShifted = m_pWorkData;
	if (pWorkData)
		memset(pWorkData, 0, m_width*m_height*3*sizeof(int));
	if (pWorkDataShifts)
		memset(pWorkDataShifts, 0, m_height*sizeof(int));
	int first, last, st;
	st = (nSteps < 0) ? -1 : 1;
	int shift = abs(nSteps);
	if (nSteps == 0) nSteps = 1;
	for ( int s = 0; s != nSteps; s+=st)
	{
		j2 = j1-1;
		i_s_Columns += sColumns;
		j1 = h2- (int)(i_s_Columns+0.5);
/*		while ((j1 > j2 ))
		{

			i_s_Columns += sColumns;
			j1 = h2- (int)(i_s_Columns+0.5);
			s+=st;
			if (s == nSteps) return;
		}
		*/
		 for ( int j = j1; j <= j2; j++)
		 {
			 if ( (j < 0) || (j >= m_height))
				 continue;

			/* if (pWorkData)
			 {
				 for ( int i = beginWord; i < endWord; i++)
				 {
					 ((int*)pWorkData)[j*m_output_width_x2+i+abs(nSteps)-s] = GetDataInValue(dataIn, dataInMode,  i, j, m_width, m_height);
					 
				 }
			 }*/
			 if (pWorkDataShifts)
			 {
					pWorkDataShifts[j] = s;
			 }
		 }
	}

}
