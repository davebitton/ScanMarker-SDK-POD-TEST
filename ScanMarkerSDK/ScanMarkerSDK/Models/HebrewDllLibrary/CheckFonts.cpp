#include "CheckFonts.h"
#ifdef _SCANMARKER_WINDOWS_
#include "xv.h"
#include "xvbmp.h"
#else
#include "bmp/xv.h"
#include "bmp/xvbmp.h"
#endif

#define TextColor 255
CCheckFonts::CCheckFonts(void)
{
	m_width = 0;
	m_height = 0;
	m_pDataIn = 0;
	m_fontsToCheck = NULL;
	m_factorToApply = 1.0;
	m_fontApplied[0] = 0;
	m_widthHorizontalLow_1 = 0;
	m_widthHorizontalLow_2 = 0;
	m_widthHorizontalHigh_1 = 0;
	m_widthHorizontalHigh_2 = 0;
	m_widthVertical_1 = 0;
	m_widthVertical_2 = 0;
	m_widthBgHorizontal_1 = 0;
	m_widthBgHorizontal_2 = 0;

	
}

CCheckFonts::~CCheckFonts(void)
{
	m_width = 0;
	m_height = 0;
	m_pDataIn = 0;
	m_fontsToCheck = NULL;
	m_factorToApply = 1.0;
	m_fontApplied[0] = 0;
}

void CCheckFonts::Init( int width, int height, byte* pDataIn)
{
	m_width = width;
	m_height = height;
//	m_pDataIn = pDataIn;
	m_pDataIn = new byte[width*height*3];

	m_factorToApply = 1.0;
	m_fontApplied[0] = 0;

}
void CCheckFonts::Release()
{
	if (m_pDataIn)
	{
		delete [] m_pDataIn;
		m_pDataIn = NULL;
	}
}
void CCheckFonts::WriteDebugStr(char * str, bool isAppend)
{
	return;
	FILE * fff = NULL;
	if (isAppend)
		fff = fopen("C://Temp//CheckFontsDebug.txt", "at");
	else
		fff = fopen("C://Temp//CheckFontsDebug.txt", "wt");

	if (fff)
	{
		fprintf(fff, "%s", str);
		fclose(fff);
	}
}

