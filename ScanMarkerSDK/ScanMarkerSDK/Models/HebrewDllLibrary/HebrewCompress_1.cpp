#ifdef _SCANMARKER_WINDOWS_
#include "StdAfx.h"
#include <shlobj.h>
#endif
#include "HebrewCompress_1.h"
//#include "..\SplineInterpolation\SplineInterpolation.h"
#include "InOut.h"
#include "CheckFonts.h"
#include "xvbmp.h"
#define TextColor 255 
#define APP_ENGINE_LNG_KOREAN		232	
#define _DYNAMIC_WINDOW_ true

CHebrewCompress_1::CHebrewCompress_1(void)
{
	_UP_EDGE_  = 123;
	_DOWN_EDGE_  = 5;
	_NUMBER_CHANGES_TO_ANALYZE_ = 2;
	_REFERENCE_BODY_HEIGHT_ = 45;
	_DEBUG_COMPRESS_WIDTH_ = 50;
	_ONE_WORD_COMPRESS_FACTOR_ = 1.1;
	m_dataInMode = 1;
	m_aveBodyHeight = 35;
	m_doubleTextPixels = false;
	m_fontFactor = 1.0;
	m_isDoubleWidth = false;

	m_pInterpolationInput= NULL;
	m_pInterpolationInput1 = NULL;
	m_pWorkData = NULL;
	m_pWorkData1 = NULL;
	m_pWorkData2 = NULL;
	m_pIndexData = NULL;
	m_pWorkData3 = NULL;
	m_pWorkData4 = NULL;
	m_pWorkData5 = NULL;
	m_ColorsPerChanges = NULL;
	m_widthPerChange = NULL;
	m_widthPerChange1 = NULL;
	m_pBody = NULL;
	m_width = 0;
	m_height = 0;
	m_pDataIn = NULL;
	m_pDataIn3 = NULL;
	m_pDataOut = NULL;
	m_body = NULL;
	m_bodyHeight = NULL;
	m_begin_x = 0;
	m_end_x = m_width-1;
	m_begin_y = 0;
	m_end_y = m_height-1;
	m_To = m_begin_x;
	m_pInterpolationOutput= NULL;
	m_pInterpolationOutput1= NULL;
	m_checkFonts = NULL;
	m_compressFactor_In = 1.0;
	m_compressInterpolation = NULL;
	m_compressInterpolationOpposite = NULL;

	for ( int j = 0; j < _MAX_IMAGE_HEIGHT_; j++)
	{
		m_pChangesCoord[j] = NULL;
	}

	char f[1024]="";
#ifdef _SCANMARKER_WINDOWS_
    SHGetSpecialFolderPath(0, f, CSIDL_COMMON_APPDATA, false);
#else
#endif
	sprintf(b, "%s//Scanmarker//", f);



}
 CHebrewCompress_1::~CHebrewCompress_1()
{
		m_checkFonts = NULL;
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


		if (m_ColorsPerChanges != NULL)
		{
			delete[] m_ColorsPerChanges;
			m_ColorsPerChanges = NULL;
		}

		if (m_widthPerChange!= NULL)
		{
			delete[] m_widthPerChange;
			m_widthPerChange = NULL;
		}
		if (m_widthPerChange1 != NULL)
		{
			delete[] m_widthPerChange1;
			m_widthPerChange1 = NULL;
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
		if (m_pInterpolationOutput1)
		{
			delete[] m_pInterpolationOutput1;
			m_pInterpolationOutput1= NULL;
		}


		if (m_pWorkData != NULL)
		{
			delete[] m_pWorkData;
			m_pWorkData = NULL;
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
		if (m_pWorkData4 != NULL)
		{
			delete[] m_pWorkData4;
			m_pWorkData4 = NULL;
		}
		if (m_pWorkData5 != NULL)
		{
			delete[] m_pWorkData5;
			m_pWorkData5 = NULL;
		}
		
		if (m_pWorkData1 != NULL)
		{
			delete[] m_pWorkData1;
			m_pWorkData1 = NULL;
		}
		
		if (m_pIndexData != NULL)
		{
			delete[] m_pIndexData;
			m_pIndexData = NULL;
		}


}
void CHebrewCompress_1::WriteDebugStr(char * str, bool isAppend)
{
return;
	FILE * fff = NULL;
	if (isAppend)
		fff = fopen("C://Temp//CHebrewCompress_1Debug.txt", "at");
	else
		fff = fopen("C://Temp//CHebrewCompress_1Debug.txt", "wt");

	if (fff)
	{
		fprintf(fff, "%s", str);
		fclose(fff);
	}
}
void CHebrewCompress_1::AllocateMemory()
{
		m_width += 600;
		m_output_width += 600;

		if (m_ColorsPerChanges != NULL)
		{
			delete[] m_ColorsPerChanges;
			m_ColorsPerChanges = NULL;
		}
		m_ColorsPerChanges = new float[m_output_width];//*2];
		for ( int i = 0; i < m_output_width/**2*/; i++)
			m_ColorsPerChanges[i] = 0.0;


		if (m_widthPerChange!= NULL)
		{
			delete[] m_widthPerChange;
			m_widthPerChange = NULL;
		}
		m_widthPerChange = new float[m_output_width];//*2];
		for ( int i = 0; i < m_output_width/**2*/; i++)
			m_widthPerChange[i] = 0.0;

		if (m_widthPerChange1 != NULL)
		{
			delete[] m_widthPerChange1;
			m_widthPerChange1 = NULL;
		}
		m_widthPerChange1 = new float[m_output_width];//*2];
		for (int i = 0; i < m_output_width/**2*/; i++)
			m_widthPerChange1[i] = 0.0;
		if (m_pBody!= NULL)
		{
			delete[] m_pBody;
			m_pBody = NULL;
		}
		m_pBody = new byte[m_output_width*m_height];//*2];

		if (m_body!= NULL)
		{
			delete[] m_body;
			m_body = NULL;
		}
		m_body= new int[m_output_width];//*2];
		for ( int i = 0; i < m_output_width/**2*/; i++)
			m_body[i] = 0;


		if (m_bodyHeight != NULL)
		{
			delete[] m_bodyHeight;
			m_bodyHeight = NULL;
		}
		m_bodyHeight = new int[m_output_width];//*2];
		for ( int i = 0; i < m_output_width/**2*/; i++)
			m_bodyHeight[i] = 0;

		
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pChangesCoord[j] != NULL)
			{
				delete[] m_pChangesCoord[j];
				m_pChangesCoord[j] = NULL;
			}
			m_pChangesCoord[j] = new int[_NUMBER_CHANGES_TO_ANALYZE_+2];
		}

		if (m_pInterpolationOutput)
		{
			delete[] m_pInterpolationOutput;
			m_pInterpolationOutput= NULL;
		}
		m_pInterpolationOutput= new float[m_output_width];
		for ( int i = 0; i < m_output_width; i++)
			m_pInterpolationOutput[i] = 0.0;

		if (m_pInterpolationOutput1)
		{
			delete[] m_pInterpolationOutput1;
			m_pInterpolationOutput1= NULL;
		}
		m_pInterpolationOutput1= new float[m_output_width];
		for ( int i = 0; i < m_output_width; i++)
			m_pInterpolationOutput1[i] = 0.0;



		
		
		if (m_pWorkData!= NULL)
		{
			delete[] m_pWorkData;
			m_pWorkData = NULL;
		}
		m_pWorkData = new byte[m_output_width*m_height*3];
		memset(m_pWorkData, 255-TextColor, m_output_width*m_height*3*sizeof(byte));
		


		


		/*if (m_pWorkData5!= NULL)
		{
			delete[] m_pWorkData5;
			m_pWorkData5= NULL;
		}
		m_pWorkData5= new byte[m_output_width*m_height*3];
		memset(m_pWorkData5, 0, m_output_width*m_height*3*sizeof(byte));

		if (m_pWorkData4 != NULL)
		{
			delete[] m_pWorkData4;
			m_pWorkData4 = NULL;
		}
		m_pWorkData4 = new byte[m_output_width*m_height*3];
		memset(m_pWorkData4, 0, m_output_width*m_height*3*sizeof(byte));
*/
		if (m_pWorkData3 != NULL)
		{
			delete[] m_pWorkData3;
			m_pWorkData3 = NULL;
		}
		m_pWorkData3 = new byte[m_output_width*m_height*3];
		memset(m_pWorkData3, 255-TextColor, m_output_width*m_height*3*sizeof(byte));
		
	/*	if (m_pWorkData2!= NULL)
		{
			delete[] m_pWorkData2;
			m_pWorkData2 = NULL;
		}
		m_pWorkData2 = new byte[m_output_width*m_height*3];
		memset(m_pWorkData2, 255-TextColor, m_output_width*m_height*3*sizeof(byte));
*/
		if (m_pWorkData1!= NULL)
		{
			delete[] m_pWorkData1;
			m_pWorkData1 = NULL;
		}
		m_pWorkData1 = new byte[m_output_width*m_height*3];
		memset(m_pWorkData1, 255-TextColor, m_output_width*m_height*3*sizeof(byte));



		if (m_pIndexData!= NULL)
		{
			delete[] m_pIndexData;
			m_pIndexData= NULL;
		}
		m_pIndexData= new int[m_output_width*m_height*3];
		memset(m_pIndexData, 255-TextColor, m_output_width*m_height*3*sizeof(int));


		
		if (m_pInterpolationInput1)
		{
			delete[] m_pInterpolationInput1;
			m_pInterpolationInput1= NULL;
		}
		m_pInterpolationInput1= new float[m_output_width];
		for ( int i = 0; i < m_output_width; i++)
			m_pInterpolationInput1[i] = 0.0;

		
		if (m_pInterpolationInput)
		{
			delete[] m_pInterpolationInput;
			m_pInterpolationInput= NULL;
		}
		m_pInterpolationInput= new float[m_output_width];
		for ( int i = 0; i < m_output_width; i++)
			m_pInterpolationInput[i] = 0.0;
		m_width -= 600;
		m_output_width -= 600;


}

