//#include "StdAfx.h"
#include <stdio.h>
#include "Jerky_new.h"

#define White 255
#define Black 0
#define TextColor 255
Jerky_new::Jerky_new(void)
{
	m_width = 0;
	m_height = 0;
	m_pDataIn = NULL;
	m_pDataOut = NULL;
	_HALF_NUMBER_CHANGES_TO_ANALIZE_ = 2;
	m_pChangesCoord = NULL;
	m_pChangesCoord1 = NULL;
	m_pNumberWhite = NULL;
	m_pNumberBlack = NULL;
	m_activeChangesIndex = 0;
	m_isInit = false;
	m_pDataOutInt = NULL;
	m_pDataOutInt1 = NULL;
	m_pDataOutInt2 = NULL;


}
Jerky_new::~Jerky_new()
{
	if (m_pChangesCoord != NULL)
	{
		delete[] m_pChangesCoord;
		m_pChangesCoord = NULL;
	}
	if (m_pChangesCoord1 != NULL)
	{
		delete[] m_pChangesCoord1;
		m_pChangesCoord1 = NULL;
	}
	if (m_pNumberWhite!= NULL)
	{
		delete[] m_pNumberWhite;
		m_pNumberWhite= NULL;
	}

	if (m_pDataOutInt )
	{
		delete[] m_pDataOutInt ;
		m_pDataOutInt = NULL;
	}

	if (m_pDataOutInt1 )
	{
		delete[] m_pDataOutInt1 ;
		m_pDataOutInt1 = NULL;
	}
	if (m_pDataOutInt2 )
	{
		delete[] m_pDataOutInt2 ;
		m_pDataOutInt2 = NULL;
	}
	
	if (m_pNumberBlack!= NULL)
	{
		delete[] m_pNumberBlack;
		m_pNumberBlack= NULL;
	}
}

