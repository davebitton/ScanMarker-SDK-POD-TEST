//#include "StdAfx.h"
#include <stdio.h>
#include "TextIsOut.h"
#define TextColor 255

#define White 255
#define Black 0


TextIsOut::TextIsOut(void)
{
	_UP_EDGE_  = 122;
	_DOWN_EDGE_  = 6;
	_BAD_RESULT_BODY_PERCENT_= 0.9;

	m_isStart = true;
	m_aveHeight  = 0;
	m_numberInAve = 0;
	m_TextOut_first_i = -1;
	m_number_i = 0;

	m_bodyHeight = 30; 
	m_width = 0;
	m_height = 0;
	m_pDataIn = NULL;
	m_pDataOut = NULL;
	m_pWorkData = 0;
	m_numberChanges = 2;
	_HALF_NUMBER_CHANGES_TO_ANALIZE_ = 2;
	for ( int j = 0; j < 127; j++)
	{
		m_pChangesCoord[j] = NULL;
		m_isFull[j] = NULL;
		m_pNumberWhite[j] = NULL;
		m_pNumberBlack[j] = NULL;
	}
	m_activeChangesIndex = 0;
	m_isInit = false;
	
	m_isFirstHistogram[0] = true;
	m_isFirstHistogram[1] = true;
	m_isFirstHistogram[2] = true;
	m_middle[1] = 64;
	m_up[1] = _UP_EDGE_-4;
	m_down[1] = _DOWN_EDGE_+4;
	m_numberBadColumns = 0;
	_SEARCH_START_MIDDLE_ = 64;
	_SEARCH_START_RANGE_ = 15;



	/*m_middle[0] = 64;
	m_up[0] = 80;
	m_down[0] = 50;

	m_middle[2] = 64;
	m_up[2] = 80;
	m_down[2] = 50;

*/




	m_middle[0] = 100;
	m_up[0] = 120;
	m_down[0] = 80;

	m_middle[2] = 18;
	m_up[2] = 30;
	m_down[2] = 5;
	m_numberChangesInRow[0] = NULL;
	m_numberChangesInRow[1] = NULL;


}
 TextIsOut::~TextIsOut()
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

	for ( int j = 0; j < 127; j++)
	{
		if (m_pChangesCoord[j] != NULL)
		{
			delete[] m_pChangesCoord[j];
			m_pChangesCoord[j] = NULL;
		}
		
	
		if (m_isFull[j] != NULL)
		{
			delete[] m_isFull[j];
			m_isFull[j] = NULL;
		}
		


		if (m_pNumberWhite[j] != NULL)
		{
			delete[] m_pNumberWhite[j];
			m_pNumberWhite[j]= NULL;
		}


		if (m_pNumberBlack[j]!= NULL)
		{
			delete[] m_pNumberBlack[j];
			m_pNumberBlack[j] = NULL;
		}

		if (m_pWorkData != NULL)
		{
			delete[] m_pWorkData;
			m_pWorkData = NULL;
		}
		

	}
}
void TextIsOut::AllocateMemory()
{
	m_activeChangesIndex = _HALF_NUMBER_CHANGES_TO_ANALIZE_*2;
	for ( int j = 0; j < 127; j++)
	{
		if (m_pChangesCoord[j] != NULL)
		{
			delete[] m_pChangesCoord[j];
			m_pChangesCoord[j] = NULL;
		}
		
		m_pChangesCoord[j] = new int[m_activeChangesIndex+1];
	
		if (m_isFull[j] != NULL)
		{
			delete[] m_isFull[j];
			m_isFull[j] = NULL;
		}
		
		m_isFull[j] = new bool[m_activeChangesIndex+1];


		if (m_pNumberWhite[j] != NULL)
		{
			delete m_pNumberWhite[j];
			m_pNumberWhite[j]= NULL;
		}
		m_pNumberWhite[j]= new int[m_activeChangesIndex+1];


		if (m_pNumberBlack[j]!= NULL)
		{
			delete[] m_pNumberBlack[j];
			m_pNumberBlack[j] = NULL;
		}
		m_pNumberBlack[j]= new int[m_activeChangesIndex+1];

		
		for ( int index = 0; index <=m_activeChangesIndex;index++)
		{
			m_isFull[j][index] = 0;
			m_pNumberBlack[j][index] = 0;
			m_pNumberWhite[j][index] = 0;
			m_pChangesCoord[j][index] = 0;
		}
	}
	m_numberChangesFound = 0;
	m_isFullInfo = false;
	m_From[0] = 0;
	m_To[0] = m_width-1;
	m_From[1] = 0;
	m_To[1] = m_width-1;
	m_From[2] = 0;
	m_To[2] = m_width-1;
	if (m_numberChangesInRow[0])
	{
		delete[] m_numberChangesInRow[0];
		m_numberChangesInRow[0]= NULL;
	}
	m_numberChangesInRow[0]= new int[m_height];
	memset(m_numberChangesInRow[0], 0, m_height*sizeof(int));

	if (m_numberChangesInRow[1])
	{
		delete[] m_numberChangesInRow[1];
		m_numberChangesInRow[1]= NULL;
	}
	m_numberChangesInRow[1]= new int[m_height];
	memset(m_numberChangesInRow[1], 0, m_height*sizeof(int));
	if (m_pWorkData != NULL)
	{
		delete[] m_pWorkData;
		m_pWorkData = NULL;
	}
	m_pWorkData =  new byte[m_width];
	memset(m_pWorkData, 0, m_width*sizeof(byte));


}

