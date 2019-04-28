/*
typedef struct 
{
	char font_name[100];
	int  font_code;
	float thickHorizontalLimits[2];
	float thickVerticalLimits[4];
	float widthLongHorizontalLimits[4];
	float widthBgHorizontalLimits[4];
	float ratioThicks;
} OneFont;
*/

#define _LIBRARY_VERSION_ "2.1.0.3"
#define byte uint8_t

extern "C" int HebrewImageProcess(char* filenameIn,
														 char* filenameOut_OCR,
														 char* filenameOut_IC, 
														 int _debugMode,
														 bool isOldStraight = true,
														 int isWhiteBackground = true);
//extern "C" __declspec(dllexport) int HebrewImageProcessByMemory(byte* input, int w_In, int h_In,
extern "C" int ProcessImage(byte* input, int w_In, int h_In,
																 byte* inputRaw, int w_Raw, int h_Raw, 
																 byte* output_OCR, int & w_OCR, int & h_OCR,
																 byte * output_IC, int & w_IC, int & h_IC,
																 int &italicStep,
																 int _debugMode, 
																 char* debugFileName,
																 long fontsPtr = NULL,
																 int languageCode = 0, 
																 float compressFactor = 1.0,
																 int heightOfOutputBody = 44, 
														 		 bool corierActive = true,
																 bool isOldStraight = true,
																 bool directionFromLeftToRight = true, 
																 int aveHeightDefault = -1, 
																 int* param = NULL, 
																 bool isVertical = false, 
																 void *pIdrsReader = NULL, 
#ifndef _SCANMARKER_WINDOWS_
                                                                 void *pobjIDRS = NULL,
#endif
																 bool isTableWithLines = false);


extern "C" void CreateFontsStruct(long & fontsPtr);
extern "C" void DestroyFontsStruct(long & fontsPtr);


extern "C" bool ReadFontFile(long  fontsPtr, char* language, int languageCode, char* fileDir, bool isAppend = false);

extern "C" void WriteAllFonts(long  fontsPtr, char* fn);
