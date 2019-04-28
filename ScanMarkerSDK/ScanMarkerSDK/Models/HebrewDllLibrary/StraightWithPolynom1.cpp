#include "StraightWithPolynom1.h"
#include "InOut.h"
#ifdef _SCANMARKER_WINDOWS_
#include "xvbmp.h"
#else
#include "bmp/xvbmp.h"
#endif
#include <vector>
#define APP_ENGINE_LNG_JAPANESE 		278
#define APP_ENGINE_LNG_KOREAN					232	
#define APP_ENGINE_LNG_CMC7					    332
#define APP_ENGINE_LNG_CHINESE_TRADITIONAL		221
#define APP_ENGINE_LNG_CHINESE_SIMPLIFIED		222
#define APP_ENGINE_LNG_HEBREW					230

#define PI 3.141592

CStraightWithPolynom1::CStraightWithPolynom1(void)
{

	for ( int l = 0; l < _MAX_NUMBER_LETTERS_; l++)
	{
		m_letterEdges[l][0] = -1;
		m_letterEdges[l][1] = -1;
		m_letterEdges[l][2] = -1;
		m_letterEdges[l][3] = -1;
		m_letterPointUp[l][0] = -1;
		m_letterPointUp[l][1] = -1;
		m_letterPointDown[l][0] = -1;
		m_letterPointDown[l][1] = -1;
		m_letterHeight[l] = -1;
	}
	
	m_doNothing = false;
	m_isTable = false;
	m_pDataRaw = NULL;
	m_widthRaw = 0;
	m_heightRaw = 0;
	m_isVertical = false;
	m_aveHeightDefault= -1;


		_UP_EDGE_  = 124;
	_DOWN_EDGE_  = 4;
	_NUMBER_CHANGES_TO_ANALYZE_ = 3;
	_REFERENCE_BODY_HEIGHT_ = 45;
	

	m_aveBodyHeight = 35;


	m_pWorkData1 = NULL;
	m_pWorkData2 = NULL;
	m_pWorkData3 = NULL;
	m_pWorkData4 = NULL;
	m_pBody = NULL;
	m_width = 0;
	m_height = 0;
	m_pDataIn = NULL;
	m_pDataOut = NULL;
	m_body = NULL;
	m_bodyHeight = NULL;
	m_begin_x = 0;
	m_end_x = m_width-1;
	m_begin_y = 0;
	m_end_y = m_height-1;
	m_To = m_begin_x;
	m_pInterpolationOutput= NULL;
	m_pInterpolationInput = NULL;
	/*for ( int j = 0; j < _MAX_IMAGE_HEIGHT_; j++)
	{
		m_pChangesCoord[j] = NULL;
	}
	*/
	//_KOREAN_THICK_ = false;	
	m_heightOfOutputBody = 44;

}

CStraightWithPolynom1::~CStraightWithPolynom1(void)
{

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
		if (m_pWorkData4 != NULL)
		{
			delete[] m_pWorkData4;
			m_pWorkData4 = NULL;
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
		if (m_pInterpolationOutput)
		{
			delete[] m_pInterpolationOutput;
			m_pInterpolationOutput= NULL;
		}
		if (m_pInterpolationInput)
		{
			delete[] m_pInterpolationInput;
			m_pInterpolationInput= NULL;
		}
		/*for ( int j = 0; j < m_height; j++)
	    {
			if (m_pChangesCoord[j] != NULL)
			{
				delete[] m_pChangesCoord[j];
				m_pChangesCoord[j] = NULL;
			}
		}*/

		
}
void CStraightWithPolynom1::AllocateMemory()
{
	m_width += 600;

		if (m_pBody!= NULL)
		{
			delete[] m_pBody;
			m_pBody = NULL;
		}
		m_pBody = new int[m_width*m_height];
		memset(m_pBody, 0, m_width*m_height*sizeof(int));
		if (m_body!= NULL)
		{
			delete[] m_body;
			m_body = NULL;
		}
		m_body= new int[m_width];
		if (m_bodyHeight != NULL)
		{
			delete[] m_bodyHeight;
			m_bodyHeight = NULL;
		}
		m_bodyHeight = new int[m_width];

		/*for ( int j = 0; j < m_height; j++)
		{
			if (m_pChangesCoord[j] != NULL)
			{
				delete[] m_pChangesCoord[j];
				m_pChangesCoord[j] = NULL;
			}
			m_pChangesCoord[j] = new int[_NUMBER_CHANGES_TO_ANALYZE_+2];
		}*/

		
		if (m_pInterpolationInput)
		{
			delete[] m_pInterpolationInput;
			m_pInterpolationInput= NULL;
		}
		m_pInterpolationInput = new float[m_output_width];
		for ( int i = 0; i < m_output_width; i++)
			m_pInterpolationInput[i] = 0;

		if (m_pInterpolationOutput)
		{
			delete[] m_pInterpolationOutput;
			m_pInterpolationOutput= NULL;
		}
		m_pInterpolationOutput= new float[m_output_width];
		for ( int i = 0; i < m_output_width; i++)
		m_pInterpolationOutput[i] = 0;

		if (m_pWorkData1!= NULL)
		{
			delete[] m_pWorkData1;
			m_pWorkData1 = NULL;
		}
		m_pWorkData1 = new byte[(m_output_width + 600)*m_height*3];
		memset(m_pWorkData1, 255-TextColor, (m_output_width + 600)*m_height*3*sizeof(byte));

		if (m_pWorkData2 != NULL)
		{
			delete[] m_pWorkData2;
			m_pWorkData2 = NULL;
		}
		m_pWorkData2 = new int[m_width*m_height*3*2*3];
		memset(m_pWorkData2 , 0, m_width*m_height*3*2*3*sizeof(int));

		if (m_pWorkData3!= NULL)
		{
			delete[] m_pWorkData3;
			m_pWorkData3 = NULL;
		}
		m_pWorkData3= new int[m_width*m_height*3*3];
		memset(m_pWorkData3, 255-TextColor, m_width*m_height*3*3*sizeof(int));



		if (m_pWorkData4!= NULL)
		{
			delete[] m_pWorkData4;
			m_pWorkData4 = NULL;
		}
		m_pWorkData4= new int[m_width*m_height*3];
		memset(m_pWorkData4, 255-TextColor, m_width*m_height*3*sizeof(int));


		
	m_width -= 600;

		
}

void CStraightWithPolynom1::Init(int width, int height, int outputWidth, byte* pDataIn, byte* pDataOut, byte* pDataIn3)
{
	m_width = width;
	m_output_width = outputWidth;
	m_height = height;
	m_pDataIn = pDataIn;
	m_pDataOut = pDataOut;
	m_pDataIn3 = pDataIn3;
	AllocateMemory();
	


}
void CStraightWithPolynom1::ReadCfgFile(char* dir)
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
void CStraightWithPolynom1::PrintDebugStr(char* str, bool append)
{
	return;
	FILE * fff = NULL;
	if (append)
		fff = fopen("C://Temp//debug5.txt", "at");
	else
		fff = fopen("C://Temp//debug5.txt", "wt");

	if (!fff) return;
	fprintf(fff, "\n%s", str);
	fclose(fff);
}
void CStraightWithPolynom1::PrintTimeStr(char* str, int ms, bool append)
{
	return;
	FILE * fff = NULL;
	//if (append)
		fff = fopen("C:\\Temp\\TextImageTime.txt", "at");
	//else
		//fff = fopen("C://Temp//debug5.txt", "wt");

	if (!fff) return;
	fprintf(fff, "\n%s %d", str, ms);
	fclose(fff);
}
void CStraightWithPolynom1::CalculateLetterEdges_CMC7()
{
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 + 1] = 0;
			m_pWorkData1[j*m_width*3 + i*3 + 0] = 0;
			m_pWorkData1[j*m_width*3 + i*3 + 2] = 0;
		}
	}
	
	int i_letter = 0;
	int i_start = m_begin_x;
	int step = 20;
	while (i_start < m_end_x)
	{
		
		int i_begin = i_start + step;
		
		int i_end = i_start;
		int j_begin = m_height - 5;
		int j_end =  5;
		for ( int i = i_start; i < i_start+step; i++)
		{
			if ( i >= m_width - 5) continue;
			for ( int j = 0; j < m_height; j++)
			{
				if ( m_pDataIn[j*m_width+i] == TextColor)
				{
					if ( i_begin > i) i_begin = i;
					if ( i_end < i) i_end = i;
					if ( j_begin > j) j_begin = j;
					if ( j_end < j) j_end = j;
				}
			}
		}
		i_start += step;
		if ( i_begin >= i_end)
			continue;

		for ( int i = i_begin; i <= i_end ; i++)
		{
			for ( int j = j_begin; j <= j_end; j++)
			{
				m_pWorkData1[j*m_width*3 + i*3 + 1] = 100;
			}
		}

		m_letterEdges[i_letter][0] = i_begin;
		m_letterEdges[i_letter][1] = j_begin;
		m_letterEdges[i_letter][2] = i_end;
		m_letterEdges[i_letter][3] = j_end;
		for ( int i = i_begin; i <= i_end ; i++)
		{
			if ( m_pDataIn[j_begin * m_width + i] == TextColor)
			{
				m_letterPointUp[i_letter][0] = i;
				m_letterPointUp[i_letter][1] = j_begin;
			}
			if ( m_pDataIn[j_end * m_width + i] == TextColor)
			{
				m_letterPointDown[i_letter][0] = i;
				m_letterPointDown[i_letter][1] = j_end;
			}
		}
		m_letterHeight[i_letter] = j_end - j_begin + 1;;
		if ( i_letter < _MAX_NUMBER_LETTERS_-1 )
			i_letter++;


		//i_begin = i_end;
		//j_begin = j_end;

	}
	m_numberLetters = i_letter;
		
}

void CStraightWithPolynom1::CalculateLetterEdgesVerticalAsia()
{
	int i_begin = 10000000;
	int i_end = -1;
	int j_begin = 10000000;
	int j_end = -1;
	bool insideWord = false;
	int numberEmptyColumns = 0;
	int i_letter = 0;
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
			numberEmptyColumns++;
		else
			numberEmptyColumns = 0;
		if (insideWord && (numberEmptyColumns >= 2 || i >= m_end_x-1))
		{
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
void CStraightWithPolynom1::CalculateLetterEdges()
{


	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width*3 + i*3 + 1] = 0;
			m_pWorkData1[j*m_width*3 + i*3 + 0] = 0;
			m_pWorkData1[j*m_width*3 + i*3 + 2] = 0;
		}
	}
	
	int i_begin = m_begin_x-1;
	int j_begin = 5;

	int i_letter = 0;

	while (i_begin < m_end_x)
	{
		bool found = false;
		for ( ;i_begin < m_end_x+1; i_begin++)
		{
			for ( j_begin = m_begin_y; j_begin < m_end_y+1; j_begin++)
			{
				if ((m_pDataIn[j_begin*m_width + i_begin] == TextColor) && (m_pWorkData1[(j_begin)*m_width*3 + (i_begin)*3 + 2] == 0)) 
				{
					found = true;
					break;
				}
			}
			if (found) break;
		}
		if ( i_begin >= m_end_x) break;

		int i_end = i_begin;
		int j_end = j_begin;
		int count = 1;
		m_pWorkData1[(j_begin)*m_width*3 + (i_begin)*3 + 2] = 1;
		while (count > 0)
		{
			count = 0;
			for ( int i = i_begin; i <= i_end; i++)
			{
				for ( int j = j_begin; j <= j_end; j++)
				{
					//if (m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] == 2) continue;
					if (m_pDataIn[(j)*m_width + i] != TextColor) continue;
					if (m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] != 1) continue;
															
					if ( i < 2) continue;
					if ( i >= m_width - 2) continue;
					if ( j < 5) continue;
					if ( j >= m_height - 2) continue;
					for ( int ii = i-1; ii <= i+1; ii++)
					{
						for ( int jj = j-1; jj <= j+1; jj++)
						{
							if ( ii == i && jj == j) continue;
							if (m_pDataIn[(jj)*m_width + ii] == TextColor) 
							{
								if (m_pWorkData1[(jj)*m_width*3 + (ii)*3 + 2] == 0)
								{
									m_pWorkData1[(jj)*m_width*3 + (ii)*3 + 2] = 1;
									count++;
									if (i_begin > ii) i_begin = ii;
									if ( i_end < ii) i_end = ii;
									if (j_begin > jj) j_begin = jj;
									if ( j_end < jj) j_end = jj;
								}
							}
						}
					}
					m_pWorkData1[(j)*m_width*3 + (i)*3 + 2] = 2;
				}
			}
		}
		for ( int i = i_begin; i <= i_end ; i++)
		{
			for ( int j = j_begin; j <= j_end; j++)
			{
				m_pWorkData1[j*m_width*3 + i*3 + 1] = 100;
			}
		}




