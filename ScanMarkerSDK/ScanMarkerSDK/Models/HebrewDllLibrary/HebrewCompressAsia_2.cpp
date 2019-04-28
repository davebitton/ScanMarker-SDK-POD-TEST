#include "HebrewCompressAsia_2.h"
//#include "..\SplineInterpolation\SplineInterpolation.h"
#include "InOut.h"
#include "CheckFonts.h"
#ifdef _SCANMARKER_WINDOWS_
#include "xvbmp.h"
#else
#include "bmp/xvbmp.h"
#endif
//#define _ENCRYPTED_
#ifdef _ENCRYPTED_
#include "lmpolicy.h"
#endif
#define TextColor 255 

#define APP_ENGINE_LNG_CHINESE_TRADITIONAL		221
#define APP_ENGINE_LNG_CHINESE_SIMPLIFIED		222
#define APP_ENGINE_LNG_HEBREW					230
#define APP_ENGINE_LNG_JAPANESE 				278
#define APP_ENGINE_LNG_KOREAN					232

#define LANGUAGE_FACTOR_HORIZONTAL 1.25
#define LANGUAGE_FACTOR_VERTICAL 0.625
#define CHINESE_LANGUAGE_FACTOR_HORIZONTAL 1.35
#define CHINESE_LANGUAGE_FACTOR_VERTICAL 0.58
#define JAPANESE_LANGUAGE_FACTOR_HORIZONTAL 1.13
#define JAPANESE_LANGUAGE_FACTOR_VERTICAL 0.93




