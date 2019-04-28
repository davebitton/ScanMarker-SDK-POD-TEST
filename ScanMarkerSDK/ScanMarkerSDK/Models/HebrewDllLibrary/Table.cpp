#ifdef _SCANMARKER_WINDOWS_
#include "xv.h"
#include "xvbmp.h"
#else
#include "Include/types_hebrewdll.h"
#endif
#include "Table.h"
#define TextColor 255

Table::Table(void)
{
	m_pDataIn = NULL;
	m_pDataRaw = NULL;
	m_pDataOut = NULL;
	
	m_width = 0;
	m_height = 0;
	m_outputWidth = 0;
	m_outputHeight = 0;

	m_pWorkData1 = NULL;
	m_pWorkData2 = NULL;

	m_begin_x = 0;
	m_end_x = 0;
	m_begin_y = 0;
	m_end_y = 0;
	m_StraightLineByX = NULL;
	m_StraightLineByY = NULL;

	m_isDebugMode = false;
	m_debugFileBasicName[0] = 0;
	m_dirName[0] = 0;
	

}


Table::~Table(void)
{
	m_pDataIn = NULL;
	m_pDataRaw = NULL;
	m_pDataOut = NULL;
	
	m_width = 0;
	m_height = 0;
	m_outputWidth = 0;
	m_outputHeight = 0;

	if (m_pWorkData1 )
	{
		delete []m_pWorkData1;
		m_pWorkData1 = NULL;
	}
	if (m_pWorkData2 )
	{
		delete []m_pWorkData2;
		m_pWorkData2 = NULL;
	}
	if (m_StraightLineByX )
	{
		delete []m_StraightLineByX;
		m_StraightLineByX= NULL;
	}
	
	if (m_StraightLineByY )
	{
		delete []m_StraightLineByY;
		m_StraightLineByY= NULL;
	}
	
}

void Table::Init(int width, int height, int outputWidth,byte* pDataIn, byte* pDAtaRaw, byte* pDataOut)
{
	m_pDataIn = pDataIn;
	m_pDataRaw = pDAtaRaw;
	m_pDataOut = pDataOut;
	
	m_width = width;
	m_height = height;
	m_outputWidth = outputWidth;
	m_outputHeight = height;
	AllocateMemory();

}

void Table::AllocateMemory()
{
	if (m_pWorkData1 )
	{
		delete []m_pWorkData1;
		m_pWorkData1 = NULL;
	}
	m_pWorkData1 = new byte[m_height*m_width*3];
	memset(m_pWorkData1, 0, m_height*m_width*3);



	if (m_pWorkData2 )
	{
		delete []m_pWorkData2;
		m_pWorkData2 = NULL;
	}
	m_pWorkData2 = new byte[m_height*m_width*3];
	memset(m_pWorkData2, 0, m_height*m_width*3);

	if (m_StraightLineByX )
	{
		delete []m_StraightLineByX;
		m_StraightLineByX= NULL;
	}
	m_StraightLineByX = new int[m_width];
	memset(m_StraightLineByX, 0, m_width*sizeof(int));

	if (m_StraightLineByY )
	{
		delete []m_StraightLineByY;
		m_StraightLineByY= NULL;
	}
	m_StraightLineByY = new int[m_height];
	memset(m_StraightLineByY, 0, m_height*sizeof(int));

}

void Table::FillSnake()
{


	


	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 + 0] = m_pDataIn[j*m_width+i];
			m_pWorkData1[j*m_width*3 + i*3 + 1] = m_pDataIn[j*m_width+i];
			m_pWorkData1[j*m_width*3 + i*3 + 2] = m_pDataIn[j*m_width+i];
		}
	}

	for (int i = m_begin_x; i <= m_end_x; i++)
	{
		int beginScope = i;
		int endScope = m_end_x;
		FindNearest(beginScope, 10, endScope);
		for ( int j = m_begin_y; j <= m_end_y; j++)
		{
			if ( m_pDataIn[j*m_width+i] == TextColor)
			{
				for ( int ii = beginScope; ii <= endScope; ii++)
				{
					m_pWorkData1[j*m_width*3 + ii*3 + 0] = TextColor;
				}
			}
		}

	}

}