int numberLetters = CheckConnectedLetters(i_begin, i_end, j_begin, j_end);

		if ( numberLetters > 1)
		{
			int w = (i_end - i_begin) / numberLetters;
			for ( int ll = 0; ll < numberLetters; ll++)
			{
				int i1 = i_begin + ll*w;
				int i2 = i1 + w;
				int j1 = m_height;
				int j2 = 0;
				for ( int j = j_begin; j < j_end; j++)
				{
					for ( int i = i1; i < i2; i++)
					{
						if ( m_pDataIn[j*m_width+i] == TextColor)
						{
							if ( j1 > j) j1 = j;
							if (j2 < j) j2 = j;
						}
					}
				}
				m_letterEdges[i_letter][0] = i1;
				m_letterEdges[i_letter][1] = j1;
				m_letterEdges[i_letter][2] = i2;
				m_letterEdges[i_letter][3] = j2;
				for ( int i = i1; i <= i2; i++)
				{
					if ( m_pDataIn[j1* m_width + i] == TextColor)
					{
						m_letterPointUp[i_letter][0] = i;
						m_letterPointUp[i_letter][1] = j1;
					}
					if ( m_pDataIn[j2* m_width + i] == TextColor)
					{
						m_letterPointDown[i_letter][0] = i;
						m_letterPointDown[i_letter][1] = j2;
					}
				}
				m_letterHeight[i_letter] = j2- j1+ 1;;
				
				if ( i_letter < _MAX_NUMBER_LETTERS_-1 )
					i_letter++;

			}
			continue;
		}


		if (m_language ==APP_ENGINE_LNG_CHINESE_SIMPLIFIED ||  //Korean
			m_language ==APP_ENGINE_LNG_CHINESE_TRADITIONAL ||  //Korean
			m_language ==APP_ENGINE_LNG_KOREAN ||  //Korean
			m_language == APP_ENGINE_LNG_JAPANESE)    //Japanese
		{
			
			for ( int i = i_begin; i <= i_end; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					if ( m_pDataIn[j*m_width+i] == TextColor)
					{
						if ( j < j_begin) j_begin = j;
						if ( j > j_end ) j_end = j;

					}
				}
			}
		}


		m_letterEdges[i_letter][0] = i_begin;
		m_letterEdges[i_letter][1] = j_begin;
		m_letterEdges[i_letter][2] = i_end;
		m_letterEdges[i_letter][3] = j_end;
		for ( int i = i_begin; i <= i_end ; i++)
		{
			if ( m_pDataIn[j_begin * m_width + i] == TextColor)
			{
				m_letterPointUp[i_letter][0] = i;
				m_letterPointUp[i_letter][1] = j_begin;
			}
			if ( m_pDataIn[j_end * m_width + i] == TextColor)
			{
				m_letterPointDown[i_letter][0] = i;
				m_letterPointDown[i_letter][1] = j_end;
			}
		}
		m_letterHeight[i_letter] = j_end - j_begin + 1;;
		if ( i_letter < _MAX_NUMBER_LETTERS_-1 )
			i_letter++;


		//i_begin = i_end;
		//j_begin = j_end;

	}
	m_numberLetters = i_letter;
		
}
int CStraightWithPolynom1::CheckConnectedLetters( int i1, int i2, int j1, int j2)
{
	int maxChanges = 0;
	for ( int j = j1; j < j2; j++)
	{
		int nChanges = 0;
		for ( int i = i1; i <= i2; i++)
		{
			if (m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width + i-1]) 
				nChanges++;
		}
		if (nChanges > maxChanges) maxChanges = nChanges;
	}

	int w = i2 - i1;
	if ( !w) w = 1;
	if (maxChanges < 6) return 1;
	int n = maxChanges / 4;
	if (m_language ==APP_ENGINE_LNG_CHINESE_SIMPLIFIED ||  //Korean
			m_language ==APP_ENGINE_LNG_CHINESE_TRADITIONAL ||  //Korean
			m_language ==APP_ENGINE_LNG_KOREAN ||  //Korean
			m_language == APP_ENGINE_LNG_JAPANESE)    //Japanese
		n = maxChanges / 8;
	if (n == 0) n = 1;
	//int oneChangeWidth = w / maxChanges;
	int oneLetterWidth = w / n;
	if (oneLetterWidth > 15 ) return n;
	if (w < 100) return 1;
	return (w/40);

}
void CStraightWithPolynom1::ConnectLetters()
{
	for  (int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData2[j*m_width*3 + i*3 +0] = -1;
			m_pWorkData2[j*m_width*3 + i*3 +1] = -1;
			m_pWorkData2[j*m_width*3 + i*3 +2] = -1;
			m_pWorkData3[j*m_width*3 + i*3 +0] = -1;
			m_pWorkData3[j*m_width*3 + i*3 +1] = -1;
			m_pWorkData3[j*m_width*3 + i*3 +2] = -1;


		}
	}
	for ( int l = 0; l < m_numberLetters; l++)
	{
		for ( int  i = m_letterEdges[l][0]; i <= m_letterEdges[l][2]; i++)
		{
            if (i < 0 || i >= m_width-1) continue;
			for ( int j = m_letterEdges[l][1]; j <= m_letterEdges[l][3]; j++)
			{
                if (j < 0 || j >= m_height-1) continue;
				m_pWorkData2[j*m_width*3+i*3+0] = l;
			}
		}
	}