void Jerky_new::AllocateMemory(bool flag)
{
	if (m_pChangesCoord != NULL)
	{
		delete[] m_pChangesCoord;
		m_pChangesCoord = NULL;
	}
	
	m_pChangesCoord = new int[_HALF_NUMBER_CHANGES_TO_ANALIZE_*2+4];

	if (m_pChangesCoord1 != NULL)
	{
		delete[] m_pChangesCoord1;
		m_pChangesCoord1 = NULL;
	}
	
	m_pChangesCoord1 = new int[_HALF_NUMBER_CHANGES_TO_ANALIZE_*2+4];


	if (m_pNumberWhite!= NULL)
	{
		delete[] m_pNumberWhite;
		m_pNumberWhite= NULL;
	}
	m_pNumberWhite= new int[_HALF_NUMBER_CHANGES_TO_ANALIZE_*2+4];

	if (m_pDataOutInt )
	{
		delete[] m_pDataOutInt ;
		m_pDataOutInt = NULL;
	}
	m_pDataOutInt = new int[m_width*m_height*3];
	memset(m_pDataOutInt , 0, m_width*m_height*3*sizeof(int));
if (flag)
{
	if (m_pDataOutInt1 )
	{
		delete[] m_pDataOutInt1 ;
		m_pDataOutInt1 = NULL;
	}
	m_pDataOutInt1 = new int[m_width*3];
	memset(m_pDataOutInt1 , 0, m_width*3*sizeof(int));
}
	if (m_pDataOutInt2 )
	{
		delete[] m_pDataOutInt2 ;
		m_pDataOutInt2 = NULL;
	}
	m_pDataOutInt2 = new int[m_width*3];
	memset(m_pDataOutInt2 , 0, m_width*3*sizeof(int));

	if (m_pNumberBlack!= NULL)
	{
		delete[] m_pNumberBlack;
		m_pNumberBlack= NULL;
	}
	m_pNumberBlack= new int[_HALF_NUMBER_CHANGES_TO_ANALIZE_*2+4];
	m_activeChangesIndex = _HALF_NUMBER_CHANGES_TO_ANALIZE_*2+3;
	m_numberChangesFound = 0;
	m_isFullInfo = false;
	

}
void Jerky_new::Init(int width, int height, byte* pDataIn, byte* pDataOut, byte* pDataOut1)
{
	m_width = width;
	m_height = height;
	m_pDataIn = pDataIn;
	m_pDataOut = pDataOut;
	m_pDataOut1 = pDataOut1;
	//AllocateMemory();
	m_isInit = true;


}
void Jerky_new::ReadCfgFile(char* dir)
{
	return;
	char fileName[255];

          	sprintf(fileName, "%s//%s", dir, "Jerky_new.cfg");
	//char dir1[1024];
	//GetCurrentDirectoryA(1024, dir1);
	//sprintf(fileName, "%s//%s", dir1, "Jerky_new.cfg");
	//sprintf(fileName, "Jerky_new.cfg");
	FILE * fff = fopen(fileName, "rt");
	if (!fff) return;

	char str[255];
	while (fgets(str, 254, fff))
	{
		if (str[0] == '#') continue;

		
		if ( strstr(str, "_LETTER_HEIGHT_"))
		{
			sscanf(strstr(str, "_LETTER_HEIGHT_") + strlen("_LETTER_HEIGHT_"), "%ld", &_LETTER_HEIGHT_);
		}
		if ( strstr(str, "_HALF_NUMBER_CHANGES_TO_ANALIZE_"))
		{
			sscanf(strstr(str, "_HALF_NUMBER_CHANGES_TO_ANALIZE_") + strlen("_HALF_NUMBER_CHANGES_TO_ANALIZE_"), "%ld", &_HALF_NUMBER_CHANGES_TO_ANALIZE_);
		}
		if ( strstr(str, "_GOOD_NUMBER_WHITES_PER_CHANGES_"))
		{
			sscanf(strstr(str, "_GOOD_NUMBER_WHITES_PER_CHANGES_") + strlen("_GOOD_NUMBER_WHITES_PER_CHANGES_"), "%ld", &_GOOD_NUMBER_WHITES_PER_CHANGES_);
		}

		if ( strstr(str, "_GOOD_PER_BAD_COLUMN_ "))
		{
			sscanf(strstr(str, "_GOOD_PER_BAD_COLUMN_ ") + strlen("_GOOD_PER_BAD_COLUMN_ "), "%ld", &_GOOD_PER_BAD_COLUMN_ );
		}
		if ( strstr(str, "_BAD_PER_COLUMN_PERCENT_"))
		{
			sscanf(strstr(str, "_BAD_PER_COLUMN_PERCENT_") + strlen("_BAD_PER_COLUMN_PERCENT_"), "%ld", &_BAD_PER_COLUMN_PERCENT_);
		}
		if ( strstr(str, "_MAX_NUMBER_BAD_COLUMNS_ "))
		{
			sscanf(strstr(str, "_MAX_NUMBER_BAD_COLUMNS_ ") + strlen("_MAX_NUMBER_BAD_COLUMNS_ "), "%ld", &_MAX_NUMBER_BAD_COLUMNS_ );
		}
		if ( strstr(str, "_RED_BAD_COLOR_THRESHOLD_1"))
		{
			sscanf(strstr(str, "_RED_BAD_COLOR_THRESHOLD_1") + strlen("_RED_BAD_COLOR_THRESHOLD_1"), "%ld", &_RED_BAD_COLOR_THRESHOLD_1);
		}
		if ( strstr(str, "_RED_BAD_COLOR_THRESHOLD_2"))
		{
			sscanf(strstr(str, "_RED_BAD_COLOR_THRESHOLD_2") + strlen("_RED_BAD_COLOR_THRESHOLD_2"), "%ld", &_RED_BAD_COLOR_THRESHOLD_2);
		}
		if ( strstr(str, "_GREEN_GOOD_COLOR_THRESHOLD_"))
		{
			sscanf(strstr(str, "_GREEN_GOOD_COLOR_THRESHOLD_") + strlen("_GREEN_GOOD_COLOR_THRESHOLD_"), "%ld", &_GREEN_GOOD_COLOR_THRESHOLD_);
		}
		if ( strstr(str, "_MAX_NUMBER_GOOD_COLUMNS_"))
		{
			sscanf(strstr(str, "_MAX_NUMBER_GOOD_COLUMNS_") + strlen("_MAX_NUMBER_GOOD_COLUMNS_"), "%ld", &_MAX_NUMBER_GOOD_COLUMNS_);
		}
		
	}


	fclose(fff);
}
void Jerky_new::SetParameters(int numberChanges, int goodNumberWhitesPerChanges, int goodPerBadColumn, int badsPerColumnPercent, int numberBadsForFail, int letterHeight, FILE* fStatistics)
{


 m_fStatistics = fStatistics;
 _LETTER_HEIGHT_ = letterHeight;
_HALF_NUMBER_CHANGES_TO_ANALIZE_ = 5;
_GOOD_NUMBER_WHITES_PER_CHANGES_ = 25;
_GOOD_PER_BAD_COLUMN_ = 20;
_BAD_PER_COLUMN_PERCENT_ = 40;
_MAX_NUMBER_BAD_COLUMNS_  = 1000;
_MAX_NUMBER_GOOD_COLUMNS_ = 1000;
_RED_BAD_COLOR_THRESHOLD_1 = 800;
_RED_BAD_COLOR_THRESHOLD_2 = 800;
_GREEN_GOOD_COLOR_THRESHOLD_ = 800;


	/*_HALF_NUMBER_CHANGES_TO_ANALIZE_ = _HALF_NUMBER_CHANGES_TO_ANALIZE_;
	_GOOD_NUMBER_WHITES_PER_CHANGES_ = _GOOD_NUMBER_WHITES_PER_CHANGES_;
	_GOOD_PER_BAD_COLUMN_ = _GOOD_PER_BAD_COLUMN_;
	_BAD_PER_COLUMN_PERCENT_= _BAD_PER_COLUMN_PERCENT_;
	_NUMBER_BADS_FOR_FAIL_ = _NUMBER_BADS_FOR_FAIL_;
*/








	//AllocateMemory();

}
void Jerky_new::ChangeFound()
{
	CalculateColor();
	for ( int i = 1; i <= m_activeChangesIndex; i++)
	{
		m_pChangesCoord[i-1] = m_pChangesCoord[i];
		m_pChangesCoord1[i-1] = m_pChangesCoord1[i];
		m_pNumberBlack[i-1] = m_pNumberBlack[i];
		m_pNumberWhite[i-1] = m_pNumberWhite[i];
	}
	m_pChangesCoord1[m_activeChangesIndex] = 0;
	m_pChangesCoord[m_activeChangesIndex] = 0;
	m_pNumberBlack[m_activeChangesIndex] = 0;
	m_pNumberWhite[m_activeChangesIndex] = 0;

	m_numberChangesFound++;
	if (m_numberChangesFound >= m_activeChangesIndex-1)
		m_isFullInfo = true;

}
void Jerky_new::CalculateColumnColors(int i, int &numberWhites, int &numberBlacks,  int &numberChanges)
{
	numberWhites = 0;
	numberBlacks = 0;

	int prevColor = m_pDataIn[i];
	for ( int j = 5; j < m_height-8; j++)
	{
		int color = m_pDataIn[j*m_width+i];
		if (color == Black)
			numberBlacks++;

		if (color  == White)
			numberWhites++;

		if (prevColor != color)
		{
			numberChanges ++;
		}
		prevColor = color ;
	}
}
void Jerky_new::CalculateNumberGoodsBads(int i, int &numberGoods, int &numberBads)
{
	numberGoods = 0;
	numberBads = 0;
	/*for ( int j = 5; j < m_height-8; j++)
	{
		if (m_pDataOut[j*m_width*3+i*3+0] > _RED_BAD_COLOR_THRESHOLD_)
		{
			numberBads++;
		}
		if (m_pDataOut[j*m_width*3+i*3+1] > _GREEN_GOOD_COLOR_THRESHOLD_)
		{
			numberGoods++;
		}

	}*/

	
}

