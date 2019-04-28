#include "string.h"
#include "Fonts.h"
#ifdef _SCANMARKER_WINDOWS_
#include <shlobj.h>
#else
#endif

CFonts::CFonts(void)
{
	InitAllFonts();
}

CFonts::~CFonts(void)
{
	InitAllFonts();
}
void CFonts::InitAllFonts()
{
	m_currentLanguage[0] = 0;
	m_currentLanguageCode = -1;
	m_fontsDirname[0] = 0;
	m_fontsFilename[0] = 0;
	m_thickHorizontalBetween[0] = 0;
	m_thickHorizontalBetween[1] = 0;
	m_thickLongHorizontalBetween[0] = 0;
	m_thickLongHorizontalBetween[1] = 0;
	m_widthBgHorizontalBetween[0] = 0;
	m_widthBgHorizontalBetween[1] = 0;

	for ( int i = 0; i < _MAX_NUMBER_FONTS_; i++)
	{
		m_fonts[i].structInUse = false;
		m_fonts[i].font_name[0] = 0;

		m_fonts[i].thickHorizontalLimits[0] = 0;
		m_fonts[i].thickHorizontalLimits[1] = 0;
		
		m_fonts[i].thickVerticalLimits[0] = 0;
		m_fonts[i].thickVerticalLimits[1] = 0;
		
		m_fonts[i].widthLongHorizontalLimits[0] = 0;
		m_fonts[i].widthLongHorizontalLimits[1] = 0;
		
		m_fonts[i].widthBgHorizontalLimits[0] = 0;
		m_fonts[i].widthBgHorizontalLimits[1] = 0;

		m_fonts[i].ratioHorizontalToVerticalThicksLimits[0] = 0;
		m_fonts[i].ratioHorizontalToVerticalThicksLimits[1] = 0;

		m_fonts[i].factorToApply = 1.0;
	}
	m_defaultFactorToApply = 1.0;
}

void CFonts::WriteErrors(char* str)
{
	return;
	FILE * fff = fopen(m_fontErrorsFile, "at");
	if (fff)
	{
		fprintf(fff, "%s\n", str);
		fclose(fff);
	}
}