PrintDebugStr("aaaaaaaa");

	int i_begin = 0;
	int j_begin = 0;
	while (true)
	{
		bool found = false;
		for ( int i = i_begin; i < m_width; i++)
		{
            if (i < 0 || i >= m_width-1) continue;
			for ( int j = 0; j < m_height; j++)
			{
				if (m_pWorkData2[j*m_width*3+i*3+0] != -1 && m_pWorkData2[j*m_width*3+i*3+2] == -1)
				{
					i_begin = i;
					j_begin = j;
					found = true;
					break;
				}
			}
			if (found) break;
		}

		if (!found) break;
PrintDebugStr("bbbbbbb");

		int l1= m_pWorkData2[j_begin*m_width*3 + i_begin*3 + 0];
		int j1 = m_letterEdges[l1][1];
		int j2 = m_letterEdges[l1][3];

		int l2 = -1;
		found = false;
		for ( int i = m_letterEdges[l1][0]; i >= 0; i--)
		{
            if (i < 0 || i >= m_width-1) continue;
			for ( int j = j1; j <= j2; j++)
			{
                if (j < 0 || j >= m_height-1) continue;
				if (m_pWorkData2[j*m_width*3 + i*3 + 0] != -1)
				{
					l2 = m_pWorkData2[j*m_width*3 + i*3 + 0];
					found = true;
					break;
				}
			}
			if ( found) break;
		}
PrintDebugStr("ccccccccc");
		int i1 = m_letterEdges[l1][0];
		int i2 = m_letterEdges[l1][2];
		j1 = m_letterEdges[l1][1];
		j2 = m_letterEdges[l1][3];
				
		if ( l2 != -1)
		{
			i1 = m_letterEdges[l2][0];
			if (m_letterEdges[l2][1] < j1) 
				j1 = m_letterEdges[l2][1];
			if (m_letterEdges[l2][3] > j2)  
				j2 = m_letterEdges[l2][3];
		}
PrintDebugStr("dddddddddd");

		for ( int j = j1; j <= j2; j++)
		{
            if (j < 0 || j >= m_height-1) continue;
			int ii1 = i1;
			int ii2 = i2;
            if (ii1 < 0 || ii1>= m_width-1) continue;
            if (ii2< 0 || ii2 >= m_width-1) continue;
			while (m_pDataIn[j*m_width+ii1] != TextColor && ii1 < m_width-1) ii1++;
			while (m_pDataIn[j*m_width+ii2] != TextColor && ii2 > 0) ii2--;
			for ( int i = ii1; i <= ii2; i++)
			{
                if (i < 0 || i >= m_width-1) continue;
				m_pWorkData2[j*m_width*3+i*3+1] = 100;
			}
		}
PrintDebugStr("eeeeeeeeee");

		for ( int i = i1; i <= i2; i++)
		{
            if (i < 0 || i >= m_width-1) continue;
			int jj1 = j1;
			int jj2 = j2;
            if (jj1 < 0 || jj1 >= m_height-1) continue;
            if (jj2 < 0 || jj2 >= m_height-1) continue;
			while (m_pDataIn[jj1*m_width+i] != TextColor && jj1 < m_height-1) jj1++;
			while (m_pDataIn[jj2*m_width+i] != TextColor && jj2 > 0) jj2--;
			for ( int j = jj1; j <= j2; j++)
			{
                if (j < 0 || j >= m_height-1) continue;
				m_pWorkData3[j*m_width*3+i*3+0] = 100;
			}
		}

		PrintDebugStr("ffffffffff");

		for ( int i = m_letterEdges[l1][0]; i <= m_letterEdges[l1][2]; i++)
		{
            if (i < 0 || i >= m_width-1) continue;
			for ( int j = m_letterEdges[l1][1]; j <= m_letterEdges[l1][3]; j++)
            {
                if (j < 0 || j >= m_height-1) continue;
				m_pWorkData2[j*m_width*3+i*3+2] = 100;
            }
		}
PrintDebugStr("gggggggggg");

		if ( l2 == -1) continue;
		
		int l3 = -1;
		found = false;
		for ( int i = m_letterEdges[l2][0]; i >= 0; i--)
		{
            if (i < 0 || i >= m_width-1) continue;
			for ( int j = j1; j <= j2; j++)
			{
                if (j < 0 || j >= m_height-1) continue;
				if (m_pWorkData2[j*m_width*3 + i*3 + 0] != -1)
				{
					l3 = m_pWorkData2[j*m_width*3 + i*3 + 0];
					found = true;
					break;
				}
			}
			if ( found) break;
		}

		i1 = m_letterEdges[l1][0];
		i2 = m_letterEdges[l1][2];
		j1 = m_letterEdges[l1][1];
		j2 = m_letterEdges[l1][3];

		if (l3 == -1) continue;
		PrintDebugStr("hhhhhhhhhhh");

		i1 = m_letterEdges[l3][0];
		if (m_letterEdges[l3][1] < j1) 
		j1 = m_letterEdges[l3][1];
		if (m_letterEdges[l3][3] > j2)  
		j2 = m_letterEdges[l3][3];
		
		for ( int j = j1; j <= j2; j++)
		{
            if (j < 0 || j >= m_height-1) continue;
			int ii1 = i1;
			int ii2 = i2;
            if (ii1 < 0 || ii1 >= m_width-1) continue;
            if (ii2 < 0 || ii2 >= m_width-1) continue;
			while (m_pDataIn[j*m_width+ii1] != TextColor && ii1 < m_width-1) ii1++;
			while (m_pDataIn[j*m_width+ii2] != TextColor && ii2 > 0) ii2--;
			for ( int i = ii1; i <= ii2; i++)
			{
                if (i < 0 || i >= m_width-1) continue;
				m_pWorkData3[j*m_width*3+i*3+1] = 100;
			}
		}
PrintDebugStr("iiiiiiiii");

/*		for ( int i = m_letterEdges[l3][0]; i <= m_letterEdges[l3][1]; i++)
		{
			int jj1 = j1;
			int jj2 = j2;
			while (m_pDataIn[jj1*m_width+i] != TextColor) jj1++;
			while (m_pDataIn[jj2*m_width+i] != TextColor) jj2--;
			for ( int j = jj1; j <= j2; j++)
			{
				m_pWorkData2[j*m_width*3+i*3+1] = 100;
			}
		}
*/

		
	}

}
void CStraightWithPolynom1::PreparePointsForPolynom(int * points)
{
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
			m_pWorkData1[j*m_width*3+i*3+0] = 0;
	}
	
	PrintDebugStr("asasasas");
	int heights[128];
	for ( int h = 0; h < 128; h++) heights[h] = 0;
	
	for ( int i = 0; i < m_width; i++)
	{
		int j1 = 0;
		while (m_pWorkData2[j1*m_width*3+i*3+1] != 100 && j1 < m_height-1) j1++;
		int j2 = m_height-1;
		while (m_pWorkData2[j2*m_width*3+i*3+1] != 100 && j2 > 0) j2--;
		if ( j2 == 0 && j1 == m_height-1) continue;
		m_pWorkData1[i] = j1;
		m_pWorkData1[i+m_width] = j2;
		if (j1 <= 4) continue;
		if (j2 >= m_height - 5) continue;

		int h = j2 - j1;
		heights[h]++;
	}
	PrintDebugStr("bvbvbvbvbvb");
	for ( int i = 0; i < m_width; i++)
	{
		int j1 = 0;
		while (m_pWorkData3[j1*m_width*3+i*3+0] != 100 && j1 < m_height-1) j1++;
		int j2 = m_height-1;
		while (m_pWorkData3[j2*m_width*3+i*3+0] != 100 && j2 > 0) j2--;
		if ( j2 == 0 && j1 == m_height-1) continue;
		m_pWorkData1[i+2*m_width] = j1;
		m_pWorkData1[i+3*m_width] = j2;

		if (j1 <= 4) continue;
		if (j2 >= m_height - 5) continue;
		int h = j2 - j1;
		heights[h]++;
	}
	PrintDebugStr("cxcxcxcxcxc");

	for ( int i = 0; i < m_width; i++)
	{
		int j1 = 0;
		while (m_pWorkData3[j1*m_width*3+i*3+1] != 100 && j1 < m_height-1) j1++;
		int j2 = m_height-1;
		while (m_pWorkData3[j2*m_width*3+i*3+1] != 100 && j2 > 0) j2--;
		if ( j2 == 0 && j1 == m_height-1) continue;
		m_pWorkData1[i+4*m_width] = j1;
		m_pWorkData1[i+5*m_width] = j2;

		if (j1 <= 4) continue;
		if (j2 >= m_height - 5) continue;
		int h = j2 - j1;
		heights[h]++;
	}
	PrintDebugStr("dssdsdsdsds");

	int max_h = 0;
	int count_max_h = 0;
	for ( int h = 13; h < 128; h++)
	{
		if ( heights[h] > count_max_h) 
		{
			count_max_h = heights[h];
			max_h = h;
		}
	}
	
	int low_h = max_h * 0.8;
	int high_h = max_h * 1.2;

	if (m_language == APP_ENGINE_LNG_CMC7)
	{
		low_h = max_h - 2;
		high_h = max_h + 2;
	}
	PrintDebugStr("ewewewewewe");


	for ( int i = 0; i < m_width; i++)
	{
		int h = m_pWorkData1[i+m_width] - m_pWorkData1[i];
		if ( h < low_h || h > high_h) continue;
		points[i] = m_pWorkData1[i];
	}
	PrintDebugStr("fdfdfdfdfdf");

	for ( int i = 0; i < m_width; i++)
	{
		int h = m_pWorkData1[i+5*m_width] - m_pWorkData1[i+4*m_width];
		if ( h < low_h || h > high_h) continue;
		points[i] = m_pWorkData1[i+4*m_width];
	}
	PrintDebugStr("gfgfgfgfgfgfg");

	for ( int i = 0; i < m_width; i++)
	{
		int h = m_pWorkData1[i+3*m_width] - m_pWorkData1[i+2*m_width];
		if ( h < low_h || h > high_h) continue;
		points[i] = m_pWorkData1[i+2*m_width];
	}
	PrintDebugStr("hghgghghghghgh");
	for ( int k = 0; k < 2; k++)
	{
		int l1 = m_numberLetters-1;
		while (m_letterEdges[l1][0] > m_end_x-300 && l1 > 4)
			l1--;
		if ( k == 1) 
		{
			l1 = 0;
			while (m_letterEdges[l1][0] < m_begin_x+300 && l1 < m_numberLetters - 4)
			l1++;
		}
		int i1 = m_letterEdges[l1][0];
		int i2 = m_letterEdges[l1][2];
		int j1 = m_letterEdges[l1][1];
		int j2 = m_letterEdges[l1][3];
		int h = j2 - j1;
		int l2 = l1-1;
		if ( k == 1)
			l2 = l1+1;
		int ii1 = m_letterEdges[l2][0];
		int ii2 = m_letterEdges[l2][2];
		int jj1 = m_letterEdges[l2][1];
		int jj2 = m_letterEdges[l2][3];
		int hh = jj2 - jj1;
		if ( hh < low_h || hh > high_h)
		{
			l2 = l1-2;
			ii1 = m_letterEdges[l2][0];
			ii2 = m_letterEdges[l2][2];
			jj1 = m_letterEdges[l2][1];
			jj2 = m_letterEdges[l2][3];
			hh = jj2 - jj1;
		}
		if ( h > high_h && hh >= low_h && hh <= high_h)
		{
			int diff1 = abs(j1-jj1);
			int diff2 = abs(j2 - jj2);
			if (diff2 < diff1)
			{
				for ( int ii = i1; ii < i2; ii++)
				{
                    if (ii < 0 || ii >= m_width-1) continue;
					if ( m_pDataIn[(j2-1)*m_width+ii] == TextColor)
					{
						points[ii] = j2 - m_aveBodyHeight;
					}
					if ( m_pDataIn[(j1+1)*m_width+ii] == TextColor)
					{
						points[ii] = j1 + h -  m_aveBodyHeight;;
					}
				
				}
			}
			else
			{
				for ( int ii = i1; ii < i2; ii++)
				{
                    if (ii < 0 || ii >= m_width-1) continue;
					if ( m_pDataIn[(j1+1)*m_width+ii] == TextColor)
					{
						points[ii] = j1;
					}
					if ( m_pDataIn[(j2-1)*m_width+ii] == TextColor)
					{
						points[ii] = j2 - h;
					}
				
				}
			}
		}
	}



}
void CStraightWithPolynom1::CalculateImageBoundaries()
{
	PrintDebugStr("CalculateImageBoundaries Begin" );
	m_begin_x = 0;
	m_end_x = m_width-1;
	m_begin_y = 0;
	m_end_y = m_height-1;
	bool found = false;
	for ( int i = 52; i < m_width-50; i++)
	{
		for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
		{
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				if (!found)
				{
					m_begin_x = i;
					found = true;
				}
				break;
			}
		}
		
		if (found) break;
	}
	for ( int i = m_width-20; i >= 51; i--)
	{
		bool found = false;
		for ( int j = _DOWN_EDGE_; j < _UP_EDGE_; j++)
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
				m_pWorkData2[j*newWidth*3+(i+300)*3+0] = m_pDataIn3[j*m_width*3+i*3+0];
				m_pWorkData2[j*newWidth*3+(i+300)*3+1] = m_pDataIn3[j*m_width*3+i*3+1];
				m_pWorkData2[j*newWidth*3+(i+300)*3+2] = m_pDataIn3[j*m_width*3+i*3+2];
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
				m_pDataIn3[j*m_width*3+i*3+0] = m_pWorkData2[j*m_width*3+i*3+0];
				m_pDataIn3[j*m_width*3+i*3+1] = m_pWorkData2[j*m_width*3+i*3+1];
				m_pDataIn3[j*m_width*3+i*3+2] = m_pWorkData2[j*m_width*3+i*3+2];
			}
		}
	
		/*FILE* fp = fopen("C://Temp//debug.bmp", "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataIn3,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}*/
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



	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		for ( int j = _DOWN_EDGE_; j <= _UP_EDGE_; j++)
		{
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				m_pWorkData3[m_width+i] = j;
				break;
			}
		}
		for ( int j = _UP_EDGE_; j >= _DOWN_EDGE_; j--)
		{
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				m_pWorkData3[i] = j;
				break;
			}
		}
		
	}
	for ( int i = m_begin_x+1; i< m_end_x; i++)
	{
		int numberChanges = 0;
		for ( int j = m_begin_y; j <= m_end_y; j++)
		{
			if (m_pDataIn[j*m_width+i-1] != m_pDataIn[j*m_width+i])
			{
				numberChanges++;
			}
		}
		m_pWorkData3[2*m_width+i] = numberChanges;
	}
    if (m_begin_x < 10) m_begin_x = 10;
    if (m_end_x > m_width-10) m_end_x = m_width - 10;
    if (m_begin_y < 3) m_begin_y = 3;
    if (m_end_y > m_height-3) m_end_y = m_height - 3;
}
void CStraightWithPolynom1::ProcessDown()
{
	int numberChangesAve = 0;
	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		int j1 = m_pWorkData3[i];
		if (j1 == 0) continue;
		int i1 = i;
		int i2 = i1+1;
		while ( true)
		{
			i2++;
			while ((m_pWorkData3[i2]  < j1) && (i2 < m_end_x))
			{
				i2++;
				int  iSSpaceBetweenWords = true;
				for ( int ii = i2; ii < i2 + 15; ii++)
				{
					int numberTextPixels = 0;
					for ( int jj = m_begin_y; jj < m_end_y; jj++)
					{
						if (m_pDataIn[jj*m_width+ii] == TextColor)
						{
							iSSpaceBetweenWords = false;
						}
					}
				}
				if (iSSpaceBetweenWords)
				{
					break;
				}

			}
			int numberChanges[_MAX_IMAGE_HEIGHT_];
			for ( int jj = 0; jj < _MAX_IMAGE_HEIGHT_; jj++)
			{
				numberChanges[jj] = 0;
			}
			int jMin = m_end_y;
			int jMax = m_begin_y;
			numberChangesAve = 0;
			for ( int ii = i1; ii < i2; ii++)
			{
				
				if (jMin > m_pWorkData3[m_width+ii] && m_pWorkData3[m_width+ii] > 0) 
					jMin = m_pWorkData3[m_width+ii];
				if (jMax < m_pWorkData3[ii]) 
					jMax = m_pWorkData3[ii];

				for ( int jj = m_begin_y; jj < m_end_y; jj++)
				{
					if (m_pDataIn[jj*m_width+ii-1] != m_pDataIn[jj*m_width+ii])
					{
						numberChanges[jj]++;
					}
					
				}
				
			}

			for ( int jj = m_begin_y; jj < m_end_y; jj++)
			{
						if (numberChanges[jj] > numberChangesAve )
						{
							numberChangesAve = numberChanges[jj];
						}
			}
			/*int numberRows = (jMax-jMin) / 3;

			for ( int jj = jMin; jj < jMax; jj++)
			{
				for ( int jj_=jj+1; jj_ < jMax; jj_++)
				{
					if (numberChanges[jj] < numberChanges[jj_])
					{
						int temp = numberChanges[jj_];
						numberChanges[jj_] = numberChanges[jj];
						numberChanges[jj] = temp;
					}
				}
			}

			numberChangesAve = 0;
			for ( int  k = numberRows/2; k < numberRows; k++)
			{
				numberChangesAve += numberChanges[jMin + k];
			}
			if (numberRows < 2) numberRows = 2;
			numberChangesAve /= numberRows/2;*/
			if (numberChangesAve <= 4 )
				break;
			i++;
			j1 = m_pWorkData3[i];
			while (j1 == 0 && i < m_end_x) 
			{
				j1 = m_pWorkData3[i];
				i++;
			}
			if ( i > m_end_x) break;
			i1 = i;
			i2= i1+1;
		}
		if (numberChangesAve <= 4 )
		{
			//m_pWorkData2[i1] = j1;
			for ( int ii = i1; ii <= i2; ii++)
			{
				m_pWorkData2[ii] = j1;
			}
		}
		i = i2-1;
	}

}

