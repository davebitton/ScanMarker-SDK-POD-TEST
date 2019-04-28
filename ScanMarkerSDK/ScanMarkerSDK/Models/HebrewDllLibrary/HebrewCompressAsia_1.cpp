#ifdef _SCANMARKER_WINDOWS_
#include "StdAfx.h"
#include <shlobj.h>
#endif
#include "HebrewCompressAsia_1.h"
//#include "..\SplineInterpolation\SplineInterpolation.h"
#include "InOut.h"
#include "CheckFonts.h"
#include "xvbmp.h"
//#define _ENCRYPTED_
#ifdef _ENCRYPTED_
#include "lmpolicy.h"
#endif
#define TextColor 255 

#define APP_ENGINE_LNG_CHINESE_TRADITIONAL		221
#define APP_ENGINE_LNG_CHINESE_SIMPLIFIED		222
#define APP_ENGINE_LNG_HEBREW					230
CHebrewCompressAsia_1::CHebrewCompressAsia_1(void)
{
	m_pWorkData = NULL;
	m_pWorkData1 = NULL;
	m_pWorkData2 = NULL;
	m_pWorkData3 = NULL;
	m_width = 0;
	m_height = 0;
	m_pDataIn = NULL;
	m_pDataIn3 = NULL;
	m_pDataOut = NULL;
	m_begin_x = 0;
	m_end_x = 0;
	m_begin_y = 0;
	m_end_y = 0;
	m_widthHorizontal = NULL;
	m_widthVertical = NULL;
	m_widthPerChange = NULL;
	m_widthPerChange_1 = NULL;
	m_pInterpolationOutput = NULL; 
	m_pInterpolationInput = NULL;
	m_pInterpolationInput1 = NULL;
	m_pInterpolationOutput1 = NULL;
	m_pIndexData = NULL;
	m_debugFileBasicName[0] = 0;
	m_additionalFactor = 1.0;
	char f[1024]="";
#ifdef _SCANMARKER_WINDOWS_
    SHGetSpecialFolderPath(0, f, CSIDL_COMMON_APPDATA, false);
#else
#endif
	sprintf(b, "%s//Scanmarker//", f);



}
 CHebrewCompressAsia_1::~CHebrewCompressAsia_1()
{
		
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
		if (m_pWorkData3 != NULL)
		{
			delete[] m_pWorkData3;
			m_pWorkData3 = NULL;
		}
		if (m_pIndexData != NULL)
		{
			delete[] m_pIndexData;
			m_pIndexData = NULL;
		}

		if (m_pInterpolationOutput)
		{
			delete[] m_pInterpolationOutput;
			m_pInterpolationOutput= NULL;
		}
		if (m_pInterpolationOutput1)
		{
			delete[] m_pInterpolationOutput1;
			m_pInterpolationOutput1= NULL;
		}

		if (m_widthPerChange!= NULL)
		{
			delete[] m_widthPerChange;
			m_widthPerChange = NULL;
		}

		if (m_widthPerChange_1 != NULL)
		{
			delete[] m_widthPerChange_1;
			m_widthPerChange_1 = NULL;
		}

		
		if (m_pInterpolationInput)
		{
			delete[] m_pInterpolationInput;
			m_pInterpolationInput= NULL;
		}
		if (m_pInterpolationInput1)
		{
			delete[] m_pInterpolationInput1;
			m_pInterpolationInput1= NULL;
		}
		if (m_widthHorizontal)
		{
			delete[] m_widthHorizontal;
			m_widthHorizontal= NULL;
		}
		if (m_widthVertical)
		{
			delete[] m_widthVertical;
			m_widthVertical= NULL;
		}
		
}
void CHebrewCompressAsia_1::WriteDebugStr(char * str, bool isAppend)
{
return;
	FILE * fff = NULL;
	if (isAppend)
		fff = fopen("C://Temp//CHebrewCompressAsia_1Debug.txt", "at");
	else
		fff = fopen("C://Temp//CHebrewCompressAsia_1Debug.txt", "wt");

	if (fff)
	{
		fprintf(fff, "%s", str);
		fclose(fff);
	}
}
void CHebrewCompressAsia_1::AllocateMemory()
{
		m_width += 600;
		m_output_width += 600;

		
		
		if (m_widthPerChange!= NULL)
		{
			delete[] m_widthPerChange;
			m_widthPerChange = NULL;
		}
		m_widthPerChange = new float[m_output_width*2];
		for ( int i = 0; i < m_output_width*2; i++)
			m_widthPerChange[i] = 0.0;

		if (m_widthPerChange_1 != NULL)
		{
			delete[] m_widthPerChange_1;
			m_widthPerChange_1 = NULL;
		}
		m_widthPerChange_1 = new float[m_output_width*2];
		for ( int i = 0; i < m_output_width*2; i++)
			m_widthPerChange_1[i] = 0.0;



		
		if (m_pInterpolationOutput)
		{
			delete[] m_pInterpolationOutput;
			m_pInterpolationOutput= NULL;
		}
		m_pInterpolationOutput= new float[m_output_width*2];
		for ( int i = 0; i < m_output_width*2; i++)
			m_pInterpolationOutput[i] = 0.0;

		if (m_pInterpolationOutput1)
		{
			delete[] m_pInterpolationOutput1;
			m_pInterpolationOutput1= NULL;
		}
		m_pInterpolationOutput1= new float[m_output_width*2];
		for ( int i = 0; i < m_output_width*2; i++)
			m_pInterpolationOutput1[i] = 0.0;


		if (m_pInterpolationInput1)
		{
			delete[] m_pInterpolationInput1;
			m_pInterpolationInput1= NULL;
		}
		m_pInterpolationInput1= new float[m_output_width*2];
		for ( int i = 0; i < m_output_width*2; i++)
			m_pInterpolationInput1[i] = 0.0;

		
		if (m_pInterpolationInput)
		{
			delete[] m_pInterpolationInput;
			m_pInterpolationInput= NULL;
		}
		m_pInterpolationInput= new float[m_output_width*2];
		for ( int i = 0; i < m_output_width*2; i++)
			m_pInterpolationInput[i] = 0.0;
		
		
		
		
		
		if (m_pWorkData!= NULL)
		{
			delete[] m_pWorkData;
			m_pWorkData = NULL;
		}
		m_pWorkData = new byte[m_output_width*m_height*3*2];
		memset(m_pWorkData, 255-TextColor, m_output_width*m_height*3*2*sizeof(byte));
		


		


		
		if (m_pWorkData1!= NULL)
		{
			delete[] m_pWorkData1;
			m_pWorkData1 = NULL;
		}
		m_pWorkData1 = new byte[m_output_width*m_height*3*2];
		memset(m_pWorkData1, 255-TextColor, m_output_width*m_height*3*2*sizeof(byte));



		
		if (m_pWorkData3 != NULL)
		{
			delete[] m_pWorkData3;
			m_pWorkData3 = NULL;
		}
		m_pWorkData3 = new byte[m_output_width*m_height*3*3*2];
		memset(m_pWorkData3, 255-TextColor, m_output_width*m_height*3*3*2*sizeof(byte));
		
		if (m_pWorkData2!= NULL)
		{
			delete[] m_pWorkData2;
			m_pWorkData2 = NULL;
		}
		m_pWorkData2 = new byte[m_output_width*m_height*3*3];
		memset(m_pWorkData2, 255-TextColor, m_output_width*m_height*3*3*sizeof(byte));

		
		if (m_pIndexData!= NULL)
		{
			delete[] m_pIndexData;
			m_pIndexData= NULL;
		}
		m_pIndexData= new int[m_output_width*m_height*3*2];
		memset(m_pIndexData, 255-TextColor, m_output_width*m_height*3*2*sizeof(int));


		if (m_widthHorizontal != NULL)
		{
			delete[] m_widthHorizontal;
			m_widthHorizontal = NULL;
		}
		m_widthHorizontal = new float[m_output_width];
		for ( int i = 0; i < m_output_width; i++)
		{
			m_widthHorizontal[i] = 0.0;
		}

		if (m_widthVertical != NULL)
		{
			delete[] m_widthVertical;
			m_widthVertical = NULL;
		}
		m_widthVertical = new float[m_output_width];
		for ( int i = 0; i < m_output_width; i++)
		{
			m_widthVertical[i] = 0.0;
		}

		m_width -= 600;
		m_output_width -= 600;



}