void Jerky_new::SummaryByColumns(int index)
{
	for ( int i = 0; i < m_width-1; i++)
	{
		int numberBadInColumn=0;
		for (int j = 0; j < 120; j++)
		{
			if (m_pDataOutInt[j*m_width+i] ==255)
			{
				numberBadInColumn++;
			}
		}
		m_pDataOutInt1[i] = numberBadInColumn;
	}
}
void Jerky_new::FinishSummary()
{
	m_numberBadColumns=0;
	m_numberGoodsColumns = 0;
	int m_numberBadPieces = 0;
	bool isBad = false;
	for ( int i = 0; i < m_width; i++)
	{

		if (m_pDataOutInt1[i] >= 4)
		{
			for ( int jj = 0; jj < 10; jj++)
				m_pDataOut1[jj*m_width*3+i*3 + 0] = 255;


			isBad=true;
			m_numberBadColumns++;
		}
		else 
		{
			if (isBad)
			{
				m_numberBadPieces++;
			}
			isBad = false;

			m_numberGoodsColumns++;
			for ( int jj = 0; jj < 10; jj++)
				m_pDataOut1[jj*m_width*3+i*3 + 1] = 255;
		}

	}
	if ((m_numberBadPieces >= 2 ))// &&
	    /*(m_numberGoodsColumns < _MAX_NUMBER_GOOD_COLUMNS_))*/
	{
		m_result = false;
	}
	else
	{
		m_result=true;
	}
	
/*	if ((m_numberBadColumns > _MAX_NUMBER_BAD_COLUMNS_ ))// &&
	    /*(m_numberGoodsColumns < _MAX_NUMBER_GOOD_COLUMNS_))* /
	{
		m_result = false;
	}
	else
	{
		m_result=true;
	}
*/
}

