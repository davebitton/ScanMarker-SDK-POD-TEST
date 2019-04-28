//#include "StdAfx.h"
#include <stdio.h>
#include "OneLine.h"
//#include "..\SplineInterpolation\SplineInterpolation.h"
#define NUMBER_WORDS_IN_STEP 1
#define NUMBER_WORDS_IN_CALC 0
#define NUMBER_WORDS_IN_ITALIC 3
//#define TextColor 255

#define White 255
#define Black 0
void OneLine::PrintDebugStr(char* str, bool append)
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
OneLine::OneLine(void)
{
	_UP_EDGE_  = 124;
	_DOWN_EDGE_  = 4;
	m_isStart = true;
	m_bodyHeight = 30;
	m_width = 0;
	m_height = 0;
	m_pDataIn = NULL;
	m_pDataOut = NULL;
	m_pDataStraight = NULL;
	m_pDataItalic = NULL;
	m_pDataCompressed = NULL;
	m_pWorkData = NULL;
	m_pWorkData1 = NULL;
	m_pWorkData2 = NULL;

	m_pDataInCopy = NULL;
	m_pDataOutCopy = NULL;

	prev_i2 = -1;
	m_verticalHistogram = NULL;
	m_newFrom = -1;

	m_numberChanges = 2;
	m_pInterpolationInput = 0;
	m_pInterpolationOutput = 0;
	_HALF_NUMBER_CHANGES_TO_ANALIZE_ = 2;
	for ( int j = 0; j < 1000; j++)
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
	
	m_isHistogramExists[0] = true;
	m_isHistogramExists[1] = true;
	m_isHistogramExists[2] = true;
	
	m_middle[1] = 64;
	m_up[1] = 100;
	m_down[1] = 24;
	m_numberBadColumns = 0;
	m_prev_down[1] = -1;
	m_prev_up[1] = -1;

	/*m_middle[0] = 64;
	m_up[0] = 80;
	m_down[0] = 50;

	m_middle[2] = 64;
	m_up[2] = 80;
	m_down[2] = 50;

*/




	m_middle[0] = 100;
	m_up[0] = 120;
	m_down[0] = 85;

	m_middle[2] = 18;
	m_up[2] = 25;
	m_down[2] = 5;

	
	m_maxHeight[0] = 0;
	m_maxHeight[1] = 0;
	m_maxHeight[2] = 0;
	m_numberChangesInRow[0] = NULL;
	m_numberChangesInRow[1] = NULL;


}
 OneLine::~OneLine()
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
	if (m_pDataOutCopy)
	{
		delete[] m_pDataOutCopy;
		m_pDataOutCopy = NULL;
	}
	if (m_pDataInCopy)
	{
		delete[] m_pDataInCopy;
		m_pDataInCopy = NULL;
	}

	for ( int j = 0; j < m_height; j++)
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
		
		if (m_pWorkData != NULL)
		{
			delete[] m_pWorkData;
			m_pWorkData = NULL;
		}
		if (m_pDataStraight != NULL)
		{
			delete[] m_pDataStraight;
			m_pDataStraight = NULL;
		}
		if (m_pDataItalic != NULL)
		{
			delete[] m_pDataItalic;
			m_pDataItalic= NULL;
		}
		if (m_pDataCompressed != NULL)
		{
			delete[] m_pDataCompressed;
			m_pDataCompressed= NULL;
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

		if (m_pInterpolationInput)
		{
			delete[] m_pInterpolationInput;
			m_pInterpolationInput= NULL;
		}
		if (m_pInterpolationOutput)
		{
			delete[] m_pInterpolationOutput;
			m_pInterpolationOutput= NULL;
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
		if (m_verticalHistogram )
		{
			delete[] m_verticalHistogram ;
			m_verticalHistogram = NULL;
		}
	
	}
}
void OneLine::AllocateMemory()
{
	m_activeChangesIndex = _HALF_NUMBER_CHANGES_TO_ANALIZE_*2+4;

	if (m_verticalHistogram )
	{
		delete[] m_verticalHistogram ;
		m_verticalHistogram = NULL;
	}
	m_verticalHistogram = new int[m_width];
	memset(m_verticalHistogram, 0, m_width*sizeof(int));


	if (m_pWorkData!= NULL)
	{
		delete[] m_pWorkData;
		m_pWorkData = NULL;
	}
	m_pWorkData = new int[m_width*m_height*3];
	memset(m_pWorkData, 0, m_width*m_height*3*sizeof(int));

	if (m_pDataInCopy)
	{
		delete[] m_pDataInCopy;
		m_pDataInCopy= NULL;
	}
	m_pDataInCopy = new byte[m_width*m_height*3];
	memset(m_pDataInCopy, 0, m_width*m_height*3*sizeof(byte));

	if (m_pDataOutCopy)
	{
		delete[] m_pDataOutCopy;
		m_pDataOutCopy= NULL;
	}
	m_pDataOutCopy = new byte[m_width*m_height*3];
	memset(m_pDataOutCopy, 0, m_width*m_height*3*sizeof(byte));

	if (m_pWorkData1)
	{
		delete[] m_pWorkData1;
		m_pWorkData1= NULL;
	}
	m_pWorkData1 = new int[m_width*m_height*3];
	memset(m_pWorkData1, 0, m_width*m_height*3*sizeof(int));
	if (m_pWorkData2)
	{
		delete[] m_pWorkData2;
		m_pWorkData2= NULL;
	}
	m_pWorkData2 = new int[m_width*m_height*3];
	memset(m_pWorkData2, 0, m_width*m_height*3*sizeof(int));



	if (m_pDataStraight != NULL)
	{
		delete[] m_pDataStraight;
		m_pDataStraight = NULL;
	}
	m_pDataStraight = new byte[m_width*m_height*3];
	memset(m_pDataStraight, 0, m_width*m_height*3*sizeof(byte));

	if (m_pDataItalic != NULL)
	{
		delete[] m_pDataItalic;
		m_pDataItalic= NULL;
	}
	m_pDataItalic= new byte[m_width*m_height*3];
	memset(m_pDataItalic, 0, m_width*m_height*3*sizeof(byte));

	if (m_pDataCompressed != NULL)
	{
		delete[] m_pDataCompressed;
		m_pDataCompressed= NULL;
	}
	m_pDataCompressed = new byte[m_width*m_height*3];
	memset(m_pDataCompressed, 0, m_width*m_height*3*sizeof(byte));




	if (m_pInterpolationInput )
	{
		delete[] m_pInterpolationInput ;
		m_pInterpolationInput = NULL;
	}
	m_pInterpolationInput= new float[m_width];
	for ( int i = 0; i < m_width; i++)
		m_pInterpolationInput[i] = 0.0;
	
	if (m_pInterpolationOutput)
	{
		delete[] m_pInterpolationOutput;
		m_pInterpolationOutput= NULL;
	}
	m_pInterpolationOutput= new float[m_width];
	for ( int i = 0; i < m_width; i++)
	m_pInterpolationOutput[i] = 0.0;
	
	for ( int j = 0; j < m_height; j++)
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
			m_isFull[j][index] = false;
			m_pNumberBlack[j][index] = 0;
			m_pNumberWhite[j][index] = 0;
			m_pChangesCoord[j][index] = 51;
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

}