void CHebrewCompress_1::Init(bool corierActive,int width, int height, int outputWidth, byte* pDataIn, byte* pDataOut, byte* pDataIn3)
{
	m_corierActive = corierActive;
	m_width = width;
	m_output_width = outputWidth;
	m_height = height;
	m_pDataIn = pDataIn;
	m_pDataIn3 = pDataIn3;
	m_pDataOut = pDataOut;
	_UP_EDGE_  = height-1;
	_DOWN_EDGE_  = 0;
	
	AllocateMemory();


}
void CHebrewCompress_1::ReadCfgFile(char* dir)
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
	}
	fclose(fff);
	
}
void CHebrewCompress_1::CalculateImageBoundaries()
{
	m_begin_x = m_width-1;
	m_end_x = 0;
	m_begin_y = 0;
	m_end_y = m_height-1;
	for ( int i = 52; i < m_width-30; i++)
	{
		bool found = false;
		for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
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

	for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
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
	for ( int j = _UP_EDGE_; j >= _DOWN_EDGE_ ; j--)
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

	if ( m_end_x - m_begin_x < 1200)
	{
		for ( int i = m_begin_x; i < m_end_x; i++)
		{
			int ii = m_end_x + (m_end_x - i);
			if ( ii > m_width - 1) continue;
			for (int j = 0; j < m_height; j++)
				m_pDataIn[j*m_width+ii] = m_pDataIn[j*m_width+i];
		}
		m_isDoubleWidth = true;
		m_end_x += m_end_x - m_begin_x;
	}

	if (m_begin_x < 0 ) m_begin_x = 0;
	if (m_begin_x > m_width-1) m_begin_x = m_width-1;
	if (m_end_x < 0 ) m_end_x = 0;
	if (m_end_x > m_width-1) m_end_x = m_width-1;

	if (m_begin_y < 0 ) m_begin_y = 0;
	if (m_begin_y > m_height-1) m_begin_y = m_height-1;
	if (m_end_y < 0 ) m_end_y = 0;
	if (m_end_y > m_height-1) m_end_y = m_height-1;



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
			if (m_pDataIn3)
			{
				m_pWorkData3[j*newWidth*3+(i+300)*3+0] = m_pDataIn3[j*m_width*3+i*3+0];
				m_pWorkData3[j*newWidth*3+(i+300)*3+1] = m_pDataIn3[j*m_width*3+i*3+1];
				m_pWorkData3[j*newWidth*3+(i+300)*3+2] = m_pDataIn3[j*m_width*3+i*3+2];
			}
		}
	}
	m_width = newWidth;
	memcpy(m_pDataIn, m_pWorkData1, m_width*m_height*sizeof(byte));
	if (m_pDataIn3)
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pDataIn3[j*m_width*3+i*3+0] = 0;
				m_pDataIn3[j*m_width*3+i*3+1] = 0;
				m_pDataIn3[j*m_width*3+i*3+2] = 0;
			}
		}
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = m_begin_y; j < m_end_y; j++)
			{
				m_pDataIn3[j*m_width*3+i*3+0] = m_pWorkData3[j*m_width*3+i*3+0];
				m_pDataIn3[j*m_width*3+i*3+1] = m_pWorkData3[j*m_width*3+i*3+1];
				m_pDataIn3[j*m_width*3+i*3+2] = m_pWorkData3[j*m_width*3+i*3+2];
			}
		}
	}
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

	if (m_begin_x < 0 ) m_begin_x = 0;
	if (m_begin_x > m_width-1) m_begin_x = m_width-1;
	if (m_end_x < 0 ) m_end_x = 0;
	if (m_end_x > m_width-1) m_end_x = m_width-1;

}
//unsigned int getMillisecond();

void CHebrewCompress_1::Calculate(void *job, int* d, int* o)
{
//	int tStart, t1, t2, t3, t3_0, t3_1, t4, t5, t6, t6_0, t6_1, t7, t7_0, t7_1, t8, t9,t9_0, t9_1, t10;

//	tStart = getMillisecond();
	m_To = m_begin_x;
	CalculateImageBoundaries();
	if (m_end_x <= m_begin_x) return;
//	t1 = getMillisecond();
	CalculateBody();
//	t2 = getMillisecond();
	
	/*{
		CIdealImage idealIimage;
		//fnOut = fnOutSrc +"_ideal_image_1_polynom.bmp";
		idealIimage.Init(861, m_height, m_output_width, m_pDataIn, m_pDataOut);
		idealIimage.Calculate();
	}*/




	//CalculateWidthPerChange();
	//CalculateColorPerChanges();


	/*{
		SplineInterpolation splineInterpolation(m_ColorsPerChanges, m_pInterpolationOutput, m_width, m_height, 2, 2);
		for ( int i = 0; i < m_width; i++)
			m_ColorsPerChanges[i] = m_pInterpolationOutput[i];

		//DrawOutput(m_ColorsPerChanges,2,255,0,0, 3, 35);
	}*/
	{
		for ( int i = 0; i < m_output_width; i++)
		{
			m_widthPerChange[i] = m_widthPerChange1[i]  = m_pInterpolationOutput[i] = 0.0;
		}
		CalculateWidthPerChange(300,20);
		CalculateWidthPerChanges1(1000, 20);
	
//		Compress(false);return;
		//return;
		for ( int i = 0; i < m_output_width; i++)
		{
			m_pInterpolationOutput[i] = m_pInterpolationInput[i] = m_widthPerChange[i];
		}
		/*t3_0 = getMillisecond();
		SplineInterpolation splineInterpolation(m_widthPerChange, m_pInterpolationOutput, m_output_width, m_height, 2, 2);
		t3_1 = getMillisecond();
		for ( int i = 0; i < m_output_width; i++)
			m_widthPerChange[i] = m_pInterpolationOutput[i];*/
		//DrawOutput(m_widthPerChange,2,0,255,0, 0, 4);
		//return;
	}

float ave = 0;
	Compress(true);



		float factor10 = CalculateFactor_3();
	
		for ( int i = 0; i < m_output_width; i++)
			m_pInterpolationOutput[i] = m_widthPerChange[i] = m_pInterpolationInput[i]*1.5*3.0/factor10/m_compressFactor_In;
		if ( m_language == APP_ENGINE_LNG_KOREAN )
		{
			for ( int i = 0; i < m_output_width; i++)
				m_pInterpolationOutput[i] = m_widthPerChange[i] = m_pInterpolationInput[i]*1.75*3.0/factor10/m_compressFactor_In;
		}

		m_fontFactor = 1.0;

	if (m_checkFonts)
	{
		Compress(true);
		WriteDebugStr("\naaaaa m_checkFonts->m_fontsToCheck", (long)m_checkFonts->m_fontsToCheck );
		m_checkFonts->Init(m_output_width, m_height, m_pWorkData );
		for ( int i = 0; i < m_output_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_checkFonts->m_pDataIn[j*m_output_width+i] = m_pWorkData[j*m_output_width+i];
			}
		}
		WriteDebugStr("\nbbbbb m_checkFonts->m_fontsToCheck", (long)m_checkFonts->m_fontsToCheck );
		m_checkFonts->Calculate();
		WriteDebugStr("cccccc");
		m_fontFactor = m_checkFonts->m_factorToApply; 
		
//		t6_0 = getMillisecond();
		//SplineInterpolation splineInterpolation(m_pInterpolationInput, m_pInterpolationOutput, m_output_width, m_height, 2, 2);
//		t6_1 = getMillisecond();
		for ( int i = 0; i < m_output_width; i++)
			m_pInterpolationOutput[i] = m_widthPerChange[i] = m_pInterpolationInput[i]*1.5*3.0/factor10/m_fontFactor;
	}



	
	
	
	
	//	t6 = getMillisecond();
	//DrawOutput(m_widthPerChange,2,255,0,0, 0, 6);return;
	//Compress(false);return;
	Compress(false);

	CalculateFactor_2(true);
	if (m_corierActive)
	{
		//if (m_aveBgToTExtRatio < 85.0 && m_minWidth < 3.5)
		if (m_minWidth < 1.5)
			AddWidthTCorierFont();
	}
	
	return;
	CalculateFactor_3()	;