void TextIsOut::Init(int width, int height, byte* pDataIn, byte* pDataOut, byte* pDataOut1)
{
	m_width = width;
	m_height = height;
	m_pDataIn = pDataIn;
	m_pDataOut = pDataOut;
	m_pDataOut1 = pDataOut1;
	//AllocateMemory();
	m_isInit = true;



}
void TextIsOut::ReadCfgFile(char* dir)
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

		if ( strstr(str, "_BAD_RESULT_BODY_PERCENT_"))
		{
			sscanf(strstr(str, "_BAD_RESULT_BODY_PERCENT_") + strlen("_BAD_RESULT_BODY_PERCENT_"), "%f", &_BAD_RESULT_BODY_PERCENT_);
		}

		if ( strstr(str, "_HALF_NUMBER_CHANGES_TO_ANALIZE_"))
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
		if ( strstr(str, "_SEARCH_START_RANGE_ "))
		{
			sscanf(strstr(str, "_SEARCH_START_RANGE_ ") + strlen("_SEARCH_START_RANGE_ "), "%ld", &_SEARCH_START_RANGE_ );
		}
		if ( strstr(str, "_SEARCH_START_MIDDLE_"))
		{
			sscanf(strstr(str, "_SEARCH_START_MIDDLE_") + strlen("_SEARCH_START_MIDDLE_"), "%ld", &_SEARCH_START_MIDDLE_);
		}

		
	}
	

	fclose(fff);

	m_middle[1] = _SEARCH_START_MIDDLE_;
	m_up[1] = _SEARCH_START_MIDDLE_ + _SEARCH_START_RANGE_;
	m_down[1] = _SEARCH_START_MIDDLE_ - _SEARCH_START_RANGE_;
	if (m_up[1] >= _UP_EDGE_-1 ) m_up[1] = _UP_EDGE_-1;
	if (m_down[1] <= _DOWN_EDGE_ ) m_down[1] = _DOWN_EDGE_ + 1;
	m_up[1] = _UP_EDGE_-4;
	m_down[1] = _DOWN_EDGE_+4;
	