void OneLine::Init(int width, int height, byte* pDataIn, byte* pDataOut, byte* pDataOut1)
{
	m_width = width;
	m_height = height;
	m_pDataIn = pDataIn;
	m_pDataOut = pDataOut;
//	m_pDataOut1 = pDataOut1;
	memset(m_pDataOut, Black, m_width*m_height*3*sizeof(byte));
	//AllocateMemory();
	m_isInit = true;



}
void OneLine::ReadCfgFile(char* dir)
{
	char fileName[255];
	//sprintf(fileName, "%s//%s", dir, "text_is_out.cfg");
	sprintf(fileName, "china_compress_big.cfg");
	FILE * fff = fopen(fileName, "rt");
	if (!fff) return;

	char str[255];
	while (fgets(str, 254, fff))
	{
		if (str[0] == '#') continue;

		
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
	}
	

	fclose(fff);
}
void OneLine::SetParameters(int numberChanges, int goodNumberWhitesPerChanges, int goodPerBadColumn, int badsPerColumnPercent, int numberBadsForFail)
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
/*void OneLine::ChangeFound()
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
void OneLine::ChangeFound(int jj, bool isLast )
{
/*	if ( (jj >= m_down[0]+3) && (jj <= m_up[0]-3) )
	{
		if (m_isFull[jj][0] && (( (m_pChangesCoord[jj][m_activeChangesIndex] >  m_To[0]) &&  (m_pChangesCoord[jj][0] > m_From[0])&&  (m_pChangesCoord[jj][0] < m_To[0]))  || m_isFirstHistogram[0]))
			CalculateHistogram(jj,0);	
	}*/
	int diff = m_pChangesCoord[jj][m_activeChangesIndex] - m_pChangesCoord[jj][m_activeChangesIndex-1];
	if ( (jj >= m_down[1]-1) && (jj <= m_up[1]+3) && ((diff <= m_widthOfInterest)))// || m_isFirstHistogram[1]))
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
	
/*	if ((m_up[2] - m_down[2]) < 10)
		m_isHistogramExists[2] = false;

	if (m_isHistogramExists[2])
	{
		if ( (jj >= m_down[2]+3) && (jj <= m_up[2]-3) && ((m_up[2] - m_down[2]) > 10) )
		{
			if (m_isFull[jj][0] && (( (m_pChangesCoord[jj][m_activeChangesIndex] >  m_To[2]) &&  ((m_pChangesCoord[jj][0] > m_From[2]) || isLast)&&  (m_pChangesCoord[jj][0] < m_To[2]))  || m_isFirstHistogram[2]))
				CalculateHistogram(jj,2);	
		}
	}
*/
	if (!isLast)
	{
		for ( int index = 1; index <= m_activeChangesIndex; index++)
		{
			m_pChangesCoord[jj][index-1] = m_pChangesCoord[jj][index];
			m_pNumberBlack[jj][index-1] = m_pNumberBlack[jj][index];
			m_pNumberWhite[jj][index-1] = m_pNumberWhite[jj][index];
			m_isFull[jj][index-1] = m_isFull[jj][index];
		}
		m_pChangesCoord[jj][m_activeChangesIndex] = 51;
		m_pNumberBlack[jj][m_activeChangesIndex] = 0;
		m_pNumberWhite[jj][m_activeChangesIndex] = 0;
		m_isFull[jj][m_activeChangesIndex] = false;
	}
}