//	t7 = getMillisecond();

	{
//		t7_0 = getMillisecond();
		
		//SplineInterpolation splineInterpolation(m_pInterpolationInput, m_pInterpolationOutput, m_output_width, m_height, 2, 2);
//		t7_1 = getMillisecond();
		for ( int i = 0; i < m_output_width; i++)
			m_pInterpolationOutput[i] = m_widthPerChange[i] = m_pInterpolationInput[i];
	}
	CalculateFactor_2(true);
//	t7 = getMillisecond();

	float factor5 = 1.1;
	
	if (m_aveBgToTExtRatio <= 40.0) factor5 = 1.12;
	else if (m_aveBgToTExtRatio <= 45.0) factor5 = 1.107;
	else if (m_aveBgToTExtRatio <= 50.0) factor5 = 1.103;
	else if (m_aveBgToTExtRatio <= 55.0) factor5 = 1.1;
	else if (m_aveBgToTExtRatio <= 60.0) factor5 = 1.085;
	else if (m_aveBgToTExtRatio <= 65.0) factor5 = 1.07;
	else if (m_aveBgToTExtRatio <= 70.0) factor5 = 0.95; // 1.06 raw 56
	else if (m_aveBgToTExtRatio <= 75.0) factor5 =  0.95;
	else if (m_aveBgToTExtRatio <= 80.0) factor5 = 0.95;
	else if (m_aveBgToTExtRatio <= 85.0) factor5 = 0.9;
	else factor5 = 0.8;
	if ( m_aveAve < 2.5 )
		m_doubleTextPixels = true;
	else
		m_doubleTextPixels = false;


	{
//		t9_0 = getMillisecond();

		//SplineInterpolation splineInterpolation(m_pInterpolationInput, m_pInterpolationOutput, 1, m_height, 2, 2);
//		t9_1 = getMillisecond();
		for ( int i = 0; i < m_output_width; i++)
			m_pInterpolationOutput[i] = m_widthPerChange[i] = m_pInterpolationInput[i]*factor5;
	}
	//Compress(true);
	//CalculateFactor_2(true);
	ave = 0;
	for ( int i = 0; i < 100; i++)
	{
		ave += m_widthPerChange[i];
	}

	for ( int i = 1; i < m_output_width-100; i++)
	{
		ave -= m_widthPerChange[i-1];
		ave += m_widthPerChange[i+100];
		m_pInterpolationOutput[i+50] = ave / 100.0;
	}
	for ( int i = 0; i < m_output_width; i++)
	{
		m_widthPerChange[i] = m_pInterpolationOutput[i];
	}
	//DrawOutput(m_pInterpolationOutput,2,255,0,0);
	//return;
	Compress(false);

//	t9 = getMillisecond();

	{
	
/*		int all = t9 - tStart;
		t9_1 -= t9_0;
		t9 -= t8;
		t8 -= t7;
		t7_1 -= t7_0;
		t7 -= t6;
		t6_1 -= t6_0;
		t6 -= t5;
		t5 -= t4;
		t4 -= t3;
		t3_1 -= t3_0;
		t3 -= t2;
		t2 -= t1;
		t1 -= tStart;
/*		FILE *fff1 = fopen("C:\\Temp\\TextImageTime.txt", "at");
		if ( fff1)
		{
			SYSTEMTIME currentTime;
			GetSystemTime(&currentTime);
			fprintf(fff1, "\n\n HebrewCompress\nt1=%ld \nt2=%ld \nt3_1 - t3_0=%ld \nt3=%ld \nt4=%ld \nt5=%ld \nt6_1 - t6_0=%ld  \nt6=%ld \nt7_1 - t7_0=%ld  \nt7=%ld \nt8=%ld \nt9_1 - t9_0=%ld  \nt9=%ld\nall=%ld\n\n", 
				t1, t2, t3_1,t3,  t4, t5, t6_1, t6, t7_1, t7, t8, t9_1, t9, all);
		}
*/
	}

}

