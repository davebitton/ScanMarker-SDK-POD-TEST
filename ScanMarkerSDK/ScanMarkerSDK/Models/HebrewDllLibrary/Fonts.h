#pragma once
#include <stdio.h>
#define _MAX_NUMBER_FONTS_ 100


typedef struct 
{
	bool structInUse;
	char font_name[100];
	float thickHorizontalLimits[2];
	float thickVerticalLimits[2];
	float widthLongHorizontalLimits[2];
	float widthBgHorizontalLimits[2];
	float ratioHorizontalToVerticalThicksLimits[2];
	float factorToApply;
} OneFont;

class CFonts
{
public:
	CFonts(void);
	~CFonts(void);
	void InitAllFonts();
	bool ReadAllFonts(char* language, int languageCode, char* fileDir, char * filename, bool isAppend = false);
	bool ReadOneFont(FILE* fff);
	void WriteErrors(char* str);
	void WriteAllFonts(char* fn);


	char m_strLog[1000];
	float m_thickHorizontalBetween[2];
	float m_thickLongHorizontalBetween[2];
	float m_widthBgHorizontalBetween[2];
	OneFont m_fonts[_MAX_NUMBER_FONTS_];
	float m_defaultFactorToApply;

	char m_currentLanguage[100];
	int m_currentLanguageCode;
	char m_fontsDirname[1000];
	char m_fontsFilename[1000];
	char m_fontErrorsFile[1000];
	



	


};