void  CStraightWithPolynom1::RemoveHorizontalLines()
{
	int begin_x = m_output_width-1;
	int end_x = 0;
	for (int i = 0; i < m_output_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			if (m_pDataOut[j*m_output_width*3+i*3+0] == TextColor || m_pDataOut[j*m_output_width*3+i*3+0] == 127)
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
		for ( int i = begin_x; i < m_output_width-500; i+=250)
		{
			int countText = 0;
			int countBG = 0;
			int maxCountText = 0;
			for ( int ii = i; ii < i + 500; ii++)
			{
				bool isTextPixel = false;
				for ( int jj = j - 10; jj < j; jj++)
				{
					if (m_pDataOut[jj*m_output_width*3+ii*3+0] == TextColor ||m_pDataOut[jj*m_output_width*3+ii*3+0] == 127)
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
						if (m_pDataOut[jj*m_output_width*3+ii*3+0] != m_pDataOut[jj*m_output_width*3+(ii+1)*3+0])
							numberMaavarimDown++;
					}
					for ( int jj = j-12 ; jj >= 0; jj--)
					{
						if (m_pDataOut[jj*m_output_width*3+ii*3+0] != m_pDataOut[jj*m_output_width*3+(ii+1)*3+0])
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
							if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] == 255 - TextColor && step == 1)
								break;
							if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] == TextColor || m_pDataOut[jj*m_output_width*3+(ii)*3+0]== 127)
							{
								step = 1;
								m_pDataOut[jj*m_output_width*3+(ii)*3+0] = 127;
							}
						}
					}
					else
					{
						for ( int jj = j-10 ; jj <= j; jj++)
						{
							if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] == 255 - TextColor && step == 1)
								break;
							if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] == TextColor || m_pDataOut[jj*m_output_width*3+(ii)*3+0] == 127)
							{
								step = 1;
								m_pDataOut[jj*m_output_width*3+(ii)*3+0] = 127;
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
				if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] == TextColor || m_pDataOut[jj*m_output_width*3+(ii)*3+0] == 127)
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
					if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] != m_pDataOut[jj*m_output_width*3+(ii+1)*3+0])
						numberMaavarimDown++;
				}
				for ( int jj = j-12 ; jj >= 0; jj--)
				{
					if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] != m_pDataOut[jj*m_output_width*3+(ii+1)*3+0])
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
						if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] == 255 - TextColor && step == 1)
							break;
						if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] == TextColor ||m_pDataOut[jj*m_output_width*3+(ii)*3+0] == 127)
						{
							step = 1;
							m_pDataOut[jj*m_output_width*3+(ii)*3+0] = 127;
						}
					}
				}
				else
				{
					for ( int jj = j-10 ; jj <= j; jj++)
					{
						if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] == 255 - TextColor && step == 1)
							break;
						if (m_pDataOut[jj*m_output_width*3+(ii)*3+0] == TextColor || m_pDataOut[jj*m_output_width*3+(ii)*3+0] == 127)
						{
							step = 1;
							m_pDataOut[jj*m_output_width*3+(ii)*3+0] = 127;
						}
					}
				}
			}
		}
	}
}


bool CStraightWithPolynom1::Calculate(char* filenamePolynom)
{
	if (m_doNothing) return 0;
	PrintDebugStr("10101");
memset(m_pWorkData1, 255-TextColor, (m_output_width + 600)*m_height*3*sizeof(byte));
	memset(m_pWorkData2 , 0, m_width*m_height*3*sizeof(int));

	PrintDebugStr("121212");

	memset(m_pWorkData3, 0, m_width*m_height*sizeof(int));
		PrintDebugStr("565656");

	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int ii = m_width-1 - i;
			m_pWorkData3[j*m_width+ii] = m_pDataIn[j*m_width+i];
		}
	}
	PrintDebugStr("343434");
memset(m_pDataIn, 0, m_width*m_height*sizeof(byte));
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int ii = m_width-1 - i;
			m_pDataIn[j*m_width+i] = m_pWorkData3[j*m_width+i];
		}
	}
	if (m_pDataIn3)
	{
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				int ii = m_width-1 - i;
				m_pWorkData3[j*m_width*3+ii*3+0] = m_pDataIn3[j*m_width*3+i*3+0];
				m_pWorkData3[j*m_width*3+ii*3+1] = m_pDataIn3[j*m_width*3+i*3+1];
				m_pWorkData3[j*m_width*3+ii*3+2] = m_pDataIn3[j*m_width*3+i*3+2];
			}
		}
		for ( int i = 0; i < m_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				m_pDataIn3[j*m_width*3+i*3+0] = m_pWorkData3[j*m_width*3+i*3+0];
				m_pDataIn3[j*m_width*3+i*3+1] = m_pWorkData3[j*m_width*3+i*3+1];
				m_pDataIn3[j*m_width*3+i*3+2] = m_pWorkData3[j*m_width*3+i*3+2];
			}
		}
		/*FILE* fp = fopen("C://Temp//debug2.bmp", "wb");
		if (fp)
		{
			WriteBMP(fp,m_pDataIn3,PIC24,m_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
		}*/

	}
	PrintDebugStr("44454545454545");
m_begin_x = m_width-1 - m_begin_x;
	m_end_x = m_width-1 - m_end_x;
PrintDebugStr("232323");

	CalculateImageBoundaries();
	
	//CalculateBody();