void CHebrewCompress_1::AddWidthTCorierFont()
{
	memset(m_pWorkData1, 0, m_output_width);
	for ( int i = 0; i < m_output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if ( m_pDataOut[m_output_width*j*3 + i*3 + 0] == TextColor)
			{
				m_pWorkData1[i] = 1;
				break;
			}
		}
	}
	for ( int i = 0; i < m_output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[m_output_width*j*3 + i*3 + 1] = 0;
			m_pDataOut[m_output_width*j*3 + i*3 + 2] = 0;
			
		}
	}
	//memset(m_pWorkData2, 0, m_output_width*m_height);
	for ( int i = 2; i < m_output_width-2; i++)
	{
		for ( int j = 2; j < m_height-2; j++)
		{
			if ( m_pDataOut[m_output_width*j*3 + i*3 + 0] == TextColor)
			{
				if ( m_pWorkData1[i-1] == 0 && m_pWorkData1[i+1] == 1 )
				{
					m_pDataOut[m_output_width*(j-1)*3 + (i+2)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j-1)*3 + (i)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j-1)*3 + (i+1)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j)*3 + (i+2)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j)*3 + (i)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j)*3 + (i+1)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j+1)*3 + (i+2)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j+1)*3 + (i)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j+1)*3 + (i+1)*3 + 1] = TextColor;
				}
				else if ( m_pWorkData1[i-1] == 1 && m_pWorkData1[i+1] == 0 )
				{
					m_pDataOut[m_output_width*(j-1)*3 + (i-2)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j-1)*3 + (i)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j-1)*3 + (i-1)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j)*3 + (i-2)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j)*3 + (i)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j)*3 + (i-1)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j+1)*3 + (i-2)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j+1)*3 + (i)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j+1)*3 + (i-1)*3 + 1] = TextColor;
				}
				else
				{
					m_pDataOut[m_output_width*(j-1)*3 + (i+1)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j-1)*3 + (i)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j-1)*3 + (i-1)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j)*3 + (i+1)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j)*3 + (i)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j)*3 + (i-1)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j+1)*3 + (i+1)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j+1)*3 + (i)*3 + 1] = TextColor;
					m_pDataOut[m_output_width*(j+1)*3 + (i-1)*3 + 1] = TextColor;
				}

			}
		}
	}

	for ( int i = 0; i < m_output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[m_output_width*j*3 + i*3 + 0] = m_pDataOut[m_output_width*j*3 + i*3 + 1];
			m_pDataOut[m_output_width*j*3 + i*3 + 2] = m_pDataOut[m_output_width*j*3 + i*3 + 1];
			
		}
	}





}
float CHebrewCompress_1::CalculateFactor_3()
{
	for ( int i = 0; i <m_output_width; i++)
		m_pInterpolationOutput[i] = 0.0;

	m_begin_x_1 = 0;
	m_end_x_1 = m_output_width-1;
	for ( int i = 52; i < m_output_width-30; i++)
	{
		bool found = false;
		for ( int j = 8; j < m_height-8; j++)
		{
			if (m_pWorkData[j*m_output_width+i] == TextColor)
			{
				m_begin_x_1 = i;
				found = true;
				break;
			}
		}
		if (found) break;
	}
 	for ( int i = m_output_width-20; i >= 51; i--)
	{
		bool found = false;
		for ( int j = 8; j < m_height-8; j++)
		{
			if (m_pWorkData[j*m_output_width+i] == TextColor)
			{
				m_end_x_1 = i;
				found = true;
				break;
			}
		}
		if (found) break;
	}
	m_numberTextColumns = 0;
	int j_f = (m_end_y - m_begin_y) / 2;
	for ( int i = m_begin_x_1; i < m_end_x_1; i++)
	{
		for ( int j = 0; j < j_f; j++)
		{
			int jj = j + m_begin_y;
			if (IsText(m_pWorkData[jj*m_output_width+i], TextColor))
			{
				m_numberTextColumns++;
				break;
			}
			jj = m_end_y - j;
			if (IsText(m_pWorkData[jj*m_output_width+i], TextColor))
			{
				m_numberTextColumns++;
				break;
			}
		}
	}

	int numberChanges = 0;
	for( int i = 1; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if ( m_pWorkData[j*m_output_width+i] != m_pWorkData[j*m_output_width+i+1] )
			{
				numberChanges++;
			}
		}
	}
	float factor = (float) numberChanges / (float)m_numberTextColumns;
	if (m_begin_y >= m_end_y) return 1;
	if ((numberChanges / (m_end_y - m_begin_y)) < 200)
	{
		factor /= _ONE_WORD_COMPRESS_FACTOR_;
	}
	else
		factor /= 1.1;
	return factor;
	for ( int i = 0; i <m_output_width; i++)
		m_pInterpolationOutput[i] = 0.0;

	for ( int i = 0; i <m_output_width; i++)
	{
		int i1 = m_pIndexData[i];
		int i2 = m_pIndexData[i+1];
		for ( int ii1 = i1; ii1 < i2; ii1++)
		{
			if (!m_numberTextColumns) m_numberTextColumns = 1;
			if (!m_numberChangesAllAve) m_numberChangesAllAve = 1;
			m_pInterpolationOutput[ii1] = m_pInterpolationInput[ii1]*2.3/(((float)m_numberChangesAllAve/(float)m_numberTextColumns)/**(float)m_aveBodyHeight*/);///((float)m_numberChangesAllAve*m_aveBodyHeight);
		}	}
	for ( int i = 0; i <m_output_width; i++)
		m_pInterpolationInput[i] = m_pInterpolationOutput[i];
	for ( int i = 0; i <m_output_width; i++)
		m_pInterpolationOutput[i] = 0.0;
}
void CHebrewCompress_1::CalculateFactor_2(bool print)
{
	for ( int i = 0; i <m_output_width; i++)
		m_pInterpolationOutput[i] = 0.0;

	float minAve = 1000;
	float maxAve = 0;
	float aveAve = 0;
	int countAveAve = 0;
	float aveBgToPixelRatio = 0;
	int countBgToPixelRatio = 0;
	float min_aveBgToPixelRatio = 1000;
	float max_aveBgToPixelRatio = 0;
	m_begin_x_1 = 0;
	m_end_x_1 = m_output_width-1;
	for ( int i = 52; i < m_output_width-30; i++)
	{
		bool found = false;
		for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
		{
			if (m_pWorkData[j*m_output_width+i] == TextColor)
			{
				m_begin_x_1 = i;
				found = true;
				break;
			}
		}
		if (found) break;
	}
	for ( int i = m_output_width-20; i >= 51; i--)
	{
		bool found = false;
		for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
		{
			if (m_pWorkData[j*m_output_width+i] == TextColor)
			{
				m_end_x_1 = i;
				found = true;
				break;
			}
		}
		if (found) break;
	}




	m_numberTextColumns = 0;
	
	for ( int i = m_begin_x_1; i < m_end_x_1; i++)
	{
		m_pWorkData1[i] = 0;
		for ( int j = 0; j < m_height/2; j+=2)
		{
			int jj = j + m_begin_y;
			if (jj > 0 && jj < m_height)
			{
				if (m_pWorkData[jj*m_output_width+i] ==  TextColor)
				{
					m_pWorkData1[i] = 1;
					m_pWorkData1[i+1] = 1;
					m_numberTextColumns++;
					break;
				}
			}
			jj = m_end_y - j;
			if (jj > 0 && jj < m_height)
			{
				if (m_pWorkData[jj*m_output_width+i] == TextColor)
				{
					m_pWorkData1[i] = 1;
					m_pWorkData1[i+1] = 1;
					m_numberTextColumns++;
					break;
				}
			}
		}
	}




	int firstMiddle = 0;
	int lastMiddle = m_output_width;
	int step = 20;
	int firstMiddle1 = 0;
	int lastMiddle1 = m_output_width;
	int step1 = 20;
	int checked_width = m_aveBodyHeight;//*3;
	//if (print)
	//	checked_width*=3;
	int prev = 0;
	int prev1 = 0;
	int numberPixelText = 0;
	int numberPixelBg = 0;
	float BgToPixelRatio = 0;
	int ar_width[400];

	int end = m_end_x_1-2*step;
	int start;
	int finish;
	int numberTextColumns = 0;
	int m_begin_y_1 = -1;
	int m_end_y_1 = -1;
	int numberSpaces = 0;
	int startFound = false;
	int start1 = 0;
	int finish1 = 0;
	int numberPieces = 0;
	for ( int ii = m_begin_x_1; ii <= end; ii+=step)
	{
		start = ii;
		finish = start+1;
		numberTextColumns = 0;
		m_begin_y_1 = -1;
		m_end_y_1 = -1;

		numberSpaces = 0;
		startFound = false;
		while (numberTextColumns < checked_width)
		{
			if ( finish >= m_output_width - 1) break;
			if (m_pWorkData1[finish+=2] == 1)
			{
				numberSpaces = 0;
				numberTextColumns+=2;
				if ( numberTextColumns > (m_aveBodyHeight/3) ) startFound = true;
			}
			else
			{
				numberSpaces++;
				if ( numberSpaces > (m_aveBodyHeight *2/ 3) )
				{
					if (startFound) 
						break;
					else
					{
						start = finish;
						numberTextColumns = 0;
						ii = start-1;
					}
				}
			}
		}

		bool found = false;
		if ( numberPieces == 0)
		{
			numberPixelText = 0;
			numberPixelBg = 0;
		}
		for ( int j = m_begin_y; j < m_end_y; j+=2)
		{
			for ( int i1 = start; i1 < finish; i1+=2)
			{
				int value = m_pWorkData[j*m_output_width+i1];
				if (value == TextColor)
				{
					m_begin_y_1 = j-1;
					found = true;
					break;
				}
			}
			if ( found) break;
		}
		found = false;
		for ( int j = m_end_y; j >=m_begin_y ; j-=2)
		{
			for ( int i1 = start; i1 < finish; i1+=2)
			{
				int value = m_pWorkData[j*m_output_width+i1];
				if (value == TextColor)
				{
					m_end_y_1 = j+1;
					found = true;
					break;
				}
			}
			if ( found) break;
		}
		//int ar_width[200];
		int count = 0;
		//memset(ar_width, 0, 100*sizeof(int));
	
		for ( int kkk = 0; kkk < 400; kkk++)
		{
			ar_width[kkk] = 0;
		}
		int inside = false;
		int first = -1;
		for ( int j = m_begin_y_1; j < m_end_y_1; j+=1)
		{
			if ( j < 0) continue;
			inside = false;
		    first = -1;
		    for ( int i = start; i <= finish; i+=1)
			{
				if ( i < 0 || i >= m_output_width) continue;
				int value = m_pWorkData[j*m_output_width+i];
				int prev_value = m_pWorkData[j*m_output_width+i-1];
				{
					if ( m_pWorkData1[i] == 1)
					{
						if (value == TextColor)
							numberPixelText+=1;
						else
							numberPixelBg+=1;
					}
				}
				if ((value == TextColor) && (prev_value!=TextColor))
				{
					inside = true;
					first = i;
				}
				if (inside && (prev_value==TextColor) && (value!=TextColor))
				{
					inside = false;
					int ww = i - first;
					if (ww  < 100)
					{
						ar_width[ww]++;
						//ar_width[ww+1]++;
						count++;
					}
				}

			}
		}
	
		int enough_part = count*6/10;
		int n = 0;
		int nn = 0;
		//float aveWidth = 0;
		float max_s = 0;
		for ( int k = 1; k <= 20;/*checked_width*/ k++)
		{
			//if ( n < enough_part)
			if ( k <= 20 && k >= 1)
			{
				/*int s = ar_width[k] + ar_width[k-1];
				if ( s > max_s)
				{
					max_s = s;
					aveWidth = (float) (k + (k-1)) / 2.0;
				}*/
				n += ar_width[k];
				nn += ar_width[k]*k;
			}
			else
				ar_width[k] = 0;
		}
		if (!n) n = 1;
	
		float aveWidth = (float)nn / (float)n;
/*		if (print)
		{
			aveWidth = 0;
			int maxWidth = 0;
			for ( int k = 2; k < 20;/*checked_width* / k++)
			{
				int s = ar_width[k-1] + ar_width[k] + ar_width[k+1];
				if (s > maxWidth)
				{
					maxWidth = s;
					aveWidth = k;
				}

			}
		}
*/
		if (aveWidth > 0.0001)
		{
			if ( minAve > aveWidth) minAve = aveWidth;
			if ( maxAve < aveWidth) maxAve = aveWidth;
			aveAve += aveWidth;
			countAveAve++;
		}
		numberPieces++;
		if ( numberPieces == 1)
			start1 = start;


		int middle = (finish+start) /2;
		if ( firstMiddle == 0) firstMiddle = middle;
		lastMiddle = middle;
		for ( int k = prev+1; k<= middle+step/2+1; k++)
		{
			if ( k < 0 || k >= m_output_width) continue;
			m_pInterpolationOutput[k] = aveWidth;
		}
		prev = middle+step/2;



		if ( numberPieces == 3)
		{
			finish1 = finish;
			if (!numberPixelBg) numberPixelBg = 1;
			BgToPixelRatio = (float)numberPixelText / (float) numberPixelBg * 100.0; ;
			if (BgToPixelRatio < min_aveBgToPixelRatio)// && BgToPixelRatio > 20.0)
				min_aveBgToPixelRatio = BgToPixelRatio;
			if (BgToPixelRatio > max_aveBgToPixelRatio)
				max_aveBgToPixelRatio = BgToPixelRatio;
			aveBgToPixelRatio += BgToPixelRatio;
			countBgToPixelRatio++;
		
			int middle1 = (finish1+start1) /2;
			if ( firstMiddle1 == 0) firstMiddle1 = middle1;
			lastMiddle1 = middle1;
			for ( int k = prev1+1; k<= middle1+step1/2+1; k++)
			{
				if ( k < 0 || k >= m_output_width) continue;
				m_pInterpolationOutput1[k] = BgToPixelRatio;
			}
			prev1 = middle+step/2;
			numberPieces = 0;
		}
		
	}

	for ( int i = 0; i < firstMiddle - step/2; i++)
		m_pInterpolationOutput[i] = m_pInterpolationOutput[firstMiddle];

	for ( int i = lastMiddle+step/2; i < m_output_width; i++)
		m_pInterpolationOutput[i] = m_pInterpolationOutput[lastMiddle];

	if (!countAveAve) countAveAve = 1;
	aveAve = (float)aveAve / (float)countAveAve;
	m_aveAve = aveAve;
	if (!countBgToPixelRatio) countBgToPixelRatio = 1;
	aveBgToPixelRatio /= (float) countBgToPixelRatio;
	m_aveBgToTExtRatio = aveBgToPixelRatio;
	m_minWidth = minAve;
	m_maxWidth = maxAve;
	if (print)
	{
	/*	FILE* fff = fopen("C:\\Disk_D\\Projects\\TopScan\\Hebrew\\Data\\Results_190810\\\\info.txt", "at");
		if (fff)
		{
			fprintf(fff, "aveAve =  %f\n", aveAve);
			fprintf(fff, "minAve =  %f\n", minAve);
			fprintf(fff, "maxAve =  %f\n", maxAve);
			fprintf(fff, "(minAve+maxAve)//2 =  %f\n", (minAve+maxAve)/2 );
			fprintf(fff, "m_aveBodyHeight =  %ld\n", m_aveBodyHeight);
			fprintf(fff, "aveAve//m_aveBodyHeight =  %f\n", aveAve/m_aveBodyHeight);
			fprintf(fff, "aveBgToPixelRatio = %f\n", aveBgToPixelRatio);
			fprintf(fff, "aveBgToPixelRatio*aveAve = %f\n", aveBgToPixelRatio*aveAve);
			fprintf(fff, "aveBgToPixelRatio*aveAve//m_aveBodyHeight = %f\n", aveBgToPixelRatio*aveAve/(float)m_aveBodyHeight);
			fprintf(fff, "aveBgToPixelRatio//m_aveBodyHeight = %f\n", aveBgToPixelRatio/(float)m_aveBodyHeight);
			fprintf(fff, "aveAve//(m_aveBodyHeight - 24) = %f\n", aveAve/(float)(m_aveBodyHeight - 24));
			fclose(fff);
		}
		*/
		return;
	}

	float thresholdBgToPixelRatio = 80.0;//max_aveBgToPixelRatio*0.6;;
	float minAve1 = 1000;
	float maxAve1 = 0;
	float minAve2 = 1000;
	float maxAve2 = 0;

	float stDev = 0.0;
	int count = 0.0;
	for ( int ii = m_begin_x_1; ii <= end; ii+=step)
	{
		if (m_pInterpolationOutput1[ii] > 1.0)
		{
			stDev += (m_pInterpolationOutput1[ii] - aveBgToPixelRatio)*(m_pInterpolationOutput1[ii] - aveBgToPixelRatio);
			count++;
		}
	}
	stDev /= count;
    //stDev = sqrt(stDev);
	//stDev = stDev/aveBgToPixelRatio/aveBgToPixelRatio*100.0;
	bool isBold = false;
	float ave1 = 0, ave2 = 0;
	int c1=0, c2=0;
	float ave = 0;
	int c = 0;
	for ( int ii = m_begin_x_1; ii <= end; ii++)
	{
		if (m_pInterpolationOutput[ii] < 0.5) continue;
		ave += m_pInterpolationOutput[ii];
		c++;
	}
	if ( !c) c = 1;
	ave /= c;
	if (stDev > 500.0)
	{
		isBold = true;
		//thresholdBgToPixelRatio = (max_aveBgToPixelRatio-min_aveBgToPixelRatio)/2.0 + min_aveBgToPixelRatio;
		
	
		for ( int ii = m_begin_x_1; ii <= end; ii++)
		{
			if (m_pInterpolationOutput[ii] < 0.5) continue;
			if (m_pInterpolationOutput[ii] < ave)
			{
				if (m_pInterpolationOutput[ii] < minAve1) 
					minAve1 = m_pInterpolationOutput[ii];
				if (m_pInterpolationOutput[ii] > maxAve1) 
					maxAve1 = m_pInterpolationOutput[ii];
				ave1 += m_pInterpolationOutput[ii];
				c1++;
			}
			else
			{
				if (m_pInterpolationOutput[ii] < minAve2) 
					minAve2 = m_pInterpolationOutput[ii];
				if (m_pInterpolationOutput[ii] > maxAve2) 
					maxAve2 = m_pInterpolationOutput[ii];
				ave2 += m_pInterpolationOutput[ii];
				c2++;
			}
		}
	}


	if ( !c1) c1 = 1;
	if ( !c2) c2 = 1;
	ave1 /= c1;
	ave2 /= c2;

	
	for ( int i = 0; i <m_output_width; i++)
	{
		float factor = 1.0;
		//if (m_pInterpolationOutput1[i] < 40.0) 
		//	factor = (40.0 / m_pInterpolationOutput1[i])*(maxAve+minAve)/2.0/m_pInterpolationOutput[i];
		//if (m_pInterpolationOutput1[i] < 90.0) 
			if ( m_pInterpolationOutput[i] < 0.001)
			{
				factor = 1.0;
			//	continue;
			}
			else
			{
				factor = (maxAve+minAve)/2.0/m_pInterpolationOutput[i];

				if (isBold)
				{
					if (m_pInterpolationOutput[i] < ave)
					{
						factor = /*(maxAve1+minAve1)/2.0*/ ave1/m_pInterpolationOutput[i];
					}
					else
					{
						factor = /*(maxAve2+minAve2)/2.0*/ ave2/m_pInterpolationOutput[i];
					}
				}
			}
			//else
		//	factor = (m_pInterpolationOutput1[i]/90.0)*(maxAve+minAve)/2.0/m_pInterpolationOutput[i];;
		int i1 = m_pIndexData[i];
		int i2 = m_pIndexData[i+1];
		for ( int ii1 = i1; ii1 < i2; ii1++)
		{
		//m_pInterpolationInput[ii1] = m_pInterpolationInput[ii1]/factor*1.7*(32.0/733.0/((float)m_numberChangesAllAve/(float)m_numberTextColumns));
			if (factor < 0.001) factor = 1.0;
			if ( ii1 < 0 || ii1 >= m_output_width) continue;
			m_pInterpolationInput1[ii1] = m_pInterpolationInput[ii1]/factor;//*1.7;//*(1.5/((float)m_numberChangesAllAve/(float)m_numberTextColumns));
//			m_pInterpolationInput1[ii1] = factor;//*1.7;//*(1.5/((float)m_numberChangesAllAve/(float)m_numberTextColumns));
		}
	
	}
/*
	ave = 0;
	for ( int i = 0; i < 40; i++)
	{
		ave += m_pInterpolationInput1[i];
	}

	for ( int i = 1; i < m_output_width-40; i++)
	{
		ave -= m_pInterpolationInput1[i-1];
		ave += m_pInterpolationInput1[i+39];
		m_pInterpolationOutput[i+20] = ave / 40.0;
	}
	for ( int i = 0; i < m_output_width; i++)
	{
		m_pInterpolationInput[i] = m_widthPerChange[i] = m_pInterpolationInput[i]/m_pInterpolationOutput[i];;
	}
*/

	for ( int i = 0; i <m_output_width; i++)
		m_pInterpolationInput[i] = m_pInterpolationInput1[i];

	for ( int i = 0; i <m_output_width; i++)
		m_pInterpolationOutput[i] = 0.0;

}
void CHebrewCompress_1::Compress(int isTempOutput)
{
	int BgColr = 255-TextColor;
	if (isTempOutput == 1  )
	{
		memset(m_pWorkData, BgColr, m_output_width*m_height*sizeof(byte));
		memset(m_pIndexData, 0, m_output_width*sizeof(int));
		
	}
	/*if (isTempOutput == 2  )
	{
		memset(m_pWorkData2, BgColr, m_output_width*m_height*sizeof(byte));
		memset(m_pIndexData, 0, m_output_width*sizeof(int));
		
	}

     */


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
	float bodyRatio = (float)m_aveBodyHeight/(float)_REFERENCE_BODY_HEIGHT_;
	
	memset(m_pIndexData, 0, m_output_width*m_height*3*sizeof(int));
		
	
	for ( int i = m_begin_x; i <= m_end_x; i++)
	{
		if ( i >= m_width) break;
		if (indOutput >= m_output_width) 
			break;
		int k = i;
		//if ( i > m_end_x-150) k = m_end_x-150;
	 	//float value1 = ratio1*m_ColorsPerChanges[k] + offset1;
		//float value2 = ratio2*m_widthPerChange[k] + offset2;
	
		if (fabs(m_widthPerChange[k]) < 0.005) continue;
		if (fabs(m_widthPerChange1[k]) < 0.005) continue;
		if (fabs(m_widthPerChange1[k]) > 3.0) continue;
		float value;// = (value2+value2) /  2;
		value = 1.18*m_widthPerChange[k];
		value = 0.2/m_widthPerChange[k];
		value /= bodyRatio;
		

		//if (value < 4)
			//value = 1.0/(1.3*value-0.3/value);
	/*	if ( fabs(3-value) > 0.3)  
			value = 1.0/( );
		else
			value = 1.0/(2.2*value-1.0);
*/
		//value = 2.5/(value*value);
		//else
		//	value = 1.0/(value+1.0/value);
		//value /= 1.5;
		//value =  1.8/(7.0*value+value*value*0.4+value*value*value*1.7 - value*value*value*value*0.35);
		if (value < 0) continue;
		float value6 = value*100.0;
		float value3 =  12.0/(35.0*value+value*value*0.4+value*value*value*1.7 - value*value*value*value*0.14); //- value*value*value*value*value*0.01 + value*value*value*value*value*value*0.001);
		float value4 = (35.0*value+value*value*0.4+value*value*value*1.7 - value*value*value*value*0.14);
		
		float value5 = value4*3.0;
		value4 = 42.0 + (value6-40.0)*(141.0-40.0)/(164.0 - 42.0);

		value3 = 12.0 / value4;
		value3*=2.9;
		if (value3 < 0) continue;
		//float value2 = value3;
		float value2 = value;
		value2 /= bodyRatio;
		nc+= value2;
		ncInt = (int) nc;
		int add = ncInt - prevNc;
		if (i >= m_end_x-25)
			int a = 0;//add++;
		int end_x = m_end_x - 300;
		if (m_isDoubleWidth)
		{
			end_x = m_begin_x + 300 + (end_x - (m_begin_x +  300)) / 2;
		}
		for ( int k = 0; k < add; k++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (isTempOutput )
					m_pWorkData[j*m_output_width+ indOutput] = m_pDataIn[j*m_width+i];
				else
				{
					if ( i < m_begin_x + 300 || i > end_x ) continue;
				
					m_pDataOut [j*m_output_width*3+ indOutput*3 +0] = (m_pDataIn3) ? m_pDataIn3[j*m_width*3 + i*3+0]: m_pDataIn[j*m_width + i];
					m_pDataOut [j*m_output_width*3+ indOutput*3 +1] = (m_pDataIn3) ? m_pDataIn3[j*m_width*3 + i*3+1]: m_pDataIn[j*m_width + i];
					m_pDataOut [j*m_output_width*3+ indOutput*3 +2] = (m_pDataIn3) ? m_pDataIn3[j*m_width*3 + i*3+2]: m_pDataIn[j*m_width + i];
					if (m_compressInterpolation)
						m_compressInterpolation[indOutput] = i-300;
					if (m_compressInterpolationOpposite)
						m_compressInterpolationOpposite[i-300] = indOutput;
				}

			}
			m_pIndexData[indOutput] = i;
			indOutput++;
			if (indOutput >= m_output_width) break;
		}
		if (add)
			prevNc = ncInt;
	
	}
	/*if (m_doubleTextPixels)
	{
		for ( int i = 0; i < m_output_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (IsText(m_pDataOut [j*m_output_width*3+ i*3 +0], TextColor))
				{
					//m_pDataOut [j*m_output_width*3+ (i-2)*3 +0] = TextColor;
					//m_pDataOut [j*m_output_width*3+ (i-2)*3 +1] = TextColor;
					//m_pDataOut [j*m_output_width*3+ (i-2)*3 +2] = TextColor;
					m_pDataOut [j*m_output_width*3+ (i-1)*3 +0] = TextColor;
					m_pDataOut [j*m_output_width*3+ (i-1)*3 +1] = TextColor;
					m_pDataOut [j*m_output_width*3+ (indOutput-1)*3 +2] = TextColor;
				}
			}
		}
	}*/
}
void CHebrewCompress_1::CalculateWidthPerChanges1(int numberChangesMax, int step)
{

	memset(m_pWorkData, 0, m_output_width*m_height*sizeof(byte));
	memset(m_pWorkData1, 0, m_output_width*sizeof(byte));





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

	for ( int i = m_begin_x; i< m_end_x-step; i+=step)
	{

		int i1 = i;
		int i2 = i+step;







		int iiii = 0;
		int numberChanges = 0;
		int ii = (i1+i2) / 2;
		int delta = 0;
#ifdef _DYNAMIC_WINDOW_
		while ( numberChanges < numberChangesMax/2)
#else
		while ( numberChanges < numberChangesMax)
#endif
		{
			bool isContinue = false;
			int ii1 = ii+delta;
			if ( ii1 < m_begin_x)
					ii1 = m_begin_x + (m_begin_x - ii1);
				
			if ( ii1 < m_width && ii1 > 0) 
			{
				bool isText = false;
				for ( int j = m_begin_y; j < m_end_y; j++)
				{
					if (m_pWorkData[j*m_width+ii1])
						numberChanges++;
				}
				if (m_pWorkData1[ii1])
					iiii++;
				isContinue = true;
			}
#ifdef _DYNAMIC_WINDOW_
			if (!isContinue)
				break;
#endif
			delta++;
#ifdef _DYNAMIC_WINDOW_
		}
		numberChanges = 0;
		delta = 0;
		while ( numberChanges < numberChangesMax/2)
		{
			bool isContinue = false;
			int ii1 = ii-delta;
#else
			ii1 = ii-delta;
#endif
			if ( ii1 < m_begin_x)
					ii1 = m_begin_x + (m_begin_x - ii1);
				
			if (ii1 < m_width && ii1 > 0)
			{
				bool isText = false;
				for ( int j = m_begin_y; j < m_end_y; j++)
				{
					if (m_pWorkData[j*m_width+ii1])
						numberChanges++;
				}
				if (m_pWorkData1[ii1])
					iiii++;
				isContinue = true;
			}
			if (!isContinue)
				break;
#ifdef _DYNAMIC_WINDOW_
			delta++;
#endif			
		}
		if (numberChanges < 5) continue;
		float ratio = (float) (iiii) / (float) numberChanges;
		if (ratio > 6) continue;
		
		int i_b = (i1+i2)/2-step/2;
		int i_e = (i1+i2)/2+step/2;

		if ( (i_e < m_width) && (i_b >= 0))
		{
			for ( int i12 = i_b;i12 < i_e; i12++) 
				m_widthPerChange[i12] = ratio;
			lastSetIndex = i_e;
			lastSetValue = ratio;
		}
		
	}
}
void CHebrewCompress_1::CalculateWidthPerChange(int window, int step)
{

	memset(m_pWorkData, 0, m_output_width*m_height*sizeof(byte));
	memset(m_pWorkData1, 0, m_output_width*sizeof(byte));





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
/*	for ( int i = m_begin_x+300; i< m_end_x-300; i++)
	{
			bool columnsEqual = true;
			if (m_pWorkData1[i])
			{
				for ( int j = m_begin_y;/*m_body[ii] + m_bodyHeight[ii];* / j < m_end_y;/*m_body[ii] + m_bodyHeight[ii];* / j++)
				{
					if (m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i-1])
					{
						columnsEqual = false;
						break;
					}
				}
				numberColumnsEqual++;
			}
			if ((!columnsEqual) || (!m_pWorkData1[i]))
			{
				if (numberColumnsEqual > 45)
				{
					wasJerky = true;
					for (int ii = i - numberColumnsEqual + 15; ii < i-1; ii++)
						m_pWorkData4[ii] = 1;
				}
				numberColumnsEqual = 0;
				continue;
			}
	}

	if (wasJerky)
	{
		int indOutput = m_begin_x+2;
		for ( int i = m_begin_x+2; i< m_end_x; i++)
		{
			if (m_pWorkData4[i]) 
			{
				i++;
				continue;
			}
			for ( int j = m_begin_y;/*m_body[ii] + m_bodyHeight[ii];* / j < m_end_y;/*m_body[ii] + m_bodyHeight[ii];* / j++)
			{
				m_pWorkData5[j*m_width+indOutput] = m_pDataIn[j*m_width+i];
			}
			indOutput++;
		}
		m_end_x = indOutput;
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = m_begin_y;/*m_body[ii] + m_bodyHeight[ii];* / j < m_end_y;/*m_body[ii] + m_bodyHeight[ii];* / j++)
			{
				m_pDataIn[j*m_width+i] = m_pWorkData5[j*m_width+i];
				m_pWorkData5[j*m_width+i] = 0;
			}
		}
	}
*/
/*	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = m_begin_y;/*m_body[ii] + m_bodyHeight[ii];* / j < m_end_y;/*m_body[ii] + m_bodyHeight[ii];* / j++)
		{
			m_pDataOut[j*m_output_width*3+i*3+0] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_output_width*3+i*3+1] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_output_width*3+i*3+2] = m_pDataIn[j*m_width+i];
		}
	}
*/







	int lastSetIndex = 0;
	float lastSetValue = 1.0;
	for ( int i = m_begin_x-window; i< m_end_x; i+=step)
	{
		//if ( i < 0) continue;
		//if ( i >= m_width) break;

			int i1 = i;
			//int i2 = i+100;
			int i2 = i+window;
			//if (i2 > m_width) 
			//	i2 = m_width-1;

/*		for ( int j = m_begin_y; j < m_end_y; j++)
		{
			bool isText = false;
				
			int x1 = 0;
			for (int ii = i1; ii < i2; ii++)
			{
				byte color = m_pDataIn[j*m_width+ii];
				byte prevColor = m_pDataIn[j*m_width+ii-1];
				byte prevprevColor = m_pDataIn[j*m_width+ii-2];
				
				if ((color == TextColor) && (prevColor != TextColor) && (prevprevColor != TextColor))
				{
					isText = true;
					x1 = ii;
				}
				if ((color != TextColor) && (prevColor == TextColor) && (prevprevColor == TextColor))
				{
					
					if (isText)
					{
						int l = ii - x1;
						if ( l < 100)
							thick[l]++;
					}
					isText = false;
					x1 = 0;
				}
			}
		}
		int max = 0;
		int lMax = 0;
		for ( int l = 2; l < 97; l++)
		{
			int current = thick[l-1]+thick[l]+thick[l+1];
			if (current > max)
			{
				max = current;
				lMax = l+1;
			}
		}
		m_ColorsPerChanges[(i1+i2) / 2] = lMax;

*/






		int iiii = 0;
		int numberChanges = 0;
		for (int ii = i1; ii < i2; ii++)
		{
/*			int iii[3] = {ii, ii-1, ii-2};
			for ( int k = 0; k < 3; k++)
			{
				if ( iii [k] < m_begin_x)
					iii[k] = m_begin_x + (m_begin_x - iii[k]);
				if ( iii [k] > m_end_x)
					iii[k] = m_end_x - (iii[k] - m_end_x);
			}
*/
			int ii1 = ii;
			if ( ii1 < m_begin_x)
					ii1 = m_begin_x + (m_begin_x - ii1);
				
			if ( ii1 >= m_width || ii1 < 0) continue;
			bool isText = false;
			for ( int j = m_begin_y;/*m_body[ii] + m_bodyHeight[ii];*/ j < m_end_y;/*m_body[ii] + m_bodyHeight[ii];*/ j++)
			{
/*				byte color = m_pDataIn[j*m_width+iii[0]];
				byte prevColor = m_pDataIn[j*m_width+iii[1]];
				byte prevprevColor = m_pDataIn[j*m_width+iii[2]];

				if (color == TextColor)
				{
					isText = true;
					//numberTextPixels++;
				}

				if ((color == TextColor) && (prevColor != TextColor)&&(prevprevColor != TextColor))
				{
					numberChanges++;
				}
				if ((color != TextColor) && (prevColor == TextColor)&&(prevprevColor == TextColor))
				{
					numberChanges++;
				}
*/			
				if (m_pWorkData[j*m_width+ii1])
					numberChanges++;
			}
			//if (isText) iiii++;
			if (m_pWorkData1[ii1])
				iiii++;

		}
		//if (iiii < 100) continue;
		if (numberChanges < 5) continue;
		float ratio = (float) (iiii) / (float) numberChanges;
		if (ratio > 6 || ratio < 0.001) continue;
		/*int i12_1 = (i1+i2)/4;
		int i12_2 = (i1+i2)*2/4;
		int i12_3 = (i1+i2)*3/4;
*/
		int i_b = (i1+i2)/2-step/2;
		int i_e = (i1+i2)/2+step/2;

		if ( (i_e < m_width) && (i_b >= 0))
		{
			/*m_widthPerChange[i12_1] = ratio;
			m_widthPerChange[i12_2] = ratio;
			m_widthPerChange[i12_3] = ratio;*/
				for ( int i12 = i_b;i12 < i_e; i12++) 
			{
				if (i12 < 0 || i12 >= m_output_width) continue;
				m_widthPerChange1[i12] = ratio;
			}
			lastSetIndex = i_e;
			lastSetValue = ratio;
		}
		/*if ((i1 - m_begin_x) < 0)
		{
			for ( int k = m_begin_x; k < (i1 +i2)/2 - 40; k+=20)
			{
				m_widthPerChange[k] = ratio;
			}
		}*/

/*		if ((i2 - m_end_x ) > 200)
		{
			for ( int i = (i1+i2)/2+40; i <= m_end_x+20; i+=20)
			{
				if ( i >= m_width) break;
				m_widthPerChange[i] = ratio;
			}
		}
		*/
		
	}
	/*for ( int i = m_end_x+40 ; i >= lastSetIndex; i-=20)
	{
		if ( i >= m_width) continue;
		m_widthPerChange[i] = lastSetValue;
	}
	for ( int i = m_width-1; i >= m_end_x+1; i-=20)
	{
		m_widthPerChange[i] = lastSetValue;
	}*/




	/*
// good enough
	for ( int i = m_begin_x-2; i< m_end_x+2; i+=50)
	{
		int i1 = i;
		int i2 = i+400;
		if (i2 > m_end_x) 
			i2 = m_end_x;

		int numberTextPixels = 0;
		int numberChanges = 0;
		int iiii = 0;
		for (int ii = i1; ii < i2; ii++)
		{
			bool isText = false;
			for ( int j = m_body[ii] + m_bodyHeight[ii]/3; j < m_body[ii] + m_bodyHeight[ii]*2/3; j++)
			{
				byte color = m_pDataIn[j*m_width+ii];
				byte prevColor = m_pDataIn[j*m_width+ii-1];
				byte prevprevColor = m_pDataIn[j*m_width+ii-2];

				if (color == TextColor)
				{
					isText = true;
					numberTextPixels++;
				}

				if ((color == TextColor) && (prevColor != TextColor)&&(prevprevColor != TextColor))
				{
					numberChanges++;
				}
				
			}
			if (isText) iiii++;
		}
		
		float ratio = (float) numberTextPixels/ (float) numberChanges;
		m_ColorsPerChanges[(i1+i2) / 2] = ratio;
		m_widthPerChange[(i1+i2) / 2] = (float) (iiii) / (float) numberChanges;
		/*
		if ((i1 - m_begin_x) < 20)
		{
			for ( int i = m_begin_x; i < (i1 +i2)/2 - 40; i+=20)
			{
				m_ColorsPerChanges[i] = ratio;
				m_widthPerChange[i] = (float) (i2-i1) / (float) numberChanges;
			}
		}

		if ((m_end_x - i2) < 20)
		{
			for ( int i = (i1+i2)/2+40; i < m_end_x; i+=20)
			{
				m_ColorsPerChanges[i] = ratio;
				m_widthPerChange[i] = (float) (i2-i1) / (float) numberChanges;
			}* /
	}*/
	
/*
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_output_width*3 + i*3+1] = m_pBody[j*m_width+i]*255;
		}
	}
*/
}