void Table::FixDatainFromRaw(int addHeight, int addWidth)
{
	
	int max_h = 0;

	for ( int i = m_begin_x; i <= m_end_x; i++)
	{
		int j1 = m_end_y;
		int j2 = m_begin_y;
		for ( int j = m_begin_y; j <= m_end_y; j++)
		{
			if (m_pWorkData1[j*m_width*3 + i*3 + 0] == TextColor)
			{
				if ( j1 > j) j1 = j;
				if ( j2 < j) j2 = j;
			}
		}
		int h = j2 - j1 + 1;
		if ( max_h < h) 
			max_h = h;
	}

	for ( int i = m_begin_x; i <= m_end_x; i++)
	{
		int j1 = m_end_y;
		int j2 = m_begin_y;
		for ( int j = m_begin_y; j <= m_end_y; j++)
		{
			if (m_pWorkData1[j*m_width*3 + i*3 + 0] == TextColor)
			{
				if ( j1 > j) j1 = j;
				if ( j2 < j) j2 = j;
			}
		}
		j1 -= addHeight;
		j2 += addHeight;
		for ( int jj = j1; jj <= j2; jj++)
		{
			if ( jj < 0 || jj >= m_height) continue;
			m_pDataIn[jj*m_width+i] = m_pDataRaw[jj*m_width+i];
		}
		
	}
	for ( int delta = 0; delta <= addWidth; delta++)
	{
		for ( int j = m_begin_y; j <= m_end_y; j++)
		{
			m_pDataIn[j*m_width + m_begin_x - delta] = m_pDataRaw[j*m_width + m_begin_x - delta];
			m_pDataIn[j*m_width + m_end_x + delta] = m_pDataRaw[j*m_width + m_end_x + delta];
		}
	}
	
}
void Table::Calculate()
{

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < 5; j++)
		{
			m_pDataIn[j*m_width+i] = 0;
		}
	}



	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 +0] = m_pDataRaw[j*m_width+i];
			m_pWorkData1[j*m_width*3 + i*3 +1] = m_pDataRaw[j*m_width+i];
			m_pWorkData1[j*m_width*3 + i*3 +2] = m_pDataRaw[j*m_width+i];
		}
	}
	char fnOut[1000];
	sprintf(fnOut, "%s_raw.bmp", m_debugFileBasicName);
/*
	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_pWorkData1,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}
*/
	CalculateImageBoundaries();
	FillSnake();
	//char fnOut[1000];
	sprintf(fnOut, "%s_debug1.bmp", m_debugFileBasicName);
/*
	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_pWorkData1,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}
*/
	FixDatainFromRaw(3, 15);
	
	CalculateImageBoundaries();
	FillSnake();
	
	//char fnOut[1000];
	sprintf(fnOut, "%s_debug2.bmp", m_debugFileBasicName);
/*
	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_pWorkData1,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}
*/




	Straight();
	Italic();
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 +0] = 0;
			m_pWorkData1[j*m_width*3 + i*3 +1] = 0;
			m_pWorkData1[j*m_width*3 + i*3 +2] = 0;
		}
	}

	int j1 = (m_height - (m_end_y - m_begin_y) ) / 2;

	for ( int i = 0; i < m_width; i++)
	{
		int delta_j = -(int) m_StraightLineByX[i];// - j1;
		for ( int j = 0; j < m_height; j++)
		{
			int delta_i = -(int)m_StraightLineByY[j];
			if (!m_italicSuccess) 
				delta_i = 0;
			int new_i = i + delta_i;
			int new_j = j + delta_j;
			if ( new_i < 0 || new_i >= m_width) continue;
			if ( new_j < 0 || new_j >= m_height) continue;
			m_pWorkData1[new_j*m_width*3 + new_i*3 +0] = m_pDataIn[j*m_width+i];
			m_pWorkData1[new_j*m_width*3 + new_i*3 +1] = m_pDataIn[j*m_width+i];
			m_pWorkData1[new_j*m_width*3 + new_i*3 +2] = m_pDataIn[j*m_width+i];
			m_pWorkData2[new_j*m_width*3 + new_i*3 +0] = m_pDataRaw[j*m_width+i];
			m_pWorkData2[new_j*m_width*3 + new_i*3 +1] = m_pDataRaw[j*m_width+i];
			m_pWorkData2[new_j*m_width*3 + new_i*3 +2] = m_pDataRaw[j*m_width+i];
		}
	}

	//char fnOut[1000];
	sprintf(fnOut, "%s_straightItalic.bmp", m_debugFileBasicName);