void CHebrewCompressAsia_1::Init(bool corierActive,int width, int height, int outputWidth, byte* pDataIn, byte* pDataOut, byte* pDataIn3)
{
	m_width = width;
	m_output_width = outputWidth;
	m_height = height;
	m_pDataIn = pDataIn;
	m_pDataIn3 = pDataIn3;
	m_pDataOut = pDataOut;
	
	AllocateMemory();


}
void CHebrewCompressAsia_1::ReadCfgFile(char* dir)
{
	char fileName[255];
	sprintf(fileName, "%s//%s", dir, "text_is_out.cfg");
	//sprintf(fileName, "text_is_out.cfg");
	FILE * fff = fopen(fileName, "rt");
	if (!fff) return;

	char str[255];
	while (fgets(str, 254, fff))
	{
		if (str[0] == '#') continue;

	/*
		if ( strstr(str, "_UP_EDGE_"))
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
		}
	*/
	}
	fclose(fff);
	
}
void CHebrewCompressAsia_1::CalculateWidthLines()
{
	for ( int i = m_begin_x+300; i < m_end_x-300; i+=40)
	{
		int widthHorizontal[50];
		for ( int w = 0; w < 50; w++)
			widthHorizontal[w] = 0;

		for ( int j = 0; j < m_height; j++)
		{
			int isLine = -1;
			int beginLine = -1;
			for ( int ii = i-300; ii < i+300; ii++) 
			{
				if (isLine == -1 && m_pDataIn[j*m_width+ii] != TextColor)
				{
					isLine = 0;
					continue;
				}
				if (isLine == 0 && m_pDataIn[j*m_width+ii] == TextColor)
				{
					isLine = 1;
					beginLine = ii;
					continue;
				}
				if (isLine == 1 && m_pDataIn[j*m_width+ii] != TextColor)
				{
					isLine = 0;
					int w = ii - beginLine-3;
					if (m_language ==APP_ENGINE_LNG_HEBREW)
						w = ii - beginLine-1;
					if ( w < 50 && w > 0)
						widthHorizontal[w]++;
				}
			}
		}

		int widthVertical[50];
		for ( int w = 0; w < 50; w++)
			widthVertical[w] = 0;

		for ( int ii = i-300; ii < i+300; ii++) 
		{
			int isLine = -1;
			int beginLine = -1;
			for ( int j = 0; j < m_height; j++)
			{
				if (isLine == -1 && m_pDataIn[j*m_width+ii] != TextColor)
				{
					isLine = 0;
					continue;
				}
				if (isLine == 0 && m_pDataIn[j*m_width+ii] == TextColor)
				{
					isLine = 1;
					beginLine = j;
					continue;
				}
				if (isLine == 1 && m_pDataIn[j*m_width+ii] != TextColor)
				{
					isLine = 0;
					int w = j - beginLine-3;
					if (m_language ==APP_ENGINE_LNG_HEBREW)
						w = j - beginLine-1;
					if ( w < 50 && w > 0)
						widthVertical[w]++;
				}
			}
		}

		float widthVerticalResult = 0;
		float widthHorizontalResult = 0;
		{
			/*int n_allLines = 0;
			for ( int w = 0; w < 20; w++)
			{
				n_allLines += widthVertical[w];
			}
			int n_Lines = 0;
			for ( int w = 0; w < 20; w++)
			{
				n_Lines += widthVertical[w];
				if ( n_Lines > n_allLines / 2) 
				{
					widthVerticalResult = w;
					break;
				}
			}*/
			int maxWidth = 0;
			int maxWidthCount = 0;
			for ( int w = 1; w < 49; w++)
			{
				int count = widthVertical[w-1] + widthVertical[w] + widthVertical[w+1];
				if (count > maxWidthCount)
				{
					maxWidthCount = count;
					maxWidth = w;
				}
			}
			float ave = widthVertical[maxWidth-1]*(maxWidth-1) + widthVertical[maxWidth]*maxWidth + widthVertical[maxWidth+1]*(maxWidth+1);
			float c = widthVertical[maxWidth-1] + widthVertical[maxWidth] + widthVertical[maxWidth+1];
			if (c > 0)
				widthVerticalResult = ave / c;
		}
		{
			/*int n_allLines = 0;
			for ( int w = 0; w < 20; w++)
			{
				n_allLines += widthHorizontal[w];
			}
			int n_Lines = 0;
			for ( int w = 0; w < 20; w++)
			{
				n_Lines += widthHorizontal[w];
				if ( n_Lines > n_allLines / 2) 
				{
					widthHorizontalResult = w;
					break;
				}
			}
			*/
			int maxWidth = 0;
			int maxWidthCount = 0;
			for ( int w = 1; w < 49; w++)
			{
				int count = widthHorizontal[w-1] + widthHorizontal[w] + widthHorizontal[w+1];
				if (count > maxWidthCount)
				{
					maxWidthCount = count;
					maxWidth = w;
				}
			}
			float ave = widthHorizontal[maxWidth-1]*(maxWidth-1) + widthHorizontal[maxWidth]*maxWidth + widthHorizontal[maxWidth+1]*(maxWidth+1);
			float c = widthHorizontal[maxWidth-1] + widthHorizontal[maxWidth] + widthHorizontal[maxWidth+1];
			if (c > 3)
				widthHorizontalResult = ave / c;
		
		}
		 
		int i1 = i-20;
		int i2 = i+20;

		float languaheFactor = 1.25;
		if (m_isVertical)
			languaheFactor = 0.625;
		
		if (m_language ==APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
			m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED)
		{
			languaheFactor = 1.35;
			if (m_isVertical)
				languaheFactor = 0.48;
			else
			{
				languaheFactor *= m_additionalFactor;
			}
		
		}
		
		for ( int ii = i1; ii < i2; ii++)
		{
			if (widthHorizontalResult < 0.001)
				m_pInterpolationOutput[ii] = 0.0;
			else
				m_pInterpolationOutput[ii] =  widthVerticalResult / widthHorizontalResult *  languaheFactor;
			m_pInterpolationOutput1[ii] = widthHorizontalResult;
		}
	}
}
void CHebrewCompressAsia_1::CalculateWidthPerChange(int window, int step)
{
	memset(m_pWorkData, 0, m_output_width*m_height*sizeof(byte));
	memset(m_pWorkData1, 0, m_output_width*sizeof(byte));
	//memset(m_widthPerChange_1, 0, m_output_width*sizeof(float));
	for ( int i = 0; i < m_output_width; i++)
	{
		m_widthPerChange_1[i] = 0.0;
	}
	



	bool isText = false;
	for ( int i = m_begin_x+2; i< m_end_x; i++)
	{
			isText = false;
			for ( int j = m_begin_y;/*m_body[ii] + m_bodyHeight[ii];*/ j < m_end_y;/*m_body[ii] + m_bodyHeight[ii];*/ j++)
			{
				byte color = m_pDataIn[j*m_width+i];
				byte prevColor = m_pDataIn[j*m_width+i-1];
				byte prevprevColor = m_pDataIn[j*m_width+i-2];

				if (color == TextColor)
				{
					isText = true;
					//numberTextPixels++;
				}

				if ((color != prevColor ) && (color != prevprevColor))
				{
					m_pWorkData[j*m_width+i] = 1;
				}
				
			}

			if (isText)
			{
				m_pWorkData1[i] = 1;
			}
	}

	int numberColumnsEqual = 0;
	bool wasJerky = false;


	int lastSetIndex = 0;
	float lastSetValue = 1.0;
	for ( int i = m_begin_x-window; i< m_end_x; i+=step)
	{

			int i1 = i;
			int i2 = i+window;







		int iiii = 0;
		int numberChanges = 0;
		for (int ii = i1; ii < i2; ii++)
		{
			int ii1 = ii;
			if ( ii1 < m_begin_x)
					ii1 = m_begin_x + (m_begin_x - ii1);
				
			if ( ii1 >= m_width) continue;
			bool isText = false;
			for ( int j = m_begin_y;/*m_body[ii] + m_bodyHeight[ii];*/ j < m_end_y;/*m_body[ii] + m_bodyHeight[ii];*/ j++)
			{
				if (m_pWorkData[j*m_width+ii1])
					numberChanges++;
			}
			if (m_pWorkData1[ii1])
				iiii++;

		}
		if (numberChanges < 5) continue;
		float ratio = (float) (iiii) / (float) numberChanges;
		if (ratio > 6) continue;
		
		int i_b = (i1+i2)/2-step/2;
		int i_e = (i1+i2)/2+step/2;

		if ( (i_e < m_width) && (i_b >= 0))
		{
			for ( int i12 = i_b;i12 < i_e; i12++) 
				m_widthPerChange_1[i12] = ratio;
			lastSetIndex = i_e;
			lastSetValue = ratio;
		}
		
	}
	
}
	