PrintDebugStr("aaaaaaaa 11111111");
	if (m_language == APP_ENGINE_LNG_CMC7)
	{
		CalculateLetterEdges_CMC7();
	}
	else if (m_isVertical)
	{
		CalculateLetterEdgesVerticalAsia();
	}
	else if (m_language == APP_ENGINE_LNG_KOREAN)
	{
		CalculateLetterEdgesVerticalAsia();
	}
	else
	{
		CalculateLetterEdges();
	}


		if (m_language == APP_ENGINE_LNG_KOREAN)
		{
			int heights[_MAX_NUMBER_LETTERS_];
			for ( int l = 0; l < m_numberLetters; l++)
			{
				heights[l] = m_letterEdges[l][3] - m_letterEdges[l][1];
			}
			for ( int h1 = 0; h1 < m_numberLetters-1; h1++)
			{
				for ( int h2 = h1+1; h2 < m_numberLetters; h2++)
				{
					if (heights[h1] < heights[h2])
					{
						int tmp = heights[h1];
						heights[h1] = heights[h2];
						heights[h2] = tmp;
					}
				}
			}
			m_aveBodyHeight = heights[0];
			if (m_numberLetters <= 5)
				m_aveBodyHeight = heights[2];
			
			if (m_numberLetters >= 5)
			{
				int l = 2;
				while (heights[l] > heights[2]-5 && heights[l] > 60 && l < m_numberLetters/2) 
					l++;
				m_aveBodyHeight = heights[l];
				
			}
		}
		else
		{
		int heights[_MAX_IMAGE_HEIGHT_];
		for ( int i = 0; i < _MAX_IMAGE_HEIGHT_; i++) heights[i] = 0;
		for ( int l = 0; l < m_numberLetters; l++)
		{
			int h = m_letterEdges[l][3] - m_letterEdges[l][1];
			heights[h]++;
		}

		int max_h3 = 0;
		int ave_h = 1;
		for ( int h = 10/*18*/; h < m_height-10; h++)
		{
			int h3 = heights[h-1]+heights[h]+heights[h+1];
			if ( h3 > max_h3)
			{
				max_h3 = h3;
				ave_h = h;
			}
		}
		m_aveBodyHeight = ave_h;
		}
		if (m_aveHeightDefault > 0)
		{
			m_aveBodyHeight = m_aveHeightDefault;
			m_isTable = true;
		}
		bool isShortScan = CheckIfShortScan();

		if (!isShortScan)
		{
			if (m_language == APP_ENGINE_LNG_KOREAN)
			{
				for ( int i = 0; i < 4*m_width; i++)
				{
					m_pWorkData4[i] = 0;
				}
				for ( int l = 0; l < m_numberLetters; l++)
				{
					int hh = m_letterEdges[l][3] - m_letterEdges[l][1];
					if ( hh < m_aveBodyHeight - 5 || hh > m_aveBodyHeight + 5 )
						continue;
					int j1 = m_letterEdges[l][1];
					//int ii1 = m_letterEdges[l][0] + (m_letterEdges[l][2] - m_letterEdges[l][0]) / 3;
					//int ii2 = m_letterEdges[l][0] + (m_letterEdges[l][2]- m_letterEdges[l][0]) *2/ 3;
					int i1 = m_letterEdges[l][0];
					int i2 = m_letterEdges[l][2];
					for ( int i = i1; i <= i2; i++)
					{
						if (m_pDataIn[j1*m_width + i] == TextColor)
						{
							m_pWorkData4[4*m_width+i] = j1;
						}
					}
				}
			}
			else
			{
				ConnectLetters();
				PreparePointsForPolynom(m_pWorkData4+4*m_width);
			}
		/*
			for ( int i = 0; i < m_width; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					m_pDataOut[j*m_output_width*3 + i*3+0]  = m_pDataIn[j*m_width+i];
					m_pDataOut[j*m_output_width*3 + i*3+1]  = m_pDataIn[j*m_width+i];
					m_pDataOut[j*m_output_width*3 + i*3+2]  = m_pDataIn[j*m_width+i];
				}
			}
			for ( int l = 0; l < _MAX_NUMBER_LETTERS_; l++)
			{
				for ( int i = m_letterEdges[l][0]; i < m_letterEdges[l][2];  i++)
				{
					for ( int j = m_letterEdges[l][1]; j < m_letterEdges[l][3];  j++)
					{
						m_pDataOut[j*m_output_width*3 + i*3+2] = 200;
					}
				}
			}
		*/
			for ( int i = 0; i < m_width; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					m_pWorkData2[j*m_width*3+i*3+0] = 0;
					m_pWorkData2[j*m_width*3+i*3+1] = 0;
					m_pWorkData2[j*m_width*3+i*3+2] = 0;

					m_pWorkData1[j*m_width*3+i*3+0] = 0;
					m_pWorkData1[j*m_width*3+i*3+1] = 0;
					m_pWorkData1[j*m_width*3+i*3+2] = 0;

					m_pWorkData3[j*m_width*3+i*3+0] = 0;
					m_pWorkData3[j*m_width*3+i*3+1] = 0;
					m_pWorkData3[j*m_width*3+i*3+2] = 0;

				}
			}
		
			CalculatePolinom(m_pWorkData4+4*m_width, m_pWorkData4+m_width);
			
		



			DrawOutput(m_pWorkData4+m_width, 1,   255, 0,0,0, m_height );
		///	DrawOutput(m_pWorkData2, 1, 0,  255, 0,0, m_height);
			//DrawOutput(m_pWorkData4+4*m_width, 1, 0,  255, 0, 0, m_height );
			for ( int i = 0; i < m_width; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					m_pDataOut[j*m_output_width*3 + i*3+2] = m_pDataIn[j*m_width+i];
				}
			}
			for ( int i = 0; i < m_width; i++)
			{
				if (m_pWorkData4[4*m_width+i] > 0)
				{
					int j = m_pWorkData4[4*m_width+i];
					

					m_pDataOut[j*m_output_width*3 + i*3+0] = 50;
					m_pDataOut[(j-1)*m_output_width*3 + i*3+0] = 50;
					m_pDataOut[(j-1)*m_output_width*3 + (i-1)*3+0] = 50;
					m_pDataOut[j*m_output_width*3 + (i-1)*3+0] = 50;

					m_pDataOut[j*m_output_width*3 + i*3+1] = 250;
					m_pDataOut[(j-1)*m_output_width*3 + i*3+1] = 250;
					m_pDataOut[(j-1)*m_output_width*3 + (i-1)*3+1] = 250;
					m_pDataOut[j*m_output_width*3 + (i-1)*3+1] = 250;

					/*m_pDataOut[j*m_output_width*3 + i*3+1] = 250;
					m_pDataOut[(j-1)*m_output_width*3 + i*3+1] = 250;
					m_pDataOut[(j-1)*m_output_width*3 + (i-1)*3+1] = 250;
					m_pDataOut[j*m_output_width*3 + (i-1)*3+1] = 250;
		*/

				}
			}
			
			
			/*FILE* fp = fopen(filenamePolynom, "wb");
			if (fp)
			{
				WriteBMP(fp,m_pDataOut,PIC24,m_output_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
				fclose(fp);
			}
			*/
			for ( int i = 0; i < m_width; i++)
			{
				m_pWorkData1[i] = 0;
				if (m_pWorkData4[i+m_width] >= 0 && m_pWorkData4[i+m_width] < m_height);
					m_pWorkData1[i] = (byte)m_pWorkData4[i+m_width];
			}
		}
		else
		{
			for ( int i = 0; i < m_width; i++)
			{
				m_pWorkData1[i] = 0;
			}
		}
	int j1 = 0;
	int j2 = 0;
	int i1 = m_begin_x;// + 300;
	
	j1 = m_pWorkData1[i1];
	while (j1 == 0 || j1 == 255)
	{
		i1++;
		if (i1 > m_width-1) break;
		j1 = m_pWorkData1[i1];
		
	}

	int i2 = i1+1;
	j2 = m_pWorkData1[i2];
	while (j2 == 0 || j2 == 255)
	{
		i2++;
		if (i2 > m_width-1) break;
		j2 = m_pWorkData1[i2];
	}


	/*int n = 0;
	_LOW_Y_ = 0;
	for ( int i = m_begin_x/*+300* /; i < m_end_x; i++)
	{
		if (m_pWorkData1[i] <= 0 ) continue;
		_LOW_Y_ += m_pWorkData1[i];
		n++;
	}
	if (!n) n= 1;
	_LOW_Y_ /= n;
	if (_LOW_Y_ <= 0) _LOW_Y_ = 0;
	_LOW_Y_ = 15;
	*/
	///if (isOldStraight)
	//	_LOW_Y_	= 100;
	bool isBegin = true;
	for ( int i = m_begin_x/*+300*/; i < m_end_x; i++)
	{
		if ( i > i2)
		{
			i1 = i2;
			j1 = j2;
			i2++;
			j2 = m_pWorkData1[i2];
			while (j2 == 0 || j2 == 255)
			{
				i2++;
				if (i2 > m_width - 10) break;
				j2 = m_pWorkData1[i2];
			}
			isBegin = false;

		}
		int delta = 0;
		if (isBegin)
		{
			if ( i >= i1 && i <= i2)
			{
				delta =  j1 + (int)((float)((i-i1)*(j2-j1))/(float)(i2-i1) + 0.5);
			}
			else
				delta =  j1;
			
		}
		else
		{
			if ( i >= i1 && i <= i2)
			{
				delta =  j1 + (int)((float)((i-i1)*(j2-j1))/(float)(i2-i1) + 0.5);
			}
		}
		m_pWorkData1[2*m_width+i] = delta;
		m_pWorkData1[3*m_width+i] = delta;

	}
	int min_y_delta = 10000;
	int max_y_delta = -10000;

	for ( int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			if (m_pDataIn[j*m_width+i] == TextColor)
			{
				int jj = j - m_pWorkData1[2*m_width+i];
				if (jj < min_y_delta) min_y_delta = jj;
				if (jj > max_y_delta) max_y_delta = jj;
				
			}
		}
	}
	
	_LOW_Y_ = m_height - (max_y_delta - min_y_delta) - 15;

	if (_LOW_Y_ < 10) _LOW_Y_ = 10;
	/*int count = 0;
	ave = 0;
	for ( int i = m_begin_x; i < m_begin_x + 50; i++)
	{
		ave += m_pWorkData1[2*m_width+i];
		count++;
	}

	for ( int i = m_begin_x+25; i < m_end_x; i++)
	{
		if (m_pWorkData1[2*m_width+i-25] != 0)
		{
			ave -= m_pWorkData1[2*m_width+i-25];
			count--;
		}
		if (m_pWorkData1[2*m_width+i+25] != 0)
		{
			ave += m_pWorkData1[2*m_width+i+25];
			count++;
		}
		if (!count) count = 1;
		m_pWorkData1[3*m_width+i] = ave / count;
		//m_pWorkData1[3*m_width+i] = m_pWorkData1[2*m_width+i];
	}

*/

    













	//_LOW_Y_ = 100;
	memset(m_pDataOut, 0, m_output_width*m_height*3*sizeof(byte));
	

	
	//if (_LOW_Y_ > 4*addHeight ) _LOW_Y_ -= 3*addHeight;
	//else if (_LOW_Y_  < m_height - 4*addHeight) _LOW_Y_ += addHeight;
	
	int numberPixels[128];
	for ( int j = 0; j < m_height; j++)
		numberPixels[j] = 0;
	for ( int i = m_begin_x+300; i < m_end_x-300; i++)
	{
		//int delta = m_pWorkData1[3*m_width+i];
		
		int delta = m_pWorkData1[2*m_width+i];
		//int delta = m_pWorkData2[m_width+i];
		for ( int j = 3; j < m_height; j++)
		{
			int newY = _LOW_Y_ + j -delta;
			//int newY = _LOW_Y_ + delta;
			if ( newY  <= 0 || newY >= m_height) continue;

			m_pDataOut[newY*m_output_width*3+i*3+0] = (m_pDataIn3) ? m_pDataIn3[j*m_width*3 + i*3+0]: m_pDataIn[j*m_width + i];
			m_pDataOut[newY*m_output_width*3+i*3+1] = (m_pDataIn3) ? m_pDataIn3[j*m_width*3 + i*3+1]: m_pDataIn[j*m_width + i];
			m_pDataOut[newY*m_output_width*3+i*3+2] = (m_pDataIn3) ? m_pDataIn3[j*m_width*3 + i*3+2]: m_pDataIn[j*m_width + i];
			if (m_pDataOut[newY*m_output_width*3+i*3+0] == TextColor)
				numberPixels[newY]++;
			/*m_pDataOut[newY*m_output_width*3+i*3+0] = m_pDataIn[j*m_width + i];
			m_pDataOut[newY*m_output_width*3+i*3+1] = m_pDataIn[j*m_width + i];
			m_pDataOut[newY*m_output_width*3+i*3+2] = m_pDataIn[j*m_width + i];
			*/
		}
	}
	//if (m_pDataIn3) return 1;

	int numberWhite = 0;
	int numberBlack = 0;
	for ( int i = 0; i < m_widthRaw; i++)
	{
		for ( int j = 3; j < m_heightRaw; j++)
		{
			if ( m_pDataRaw[j*m_widthRaw+i] == 0) 
				numberBlack++;
			else
				numberWhite++;
		}
	}
	bool isTextColorWhite = (numberBlack > numberWhite) ;
	//if (!isShortScan)
	{
	//for ( int i = m_begin_x+300; i < m_end_x-300; i++)
		int addHeight = AddHeightsToLetters();
		int jj1 = _LOW_Y_ - addHeight/2;// - addHeight / 3;
		int jj2 = jj1 + 7*addHeight/2 - addHeight/3;;
		
		if (m_language == APP_ENGINE_LNG_JAPANESE && m_isVertical)
		{
			addHeight = AddHeightsToLetters1();
			jj1 = _LOW_Y_ - addHeight;
			jj2 = jj1 + 4*addHeight;
		}
		if (m_language == APP_ENGINE_LNG_CMC7)
		{
			jj1 = m_height-1;
			jj2 = 0;
			for ( int j = 0; j < m_height; j++)
			{
				if (numberPixels[j] > 10)
				{
					if ( jj1 > j ) jj1 = j;
					if ( jj2 < j) jj2 = j;
				}
			}
		}
		if (isShortScan) 
		{
			jj1 = m_height;
			jj2 = 0;
			for ( int i = m_begin_x+300; i < m_end_x-300; i++)
			{
				for ( int j = 3; j < m_height; j++)
				{
					if (m_pDataOut[j*m_output_width*3+i*3+0] == TextColor)
					{
						if (j < jj1) jj1 = j;
						if (j > jj2) jj2 = j;
					}
				}
			}
		}
		else if (m_language == APP_ENGINE_LNG_KOREAN)
		{
			jj1 = _LOW_Y_ - addHeight/2;
			jj2 = jj1 + 7*addHeight/2;
			/*jj1 -= 5;
			jj2 += 5;*/
		}
		else if (m_language == APP_ENGINE_LNG_CHINESE_TRADITIONAL || 
			m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED )
		{
			jj1 -= 1;
			jj2 += 1;
		}
		if (m_pDataRaw)
		{
			for ( int i = m_begin_x+300; i < m_end_x-300; i++)
			{
				//int delta = m_pWorkData1[3*m_width+i];
		
				int delta = m_pWorkData1[2*m_width+i];
				//int delta = m_pWorkData2[m_width+i];
				for ( int j = 3; j < m_height; j++)
				{
					int newY = _LOW_Y_ + j -delta;
					//int newY = _LOW_Y_ + delta;
					if ( newY  <= 0 || newY >= m_height-1) continue;
					if (newY < jj1 || newY > jj2) continue;
					int w = m_widthRaw;
					if (m_widthRaw < 1600)
						w = m_widthRaw*2;
					if (m_widthRaw < 1000)
						w = m_widthRaw*3;

                    if ( i-300 - w > m_widthRaw-1) continue;
					int value = m_pDataRaw[j*m_widthRaw + m_widthRaw-(i-300 - w)];

					if (!isTextColorWhite)
						value = 255 - value;
					
					//if (value != TextColor) continue;
					m_pDataOut[newY*m_output_width*3+i*3+0] = value;
					m_pDataOut[newY*m_output_width*3+i*3+1] = value;
					m_pDataOut[newY*m_output_width*3+i*3+2] = value;
				}
			}
			
	        }
	 
	}
for ( int i = 0; i < m_begin_x+300; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_output_width*3 + i*3 +0] = 255 - TextColor;
			m_pDataOut[j*m_output_width*3 + i*3 +1] = 255 - TextColor;
			m_pDataOut[j*m_output_width*3 + i*3 +2] = 255 - TextColor;
		}
	}
	
	for ( int i = m_end_x; i < m_output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_output_width*3 + i*3 +0] = 255 - TextColor;
			m_pDataOut[j*m_output_width*3 + i*3 +1] = 255 - TextColor;
			m_pDataOut[j*m_output_width*3 + i*3 +2] = 255 - TextColor;
		}
	}




	
//	Clamping();