void CCheckFonts::WriteTablesDebug(char* filename_basic)
{	
	return;
	char fn[1000];
	{
		sprintf(fn, "%s_font_tables_vertical.txt", filename_basic);
		FILE * fff = fopen(fn, "wt");
		if (fff)
		{
			fprintf(fff, "widthVertical Found By Max Value= %.2f\nTable = %d %d\n", m_widthVertical, m_widthVertical_1, m_widthVertical_2);
			for ( int w = 0; w  < 20; w++)
			{
				fprintf(fff, "%d %.2f \n", w, m_widthVerticalArray[w]);
			}

			fclose(fff);
		}
	}

	{
		sprintf(fn, "%s_font_tables_horizontal.txt", filename_basic);
		FILE * fff = fopen(fn, "wt");
		if (fff)
		{
			fprintf(fff, "widthHorizontalLow Found = %.2f\nwidthHorizontalHigh Found = %.2f\nTable Low= %d %d\nTable High= %d %d\n", 
				m_widthHorizontalLow, m_widthHorizontalHigh,  m_widthHorizontalLow_1, m_widthHorizontalLow_2, m_widthHorizontalHigh_1, m_widthHorizontalHigh_2);
			int count = 0;
			for ( int w = 0; w  < 50; w++)
			{
				count += m_widthHorizontalArray[w];
				fprintf(fff, "%d %.2f  \n", w, m_widthHorizontalArray[w]);
			}

			fprintf(fff, "\n\n\nSorted By  Percentage\n\n");
			float highPerc = ((float) (1.0 * count)) / 100.0;
			float lowPerc = 0.0;
			int w = 0;
			float c1 = 0;
			float c2 = 0;
			float w1 = 0;
			for ( int perc = 2; perc < 100; perc++)
			{
				highPerc = ((float) (perc * count)) / 100.0;
				while (c1 < highPerc && w < 50)
				{
					w++;
					c2 +=  m_widthHorizontalArray[w];
					c1 +=  m_widthHorizontalArray[w];
					w1 += w*m_widthHorizontalArray[w];
				}
				c2 -= (c1 - highPerc);
				w1 -= w* (c1 - highPerc);
				float wInPerc = w1 / (float) c2;
				fprintf(fff, "%d %.2f\n", perc, wInPerc);

				c2 = c1 - highPerc;
				w1 = c2*w;
			}

			fclose(fff);
		}
	}

	{
		sprintf(fn, "%s_font_tables_BgHorizontal.txt", filename_basic);
		FILE * fff = fopen(fn, "wt");
		if (fff)
		{
			fprintf(fff, "widthBgHorizontal Found = %.2f\nTable = %d %d\n", m_widthBgHorizontal, m_widthBgHorizontal_1, m_widthBgHorizontal_2);
			int count = 0;
			for ( int w = 0; w  < 100; w++)
			{
				count += m_widthBgHorizontalArray[w];
				fprintf(fff, "%d %.2f  \n", w, m_widthBgHorizontalArray[w]);
			}


			fprintf(fff, "\n\n\nSorted By  Percentage\n\n");
			float highPerc = ((float) (1.0 * count)) / 100.0;
			float lowPerc = 0.0;
			int w = 0;
			int c1 = 0;
			int c2 = 0;
			float w1 = 0;
			for ( int perc = 2; perc < 100; perc++)
			{
				highPerc = ((float) (perc * count)) / 100.0;
				while (c1 < highPerc && w < 100)
				{
					w++;
					c2 +=  m_widthBgHorizontalArray[w];
					c1 +=  m_widthBgHorizontalArray[w];
					w1 += w*m_widthBgHorizontalArray[w];
				}
				c2 -= (c1 - highPerc);
				w1 -= w* (c1 - highPerc);
				
				float wInPerc = w1 / (float) c2;
				fprintf(fff, "%d %.2f\n", perc, wInPerc);

				c2 = c1 - highPerc;
				w1 = c2*w;
			}

			fclose(fff);
		}
	}

	{
		byte *dataOut = new byte[m_width*m_height*3];

		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				dataOut[j*m_width*3 + i*3 + 0] = m_pDataIn[j*m_width+i];
				dataOut[j*m_width*3 + i*3 + 1] = m_pDataIn[j*m_width+i];
				dataOut[j*m_width*3 + i*3 + 2] = m_pDataIn[j*m_width+i];
			}
		}
		
		for ( int i = 0; i < m_width; i++)
		{
			int j_start = -1;
			for ( int j = 1; j < m_height; j++)
			{
				if ( m_pDataIn[j*m_width + i] != m_pDataIn[(j-1)*m_width + i])
				{
					if (m_pDataIn[j*m_width + i] == TextColor)
					{
						j_start = j;
					}
					else
					{
						int w = j - j_start;
						/*if ( w == m_widthVertical)
						{
							for ( int j1 = j_start; j1 < j; j1++)
							{
								dataOut[j1*m_width*3 + i*3 + 0] = 127;
								dataOut[j1*m_width*3 + i*3 + 1] = 127;
							}
						}*/
						if ( w >= m_widthVertical_1 && w <= m_widthVertical_2)
						{
							for ( int j1 = j_start; j1 < j; j1++)
							{
								dataOut[j1*m_width*3 + i*3 + 0] = 127;
								dataOut[j1*m_width*3 + i*3 + 1] = 127;
							}
						}
					}
				}
			}
		}


		for ( int j = 0; j < m_height; j++)
		{
			int i_start = -1;	
			for ( int i = 1; i < m_width; i++)
			{
				if ( m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i-1])
				{
					if (m_pDataIn[j*m_width + i] == TextColor)
					{
						i_start = i;
					}
					else
					{
						if ( i_start == -1) continue;
						float w = i - i_start;
/*						if ( fabs(w - m_widthHorizontalLow) <= 1.1)
						{
							for ( int i1 = i_start; i1 < i; i1++)
							{
								dataOut[j*m_width*3 + i1*3 + 1] = 127;
								dataOut[j*m_width*3 + i1*3 + 2] = 127;
							}
						}
						if ( fabs(w - m_widthHorizontalHigh) <= 1.1)
						{
							for ( int i1 = i_start; i1 < i; i1++)
							{
								dataOut[j*m_width*3 + i1*3 + 0] = 127;
								dataOut[j*m_width*3 + i1*3 + 2] = 127;
							}
						}
*/
						if ( w >= m_widthHorizontalLow_1 && w <= m_widthHorizontalLow_2 )
						{
							for ( int i1 = i_start; i1 < i; i1++)
							{
								dataOut[j*m_width*3 + i1*3 + 1] = 127;
								dataOut[j*m_width*3 + i1*3 + 2] = 127;
							}
						}
						if ( w >= m_widthHorizontalHigh_1 && w <= m_widthHorizontalHigh_2)
						{
							for ( int i1 = i_start; i1 < i; i1++)
							{
								dataOut[j*m_width*3 + i1*3 + 0] = 127;
								dataOut[j*m_width*3 + i1*3 + 2] = 127;
							}
						}
					}
				}
			}
		}

		for ( int j = 0; j < m_height; j++)
		{
			int i_start = -1;	
			for ( int i = 1; i < m_width; i++)
			{
				if ( m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i-1])
				{
					if (m_pDataIn[j*m_width + i] != TextColor)
					{
						i_start = i;
					}
					else
					{
						if ( i_start == -1) continue;
						float w = i - i_start;

/*						if ( fabs(w - m_widthBgHorizontal) <= 1.1)
						{
							for ( int i1 = i_start; i1 < i; i1++)
							{
								dataOut[j*m_width*3 + i1*3 + 0] = 55;
								dataOut[j*m_width*3 + i1*3 + 1] = 127;
								dataOut[j*m_width*3 + i1*3 + 2] = 180;
							}
						}
*/						
						if ( w >= m_widthBgHorizontal_1 && w <= m_widthBgHorizontal_2 )
						{
							for ( int i1 = i_start; i1 < i; i1++)
							{
								dataOut[j*m_width*3 + i1*3 + 0] = 55;
								dataOut[j*m_width*3 + i1*3 + 1] = 127;
								dataOut[j*m_width*3 + i1*3 + 2] = 180;
							}
						}
					}
					
				}
			}
		}

		sprintf(fn, "%s_font_tables_ImageOut.bmp", filename_basic);
		FILE* fp = fopen(fn, "wb");
		if (fp)
		{
			WriteBMP(fp,dataOut,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
		delete [] dataOut;
	}



}