void CHebrewCompressAsia_1::CalculateImageBoundaries()
{
	m_begin_x = m_width-1;
	m_end_x = 0;
	m_begin_y = 0;
	m_end_y = m_height-1;
	for ( int i = 52; i < m_width-30; i++)
	{
		bool found = false;
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				m_begin_x = i;
				found = true;
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
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				m_end_x = i;
				found = true;
				break;
			}
		}
		if (found) break;
	}

	if (m_end_x <= m_begin_x) return;

	for ( int j = 0; j < m_height; j++)
	{
		bool found = false;
		for ( int i = 51; i < m_width-20; i++)
		{
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				m_begin_y = j;
				found = true;
				break;
			}
		}
		if (found) break;
	}
	for ( int j = 0; j >= m_height ; j--)
	{
		bool found = false;
		for ( int i = 51; i < m_width-20; i++)
		{
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				m_end_y = j;
				found = true;
				break;
			}
		}
		if (found) break;
	}





	int newWidth = m_width;
	//if (m_begin_x < 300)
	{
		newWidth += 300;
	}
	//if (m_end_x > m_width - 300)
	{
		newWidth +=300;
	}
	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		for ( int j = m_begin_y; j < m_end_y; j++)
		{
			m_pWorkData1[j*newWidth+i+300] = m_pDataIn[j*m_width+i];
		}
	}
	m_width = newWidth;
	memcpy(m_pDataIn, m_pWorkData1, m_width*m_height*sizeof(byte));
	
	m_begin_x += 300;
	m_end_x += 300;

	for ( int i = 0; i < 300; i++)
	{
		for ( int j = m_begin_y; j < m_end_y; j++)
		{
			m_pDataIn[j*m_width+m_begin_x-i] = m_pDataIn[j*m_width+m_begin_x+i];
		}
	}

	for ( int i = 0; i < 300; i++)
	{
		for ( int j = m_begin_y; j < m_end_y; j++)
		{
			m_pDataIn[j*m_width+m_end_x+i] = m_pDataIn[j*m_width+m_end_x-i];
		}
	}

	m_begin_x -= 300;
	m_end_x += 300;


}
//unsigned int getMillisecond();
  