/*void OneLine::SmoothCompressCurve()
{
	SplineInterpolation splineInterpolation(m_pInterpolationInput, m_pInterpolationOutput, m_width, m_height, 2, 2);
}*/

void OneLine::FillFromHistogram(int jj, int i1, int i2, int k)
{

	int start= 0;
	int begin_x_Out = i1;
	if ( m_isStart && k == 1)
	{
		begin_x_Out = 0;
		int start1 = 0;
		bool found = false;
		int step = 0;
		for ( int l = 0; l < m_halfHeight; l++)
		{
			step = ((l%2) == 0) ? (abs(step)+1):(-abs(step));
			int st = m_middle[k] + step;
			
			if ( st >= m_up[k] || st <= m_down[k]) continue;
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
		if (!found)
		{
			m_up[k] = m_height - 2;
			m_down[k] = 2;
			m_middle[k] = (m_up[k]+m_down[k])/2;
		}
		//m_isStart = false;
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
		if (start < 2) return;
	}
	int j1 = start;

	while ((m_histogram[j1+3] > 3) ||
		(m_histogram[j1+2] > 3) ||
		(m_histogram[j1+1] > 3) ||
		( m_histogram[j1] > 3) || 
		(m_histogram[j1-1] > 3) 
		||(m_histogram[j1-2] > 3)||
		(m_histogram[j1-3] > 3))//||(m_histogram[j1-3] > 0)   ) 
	{
		j1--;
		if ( j1 < m_down[k]+3) break;
	}
	while (( m_histogram[j1] > 3) || (m_histogram[j1-1] > 3))//||(m_histogram[j1-3] > 0)   ) 
	{
		j1--;
		if ( j1 < _DOWN_EDGE_ ) break;
	}


	if ( j1 < _DOWN_EDGE_ ) j1 = _DOWN_EDGE_;
	
	//if (!m_isFirstHistogram)
	//	if ( j1 < m_down -  20) j1 = m_down-20;
	
	
	int j2 = start;
	while ((m_histogram[j2-3] > 3) ||
		(m_histogram[j2-2] > 3) || 
		(m_histogram[j2-1] > 3) || 
		(m_histogram[j2] > 3) || 
		(m_histogram[j2+1] > 3) || 
		(m_histogram[j2+2] > 3 ) || 
		(m_histogram[j2+3] > 3 ))// || (m_histogram[j2+3] > 0 )   ) 
	{
		j2++;
		if ( j2 >m_up[k]-3) break;
	}
	while ((m_histogram[j2] > 3) || (m_histogram[j2+1] > 3) )// || (m_histogram[j2+3] > 0 )   ) 
	{
		j2++;
		if ( j2 > _UP_EDGE_) break;
	}
	if ( j2 > _UP_EDGE_) j2 = _UP_EDGE_;





	if ( (!m_isStart) && k == 1)
	{
		if ((m_down[k] - j1) > 12)
		{
			
			int minHist = m_histogram[m_down[k]];
			
			int jjj = m_down[k] + 3;
			int jj = m_down[k] - 2;
			while ( jj != jjj)
			{
				if (jj < _DOWN_EDGE_) continue;
				int current = m_histogram[jj];
				if (minHist >= current) 
				{
					minHist = current;
					if (minHist < 12)
						j1 = jj;
				}
				jj++;
			}
			
		}	
			
		if ((j2 - m_up[k]) > 12)
		{
			int minHist = m_histogram[m_up[k]];
			
			int jjj = m_up[k] - 3;
			int jj = m_up[k] + 2;
			while ( jj != jjj)
			{
				if (jj > _UP_EDGE_) continue;
				int current = m_histogram[jj];
				if (minHist >= current) 
				{
					minHist = current;
					if (minHist < 12)
						j2 = jj;
				}
				jj--;
			}
		}

	}
	else
	{
		m_isStart = false;
	}


//		j2 = m_up[k];
/*		jj = m_up[k];
		int minHist = m_histogram[jj];

		for ( int jj = m_up[k] - 3; jj < m_up[k]+3; jj++)
		{
			if (jj > _UP_EDGE_) continue;
			int current = m_histogram[jj];
			if (minHist > current) 
			{
				minHist > current;
				j2 = jj;
			}
		}
*/
		
		

	if ((j2-j1) > m_halfHeight*2/3) return;
    if ((j2 - j1) > 7)
	{
		m_down[k]= j1;
		m_up[k]= j2;
	}

	if ( m_up[k] > m_halfHeight - 4) m_up[k] = m_halfHeight-4;
	if ( m_down[k] < 3) m_down[k] = 3;

//	m_middle[k] = (j1+j2)/2;
	m_middle[k] = (m_up[k]+m_down[k])/2;









	bool inTheModdle = true;

	if (begin_x_Out == 0)
	{
		for ( int j = j1; j < j2; j++)
		{
			for ( int i = i1 - 100; i < i1; i++)
			{
				if ( i < 0 ) continue;
				int value = 255;//m_pDataIn[j*m_width+i];
				m_pDataOut[j*m_width*3 + i*3 + 0] = value;
				m_pDataOut[j*m_width*3 + i*3 + 1] = value;
				m_pDataOut[j*m_width*3 + i*3 + 2] = value;
			}
		}
	}
	/*int iiii[11];
	int jjjj1[10];
	int jjjj2[10];

	int count = m_activeChangesIndex;
	if (count > 10) count= 10;
	int step = (i2 - i1) / count;
	for ( int m = 0; m < count; m++)
	{
		iiii[m] = m* step+i1;
		iiii[m+1] = (m+1)* step+i1;
		if (iiii[m+1] > i2) 
			iiii[m+1] = i2;
		jjjj1[m] = j1;
		jjjj2[m] = j2;


	}
	iiii[count] = i2;
*/
	if (prev_i2== -1) prev_i2= i1;
	if (prev_i2 > i1+5) prev_i2 = i1;
	
	int j1_stored = j1;
//	for ( int m = 2; m < count; m++)
	{
		int isThreeLinesBlack = 0;
		
//		for ( int j = jjjj1[m]; j >= jjjj1[m]- 15; j--)
		for ( int j = j1; j >= j1 - 8; j--)
		{
			if ( j < _DOWN_EDGE_) 
			{
				//jjjj1[m] = j;
				//j1 = j;
				break;
			}
			isThreeLinesBlack++;
			//for ( int i = iiii[m-2]; i < iiii[m+1]; i++)
			int number_whites = 0;
			for ( int i = prev_i2; i < i2; i++)
			{
				if ( m_pDataIn[j*m_width+i] == 255)
				{
					isThreeLinesBlack = 0;			
					number_whites++;
					if (number_whites > 10) break;
				}
			}
			if (number_whites > 10) 
			{
				//j1 = j+1;
				//break;
			}
			if (isThreeLinesBlack == 5)
			{
				//jjjj1[m] = j;
				j1 = j;
				break;
			}
		}
		//if ( j1 != j1_stored)
		{
			bool blackLineFound = false;
			for ( int j = j1; j < j1+8; j++)
			{
				int numberTextPixels = 0;
				for ( int i = prev_i2; i < i2; i++)
				{
					if (j >= _UP_EDGE_) break;
					if ( m_pDataIn[(j+1)*m_width+i] == 255)
					{
						numberTextPixels++;
					}
					if ( m_pDataIn[j*m_width+i] == 255)
					{
						numberTextPixels++;
					}
					if ( m_pDataIn[(j-1)*m_width+i] == 255)
					{
						numberTextPixels++;
					}
				}
				if (numberTextPixels > (i2 - prev_i2) *2.5)
					blackLineFound = true;
				if (blackLineFound && 
					numberTextPixels < (i2 - prev_i2) / 2)
				{
					j1 = j;
					break;
				}

			}
		}


		int j2_stored = j1;
		isThreeLinesBlack = 0;
		//for ( int j = jjjj2[m]; j <= jjjj2[m] + 15; j++)
		for ( int j = j2; j <= j2 + 8; j++)
		{
			if ( j > _UP_EDGE_) 
			{
				//jjjj2[m] = j;
				//j2 = j;
				break;
			}
			isThreeLinesBlack++;
			int number_whites = 0;
			for ( int i = prev_i2; i < i2; i++)
			{
				if ( m_pDataIn[j*m_width+i] == 255)
				{
					isThreeLinesBlack = 0;			
					number_whites++;
					if (number_whites > 10) break;
				}
			}
			if (number_whites > 10) 
			{
				//j2 = j-1;
				//break;
			}
			if (isThreeLinesBlack == 5)
			{
				//jjjj2[m] = j;
				j2 = j;
				break;
			}
		}

		//if (j2 != j2_stored)
		{
			bool blackLineFound = false;
			int max = 0;
			for ( int j = j2; j > j2-8; j--)
			{
				if (j <= _DOWN_EDGE_) break;
				int numberTextPixels = 0;
				for ( int i = prev_i2; i < i2; i++)
				{
					if ( m_pDataIn[(j+1)*m_width+i] == 255)
					{
						numberTextPixels++;
					}
					if ( m_pDataIn[(j-1)*m_width+i] == 255)
					{
						numberTextPixels++;
					}
					if ( m_pDataIn[j*m_width+i] == 255)
					{
						numberTextPixels++;
					}
				}
				if (numberTextPixels > (i2 - prev_i2)*2.5)
				{
					blackLineFound = true;
					if (numberTextPixels > max)
						max = numberTextPixels;
				}
				if (blackLineFound && 
					numberTextPixels < (i2 - prev_i2)/2)//max / 1.9)
				{
					j2 = j;
					break;
				}

			}
		}
		//for ( int j = jjjj1[m]; j < jjjj2[m]; j++)
		for ( int j = j1; j < j2; j++)
		{
			//for ( int i = iiii[m-2]; i < iiii[m+1]; i++)
		for ( int i = prev_i2-5; i < i2+5; i++)
			{
				if ( (k == 1)  && ((j > 118) || (j < 8))  /*&& (m_pDataIn[j*m_width+i] == White)*/)
				{
					m_numberBadColumns++;
				} 

				int value = 1;//m_pDataIn[j*m_width+i];
				value = m_pDataIn[j*m_width+i];
				if (value != 0)
				{
					value = 255;
				}
				value = 255;
				m_pDataOut[j*m_width*3 + i*3 + k] = value;
				//m_pDataOut[j*m_width*3 + i*3 + 0] = 0;
				//m_pDataOut[j*m_width*3 + i*3 + 2] = 0;
				//if ( (k == 1) && (j != j_middle))
	//			if ( (k == 1) && (j != j_middle))
				{
					m_pDataOut[j*m_width*3 + i*3 + 0] = value;
					m_pDataOut[j*m_width*3 + i*3 + 2] = value;
				}
			}
		}
	}
	
	prev_i2= i2;
	
}
void OneLine::DrawOutput(void* values, int valuesType, int red, int green, int blue)
{
	float ratio_min = 10000;
	float ratio_max = 0;
	for ( int i = 100; i < m_width; i++)
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


		if (fabs(value)  < 0.0001)
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

	ratio_min = m_ratio_min;
	ratio_max = m_ratio_max;
	float delta = ratio_max - ratio_min;
	
	float show_ratio = 1.0;
	if (delta  > 0)
		show_ratio = 1;//(float)(m_height - 10) / delta;

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

		if (fabs(value)< 0.001)
		{
			continue;
		}

		int j = (int) value;//(m_height - 1 - show_ratio * (value - ratio_min)) ;
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
void OneLine::CalculateHistogram(int jj, int k)
{
	for ( int j = 0; j < m_halfHeight; j++)
	{
		m_histogram[j] = 0;
	}
	//int i1 = prev_i2;//m_pChangesCoord[jj][0];
	int i1 = m_pChangesCoord[jj][0];
	int i2 = m_pChangesCoord[jj][m_activeChangesIndex];
	//m_From[k] = m_pChangesCoord[jj][m_activeChangesIndex-1]-1;
	m_To[k] = m_pChangesCoord[jj][m_activeChangesIndex]+1;
	
	//if ((m_To[k] - m_From[k] < 50))
	{

		if (m_From[k] < m_pChangesCoord[jj][0]+1)
		{
			m_From[k] = m_pChangesCoord[jj][0]+1;
		}
		else
		{
			i1 = m_From[k] - 1;
		}
		/*m_From[k] = m_newFrom;
		if (m_newFrom == -1)
			m_From[k] = m_pChangesCoord[jj][0]+1;
			*/	}
	//m_newFrom = m_pChangesCoord[jj][1]+1;
	for ( int j = 0; j < m_halfHeight; j++)
	{
		for ( int i = i1; i < i2; i++)
		{
			m_histogram[j] += (m_pDataIn[j*m_width+i] == White);
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
	m_isFirstHistogram[k] = false;
	
}



int OneLine::CalculateWidthOfInterest()
{

	int sum = 0;
	int n = 0;
	for ( int i = 60; i < m_halfWidth-1; i++)
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
			if (m_pChangesCoord[jj][m_activeChangesIndex] > 51)
			{
				int diff = m_pChangesCoord[jj][m_activeChangesIndex] - m_pChangesCoord[jj][0];
				if ( diff < 150)
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

void OneLine::Calculate()
{
	
 /* for (int i = 0; i < m_width; i++)
	   for ( int j = 0; j < m_height; j++)
			m_pDataIn[m_width*j+i] = 255 - m_pDataIn[m_width*j+i];
*/		

//	int t1, t2, t3, t4, t5, t6, t7;
//	extern unsigned int getMillisecond(); 
//	int tStart = getMillisecond();

bool isFirst = true;
	AllocateMemory();


	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataInCopy[m_width*j+i] = m_pDataIn[m_width*j+i];
		}
	}
	m_halfWidth = m_width / 2;
	m_halfHeight = m_height / 2;
	memset(m_pDataIn, 0, m_width*m_height*sizeof(byte));

	for ( int i = 0; i < m_width; i+=2)
	{
		for ( int j = 0; j < m_height; j+=2)
		{
			m_pDataIn[m_width*j/2+i/2] = m_pDataInCopy[m_width*j+i];
		}
	}

	for ( int k = 0; k < 3; k++)
	{
		m_down[k] /= 2;
		m_up[k] /= 2;
		m_middle[k] /= 2;
	}

	
	_UP_EDGE_ = _UP_EDGE_/2-1;
	_DOWN_EDGE_ =_DOWN_EDGE_/2+1;


	int rowUp[5] = { 0, 0, 0, 0 ,0};
	int rowDown[5] = { 0, 0, 0, 0 ,0};
	int rowMiddle[5] = { 0, 0, 0, 0 ,0};
	int nearestRow = 1;
	int nearest = 10000;
	int middle = m_middle[1];
	m_start_x = 30;//i;
		
	for ( int i = 0; i < 1200; i+=50)
	{
		for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++) m_histogram[j] = 0;
		
		for ( int ii = i; ii < i+50; ii++)
		{
			for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
			{
				m_histogram[j] += (m_pDataIn[m_width*j+ii]) ? 1: 0;
			}
		}
		int nRow = -1;
		bool inModdle = false;
		bool found = false;
		for ( int j = _DOWN_EDGE_+1; j < _UP_EDGE_-1; j++)
		{
			if ( (m_histogram[j-1] > 3) && m_histogram[j] > 3 && m_histogram[j+1] > 3)
			{
				if (!inModdle)
				{
					nRow++;
					rowDown[nRow] = j-1;
					inModdle = true;
				}
			}
			if ( (m_histogram[j-1] == 0) && m_histogram[j] == 0 && m_histogram[j+1] == 0)
			{
				if (inModdle)
				{
					rowUp[nRow] = j+1;
					if (rowUp[nRow] - rowDown[nRow] < 10) nRow--;
					inModdle = false;
					found = true;
					if ( nRow == 5) break;
				}
			}
			if ( inModdle && j >= _UP_EDGE_-2)
			{
					rowUp[nRow] = j+1;
					if (rowUp[nRow] - rowDown[nRow] < 10) nRow--;
					inModdle = false;
					found = true;
					if ( nRow == 5) break;

			}
		}
		
		if (!found) continue;
		for ( int r = 0; r < 5; r++)
		{
			if (rowUp[r]-rowDown[r] < 10) continue;
			if (rowUp[r]-rowDown[r] > 45) 
			{
				m_start_x = i+50;
		
				continue;
			}
			rowMiddle[r] = (rowUp[r]+rowDown[r]) / 2;
			int distance = abs(middle - rowMiddle[r]);
			if (  distance < nearest)
			{
				nearest = distance;
				nearestRow = r;
				m_middle[1] = rowMiddle[nearestRow];
				m_up[1] = rowUp[nearestRow];
				m_down[1] = rowDown[nearestRow];;

				
			}
		}
		if (nearest < 14) break;
	}









//	t1 = getMillisecond();
		m_widthOfInterest = 4*CalculateWidthOfInterest();
//	t2 = getMillisecond();

	for ( int i = m_start_x; i < m_halfWidth-1; i++)
	{
		int j1 = m_down[1];
		int j2 = m_up[1];
		if ( (j2 - j1) < m_maxHeight[1]/4)//2)
		{
			j1 -= m_maxHeight[1]*2/3;
			if ( j1 <= 0) j1 = 0;

			j2 += m_maxHeight[1]*2/3;
			if ( j2 >= m_halfHeight) j2 = m_halfHeight-1;

		}
		int minJ = (j1+j2)/2;
		int minJLength = 100000;
		for (  int jj = j1; jj < j2; jj++)
		{
			byte color = m_pDataIn[jj*m_width+i];
			byte prevColor = m_pDataIn[jj*m_width+i-1];
			byte prevprevColor = m_pDataIn[jj*m_width+i-2];


			/*if (color == Black)
			{
				m_pNumberBlack[jj][m_activeChangesIndex-1]++;
			}
			if (color == White)
			{
				m_pNumberWhite[jj][m_activeChangesIndex-1]++;
			}
*/
			if ((color != prevColor)&&(color != prevprevColor))
			{
				m_pChangesCoord[jj][m_activeChangesIndex] = i;
				int length = m_pChangesCoord[jj][m_activeChangesIndex] - m_pChangesCoord[jj][0];
				if (((minJLength > length) )   && (length > (m_activeChangesIndex*2.5)))
				{
					minJLength = length;
					minJ = jj;
				}
				m_isFull[jj][m_activeChangesIndex] = true;
				//ChangeFound(jj);
			}
		}
	


		for (  int jj = j1; jj < j2; jj++)
		{
			if (m_pChangesCoord[jj][m_activeChangesIndex] > 51) 
		//	if (m_pChangesCoord[minJ][m_activeChangesIndex] > 51) 
			{
				if (isFirst)
				{
					m_pChangesCoord[jj][0] = 51;
				}
				ChangeFound(jj, false);
			}
		}
		isFirst = false;
	}
	//m_activeChangesIndex--;
//	t3 = getMillisecond();
	int j1 = m_down[1];
	int j2 = m_up[1];
	if ( (j2 - j1) < m_maxHeight[1]/2)
	{
		j1 -= m_maxHeight[1]*2/3;
		if ( j1 <= 0) j1 = 0;

		j2 += m_maxHeight[1]*2/3;
		if ( j2 >= m_halfHeight) j2 = m_halfHeight-1;

	}
	
//	for (  int jj = j1; jj < j2; jj++)
	for (  int jj = _DOWN_EDGE_; jj < _UP_EDGE_; jj++)
	{
		if (m_pChangesCoord[jj][m_activeChangesIndex-1] != 0)
		{
			m_pChangesCoord[jj][m_activeChangesIndex] +=200;//m_width - 100;
			ChangeFound(jj, true);
		}
	}
	//	t4 = getMillisecond();

	if (m_numberBadColumns > _MAX_NUMBER_BAD_COLUMNS_)
	{
		m_result = false;
	}
	else
	{
		m_result = true;
	}
/*t1-=tStart;
	t2-=tStart;
	t3-=tStart;
	t4-=tStart;
	t5-=tStart;
	t6-=tStart;
	t7-=tStart;

bool isDebugMode = true;
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
			
			fprintf(fff1, "\n  t1 = %ld\n t2 = %ld\n t3 = %ld\n t4 = %ld\n t5 = %ld\n  t6 = %ld\n t7 = %ld\n",
				 t1, t2, t3, t4, t5, t6, t7);
		}
	}
*/
	/*SmoothCompressCurve();
	m_ratio_min = 0;
	m_ratio_max = m_height-1;
	//StraightTextLine(m_pDataOut,m_pInterpolationOutput, 2);
	//memset(m_pDataOut, 0, m_width*m_height*3*sizeof(byte));
	//ProcessItalic(m_pDataIn, 1,  m_pDataOut, 0, 255);
	
	memcpy(m_pDataStraight, m_pDataOut, m_width*m_height*sizeof(byte)*3);
	StraightTextLine(m_pDataStraight,m_pInterpolationOutput, 2);
	memset(m_pDataOut, 0, m_width*m_height*3*sizeof(byte));
	ProcessItalic(m_pDataStraight, 0,  m_pDataOut, 0, 255);
	
	*/
	//DrawOutput(m_pInterpolationOutput, 2, 255, -1, 0);
//	SummaryByColumns();

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
/*
			if ( m_pDataOut[m_width*j*3+i*3])
			{
				m_pDataOutCopy[m_width*j*3+i*3+0]  = m_pDataIn[m_width*j+i];
				m_pDataOutCopy[m_width*j*3+i*3+1]  = m_pDataIn[m_width*j+i];
				m_pDataOutCopy[m_width*j*3+i*3+2]  = m_pDataIn[m_width*j+i];

			}
*/
			if ( m_pDataOut[m_width*j*3/2+i*3/2])
			{
				m_pDataOutCopy[m_width*j*3+i*3+0]  = m_pDataInCopy[m_width*j+i];
				m_pDataOutCopy[m_width*j*3+i*3+1]  = m_pDataInCopy[m_width*j+i];
				m_pDataOutCopy[m_width*j*3+i*3+2]  = m_pDataInCopy[m_width*j+i];

				m_pDataOutCopy[m_width*(j+1)*3+i*3+0]  = m_pDataInCopy[m_width*(j+1)+i];
				m_pDataOutCopy[m_width*(j+1)*3+i*3+1]  = m_pDataInCopy[m_width*(j+1)+i];
				m_pDataOutCopy[m_width*(j+1)*3+i*3+2]  = m_pDataInCopy[m_width*(j+1)+i];

				m_pDataOutCopy[m_width*j*3+(i+1)*3+0]  = m_pDataInCopy[m_width*j+i+1];
				m_pDataOutCopy[m_width*j*3+(i+1)*3+1]  = m_pDataInCopy[m_width*j+i+1];
				m_pDataOutCopy[m_width*j*3+(i+1)*3+2]  = m_pDataInCopy[m_width*j+i+1];

				m_pDataOutCopy[m_width*(j+1)*3+(i+1)*3+0]  = m_pDataInCopy[m_width*(j+1)+i+1];
				m_pDataOutCopy[m_width*(j+1)*3+(i+1)*3+1]  = m_pDataInCopy[m_width*(j+1)+i+1];
				m_pDataOutCopy[m_width*(j+1)*3+(i+1)*3+2]  = m_pDataInCopy[m_width*(j+1)+i+1];
			
			}
		}
	}

	memset(m_pDataOut, 0, m_width*m_height*sizeof(byte)*3);
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{

				m_pDataOut[m_width*j*3+i*3+0]  = m_pDataOutCopy[m_width*j*3+i*3+0];
				m_pDataOut[m_width*j*3+i*3+1]  = m_pDataOutCopy[m_width*j*3+i*3+1];
				m_pDataOut[m_width*j*3+i*3+2]  = m_pDataOutCopy[m_width*j*3+i*3+2];

		}
	}


}