void Jerky_new::CalculateColor()
{
	if (!m_isFullInfo) return;
	int i1 = 0;
	int i2 = 0;


	int numberLong = 0;
	int numberShort = 0;
	int max = 0;
	int min1 = 10000;
	int min2 = 10000;
	int min3 = 10000;
	for ( int i = 1; i <= m_activeChangesIndex-1; i++)
	{
/*		if ( m_pNumberWhite[i] > 200)
			numberLong++;
		if ( m_pNumberWhite[i] < 50)
			numberShort++;
*/
		if (max < m_pNumberWhite[i])
		{
			i1 = m_pChangesCoord[i];
			i2 = m_pChangesCoord1[i];
			max = m_pNumberWhite[i];
		}

		if ( m_pNumberWhite[i] < min1)
		{
			min3 = min2;
			min2 = min1;
			min1 = m_pNumberWhite[i];
		}
		else if ( (m_pNumberWhite[i] > min1) && (m_pNumberWhite[i] < min2))
		{
			min3 = min2;
			min2 = m_pNumberWhite[i];
		}
		else if ( (m_pNumberWhite[i] > min2) && (m_pNumberWhite[i] < min3))
		{
			min3 = m_pNumberWhite[i];
		}
	}
	if (m_activeChangesIndex <=3)
	{
		min3 = min1;
	}
	if (min3 == 10000) min3 = min2;
	if (min3 == 10000) min3 = min1;

	int redColor = 0;
	int greenColor = 0;

	if ((( (float)max / (float)min3) > 10) && (max > 150))
	{
		redColor = 255;
	}
	else
	{
		greenColor = 255;
	}
	if ((i1 == 0) || (i2 == 0)) return;
	for ( int i = i1; i < i2; i++)
	{
		if (redColor == 255)
			m_pDataOutInt[m_j*m_width+i] = 255;
		else
			m_pDataOutInt[m_j*m_width+i] = 0;

		if (m_isOutput)
		{
			m_pDataOut[m_j*m_width*3 + i*3+0] = redColor;
			m_pDataOut[m_j*m_width*3 + i*3+1] = greenColor;
			m_pDataOut[m_j*m_width*3 + i*3+2] = 0;
		}
	}
}
void  Jerky_new::RemoveHorizontalLines()
{
	int begin_x = m_width-1;
	int end_x = 0;
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			if (m_pDataIn[j*m_width+i] == TextColor || m_pDataIn[j*m_width+i] == 127)
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
		for ( int i = begin_x; i < m_width-500; i+=250)
		{
			int countText = 0;
			int countBG = 0;
			int maxCountText = 0;
			for ( int ii = i; ii < i + 500; ii++)
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
			if (maxCountText > 450)
			{ 
				int numberMaavarimUp = 0;
				int numberMaavarimDown = 0;
				for ( int ii = i; ii < i + 499; ii++)
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
				for ( int ii = i; ii < i + 500; ii++)
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
		}	
		int i = end_x - 500+5;
		if (i < 0) i = 0;
		int countText = 0;
		int countBG = 0;
		int maxCountText = 0;
		for ( int ii = i; ii < i + 500; ii++)
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
		if (maxCountText > 450)
		{ 
			int numberMaavarimUp = 0;
			int numberMaavarimDown = 0;
			for ( int ii = i; ii < i + 499; ii++)
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
			for ( int ii = i; ii < i + 500; ii++)
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
	}
}
void Jerky_new::CalculateJerky()
{
m_isOutput = true;
	AllocateMemory(true);
	int firstWhiteColumn = m_width+1;
	int lastWhiteColumn = -1;
	m_allnumberChanges = 0;
	m_isShortWord = false;
	int begin_x = 5;
	int end_x = m_width-5;
	int changes[128];
	RemoveHorizontalLines();
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
	for ( int c = 0; c < 128; c++)
		changes[c] = 0;

	for ( int i = 5; i < m_width - 1; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i-1])
			{
				changes[j]++;
			}
			if (changes[j] >= 2)
			{
				begin_x = i-1;
				break;
			}
		}
		if (begin_x > 5) break;
	}
	for ( int c = 0; c < 128; c++)
		changes[c] = 0;

	for ( int i = m_width - 5; i >= 1; i--)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i-1])
			{
				changes[j]++;
			}
			if (changes[j] >= 5)
			{
				end_x = i+1;
				break;
			}
		}
		if (end_x < m_width-5) break;
	}
	for (  m_j = 0; m_j < m_height; m_j++)
	{
		byte prevprevColor = m_pDataIn[m_j*m_width+begin_x];
		byte prevColor = m_pDataIn[m_j*m_width+1+begin_x];
		int numberChanges = 0;
		for ( int i = begin_x+1; i < end_x; i++)
		{
			byte color = m_pDataIn[m_j*m_width+i];
			if ((color == Black) && (prevColor == White)&&(prevprevColor == White))
			{
				if (lastWhiteColumn < i)
				{
					lastWhiteColumn = i;
				}

			}
			if ((color == White) && (prevColor == Black)&&(prevprevColor == Black))
			{

				if (firstWhiteColumn > i)
				{
					firstWhiteColumn = i;
				}
				numberChanges++;
			}
			prevprevColor = prevColor;
			prevColor = color;
		}
		if ( m_allnumberChanges < numberChanges)
		{
			m_allnumberChanges = numberChanges;
		}
	}
	_MAX_NUMBER_BAD_COLUMNS_  = (lastWhiteColumn - firstWhiteColumn) / 6;
	if (m_allnumberChanges <= 3) return;
	if (m_allnumberChanges <= m_activeChangesIndex*2)
	{
		m_isShortWord = true;
		int oldActiveChangesIndex = m_activeChangesIndex;
		m_activeChangesIndex = m_allnumberChanges / 3;
		if (m_activeChangesIndex < 2)
		{
			m_activeChangesIndex = 2;
		}
		_RED_BAD_COLOR_THRESHOLD_1 /= oldActiveChangesIndex;
		_RED_BAD_COLOR_THRESHOLD_1 *= m_activeChangesIndex;
	
		_HALF_NUMBER_CHANGES_TO_ANALIZE_ = m_activeChangesIndex / 2-1;
		if (_HALF_NUMBER_CHANGES_TO_ANALIZE_ < 1)
			_HALF_NUMBER_CHANGES_TO_ANALIZE_ = 1;
	}

	for (  m_j = 1; m_j < m_height-1; m_j++)
	{
		m_isFullInfo = false;
		for ( int i = 0; i<= m_activeChangesIndex; i++)
		{
			m_pNumberBlack[i] = 0;
			m_pNumberWhite[i] = 0;
			m_pChangesCoord[i] = 0;
			m_pChangesCoord1[i] = 0;

		}
		m_numberChangesFound = 0;

		byte prevprevColor = m_pDataIn[m_j*m_width+begin_x];
		byte prevColor = m_pDataIn[m_j*m_width+1+begin_x];
		for ( int i = begin_x+1; i < end_x; i++)
		{
			byte color = m_pDataIn[m_j*m_width+i];
			if (color == Black)
			{
				m_pNumberBlack[m_activeChangesIndex]++;
			}
			if (color == White)
			{
				m_pNumberWhite[m_activeChangesIndex]++;
			}

			if ((color == White) && (prevColor == Black)&&(prevprevColor == Black))
			{
				m_pChangesCoord[m_activeChangesIndex] = i;
			}
			if ((color == Black) && (prevColor == White)&&(prevprevColor == White))
			{
				m_pChangesCoord1[m_activeChangesIndex] = i;
				ChangeFound();
			}
			prevprevColor = prevColor;
			prevColor = color;
		}
	}
	SummaryByColumns(1);


	FinishSummary();
}