void CHebrewCompressAsia_1::Calculate(void *job, int* d, int* o)
{

	CalculateImageBoundaries();
	if (m_end_x <= m_begin_x) return;

	CalculateWidthLines();
	CalculateWidthPerChange(300,20);
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn[j*m_width+i];
		}
	}
	for ( int i = 0; i < m_width; i++)
	{
		m_widthPerChange[i] = m_pInterpolationOutput[i];
	}
	DrawOutput(m_pInterpolationOutput, 2, 255, -1, -1, 0, 20);
	DrawOutput(m_pInterpolationOutput1, 2, -1, 255, -1, 0, 20);

/*	if (m_isDebugMode)
	{
		char fn[1000];
		sprintf(fn, "%s_widthLines.bmp", m_debugFileBasicName);
		FILE* fp = fopen(fn, "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}
*/
	for ( int i = 0; i < m_width; i++)
	{
		m_widthPerChange[i] = m_pInterpolationOutput[i];
	}
	int flag = 5;
#ifdef _ENCRYPTED_
		byte n[128];
	for ( int j = 0; j < 128; j++)
		n[j] = 0;
	int i = 0; 
	int j = 0;
	int k = 0;
	while (i < 53)
	{
		byte aa = d[i] + o[j];
		n[k] = aa;
		i+= 3;
		j += 2;
		k++;
	}

	LP_HANDLE *lp_handle = NULL;
	LP_HANDLE *lp_handl  = NULL;
	char feature[3] = "f1";
	char featur [3] = "f2";
	char a[1024];
	sprintf(a, "%s//%s",b, (char*) n);
