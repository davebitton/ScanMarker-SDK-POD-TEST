//#include "StdAfx.h"
#include <stdio.h>
#include "SlowLength.h"
#include "xvbmp.h"

#define White 255
#define Black 0
#define APP_ENGINE_LNG_CHINESE_TRADITIONAL		221
#define APP_ENGINE_LNG_CHINESE_SIMPLIFIED		222
#define APP_ENGINE_LNG_JAPANESE 				278
#define APP_ENGINE_LNG_VERTICAL_CHINESE_TRADITIONAL		364
#define APP_ENGINE_LNG_VERTICAL_CHINESE_SIMPLIFIED		365
#define APP_ENGINE_LNG_VERTICAL_JAPANESE 				366

SlowLength::SlowLength(void)
{
	_UP_EDGE_  = 123;
	_DOWN_EDGE_  = 5;
	m_bodyHeight = 30;
	m_width = 0;
	m_height = 0;
	m_pDataIn = NULL;
	m_pDataOut = NULL;
	_HALF_NUMBER_CHANGES_TO_ANALIZE_ = 2;
	m_pChangesCoord = NULL;
	m_pNumberWhite = NULL;
	m_pNumberBlack = NULL;
	m_activeChangesIndex = 0;
	m_isInit = false;
	m_pDataOutInt = NULL;
	m_pDataOutInt1 = NULL;
	m_pDataOutInt2 = NULL;
	m_numberChangesInRow[0] = NULL;
	m_numberChangesInRow[1] = NULL;
	_PRINT_DEBUG_ = false;

}
SlowLength::~SlowLength()
{
	if (m_numberChangesInRow[0])
	{
		delete[] m_numberChangesInRow[0];
		m_numberChangesInRow[0]= NULL;
	}
	if (m_numberChangesInRow[1])
	{
		delete[] m_numberChangesInRow[1];
		m_numberChangesInRow[1] = NULL;
	}

	if (m_pChangesCoord != NULL)
	{
		delete[] m_pChangesCoord;
		m_pChangesCoord = NULL;
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

void SlowLength::AllocateMemory(bool flag)
{
	if (m_pChangesCoord != NULL)
	{
		delete[] m_pChangesCoord;
		m_pChangesCoord = NULL;
	}
	
	m_pChangesCoord = new int[_HALF_NUMBER_CHANGES_TO_ANALIZE_*2+2];



	if (m_pNumberWhite!= NULL)
	{
		delete[] m_pNumberWhite;
		m_pNumberWhite= NULL;
	}
	m_pNumberWhite= new int[_HALF_NUMBER_CHANGES_TO_ANALIZE_*2+2];

	if (m_pDataOutInt )
	{
		delete[] m_pDataOutInt ;
		m_pDataOutInt = NULL;
	}
	m_pDataOutInt = new int[m_width*m_height*3];
	memset(m_pDataOutInt , 0, m_width*m_height*3*sizeof(int));


	if (m_numberChangesInRow[0])
	{
		delete[] m_numberChangesInRow[0];
		m_numberChangesInRow[0]= NULL;
	}
	m_numberChangesInRow[0]= new int[m_height*3];
	memset(m_numberChangesInRow[0], 0, m_height*3*sizeof(int));

	if (m_numberChangesInRow[1])
	{
		delete[] m_numberChangesInRow[1];
		m_numberChangesInRow[1]= NULL;
	}
	m_numberChangesInRow[1]= new int[m_height*3];
	memset(m_numberChangesInRow[1], 0, m_height*3*sizeof(int));

	
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
	m_pNumberBlack= new int[_HALF_NUMBER_CHANGES_TO_ANALIZE_*2+2];
	m_activeChangesIndex = _HALF_NUMBER_CHANGES_TO_ANALIZE_*2+1;
	m_numberChangesFound = 0;
	m_isFullInfo = false;
	

}
void SlowLength::Init(int width, int height, byte* pDataIn, byte* pDataOut, byte* pDataOut1)
{
	m_width = width;
	m_height = height;
	m_pDataIn = pDataIn;
	m_pDataOut = pDataOut;
	m_pDataOut1 = pDataOut1;
	//AllocateMemory();
	m_isInit = true;
	


}
void SlowLength::ReadCfgFile(char* dir)
{
	return;
	char fileName[255];
	
	sprintf(fileName, "%s//%s", dir, "slowLength.cfg");
/*{
				FILE * fff = fopen("C://Temp//debug.txt", "at");
				if (fff)
				{
					fprintf(fff, "\n cfg = ", fileName );
					fclose(fff);
				}
			}*/

	//char dir1[1024];
	//GetCurrentDirectoryA(1024, dir1);
	//sprintf(fileName, "%s//%s", dir1, "SlowLength.cfg");
	//sprintf(fileName, "slowLength.cfg");
	FILE * fff = fopen(fileName, "rt");
	if (!fff) return;
/*{
				FILE * fff = fopen("C://Temp//debug.txt", "at");
				if (fff)
				{
					fprintf(fff, "opened");
					fclose(fff);
				}
			}*/

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
void SlowLength::DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue, float maxValue)
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
void SlowLength::SetParameters(int numberChanges, int goodNumberWhitesPerChanges, int goodPerBadColumn, int badsPerColumnPercent, int numberBadsForFail, int letterHeight, FILE* fStatistics)
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


_SLOW_MOTION_THRESHOLD_ = 600;//630;
_FAST_MOTION_THRESHOLD_ = 100;//630;
	/*_HALF_NUMBER_CHANGES_TO_ANALIZE_ = _HALF_NUMBER_CHANGES_TO_ANALIZE_;
	_GOOD_NUMBER_WHITES_PER_CHANGES_ = _GOOD_NUMBER_WHITES_PER_CHANGES_;
	_GOOD_PER_BAD_COLUMN_ = _GOOD_PER_BAD_COLUMN_;
	_BAD_PER_COLUMN_PERCENT_= _BAD_PER_COLUMN_PERCENT_;
	_NUMBER_BADS_FOR_FAIL_ = _NUMBER_BADS_FOR_FAIL_;
*/








	//AllocateMemory();

}
void SlowLength::ChangeFound()
{
	CalculateColor();
	for ( int i = 1; i <= m_activeChangesIndex; i++)
	{
		m_pChangesCoord[i-1] = m_pChangesCoord[i];
		m_pNumberBlack[i-1] = m_pNumberBlack[i];
		m_pNumberWhite[i-1] = m_pNumberWhite[i];
	}
	m_pChangesCoord[m_activeChangesIndex] = 0;
	m_pNumberBlack[m_activeChangesIndex] = 0;
	m_pNumberWhite[m_activeChangesIndex] = 0;

	m_numberChangesFound++;
	if (m_numberChangesFound >= m_activeChangesIndex)
		m_isFullInfo = true;

}
void SlowLength::CalculateColumnColors(int i, int &numberWhites, int &numberBlacks,  int &numberChanges)
{
	numberWhites = 0;
	numberBlacks = 0;

	int prevColor = m_pDataIn[i];
	for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
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
void SlowLength::CalculateNumberGoodsBads(int i, int &numberGoods, int &numberBads)
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

void SlowLength::SummaryByColumns(int index)
{
	//m_numberBadSlowColumns=0;
	for ( int i = 0; i < m_width-1; i++)
	{
	
		for (int j = 5; j < 120; j++)
		{
			min[j] = m_pDataOutInt[j*m_width+i];
			
			indexMin[j] = j;
		}
		for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
		{
			int minmin = min[j];
			int indexMinmin = j;
			for ( int j1 = j+1; j1 < _UP_EDGE_; j1++)
			{
				int value = min[j1];
				if (value < minmin)
				{
					minmin = value;
					indexMinmin = j1;
				}
			}

			min[indexMinmin]= min[j];
			min[j] = minmin;
			indexMin[indexMinmin] = j;
			indexMin[j] = indexMinmin;


		}

		int prevMin = 100000;
		int jPrevMin = -100;
		int ave = 0;
		int nAve = 0;
		for ( int ii = 0; ii< 10; ii++)
		{
			bool found = false;
			for ( int j = 5; j < 120; j++)
			{
				if ( min[j] > 99999) continue;
				if (min[j] == 0) continue;
				if ( min[j] < prevMin)
				{
					prevMin = min[j];
					jPrevMin = indexMin[j];
					found = true;
				}
			}
			if ( found)
			{
				ave += prevMin;
				nAve++;
				for ( int j = 5; j< 120; j++)
				{
					if (abs(indexMin[j] - jPrevMin) < _LETTER_HEIGHT_)
					{
						min[j] = 100000;
						indexMin[j] = -100;
					}
				}
			}
		}

		int length = -1;
		if (nAve > 0)
			length = ave / nAve;

		if ( index == 1)
			m_pDataOutInt1[i] = length;
		else
			m_pDataOutInt2[i] = length;

		if (m_fStatistics)
		{
			{
		if ( index == 1)
				fprintf(m_fStatistics, "%d in width i = %3d length = %10d\n", m_activeChangesIndex, i, length);
		else
				fprintf(m_fStatistics, "%d in width i = %3d length = %10d\n", m_activeChangesIndex, i, length);

			}
		}
	}
}
void SlowLength::FinishSummary()
{

	m_numberBadSlowColumns=0;
	m_numberGoodsSlowColumns = 0;
	m_numberBadFastColumns=0;
	m_numberGoodsFastColumns = 0;
	for ( int i = 0; i < m_width; i++)
	{

		if ((m_pDataOutInt1[i] ==-1))// &&(m_pDataOutInt2[i]==-1))
		{
			;
		}
		//else if ( (m_pDataOutInt1[i] > _RED_BAD_COLOR_THRESHOLD_1))
		else if ( (m_pDataOutInt1[i] > _SLOW_MOTION_THRESHOLD_))
			//&&
			//(m_pDataOutInt2[i] > _RED_BAD_COLOR_THRESHOLD_2))
		{
			/*for ( int jj = 0; jj < 10; jj++)
				m_pDataOut1[jj*m_width*3+i*3 + 0] = 255;
*/
			m_numberBadSlowColumns++;
		}
		else 
		{
			if (m_pDataOutInt1[i] < _GREEN_GOOD_COLOR_THRESHOLD_)
			{
				m_numberGoodsSlowColumns++;
			}
			/*for ( int jj = 0; jj < 10; jj++)
				m_pDataOut1[jj*m_width*3+i*3 + 1] = 255;
*/
		}

		if ((m_pDataOutInt1[i] ==-1))// &&(m_pDataOutInt2[i]==-1))
		{
			;
		}
		//else if ( (m_pDataOutInt1[i] > _RED_BAD_COLOR_THRESHOLD_1))
		else if ( (m_pDataOutInt1[i] > _FAST_MOTION_THRESHOLD_))
			
			//&&
			//(m_pDataOutInt2[i] > _RED_BAD_COLOR_THRESHOLD_2))
		{
			/*for ( int jj = 0; jj < 10; jj++)
				m_pDataOut1[jj*m_width*3+i*3 + 0] = 255;
*/
			m_numberGoodsFastColumns++;
		}
	}

/*{
				FILE * fff = fopen("C://Temp//debug.txt", "at");
				if (fff)
				{
					fprintf(fff, "\n m_result = %s" , (m_result? "true":"false"));
					fclose(fff);
				}
			}*/



	if ((m_numberBadSlowColumns > _MAX_NUMBER_BAD_COLUMNS_ ))// &&
	    //(m_numberGoodsColumns < _MAX_NUMBER_GOOD_COLUMNS_))
	{
		m_resultTooSlow = false;
	}
	else
	{
		m_resultTooSlow=true;
	}
	if ((m_numberGoodsFastColumns > _MAX_NUMBER_BAD_COLUMNS_ ))// &&
	    //(m_numberGoodsColumns < _MAX_NUMBER_GOOD_COLUMNS_))
	{
		m_resultTooFast = true;
	}
	else
	{
		m_resultTooFast=false;
	}

}

void SlowLength::CalculateColor()
{
	if (!m_isFullInfo) return;
	int i1 = (m_pChangesCoord[_HALF_NUMBER_CHANGES_TO_ANALIZE_-1] + m_pChangesCoord[_HALF_NUMBER_CHANGES_TO_ANALIZE_])/2;
	int i2 = (m_pChangesCoord[_HALF_NUMBER_CHANGES_TO_ANALIZE_] + m_pChangesCoord[_HALF_NUMBER_CHANGES_TO_ANALIZE_+1])/2;

/*	int i1 = m_pChangesCoord[0];// + m_pChangesCoord[_HALF_NUMBER_CHANGES_TO_ANALIZE_])/2;
	int i2 = m_pChangesCoord[m_activeChangesIndex];// + m_pChangesCoord[_HALF_NUMBER_CHANGES_TO_ANALIZE_+1])/2;
*/
	
	
	int numberWhites = 0;
	int numberBlacks= 0;





	for ( int i = 0; i <= m_activeChangesIndex; i++)
	{
		numberWhites += m_pNumberWhite[i];
		numberBlacks += m_pNumberBlack[i];
	}
   /*

	int diff = 0;
	int ave = numberWhites / (m_activeChangesIndex+1);
	for ( int i = 0; i <= m_activeChangesIndex; i++)
	{
		diff += abs(ave- m_pNumberWhite[i]);//*(ave- m_pNumberWhite[i]);
	}

	diff /= (m_activeChangesIndex+1);
*/

	int median = 0;
	
	/*for ( int i = 0; i <= m_activeChangesIndex/2; i++)
	{
		int min = 1000000;
		for ( int j = 0; j <= m_activeChangesIndex; j++)
		{

			if ((m_pNumberWhite[j] < min) && (m_pNumberWhite[j] > median))
			{
				min = m_pNumberWhite[j];
			}
		}
		median = min;
	}
*/

	int n = 0;
	float length = 0;
	for ( int i = 0; i < m_activeChangesIndex; i++)
	{
		//if (( abs(m_pChangesCoord[_HALF_NUMBER_CHANGES_TO_ANALIZE_-1] - m_pChangesCoord[i]) > 2000) ||
		//	( abs(m_pChangesCoord[_HALF_NUMBER_CHANGES_TO_ANALIZE_] - m_pChangesCoord[i+1]) > 2000))
		if ( abs(m_pChangesCoord[i+1]- m_pChangesCoord[i]) > 300)
			continue;
		if ((m_pChangesCoord[i+1] == 0) || (m_pChangesCoord[i] == 0))
			continue;
		length += m_pChangesCoord[i+1]- m_pChangesCoord[i];
		int i1 = m_pChangesCoord[i];
		int i2 = m_pChangesCoord[i+1];
		for ( int ii = i1; ii < i2; ii++)
		{
			bool isSpaceColumn = true;
			for ( int j = 1; j < m_height; j++)
			{
				if ( m_pDataIn[j*m_width+ii] != m_pDataIn[(j-1)*m_width+ii])
				{
					isSpaceColumn = false;
					break;
				}
			}
			if (isSpaceColumn)
			{
				length--;
			}
		}


		if (length > 0)
		n++;
	}

	if ( n > 0)
		length /= (float) n;
	
	//length *= (float) m_activeChangesIndex;
	float ratio = (float) length / (float)(_GOOD_NUMBER_WHITES_PER_CHANGES_);

	int redColor = (ratio - 1.0) * 127+127;
	if (redColor > 255) redColor = 255;
	if (redColor < 1 ) redColor = 1;
	int greenColor = (1.0 - ratio )* 127+127;
	if (greenColor > 255) greenColor= 255;
	if (greenColor < 1 ) greenColor= 1;

	if (length <0.001)
	{
		redColor = 0;
		greenColor = 0;
	}
	

	for ( int i = i1; i < i2; i++)
	{
	//if (n >= m_activeChangesIndex-1)
		m_pDataOutInt[m_j*m_width+i] = length*m_activeChangesIndex;
	//else
	//	m_pDataOutInt[m_j*m_width+i] = 0;
if (m_isOutput)
{
	m_pDataOut[m_j*m_width*3 + i*3+0] = redColor;
		m_pDataOut[m_j*m_width*3 + i*3+1] = greenColor;
		m_pDataOut[m_j*m_width*3 + i*3+2] = 0;
}
	}
}


void SlowLength::CalculateSlow()
{
m_isOutput = true;
	AllocateMemory(true);
	int firstWhiteColumn = m_width+1;
	int lastWhiteColumn = -1;
	m_allnumberChanges = 0;
	m_isShortWord = false;
	m_begin_x = m_width-1;
	m_end_x = 0;
		int numberChanges = 0;
	for ( int i = 1; i < m_width; i++)
	{
		for ( int j = 5; j < m_height - 5; j++)
		{
			if ( m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i-1])
			{
				numberChanges++;
				if ( m_begin_x > i) m_begin_x = i;
				if ( m_end_x < i ) m_end_x = i;
			}
		}
	}
	for (  m_j = _DOWN_EDGE_; m_j < _UP_EDGE_; m_j++)
	{
		byte prevprevColor = m_pDataIn[m_j*m_width];
		byte prevColor = m_pDataIn[m_j*m_width+1];
		int numberChanges = 0;
		for ( int i = 2; i < m_width; i++)
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
		_SLOW_MOTION_THRESHOLD_ /= oldActiveChangesIndex;
		_SLOW_MOTION_THRESHOLD_ *= m_activeChangesIndex;
		_FAST_MOTION_THRESHOLD_ /= oldActiveChangesIndex;
		_FAST_MOTION_THRESHOLD_ *= m_activeChangesIndex;
	
		_HALF_NUMBER_CHANGES_TO_ANALIZE_ = m_activeChangesIndex / 2-1;
		if (_HALF_NUMBER_CHANGES_TO_ANALIZE_ < 1)
			_HALF_NUMBER_CHANGES_TO_ANALIZE_ = 1;
	}

	for (  m_j = _DOWN_EDGE_; m_j < _UP_EDGE_; m_j++)
	{
		m_isFullInfo = false;
		for ( int i = 0; i<= m_activeChangesIndex; i++)
		{
			m_pNumberBlack[i] = 0;
			m_pNumberWhite[i] = 0;
			m_pChangesCoord[i] = 0;

		}
		m_numberChangesFound = 0;

		byte prevprevColor = m_pDataIn[m_j*m_width];
		byte prevColor = m_pDataIn[m_j*m_width+1];
		for ( int i = 0; i < m_width; i++)
		{
			byte color = m_pDataIn[m_j*m_width+i];
			if (color == Black)
			{
				m_pNumberBlack[m_activeChangesIndex-1]++;
			}
			if (color == White)
			{
				m_pNumberWhite[m_activeChangesIndex-1]++;
			}
			if ((color == Black) && (prevColor == White)&&(prevprevColor == White))
			{
				lastWhiteColumn = i;
			}
			if ((color == White) && (prevColor == Black)&&(prevprevColor == Black))
			{
				m_numberChangesInRow[0][m_j]++;
				if (firstWhiteColumn == -1)
				{
					firstWhiteColumn = i;
				}
				m_pChangesCoord[m_activeChangesIndex] = i;
				ChangeFound();
			}
			prevprevColor = prevColor;
			prevColor = color;
		}
	}
	if (m_language ==  APP_ENGINE_LNG_CHINESE_TRADITIONAL ||
		m_language ==  APP_ENGINE_LNG_CHINESE_SIMPLIFIED  ||
		m_language ==  APP_ENGINE_LNG_JAPANESE 			  ||
		m_language ==  APP_ENGINE_LNG_VERTICAL_CHINESE_TRADITIONAL		||
		m_language ==  APP_ENGINE_LNG_VERTICAL_CHINESE_SIMPLIFIED		||
		m_language ==  APP_ENGINE_LNG_VERTICAL_JAPANESE )
	{
	m_bodyHeight = CalculateBodyHeight();
	}
	float ratio = (float) m_bodyHeight / (float)_LETTER_HEIGHT_;
	_LETTER_HEIGHT_ = (int) ((float) _LETTER_HEIGHT_ * ratio);
	_RED_BAD_COLOR_THRESHOLD_1 = (int) ((float) _RED_BAD_COLOR_THRESHOLD_1 * ratio);
	_RED_BAD_COLOR_THRESHOLD_2 = (int) ((float) _RED_BAD_COLOR_THRESHOLD_2 * ratio);
	_GREEN_GOOD_COLOR_THRESHOLD_ = (int) ((float) _GREEN_GOOD_COLOR_THRESHOLD_ * ratio);
	_SLOW_MOTION_THRESHOLD_ = (int) ((float) _SLOW_MOTION_THRESHOLD_ * ratio);
	_FAST_MOTION_THRESHOLD_ = (int) ((float) _FAST_MOTION_THRESHOLD_ * ratio);



	//if ((lastWhiteColumn - firstWhiteColumn) > _MAX_NUMBER_BAD_COLUMNS_ / 2)
	//	_MAX_NUMBER_BAD_COLUMNS_ = (lastWhiteColumn - firstWhiteColumn) / 2;
	SummaryByColumns(1);



/*m_isOutput = false;

	//_HALF_NUMBER_CHANGES_TO_ANALIZE_ = 2;
	AllocateMemory(false);
	for (  m_j = 0; m_j < m_height; m_j++)
	{
		m_isFullInfo = false;
		for ( int i = 0; i<= m_activeChangesIndex; i++)
		{
			m_pNumberBlack[i] = 0;
			m_pNumberWhite[i] = 0;
			m_pChangesCoord[i] = 0;

		}
		m_numberChangesFound = 0;

		byte prevprevColor = m_pDataIn[m_j*m_width];
		byte prevColor = m_pDataIn[m_j*m_width+1];
		for ( int i = 0; i < m_width; i++)
		{
			byte color = m_pDataIn[m_j*m_width+i];
			if (color == Black)
			{
				m_pNumberBlack[m_activeChangesIndex-1]++;
			}
			if (color == White)
			{
				m_pNumberWhite[m_activeChangesIndex-1]++;
			}

			if ((color == White) && (prevColor == Black)&&(prevprevColor == Black))
			{
				m_pChangesCoord[m_activeChangesIndex] = i;
				ChangeFound();
			}
			prevprevColor = prevColor;
			prevColor = color;
		}
	}

	SummaryByColumns(2);*/
	FinishSummary();
}
int SlowLength::CalculateBodyHeight()
{
	int heights[128];
	for ( int h = 0; h < 128; h++)
	{
		heights[h] = 0;
	}
	int i1 = m_begin_x; 
	int i2 = 0;
	int numberSpaces = 0;
	int j1 = m_height - 1;
	int j2 = 0;
	int count = 0;
	for ( int i = m_begin_x+100 ; i < m_end_x-100 ; i +=25 )
	{
		j1 = m_height-1;
		j2 = 0;
		for ( int ii = i - 100; ii < i+100; ii++)
		{
			for ( int j = 1; j < m_height; j++)
			{
				if ( m_pDataIn[j*m_width+ii] != m_pDataIn[(j-1)*m_width+ii])
				{
					if ( j1 > j-1) j1 = j-1;
					if (j2 < j) j2 = j;
				}
			}
		}
		

		int h = j2 - j1;
		if ( h > 0 && h < 127)
			heights[h]++;
		
			}
	count = 0;
	for ( int h = 25; h < 127; h++)
		count += heights[h];

	int half = count *4/ 5;
	int hBody = 0;
	count = 0;
	for ( int h = 25; h < 127; h++)
	{
		count += heights[h];
		if (count > half)
		{
			hBody = h;
			break;
		}
	}


	return hBody;


}
/*
int SlowLength::CalculateBodyHeight()
{
	for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
	{
		byte prevprevColor = m_pDataIn[j*m_width];
		byte prevColor = m_pDataIn[j*m_width+1];
		int start = -1;
		int finish = -1;
		for ( int i = 0; i < m_width; i++)
		{
			byte color = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3+i*3+0] = color;
			m_pDataOut[j*m_width*3+i*3+2] = color;

			if ((color == Black) && (prevColor == White)&&(prevprevColor == White))
			{
			//}

			//if ((color == White) && (prevColor == Black)&&(prevprevColor == Black))
			//{
				finish = i;

				if ( (start != -1) && ((finish - start) < 350) )
				{
					for ( int i1 = start; i1 < finish; i1++)
					{
						m_pDataOut[j*m_width*3+i1*3+1] = 100;
					}
				}
				start = i;
			
			}

			prevprevColor = prevColor;
			prevColor = color;
		}
	}

	for ( int i = 0; i < m_width; i++)
	{
		byte prevprevColor = m_pDataOut[0*m_width*3+i*3+1];
		byte prevColor = m_pDataOut[1*m_width*3+i*3+1];
		int start= -1;
		int finish = -1;
		for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
		{

			byte color = m_pDataOut[j*m_width*3+i*3+1];
			if ((color == 100) && (prevColor != 100)&&(prevprevColor != 100))
			{
				start = j;
			}
	
			if ((color != 100) && (prevColor == 100)&&(prevprevColor == 100))
			{
			
				finish = j;

				if ( (start != -1) && ((finish - start) < 350) )
				{
					int h = finish - start +1;
					if ((h >= 0) && (h < m_height))
						m_numberChangesInRow[0][h]++;

				}
			}

			prevprevColor = prevColor;
			prevColor = color;
		}
	}

/*	for ( int j = 0; j < m_height; j++)
	{
		byte prevprevColor = m_pDataIn[j*m_width];
		byte prevColor = m_pDataIn[j*m_width+1];
		for ( int i = 0; i < m_width; i++)
		{
			byte color = m_pDataIn[j*m_width+i];
			if ((color == White) && (prevColor == Black)&&(prevprevColor == Black))
			{
				m_numberChangesInRow[0][j]++;
			}
			prevprevColor = prevColor;
			prevColor = color;
		}
	}
* /

	int max = 0;
	int jMax = 30;
	for ( int j = 10; j < m_height-2; j++)
	{
		int current = m_numberChangesInRow[0][j-2];
		current += m_numberChangesInRow[0][j-1];
		current += m_numberChangesInRow[0][j];
		current += m_numberChangesInRow[0][j+1];
		current += m_numberChangesInRow[0][j+2];
		if (current > max)
		{
			max = current;
			jMax = j;
		}

	}
	for ( int i = 0; i < m_width; i++)
	{
		byte prevprevColor = m_pDataOut[0*m_width*3+i*3+1];
		byte prevColor = m_pDataOut[1*m_width*3+i*3+1];
		int start= -1;
		int finish = -1;
		for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
		{

			byte color = m_pDataOut[j*m_width*3+i*3+1];
			if ((color == 100) && (prevColor != 100)&&(prevprevColor != 100))
			{
				start = j;
			}
	
			if ((color != 100) && (prevColor == 100)&&(prevprevColor == 100))
			{
			
				finish = j;

				if ( (start != -1) && ((finish - start) < 50) )
				{
					int h = finish - start +1;
					if (( h >= jMax-4 ) && ( h <= jMax+4))
					{
			//			m_isGoodColumn[i] = finish;						
					}

				}
			
			}

			prevprevColor = prevColor;
			prevColor = color;
		}
	}

/*	
	for ( int i = 0; i < m_width; i++)
	{

		for ( int j = 0; j < m_height; j++)
		{
			byte value = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 + 0] = value;
			m_pDataOut[j*m_width*3 + i*3 + 1] = value;
			m_pDataOut[j*m_width*3 + i*3 + 2] = value;

		}
	}
	for ( int i = 0; i < m_width; i++)
	{

		for ( int j = 0; j < m_height; j++)
		{
			if (m_pDataIn[j*m_width+i] == 100)
				m_pDataIn[j*m_width+i] = Black;
		}
	}* /
	return jMax;
}*/