bool CFonts::ReadOneFont(FILE* fff)
{
	char lastFontName[100] = "";
	OneFont * font = NULL;
	for ( int i = 0; i < _MAX_NUMBER_FONTS_; i++)
	{
		if (!m_fonts[i].structInUse )
		{
			font = &m_fonts[i];
			font->structInUse = true;
			break;
		}
	}
	char str[1000];
	int thickHorizontalLimitsNumberDef[2] = {0, 0};
	int thickVerticalLimitsNumberDef[2] = {0, 0};
	int widthLongHorizontalLimitsNumberDef[2] = {0, 0};
	int widthBgHorizontalLimitsNumberDef[2] = {0, 0};
	int ratioHorizontalToVerticalThicksLimitsNumberDef[2] = {0, 0};
	int factorToApplyNumberDef = 0;
	int font_nameNumberDef = 0;
	
	bool endFontFound = false;
	while (fgets(str, 999, fff))
	{
		if (str[0] == '#') continue;
		//WriteErrors(str);
		if ( font == NULL)
		{

			sprintf(m_strLog, "CFonts:ERROR All font struct area in use");
			WriteErrors(m_strLog);
			return false;
		}
		if (strstr(str, "</FONT_DEFINITION>"))
		{
			endFontFound = true;
			break;
		}
		
		if (strstr(str, "_FONT_NAME_"))
		{
			strncpy(font->font_name , strstr(str, "_FONT_NAME_ ")+strlen("_FONT_NAME_ "), 99);
			int len = strlen(font->font_name);//,"test");
			font->font_name[len-1]=0;
			font_nameNumberDef++;
			strncpy(lastFontName, font->font_name, 99);
		}
		if (strstr(str, "_THICK_HORIZONTAL_LIMITS_LOW_"))
		{
			sscanf(strstr(str, "_THICK_HORIZONTAL_LIMITS_LOW_") + strlen("_THICK_HORIZONTAL_LIMITS_LOW_"), "%f", &font->thickHorizontalLimits[0]);
			thickHorizontalLimitsNumberDef[0]++;
		}
		if (strstr(str, "_THICK_HORIZONTAL_LIMITS_HIGH_"))
		{
			sscanf(strstr(str, "_THICK_HORIZONTAL_LIMITS_HIGH_") + strlen("_THICK_HORIZONTAL_LIMITS_HIGH_"), "%f", &font->thickHorizontalLimits[1]);
			thickHorizontalLimitsNumberDef[1]++;
		}
		if (strstr(str, "_WIDTH_LONG_HORIZONTAL_LIMITS_LOW_"))
		{
			sscanf(strstr(str, "_WIDTH_LONG_HORIZONTAL_LIMITS_LOW_") + strlen("_WIDTH_LONG_HORIZONTAL_LIMITS_LOW_"), "%f", &font->widthLongHorizontalLimits[0]);
			widthLongHorizontalLimitsNumberDef[0]++;
		}
		if (strstr(str, "_WIDTH_LONG_HORIZONTAL_LIMITS_HIGH_"))
		{
			sscanf(strstr(str, "_WIDTH_LONG_HORIZONTAL_LIMITS_HIGH_") + strlen("_WIDTH_LONG_HORIZONTAL_LIMITS_HIGH_"), "%f", &font->widthLongHorizontalLimits[1]);
			widthLongHorizontalLimitsNumberDef[1]++;
		}
		
		if (strstr(str, "_WIDTH_BACKGROUND_HORIZONTAL_LIMITS_LOW_"))
		{
			sscanf(strstr(str, "_WIDTH_BACKGROUND_HORIZONTAL_LIMITS_LOW_") + strlen("_WIDTH_BACKGROUND_HORIZONTAL_LIMITS_LOW_"), "%f", &font->widthBgHorizontalLimits[0]);
			widthBgHorizontalLimitsNumberDef[0]++;
		}
		if (strstr(str, "_WIDTH_BACKGROUND_HORIZONTAL_LIMITS_HIGH_"))
		{
			sscanf(strstr(str, "_WIDTH_BACKGROUND_HORIZONTAL_LIMITS_HIGH_") + strlen("_WIDTH_BACKGROUND_HORIZONTAL_LIMITS_HIGH_"), "%f", &font->widthBgHorizontalLimits[1]);
			widthBgHorizontalLimitsNumberDef[1]++;
		}
		if (strstr(str, "_THICK_VERTICAL_LIMITS_LOW_"))
		{
			sscanf(strstr(str, "_THICK_VERTICAL_LIMITS_LOW_") + strlen("_THICK_VERTICAL_LIMITS_LOW_"), "%f", &font->thickVerticalLimits[0]);
			thickVerticalLimitsNumberDef[0]++;
		}
		if (strstr(str, "_THICK_VERTICAL_LIMITS_HIGH_"))
		{
			sscanf(strstr(str, "_THICK_VERTICAL_LIMITS_HIGH_") + strlen("_THICK_VERTICAL_LIMITS_HIGH_"), "%f", &font->thickVerticalLimits[1]);
			thickVerticalLimitsNumberDef[1]++;
		}	
		if (strstr(str, "_VERTICAL_TO_HORIZONTAL_RATIO_LIMITS_LOW_"))
		{
			sscanf(strstr(str, "_VERTICAL_TO_HORIZONTAL_RATIO_LIMITS_LOW_") + strlen("_VERTICAL_TO_HORIZONTAL_RATIO_LIMITS_LOW_"), "%f", &font->ratioHorizontalToVerticalThicksLimits[0]);
			ratioHorizontalToVerticalThicksLimitsNumberDef[0]++;
		}
		if (strstr(str, "_VERTICAL_TO_HORIZONTAL_RATIO_LIMITS_HIGH_"))
		{
			sscanf(strstr(str, "_VERTICAL_TO_HORIZONTAL_RATIO_LIMITS_HIGH_") + strlen("_VERTICAL_TO_HORIZONTAL_RATIO_LIMITS_HIGH_"), "%f", &font->ratioHorizontalToVerticalThicksLimits[1]);
			ratioHorizontalToVerticalThicksLimitsNumberDef[1]++;
		}
		if (strstr(str, "_FACTOR_TO_APPLY_"))
		{
			sscanf(strstr(str, "_FACTOR_TO_APPLY_") + strlen("_FACTOR_TO_APPLY_"), "%f", &font->factorToApply);
			factorToApplyNumberDef++;
		}


		
	}


	if (!endFontFound)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s no closing tag", font->font_name);
		WriteErrors(m_strLog);
	}
	if (!font_nameNumberDef)
	{
		sprintf(m_strLog, "CFonts:ERROR Font after %s - no font name", lastFontName);
		WriteErrors(m_strLog);
	}
	if (font_nameNumberDef != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font  %s - font name defined %d times in one font structure", lastFontName, font_nameNumberDef);
		WriteErrors(m_strLog);
	}
	if (thickHorizontalLimitsNumberDef[0] != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _THICK_HORIZONTAL_LIMITS_LOW_ set %d times", thickHorizontalLimitsNumberDef[0]);
		WriteErrors(m_strLog);
	}
	if (thickHorizontalLimitsNumberDef[1] != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _THICK_HORIZONTAL_LIMITS_HIGH_ set %d times", thickHorizontalLimitsNumberDef[1]);
		WriteErrors(m_strLog);
	}
	if (thickVerticalLimitsNumberDef[0] != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _THICK_VERTICAL_LIMITS_LOW_ set %d times", thickVerticalLimitsNumberDef[0]);
		WriteErrors(m_strLog);
	}
	if (thickVerticalLimitsNumberDef[1] != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _THICK_VERTICAL_LIMITS_HIGH_ set %d times", thickVerticalLimitsNumberDef[1]);
		WriteErrors(m_strLog);
	}
	if (widthLongHorizontalLimitsNumberDef[0] != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _WIDTH_LONG_HORIZONTAL_LIMITS_LOW_ set %d times", widthLongHorizontalLimitsNumberDef[0]);
		WriteErrors(m_strLog);
	}
	if (widthLongHorizontalLimitsNumberDef[1] != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _WIDTH_LONG_HORIZONTAL_LIMITS_HIGH_ set %d times", widthLongHorizontalLimitsNumberDef[1]);
		WriteErrors(m_strLog);
	}
	if (widthBgHorizontalLimitsNumberDef[0] != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _WIDTH_BACKGROUND_HORIZONTAL_LIMITS_LOW_ set %d times", widthBgHorizontalLimitsNumberDef[0]);
		WriteErrors(m_strLog);
	}
	if (widthBgHorizontalLimitsNumberDef[1] != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _WIDTH_BACKGROUND_HORIZONTAL_LIMITS_HIGH_ set %d times", widthBgHorizontalLimitsNumberDef[1]);
		WriteErrors(m_strLog);
	}
	if (ratioHorizontalToVerticalThicksLimitsNumberDef[0] != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _VERTICAL_TO_HORIZONTAL_RATIO_LIMITS_LOW_ set %d times", ratioHorizontalToVerticalThicksLimitsNumberDef[0]);
		WriteErrors(m_strLog);
	}
	if (ratioHorizontalToVerticalThicksLimitsNumberDef[1] != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _VERTICAL_TO_HORIZONTAL_RATIO_LIMITS_HIGH_ set %d times", ratioHorizontalToVerticalThicksLimitsNumberDef[1]);
		WriteErrors(m_strLog);
	}
	if (factorToApplyNumberDef != 1)
	{
		sprintf(m_strLog, "CFonts:ERROR Font %s - _FACTOR_TO_APPLY_ set %d times", factorToApplyNumberDef);
		WriteErrors(m_strLog);
	}
	
    return false;
}