#endif
	DrawOutput(m_pInterpolationOutput, 2, 255, -1, -1, 0, 20);
	DrawOutput(m_pInterpolationOutput1, 2, -1, 255, -1, 0, 20);

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_width*3 + i*3 + 0] = 0;
			m_pDataOut[j*m_width*3 + i*3 + 1] = 0;
			m_pDataOut[j*m_width*3 + i*3 + 2] = 0;
		}
	}
#ifdef _ENCRYPTED_
	if (lp_checkout(LPCODE, LM_RESTRICTIVE, feature, "1.0", 1,(char*) a,/*(char*)n,*/ &lp_handle)&&
		lp_checkout(LPCODE, LM_RESTRICTIVE, featur , "1.0", 1,(char*) a,/*(char*)n,*/ &lp_handl )
		)
	{
		flag = 130;
	}
	else
#endif
	{
		flag = 128;
#ifdef _ENCRYPTED_
	if (lp_handl)
		lp_checkin(lp_handl);
	if (lp_handle)
		lp_checkin(lp_handle);
#endif
	}
#ifdef _ENCRYPTED_
	sprintf(featur, "00");
	sprintf(feature, "00");
	for ( int j = 0; j < 128; j++)
		n[j] =  d[j%40];
	for ( int j = 0; j < 1024; j++)
		a[j] =  d[j%40];