/*
	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_pWorkData1,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}
*/
 {
		int i1 = m_width - 1;
		int i2 = 0;
		int j1 = m_height - 1;
		int j2 = 0;
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 5; j < m_height; j++)
			{
				if (m_pWorkData1[j*m_width*3 + i*3 +1] == TextColor)
				{
					if ( i < i1) i1 = i;
					if ( i > i2) i2 = i;
					if ( j < j1)
						j1 = j;
					if ( j > j2) j2 = j;
				}
			}
		}
	
		for ( int i = i1; i <= i2; i++)
		{
			for ( int j = j1; j <= j2; j++)
			{
				m_pWorkData1[j*m_width*3 + i*3 +0] = m_pWorkData2[j*m_width*3 + i*3 +0];
				m_pWorkData1[j*m_width*3 + i*3 +1] = m_pWorkData2[j*m_width*3 + i*3 +0];
				m_pWorkData1[j*m_width*3 + i*3 +2] = m_pWorkData2[j*m_width*3 + i*3 +0];
			}
		}
	}
/*
	for ( int j = 0; j < m_height; j++)
	{
		int delta = -(int)m_StraightLineByY[j];
		for ( int i = 0; i < m_width; i++)
		{
			int new_i = i + delta;
			if ( new_i < 0 || new_i >= m_width) continue;
			m_pWorkData2[j*m_width*3 + new_i*3 +0] = m_pWorkData1[j*m_width*3 + i*3 +0];
			m_pWorkData2[j*m_width*3 + new_i*3 +1] = m_pWorkData1[j*m_width*3 + i*3 +0];
			m_pWorkData2[j*m_width*3 + new_i*3 +2] = m_pWorkData1[j*m_width*3 + i*3 +0];
		}
	}
*/

	/*sprintf(fnOut, "%s_italicTable.bmp", m_debugFileBasicName);
		
	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_pWorkData2,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}
	*/
	
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_outputWidth*3 + i*3 +0] = m_pWorkData1[j*m_width*3 + i*3 +0];
			m_pDataOut[j*m_outputWidth*3 + i*3 +1] = m_pWorkData1[j*m_width*3 + i*3 +0];
			m_pDataOut[j*m_outputWidth*3 + i*3 +2] = m_pWorkData1[j*m_width*3 + i*3 +0];
		}
	}

	sprintf(fnOut, "%s_OutitalicTable.bmp", m_debugFileBasicName);
/*
	if (m_isDebugMode)
	{
		FILE* fp = fopen(fnOut, "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataOut,PIC24,m_outputWidth,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}
	}
*/

}
void Table::FindNearest(int begin_x, int numberChanges, int & end_x)
{

	int numberChangesinRaw[128];
	for ( int j = 0; j < 128; j++)
	{
		numberChangesinRaw[j] = 0;
	}
	end_x = -1;

	for ( int i = begin_x; i <= m_end_x; i++)
	{
		for ( int j = m_begin_y; j <= m_end_y; j++)
		{
			if (m_pDataIn[j*m_width+i+1] != m_pDataIn[j*m_width+i])
			{
				numberChangesinRaw[j]++;
				if ( numberChangesinRaw[j] >= numberChanges)
				{
					end_x = i;
					break;
				}
			}
		}
		if ( end_x >= 0) break;
	}

	if ( end_x == -1)
		end_x = m_end_x;
}