void CCheckFonts::Calculate()
{
	if (!m_fontsToCheck) return;

	WriteDebugStr("aaaaaaaa", false);
	int widthVertical = 0;
	//int allWidthVertical[128];
	
	for ( int j = 0; j < m_height; j++)
		m_widthVerticalArray[j] = 0;
	int allLines = 0;

	for ( int i = 0; i < m_width; i++)
	{
		int j_start = -1;
		for ( int j = 1; j < m_height; j++)
		{
			if ( m_pDataIn[j*m_width + i] != m_pDataIn[(j-1)*m_width + i])
			{
				if (m_pDataIn[j*m_width + i] == TextColor)
				{
					j_start = j;
				}
				else
				{
					int w = j - j_start;
					m_widthVerticalArray[w]++;
					allLines++;
				}
			}
		}
	}
	WriteDebugStr("bbbbb" );
	int maxValue = 0;
	int w_maxValue = -1;
	char str1[1000];
//22222222
	for ( int w = 1; w < 20; w++)
	{
		///int currentValue = m_widthVerticalArray[w-1]+m_widthVerticalArray[w]+m_widthVerticalArray[w+1];
		int currentValue = m_widthVerticalArray[w];
		sprintf(str1, " w = %d m_widthVerticalArray[w] =%d\n",w, m_widthVerticalArray[w]);
		WriteDebugStr(str1);
		if (currentValue > maxValue)
		{
			w_maxValue = w;
			maxValue = currentValue;
		}
	}

	float threshold = maxValue / 2.0;
	for ( int w = w_maxValue - 3; w <= w_maxValue + 3; w++)
	{
		if ( w <= 0 ) continue;
		if (m_widthVerticalArray[w] >= threshold)
		{
			if (m_widthVertical_1 == 0) m_widthVertical_1 = w;
			m_widthVertical_2 = w;

		}
	}





	float _sum = 0;
	int _count = 0;
	for ( int w = m_widthVertical_1; w <=m_widthVertical_2;w++)
	{
		_sum += w * m_widthVerticalArray[w];
		_count += m_widthVerticalArray[w];
	}
/*
m_widthHorizontalLow_1 = 0;
	m_widthHorizontalLow_2 = 0;
	m_widthHorizontalHigh_1 = 0;
	m_widthHorizontalHigh_2 = 0;
	m_widthVertical_1 = 0;
	m_widthVertical_2 = 0;
	m_widthBgHorizontal_1 = 0;
	m_widthBgHorizontal_2 = 0;
*/

	//11111
	if (!_count) _count = 1;

	//widthVertical = w_maxValue;
	m_widthVertical = _sum/(float)_count;

	sprintf(str1, "\nwidthVertical =%d\n", widthVertical);
	WriteDebugStr(str1);
	for ( int w = 0; w < 100; w++)
	{
		m_widthHorizontalArray[w] = 0;
	}
	for ( int j = 0; j < m_height; j++)
	{
		int i_start = -1;	
		for ( int i = 1; i < m_width; i++)
		{
			if ( m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i-1])
			{
				if (m_pDataIn[j*m_width + i] == TextColor)
				{
					i_start = i;
				}
				else
				{
					if ( i_start == -1) continue;
					int w = i - i_start;
					if (w >= 100) continue;
					m_widthHorizontalArray[w]++;
				}
				
			}
		}
	}
	
	WriteDebugStr("dddddddddddd" );
	WriteDebugStr("111111" );
	
	int allWidthCount = 0;
	for ( int w = 1; w < 50; w++)
	{
		allWidthCount += m_widthHorizontalArray[w];
	}
	WriteDebugStr("222222" );
	char str[1000];
	//sprintf(str, "\nm_fontsToCheck = %ld\n", (int) m_fontsToCheck);
	WriteDebugStr(str);
	sprintf(str, "\nallWidthCount = %d m_fontsToCheck->m_thickHorizontalBetween[1] = %f\n", allWidthCount,m_fontsToCheck->m_thickHorizontalBetween[1]); 
	WriteDebugStr(str);
	sprintf(str, "allWidthCount = %d m_fontsToCheck->m_thickHorizontalBetween[0] = %f\n", allWidthCount,m_fontsToCheck->m_thickHorizontalBetween[0]); 
	WriteDebugStr(str );
	
	
	int wHighCount = (int) ((float)allWidthCount * m_fontsToCheck->m_thickHorizontalBetween[1]);
	int wLowCount = (int) ((float)allWidthCount * m_fontsToCheck->m_thickHorizontalBetween[0]);;

	WriteDebugStr("3333333333333" );
	
	float aveLow = 0;
	int count = 0;
	int count1 = 0;
	int max_w = 0;
	int max_count_w = 0;
	for ( int w = 1; w < 50; w++)
	{
		if (count > wHighCount) break;
		count += m_widthHorizontalArray[w];
		if (count < wLowCount) continue;
		aveLow += w*m_widthHorizontalArray[w];
		count1 += m_widthHorizontalArray[w];
		if (max_count_w < m_widthHorizontalArray[w])
		{
			max_count_w = m_widthHorizontalArray[w];
			max_w = w;
		}
	}
	WriteDebugStr("4444444444444" );
	
	if (!count1) count1 = 1;
	aveLow /= (float) count1;