//	m_middle[1] = (m_down[1] + m_up[1]) / 2;
	
}
void TextIsOut::SetParameters(int numberChanges, int goodNumberWhitesPerChanges, int goodPerBadColumn, int badsPerColumnPercent, int numberBadsForFail)
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
/*void TextIsOut::ChangeFound()
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
*/
void TextIsOut::ChangeFound(int jj , bool isLast)
{
/*	if ( (jj >= m_down[0]+3) && (jj <= m_up[0]-3) )
	{
		if (m_isFull[jj][0] && (( (m_pChangesCoord[jj][m_activeChangesIndex] >  m_To[0]) &&  (m_pChangesCoord[jj][0] > m_From[0])&&  (m_pChangesCoord[jj][0] < m_To[0]))  || m_isFirstHistogram[0]))
			CalculateHistogram(jj,0);	
	}*/
	int diff = m_pChangesCoord[jj][m_activeChangesIndex] - m_pChangesCoord[jj][m_activeChangesIndex-1];
	if ( (jj >= m_down[1]-1) && (jj <= m_up[1]+3) && (diff <= m_widthOfInterest))
	{
		
		/*int numberBlack = 0;
		for ( int lll = 0; lll < m_activeChangesIndex; lll++)
			numberBlack += m_pNumberBlack[jj][lll];
		int length = m_pChangesCoord[jj][m_activeChangesIndex] - m_pChangesCoord[jj][0];
		if (numberBlack < length*5/6)*/
		{
			//if (m_isFull[jj][0] && (( /*(m_pChangesCoord[jj][m_activeChangesIndex] >  m_To[1]) &&  */(m_pChangesCoord[jj][0] > m_From[1]) &&  (m_pChangesCoord[jj][0] < m_To[1]))  || (m_isFirstHistogram[1] || isLast)))
			if (m_isFull[jj][0] && (( /*(m_pChangesCoord[jj][m_activeChangesIndex] >  m_To[1]) &&  */
				((m_pChangesCoord[jj][m_activeChangesIndex-1] > m_To[1]) ||
				((m_pChangesCoord[jj][m_activeChangesIndex-1] > m_From[1]) &&  (m_pChangesCoord[jj][m_activeChangesIndex-1] < m_To[1])))
				|| (m_isFirstHistogram[1] || isLast))))
				CalculateHistogram(jj,1);	
		}
	} 
/*	if ( (jj >= m_down[2]+3) && (jj <= m_up[2]-3) )
	{
		if (m_isFull[jj][0] && (( (m_pChangesCoord[jj][m_activeChangesIndex] >  m_To[2]) &&  (m_pChangesCoord[jj][0] > m_From[2])&&  (m_pChangesCoord[jj][0] < m_To[2]))  || m_isFirstHistogram[2]))
			CalculateHistogram(jj,2);	
	}
*/
	for ( int index = 1; index <= m_activeChangesIndex; index++)
	{
		m_pChangesCoord[jj][index-1] = m_pChangesCoord[jj][index];
		m_pNumberBlack[jj][index-1] = m_pNumberBlack[jj][index];
		m_pNumberWhite[jj][index-1] = m_pNumberWhite[jj][index];
		m_isFull[jj][index-1] = m_isFull[jj][index];
	}
	m_pChangesCoord[jj][m_activeChangesIndex] = 0;
	m_pNumberBlack[jj][m_activeChangesIndex] = 0;
	m_pNumberWhite[jj][m_activeChangesIndex] = 0;
	m_isFull[jj][m_activeChangesIndex] = false;
}


