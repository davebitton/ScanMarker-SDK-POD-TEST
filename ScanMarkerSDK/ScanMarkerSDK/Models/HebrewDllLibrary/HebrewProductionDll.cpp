#include <stdio.h>
#ifdef _SCANMARKER_WINDOWS_
#include <conio.h>
#include "xv.h"
#include "xvbmp.h"
#else
#include "bmp/xv.h"
#include "bmp/xvbmp.h"
#include <mach-o/dyld.h>
#endif
#include <string.h>
#include "HebrewFull.h"
//#include "HebrewFull_CMC7.h"

#include "fonts.h"
#define APP_ENGINE_LNG_CMC7			332
#define APP_ENGINE_LNG_HEBREW		230
#define APP_ENGINE_LNG_VERTICAL_CHINESE_TRADITIONAL		364
#define APP_ENGINE_LNG_VERTICAL_CHINESE_SIMPLIFIED		365
#define APP_ENGINE_LNG_VERTICAL_JAPANESE 				366
#define APP_ENGINE_LNG_VERTICAL_KOREAN					367	
#define APP_ENGINE_LNG_CHINESE_TRADITIONAL		221
#define APP_ENGINE_LNG_CHINESE_SIMPLIFIED		222
#define APP_ENGINE_LNG_HEBREW					230
#define APP_ENGINE_LNG_JAPANESE 				278
#define APP_ENGINE_LNG_KOREAN					232
void GetInputFile(PICINFO &pinfo, byte * input);
 //extern "C" __declspec(dllexport) int HebrewImageProcessByMemory(byte* input, int w_In, int h_In, 