//if (!m_pDataIn3)
{
	memset(m_pWorkData3, 0, m_output_width*m_height*sizeof(byte));

	for ( int i =  m_begin_x+300; i < m_end_x-300; i++)
	//for ( int i = 0; i < m_output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int ii = m_output_width - i;
			m_pWorkData3[j*m_output_width*3+ii*3+0] = m_pDataOut[j*m_output_width*3+i*3+0];
			m_pWorkData3[j*m_output_width*3+ii*3+1] = m_pDataOut[j*m_output_width*3+i*3+1];
			m_pWorkData3[j*m_output_width*3+ii*3+2] = m_pDataOut[j*m_output_width*3+i*3+2];
		}
	}

	memset(m_pDataOut, 0, m_output_width*m_height*sizeof(byte)*3);
	for ( int i = 0; i < m_output_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			int ii = i;//m_output_width - i;
			m_pDataOut[j*m_output_width*3+i*3+0] = m_pWorkData3[j*m_output_width*3+i*3+0];
			m_pDataOut[j*m_output_width*3+i*3+1] = m_pWorkData3[j*m_output_width*3+i*3+1];
			m_pDataOut[j*m_output_width*3+i*3+2] = m_pWorkData3[j*m_output_width*3+i*3+2];
		}
	}
	//m_begin_x = m_width - m_begin_x;
	//m_end_x = m_width - m_end_x;

}


	RemoveGarbageAtBeginAndFinish();

	if (m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED ||
		m_language == APP_ENGINE_LNG_CHINESE_TRADITIONAL ||
		m_language == APP_ENGINE_LNG_KOREAN ||
		m_language == APP_ENGINE_LNG_JAPANESE)
	{
		RemoveHorizontalLines();
		for ( int i = 0; i < m_output_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (m_pDataOut[j*m_output_width*3+i*3+0] == 127)
				{
					m_pDataOut[j*m_output_width*3+i*3+0] = 255 - TextColor;
					m_pDataOut[j*m_output_width*3+i*3+1] = 255 - TextColor;
					m_pDataOut[j*m_output_width*3+i*3+2] = 255 - TextColor;
				//	m_pDataIn3[j*m_width*3+i*3+0] = 255 - TextColor;
				//	m_pDataIn3[j*m_width*3+i*3+1] = 255 - TextColor;
				//	m_pDataIn3[j*m_width*3+i*3+2] = 255 - TextColor;
				}
			}
		}
	}
	if (m_pDataIn3) return 1;


	//if (m_aveBodyHeight <= 25)
	{

		memcpy(m_pWorkData1, m_pDataOut, m_output_width*m_height*3*sizeof(byte));
		memset(m_pDataOut, 0, m_output_width*m_height*3*sizeof(byte));

		int new_begin_y = -1;
		for ( int j = 0; j < m_height; j++)
		{
			for ( int i = 0; i < m_output_width; i++)
			{
				if (IsText(m_pWorkData1[m_output_width*j*3+i*3+0], TextColor))
				{
					new_begin_y = j;
					break;
				}
			}
			if (new_begin_y > -1) break;
		}
		new_begin_y -= 5;
		if (new_begin_y <= 0) new_begin_y = 0;
		int new_end_y = -1;
		for ( int j = m_height-1; j >= 0; j--)
		{
			for ( int i = 0; i < m_output_width; i++)
			{
				if (IsText(m_pWorkData1[m_output_width*j*3+i*3+0], TextColor))
				{
					new_end_y = j;
					break;
				}
			}
			if (new_end_y > -1) break;
		}
		new_end_y += 5;
		if (new_end_y >= m_height)
			new_end_y = m_height - 1;
		int new_begin_x = -1;
		for ( int i = 0; i < m_output_width; i++)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (IsText(m_pWorkData1[m_output_width*j*3+i*3+0], TextColor))
				{
					new_begin_x = i;
					break;
				}
			}
			if (new_begin_x > -1) break;
		}

		int new_end_x = -1;
		for ( int i = m_output_width; i >= 0; i--)
		{
			for ( int j = 0; j < m_height; j++)
			{
				if (IsText(m_pWorkData1[m_output_width*j*3+i*3+0], TextColor))
				{
					new_end_x = i;
					break;
				}
			}
			if (new_end_x > -1) break;
		}
		FILE* df = NULL;//fopen("C://Temp/dbg.txt", "at");
		if (df)
		{
			fprintf(df, "m_aveBodyHeight = %d", m_aveBodyHeight);
		}
		float factor = 32.0/(float)m_aveBodyHeight;//6.0/4.0;
		if ( m_heightOfOutputBody > 0)
			factor = (float)m_heightOfOutputBody/(float)m_aveBodyHeight;//6.0/4.0;
		else
			factor = 1.0;
		if ((m_language ==APP_ENGINE_LNG_CHINESE_TRADITIONAL	||
			m_language == APP_ENGINE_LNG_CHINESE_SIMPLIFIED ||
			m_language == APP_ENGINE_LNG_JAPANESE ))// ||
			//m_language == APP_ENGINE_LNG_KOREAN) )
		{
			factor = (float)65.0/(float)m_aveBodyHeight;//6.0/4.0;
			if (factor > 2.0) factor = 2.0;
#ifdef HALF_SAMPLE_RATE_PLUS
			if (factor > 1.3) factor = 1.3;
#endif			
		}
		if (m_language == APP_ENGINE_LNG_KOREAN) 
		{
			factor = (float)43.0/(float)m_aveBodyHeight;//6.0/4.0;
			if (factor > 2.0) factor = 2.0;
		}
		if (df)
		{
			fprintf(df, "m_language = %d factor=%f\n", m_language, factor);
			fclose(df);
		}
		int *pOppositeIndex = new int[m_output_width*m_height*2]; 
		memset(pOppositeIndex, 0, m_output_width*m_height*2);
//if (_KOREAN_THICK_)
		//	factor = /*44.0*/(float) _KOREAN_THICK_HEIGHT_/(float)m_aveBodyHeight;
		int _LOW_Y_1 = m_height - new_end_y-5;//20;//(m_height - (new_end_y - new_begin_y)*factor)/2;
		if (_LOW_Y_1 < 0) _LOW_Y_1 = 0;
		if (_LOW_Y_1 > 20) _LOW_Y_1 = 20;
		if (new_begin_y < 0) return 1;
		int ii = 100;
		float f_jj, f_ii;
		int int_ii, int_jj;
		f_ii = factor;
		
		for ( int i = new_begin_x; i <= new_end_x; i++)
		{
			if ( i < 0) continue;
			if ( ii+factor+2 > m_output_width) continue;
			int jj = 0;
			int_ii = (int)(f_ii+0.5);
			f_ii -= (float)int_ii;

			f_jj =factor;
			for ( int j1 = new_begin_y; j1 <= new_end_y; j1++)
			{
				
				//if ( j*2 > m_height + _LOW_Y_1) continue;
				//int j = j1 - new_begin_y;
				int_jj = (int)(f_jj+0.5);
				f_jj -= (float) int_jj;
				if ((jj + factor+2 + _LOW_Y_1) >= m_height) continue;
				if ((jj  + _LOW_Y_1) < 0) continue;
				ii = i;
				if (( int_jj == 0) && m_pWorkData1[j1*m_output_width*3+ i*3 + 0] == TextColor)
				{
					m_pDataOut[(jj + 0 + _LOW_Y_1)*m_output_width*3 + (ii+0)*3+0] = 127;
					m_pDataOut[(jj + 0 + _LOW_Y_1)*m_output_width*3 + (ii+0)*3+1] = TextColor;
					m_pDataOut[(jj + 0 + _LOW_Y_1)*m_output_width*3 + (ii+0)*3+2] = TextColor;
				}
				pOppositeIndex[((jj + 0 + _LOW_Y_1)*m_output_width + (ii+0))*2+0] = i;
				pOppositeIndex[((jj + 0 + _LOW_Y_1)*m_output_width + (ii+0))*2+1] = j1;
				//for ( int ki = 0; ki < int_ii; ki++)
				{
					int ki = 0; 
					for ( int kj = 0; kj < int_jj; kj++)
					{
						int value =  m_pWorkData1[j1*m_output_width*3+ i*3 + 0];
						if (m_pDataOut[(jj +  kj + _LOW_Y_1)*m_output_width*3 + (ii+ki)*3+0] == 127) 
							value = TextColor;
						m_pDataOut[(jj + kj + _LOW_Y_1)*m_output_width*3 + (ii+ki)*3+0]  = value;
						m_pDataOut[(jj + kj + _LOW_Y_1)*m_output_width*3 + (ii+ki)*3+1]  = value;
						m_pDataOut[(jj + kj + _LOW_Y_1)*m_output_width*3 + (ii+ki)*3+2]  = value;
						pOppositeIndex[((jj + kj + _LOW_Y_1)*m_output_width + (ii+ki))*2+0] = i;
						pOppositeIndex[((jj + kj + _LOW_Y_1)*m_output_width + (ii+ki))*2+1] = j1;
						
					}
					
				}
				jj+=int_jj;
				f_jj += factor;
			}
			ii+=int_ii;
			f_ii += factor;
		}
		if (m_language == APP_ENGINE_LNG_HEBREW)
		{
			/*FILE* fff = fopen("D:\\DebugDina\\debug.bmp", "wb");
			if (fff)
			{
				WriteBMP(fff,m_pWorkData1,PIC24,m_output_width,m_height,NULL,NULL, NULL,8,F_FULLCOLOR);
				fclose(fff);
			}*/
			for ( int i = 0; i < m_output_width; i++)
			{
				for ( int j = 0; j < m_height; j++)
				{
					int iOpposite = pOppositeIndex[(j*m_output_width+i)*2+0];
					int jOpposite = pOppositeIndex[(j*m_output_width+i)*2+1];
					if ( jOpposite < 0 || jOpposite >= m_height) continue;
					if (iOpposite < 0 || iOpposite >= m_output_width) continue;

					if (m_pWorkData1[jOpposite*m_output_width*3+iOpposite*3+0] == 255 - TextColor)
					{
						m_pDataOut[j*m_output_width*3+i*3+0] = 255 - TextColor;
						m_pDataOut[j*m_output_width*3+i*3+1] = 255 - TextColor;
						m_pDataOut[j*m_output_width*3+i*3+2] = 255 - TextColor;
					}
				}
			}
		}
		if (pOppositeIndex)
		{
			delete [] pOppositeIndex;
			pOppositeIndex = NULL;
		}
		//PrintDebugStr("Calculate  end" );
	
		return 1;
	}


}
void CStraightWithPolynom1::RemoveGarbageAtBeginAndFinish()
{
	return;
	int spaces[300][2];
	int numberSpaces = 0;
	//int s1 = -1; 
	//int s2 = -1;
	bool isSpace = false;
	m_begin_x_1 = 0;
	m_end_x_1 = m_widthRaw-1;
	int i;
	for ( i = 6; i < m_widthRaw-6; i++)
	{
		bool columnSpace = true;
		for (int j = 0; j < m_height; j++)
		{
			if (m_pDataOut[j*m_output_width*3+i*3] == TextColor)
			{
				columnSpace = false;
				break;
			}
		}
		if (columnSpace &&  (!isSpace))
		{
			isSpace = true;
			//s1 = i;
			spaces[numberSpaces][0] = i;
		}
		else if (!columnSpace &&  (isSpace))
		{
			isSpace = false;
			//s2 = i;
			spaces[numberSpaces++][1] = i;// s2 - s1;
			if (numberSpaces >= 300)
				break;
		}
	}
	if (isSpace && i >= m_widthRaw-6)
			spaces[numberSpaces++][1] = m_widthRaw-6;// s2 - s1;
	if (numberSpaces <= 4) return;
	for ( int i = 0; i < numberSpaces-4; i++)
	{
		int widthSpaces = spaces[i][1] - spaces[i][0];
		int widthLetter1 = spaces[i+1][0] - spaces[i][1];
		int widthLetter2 = spaces[i+2][0] - spaces[i+1][1];
		int widthLetter3 = spaces[i+3][0] - spaces[i+2][1];
		if (widthSpaces > widthLetter1/2 && widthSpaces > widthLetter2/2 && widthSpaces > widthLetter3/2)
		{
			m_begin_x_1 = spaces[i][1] + 1;
			break;
		}
	}
	
	for ( int i = numberSpaces-1; i >= 4; i--)
	{
		int widthSpaces = spaces[i][1] - spaces[i][0];
		int widthLetter1 = spaces[i][0] - spaces[i-1][1];
		int widthLetter2 = spaces[i-1][0] - spaces[i-2][1];
		int widthLetter3 = spaces[i-2][0] - spaces[i-3][1];
		if (widthSpaces > widthLetter1/2 && widthSpaces > widthLetter2/2 && widthSpaces > widthLetter3/2)
		{
			m_end_x_1 = spaces[i][0] - 1;
			break;
		}
	}
	
	
	
	for ( int i = 0; i < m_begin_x_1; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_output_width*3+i*3+0] = 255 - TextColor;
			m_pDataOut[j*m_output_width*3+i*3+1] = 255 - TextColor;
			m_pDataOut[j*m_output_width*3+i*3+2] = 255 - TextColor;
		}
	}
	for ( int i = m_end_x_1; i < m_widthRaw; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_output_width*3+i*3+0] = 255 - TextColor;
			m_pDataOut[j*m_output_width*3+i*3+1] = 255 - TextColor;
			m_pDataOut[j*m_output_width*3+i*3+2] = 255 - TextColor;
		}
	}
	
}
int CStraightWithPolynom1::AddHeightsToLetters()
{
/*	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j< m_height; j++)
		{
			if ( m_pWorkData1[j*m_width*3 + i*3 + 1] != n) continue;
			
			m_pDataOut[j*m_width*3 + i*3 +0] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 +1] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn[j*m_width+i];
		}
	}
*/
	int heights_count[128];
	for ( int h = 0; h < 128; h++) heights_count[h] = 0;

	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		int letterHeight = m_letterEdges[l][3] - m_letterEdges[l][1];
		if (letterHeight > 5 && letterHeight < 127)
			heights_count[letterHeight] ++;
	}
	int max_h = 10;
	for ( int h = 10; h < 128; h++) 
	{
			if (heights_count[h] > heights_count[max_h])
			{
				max_h =h;
			}
	}

	int add_h = max_h / 2;
	return add_h;

}
int CStraightWithPolynom1::AddHeightsToLetters1()
{
/*	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j< m_height; j++)
		{
			if ( m_pWorkData1[j*m_width*3 + i*3 + 1] != n) continue;
			
			m_pDataOut[j*m_width*3 + i*3 +0] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 +1] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3 + i*3 +2] = m_pDataIn[j*m_width+i];
		}
	}
*/
	int heights_count[128];
	for ( int h = 0; h < 128; h++) heights_count[h] = 0;

	int max_h = 10;
	for ( int l = 0; l < m_numberLetters; l++)
	{
		if (m_letterEdges[l][0] == -1) continue;
		int letterHeight = m_letterEdges[l][3] - m_letterEdges[l][1];
		if (letterHeight > 5 && letterHeight < 50 && letterHeight > max_h)
		{
			max_h = letterHeight;
		}
	}
	/*int max_h = 10;
	for ( int h = 10; h < 128; h++) 
	{
			if (heights_count[h] > heights_count[max_h])
			{
				max_h =h;
			}
	}
	*/
	int add_h = max_h / 3;
	return add_h;

}