void TextIsOut::FillFromHistogram(int jj, int i1, int i2, int k)
{
	int start= 0;
	if ( m_isStart && k == 1)
	{
		int start1 = 0;
		bool found = false;
		int step = 0;
		for ( int l = 0; l < m_height; l++)
		{
			step = ((l%2) == 0) ? (abs(step)+1):(-abs(step));
			int st = m_middle[k] + step;
			
			if ( st >= _UP_EDGE_-3 || st <= _DOWN_EDGE_+3) continue;
			{
				if (m_histogram[st] > 3  ) 
				{

					found = true;
					for ( int s1= 0; s1 < 3; s1++)
					{
						int st1 = st + ((step > 0)?s1:-s1);
						if (m_histogram[st1] < 3  ) 
						{
							found = false;
						}
					}
					if ( found)
					{
						start = st;
						break;
					}

				}
			}
		}
		m_isStart = false;
	}
	else
	{
		for ( int st= m_middle[k]; st<= m_up[k]; st++)
		{
			if (m_histogram[st] > 0  ) 
			{
				start = st;
				 break;
			}
		}
		if (start == 0)
		{
			for ( int st= m_middle[k]; st >= m_down[k]; st--)
			{
				if (m_histogram[st] > 0  ) 
				{
					 start = st;
					 break;
				}
			}

		}
		if (start == 0) return;
	}
	int j1 = start;
	while ((m_histogram[j1] > 0))// || (m_histogram[j1-1] > 0) ||(m_histogram[j1-2] > 0)||(m_histogram[j1-3] > 0)   ) 
	{
		j1--;
		//if ( j1 < 4) break;
	}

	if ( j1 < _DOWN_EDGE_ ) j1 = _DOWN_EDGE_;
	
	//if (!m_isFirstHistogram)
	//	if ( j1 < m_down -  20) j1 = m_down-20;
	
	
	int j2 = start;
	while ((m_histogram[j2] > 0))// || (m_histogram[j2+1] > 0) || (m_histogram[j2+2] > 0 ) || (m_histogram[j2+3] > 0 )   ) 
	{
		j2++;
		//if ( j2 >124) break;
	}
	if ( j2 > _UP_EDGE_) j2 = _UP_EDGE_;

	//if (!m_isFirstHistogram)
	//	if ( j2 > m_up+ 20) j2 = m_up+20;

	if ((j2-j1) > m_height*2/3) return;
    if ((j2 - j1) > 15)
	{
		m_down[k]= j1;
		m_up[k]= j2;
	}

	if ( m_up[k] > m_height - 4) m_up[k] = m_height-4;
	if ( m_down[k] < 3) m_down[k] = 3;

	m_middle[k] = (j1+j2)/2;

	bool inTheModdle = true;



	for ( int j = j1; j <= j2; j++)
	{
		for ( int i = i1; i < i2; i++)
		{
			//if ( (k == 1)  && ((j > 118) || (j < 8)) && ((j2-j1) < m_bodyHeight/5*2)) // /*&& (m_pDataIn[j*m_width+i] == White)*/)
			if ( (k == 1)  && ((j > _UP_EDGE_-1) || (j < _DOWN_EDGE_+1)))
			{
	
				inTheModdle = false;
				if (m_numberInAve)
				{
					if ((j2-j1) < ((float)m_aveHeight/(float)m_numberInAve)*_BAD_RESULT_BODY_PERCENT_)
					{
						if (m_TextOut_first_i == -1)
						{
							m_TextOut_first_i = i;
						}
						m_pWorkData[i] = 1;
					}
				}
				//m_numberBadColumns+=10;
			}

			m_pDataOut[j*m_width*3 + i*3 + k] = m_pDataIn[j*m_width+i]*255;
			if ( k == 1)
			{
				m_pDataOut[j*m_width*3 + i*3 + 0] = m_pDataIn[j*m_width+i]*255;
				m_pDataOut[j*m_width*3 + i*3 + 2] = m_pDataIn[j*m_width+i]*255;

			}
		}
	}


	if (inTheModdle)
	{
		m_aveHeight += j2 - j1;
		m_numberInAve++;

	}

}
void TextIsOut::CalculateHistogram(int jj, int k)
{
	for ( int j = 0; j < 127; j++)
	{
		m_histogram[j] = 0;
	}
	//int i1 = m_pChangesCoord[jj][0];
	int i1 = m_pChangesCoord[jj][m_activeChangesIndex-1];
	int i2 = m_pChangesCoord[jj][m_activeChangesIndex];
	m_From[k] = m_pChangesCoord[jj][m_activeChangesIndex-1]-1;
	m_To[k] = m_pChangesCoord[jj][m_activeChangesIndex]+1;
	if (m_isFirstHistogram[k])
	{
		i1 = m_pChangesCoord[jj][0];
		m_From[k] = m_pChangesCoord[jj][0]-1;
	}
	m_isFirstHistogram[k] = false;
	
	for ( int j = 0; j < 127; j++)
	{
		for ( int i = i1; i < i2; i++)
		{
			m_histogram[j] += m_pDataIn[j*m_width+i];
		}
	}


	/*if (m_isFirstHistogram)
	{
		m_middle[k] = 64;
		m_up[k] = 80;
		m_down[k] = 50;
	}
*/
	FillFromHistogram( jj, i1, i2, k);
	for ( int i = i1; i < i2; i++)
	{
		for ( int j = m_down[1]; j < m_up[1]; j++)
		{
			m_pDataOut[j*3*m_width+i*3 + 0] = m_pDataIn[j*m_width+i];;
			m_pDataOut[j*3*m_width+i*3 + 1] = m_pDataIn[j*m_width+i];;
			m_pDataOut[j*3*m_width+i*3 + 2] = m_pDataIn[j*m_width+i];;
		}
	}
}