void PrintDebugStr(char* str, bool append = true)
{
	//printf(str);
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
extern "C" int ProcessImage(byte* input1, int w_In, int h_In,
												  byte* inputRaw, int w_Raw, int h_Raw, 
															     byte* output_OCR1, int & w_OCR, int & h_OCR,
																 byte * output_IC1, int & w_IC, int & h_IC,
																  int &italicStep,
																 int _debugMode, 
																 char* debugFileName,
																 long fontsPtr ,
																 int languageCode, 
																  float compressFactor,
																 int heightOfOutputBody,
																 bool corierActive, 
																 bool isOldStraight,
																 bool directionFromLeftToRight, 
																 int aveHeightDefault, 
																 int* param, 
																 bool isVertical,
																 void *pIdrsReader,
#ifndef _SCANMARKER_WINDOWS_
                                                                 void *pobjIDRS,
#endif
																 bool isTableWithLines) 
 { 
  

	 
	if (languageCode == APP_ENGINE_LNG_VERTICAL_CHINESE_TRADITIONAL)
	{
		languageCode = APP_ENGINE_LNG_CHINESE_TRADITIONAL;
		isVertical = true;
	}
	if (languageCode == APP_ENGINE_LNG_VERTICAL_CHINESE_SIMPLIFIED)
	{
		languageCode = APP_ENGINE_LNG_CHINESE_SIMPLIFIED;
		isVertical = true;
	}
	if (languageCode == APP_ENGINE_LNG_VERTICAL_JAPANESE)
	{
		languageCode = APP_ENGINE_LNG_JAPANESE;
		isVertical = true;
	}
	if (languageCode == APP_ENGINE_LNG_VERTICAL_KOREAN)
	{
		languageCode = APP_ENGINE_LNG_KOREAN;
		isVertical = true;
	}
	FILE * fff = NULL;
	char fn[1000];
	sprintf(fn, "%s_hebrewFull__Log.txt", debugFileName);
     //int wRaw1 = 0;

    int sizeIn= (w_In*3*2 + 600)*h_In*2*2*2;
	int sizeOut  = (w_In*2*3*h_In*2+600)*2*2;
	int maxWidthOut = w_In*2*16;
	byte* input = new byte[sizeIn];
	memset(input, 0, sizeIn);
	byte* output_OCR = new byte[sizeOut];
	memset(output_OCR, 0, sizeOut);
	byte* output_IC = new byte[sizeOut];
	memset(output_IC, 0, sizeOut);
	for ( int i = 0; i < w_In; i++)
	{
		for ( int j = 0; j < h_In; j++)
		{
			input[j*w_In+i] = input1[j*w_In+i];
		}
	}

/* 
	for (int i = 0; i < w_In; i++)
	{
		for ( int j = 0; j < h_In; j++)
		{
			output_OCR[w_In*3*j+i*3+0] = input[w_In*j+i];
			output_OCR[w_In*3*j+i*3+1] = input[w_In*j+i];
			output_OCR[w_In*3*j+i*3+2] = input[w_In*j+i];
		}
	}
	FILE *fp = fopen("C://Temp//debug.bmp", "wb");
	
	WriteBMP(fp,output_OCR,PIC24,output_width,output_height,NULL,NULL, NULL,8,F_FULLCOLOR);
	fclose(fp);

*/



	int output_width = 0;
	int output_height = 0;
	int output_width_2 = 0;
	int output_height_2 = 0;
    int res = 0;
	//if (languageCode == APP_ENGINE_LNG_CMC7)
	//{
		/*HebrewFull_CMC7 hebrewFull;

	char dirName[1000];
    //--shimon
#ifdef _SCANMARKER_WINDOWS_
    GetCurrentDirectory(999, (LPTSTR) dirName);
#else
    uint32_t size = sizeof(dirName);
    _NSGetExecutablePath(dirName, &size);
#endif
    
		res = hebrewFull.Init(input, output_OCR, w_In, h_In, w_In, _debugMode, debugFileName);
		
		hebrewFull.m_dataRaw = inputRaw;
		hebrewFull.m_widthRaw = w_Raw;
		hebrewFull.m_heightRaw = h_Raw;
		hebrewFull.m_pIdrsReader = pIdrsReader;
#ifndef _SCANMARKER_WINDOWS_
		hebrewFull.m_pObjIDRS = pobjIDRS;
#endif
        int result = 0;
		if ( res != -1000) 
		{
		   try
		   {
				res = hebrewFull.Calculate(isOldStraight);
				output_width = hebrewFull.m_width;
				output_height = hebrewFull.m_height;
				output_width_2 = hebrewFull.m_width;
				output_height_2 = hebrewFull.m_height;
 
		   }
		   catch(...)
		   {
			   ;
		   }
		   
		
		}
		
	}
	else*/
	{
		HebrewFull hebrewFull;

		char dirName[1000];
    //--shimon
#ifdef _SCANMARKER_WINDOWS_
    GetCurrentDirectory(999, (LPTSTR) dirName);
#else
    uint32_t size = sizeof(dirName);
    _NSGetExecutablePath(dirName, &size);
#endif
		res = hebrewFull.Init(corierActive, w_In, h_In, input, output_OCR,output_IC,  _debugMode,
			dirName, debugFileName, directionFromLeftToRight);
		hebrewFull.m_dataRaw = inputRaw;
		hebrewFull.m_widthRaw = w_Raw;
		hebrewFull.m_heightRaw = h_Raw;
		hebrewFull.m_language_In = languageCode;
		if (param)
			compressFactor = param[0]/1000.0;
		hebrewFull.m_compressFactor_In = compressFactor;
		hebrewFull.m_heightOfOutputBody = heightOfOutputBody;
		hebrewFull.m_fonts = (CFonts*) fontsPtr;
	
		hebrewFull.m_aveHeightDefault = aveHeightDefault;
		hebrewFull.m_italicStep = italicStep;
		hebrewFull.m_isVertical = isVertical;
		hebrewFull.m_isTableWithLines = isTableWithLines;
		int result = 0;
		if ( res != -1000) 
		{
		   try
		   {
				res = hebrewFull.Calculate(isOldStraight, param);
				italicStep = hebrewFull.m_italicStep;
				output_width = hebrewFull.m_width;
				output_height = hebrewFull.m_height;
				output_width_2 = hebrewFull.m_width_2;
				output_height_2 = hebrewFull.m_height_2;
 
		   }
		   catch(...)
		   {
			   ;
		   }
		}
	}
	if ( res == -1000)
	{
		w_OCR = w_In;
		h_OCR = h_In;

		w_IC = w_In;
		h_IC = h_In;

		for ( int i = 0; i < w_OCR; i++)
		{
			for ( int j = 0; j < h_OCR; j++)
			{
				output_OCR[3*j*w_OCR + 3*i + 0] = input[j*w_In+i];
				output_OCR[3*j*w_OCR + 3*i + 1] = input[j*w_In+i];
				output_OCR[3*j*w_OCR + 3*i + 2] = input[j*w_In+i];
			}
		}
		for ( int i = 0; i < w_IC; i++)
		{
			for ( int j = 0; j < h_IC; j++)
			{
				output_IC[3*j*w_IC+ 3*i + 0] = input[j*w_In+i];
				output_IC[3*j*w_IC+ 3*i + 1] = input[j*w_In+i];
				output_IC[3*j*w_IC+ 3*i + 2] = input[j*w_In+i];
			}
		}
		if ( input) 
		{
			delete [] input;
			input = NULL;
		}
		if ( output_OCR) 
		{
			delete [] output_OCR;
			output_OCR = NULL;
		}
		if ( output_IC) 
		{
			delete [] output_IC;
			output_IC = NULL;
		}
		return -1000;

	}
		





   //byte * output = output_OCR;
   //byte * output_2 = output_IC;

/*fDebug = fopen(filenameDebug, "at");
fprintf(fDebug, "ddddddd");
fclose(fDebug);*/

    if (output_width < w_OCR || w_OCR == 0) 
		w_OCR = output_width;
	if (output_height < h_OCR || h_OCR == 0) 
		h_OCR = output_height;

	if (output_width_2 < w_IC || w_IC == 0) 
		w_IC = output_width_2;
	if (output_height_2 < h_IC || h_IC == 0) 
		h_IC = output_height_2;
	//if ( hebrewFull.m_isWhiteBackground)
	{
		for (int i = 0; i < output_width; i++)
		{
			if (i >= w_OCR) continue;
			for ( int j = 0; j < output_height; j++)
			{
				if (j >= h_OCR) continue;
				output_OCR1[w_OCR*3*j+i*3+0] = 255 - output_OCR[output_width*3*j+i*3+0];
				output_OCR1[w_OCR*3*j+i*3+1] = 255 - output_OCR[output_width*3*j+i*3+1];
				output_OCR1[w_OCR*3*j+i*3+2] = 255 - output_OCR[output_width*3*j+i*3+2];
			}
		}
		for (int i = 0; i < output_width_2; i++)
		{
			if (i >= w_IC) continue;
			for ( int j = 0; j < output_height_2; j++)
			{
				if (j >= h_IC) continue;
				output_IC1[w_IC*3*j+i*3+0] = 255 - output_IC[output_width_2*3*j+i*3+0];
				output_IC1[w_IC*3*j+i*3+1] = 255 - output_IC[output_width_2*3*j+i*3+1];
				output_IC1[w_IC*3*j+i*3+2] = 255 - output_IC[output_width_2*3*j+i*3+2];
			}
		}
	}


    
    FILE *fp = fopen(debugFileName, "wb");
     if (fp) {
         WriteBMP(fp,output_OCR,PIC24,w_OCR,h_OCR,NULL,NULL, NULL,8,F_FULLCOLOR);
         fclose(fp);
     }

	if ( input) 
	{
		delete [] input;
		input = NULL;
	}
	if ( output_OCR) 
	{
		delete [] output_OCR;
		output_OCR = NULL;
	}
	if ( output_IC) 
	{
		delete [] output_IC;
		output_IC = NULL;
	}
	return 1;

 }


extern "C" int HebrewImageProcess(char* filenameIn, char* filenameOut_OCR,char* filenameOut_IC,
														int _debugMode, bool isOldStraight, int isWhiteBackground, int* param) 
{ 
	//ReadCfgFile();
char  filenameDebug[1024] = "C:\\Disk_D\\Projects\\TopScan\\Work Images\\For Dll\\debug.txt";

/*FILE *fDebug = fopen(filenameDebug, "wt");
fprintf(fDebug, "aaaaaaa");
fclose(fDebug);
*/
	PICINFO pinfo;
			
	LoadBMP(filenameIn, &pinfo);
		
/*fDebug = fopen(filenameDebug, "at");
fprintf(fDebug, "bbbbbbbb");
fclose(fDebug);*/
	byte * input = new byte[(pinfo.w*3 + 600)*pinfo.h*2];
	memset(input, 0,  sizeof(byte)*(pinfo.w*3 + 600)*pinfo.h*2);
	
	GetInputFile(pinfo, input);

	if ( isWhiteBackground)
	{
		for (int i = 0; i < pinfo.w; i++)
			for ( int j = 0; j < pinfo.h; j++)
				input[pinfo.w*j+i] = 255 - input[pinfo.w*j+i];
	}


	int output_width = pinfo.w*3+600;
	int output_height = pinfo.h*2;
	int output_width_2 = pinfo.w*3+600;
	int output_height_2 = pinfo.h*2;

	byte * output = new byte[output_height*3*output_width+600];
	memset(output, 0,  sizeof(byte)*output_height*3*output_width);
	byte * output_2 = new byte[output_height*3*output_width+600];
	memset(output_2, 0,  sizeof(byte)*output_height*3*output_width);


	HebrewFull hebrewFull;

	char dirName[1000];
#ifdef _SCANMARKER_WINDOWS_
    GetCurrentDirectory(999, (LPTSTR) dirName);
#else
    uint32_t size = sizeof(dirName);
    _NSGetExecutablePath(dirName, &size);
#endif
    hebrewFull.Init(true, pinfo.w, pinfo.h, input, output,output_2,  _debugMode,
		dirName, filenameOut_OCR, true);
	int result = 0;
   try
   {
		result = hebrewFull.Calculate(isOldStraight, param);
   }
   catch(...)
   {
	   ;
   }
   output_width = hebrewFull.m_width;
   output_height = hebrewFull.m_height;
   output_width_2 = hebrewFull.m_width_2;
   output_height_2 = hebrewFull.m_height_2;
/*fDebug = fopen(filenameDebug, "at");
fprintf(fDebug, "ddddddd");
fclose(fDebug);*/

	if ( isWhiteBackground)
	{
		for (int i = 0; i < output_width; i++)
			for ( int j = 0; j < output_height; j++)
			{
				output[output_width*3*j+i*3+0] = 255 - output[output_width*3*j+i*3+0];
				output[output_width*3*j+i*3+1] = 255 - output[output_width*3*j+i*3+1];
				output[output_width*3*j+i*3+2] = 255 - output[output_width*3*j+i*3+2];
			}
		for (int i = 0; i < output_width_2; i++)
			for ( int j = 0; j < output_height_2; j++)
			{
				output_2[output_width_2*3*j+i*3+0] = 255 - output_2[output_width_2*3*j+i*3+0];
				output_2[output_width_2*3*j+i*3+1] = 255 - output_2[output_width_2*3*j+i*3+1];
				output_2[output_width_2*3*j+i*3+2] = 255 - output_2[output_width_2*3*j+i*3+2];
			}
	}

	hebrewFull.PrintDebugStr("aaaaaa", false);
	FILE *fp = fopen(filenameOut_OCR, "wb");
	
	WriteBMP(fp,output,PIC24,output_width,output_height,NULL,NULL, NULL,8,F_FULLCOLOR);
	fclose(fp);
	hebrewFull.PrintDebugStr(filenameOut_IC);
	FILE *fp_2 = fopen(filenameOut_IC, "wb");
	WriteBMP(fp_2,output_2,PIC24,output_width_2,output_height_2,NULL,NULL, NULL,8,F_FULLCOLOR);
	fclose(fp_2);
hebrewFull.PrintDebugStr("ccccc");
	
	delete[] output;
	delete[] output_2;
hebrewFull.PrintDebugStr("dddddd");
		return result;
}
void GetInputFile(PICINFO &pinfo, byte * input)
{
	int s = 0;
	if (pinfo.type == PIC32)
	{
		s = 3;
	}
	else if (pinfo.type == PIC24)
	{
		s = 3;
	}
	else if ((pinfo.type == PIC8) && (pinfo.biBitCount == 1))
	{
		s = 0;
	}
	else if (pinfo.type == PIC8) 
	{
		s = 1;
	}
	if (s == 0)
	{
		for ( int i1 = 0; i1 < pinfo.w; i1++)
		{
			for ( int j1 = 0; j1 < pinfo.h; j1++)
			{
				int r = pinfo.pic[j1*pinfo.w + i1+0];
				int color = 0;
				if (r == 1) 
					color = 255;
				else 
					color = 0;
				input[j1*pinfo.w + i1]  = color;
			}
		}
	}
	else if (s == 1)
	{
		for ( int i1 = 0; i1 < pinfo.w; i1++)
		{
			for ( int j1 = 0; j1 < pinfo.h; j1++)
			{
				int r = pinfo.pic[j1*pinfo.w*s + i1*s+0];
				int color = 0;
				if (r < 250) 
					color = 0;
				else 
					color = 255;
				input[j1*pinfo.w + i1]  = color;
			}
		}
	}
	else
	{
		for ( int i1 = 0; i1 < pinfo.w; i1++)
		{
			for ( int j1 = 0; j1 < pinfo.h; j1++)
			{
				int r = pinfo.pic[j1*pinfo.w*s + i1*s+0];
				int g = pinfo.pic[j1*pinfo.w*s + i1*s+1];
				int b = pinfo.pic[j1*pinfo.w*s + i1*s+2];
				int color = 0;
				if ((r < 50) && (g < 50) && (b < 50))
					color = 0;
				else 
					color = 255;
				input[j1*pinfo.w + i1]  = color;
			}
		}
	}

}
extern "C" void  CreateFontsStruct(long & fontsPtr)
{
	CFonts * fonts = new CFonts();
	fontsPtr = (long) fonts;

}
extern "C" void DestroyFontsStruct(long & fontsPtr)
{
	CFonts * fonts = (CFonts *) fontsPtr;
	if (!fonts) return;
	delete fonts;
	fontsPtr = NULL;

}

extern "C" bool ReadFontFile(long  fontsPtr, char* language, int languageCode, char* fileDir, bool isAppend = false)
{
	CFonts * fonts = (CFonts *) fontsPtr;
	if (!fonts) return false;
	char filename[200];
	sprintf(filename, "font_%s_%d.txt", language, languageCode);
	for  ( int i = 0;  i < strlen(filename); i++)
	{
		if (filename[i] == ',' || filename[i] == ' ')
			filename[i] = '_';
	}
	return fonts->ReadAllFonts(language, languageCode, fileDir, filename, isAppend = false);
}

extern "C" void WriteAllFonts(long  fontsPtr , char* fn )
{
	CFonts * fonts = (CFonts *) fontsPtr;
	if (!fonts) return;
	fonts->WriteAllFonts(fn);;
}