// calculate the cofactor of element (row,col)
int CStraightWithPolynom1::GetMinor(double **src, double **dest, int row, int col, int order)
{
    // indicate which col and row is being copied to dest
    int colCount=0,rowCount=0;

    for(int i = 0; i < order; i++ )
    {
        if( i != row )
        {
            colCount = 0;
            for(int j = 0; j < order; j++ )
            {
                // when j is not the element
                if( j != col )
                {
                    dest[rowCount][colCount] = src[i][j];
                    colCount++;
                }

            }
            rowCount++;
        }
    }

    return 1;
}

// Calculate the determinant recursively.
double CStraightWithPolynom1::CalcDeterminant( double **mat, int order)
{
    // order must be >= 0
    // stop the recursion when matrix is a single element
    if( order == 1 )
        return mat[0][0];

    // the determinant value

    double det = 0;

    // allocate the cofactor matrix
    double **minor;
    minor = new double*[order-1];
    for(int i=0;i<order-1;i++)
        minor[i] = new double[order-1];

    for(int i = 0; i < order; i++ )
    {
        // get minor of element (0,i)
        GetMinor( mat, minor, 0, i , order);
        // the recusion is here!

        det += (i%2==1?-1.0:1.0) * mat[0][i] * CalcDeterminant(minor,order-1);
        //det += pow( -1.0, i ) * mat[0][i] * CalcDeterminant( minor,order-1 );
    }

    // release memory
    for(int i=0;i<order-1;i++)
        delete [] minor[i];
    delete [] minor;
    return det;
}




void CStraightWithPolynom1::DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue, float maxValue)
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
		else if (valuesType ==3)
		{
			value = ((byte*)values)[i];
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
	//if (delta  > 0)
	//	show_ratio = (float)(m_height - 10) / delta;

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
		else if (valuesType ==3)
		{
			value = ((byte*)values)[i];
		}

		if (fabs(value)  > 1000)
		{
			continue;
		}
		
		int j = (int) (/*m_height - 1 - */show_ratio * (value - ratio_min)) ;
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
void CStraightWithPolynom1::CalculatePolinom(int * points, int * pointsOut)
{
int s  = 150;
int delta_x = 7;
int delta_y = 3;
int min_points = 6;
	int finish = m_begin_x+s/2;
	int prev_finish_x = 0;
	int prev_finish_value = 0;
	int prev_point_i = 0;
	int prev_point_value = 0;
	while (finish <= m_end_x-s/2)
	{
		int numberPoints = 0;
		int start = finish+1 ;
		finish = start + s;
		int ii = finish;
		int numberTextPixels = 1000;
		int firstPoint = 0;
		int lastPoint = 0;
		int firstPoint_x = -1;
		int lastPoint_x = -1;

	/*	if (points[prev_finish_x] == 0)
		{
			points[prev_finish_x] = prev_finish_value;

		}*/
		while (numberTextPixels > 2)
		{
			finish++;
			if ( finish > m_end_x-300) break;
			numberTextPixels = 0;
			for ( int j = 0; j < m_height; j++)
			{
				if (m_pDataIn[m_width*j+finish] == TextColor)
				{
					numberTextPixels++;
				}
			}

			if (points[finish] > 0)
			{
				if ( firstPoint == 0)
					firstPoint = points[finish];
				if (firstPoint_x == -1)
					firstPoint_x = finish;
				lastPoint_x = finish;
				lastPoint = points[finish];
				if (abs(finish - prev_point_i ) > delta_x ||
					abs(points[prev_point_i] - points[finish] > delta_y) )
				{
					numberPoints++;
					prev_point_i = finish;
				}
			}
		}

		int start1 = start - s/2;

		do 
		{
			start1--;
			if (points[start1] > 0)
			{
				firstPoint_x = start1;
				if (lastPoint_x == -1)
					lastPoint_x = start1;
				firstPoint = points[start1];
				if (abs(start1 - prev_point_i ) > delta_x ||
					abs(points[prev_point_i] - points[start1] > delta_y))
				{
					numberPoints++;
					prev_point_i = start1;
				}
			}
			if (start1 <= m_begin_x) 
			{
			///	points[start1] = firstPoint;			
				break;
			}
		}
		while (points[start1] == 0);
		start1--;

		int finish1 = finish + s/2;
		do
		{
			finish1++;
			if (points[finish1] > 0)
			{
				lastPoint = points[finish1];
				if ( firstPoint_x == -1)
					firstPoint_x = finish1;
				lastPoint_x = finish1;

				if (abs(finish1 - prev_point_i ) > delta_x  ||
					abs(points[prev_point_i] - points[finish1] > delta_y))
				{
					numberPoints++;
					prev_point_i = finish1;
				}
			}
			if (finish1 >= m_end_x) 
			{
				//points[finish1] = lastPoint;
				break;
			}
		}		
		while (points[finish1] == 0);

		finish1+=2;
		while (numberPoints < min_points )
		{
			if (start1 >= m_begin_x)
			{
				start1--;
				if (points[start1] > 0)
				{
					firstPoint = points[start1];
					firstPoint_x = start1;
					if (lastPoint_x == -1)
						lastPoint_x = start1;
					if (abs(start1 - prev_point_i ) > delta_x ||
					abs(points[prev_point_i] - points[start1] > delta_y))
					{
						numberPoints++;
						prev_point_i = start1;
					}
				}
			}
			if (finish1 <= m_end_x)
			{
				finish1++;
				if (points[finish1] > 0)
				{
					lastPoint_x = finish1;
					if (firstPoint_x == -1)
						firstPoint_x = finish1;
					if (abs(finish1- prev_point_i ) > delta_x ||
					abs(points[prev_point_i] - points[finish1] > delta_y))
					{
						numberPoints++;
						prev_point_i = finish1;
					}
				}
			}
			if (start1 < m_begin_x && finish1 > m_end_x)
				break;
		}
		start1 = firstPoint_x - 20;
		finish1 = lastPoint_x + 20;
		int start_ = start;
		int finish_ = finish;
		if (start_ < start1) start_ = start1;
		if (finish_ > finish1) finish_ = finish1;
		if ( finish_ < start_)
		{
			finish_ = finish1-20;
		}
	
		for ( int i = start1; i <= finish1; i++)
		{
			int ii = i - start1;
			m_pWorkData3[ii] = points[i];
		}

//		int size = finish1 - start1+1;		

		int size = finish1 - start1+1;		

		int prev_x = -1;
		for ( int x = 0; x <= size; x++)
		{
			if (m_pWorkData3[x] < 0.001)
			{
				continue;
			}
			if (prev_x > 0)
			{
				if (x - prev_x > s/2)
				{
					for ( int xx = prev_x; xx < x; xx+=s/2)
					{
						m_pWorkData3[xx] = m_pWorkData3[prev_x] + (m_pWorkData3[x] - m_pWorkData3[prev_x])*(xx-prev_x)/(x-prev_x);
					}
				}
			}
			prev_x = x;

		}


		MinLeastSquare(m_pWorkData3, m_pWorkData3+m_width, size);
		//if (start < 3960 && start > 3930 )
		{
			for ( int i = start_; i <= finish_; i++)
//			for ( int i = start1; i <= finish1; i++)
			{
				int ii = i - start1;
				pointsOut[i] = m_pWorkData3[ii+m_width];

			}
		}
		prev_finish_x = finish;
		ii = prev_finish_x - start1;
		prev_finish_value = m_pWorkData3[ii+m_width];

	}
	
	//MinLeastSquare(points, pointsOut, m_end_x);

}
void CStraightWithPolynom1::MinLeastSquare(int * points, int * pointsOut, int size)
{
#define N 5
	/*
	double S0 = 0;
	double S1 = 0;
	double S2 = 0;
	double S3 = 0;
	double S4 = 0;
	double S5 = 0;
	double S6 = 0;
	double S7 = 0;
	double S8 = 0;
	double S9 = 0;
	double S10 = 0;
	double S11 = 0;
	double V1 = 0;
	double V2 = 0;
	double V3 = 0;
	double V4 = 0;
	double V5 = 0;
	double V6 = 0;
	double V7 = 0;
*/
	double S[(N-1)*2+1];
	double V[N];
	for ( int i = 0; i < (N-1)*2+1; i++)
		S[i] = 0;
	for ( int i = 0; i < N; i++)
		V[i] = 0;
	for ( int x = 0; x < size; x++)
	{
		if ( points[x] <= 0) continue;
		double xx = (double)x / (double)(size/4);
		double yy = (double) points[x] / (double)(m_height*2);
		double xxx[(N-1)*2+1];
		xxx[0] = 1;
		for ( int k = 1; k < (N-1)*2+1; k++)
		{
			xxx[k] = xxx[k-1]*xx;
		}
		for ( int k = 0; k < (N-1)*2+1; k++)
		{
			S[k] += xxx[k];
		}

		double yyy[N];
		yyy[0] = yy;
		for ( int k = 1; k < N; k++)
		{
			yyy[k] = yyy[k-1]*xx;
		}
		for ( int k = 0; k < N; k++)
		{
			V[k] += yyy[k];
		}

	}

	/*V1 -= S6;
	V2 -= S7;
	V3 -= S8;
	V4 -= S9;
	V5 -= S10;
	V6 -= S11;
*/

	double coeff[N];
	double **XX = new double*[N];
	double **YY = new double*[N];
	for ( int i = 0; i < N; i++)
	{
		XX[i] = new double[N];
		YY[i] = new double[N];
	}
	double VV[N];

	for ( int i = 0; i < N; i++)
	{
		for ( int j = 0; j < N; j++)
		{
			int k = (N-1-i) + (N-1-j);
			YY[i][j] = S[k];
		}
		VV[i] = V[N -i - 1];
	}

/*	YY[0][0] = S10; YY[0][1] = S9; YY[0][2] = S8; YY[0][3] = S7; YY[0][4] = S6; YY[0][5] = S5; 
	YY[1][0] = S9; YY[1][1] = S8; YY[1][2] = S7; YY[1][3] = S6; YY[1][4] = S5; YY[1][5] = S4; 
	YY[2][0] = S8; YY[2][1] = S7; YY[2][2] = S6; YY[2][3] = S5; YY[2][4] = S4; YY[2][5] = S3; 
	YY[3][0] = S7; YY[3][1] = S6; YY[3][2] = S5; YY[3][3] = S4; YY[3][4] = S3; YY[3][5] = S2; 
	YY[4][0] = S6; YY[4][1] = S5; YY[4][2] = S4; YY[4][3] = S3; YY[4][4] = S2; YY[4][5] = S1; 
	YY[5][0] = S5; YY[5][1] = S4; YY[5][2] = S3; YY[5][3] = S2; YY[5][4] = S1; YY[5][5] = S0; 
	double VV[N] = { V6, V5, V4, V3, V2, V1};
*/	
	/*
	YY[0][0] = S6; YY[0][1] = S5; YY[0][2] = S4; YY[0][3] = S3; 
	YY[1][0] = 55; YY[1][1] = S4; YY[1][2] = S3; YY[1][3] = S2; 
	YY[2][0] = S4; YY[2][1] = S3; YY[2][2] = S2; YY[2][3] = S1; 
	YY[3][0] = S3; YY[3][1] = S2; YY[3][2] = S1; YY[3][3] = S0; 
	double VV[N] = { V4, V3,  V2, V1};
	*/
/*
	YY[0][0] = S4; YY[0][1] = S3; YY[0][2] = S2; 
	YY[1][0] = S3; YY[1][1] = S2; YY[1][2] = S1; 
	YY[2][0] = S2; YY[2][1] = S1; YY[2][2] = S0;
	double VV[N] = { V3, V2, V1};

/*	YY[0][0] = S2; YY[0][1] = S1; 
	YY[1][0] = S1; YY[1][1] = S0;
	double VV[N] = { V2, V1};
*/
	double YY_det = CalcDeterminant( (double**)YY, N);

	//double YY_det = YY[0][0]*YY[1][1] - YY[0][1]*YY[1][0];
	for ( int l = 0; l < N; l++)
	{
		for ( int i = 0; i < N; i++)
		{
			for ( int j = 0; j < N; j++)
			{
				XX[i][j] = YY[i][j];
			}
		}
//	XX[0][0] = V6; XX[1][0] = V5; XX[2][0] = V4; XX[3][0] = V3; XX[4][0] = V2; XX[5][0] = V1;
		for ( int k = 0; k < N; k++)
		{
			XX[k][l] = VV[k];
		}
		double det = CalcDeterminant( XX, N);
		//double det = XX[0][0]*XX[1][1] - XX[0][1]*XX[1][0];
		coeff[l] = det / YY_det;
	}




	for ( int x = 0; x < size; x++)
	{
		double xx = (double)x / (double)(size/4);
		double yy = (double) points[x] / (double)(m_height*2);
		double new_yy = 0;
		for ( int l = 0; l < N; l++)
			new_yy = new_yy*xx + coeff[l];

		new_yy *= (double)(m_height*2);
		pointsOut[x] = (int) (new_yy + 0.5);
	}						

	for ( int i = 0; i < N; i++)
	{
		delete [] XX[i];
		delete [] YY[i];
	}
	delete [] XX;
	delete [] YY;


}
void CStraightWithPolynom1::Clamping()
{
	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pWorkData1[j*m_width+i] = m_pDataOut[j*3*m_output_width+i*3 + 0];
			m_pDataOut[j*3*m_output_width+i*3 + 0] = 0;
			m_pDataOut[j*3*m_output_width+i*3 + 1] = 0;
			m_pDataOut[j*3*m_output_width+i*3 + 2] = 0;

		}
	}


	int histogram[128];
	for ( int j = 0; j < 128; j++)
	{
		histogram[j] = 0;
	}	
	for ( int j = 2; j < 126; j++)
	{
		for ( int i = m_begin_x; i < m_end_x; i++)
		{
			if ( m_pWorkData1[j*m_width+i] == TextColor)
				histogram[j]++;
		}
	}

	int begin_y = -1;
	int end_y = -1;

	for ( int j = 0; j < 128; j++)
	{
		if ( histogram[j] > 0)
		{
			if ( begin_y == -1)
				begin_y = j;
			end_y = j;
		}
	}