CHebrewCompressAsia_2::CHebrewCompressAsia_2(void)
{
	m_pWorkData = NULL;
	m_pWorkData1 = NULL;
	m_pWorkData2 = NULL;
//	m_pWorkData3 = NULL;
	m_isEmptyColumn = NULL;
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
	m_pOppositeIndex = NULL;
	m_debugFileBasicName[0] = 0;
	m_additionalFactor = 1.0;
	
	m_isDuringFix = false;


}
 CHebrewCompressAsia_2::~CHebrewCompressAsia_2()
{
		
		if (m_pOppositeIndex != NULL)
		{
			delete[] m_pOppositeIndex;
			m_pOppositeIndex = NULL;
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
		/*if (m_pWorkData3 != NULL)
		{
			delete[] m_pWorkData3;
			m_pWorkData3 = NULL;
		}
		*/
		
		if (m_isEmptyColumn!= NULL)
		{
			delete[] m_isEmptyColumn;
			m_isEmptyColumn = NULL;
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
void CHebrewCompressAsia_2::WriteDebugStr(char * str, bool isAppend)
{
return;
	FILE * fff = NULL;
	if (isAppend)
		fff = fopen("C://Temp//CHebrewCompressAsia_2Debug.txt", "at");
	else
		fff = fopen("C://Temp//CHebrewCompressAsia_2Debug.txt", "wt");

	if (fff)
	{
		fprintf(fff, "%s", str);
		fclose(fff);
	}
}
void CHebrewCompressAsia_2::AllocateMemory()
{
		m_width += 600;
		m_output_width += 600;

		if (m_pOppositeIndex != NULL)
		{
			delete[] m_pOppositeIndex;
			m_pOppositeIndex = NULL;
		}
		m_pOppositeIndex = new int[m_output_width];
		
		
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



/*		
		if (m_pWorkData3 != NULL)
		{
			delete[] m_pWorkData3;
			m_pWorkData3 = NULL;
		}
		m_pWorkData3 = new byte[m_output_width*m_height*3*3*2];
		memset(m_pWorkData3, 255-TextColor, m_output_width*m_height*3*3*2*sizeof(byte));
*/		

		if (m_isEmptyColumn!= NULL)
		{
			delete[] m_isEmptyColumn;
			m_isEmptyColumn= NULL;
		}
		m_isEmptyColumn= new bool[m_output_width];;//m_height*3*3];
		for ( int i = 0; i < m_output_width; i++)
			m_isEmptyColumn[i] = false;
		
		if (m_pWorkData2!= NULL)
		{
			delete[] m_pWorkData2;
			m_pWorkData2 = NULL;
		}
		m_pWorkData2 = new float[m_output_width*2];;//m_height*3*3];
		//memset(m_pWorkData2, 255-TextColor, m_output_width*2*sizeof(byte));//*m_height*3*3*sizeof(byte));
		for ( int i = 0; i < m_output_width*2; i++)
		{
			m_pWorkData2[i] = 0.0;
		}
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
		m_widthHorizontal = new float[m_output_width*2];
		for ( int i = 0; i < m_output_width*2; i++)
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

void CHebrewCompressAsia_2::Init(bool corierActive,int width, int height, int outputWidth, byte* pDataIn, byte* pDataOut, byte* pDataIn3)
{
	m_width = width;
	m_output_width = outputWidth;
	m_height = height;
	m_pDataIn = pDataIn;
	m_pDataIn3 = pDataIn3;
	m_pDataOut = pDataOut;
	
	AllocateMemory();


}
void CHebrewCompressAsia_2::ReadCfgFile(char* dir)
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
void CHebrewCompressAsia_2::CalculateWidthLines()
{
	for ( int i = 0; i < m_width; i++)
	{
		m_pWorkData2[i] = -1;
	}
	int range = 300;
	if (m_language == APP_ENGINE_LNG_KOREAN)
		range = 150;
	for ( int i = m_begin_x+range; i < m_end_x-range-1; i+=40)
	{
		int widthHorizontal[50];
		for ( int w = 0; w < 50; w++)
			widthHorizontal[w] = 0;
		float widthHorizontalResult = 0;
		
		for ( int j = 0; j < m_height; j++)
		{
			int isLine = -1;
			int beginLine = -1;
			for ( int ii = i-range; ii < i+range; ii++) 
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
				if (isLine == 1 && m_pDataIn[j*m_width+ii] != TextColor && m_pDataIn[j*m_width+ii+1] != TextColor)
				{
					isLine = 0;
					int w = ii - beginLine;//-3;;
					if (m_language ==APP_ENGINE_LNG_KOREAN)
						w = ii - beginLine+2;
					if (m_language ==APP_ENGINE_LNG_HEBREW/* ||
					    m_language ==APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
						m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED*/)
						w = ii - beginLine;//-1;
					if ( w < 50 && w > 0)
						widthHorizontal[w]++;
				}
			}
		}

		int widthVertical[50];
		for ( int w = 0; w < 50; w++)
			widthVertical[w] = 0;

	/*	for ( int ii = i-300; ii < i+300; ii++) 
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
					int w = j - beginLine;//-3;
					if (m_language ==APP_ENGINE_LNG_HEBREW /*||
					    m_language ==APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
						m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED* /)
						w = j - beginLine;//-1;
					if ( w < 50 && w > 0)
						widthVertical[w]++;
				}
			}
		}
		
		float widthVerticalResult = 0;

		//float widthHorizontalResult = 0;
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
			}* /
			int maxWidth = 0;
			int maxWidthCount = 0;
			int allMeasures = 0;
			for ( int w = 1; w < 48; w++)
			{
				allMeasures += widthVertical[w];
				int count = /*widthVertical[w-2] + * /widthVertical[w-1] + widthVertical[w] + widthVertical[w+1]+ widthVertical[w+2];
				if (count > maxWidthCount)
				{
					maxWidthCount = count;
					maxWidth = w;
				}
			}
			*/
			/*
			int tenPerc = (allMeasures+5) / 10;
			if ( tenPerc < 5)
			{
				int i1 = i-20;
				int i2 = i+20;
				for ( int ii = i1; ii < i2; ii++)
				{
					m_pInterpolationOutput[ii] = 0.0;
				}
				continue;
			}
			int count = 0;
			for ( int w = 1; w < 25; w++)
			{
				count += widthVertical[w];
				if (count >= tenPerc)
				{
					widthVerticalResult = widthVertical[w]; 
				}
			}
			*/
		/*
			float ave = widthVertical[maxWidth-1]*(maxWidth-1) + widthVertical[maxWidth]*maxWidth + widthVertical[maxWidth+1]*(maxWidth+1);
			float c = widthVertical[maxWidth-1] + widthVertical[maxWidth] + widthVertical[maxWidth+1];
			/*if (maxWidth > 1)
			{
				ave += widthVertical[maxWidth-2]*(maxWidth-2);
				c+= widthVertical[maxWidth-2];
			}* /
			if (maxWidth < 48)
			{
				ave += widthVertical[maxWidth+2]*(maxWidth+2);
				c+= widthVertical[maxWidth+2];
			}
			if (c > 3)
				widthVerticalResult = ave / c;
			else
				widthVerticalResult = 0;
		
		}*/
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
			int maxWidth1 = 0;
			int maxWidthCount = 0;
			int allMeasures = 0;
			float percentage = 30.0;
			for ( int w = 2; w < 48; w++)
			{
				allMeasures += widthHorizontal[w];
			}
			float limit = (float) allMeasures*percentage / 100.0;
			allMeasures = 0;
			for ( int w = 1; w < 48; w++)
			{
				allMeasures += widthHorizontal[w];
				if ( allMeasures > limit)
				{
					maxWidth1 = w;
					break;
				}
			}
			for ( int w = 2; w < 48; w++)
			{
				int count =  /*widthHorizontal[w-2] + */widthHorizontal[w-1] + widthHorizontal[w] +  widthHorizontal[w+1];// + widthHorizontal[w+2];
				if (count > maxWidthCount)
				{
					maxWidthCount = count;
					maxWidth = w;
				}
				
			}
			if ( maxWidth < maxWidth1 && (maxWidth1 - maxWidth > 3))
			{
				/*int begin = maxWidth+1;
				maxWidth = 0;
				maxWidthCount = 0;
			
				for ( int w = begin; w < 48; w++)
				{
					int count =  /*widthHorizontal[w-2] + * /widthHorizontal[w-1] + widthHorizontal[w] +  widthHorizontal[w+1];// + widthHorizontal[w+2];
					if (count > maxWidthCount)
					{
						maxWidthCount = count;
						maxWidth = w;
					}
				
				}*/
				maxWidth = 0;
				
			}

			/*
			int tenPerc = (allMeasures+5) / 10;
			if ( tenPerc < 5)
			{
				int i1 = i-20;
				int i2 = i+20;
				for ( int ii = i1; ii < i2; ii++)
				{
					m_pInterpolationOutput[ii] = 0.0;
				}
				continue;
			}
			int count = 0;
			for ( int w = 1; w < 25; w++)
			{
				count += widthHorizontal[w];
				if (count >= tenPerc)
				{
					widthHorizontalResult = widthHorizontal[w]; 
				}
			}
			*/
			if (maxWidth >= 2)
			{
				float ave = widthHorizontal[maxWidth-1]*(maxWidth-1) + widthHorizontal[maxWidth]*maxWidth + widthHorizontal[maxWidth+1]*(maxWidth+1);
				float c = widthHorizontal[maxWidth-1] + widthHorizontal[maxWidth] + widthHorizontal[maxWidth+1];
				/*if (maxWidth > 1)
				{
					ave += widthHorizontal[maxWidth-2]*(maxWidth-2);
					c+= widthHorizontal[maxWidth-2];
				}*/
				/*if (maxWidth < 48)
				{
					ave += widthHorizontal[maxWidth+2]*(maxWidth+2);
					c+= widthHorizontal[maxWidth+2];
				}*/
				if (c > 3)
					widthHorizontalResult = ave / c;
				else
					widthHorizontalResult = 0;
			}
		}
		 
		int i1 = i-20;
		int i2 = i+20;

		/*
		float languaheFactor = LANGUAGE_FACTOR_HORIZONTAL;
		if (m_isVertical)
			languaheFactor = LANGUAGE_FACTOR_VERTICAL;
		
		if (m_isVertical && m_language == APP_ENGINE_LNG_JAPANESE)
			languaheFactor = JAPANESE_LANGUAGE_FACTOR_VERTICAL;
		
		if ((!m_isVertical) && m_language == APP_ENGINE_LNG_JAPANESE)
			languaheFactor = JAPANESE_LANGUAGE_FACTOR_HORIZONTAL;
		
		if (m_language ==APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
			m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED)
		{
			languaheFactor = CHINESE_LANGUAGE_FACTOR_HORIZONTAL;
			if (m_isVertical)
				languaheFactor =  CHINESE_LANGUAGE_FACTOR_VERTICAL;
			//else
			{
				languaheFactor *= m_additionalFactor;
			}
		
		}
		*/
		for ( int ii = i1; ii < i2; ii++)
		{
			/*if (widthHorizontalResult < 0.001 )//|| widthVerticalResult < 0.001)
				m_pWorkData1[ii] = 0.0;
			else
				m_pInterpolationOutput[ii] =  widthVerticalResult / widthHorizontalResult *  languaheFactor;
			*/
			
			m_pWorkData2[ii] = widthHorizontalResult;
		}
	}

	/*float lastValueBeforeZero = 0;
	int firstZeroindex = 0;
	for ( int i = 1; i < m_width; i++)
	{
		if (m_pInterpolationOutput[i - 1] > 0.1 && m_pInterpolationOutput[i] < 0.0001)
		{
			lastValueBeforeZero = m_pInterpolationOutput[i - 1];
			firstZeroindex = i;
		}
		if (m_pInterpolationOutput[i - 1] < 0.1 && (m_pInterpolationOutput[i] > 0.0001 || i >= m_width-1))
		{
			float valueToInterpolate = 0.0;
			if (lastValueBeforeZero > 0.0001 && m_pInterpolationOutput[i] > 0.0001)
			{
				valueToInterpolate += (lastValueBeforeZero + m_pInterpolationOutput[i]) / 2.0;
			}
			else if (lastValueBeforeZero > 0.0001)
			{
				valueToInterpolate = lastValueBeforeZero;
			}
			else if (m_pInterpolationOutput[i] > 0.0001)
			{
				valueToInterpolate = m_pInterpolationOutput[i];
			}
			for ( int ii = firstZeroindex; ii < i; ii++)
			{
				m_pInterpolationOutput[ii] = valueToInterpolate;
			}
		}
		 
	}
	*/
}
void CHebrewCompressAsia_2::CalculateWidthLinesKorean()
{
	for ( int i = 0; i < m_width; i++)
	{
		m_pWorkData2[i] = -1;
	}
	int	range = 100;
	for ( int l = 0; l < m_numberLetters; l++)
	{
		int middle = (m_letterEdges[l][2] + m_letterEdges[l][0])/2;
		
		int widthHorizontal[50];
		for ( int w = 0; w < 50; w++)
			widthHorizontal[w] = 0;
		float widthHorizontalResult = 0;
		
		for ( int j = 0; j < m_height; j++)
		{
			int isLine = -1;
			int beginLine = -1;
			for ( int ii = middle-range; ii < middle+range; ii++) 
			{
                if (ii < 0 || ii >= m_width-1) continue;
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
				if (isLine == 1 && m_pDataIn[j*m_width+ii] != TextColor && m_pDataIn[j*m_width+ii+1] != TextColor)
				{
					isLine = 0;
					int w = ii - beginLine+2;
					if ( w < 50 && w > 0)
						widthHorizontal[w]++;
				}
			}
		}

		int widthVertical[50];
		for ( int w = 0; w < 50; w++)
			widthVertical[w] = 0;

		int maxWidth = 0;
		int maxWidth1 = 0;
		int maxWidthCount = 0;
		int allMeasures = 0;
		float percentage = 30.0;
		for ( int w = 1; w < 48; w++)
		{
			allMeasures += widthHorizontal[w];
		}
		float limit = (float) allMeasures*percentage / 100.0;
		allMeasures = 0;
		for ( int w = 1; w < 48; w++)
		{
			allMeasures += widthHorizontal[w];
			if ( allMeasures > limit)
			{
				maxWidth1 = w;
				break;
			}
		}
		for ( int w = 3; w < 46; w++)
		{
			int count = widthHorizontal[w-2] + widthHorizontal[w-1] + 
				widthHorizontal[w] +  widthHorizontal[w+1] + widthHorizontal[w+2];// + widthHorizontal[w+3]  ;
			if (count > maxWidthCount)
			{
				maxWidthCount = count;
				maxWidth = w;
			}
				
		}
		if ( maxWidth < maxWidth1 && (maxWidth1 - maxWidth > 3))
		{
			maxWidth = 0;
		}
		
		if (maxWidth >= 3)
		{
			float ave = widthHorizontal[maxWidth-2]*(maxWidth-2) + widthHorizontal[maxWidth-1]*(maxWidth-1) + 
						widthHorizontal[maxWidth]*maxWidth + widthHorizontal[maxWidth+1]*(maxWidth+1) + widthHorizontal[maxWidth+2]*(maxWidth+2);
						 //+ widthHorizontal[maxWidth+3]*(maxWidth+3);
			float c = widthHorizontal[maxWidth-2] + widthHorizontal[maxWidth-1] + 
				widthHorizontal[maxWidth] + widthHorizontal[maxWidth+1] + widthHorizontal[maxWidth+2];// + widthHorizontal[maxWidth+3];
			if (c > 3)
				widthHorizontalResult = ave / c;
			else
				widthHorizontalResult = 0;
		}
	
		 
		int i1 = m_letterEdges[l][0]-5;
		int i2 = m_letterEdges[l][2]+5;
		
		for ( int ii = i1; ii < i2; ii++)
		{
			
			m_pWorkData2[ii] = widthHorizontalResult;
		}
		m_letterWidthLines[l] = widthHorizontalResult;
	}

}
void CHebrewCompressAsia_2::FixByWidthWordKorean()
{
	int letterEdgesKorean1[_MAX_NUMBER_LETTERS_][4];
	int letterEdgesKorean2[_MAX_NUMBER_LETTERS_][4];
	for ( int l = 0; l < m_numberLetters; l++)
	{
		int i1 = m_letterEdges[l][0];
		int i2 = m_letterEdges[l][2];
		int j1 = m_letterEdges[l][1];
		int j2 = m_letterEdges[l][3];
		
		letterEdgesKorean1[l][0] = m_pIndexData[i1];
		letterEdgesKorean1[l][2] = m_pIndexData[i2];
		letterEdgesKorean1[l][1] = m_letterEdges[l][1];
		letterEdgesKorean1[l][3] = m_letterEdges[l][3];
	}

	int widthLetter[_MAX_NUMBER_LETTERS_];
	int heightLetter[_MAX_NUMBER_LETTERS_];
	for ( int l = 0; l < m_numberLetters; l++)
	{
		widthLetter[l] = letterEdgesKorean1[l][2] - letterEdgesKorean1[l][0];
		heightLetter[l] = letterEdgesKorean1[l][3] - letterEdgesKorean1[l][1];
	}

	for ( int l = 0; l < m_numberLetters-1; l++)
	{
		for ( int l1 = l+1; l1 < m_numberLetters; l1++)
		{
			if (widthLetter[l] > widthLetter[l1])
			{
				int tmp = widthLetter[l];
				widthLetter[l] = widthLetter[l1];
				widthLetter[l1] = tmp;
			}
			if (heightLetter[l] > heightLetter[l1])
			{
				int tmp = heightLetter[l];
				heightLetter[l] = heightLetter[l1];
				heightLetter[l1] = tmp;
			}
		}
	}

	int rightWidth = widthLetter[m_numberLetters - 1];
	int rightheight = heightLetter[m_numberLetters - 1];
	
	if (m_numberLetters > 3)
	{
		rightWidth = widthLetter[m_numberLetters *3 / 4];
		rightheight = heightLetter[m_numberLetters  *3 / 4];
	
	}
	else
	{
		rightWidth = widthLetter[m_numberLetters *2 / 3];
		rightheight = heightLetter[m_numberLetters *2 / 3];
	
	}
	//rightWidth = 35;
	for ( int l = 0; l < m_numberLetters; l++)
	{
		widthLetter[l] = letterEdgesKorean1[l][2] - letterEdgesKorean1[l][0];
		heightLetter[l] = letterEdgesKorean1[l][3] - letterEdgesKorean1[l][1];
	}

	int l2 = 0;
	for ( int l = 1; l < m_numberLetters; l++)
	{
		int widthDoubleLetter = letterEdgesKorean1[l][2] - letterEdgesKorean1[l-1][0];
		int width1 = letterEdgesKorean1[l][2] - letterEdgesKorean1[l][0];
		int width2 = letterEdgesKorean1[l-1][2] - letterEdgesKorean1[l-1][0];
		//if (widthLetter[l] < rightWidth/2 && heightLetter[l] > rightheight*2 / 3 && heightLetter[l] < rightheight*4/3 && 
		if (widthDoubleLetter < rightWidth*2 && (width1 < rightWidth / 2 || width2 < rightWidth/2) &&
			heightLetter[l] > rightheight*2 / 3 && heightLetter[l] < rightheight*4/3 && 
			heightLetter[l-1] > rightheight*2 / 3 && heightLetter[l-1] < rightheight*4/3)
		{
			letterEdgesKorean2[l2][0] = letterEdgesKorean1[l-1][0];
			letterEdgesKorean2[l2][2] = letterEdgesKorean1[l][2];
			letterEdgesKorean2[l2][1] = (letterEdgesKorean1[l-1][1] < letterEdgesKorean1[l][1]) ? letterEdgesKorean1[l-1][1] : letterEdgesKorean1[l][1];
			letterEdgesKorean2[l2][3] = (letterEdgesKorean1[l-1][3] > letterEdgesKorean1[l][3]) ? letterEdgesKorean1[l-1][3] : letterEdgesKorean1[l][3];
			l2++;
            if (l2 >= m_numberLetters) break;
			l++;
            if (l >= m_numberLetters) break;
			continue;
		}
		letterEdgesKorean2[l2][0] = letterEdgesKorean1[l-1][0];
		letterEdgesKorean2[l2][2] = letterEdgesKorean1[l-1][2];
		letterEdgesKorean2[l2][1] = letterEdgesKorean1[l-1][1];
		letterEdgesKorean2[l2][3] = letterEdgesKorean1[l-1][3];
		l2++;
        if (l2 >= m_numberLetters) break;
		if ( l == m_numberLetters-1)
		{
			letterEdgesKorean2[l2][0] = letterEdgesKorean1[l][0];
			letterEdgesKorean2[l2][2] = letterEdgesKorean1[l][2];
			letterEdgesKorean2[l2][1] = letterEdgesKorean1[l][1];
			letterEdgesKorean2[l2][3] = letterEdgesKorean1[l][3];
			l2++;
            if (l2 >= m_numberLetters) break;
		}
	}

	m_numberLetters = l2;
	for ( int l = 0; l < m_numberLetters; l++)
	{
		int hh = letterEdgesKorean2[l][3] - letterEdgesKorean2[l][1];
		if ( hh < 25 || hh > 50) continue;
		int _i1 = letterEdgesKorean2[l][0];
		int _i2 = letterEdgesKorean2[l][2];
		
        if (_i1 < 0 || _i1 >= m_output_width || _i2 < 0 || _i2 >= m_output_width) continue;
		int i1 = m_pOppositeIndex[_i1];
		int i2 = m_pOppositeIndex[_i2];
        if (i1 < 0 || i1 >= m_width || i2 < 0 || i2 >= m_width) continue;
		i1+=2;
		while (!m_isEmptyColumn[i1] && i1 > 2) i1--;
		i1++;
		i2-=2;
		while (!m_isEmptyColumn[i2] && i2 < m_width-3) i2++;
		//i2--;
		if ( i1 < m_begin_x+300) i1 = m_begin_x+300;
		if ( i2 > m_end_x-300) i2 = m_end_x-300;
		float factor = (float) 35.0 / (float) (i2 - i1);
		for ( int i = i1-2; i <= i2+2; i++)
		{
			m_pInterpolationOutput[i] = factor*0.9;
			m_widthPerChange[i] = factor*0.9;
		}

		int i3, i4;
		if ( l > 0)
		{
			i3 = (m_pOppositeIndex[letterEdgesKorean2[l-1][2]] + m_pOppositeIndex[letterEdgesKorean2[l][0]]) / 2 + 1;
			i4 = i1 - 3;
			for ( int i = i3; i <= i4; i++)
			{
                if (i < 0 || i>= m_width) continue;
				m_pInterpolationOutput[i] = factor*1.0;
				m_widthPerChange[i] = factor*1.0;
			}
		}
		i3 = i2+3;
		if ( l < m_numberLetters - 1)
			i4 = (m_pOppositeIndex[letterEdgesKorean2[l][2]]+ m_pOppositeIndex[letterEdgesKorean2[l+1][0]]) / 2 - 1;
		for ( int i = i3; i <= i4; i++)
		{
            if (i < 0 || i>= m_width) continue;
			m_pInterpolationOutput[i] = factor*1.0;
			m_widthPerChange[i] = factor*1.0;
		}


	}





}
	
void CHebrewCompressAsia_2::CalculateWidthPerChange(int window, int step)
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
	
void CHebrewCompressAsia_2::CalculateImageBoundaries()
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
	if ( m_begin_x >= m_width-1) return;
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


	if (m_end_y <= m_begin_y) return;



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
/*
void CHebrewCompressAsia_2::FixInterpolationOutput(int i1, int i2, int j1, int j2)
{
	if ( i2 - i1 < 7) return;
	if ( j2 - j1 < 20) return;

	
	float languaheFactor = LANGUAGE_FACTOR_HORIZONTAL;
	if (m_isVertical)
		languaheFactor = LANGUAGE_FACTOR_VERTICAL;
		
	if (m_language ==APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
		m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED)
	{
		languaheFactor = CHINESE_LANGUAGE_FACTOR_HORIZONTAL;
		if (m_isVertical)
		languaheFactor =  CHINESE_LANGUAGE_FACTOR_VERTICAL;
		{
			languaheFactor *= m_additionalFactor;
		}
		
	}
	float ratio = (float) (j2-j1+1) / (float) ( i2 - i1+1) *  languaheFactor;
		
	float maxValue = 0;
	float minValue = 100.0;
	for ( int i = i1-2; i <= i2 + 2; i++)
	{
		if (m_pInterpolationOutput[i] < 0.001) continue;
		if ( m_pInterpolationOutput[i]  > maxValue)
			maxValue = m_pInterpolationOutput[i];
		if ( m_pInterpolationOutput[i]  < minValue)
			minValue = m_pInterpolationOutput[i];
	}
	float diffMin = ratio/minValue;
	float diffMax = ratio / maxValue;
	if ( diffMin > 0.5 && diffMin < 1.5 && diffMax > 0.5 && diffMax < 1.5)
	{
		for ( int i = i1-2; i <= i1 + 2; i++)
		{
			m_pInterpolationOutput[i] = ratio;
		}
	}
	


}
*/	

int CHebrewCompressAsia_2::Calculate(void *job, int* d, int* o)
{

	CalculateImageBoundaries();
	
	if (m_end_x <= m_begin_x) return -1000;
	/*{
		byte * debug = new byte[m_width*m_height*3];

		for( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				debug[j*m_width*3+i*3+0] = m_pDataIn[j*m_width+i];
				debug[j*m_width*3+i*3+1] = m_pDataIn[j*m_width+i];
				debug[j*m_width*3+i*3+2] = m_pDataIn[j*m_width+i];

			}
		}


		char fn[1000];
		sprintf(fn, "%s_temp1.bmp", m_debugFileBasicName);
		FILE* fp = fopen(fn, "wb");
		if (fp)
		{
			WriteBMP(fp,debug,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
		delete [] debug;
	}*/
	CalculateLetterEdgesVerticalAsia();
	if (m_language == APP_ENGINE_LNG_KOREAN)
	{
		CalculateWidthLinesKorean();
	}
	else
	{
		CalculateWidthLines();
	}
	//CalculateLetterEdgesVerticalAsia();
	if (m_numberLetters <= 2) return -1000; 
	CalculateCompressRatio();

	/*{
		byte * debug = new byte[m_width*m_height*3];

		for( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				debug[j*m_width*3+i*3+0] = m_pDataIn[j*m_width+i];
				debug[j*m_width*3+i*3+1] = m_pDataIn[j*m_width+i];
				debug[j*m_width*3+i*3+2] = m_pDataIn[j*m_width+i];

			}
		}


		char fn[1000];
		sprintf(fn, "%s_temp2.bmp", m_debugFileBasicName);
		FILE* fp = fopen(fn, "wb");
		if (fp)
		{
			WriteBMP(fp,debug,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
		delete [] debug;
	}*/

/*	if (m_language ==APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
			m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED)
		FixByWidthWordChina();
	else if (m_language ==APP_ENGINE_LNG_JAPANESE && (!m_isVertical))
		FixByWidthWordJapaneseHorizontal();
	else if (m_language ==APP_ENGINE_LNG_JAPANESE && (m_isVertical))
		FixByWidthWordJapaneseVertical();
*/

	

	CalculateWidthPerChange(300,20);
	/*for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn[j*m_width+i];
		}
	}*/
	/*for ( int i = 0; i < m_width; i++)
	{
		m_widthPerChange[i] = m_pInterpolationOutput[i];
	}*/
/*	DrawOutput(m_pInterpolationOutput, 2, 255, -1, -1, 0, 20);
	DrawOutput(m_pInterpolationOutput1, 2, -1, 255, -1, 0, 20);
*/
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
	/*for ( int i = 0; i < m_width; i++)
	{
		m_widthPerChange[i] = m_pInterpolationOutput[i];
	}*/
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
	/*DrawOutput(m_pInterpolationOutput, 2, 255, -1, -1, 0, 20);
	DrawOutput(m_pInterpolationOutput1, 2, -1, 255, -1, 0, 20);
	*/
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
	/*char fn[1000];
	sprintf(fn, "%s_afterCompress.bmp", m_debugFileBasicName);
	FILE* fp = fopen(fn, "wb");
	if (fp)
	{
		WriteBMP(fp,m_pDataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
		fclose(fp);
	}
	*/
	//remove jerky artifact from left and right
	if ( m_language == APP_ENGINE_LNG_KOREAN)
	{
		//Compress(true, flag);
		//FixByWidthWordKorean();
		Compress(true, flag);
		FixWidthByColorChange();
		Compress(false, flag);
		return 0;
	}

	Compress(false, flag);
	return 0;
	int i_beginData = 0;
	for ( int i = 1; i < m_output_width-101; i+= 100)
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
	for ( int i = m_output_width-1; i > 101; i-= 100)
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
	return 0;
}
void CHebrewCompressAsia_2::FixWidthByColorChange()
{

	int numberColorColumns = 0;
	int numberChanges = 0;
	for ( int i = 1; i < m_output_width-1; i++)
	{
		bool isEmptyColumn = true;
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pWorkData[j*m_output_width+ i] != m_pWorkData[j*m_output_width + i-1])
			{
				numberChanges++;
			}
			if ( m_pWorkData[j*m_output_width + i] == TextColor)
				isEmptyColumn = false;
		}
		if (!isEmptyColumn)
			numberColorColumns++;
	}
	
	float colorChangesFactor = (float) numberChanges / (float) numberColorColumns;
/*
	FILE* fff = fopen("H://DebugDina//korenColorChangesFactor.txt", "at");
	if (fff)
	{
		fprintf(fff, " \n %s : ColorChangesFactor = %f", m_debugFileBasicName, colorChangesFactor);
		fclose(fff);
	}
*/
	float factor = 1.0;
	if (colorChangesFactor < 4.0 || colorChangesFactor > 5.5)
	{
		factor = colorChangesFactor / 4.0;
	}

	for ( int i = 0; i < m_output_width; i++)
	{
		m_pInterpolationOutput[i] *= factor;
		m_widthPerChange[i] *= factor;
	}



}
void CHebrewCompressAsia_2::CalculateLetterEdgesVerticalAsia()
{
	m_numberLetters = 0;
	int i_begin = 10000000;
	int i_end = -1;
	int j_begin = 10000000;
	int j_end = -1;
	bool insideWord = false;
	int numberEmptyColumns = 0;
	int i_letter = 0;
	for ( int i = 0; i < m_output_width; i++)
		m_isEmptyColumn[i] = true;
			
	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		bool isEmptyColumn = true;
		for ( int j = m_begin_y; j < m_end_y; j++)
		{
			if ( m_pDataIn[j*m_width+i] == TextColor)
			{
				if ( i_begin > i) i_begin = i;
				if ( i_end < i) i_end = i;
				if ( j_begin > j) j_begin = j;
				if ( j_end < j) j_end = j;
				isEmptyColumn = false;
			}
		}
		if (isEmptyColumn)
		{
			m_isEmptyColumn[i] = true;
			numberEmptyColumns++;
		}
		else
		{
			m_isEmptyColumn[i] = false;
			numberEmptyColumns = 0;
		}
		int EMPTY_COLUMNS_BETWEEN_LETTERS = 6;
		if (m_language == APP_ENGINE_LNG_KOREAN)
			EMPTY_COLUMNS_BETWEEN_LETTERS = 8;
		if (insideWord && (numberEmptyColumns >= EMPTY_COLUMNS_BETWEEN_LETTERS || i >= m_end_x-1))
		//if (insideWord && (numberEmptyColumns >= 6 || i >= m_end_x-1))
		{
			if (i_end < m_begin_x + 300)
			{
				i_begin = 10000000;
				i_end = -1;
				j_begin = 10000000;
				j_end = -1;
				continue;
			}
			if ( i_begin < m_begin_x + 300)
			{
				i_begin = m_begin_x+300;
			}
			if ( i_end > m_end_x-300)
			{
				i_end = m_end_x - 300;
			}
			if (i_begin > m_end_x - 300) break;
			insideWord = false;
			
			m_letterEdges[i_letter][0] = i_begin;
			m_letterEdges[i_letter][1] = j_begin;
			m_letterEdges[i_letter][2] = i_end;
			m_letterEdges[i_letter][3] = j_end;
			if (i_letter < _MAX_NUMBER_LETTERS_-1)
				i_letter++;
			i_begin = 10000000;
			i_end = -1;
			j_begin = 10000000;
			j_end = -1;
		}
		else if (!insideWord && !isEmptyColumn)
		{
			insideWord = true;
		}
	}
	m_numberLetters = i_letter;
}
void CHebrewCompressAsia_2::CalculateCompressRatio()
{
	float unitsInLetter[_MAX_NUMBER_LETTERS_];
	float ratioHeightsToUnitsWidth[_MAX_NUMBER_LETTERS_];
	float interpolationRatio[_MAX_NUMBER_LETTERS_];
	//float newWidth[_MAX_NUMBER_LETTERS_];
	for ( int l = 0; l < m_numberLetters; l++)
	{
			float widthLines = 0;
			int numberLines = 0;
	
		if (m_language != APP_ENGINE_LNG_KOREAN)
		{
		/*	float values[1000];
			int numberValues = 0;
			for ( int i = m_letterEdges[l][0] ; i < m_letterEdges[l][2]; i++)
			{
				if (m_pWorkData2[i] > 0.0001 && numberValues < 999)
				{
					//widthLines += m_pWorkData2[i];
					//numberLines++;
					values[numberValues++] = m_pWorkData2[i];
				}
			}
			for ( int k1 = 0; k1 < numberValues-1; k1++)
			{
				for ( int k2 = k1+1; k2 < numberValues; k2++)
				{
					if ( values[k1] > values[k2])
					{
						float tmp = values[k1];
						values[k1] = values[k2];
						values[k2] = tmp;
					}
				}
			}
			int widthLetter = m_letterEdges[l][2] - m_letterEdges[l][0];
			if ( /*numberValues > 15 || * /numberValues >= (widthLetter / 3))
			{
				float aveWidthLines = (values[numberValues/2] + values[(numberValues+1)/2]) / 2.0;//widthLines / (float) numberLines;
				unitsInLetter[l] = (float) (m_letterEdges[l][2] - m_letterEdges[l][0]) / aveWidthLines;
				ratioHeightsToUnitsWidth[l] = (float) (m_letterEdges[l][3] - m_letterEdges[l][1]) / unitsInLetter[l];
			}
			else
			{
				unitsInLetter[l] = 0.0;
				ratioHeightsToUnitsWidth[l] = 0.0;
			}
*/
			for ( int i = m_letterEdges[l][0] ; i < m_letterEdges[l][2]; i++)
			{
                if (i < 0 || i >= m_width-1) continue;
				if (m_pWorkData2[i] > 0.0001)
				{
					widthLines += m_pWorkData2[i];
					numberLines++;
				}
			}

			int widthLetter = m_letterEdges[l][2] - m_letterEdges[l][0];
			if ( numberLines > 25 || numberLines >= (widthLetter / 3))
			{
				float aveWidthLines = widthLines / (float) numberLines;
				unitsInLetter[l] = (float) (m_letterEdges[l][2] - m_letterEdges[l][0]) / aveWidthLines;
				ratioHeightsToUnitsWidth[l] = (float) (m_letterEdges[l][3] - m_letterEdges[l][1]) / unitsInLetter[l];
			}
			else
			{
				unitsInLetter[l] = 0.0;
				ratioHeightsToUnitsWidth[l] = 0.0;
			}
		}
		else
		{
			unitsInLetter[l] = 0.0;
			ratioHeightsToUnitsWidth[l] = 0.0;
			if (m_letterWidthLines[l] > 0.001)
			{
				unitsInLetter[l] = (float) (m_letterEdges[l][2] - m_letterEdges[l][0]) / m_letterWidthLines[l];
				ratioHeightsToUnitsWidth[l] = (float) (m_letterEdges[l][3] - m_letterEdges[l][1]) / unitsInLetter[l];
			}
			
		}
	}



	int numberChanges[_MAX_NUMBER_LETTERS_];
	int numberColorPixels[_MAX_NUMBER_LETTERS_];
	for ( int l = 0; l < m_numberLetters; l++)
	{
		int numberChangesLocal = 0;
		int numberPixels = 0;
		int i1 = m_letterEdges[l][0];
		int i2 = m_letterEdges[l][2];
		int j1 = m_letterEdges[l][1];
		int j2 = m_letterEdges[l][3];

		for ( int j = j1; j < j2; j++)
		{
            if (j <0 || j >= m_height-1) continue;
			for (int i = i1-1; i < i2+1; i++)
			{
                if (i < 0 || i >= m_width-1) continue;
				if (m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i+1])
				{
					numberChangesLocal++;
				}
				if (m_pDataIn[j*m_width+i] == TextColor)
					numberPixels++;
			}
		}
		numberChanges[l] = numberChangesLocal;
		numberColorPixels[l] = numberPixels;
	}





	int middleLetter = m_numberLetters / 2;
	int indexMiddleLetter; 
	int widthMiddleLetter;
	int heightMiddleLetter;
	float ratioMiddleLetter;
	int indexSortedLetters[_MAX_NUMBER_LETTERS_];
	/*
	if (m_isVertical && m_language != APP_ENGINE_LNG_JAPANESE)
	{
		int heightLetters[_MAX_NUMBER_LETTERS_];
		for ( int l = 0; l < m_numberLetters; l++)
		{
			heightLetters[l] = m_letterEdges[l][3] - m_letterEdges[l][1];
			indexSortedLetters[l] = l;
		}
		for ( int l =  0; l < m_numberLetters-1; l++)
		{
			for ( int l1 = l+1; l1 < m_numberLetters; l1++)
			{
				if ( heightLetters[l] > heightLetters[l1])
				{
					int temp = heightLetters[l];
					heightLetters[l] = heightLetters[l1];
					heightLetters[l1] = temp;

					temp = indexSortedLetters[l];
					indexSortedLetters[l] = indexSortedLetters[l1];
					indexSortedLetters[l1] = temp;
				}
			}
		}
		indexMiddleLetter = indexSortedLetters[middleLetter]; 
		widthMiddleLetter = m_letterEdges[indexMiddleLetter][2] - m_letterEdges[indexMiddleLetter][0]+1;
		heightMiddleLetter = m_letterEdges[indexMiddleLetter][3] - m_letterEdges[indexMiddleLetter][1];
	}
	else
	*/
	//if ( m_language == APP_ENGINE_LNG_CHINESE_TRADITIONAL || m_language == APP_ENGINE_LNG_CHINESE_TRADITIONAL)
	/*if ( m_language == APP_ENGINE_LNG_KOREAN)// || m_language == APP_ENGINE_LNG_CHINESE_TRADITIONAL)
	{
		float unitsInWidthLetters[_MAX_NUMBER_LETTERS_];
		for ( int l = 0; l < m_numberLetters; l++)
		{
			unitsInWidthLetters[l] = unitsInLetter[l];;
			indexSortedLetters[l] = l;
		}
		for ( int l =  0; l < m_numberLetters-1; l++)
		{
			for ( int l1 = l+1; l1 < m_numberLetters; l1++)
			{
				if ( unitsInWidthLetters[l] > unitsInWidthLetters[l1])
				{
					float temp = unitsInWidthLetters[l];
					unitsInWidthLetters[l] = unitsInWidthLetters[l1];
					unitsInWidthLetters[l1] = temp;

					int tmp = indexSortedLetters[l];
					indexSortedLetters[l] = indexSortedLetters[l1];
					indexSortedLetters[l1] = tmp;
				}
			}
		}
		int firstWithValue = 0;
		while (firstWithValue < m_numberLetters && unitsInWidthLetters[firstWithValue] < 0.0001)
			firstWithValue++;
		middleLetter = firstWithValue + (m_numberLetters - firstWithValue) / 2;
		indexMiddleLetter = indexSortedLetters[middleLetter]; 
		widthMiddleLetter = m_letterEdges[indexMiddleLetter][2] - m_letterEdges[indexMiddleLetter][0]+1;
		heightMiddleLetter = m_letterEdges[indexMiddleLetter][3] - m_letterEdges[indexMiddleLetter][1]+1;
	}
	
	else*/
	{
	
		float ratioHeightsToUnitsWidthLetters[_MAX_NUMBER_LETTERS_];
		for ( int l = 0; l < m_numberLetters; l++)
		{
			ratioHeightsToUnitsWidthLetters[l] = ratioHeightsToUnitsWidth[l];;
			indexSortedLetters[l] = l;
		}
		for ( int l =  0; l < m_numberLetters-1; l++)
		{
			for ( int l1 = l+1; l1 < m_numberLetters; l1++)
			{
				if ( ratioHeightsToUnitsWidthLetters[l] > ratioHeightsToUnitsWidthLetters[l1])
				{
					float temp = ratioHeightsToUnitsWidthLetters[l];
					ratioHeightsToUnitsWidthLetters[l] = ratioHeightsToUnitsWidthLetters[l1];
					ratioHeightsToUnitsWidthLetters[l1] = temp;

					int tmp = indexSortedLetters[l];
					indexSortedLetters[l] = indexSortedLetters[l1];
					indexSortedLetters[l1] = tmp;
				}
			}
		}
		int firstWithValue = 0;
		while (firstWithValue < m_numberLetters && ratioHeightsToUnitsWidthLetters[firstWithValue] < 0.0001)
			firstWithValue++;
		middleLetter = firstWithValue + (m_numberLetters - firstWithValue) / 2;
		indexMiddleLetter = indexSortedLetters[middleLetter]; 
		widthMiddleLetter = m_letterEdges[indexMiddleLetter][2] - m_letterEdges[indexMiddleLetter][0];
		heightMiddleLetter = m_letterEdges[indexMiddleLetter][3] - m_letterEdges[indexMiddleLetter][1];

	}	
		

	int step = 1;
	int ml1 = middleLetter;
	while (heightMiddleLetter < 25 || numberChanges[indexMiddleLetter] < 3*heightMiddleLetter - 30 || widthMiddleLetter > 500 ||
		 unitsInLetter[indexMiddleLetter] > 30.0 || ((indexMiddleLetter < 2 || indexMiddleLetter > m_numberLetters - 2) && m_numberLetters > 4) ||
		 (unitsInLetter[indexMiddleLetter] < 5 ))
	{
		middleLetter = ml1+step;
		if (step < 0) step = -step + 1;
		else step = -step;
		if (middleLetter < 0 || middleLetter >= m_numberLetters)
		{
			middleLetter + ml1 + step;
			if (step < 0) step = -step + 1;
			else step = -step;
		
		}
		if (middleLetter < 0 || middleLetter >= m_numberLetters)
			break;

		indexMiddleLetter = indexSortedLetters[middleLetter]; 
		widthMiddleLetter = m_letterEdges[indexMiddleLetter][2] - m_letterEdges[indexMiddleLetter][0];
		heightMiddleLetter = m_letterEdges[indexMiddleLetter][3] - m_letterEdges[indexMiddleLetter][1];
		ratioMiddleLetter = (float) widthMiddleLetter / heightMiddleLetter;
	}
	if (middleLetter < 0 || middleLetter >= m_numberLetters)
	{
		for ( int i = 0; i < m_width; i++)
			m_pInterpolationOutput[i] = 1.0;
		return;
	}
	


	float usedWidthLetter = heightMiddleLetter;
	float numberUnitsInMiddleLetter = unitsInLetter[indexMiddleLetter];
	float usedWidthUnit = usedWidthLetter / numberUnitsInMiddleLetter;
	
	if (m_isVertical)
	{
		usedWidthUnit *= 1.1;
	}

	for ( int l = 0; l < m_numberLetters; l++)
	{


		float widthLetter = m_letterEdges[l][2] - m_letterEdges[l][0];

		float desiredWidth = usedWidthUnit * unitsInLetter[l];
		/*if (unitsInLetter[l] < 0.001)
		{
			if (m_isVertical)
				desiredWidth = (m_letterEdges[l][3] - m_letterEdges[l][1]+1) * 1.1;
			else
				desiredWidth = (m_letterEdges[l][3] - m_letterEdges[l][1]+1) * 0.9;

		}
*/
		/*if (m_isVertical && m_language == APP_ENGINE_LNG_JAPANESE)
		{
			//float percentage = fabs(unitsInLetter[indexMiddleLetter] - unitsInLetter[l])/unitsInLetter[indexMiddleLetter] ;
			float h1 = m_letterEdges[indexMiddleLetter][3] - m_letterEdges[indexMiddleLetter][1];
			float h2 = m_letterEdges[l][3] - m_letterEdges[l][1];

			float percentage = fabs(h2-h1) / h1;
			if (percentage < 0.3)
				desiredWidth = usedWidthUnit * unitsInLetter[indexMiddleLetter];
		}*/
		interpolationRatio[l]  = desiredWidth/ widthLetter;
		if (unitsInLetter[l] < 0.001)
			interpolationRatio[l] = 0.0;
	}
	for ( int l = 0; l < m_numberLetters; l++)
	{
		float interpolationRatio1 = 0.0;
		for ( int step = 0; step <= 2; step++)
		{
			int l1 = l + step;
			if (l1 >= 0 && l1 < m_numberLetters && interpolationRatio[l1] > 0.0001)
			{
				interpolationRatio1 = interpolationRatio[l1];
				break;
			}
			l1 = l - step;
			if (l1 >= 0 && l1 < m_numberLetters && interpolationRatio[l1] > 0.0001)
			{
				interpolationRatio1 = interpolationRatio[l1];
				break;
			}

		}
		if (interpolationRatio1 < 0.001)
		{
			interpolationRatio1 = ( m_letterEdges[l][3] - m_letterEdges[l][1]) / ( m_letterEdges[l][2] - m_letterEdges[l][0]);
		}
		float factor = 1.0;
		//if ( m_language == APP_ENGINE_LNG_CHINESE_TRADITIONAL && (!m_isVertical))
		//	factor = 0.9;
		//interpolationRatio1 *= factor;
		interpolationRatio[l] = interpolationRatio1;
		/*for ( int i = m_letterEdges[l][0]; i < m_letterEdges[l][2]; i++)
		{
			m_pInterpolationOutput[i] = m_widthPerChange[i] = interpolationRatio1;
		}*/
		
	}
	
	for ( int l = 0; l < m_numberLetters; l++)
	{
		if ((m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED ||
			m_language == APP_ENGINE_LNG_CHINESE_TRADITIONAL) && (!m_isVertical))
		{
			float widthLetter = m_letterEdges[l][2] - m_letterEdges[l][0];
			float newWidth = widthLetter*interpolationRatio[l];
			float heightLetter =  m_letterEdges[l][3] - m_letterEdges[l][1];
			float ratio = (float) newWidth / (float) heightLetter;
			if (ratio > 0.87 && ratio < 1.2)
			{
				interpolationRatio[l] = (float) heightLetter / (float) widthLetter;
			}
			if (ratio > 0.55 && ratio < 0.83)
			{
				interpolationRatio[l] = (float) heightLetter / (float) widthLetter * 0.7;
			}
			for ( int i = m_letterEdges[l][0]; i < m_letterEdges[l][2]; i++)
			{
                if (i < 0 || i >= m_width) continue;
				m_pInterpolationOutput[i] = m_widthPerChange[i] = interpolationRatio[l];
			}
		}
		else if ((m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED ||
			m_language == APP_ENGINE_LNG_CHINESE_TRADITIONAL) && (m_isVertical))
		{
			float widthLetter = m_letterEdges[l][2] - m_letterEdges[l][0];
			float newWidth = widthLetter*interpolationRatio[l];
			float heightLetter =  m_letterEdges[l][3] - m_letterEdges[l][1];
			float ratio = (float) newWidth / (float) heightMiddleLetter;
			if (ratio > 0.7 && ratio < 1.5)
			{
				interpolationRatio[l] = (float) heightMiddleLetter / (float) widthLetter;
			}
			for ( int i = m_letterEdges[l][0]; i < m_letterEdges[l][2]; i++)
			{
                if (i < 0 || i >= m_width) continue;
				m_pInterpolationOutput[i] = m_widthPerChange[i] = interpolationRatio[l];
			}
		}
		else if (m_language == APP_ENGINE_LNG_KOREAN && (!m_isVertical))
		{	
			for ( int i = m_letterEdges[l][0]; i <= m_letterEdges[l][2]; i++)
			{
				float ratio = 1.0;
                if (i < 0 || i >= m_width) continue;
				//if (m_pWorkData2[i] > 0.01 && m_pWorkData2[i] < 100 && unitsInLetter[l] > 0.001 && unitsInLetter[l] < 200.0) 
					//ratio = ((float)(m_letterEdges[l][2] - m_letterEdges[l][0])) / m_pWorkData2[i] / unitsInLetter[l];
				m_pInterpolationOutput[i] = m_widthPerChange[i] = interpolationRatio[l]*0.8*ratio;
			}
			if ( l > 0)
			{
				int i1 = (m_letterEdges[l-1][2] + m_letterEdges[l][0]) / 2;
				for ( int i = i1; i < m_letterEdges[l][0]; i++)
				{
                    if (i < 0 || i >= m_width) continue;
					float ratio = 1.0;
					//if (m_pWorkData2[i] > 0.01 && m_pWorkData2[i] < 100 && unitsInLetter[l] > 0.001 && unitsInLetter[l] < 200.0) 
						//ratio =  ((float)(m_letterEdges[l][2] - m_letterEdges[l][0])) / m_pWorkData2[i] / unitsInLetter[l];
					m_pInterpolationOutput[i] = m_widthPerChange[i] = interpolationRatio[l]*ratio*1.0;
				}
			}
			if ( l < m_numberLetters-1)
			{
				for ( int i = m_letterEdges[l][2]+1; i < m_letterEdges[l+1][0]; i++)
				{
					float ratio = 1.0;
                    if (i < 0 || i >= m_width) continue;
					//if (m_pWorkData2[i] > 0.01 && m_pWorkData2[i] < 100 && unitsInLetter[l] > 0.001 && unitsInLetter[l] < 200.0) 
						//ratio =  ((float)(m_letterEdges[l][2] - m_letterEdges[l][0])) / m_pWorkData2[i] / unitsInLetter[l];
					m_pInterpolationOutput[i] = m_widthPerChange[i] = interpolationRatio[l]*ratio*1.0;
				}
			}
		}
		else
		{	
			for ( int i = m_letterEdges[l][0]; i < m_letterEdges[l][2]; i++)
			{
                if (i < 0 || i >= m_width) continue;
				m_pInterpolationOutput[i] = m_widthPerChange[i] = interpolationRatio[l];
			}
		}
	}



}
void CHebrewCompressAsia_2::Compress(int isTempOutput, int flag)
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
	if (m_language == APP_ENGINE_LNG_KOREAN)
		numberInAverage = 2;
	int halfOfAve = numberInAverage / 2;
	for ( int i = m_begin_x; i < m_begin_x + numberInAverage; i++)
	{
		//if (m_widthPerChange[i] <= 0.0001) continue;
		if (m_widthPerChange[i] > 0.00001 && m_widthPerChange[i] < 100.0 )
		{
			ave += m_widthPerChange[i];
			count++;
		}
	}

	for ( int i = m_begin_x+halfOfAve; i < m_end_x; i++)
	{
		if (m_widthPerChange[i-halfOfAve] > 0.00001 && m_widthPerChange[i-halfOfAve] < 100.0 )
		{
			ave -= m_widthPerChange[i-halfOfAve];
			count--;
		} 
		if (m_widthPerChange[i+halfOfAve]  > 0.00001 && m_widthPerChange[i+halfOfAve] < 100.0)
		{
			ave += m_widthPerChange[i+halfOfAve];
			count++;
		}
		if (!count) continue;
		m_pInterpolationOutput[i] = ave / count;
	}
	flag-=4;
	float lastValue = 0.0;
	if ( m_language != APP_ENGINE_LNG_KOREAN)
	{
		for ( int i = m_begin_x+halfOfAve; i < m_end_x; i++)
		{
			if (m_pInterpolationOutput[i] > 0 && m_pInterpolationOutput[i] < 100.0)
				lastValue = m_widthPerChange[i] = m_pInterpolationOutput[i];
			else
				m_widthPerChange[i] = 0.0;//lastValue;
		}
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
		m_pIndexData[i] = indOutput;
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
			m_pOppositeIndex[indOutput] = i;

			indOutput++;
			m_pOppositeIndex[indOutput] = i;
			if (indOutput >= m_output_width) break;
		}
		if (add)
			prevNc = ncInt;
	
	}

}


void CHebrewCompressAsia_2::DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue, float maxValue)
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
