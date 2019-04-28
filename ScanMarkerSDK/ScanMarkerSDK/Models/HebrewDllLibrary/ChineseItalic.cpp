#include "ChineseItalic.h"
#include "math.h"
#include "xvbmp.h"

#define _MAX_HEIGHT_ 128

#define TextColor 255
ChineseItalic::ChineseItalic(void)
{
	m_width = 0;
	m_height = 0;
	
	m_pDataIn = NULL;
	m_pWorkData = NULL;
	
	for (int st = 0; st < MAX_NUMBER_HEBREW_STEPS; st++)
		m_pItalicShifts[st] = NULL;
	

	for (int w = 0; w < MAX_NUMBER_WORDS; w++)
	{
		m_beginLetters[w] = -1;
		m_endLetters[w] = -1;
	}

	m_numberLetters = 0;

	
}

ChineseItalic::~ChineseItalic(void)
{
	for (int st = 0; st < MAX_NUMBER_HEBREW_STEPS; st++)
	{
		if (m_pItalicShifts[st])
		{
			delete [] m_pItalicShifts[st];
			m_pItalicShifts[st] = NULL;
		}
	}
	if (m_pWorkData)
	{
		delete [] m_pWorkData;
		m_pWorkData = NULL;
	}
}

void ChineseItalic::Init(int width, int height, byte* pDataIn)
{
	m_pDataIn = pDataIn;
	m_width = width;
	m_height = height;

	AllocateMemory();
}
	
void ChineseItalic::AllocateMemory()
{
	for (int st = 0; st < MAX_NUMBER_HEBREW_STEPS; st++)
		m_pItalicShifts[st] = new int[m_height];;
	
	m_pWorkData  = new byte[m_width*m_height];
}


void ChineseItalic::Calculate()
{
	
	for ( int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			m_pWorkData[j*m_width+i] = m_pDataIn[j*m_width+i];
		}
	}
	CalculateBeginEndOfWholeImage();


	int begin = m_begin_x;
	int end = m_begin_x +1;

	int l = 0;
	bool isEnd = false;
	int numberChangesToFind = 25;
	while (true)
	{
		FindPieceOfImageByNumberChangesinRaw(numberChangesToFind, begin, end); 
		if ( end < begin + 5) break;
		int h1 = 0, h2 = 0;
		int maxSteps = 0;
		CalculateHeightsOfPieceimage(begin, end, h1, h2);
		begin -= MAX_NUMBER_HEBREW_STEPS/2;
		CalculateItalicShifts( h1, h2, maxSteps);
		if (h1 < 0) h1 = 0;
		if (h2 >= m_height) h2 = m_height-1;
		int maxSpaceColumnWidth = 0;
		int beginCurrentLetter[MAX_NUMBER_HEBREW_STEPS];
		int endCurrentLetter[MAX_NUMBER_HEBREW_STEPS];
		for (int s = 0; s < MAX_NUMBER_HEBREW_STEPS; s++)
		{
			beginCurrentLetter[s]= -1;
			endCurrentLetter[s]= end;
		}
		for ( int nSteps = 0; nSteps < maxSteps; nSteps++)
		{
			for (int kk = 0; kk < 2; kk++)
			{
				int step = nSteps+MAX_NUMBER_HEBREW_STEPS/2;
				if (kk == 1)
					step = -nSteps+MAX_NUMBER_HEBREW_STEPS/2;
				if (step < 0 || step >= MAX_NUMBER_HEBREW_STEPS)
					continue;
				int spaceColumnWidth = 0;
				int beginSpace = 0;
				bool textColumnFound = false;
				bool spaceAfterTextFound = false;
				for ( int i = begin; i < end; i++)
				{
					if (i < 0) i = 0;
					if (i >= m_width ) break;
					bool isSpaceColumn = true;
					for (int j = h1; j <= h2; j++)
					{
						int ii = i - m_pItalicShifts[step][j];
						if ( ii < 0) continue;
						if (ii >= m_width) continue;
						if (m_pWorkData[j*m_width+ii] == TextColor)
						{
							isSpaceColumn = false;
							textColumnFound = true;
							if (beginCurrentLetter[step] == -1)
								beginCurrentLetter[step] = i;
							break;
						}
					}
					if (!textColumnFound) continue;
					if (isSpaceColumn)
					{
						spaceColumnWidth++;
						if (spaceColumnWidth == 1)
						{
							beginSpace = i;
						}
						if (spaceColumnWidth > 3)
							spaceAfterTextFound = true;
					}
					else if (spaceAfterTextFound && (i - beginCurrentLetter[step] > 5))
					{
						endCurrentLetter[step] = beginSpace-1;
						break;
					}
					
				}
			}

		//	if (maxSpaceColumnWidth > 10)
		//		break;
		}
		int bestStep= -1;
		int minWidth = 10000;
		for ( int step = 0; step < MAX_NUMBER_HEBREW_STEPS; step++)
		{
			if (beginCurrentLetter[step] == -1) continue;
			int widthLetter = endCurrentLetter[step] - beginCurrentLetter[step];
			if (widthLetter < minWidth)// && widthLetter >= 4)
			{
				minWidth = widthLetter;
				bestStep= step;
			}
		}
		if ( beginCurrentLetter[bestStep] >= 0 && endCurrentLetter[bestStep] >= beginCurrentLetter[bestStep])
		{
			int bl = beginCurrentLetter[bestStep];
			int el = endCurrentLetter[bestStep];
			int i1 = m_width-1;
			int i2 = 0;
			for (int i = bl; i <= el; i++)
			{
				for ( int j = h1; j <= h2; j++)
				{
					int ii = i - m_pItalicShifts[bestStep][j];
					if ( ii < 0) continue;
					if (ii >= m_width) continue;
					if (ii < i1) i1 = ii;
					if (ii > i2) i2 = ii;
					m_pWorkData[j*m_width+ii]= 255 - TextColor;
				}
			}
			/*{
				byte * debugAr = new byte[m_width*m_height*3];
				for ( int i = 0; i < m_width; i++)
				{
					for ( int j = 0; j < m_height; j++)
					{
						debugAr[j*m_width*3+i*3+0] = m_pWorkData[j*m_width+i];
						debugAr[j*m_width*3+i*3+1] = m_pWorkData[j*m_width+i];
						debugAr[j*m_width*3+i*3+2] = m_pWorkData[j*m_width+i];
					}
				}
				FILE* fp = fopen("C:\\Temp\\debug1.bmp", "wb");
				if (fp)
				{
					WriteBMP(fp,debugAr,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
					fclose(fp);
				}
				delete [] debugAr;
			}*/
			m_beginLetters[l]  = i1;
			m_endLetters[l] = i2;
			l++;
			
			numberChangesToFind = 25;
		}
		else
		{
			numberChangesToFind += 25;
		}

	}

	m_numberLetters = l;

}
void ChineseItalic::CalculateBeginEndOfWholeImage()
{
	m_begin_x = m_width-1;
	m_end_x = 0;
	for ( int i = 1; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			if (m_pWorkData[j*m_width+i] == TextColor)
			{
				if (m_begin_x > i) m_begin_x = i;
				if (m_end_x < i) m_end_x = i;
			}
		}
	}
	m_end_x++;
	if (m_end_x >= m_width) m_end_x = m_width-1;

}
void ChineseItalic::FindPieceOfImageByNumberChangesinRaw(int numberChangesToFind, int &begin, int &end)
{
	int numberChangesInRaw[_MAX_HEIGHT_];
	for ( int j = 0; j < _MAX_HEIGHT_; j++)
	{
		numberChangesInRaw[j] = 0;
	}
	begin = m_end_x;
	end = m_end_x;
	for ( int i = m_begin_x -1; i < m_end_x; i++)
	{
        if ( i < 1 || i >= m_width-1) continue;
		for (int j = 0; j < m_height; j++)
		{
			if (m_pWorkData[j*m_width+i] != m_pWorkData[j*m_width+i-1])
			{
				if (begin > i) begin = i;
				numberChangesInRaw[j]++;
				if (numberChangesInRaw[j] >= numberChangesToFind)
				{
					end = i;
					return;
				}
			}
		}
	}
}