void CHebrewCompress_1::CalculateColorPerChanges()
{
	/*m_To = m_begin_x;
	CalculateImageBoundaries();
	CalculateBody();

	for (  int jj = 0; jj < m_height; jj++)
	{
		for ( int index = 0; index <= _NUMBER_CHANGES_TO_ANALYZE_; index++)
		{
			m_pChangesCoord[jj][index] = 0;
		}
	}*/
	
/*	for ( int i = m_begin_x-2; i< m_end_x+2; i++)
	{
		for (  int jj = m_begin_y; jj <= m_end_y; jj++)
		{
			byte color = m_pDataIn[jj*m_width+i];
			byte prevColor = m_pDataIn[jj*m_width+i-1];
			byte prevprevColor = m_pDataIn[jj*m_width+i-2];


			if ((color == TextColor) && (prevColor != TextColor)&&(prevprevColor != TextColor))
			{
				bool goToNextPixel = false;
				m_pChangesCoord[jj][_NUMBER_CHANGES_TO_ANALYZE_] = i;
				if ((m_pChangesCoord[jj][0] != 0) && (i > m_To + 10) &&
					((i -  m_pChangesCoord[jj][0]) < _NUMBER_CHANGES_TO_ANALYZE_*150))
				{
					CalculateColorsToChngesRatio(jj);
					m_To = m_pChangesCoord[jj][_NUMBER_CHANGES_TO_ANALYZE_];
					goToNextPixel = true;
				}

				for ( int index = 1; index <= _NUMBER_CHANGES_TO_ANALYZE_; index++)
				{
					m_pChangesCoord[jj][index-1] = m_pChangesCoord[jj][index];
				}
				m_pChangesCoord[jj][_NUMBER_CHANGES_TO_ANALYZE_] = 0;
				if (goToNextPixel)
					break;
			}
		}
	}*/
/*
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_output_width*3 + i*3+1] = m_pBody[j*m_width+i]*255;
		}
	}
*/
	

	int lastSetIndex = 0;
	float lastSetValue = 0;
	for ( int i = m_begin_x-100; i< m_end_x-50; i+=50)
	{
		if ( i < 0) continue;
		if ( i >= m_width) break;

		int i1 = i;
		int i2 = i+300;
		//if (i2 > m_width) 
		//	i2 = m_width-1;

		int numberTextPixels = 0;
		int numberChanges = 0;
		for (int ii = i1; ii < i2; ii++)
		{
			if ( ii >= m_width) continue;
			for ( int j = m_body[ii] + m_bodyHeight[ii]/3; j < m_body[ii] + m_bodyHeight[ii]*2/3; j++)
			{
				byte color = m_pDataIn[j*m_width+ii];
				byte prevColor = m_pDataIn[j*m_width+ii-1];
				byte prevprevColor = m_pDataIn[j*m_width+ii-2];

				if (color == TextColor)
				{
					numberTextPixels++;
				}

				if ((color == TextColor) && (prevColor != TextColor)&&(prevprevColor != TextColor))
				{
					numberChanges++;
				}
				
			}
		}
		//if (numberChanges < 20) continue;

		float ratio = (float) numberTextPixels/ (float) numberChanges;
		if ( (i1 + i2) / 2 < m_width && (i1+i2) >= 0)
		{
			m_ColorsPerChanges[(i1+i2) / 2] = ratio;
			lastSetIndex = (i1+i2) / 2;
			lastSetValue = ratio;
		}
		
		if ((i1 - m_begin_x) < 50)
		{
			for ( int i = m_begin_x; i < (i1 +i2)/2 - 40; i+=20)
			{
				if ( i >= m_width) break;
				if ( i < 0) continue;
				m_ColorsPerChanges[i] = ratio;
			}
		}
/*
		if ((i2 - m_end_x ) > 200)
		{
			for ( int i = (i1+i2)/2+40; i < m_end_x+20; i+=20)
			{
				if ( i >= m_width) break;
				m_ColorsPerChanges[i] = ratio;
			}
		}
*/
	}
	for ( int i = m_end_x+40 ; i >= lastSetIndex; i-=20)
	{
		if ( i >= m_width || i < 0) continue;
		m_ColorsPerChanges[i] = lastSetValue;
	}

}