WriteDebugStr("eeeeeeeeeeeeeee" );
	
	//m_widthHorizontalLow = max_w;//aveLow;
	
	threshold = max_count_w / 2.0;


	for ( int w = max_w - 3; w <= max_w + 3; w++)
	{
		if ( w <= 0 ) continue;
		if (m_widthHorizontalArray[w] > threshold)
		{
			if (m_widthHorizontalLow_1 == 0) m_widthHorizontalLow_1 = w;
			m_widthHorizontalLow_2 = w;
		}
	}

	_sum = 0;
	_count = 0;
	for ( int w = m_widthHorizontalLow_1; w <= m_widthHorizontalLow_2; w++)
	{
		_sum += m_widthHorizontalArray[w]*w;
		_count += m_widthHorizontalArray[w];
	}
	
	if (_count == 0) _count = 1;

	m_widthHorizontalLow = _sum/(float)_count;




	int wHighCountLong = (int) ((float)allWidthCount * m_fontsToCheck->m_thickLongHorizontalBetween[1]);
	int wLowCountLong = (int) ((float)allWidthCount * m_fontsToCheck->m_thickLongHorizontalBetween[0]);
	
	float aveHigh= 0;
	count = 0;
	count1 = 0;
	max_w = 0;
	max_count_w = 0;
	for ( int w = 1; w < 50; w++)
	{
		if (count > wHighCountLong) break;
		count += m_widthHorizontalArray[w];
		if (count < wLowCountLong) continue;
		aveHigh += w*m_widthHorizontalArray[w];
		count1 += m_widthHorizontalArray[w];
		if (max_count_w < m_widthHorizontalArray[w])
		{
			max_count_w = m_widthHorizontalArray[w];
			max_w = w;
		}
		
	}
	if (!count1) count1 = 1;
	aveHigh /= (float) count1;