void ChineseItalic::CalculateHeightsOfPieceimage(int begin, int end, int &h1, int &h2)
{
	h1 = m_height-1;
	h2 = 0;
	for (int i = begin; i <= end; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			if ( m_pWorkData[j*m_width+i] == TextColor)
			{
				if (h1 > j) h1 = j;
				if (h2 <j) h2 = j;
			}
		}
	}
	h1--;
	h2++;
	if (h1 < 0) h1 = 0;
	if (h2 >= m_height) h2 = m_height-1;
}
	
	
void ChineseItalic::CalculateItalicShifts( int h1, int h2, int &maxSteps)
{
	for ( int nSteps = 0; nSteps < MAX_NUMBER_HEBREW_STEPS; nSteps++)
	{
		for ( int j = 0; j < _MAX_HEIGHT_; j++)
		{
			m_pItalicShifts[nSteps][j] = 0;
		}
	}
	 maxSteps = h2 - h1;
	for ( int nSteps = -maxSteps; nSteps < maxSteps; nSteps++)
	{
	
		float sColumns = h2 - h1+1;
		if (nSteps != 0)
			sColumns = (float)(h2 - h1+1)/ (float) nSteps;
		sColumns = fabs(sColumns);
		float i_s_Columns = 0;
		int j1 = h2;
		int j2;
		int first, last, st;
		st = (nSteps < 0) ? -1 : 1;
		int shift = abs(nSteps);
		if (nSteps == 0) nSteps = 1;
		for ( int s = 0; s != nSteps; s+=st)
		{
			j2 = j1-1;
			i_s_Columns += sColumns;
			j1 = h2- (int)(i_s_Columns+0.5);
    		 for ( int j = j1; j <= j2; j++)
			 {
				 if ( (j < 0) || (j >= m_height))
					 continue;

				 if (m_pItalicShifts)
				 {
						m_pItalicShifts[nSteps+MAX_NUMBER_HEBREW_STEPS/2][j] = s;
				 }
			 }
		}
	}
}