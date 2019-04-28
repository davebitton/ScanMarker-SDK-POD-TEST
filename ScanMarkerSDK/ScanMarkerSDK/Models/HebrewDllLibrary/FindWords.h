#ifndef _FIND_WORDS_H_
#define _FIND_WORDS_H_

#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#endif
#include <stdio.h>
#include <math.h>
#include <ctype.h>


class FindWords
{
public:

	FindWords(int width, int height, void  *m_pData, int m_dataMode,int iTextColor,int start_y = 0, int finish_y = 0);
	~FindWords();

	int m_width;
	int m_height;
	int m_endWord;
	int m_beginWord;
	int m_beginText;

	void *m_pData;
	int m_dataMode;	
	int m_iTextColor;
	int m_h1;
	int m_h2;

	int m_start_y;
	int m_finish_y;
	
	int *m_verticalHistogram;
	int m_space;
	void FindVerticalBounds(int start, int finish, int &h1, int &h2);
	int FindTextBegin();
	void FindNextWord(int start, int finish);
	void FindBigSpaces(int start, int finish);

	void FindNextWordBySpaceSize(int start, int finish);
	void CalculateVerticalHistogram(int start, int finish,   int*  pDataInShifte = NULL);

	void FindNextWordBySpaces(int start, int finish, int nSpaces);
	

};

#endif