#endif
	Compress(false, flag);
	/*
	sprintf(fn, "%s_widthLines.bmp", m_debugFileBasicName);
	FILE* fp = fopen(fn, "wb");
	if (fp)
	{
		WriteBMP(fp,m_pDataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
		fclose(fp);
	}*/
	
	int i_beginData = 0;
	for ( int i = 1; i < m_output_width-100; i+= 100)
	{
		int numberChanges = 0;
		int ib = 0;
		for ( int ii = i; ii < i+100; ii++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (m_pDataOut[j*m_output_width*3+ii*3+0] != m_pDataOut[j*m_output_width*3+(ii-1)*3+0])
				{
					numberChanges++;
				}
			}
			if (numberChanges > 0)
			{
				ib = i-10;
			}
		}
		if (numberChanges > 100)
		{
			i_beginData = ib;
			break;
		}
	}
	int i_finishData = m_output_width-1;
	for ( int i = m_output_width-1; i > 100; i-= 100)
	{
		int numberChanges = 0;
		int ie = m_width - 1;
		for ( int ii = i-100; ii < i; ii++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (m_pDataOut[j*m_output_width*3+ii*3+0] != m_pDataOut[j*m_output_width*3+(ii-1)*3+0])
				{
					numberChanges++;
				}
			}
			if (numberChanges > 0)
			{
				ie = i+10;
			}
		}
		if (numberChanges > 100)
		{
			i_finishData = ie;
			break;
		}
	}
	for (int i = 0; i < i_beginData; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_output_width*3+i*3+0] = 0;
			m_pDataOut[j*m_output_width*3+i*3+1] = 0;
			m_pDataOut[j*m_output_width*3+i*3+2] = 0;
		}
	}
	for (int i = i_finishData; i < m_output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_output_width*3+i*3+0] = 0;
			m_pDataOut[j*m_output_width*3+i*3+1] = 0;
			m_pDataOut[j*m_output_width*3+i*3+2] = 0;
		}
	}

}