WriteDebugStr("fffffffffffffff" );
	

//	m_widthHorizontalHigh = max_w;//aveHigh;

	threshold = max_count_w / 2.0;


	for ( int w = max_w - 3; w <= max_w + 3; w++)
	{
		if ( w <= 0 ) continue;
		if (m_widthHorizontalArray[w] > threshold)
		{
			if (m_widthHorizontalHigh_1 == 0) m_widthHorizontalHigh_1 = w;
			m_widthHorizontalHigh_2 = w;
		}
	}

	_sum = 0;
	_count = 0;
	for ( int w = m_widthHorizontalHigh_1; w <= m_widthHorizontalHigh_2; w++)
	{
		_sum += m_widthHorizontalArray[w]*w;
		_count += m_widthHorizontalArray[w];
	}
	
	if (_count == 0) _count = 1;

	m_widthHorizontalHigh = _sum/(float)_count;


	


	
	//float allWidthBgHorizontal[100];
	for ( int w = 0; w < 100; w++)
	{
		m_widthBgHorizontalArray[w] = 0;
	}

	for ( int j = 0; j < m_height; j++)
	{
		int i_start = -1;	
		for ( int i = 1; i < m_width; i++)
		{
			if ( m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i-1])
			{
				if (m_pDataIn[j*m_width + i] != TextColor)
				{
					i_start = i;
				}
				else
				{
					if ( i_start == -1) continue;
					int w = i - i_start;
					if (w >= 100) continue;
					m_widthBgHorizontalArray[w]++;
				}
				
			}
		}
	}
WriteDebugStr("gggggggggggggg" );
	
	int allWidthBgCount = 0;
	for ( int w = 1; w < 100; w++)
	{
		allWidthBgCount += m_widthBgHorizontalArray[w];
	}
	int wBgHighCount = (int) ((float) allWidthBgCount * m_fontsToCheck->m_widthBgHorizontalBetween[1]);
	int wBgLowCount = (int)((float)allWidthBgCount * m_fontsToCheck->m_widthBgHorizontalBetween[0]);
	

	float aveBgHigh= 0;
	count = 0;
	count1 = 0;
	max_w = 0;
	max_count_w = 0;
	for ( int w = 99; w >= 1; w--)
	{
		if (count > wBgHighCount) break;
		count += m_widthBgHorizontalArray[w];
		if (count < wBgLowCount) continue;
		aveBgHigh += w*m_widthBgHorizontalArray[w];
		count1 += m_widthBgHorizontalArray[w];
		if (max_count_w < m_widthBgHorizontalArray[w])
		{
			max_count_w = m_widthBgHorizontalArray[w];
			max_w = w;
		}
	}
	if (!count1) count1 = 1;
	aveBgHigh /= (float) count1;


WriteDebugStr("hhhhhhhhhhhhhhhhh" );
	