int TextIsOut::CalculateWidthOfInterest()
{

	int sum = 0;
	int n = 0;
	for ( int i = 60; i < m_width-1; i++)
	{
		for (  int jj = _DOWN_EDGE_; jj < _UP_EDGE_; jj++)
		{
			byte color = m_pDataIn[jj*m_width+i];
			byte prevColor = m_pDataIn[jj*m_width+i-1];
			byte prevprevColor = m_pDataIn[jj*m_width+i-2];
			if ((color == White) && (prevColor == Black)&&(prevprevColor == Black))
			{
				m_pChangesCoord[jj][m_activeChangesIndex] = i;
				m_isFull[jj][m_activeChangesIndex] = true;
				//ChangeFound(jj);
			}
		}
		for (  int jj = _DOWN_EDGE_; jj < _UP_EDGE_; jj++)
		{
			if (m_pChangesCoord[jj][m_activeChangesIndex] != 0)
			{
				int diff = m_pChangesCoord[jj][m_activeChangesIndex] - m_pChangesCoord[jj][0];
				if ( diff < 300)
				{
					sum += diff;
					n++;
				}
				for ( int index = 1; index <= m_activeChangesIndex; index++)
				{
					m_pChangesCoord[jj][index-1] = m_pChangesCoord[jj][index];
				}
				m_pChangesCoord[jj][m_activeChangesIndex] = 0;
			}
		}
		
	}
	for ( int j = 0; j < 127; j++)
	{
		for ( int index = 0; index <= m_activeChangesIndex; index++)
		{
			m_pChangesCoord[j][index] = 0;
		}	
	}
	int result = 100;
	if (n)
		result = sum / n;
	return result;
}


void TextIsOut::CalculateTextIsOut()
{

	AllocateMemory();
	CalculateBodyHeight();
	m_widthOfInterest = 4*CalculateWidthOfInterest();
	m_result = true;
	for ( int i = 60; i < m_width-1; i++)
	{
		for (  int jj = 0; jj < 127; jj++)
		{
			byte color = m_pDataIn[jj*m_width+i];
			byte prevColor = m_pDataIn[jj*m_width+i-1];
			byte prevprevColor = m_pDataIn[jj*m_width+i-2];


			if (color == Black)
			{
				m_pNumberBlack[jj][m_activeChangesIndex-1]++;
			}
			if (color == White)
			{
				m_pNumberWhite[jj][m_activeChangesIndex-1]++;
			}

			if ((color == White) && (prevColor == Black)&&(prevprevColor == Black))
			{
				m_pChangesCoord[jj][m_activeChangesIndex] = i;
				m_isFull[jj][m_activeChangesIndex] = true;
				//ChangeFound(jj);
			}
		}
		for (  int jj = m_down[1]-3; jj < m_up[1]+3; jj++)
		{
			if (m_pChangesCoord[jj][m_activeChangesIndex] != 0) 
			{
				{
					ChangeFound(jj, false);
				}
			}
		}
		
	}

	m_activeChangesIndex -= 1;
	for (  int jj = m_down[1]-3; jj < m_up[1]+3; jj++)
	{
		if (m_pChangesCoord[jj][m_activeChangesIndex] != 0)
		{
			ChangeFound(jj, true);
		}
	}


	int first_i = -1;
	int allLength = 0;
	for ( int i = 0; i < m_width-1; i++)
	{
		if ( m_pWorkData[i-1] == 0 && m_pWorkData[i] == 1)
		{
			first_i = i;
		}
		if ( m_pWorkData[i-1] == 1 && m_pWorkData[i] == 0)
		{
			int length = i - first_i;
			if ( length > m_bodyHeight*1.5 )
				m_result = false;
		}

		if ( m_pWorkData[i-1] == 1) allLength++;

	}
		if ( allLength > m_bodyHeight )
			m_result = false;




	/*if (m_numberBadColumns > _MAX_NUMBER_BAD_COLUMNS_)
	{
		m_result = false;
	}
	else
	{
		m_result = true;
	}*/
//	SummaryByColumns();
}