void CHebrewCompressAsia_1::Compress(int isTempOutput, int flag)
{
	int BgColr = 255-TextColor;
	if (isTempOutput == 1  )
	{
		memset(m_pWorkData, BgColr, m_output_width*m_height*sizeof(byte));
		memset(m_pIndexData, 0, m_output_width*sizeof(int));
		
	}
	if (isTempOutput == 2  )
	{
		memset(m_pWorkData2, BgColr, m_output_width*m_height*sizeof(byte));
		memset(m_pIndexData, 0, m_output_width*sizeof(int));
		
	}

	/*if (!isTempOutput && m_pDataIn3)
	{
		FILE* fp = fopen("C://Temp//debug00bmp", "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataIn3,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}*/

	int count = 0;
	float ave = 0;
	int numberInAverage = 40;
	int halfOfAve = numberInAverage / 2;
	for ( int i = m_begin_x; i < m_begin_x + numberInAverage; i++)
	{
		if (m_widthPerChange[i] <= 0.0001) continue;
		ave += m_widthPerChange[i];
		count++;
	}

	for ( int i = m_begin_x+halfOfAve; i < m_end_x; i++)
	{
		if (m_widthPerChange[i-halfOfAve] > 0.00001)
		{
			ave -= m_widthPerChange[i-halfOfAve];
			count--;
		}
		if (m_widthPerChange[i+halfOfAve]  > 0.00001)
		{
			ave += m_widthPerChange[i+halfOfAve];
			count++;
		}
		if (!count) count = 1;
		m_pInterpolationOutput[i] = ave / count;
	}
	flag-=4;

	for ( int i = m_begin_x+halfOfAve; i < m_end_x; i++)
	{
		m_widthPerChange[i] = m_pInterpolationOutput[i];
	}

	float ratio1 = (6.0-1.0)/(35.0-3.0);
	float ratio2 = (6.0-1.0)/(3.0-0.0);


	float offset1 = 1.0;
	float offset2 = 1.0;


	float nc = 0;
	int ncInt = 0;
	int prevNc = -1;
	int indOutput = m_begin_x;
	
	memset(m_pIndexData, 0, m_output_width*m_height*3*sizeof(int));
		
	int value11 = 255;

	for ( int i = m_begin_x; i <= m_end_x; i++)
	{
		if ( i >= m_width) break;
		if (indOutput >= m_output_width) 
			break;
		int k = i;
		if (fabs(m_widthPerChange_1[k]) < 0.005) continue;
		if (fabs(m_widthPerChange[k]) < 0.005) continue;
		if (fabs(m_widthPerChange[k]) > 100.0) continue;
		if (m_widthPerChange[k] < 0.0) continue;
		
		float value;// = (value2+value2) /  2;
		value = m_widthPerChange[k];
		float value2 = value;
		nc+= value2;
		ncInt = (int) nc;
		int add = ncInt - prevNc;
		if (add > 3)
		{
			prevNc = ncInt;
			add = 0;
		}
		if (i >= m_end_x-25)
			int a = 0;//add++;
		int end_x = m_end_x - 300;
		for ( int k = 0; k < add; k++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (isTempOutput )
					m_pWorkData[j*m_output_width+ indOutput] = m_pDataIn[j*m_width+i];
				else if ( flag == 124)
				{
					if ( i < m_begin_x + 300 || i > end_x ) continue;
				
					m_pDataOut [j*m_output_width*3+ indOutput*3 +0] = (m_pDataIn3) ? m_pDataIn3[j*m_width*3 + i*3+0]: m_pDataIn[j*m_width + i];
					m_pDataOut [j*m_output_width*3+ indOutput*3 +1] = (m_pDataIn3) ? m_pDataIn3[j*m_width*3 + i*3+1]: m_pDataIn[j*m_width + i];
					m_pDataOut [j*m_output_width*3+ indOutput*3 +2] = (m_pDataIn3) ? m_pDataIn3[j*m_width*3 + i*3+2]: m_pDataIn[j*m_width + i];
				}
				else
				{
					if ( i < m_begin_x + 300 || i > m_end_x - 300) continue;
				
					m_pDataOut [j*m_output_width*3+ indOutput*3 +0] = value11;
					m_pDataOut [j*m_output_width*3+ indOutput*3 +1] = value11;
					m_pDataOut [j*m_output_width*3+ indOutput*3 +2] = value11;
				}

			}
			m_pIndexData[indOutput] = i;
			indOutput++;
			if (indOutput >= m_output_width) break;
		}
		if (add)
			prevNc = ncInt;
	
	}

}


void CHebrewCompressAsia_1::DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue, float maxValue)
{
	float ratio_min = 10000;
	float ratio_max = 0;
	for ( int i = m_begin_x; i < m_end_x; i++)
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


		if (fabs(value)  > 1000)
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

	for ( int i = 0; i < m_width; i++)
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

		if (fabs(value)  > 1000)
		{
			continue;
		}
		
		int j = (int) (m_height - 1 - show_ratio * (value - ratio_min)) ;
		if ( j > m_height - 1) j = m_height - 1;
		if ( j < 0) j = 0;

		if ( red != -1)
			m_pDataOut[j*m_width*3 + i*3+0]  = red;

		if ( green != -1)
			m_pDataOut[j*m_width*3 + i*3+1]  = green;

		if ( blue != -1)
			m_pDataOut[j*m_width*3 + i*3+2]  = blue;

	}

}