//	m_widthVertical = widthVertical;


//	m_widthBgHorizontal = max_w;//aveBgHigh;



	threshold = max_count_w / 2.0;


	for ( int w = max_w - 3; w <= max_w + 3; w++)
	{
		if ( w <= 0 ) continue;
		if (m_widthBgHorizontalArray[w] > threshold)
		{
			if (m_widthBgHorizontal_1 == 0) m_widthBgHorizontal_1 = w;
			m_widthBgHorizontal_2 = w;
		}
	}

	_sum = 0;
	_count = 0;
	for ( int w = m_widthBgHorizontal_1; w <= m_widthBgHorizontal_2; w++)
	{
		_sum += m_widthBgHorizontalArray[w]*w;
		_count += m_widthBgHorizontalArray[w];
	}
	
	if (_count == 0) _count = 1;

	m_widthBgHorizontal = _sum/(float)_count;


	float ratioVerticalToHorizontalThick = m_widthVertical / m_widthHorizontalLow;



WriteDebugStr("iiiiiiiiiiiiiiiiii" );
	


	m_factorToApply = 1.0;
	m_fontApplied[0] = 0;
	for ( int i = 0; i < _MAX_NUMBER_FONTS_; i++)
	{
		OneFont * font = &m_fontsToCheck->m_fonts[i];
		if (!font->structInUse ) continue;
		if (m_widthHorizontalLow >= font->thickHorizontalLimits[0] && m_widthHorizontalLow <= font->thickHorizontalLimits[1] &&
			m_widthHorizontalHigh >= font->widthLongHorizontalLimits[0]  && m_widthHorizontalHigh <= font->widthLongHorizontalLimits[1] &&
			m_widthVertical >= font->thickVerticalLimits[0] && m_widthVertical <= font->thickVerticalLimits[1] &&
			ratioVerticalToHorizontalThick >= font->ratioHorizontalToVerticalThicksLimits[0]  && ratioVerticalToHorizontalThick <= font->ratioHorizontalToVerticalThicksLimits[1] &&
			m_widthBgHorizontal >= font->widthBgHorizontalLimits[0] && m_widthBgHorizontal <= font->widthBgHorizontalLimits[1] )

		{
			strncpy(m_fontApplied, font->font_name, 99);
			m_factorToApply = font->factorToApply;
			WriteDebugStr(" found" );
	
			return;
		}
	}
WriteDebugStr(" end " );
	m_factorToApply = m_fontsToCheck->m_defaultFactorToApply;
	return;
/*
	if (m_widthHorizontalLow > 3.5 && m_widthHorizontalLow < 6.5 &&
		m_widthHorizontalHigh > 14.0 && m_widthHorizontalHigh < 23.0 &&
		m_widthVertical > 5.0 && m_widthVertical < 8.5 &&
		factor > 1.0 && factor < 1.5 )
	{
		m_font = _ARIAL_FONT;
	}
	else if (m_widthHorizontalLow > 1.5 && m_widthHorizontalLow < 4.0 &&
		m_widthHorizontalHigh > 16.5 && m_widthHorizontalHigh < 22.5 &&
		m_widthVertical > 7.5 && m_widthVertical < 10.5 &&
		factor > 2.3 && factor < 5.0 )
	{
		m_font = _TIMES_NEW_ROMAN_FONT;
	}
	else if (m_widthHorizontalLow > 2.5 && m_widthHorizontalLow < 4.5 &&
		m_widthHorizontalHigh > 10.0 && m_widthHorizontalHigh < 16.5 &&
		m_widthVertical > 3.5 && m_widthVertical < 5.5 &&
		factor > 0.5 && factor < 3.5 )
	{
		m_font = _MIRIAM_FONT;
	}
	else if (m_widthHorizontalLow > 2.5 && m_widthHorizontalLow < 4.5 &&
		m_widthHorizontalHigh > 10.0 && m_widthHorizontalHigh < 16.5 &&
		m_widthVertical > 3.5 && m_widthVertical < 5.5&&
		factor > 0.5 && factor < 3.5 )
	{
		m_font = _MIRIAM_FIXED_FONT;
	}
*/
}