bool CFonts::ReadAllFonts(char* language, int languageCode, char* fileDir, char * filename, bool isAppend)
{

	if (!isAppend)
		InitAllFonts();
	strncpy(m_currentLanguage, language, 99);
	m_currentLanguageCode = languageCode;
	strncpy(m_fontsDirname,fileDir, 999) ;
	strncpy(m_fontsFilename,filename, 999) ;
	char f[1024] = "";
#ifdef _SCANMARKER_WINDOWS_
    SHGetSpecialFolderPath(0, f, CSIDL_COMMON_APPDATA, false);
#else
#endif
	sprintf(m_fontErrorsFile, "%s//Scanmarker//Fonts//%s.errors.txt", f, filename);

	FILE * ferr = fopen(m_fontErrorsFile, "rt");
	if (ferr)
	{
		fclose(ferr);
		ferr = fopen(m_fontErrorsFile, "wt");
		fclose(ferr);
	}
	char fn[1024];
	sprintf(fn, "%s//%s", fileDir, filename);
	FILE * fff = fopen(fn, "rt");
	if (!fff) 
	{
		sprintf(m_strLog, "CFont File %s not opened",filename);
		WriteErrors(m_strLog);
	  	//CLogging::Log(m_strLog);
		return false;
	}
	sprintf(m_strLog, "Font File %s start read",filename);
	//WriteErrors(m_strLog);
	  	

	char str[1000];
	while (fgets(str, 999, fff))
	{
		if (str[0] == '#') continue;
		//WriteErrors(str);
	

		if (strstr(str, "_DEFUALT_FACTOR_TO_APPLY_"))
		{
			sscanf(strstr(str, "_DEFUALT_FACTOR_TO_APPLY_") + strlen("_DEFUALT_FACTOR_TO_APPLY_"), "%f", &m_defaultFactorToApply);
		}
		
		if (strstr(str, "_THICK_HORIZONTAL_BETWEEN_LOW_"))
		{
			sscanf(strstr(str, "_THICK_HORIZONTAL_BETWEEN_LOW_") + strlen("_THICK_HORIZONTAL_BETWEEN_LOW_"), "%f", &m_thickHorizontalBetween[0]);
		}
		if (strstr(str, "_THICK_HORIZONTAL_BETWEEN_HIGH_"))
		{
			sscanf(strstr(str, "_THICK_HORIZONTAL_BETWEEN_HIGH_") + strlen("_THICK_HORIZONTAL_BETWEEN_HIGH_"), "%f", &m_thickHorizontalBetween[1]);
		}
		if (strstr(str, "_THICK_LONG_HORIZONTAL_BETWEEN_LOW_"))
		{
			sscanf(strstr(str, "_THICK_LONG_HORIZONTAL_BETWEEN_LOW_") + strlen("_THICK_LONG_HORIZONTAL_BETWEEN_LOW_"), "%f", &m_thickLongHorizontalBetween[0]);
		}
		if (strstr(str, "_THICK_LONG_HORIZONTAL_BETWEEN_HIGH_"))
		{
			sscanf(strstr(str, "_THICK_LONG_HORIZONTAL_BETWEEN_HIGH_") + strlen("_THICK_LONG_HORIZONTAL_BETWEEN_HIGH_"), "%f", &m_thickLongHorizontalBetween[1]);
		}
		if (strstr(str, "_WIDTH_BACKGROUND_HORIZONTAL_BETWEEN_LOW_"))
		{
			sscanf(strstr(str, "_WIDTH_BACKGROUND_HORIZONTAL_BETWEEN_LOW_") + strlen("_WIDTH_BACKGROUND_HORIZONTAL_BETWEEN_LOW_"), "%f", &m_widthBgHorizontalBetween[0]);
		}
		if (strstr(str, "_WIDTH_BACKGROUND_HORIZONTAL_BETWEEN_HIGH_"))
		{
			sscanf(strstr(str, "_WIDTH_BACKGROUND_HORIZONTAL_BETWEEN_HIGH_") + strlen("_WIDTH_BACKGROUND_HORIZONTAL_BETWEEN_HIGH_"), "%f", &m_widthBgHorizontalBetween[1]);
		}
		if (strstr(str, "<FONT_DEFINITION>"))
		{
			ReadOneFont(fff);
		}
	}




	fclose(fff);
	return 0;
	//sprintf(m_strLog, "Font File %s read successfully",filename);
  	//CLogging::Log(m_strLog);



}