void OneLine::SummaryByColumns(int cSrc, int cDst1, int cDst2)
{
	int numberColor = 0;
	int numberGoodColor = 0;
	for ( int ii = 0; ii < m_halfWidth; ii++)
	{
		bool isGoodColor = false;
		bool isColorExists = false;
		for ( int jj = 10; jj < m_halfHeight-10;jj++)
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
		

/*		if (isColorExists && isGoodColor)
		{
			for ( int jj = 0; jj < 10; jj++)
				m_pDataOut1[jj*m_width*3+ii*3 + 1] = 255;
		}
		if (isColorExists && (!isGoodColor))
		{
			for ( int jj = 0; jj < 10; jj++)
				m_pDataOut1[jj*m_width*3+ii*3 + 0] = 255;
		}
*/
			
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

void OneLine::FindNextWord(int *verticalHistogram, int & beginWord, int & endWord, void  *data, int dataMode, int iTextColor, int * ph1, int * ph2)
{
	beginWord = endWord;
	while ((verticalHistogram[beginWord] < 1) || 
		(verticalHistogram[beginWord-1] < 1) || 
		(verticalHistogram[beginWord-1] < 1))
	{
		if ( beginWord>= m_halfWidth -1)
		{
			break;
		}			
		beginWord++;
	}
	endWord = beginWord+1;

	while ((verticalHistogram[endWord] >= 1) ||
		(verticalHistogram[endWord-1] >= 1) ||
		(verticalHistogram[endWord-1] >= 1))
	{
		if ( endWord >= m_halfWidth -1)
		{
			break;
		}			
		endWord++;
	}

	if (data == NULL) return;

	bool found = false;
	*ph1 = -1;
	for ( int j = 0; j < m_halfHeight; j++)
	{
		for ( int i = beginWord; i < endWord; i++)
		{ 
			int value = 0;
			if (dataMode == 0)
			{
				if ((((byte*) data)[3*j*m_width+3*i+0] == iTextColor) ||
					(((byte*) data)[3*j*m_width+3*i+1] == iTextColor) ||
					(((byte*) data)[3*j*m_width+3*i+2] == iTextColor))
				{
					if (*ph1 == -1)
						*ph1 = j;
					*ph2 = j;
				}
			}
			else if (dataMode == 1)
			{
				if (((byte*) data)[j*m_width+i] == iTextColor)
				{
					if (*ph1 == -1)
						*ph1 = j;
					*ph2 = j;
				}
			}
			else if (dataMode == 2)
			{
				if (((int*) data)[j*m_width+i] == iTextColor)
				{
					if (*ph1 == -1)
						*ph1 = j;
					*ph2 = j;
				}
			}
			else if (dataMode == 3)
			{
				if ((((int*) data)[3*j*m_width+3*i+0] == iTextColor) ||
					(((int*) data)[3*j*m_width+3*i+1] == iTextColor) ||
					(((int*) data)[3*j*m_width+3*i+2] == iTextColor))
				{
					if (*ph1 == -1)
						*ph1 = j;
					*ph2 = j;
				}
			}
		}
	}
}

void OneLine::CalculateVerticalHistogram(void* data, int dataMode, int start, int finish,  int width, int* verticalHistogram, int iTextColor)
{
	for ( int i = start; i < finish; i++)
	{
		int numberTextPixels = 0;
		for ( int j = 0; j < m_halfHeight; j++)
		{
			if (dataMode == 0)
			{
				if ((((byte*) data)[3*j*m_width+3*i+0] == iTextColor) ||
					(((byte*) data)[3*j*m_width+3*i+1] == iTextColor) ||
					(((byte*) data)[3*j*m_width+3*i+2] == iTextColor))
				{
					numberTextPixels++;
				}
			}
			else if (dataMode == 1)
			{
				if (((byte*) data)[j*m_width+i] == iTextColor)
				{
					numberTextPixels++;
				}
			}
			else if (dataMode == 2)
			{
				if (((int*) data)[j*m_width+i] == iTextColor)
				{
					numberTextPixels++;
				}
			}
			else if (dataMode == 3)
			{
				if ((((int*) data)[3*j*m_width+3*i+0] == iTextColor) ||
					(((int*) data)[3*j*m_width+3*i+1] == iTextColor) ||
					(((int*) data)[3*j*m_width+3*i+2] == iTextColor))
				{
					numberTextPixels++;
				}
			}

		}
		verticalHistogram[i] = numberTextPixels;
	}

}
int OneLine::CalculateBodyHeight()
{
	for ( int j = 0; j < m_halfHeight; j++)
	{
		byte prevprevColor = m_pDataIn[j*m_width];
		byte prevColor = m_pDataIn[j*m_width+1];
		int start = -1;
		int finish = -1;
		for ( int i = 0; i < m_halfWidth; i++)
		{
			//byte color = m_pDataIn[j*m_width+i];
			byte color = m_pDataOut[j*m_width*3+i*3+0];
			//m_pDataOut[j*m_width*3+i*3+2] = color;

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

	for ( int i = 0; i < m_halfWidth; i++)
	{
		byte prevprevColor = m_pDataOut[0*m_width*3+i*3+1];
		byte prevColor = m_pDataOut[1*m_width*3+i*3+1];
		int start= -1;
		int finish = -1;
		for ( int j = 0; j < m_halfHeight; j++)
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
	for ( int j = 10; j < m_halfHeight; j++)
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
	for ( int i = 0; i < m_halfWidth; i++)
	{
		byte prevprevColor = m_pDataOut[0*m_width*3+i*3+1];
		byte prevColor = m_pDataOut[1*m_width*3+i*3+1];
		int start= -1;
		int finish = -1;
		for ( int j = 0; j < m_halfHeight; j++)
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


	for ( int j = 0; j < m_halfHeight; j++)
	{
		for ( int i = 0; i < m_halfWidth; i++)
		{

			m_pDataOut[j*m_width*3+i*3+1] = m_pDataOut[j*m_width*3+i*3+0];
		}
	}
	return jMax;
}