void Table::CalculateImageBoundaries()
{
	
	m_begin_x = m_width - 1;
	m_end_x = 0;
	m_begin_y = m_height - 1;
	m_end_y = 0;
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 3; j < m_height-3; j++)
		{
			if ( m_pDataIn[j*m_width + i] == TextColor)
			{
				if ( m_begin_x > i)
					m_begin_x = i;
				if ( m_end_x < i) 
					m_end_x = i;
				if ( m_begin_y > j) 
					m_begin_y = j;
				if ( m_end_y < j) 
					m_end_y = j;


			}
		}
	}
}

void Table::Straight()
{

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData2[j*m_width*3+i*3+0] = 0;
			m_pWorkData2[j*m_width*3+i*3+1] = 0;
			m_pWorkData2[j*m_width*3+i*3+2] = 0;
		}
	}

	int minSpaces= 7;
	for ( int i = m_begin_x; i <= m_end_x; i++)
	{
		int j = m_begin_y - minSpaces;
		while ( j >= 0)
		{
			if ( m_pDataRaw[j*m_width+i] == TextColor)
			{
				m_pWorkData2[j*m_width*3+i*3+0] = TextColor;
				break;
			}
			j--;
		}

		j = m_end_y + minSpaces;
		while ( j < m_height)
		{
			if ( m_pDataRaw[j*m_width+i] == TextColor)
			{
				m_pWorkData2[j*m_width*3+i*3+1] = TextColor;
				break;
			}
			j++;
		}
	}

	int prev_j1 = -1;
	int prev_j2 = -1;
	for (int i = m_begin_x; i <= m_end_x; i++)
	{
		int j1 = -1; 
		int j2 = -1;
		for ( int j = 0; j < m_height; j++)
		{
			if ( m_pWorkData2[j*m_width*3 + i*3 + 0] == TextColor)
				j1 = j;
			if ( m_pWorkData2[j*m_width*3 + i*3 + 1] == TextColor)
				j2 = j;

		}

		int delta = m_StraightLineByX[i-1];
		if ( prev_j1 != -1 && j1 != -1)
			delta += j1 - prev_j1;
		else if (prev_j2 != -1 && j2 != -1)
			delta += j2 - prev_j2;
		m_StraightLineByX[i] = delta;
		prev_j1 = j1;
		prev_j2 = j2;
	}





}
int Table::CheckForClick()
{


	int numberWhite = 0;
	int numberBlack = 0;

	int numberChanges = 0;
	int numberWhiteColumna = 0;


	int isClick = 0;
	for ( int j = 0; j < m_height; j++)
	{
		int numberChanges = 0;
		for ( int i = 1; i < m_width; i++)
		{
			if ( m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i-1])
			{
				numberChanges++;
			}
		}
		if ( numberChanges > 1) isClick++;
	}
	if ( isClick < 10) return -1000;
	
	return 0;

}



void Table::Italic()
{

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData2[j*m_width*3+i*3+0] = 0;
			m_pWorkData2[j*m_width*3+i*3+1] = 0;
			m_pWorkData2[j*m_width*3+i*3+2] = 0;
		}
	}

	int maxSpaces= 50;
	for ( int j = m_begin_y; j <= m_end_y; j++)
	{
		int i = m_begin_x - 1;
		while ( i >= 0 && i >= m_begin_x - maxSpaces )
		{
			if ( m_pDataRaw[j*m_width+i] == TextColor)
			{
				m_pWorkData2[j*m_width*3+i*3+0] = TextColor;
				break;
			}
			i--;
		}

		
	}

	int prev_i = -1;

	m_italicSuccess = true;
	for (int j = m_begin_y; j <= m_end_y; j++)
	{
		int i1 = -1; 
		for ( int i = m_begin_x - maxSpaces; i <= m_begin_x; i++)
		{
			if ( m_pWorkData2[j*m_width*3 + i*3 + 0] == TextColor)
				i1 = i;
			
		}
		if ( i1 == -1)
		{
			m_italicSuccess = false;
		}
		int delta = m_StraightLineByY[j-1];
		if ( prev_i != -1 && i1 != -1)
			delta += i1 - prev_i;
		m_StraightLineByY[j] = delta;
		prev_i = i1;
		
		if (abs(m_StraightLineByY[j] -  m_StraightLineByY[j-1]) > 3)
			m_italicSuccess = false;
	}



	

}