void CHebrewCompress_1::CalculateBody()
{
	for (  int jj = 0; jj < m_height; jj++)
	{
		for ( int index = 0; index <= _NUMBER_CHANGES_TO_ANALYZE_; index++)
		{
			m_pChangesCoord[jj][index] = 0;
		}
	}
	memset(m_pBody, 0, m_output_width*m_height*sizeof(byte));
	m_numberChangesAllAve = 0;
	for ( int i = m_begin_x-200; i< m_end_x+200; i++)
	{
		if ( i < 0) continue;
		if ( i >= m_width) continue;
		for (  int jj = m_begin_y; jj <= m_end_y; jj++)
		{
			byte color = m_pDataIn[jj*m_width+i];
			byte prevColor = m_pDataIn[jj*m_width+i-1];
			byte prevprevColor = m_pDataIn[jj*m_width+i-2];


			if ((color == TextColor) && (prevColor != TextColor)&&(prevprevColor != TextColor))
			{
				bool goToNextPixel = false;
				m_pChangesCoord[jj][_NUMBER_CHANGES_TO_ANALYZE_] = i;
				m_numberChangesAllAve++;
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
        if ( i < 0 || i >= m_width) continue;
		m_body[i] = 0;
		m_bodyHeight[i] = 0;
		for ( int j = m_begin_y; j <= m_end_y; j++)
		{
            if ( j < 0 || j >= m_height) continue;
			if (m_pBody[j*m_width+i] ) 
			{
				m_body[i] = j;
				break;
			}
		}
		for ( int j = m_end_y; j >= m_begin_y; j--)
		{
			if ( j < 0 || j >= m_height) continue;
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
		if ( i < 0 || i >= m_width) continue;
		if ( m_bodyHeight[i] > 0) 
		{
			ave += m_bodyHeight[i];
			n++;
		}
	}
	if (n)
		m_aveBodyHeight = ave/ n;
	//if (m_aveBodyHeight)
	//m_numberChangesAllAve /= m_aveBodyHeight;

}
void CHebrewCompress_1::CalculateColorsToChngesRatio(int jj)
{
	int i1 = m_pChangesCoord[jj][0];
	int i2 = m_pChangesCoord[jj][_NUMBER_CHANGES_TO_ANALYZE_];

	int numberTextPixels = 0;
	int numberChanges = 0;
	int numberChanges_Y = 0;
	for (int i = i1; i < i2; i++)
	{
		//for ( int j = m_body[i] + m_bodyHeight[i]/3; j < m_body[i] + m_bodyHeight[i]*2/3; j++)
		for ( int j = m_body[i]; j < m_body[i]+ m_bodyHeight[i]; j++)
		{
			byte color = m_pDataIn[j*m_width+i];
			byte prevColor = m_pDataIn[j*m_width+i-1];
			byte prevprevColor = m_pDataIn[j*m_width+i-2];
			byte prevColor_Y = m_pDataIn[(j-1)*m_width+i];
			byte prevprevColor_Y = m_pDataIn[(j-2)*m_width+i];

			if (color == TextColor)
			{
				numberTextPixels++;
			}

			if ((color == TextColor) && (prevColor != TextColor)&&(prevprevColor != TextColor))
			{
				numberChanges++;
			}
			if ((color == TextColor) && (prevColor_Y != TextColor)&&(prevprevColor_Y != TextColor))
			{
				numberChanges_Y++;
			}
			
		}
	}
	
	float ratio = (float) numberTextPixels/ (float) numberChanges;
	if ((i1+i2)/2 < m_output_width && (i1+i2) >= 0)
	{
	m_ColorsPerChanges[(i1+i2) / 2] = ratio;
		m_widthPerChange[(i1+i2) / 2] = (float) numberChanges_Y / (float) numberChanges;
	}
	/*if ((i1 - m_begin_x) < 20)
	{
		for ( int i = m_begin_x; i < (i1 +i2)/2 - 40; i+=20)
		{
			m_ColorsPerChanges[i] = ratio;
			m_widthPerChange[i] = (float) (i2-i1) / (float) numberChanges;
		}
	}

	if ((m_end_x - i2) < 20)
	{
		for ( int i = (i1+i2)/2+40; i < m_end_x; i+=20)
		{
			m_ColorsPerChanges[i] = ratio;
			m_widthPerChange[i] = (float) (i2-i1) / (float) numberChanges;
		}
	}*/

}
void CHebrewCompress_1::DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue, float maxValue)
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
			m_pDataOut[j*m_output_width*3 + i*3+0]  = red;

		if ( green != -1)
			m_pDataOut[j*m_output_width*3 + i*3+1]  = green;

		if ( blue != -1)
			m_pDataOut[j*m_output_width*3 + i*3+2]  = blue;

	}

}