begin_y -= 5;
end_y += 5;
if ( begin_y < 0) begin_y = 0;
if ( end_y >= m_height) end_y = m_height-1;



	int upLine = 40;
/*	int prev_hist = 0;
	for ( int j = begin_y; j <= end_y; j++)
	{
		if ( ((histogram[j] - prev_hist) > prev_hist*3) &&
				(histogram[j] > 30 && prev_hist > 5))
		{
			upLine = j;
			break;
		}
		prev_hist = histogram[j];
	}

	int downLine = -1;
	prev_hist = 0;
	for ( int j = end_y; j >= begin_y; j--)
	{
		if ( ((histogram[j] - prev_hist) > prev_hist*3) &&
				(histogram[j] > 30 && prev_hist > 5))
		{
			downLine = j;
			break;
		}
		prev_hist = histogram[j];
	}
*/



	int letters[_MAX_NUMBER_LETTERS_][2];
	int upDown[_MAX_NUMBER_LETTERS_][2];
	int height[_MAX_NUMBER_LETTERS_];
	int mayMovedUpDown[_MAX_NUMBER_LETTERS_][2];
	for ( int iL = 0 ; iL < _MAX_NUMBER_LETTERS_; iL++)
	{
		letters[iL][0] = -1;
		letters[iL][1] = -1;
		upDown[iL][0] = -1;
		upDown[iL][1] = -1;
		m_clamping[iL] = 0;
		height[iL] = 0;
		mayMovedUpDown[iL][0] = 0;
		mayMovedUpDown[iL][1] = 0;
		
	}
	int iL_ = 1;
	for ( int i = m_begin_x; i < m_end_x; i++)
	{
		bool found = FindNextWord(i, letters[iL_][0], letters[iL_][1]);
		if ( !found) break;

		for ( int j = begin_y; j <= end_y; j++)
		{
			for ( int ii = letters[iL_][0]; ii <= letters[iL_][1]; ii++)
			{
				if ( m_pWorkData1[j*m_width+ii] == TextColor)
				{
					upDown[iL_][0] = j;
					break;
				}
			}
			if (upDown[iL_][0] != -1) break;
		}
		
		for ( int j = end_y; j >= begin_y; j--)
		{
			for ( int ii = letters[iL_][0]; ii <= letters[iL_][1]; ii++)
			{
				if ( m_pWorkData1[j*m_width+ii] == TextColor)
				{
					upDown[iL_][1] = j;
					break;
				}
			}
			if (upDown[iL_][1] != -1) break;
		}
		height[iL_] = upDown[iL_][1] - upDown[iL_][0]+1;
		i = letters[iL_][1];
		iL_++;
		if ( iL_ >= _MAX_NUMBER_LETTERS_-1) break;
	}
	int numberLetters = iL_;
	int height1[_MAX_NUMBER_LETTERS_];
	for ( int iL = 1; iL < numberLetters; iL++)
	{
		height1[iL] = height[iL];
	}
	for ( int iL = 1; iL < numberLetters; iL++)
	{
		for ( int iL_ = iL+1; iL_ < numberLetters; iL_++)
		{
			if (height1[iL] > height1[iL_])
			{
				int temp = height1[iL];
				height1[iL] = height1[iL_];
				height1[iL_] = temp;
			}
		}
	}
	int ave = 0;
	int n = 0;
	int firstHeight = -2;
	for ( int iL = numberLetters*2/9; iL < numberLetters/2; iL++)
	{
		if (height1[iL] <= 20) continue;

		/*if (height1[iL] < 18) continue;
		if (firstHeight == -2)
			firstHeight = -1;
		else if (firstHeight == -1)
			firstHeight = height1[iL];
		else if (height1[iL] - firstHeight > 2 ) break;
*/
		ave += height1[iL];
		n++;

	}
	if (!n) n = 1;
	ave /= n;
	int downLine = upLine + ave;

	
	int hClamping = downLine - upLine;

	int count = 10;
	for ( int iL = 1; iL < numberLetters; iL++)
	{

		if ( abs(height[iL] - hClamping)  <= 4)
		{
			mayMovedUpDown[iL][0] = 2;
			mayMovedUpDown[iL][1] = 2;
		}
	}
	for ( int iL = 1; iL < numberLetters; iL++)
	{
//		if ( abs(height[iL] - hClamping)  <= 4) continue;
		if ( mayMovedUpDown[iL][0] == 2 || mayMovedUpDown[iL][1] == 2) continue;

		int diff_Up = 0;
		int diff_Low = 0;
		for ( int ll = 2; ll <= 6;ll++ )
		{
			int iL_1;
			if ( ll % 2 == 0)
				iL_1 = iL + ll / 2;
			else
				iL_1 = iL - ll / 2;
			if (letters[iL_1][0] == -1 ) continue;


			if ( mayMovedUpDown[iL_1][0] == 2)
			{
				if (abs(upDown[iL][0] - upDown[iL_1][0]) <= 4 && 
					(abs(upDown[iL][0] - upDown[iL_1][0]) < abs(upDown[iL][1] - upDown[iL_1][1])))
				{
					mayMovedUpDown[iL][0] = 2;
					break;
				}
			}
			if ( mayMovedUpDown[iL_1][1] == 2)
			{
				if (abs(upDown[iL][1] - upDown[iL_1][1]) <= 4 && 
					(abs(upDown[iL][1] - upDown[iL_1][1]) < abs(upDown[iL][0] - upDown[iL_1][0])))

				{
					mayMovedUpDown[iL][1] = 2;
					break;
				}
			}
		}
	}



	for ( int iL = 1; iL < numberLetters; iL++)
	{
		if ( letters[iL][0] == -1 ) continue;
		 if (mayMovedUpDown[iL][1] == 2)
		{
			m_clamping[iL] = upDown[iL][1] - downLine;
		}
		else if (mayMovedUpDown[iL][0] == 2)
		{
			m_clamping[iL] = upDown[iL][0] - upLine;
		}
	}
	for ( int iL = 1; iL < numberLetters; iL++)
	{
		if (letters[iL][0] == -1 ) continue;
		if ( mayMovedUpDown[iL][0] == 2 || mayMovedUpDown[iL][1] == 2) continue;
		for ( int ll = 2; ll <= 6;ll++ )
		{
			int iL_1;
			if ( ll % 2 == 0)
				iL_1 = iL + ll / 2;
			else
				iL_1 = iL - ll / 2;
			if (letters[iL_1][0] == -1 ) continue;
			if (mayMovedUpDown[iL_1][0] == 2 )
			{
				m_clamping[iL] = m_clamping[iL_1];
				break;
			}
			else if (mayMovedUpDown[iL_1][1] == 2 )
			{
				m_clamping[iL] = m_clamping[iL_1];
				break;
			}
		}
		if ( mayMovedUpDown[iL][0] != 2 || mayMovedUpDown[iL][1] != 2) continue;
		{
			m_clamping[iL] = _LOW_Y_ - upLine;//upDown[iL][1] - downLine;
		}
	}
	for ( int iL = 3; iL <= numberLetters-3; iL++)
	{
		m_clamping[iL] = _LOW_Y_ - upLine;
	}
	
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_width*3 + i*3 + 0] = 0; 
			m_pDataOut[j*m_width*3 + i*3 + 1] = 0; 
			m_pDataOut[j*m_width*3 + i*3 + 2] = 0; 
		}
	}
	for ( int iL = 1; iL < numberLetters; iL++)
	{
		for ( int i = letters[iL][0]; i <= letters[iL][1]; i++)
		{
			if ( i <= m_begin_x + 300) continue;
			if ( i >= m_end_x - 300) break;
			for ( int j = 0; j < m_height; j++)
			{
				int jj_Out = j - m_clamping[iL];
				if ( jj_Out < 0 || jj_Out >= m_height) 
					continue;
				m_pDataOut[jj_Out*m_output_width*3 + i*3 + 0] = m_pWorkData1[j*m_width+i]; 
				m_pDataOut[jj_Out*m_output_width*3 + i*3 + 1] = m_pWorkData1[j*m_width+i]; 
				m_pDataOut[jj_Out*m_output_width*3 + i*3 + 2] = m_pWorkData1[j*m_width+i]; 

			}
		}

	}

}
bool CStraightWithPolynom1::CheckIfShortScan()
{
	int numberChangesInRaw[128];
	for ( int j = 4; j < 124; j++)
	{
		numberChangesInRaw[j] = 0;
		for ( int i = 0; i < m_width-2; i++)
		{
			if ( m_pDataIn[j*m_width+i] != m_pDataIn[j*m_width+i+1])
			{
				numberChangesInRaw[j]++;
			}
		}
	}
	for ( int k = 0; k < 127; k++)
	{
		for ( int k1 = k+1; k1 < 128;k1++)
		{
			if (numberChangesInRaw[k1] > numberChangesInRaw[k])
			{
				int temp = numberChangesInRaw[k1];
				numberChangesInRaw[k1] = numberChangesInRaw[k];
				numberChangesInRaw[k] = temp;
			}
		}

	}
 
	if ( numberChangesInRaw[5] > 40/*20*/) return false;
	else return true;
}
bool CStraightWithPolynom1::FindNextWord(int i, int& beginWord, int&endWord)
{
	bool beginFound = false;
	bool endFound = false;
	int numberSpaceColumns = 0;
	for ( ; i < m_end_x+5; i++)
	{
		int isSpaceColumn = 0;
		for ( int j = 0; j < m_height; j++)
		{
			if (m_pWorkData1[j*m_width+i] == TextColor)
			{
				isSpaceColumn++ ;
				
			}
		}
	
		if (isSpaceColumn <= 2)
		{
			numberSpaceColumns++; 
			if (numberSpaceColumns >= 1 && beginFound)
			{
				endWord = i;
				endFound = true;
				return true;;

			}
		}
		else
		{
			numberSpaceColumns = 0;
			if (!beginFound)
			{
				beginWord = i;
				beginFound = true;
			}
			continue;
		}
		
	}
	return false;
}