void CFonts::WriteAllFonts(char* fn)
{
	FILE * fff = fopen(fn, "wt");
	if (!fff) return;

	fprintf(fff, "_DEFUALT_FACTOR_TO_APPLY_ %f\n", m_defaultFactorToApply);
	fprintf(fff, "_THICK_HORIZONTAL_BETWEEN_LOW_ %f\n", m_thickHorizontalBetween[0]);
	fprintf(fff, "_THICK_HORIZONTAL_BETWEEN_HIGH_ %f\n", m_thickHorizontalBetween[1]);
	fprintf(fff, "_THICK_LONG_HORIZONTAL_BETWEEN_LOW_ %f\n", m_thickLongHorizontalBetween[0]);
	fprintf(fff, "_THICK_LONG_HORIZONTAL_BETWEEN_HIGH_ %f\n", m_thickLongHorizontalBetween[1]);
	fprintf(fff, "_WIDTH_BACKGROUND_HORIZONTAL_BETWEEN_LOW_%f\n", m_widthBgHorizontalBetween[0]);
	fprintf(fff, "_WIDTH_BACKGROUND_HORIZONTAL_BETWEEN_HIGH_ %f\n", m_widthBgHorizontalBetween[1]);


	for ( int i = 0; i < _MAX_NUMBER_FONTS_; i++)
	{
		if (!m_fonts[i].structInUse ) continue;
		OneFont * font = &m_fonts[i];

		fprintf(fff, "<FONT_DEFINITION>\n");
		fprintf(fff, "\t_FONT_NAME_ %s\n", font->font_name);
		fprintf(fff, "\t _THICK_HORIZONTAL_LIMITS_LOW_ %f\n", font->thickHorizontalLimits[0]);
		fprintf(fff, "\t _THICK_HORIZONTAL_LIMITS_HIGH_ %f\n", font->thickHorizontalLimits[1]);
		fprintf(fff, "\t _WIDTH_LONG_HORIZONTAL_LIMITS_LOW_ %f\n", font->widthLongHorizontalLimits[0]);
		fprintf(fff, "\t _WIDTH_LONG_HORIZONTAL_LIMITS_HIGH_ %f\n", font->widthLongHorizontalLimits[1]);
		fprintf(fff, "\t _WIDTH_BACKGROUND_HORIZONTAL_LIMITS_LOW_ %f\n", font->widthBgHorizontalLimits[0]);
		fprintf(fff, "\t _WIDTH_BACKGROUND_HORIZONTAL_LIMITS_HIGH_ %f\n", font->widthBgHorizontalLimits[1]);
		fprintf(fff, "\t _THICK_VERTICAL_LIMITS_LOW_ %f\n", font->thickVerticalLimits[0]);
		fprintf(fff, "\t _THICK_VERTICAL_LIMITS_HIGH_ %f\n", font->thickVerticalLimits[1]);
		fprintf(fff, "\t _VERTICAL_TO_HORIZONTAL_RATIO_LIMITS_LOW_ %f\n", font->ratioHorizontalToVerticalThicksLimits[0]);
		fprintf(fff, "\t _VERTICAL_TO_HORIZONTAL_RATIO_LIMITS_HIGH_ %f\n", font->ratioHorizontalToVerticalThicksLimits[1]);
		fprintf(fff, "\t _FACTOR_TO_APPLY_ %f\n", font->factorToApply);
		fprintf(fff, "</FONT_DEFINITION>\n");
	}
	fclose(fff);

}