/*
void TextIsOut::CalculateTextIsOut()
{

	AllocateMemory();
	for (  m_j = 1; m_j < m_height-1; m_j++)
	{

		byte prevprevColor = m_pDataIn[m_j*m_width];
		byte prevColor = m_pDataIn[m_j*m_width+1];
		for ( int i = 0; i < m_width; i++)
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
				ChangeFound();
			}
			prevprevColor = prevColor;
			prevColor = color;
		}
	}
	for ( int j = 5; j < 15; j++)
		ColorSpread(j, 1, 1, 0);

	for ( int j = 120; j > 105; j--)
		ColorSpread(j, -1, 1, 2);
	
	SummaryByColumns(1, 0, 2);

}
*/
void TextIsOut::SummaryByColumns(int cSrc, int cDst1, int cDst2)
{
	int numberColor = 0;
	int numberGoodColor = 0;
	for ( int ii = 0; ii < m_width; ii++)
	{
		bool isGoodColor = false;
		bool isColorExists = false;
		for ( int jj = 10; jj < m_height-10;jj++)
		{
			if ((m_pDataOut[jj*m_width*3+ii*3+cSrc]!=0) &&
				(m_pDataOut[jj*m_width*3+ii*3+cDst1]==0) &&
				(m_pDataOut[jj*m_width*3+ii*3+cDst2]==0))
			{
				isGoodColor = true;
			}

			if ((m_pDataOut[jj*m_width*3+ii*3+cSrc]!=0) ||
				(m_pDataOut[jj*m_width*3+ii*3+cDst1]!=0) ||
				(m_pDataOut[jj*m_width*3+ii*3+cDst2]!=0))
			{
				isColorExists = true;
			}
			if (isGoodColor) 
			{
				break;
			}
		}
		if (isColorExists)
			numberColor++;
		if (isGoodColor)
			numberGoodColor++;
		

		if (isColorExists && isGoodColor)
		{
			for ( int jj = 0; jj < 10; jj++)
				m_pDataOut1[jj*m_width*3+ii*3 + 1] = 255;
		}
		if (isColorExists && (!isGoodColor))
		{
			for ( int jj = 0; jj < 10; jj++)
				m_pDataOut1[jj*m_width*3+ii*3 + 0] = 255;
		}

			
	}

	//m_numberBadColumns = numberColor - numberGoodColor;
	if (m_numberBadColumns > _MAX_NUMBER_BAD_COLUMNS_)
	{
		m_result = false;
	}
	else
	{
		m_result = true;
	}

}
int TextIsOut::CalculateBodyHeight()
{
	for ( int j = 0; j < m_height; j++)
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

				if ( (start != -1) && ((finish - start) < 150) )
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
		for ( int j = 0; j < m_height; j++)
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
*/

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
		for ( int j = 0; j < m_height; j++)
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
	}*/
	m_bodyHeight = jMax;
	memset(m_pDataOut, Black, m_width*m_height*3); 
	return jMax;
}
