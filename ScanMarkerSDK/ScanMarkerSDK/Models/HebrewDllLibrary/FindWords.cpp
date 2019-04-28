#include "StdAfx.h"
#include <stdio.h>
#include "InOut.h"

#include <string.h>

#define NUMBER_WORDS_IN_STEP 1
#define NUMBER_WORDS_IN_CALC 0
#define NUMBER_WORDS_IN_ITALIC 1
#define NUMBER_IN_MAX 2
#include "FindWords.h"
//#define TextColor 255

#define White 255
#define Black 0

FindWords::FindWords(int width, int height, void  *pData, int dataMode,int iTextColor,int start_y, int finish_y)
{
	m_width = width;
	m_height = height;
	m_start_y = start_y;
	m_finish_y = finish_y;
	if (m_finish_y == 0)
		m_finish_y = m_height;
	
	m_endWord = 61;
	m_beginWord = 61;
	m_pData = pData;
	m_dataMode = dataMode;
	m_iTextColor = iTextColor;
	m_beginText = 0;
	m_verticalHistogram = NULL;
	if (m_verticalHistogram )
	{
		delete[] m_verticalHistogram ;
		m_verticalHistogram = NULL;
	}
	m_verticalHistogram = new int[m_width];
	memset(m_verticalHistogram, 0, m_width*sizeof(int));
	m_spaceLeft = 60;
}
FindWords::~FindWords()
{
	if (m_verticalHistogram )
	{
		delete[] m_verticalHistogram ;
		m_verticalHistogram = NULL;
	}
	m_width = 0;
	m_width = 0;
	
}
int FindWords::FindTextBegin()
{
	m_beginText = m_width;
	bool found = false;
	for ( int i = m_spaceLeft; i < m_width; i++)
	{
		for ( int j = m_spaceUp; j < m_height-m_spaceBottom; j++)
		{
			int value = GetDataInValue(m_pData, m_dataMode,  i, j, m_width,  m_height);
			if (IsText(value, m_iTextColor))
			{
				m_beginText = i;								
				found = true;
				break;
			}
		}
		if (found)
		{
			break;
		}
	}
	return m_beginText;
			
}

void FindWords::FindNextWord(int start, int finish)
{
	if (finish >= m_width) finish = m_width - 1;
	m_beginWord = start;//m_endWord;
	if (m_beginWord < 5) m_beginWord = 5;
	while ((m_verticalHistogram[m_beginWord] < 1) && 
		(m_verticalHistogram[m_beginWord-1] < 1) && 
		(m_verticalHistogram[m_beginWord-2] < 1) && 
		(m_verticalHistogram[m_beginWord-3] < 1) && 
		(m_verticalHistogram[m_beginWord-4] < 1) && 
		(m_verticalHistogram[m_beginWord-5] < 1) &&
		(m_verticalHistogram[m_beginWord-6] < 1) && 
		(m_verticalHistogram[m_beginWord-7] < 1) && 
		(m_verticalHistogram[m_beginWord-8] < 1))/* &&
		(m_verticalHistogram[m_beginWord-9] < 1) && 
		(m_verticalHistogram[m_beginWord-10] < 1) && 
		(m_verticalHistogram[m_beginWord-11] < 1) &&
		(m_verticalHistogram[m_beginWord-12] < 1) && 
		(m_verticalHistogram[m_beginWord-13] < 1))*/
	{

		if ( m_beginWord >= finish) //m_width -1)
		{
			break;
		}			
		m_beginWord++;
	}
	m_beginWord--;
	m_endWord = m_beginWord+1;

	while ((m_verticalHistogram[m_endWord] >= 1) || 
		(m_verticalHistogram[m_endWord-1] >= 1) || 
		(m_verticalHistogram[m_endWord-2] >= 1) || 
		(m_verticalHistogram[m_endWord-3] >= 1) || 
		(m_verticalHistogram[m_endWord-4] >= 1) || 
		(m_verticalHistogram[m_endWord-5] >= 1) || 
		(m_verticalHistogram[m_endWord-6] >= 1) || 
		(m_verticalHistogram[m_endWord-7] >= 1) || 
		(m_verticalHistogram[m_endWord-8] >= 1))/* || 
		(m_verticalHistogram[m_endWord-9] >= 1) || 
		(m_verticalHistogram[m_endWord-10] >= 1) || 
		(m_verticalHistogram[m_endWord-11] >= 1) || 
		(m_verticalHistogram[m_endWord-12] >= 1) || 
		(m_verticalHistogram[m_endWord-13] >= 1))*/
	{
		if ( m_endWord >= finish) //m_width -1)
		{
			break;
		}			
		m_endWord++;
	}
		

	if (m_pData == NULL) return;

	bool found = false;
	m_h1 = -1;
	m_h2 = -1;
	for ( int j = 0; j < m_height; j++)
	{
		for ( int i = m_beginWord; i < m_endWord; i++)
		{ 
			int value = GetDataInValue(m_pData, m_dataMode,  i, j, m_width,  m_height);
			if (IsText(value, m_iTextColor))
			{
				if (m_h1 == -1 || j < m_h1)
					m_h1 = j;
				if (m_h2 == -1 || j > m_h2)
					m_h2 = j;
				
			}
		}
	}
}


