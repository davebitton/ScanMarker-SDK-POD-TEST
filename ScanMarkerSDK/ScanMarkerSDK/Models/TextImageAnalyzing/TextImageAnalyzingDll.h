// TextImageAnalyzingDll.h

#pragma once

//using namespace System;

#define _LIBRARY_VERSION_ "2.1.0.3"
#define byte uint8_t

/*public */ class MainClass
{
public:
	 static char m_srcFilename[255];
static char* Run(bool isBkgrWhite, int language, int imageType, int width, int height, byte* image, byte* imageOut,int* outputSnakes, int maxNumberOutputSnakes, bool isChineseVertical);
static  bool IsShortImage(byte* image,  int w, int h);
	// TODO: Add your methods for this class here.
};

void aa (void);

extern "C" char* __stdcall CheckScan(bool isBkgrWhite, int language, int imageType, int width, int height, byte* image, byte* imageOut, int* outputSnakes, int maxNumberOutputSnakes, bool isChineseVertical);
//extern "C" __declspec(dllexport) char* __stdcall Run(bool isBkgrWhite, int language, int imageType, int width, int height, byte* image, byte* imageOut);
extern "C" void SetSrcFileName(char* filename);