void FindWords::FindBigSpaces(int start, int finish)
{
	int spaces[200];
	for ( int s = 0; s < 200; s++) spaces[s] = 0;

	int iSpace = 0;

	int i = start;
	int nSpaces = 0;
	int minSpace = 200;
	int maxSpace = 0;
	while(m_verticalHistogram[i] > 0 && i < finish) i++;
	while ( i < finish )
	{
		while(m_verticalHistogram[i] > 0 && i < finish) i++;
		if ( i >= finish) break;
		
		int beginSpace = i;
		
		while(m_verticalHistogram[i] == 0 && i < finish) i++;
		if ( i >= finish) break;

		int length = i - beginSpace;;
		if (length < 200 && length > 0)
		{
			if (length > 2)
				nSpaces++;
			spaces[length]++;
		//	if (length < minSpace) minSpace = length;
		//	if (length > maxSpace) maxSpace = length;
		}
	}
	int c = 0;
	int last_c = 0;
	for (int length = 3; length < 200; length++)
	{
		if (length - last_c > 10) 
		{
			c = 0;
			last_c = 0;
		}
		c += spaces[length];
		if (spaces[length] > 0) last_c = length;
		if (spaces[length] == 0) continue;
		if (c < 3) continue;
		c = 0;

			if (length < minSpace) minSpace = length;
			if (length > maxSpace) maxSpace = length;
		
	}
	/*for (int length = 1; length < 200; length++)
	{
		if (spaces[length] == 0) continue;
			if (length < minSpace) minSpace = length;
			if (length > maxSpace) maxSpace = length;
		
	}*//*if (maxSpace / minSpace < 2.5)
	{
		int dataSize = finish - start;
		if (!nSpaces) nSpaces = 1;
		int aveLengthWord = dataSize / nSpaces;
		if (aveLengthWord > 60)
		{
			m_space = maxSpace/2;
		}
		else
			m_space = 2*maxSpace;
		return;
		
	}*/
	int spaces1[3] = {maxSpace*3/4, maxSpace/4, maxSpace*2/4};
	int nWords[3] = {0,0,0};
	int lengthWords[3] = {0,0,0};
	int maxLengthWords[3] = {0,0,0};
	int minLengthWords[3] = {10000,10000, 10000};
	for ( int k = 0; k < 3; k++)
	{
		int i = start;
		int b_w = i;
		int nS = 0;
		int e_w = i;
		while ( i < finish )
		{
			nS= 0;
			while(m_verticalHistogram[i] == 0 && i < finish) 
			{
				nS++;
				i++;
			}
			//if ( i >= finish) break;
			if ( nS >= spaces1[k])
			{
				
				int lengthWord = e_w - b_w;
				lengthWords[k] += lengthWord;
				nWords[k]++;
				if (minLengthWords[k] > lengthWord) minLengthWords[k] = lengthWord;
				if (maxLengthWords[k] < lengthWord) maxLengthWords[k] = lengthWord;
				b_w = i;
				
			}
			
			while(m_verticalHistogram[i] > 0 && i < finish) i++;
			e_w = i;
		}
		if (!nWords[k]) nWords[k] = 1;
		lengthWords[k] /= nWords[k];
	}
	bool spaceFound = false;
	for ( int k = 0; k < 3; k++)
	{
		if (maxLengthWords[k] > 700) continue;
		if (lengthWords[k] > 400) continue;
		if (nWords[k] > 20) continue;
		
		spaceFound = true;
		m_space = spaces1[k];

	}


/*	int maxSpaces[5];
	bool existWords = true;
	for ( int s = 1; s <= maxSpace; s++)
	{
		if (spaces[s] == 0)
		{
			existWords = true;
			for ( int ds = 0; ds < minSpace * 1.5; ds++)
			{
				s++;
				if  (s >= maxSpace) 
				{
					existWords = false;
					break;
				}
				if (spaces[s]!= 0)
				{
					existWords = false;
					break;
				}
			}
			if ( existWords) break;
		}
	}
*/
			

	if ( !spaceFound) 
	{
		/*
		int dataSize = finish - start;
		int aveLengthWord = dataSize / nSpaces;
		if (aveLengthWord > 60)
		{
			m_space = maxSpace/2;
		}
		else
			m_space = maxSpace*0.9;
		return;
		*/
		m_space = maxSpace/2;
	}
	//m_space = minSpace;
	//if (minSpace < 2) minSpace = 2;
	if (m_space < 15) m_space = 15;
	//m_space = maxSpace / 2.0;
}
void FindWords::FindNextWordBySpaceSize(int start, int finish)
{
	if (finish >= m_width) finish = m_width - 1;
	m_beginWord = start;//m_endWord;
	if (m_beginWord < m_space) m_beginWord = m_space;

	bool isBeginWord = false;
	while (!isBeginWord)
	{
		int i = m_beginWord;
		while ( i <= m_beginWord + m_space)
		{
			if ( i >= m_width)
			{
				m_beginWord  =-1;
				break;
			}
			if ( i < 0)
			{
				i++;
				continue;
			}
			if (m_verticalHistogram[i] >= 1)
			{
				isBeginWord = true;
				break;
			}
			i++;
		}
		if ( m_beginWord == -1)
		{
			break;
		}
		if (isBeginWord) 
			break;
		if ( m_beginWord >= finish) //m_width -1)
		{
			break;
		}			
		m_beginWord++;
	}
	//m_beginWord+=m_space-1;
	if (!isBeginWord || m_beginWord == -1)
	{
		m_beginWord = -1;
		m_endWord = -1;
		return;
	}
	m_beginWord--;
	m_endWord = m_beginWord+1;


	bool isEndWord = false;
	while (!isEndWord)
	{
		int i = m_endWord;
		isEndWord = true;
		while ( i <= m_endWord + m_space )
		{
			if ( i >= m_width)
			{
				m_endWord = m_width-1;
				break;
			}
			if ( i < 0)
			{
				i++;
				continue;
			}
			if (m_verticalHistogram[i] >= 1)
			{
				isEndWord = false;
				break;
			}
			i++;
			if ( i >= finish) break;
		}
		if (isEndWord)
			break;

		m_endWord++;
		if ( m_endWord >= finish) //m_width -1)
		{
			break;
		}			

	}

	if (m_pData == NULL) return;

	bool found = false;
	m_h1 = -1;
	m_h2 = -1;
	for ( int j = 0; j < m_height; j++)
	{
		for ( int i = m_beginWord; i < m_endWord; i++)
		{ 
			int value = GetDataInValue(m_pData, m_dataMode,  i, j, m_width,  m_height);
			if (IsText(value, m_iTextColor))
			{
				if (m_h1 == -1 || j < m_h1)
					m_h1 = j;
				if (m_h2 == -1 || j > m_h2)
					m_h2 = j;
				
			}
		}
	}
}
void FindWords::FindNextWordBySpaces(int start, int finish, int nSpaces)
{
	if (finish >= m_width) finish = m_width - 1;
	m_beginWord = start;//m_endWord;
	if (m_beginWord < nSpaces) m_beginWord = nSpaces;

	
	while (true)
	{

		bool isBreak = false;
		for ( int i = 0; i < nSpaces; i++)
		{
			if (m_verticalHistogram[m_beginWord-i] >=1)
			{
				isBreak = true;
				break;
			}
		}

		if (isBreak) break;

		if ( m_beginWord >= finish) //m_width -1)
		{
			break;
		}			
		m_beginWord++;
	}
	m_beginWord--;
	m_endWord = m_beginWord+1;

	while (true)
	{

		bool isBreak = true;
		for ( int i = 0; i < nSpaces; i++)
		{
			if (m_verticalHistogram[m_endWord-i] >= 1)
			{
				isBreak = false;
				break;
			}
		}

		if (isBreak) break;

		if ( m_endWord >= finish) //m_width -1)
		{
			break;
		}			
		m_endWord++;
	}
		

	if (m_pData == NULL) return;

	bool found = false;
	m_h1 = -1;
	m_h2 = -1;
	for ( int j = 0; j < m_height; j++)
	{
		for ( int i = m_beginWord; i < m_endWord; i++)
		{ 
			int value = GetDataInValue(m_pData, m_dataMode,  i, j, m_width,  m_height);
			if (IsText(value, m_iTextColor))
			{
				if (m_h1 == -1)
					m_h1 = j;
				m_h2 = j;
				
			}
		}
	}
}
void FindWords::FindVerticalBounds(int start, int finish, int &h1, int &h2)
{
	h1 = -1;
	h2 = -1;
	for ( int j = m_start_y; j < m_finish_y; j++)
	{
		for ( int i = start; i < finish; i++)
		{
			int value = GetDataInValue(m_pData, m_dataMode,  i, j, m_width,  m_height);
			if (IsText(value, m_iTextColor))
			{
				h1 = j;
				break;
			}
		}
		if (h1 != -1) 
			break;
	}

	for ( int j = m_finish_y-1; j >= m_start_y; j--)
	{
		for ( int i = start; i < finish; i++)
		{
			int value = GetDataInValue(m_pData, m_dataMode,  i, j, m_width,  m_height);
			if (IsText(value, m_iTextColor))
			{
				h2 = j;
				break;
			}
		}
		if (h2 != -1) 
			break;
	}


}	
void FindWords::CalculateVerticalHistogram(int start, int finish,  int*  pDataInShifte)
{
	if (start < 0) start = 0;
	if (finish >= m_width) finish = m_width - 1; 		
	memset(m_verticalHistogram, 0, m_width*sizeof(int));
	for ( int i = start; i < finish; i++)
	{
		
		int numberTextPixels = 0;
		for ( int j = m_start_y; j < m_finish_y; j++)
		{

			int value = GetDataInValue(m_pData, m_dataMode,  i, j, m_width,  m_height, pDataInShifte);
			if (IsText(value, m_iTextColor))
				numberTextPixels++;
		}
		m_verticalHistogram[i] = numberTextPixels;
	}

}