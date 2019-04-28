// This is the main DLL file.

#ifdef _SCANMARKER_WINDOWS_
#include "xv.h";
#include "xvbmp.h";
//#include <mach-o/dyld.h>
#else
#include "xv.h"
#include "xvbmp.h"
#endif
//#include "Classes/Slow.h";
#include "SlowLength.h";
#include "Jerky_new.h";
#include "TextIsOut.h";
#include "OneLine_4.h";

#include "TextImageAnalyzingDll.h"
char g_resultString[255];
char MainClass::m_srcFilename[255];
#define _DEBUG_ 0
unsigned int getTimeMillisecond() 
{
#ifdef _SCANMARKER_WINDOWS_
     __int64 freq, time, time_milli;
     unsigned int milliseconds;

     // The QueryPerformanceFrequency function retrieves the   frequency of the high-resolution performance counter, if one exists. The frequency cannot change while the system is running
     QueryPerformanceFrequency((LARGE_INTEGER*)&freq);  
 
       // The QueryPerformanceCounter function retrieves the current  value of the high-resolution performance counter
     QueryPerformanceCounter((LARGE_INTEGER*)&time);
    
     time_milli = ((time) * 1000) / freq;
     milliseconds = (unsigned int)(time_milli & 0xffffffff);
     return milliseconds;
#else
    return 0;
#endif
} 
void SetSrcFileName(char* filename)
{
	strncpy(MainClass::m_srcFilename, filename, 254);
}

void aa (void) {
    int x = 0;
}

// __declspec(dllexport) char*  __stdcall Run(bool isBkgrWhite,  int language, int imageType, int width, int height, byte* image, byte* imageOut)
char*  __stdcall CheckScan(bool isBkgrWhite,  int language, int imageType, int width, int height, byte* image, byte* imageOut,int* outputSnakes, int maxNumberOutputSnakes, bool isChineseVertical)
{
	return MainClass::Run(isBkgrWhite,  language, imageType, width, height, image, imageOut,outputSnakes, maxNumberOutputSnakes,isChineseVertical);
}
 bool MainClass::IsShortImage(byte* image,  int w, int h)
{
	int numChanges[128];
	for ( int j = 0; j < h; j++)
	{
		numChanges[j] = 0;
	}
	for ( int i = 0; i < w-1; i++)
	{
		for ( int j = 0; j < h; j++)
		{
			if ( image[j*w + i + 0] != image[j*w + (i+1) + 0])
			{
				numChanges[j]++;
			}
		}
	}
	int aveChanges = 0;
	int n = 0;
	for ( int j = 0; j < h; j++)
	{
		if (numChanges[j] >= 3)
		{
			aveChanges += numChanges[j];
			n++;
		}
	}


	if ( n ) aveChanges /= n;
	if (!n || aveChanges > 40) 
	{
		
		return false;
	}

	
	
	return true;
}
char* MainClass::Run(bool isBkgrWhite, int language, int imageType, int width, int height, byte* image1, byte* imageOut1,int* outputSnakes, int maxNumberOutputSnakes, bool isChineseVertical)
{
 
		    char currentDirectory[1024];
#ifdef _SCANMARKER_WINDOWS_
		GetCurrentDirectoryA(1024, currentDirectory);
			char appDirectory[1024];
			GetModuleFileNameA(NULL, appDirectory, 1023);
			SetCurrentDirectoryA(appDirectory);
#endif
			
			int sizeIn= (width*3 + 600)*height*2*2*2;
			int sizeOut  = (width*2*3*height*2+600)*2*2;

			byte* image = new byte[sizeIn];
			memset(image, 0, sizeIn);
			byte* imageOut = new byte[sizeOut];
			memset(imageOut, 0, sizeOut);
			for ( int i = 0; i < width; i++)
			{
				for ( int j = 0; j < height; j++)
				{
					image[j*width+i] = image1[j*width+i];
				}
			}

			
			/*PICINFO pinfo;*/
			char *fileName = new char[255];
			strcpy(fileName, m_srcFilename);
			char * dirName = new char[255];
			strcpy(dirName, fileName);
			for ( int i1 = strlen(dirName); i1 >=0; i1--)
			{
				if ( (dirName[i1] == '\\') || (dirName[i1] == '/'))
				{
					dirName[i1] = 0;
					break;
				}
			}
			/*PICINFO pinfo;
		    LoadBMP(fileName , &pinfo);
			width = pinfo.w;
			height = pinfo.h;
			{
				FILE * fff = fopen("C://Temp//debug.txt", "wt");
				if (fff)
				{
					fprintf(fff, "\n After LoadBMP" );
					fclose(fff);
				}
			}
			byte * input= new byte[width*height];
			memset(input, 0,  sizeof(byte)*width*height);
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
				for ( int i1 = 0; i1 < width; i1++)
				{
					for ( int j1 = 0; j1 < height; j1++)
					{
						int r = pinfo.pic[j1*width + i1+0];
						int color = 0;
						if (r == 1) 
							color = 255;
						else 
							color = 0;
						input[j1*width + i1]  = color;
					}
				}
			}
			else if (s == 1)
			{
				for ( int i1 = 0; i1 < width; i1++)
				{
					for ( int j1 = 0; j1 < height; j1++)
					{
						int r = pinfo.pic[j1*width*s + i1*s+0];
						int color = 0;
						if (r < 250) 
							color = 0;
						else 
							color = 255;
						input[j1*width + i1]  = color;
					}
				}
			}
			else
			{
				for ( int i1 = 0; i1 < width; i1++)
				{
					for ( int j1 = 0; j1 < height; j1++)
					{
						int r = pinfo.pic[j1*width*s + i1*s+0];
						int g = pinfo.pic[j1*width*s + i1*s+1];
						int b = pinfo.pic[j1*width*s + i1*s+2];
						int color = 0;
						if ((r < 50) && (g < 50) && (b < 50))
							color = 0;
						else 
							color = 255;
						input[j1*width + i1]  = color;
					}
				}
			}

			*/
			byte * output = new byte[width*height*3*2];
			memset(output, 0,  sizeof(byte)*width*height*3*2);
			byte * input= new byte[width*height*2];
			memset(input, 0,  sizeof(byte)*width*height*2);



			if (imageType == 1) 
			{
				for ( int i = 0; i < width; i++)
				{
					for ( int j = 0; j < height; j++)
					{
						input[j*width + i]  = image[j*width + i];
						output[j*width*3 + i*3 + 0]  = image[j*width + i];
						output[j*width*3 + i*3 + 1]  = image[j*width + i];
						output[j*width*3 + i*3 + 2]  = image[j*width + i];
					}
				}
			}
			
			int numberWhite = 0;
			int numberBlack = 0;

			for ( int i = 0; i < width; i++)
			{
				for ( int j = 0; j < height; j++)
				{
					if ( input[j*width+i] == 0)
					{
						numberBlack++;
					}
					else
					{
						numberWhite++;
					}
				}
			}

			isBkgrWhite= false;
			if ( numberWhite > numberBlack)
			{
				isBkgrWhite = true;
			}
						
			
			
			if (isBkgrWhite)
			{
				for ( int i = 0; i < width; i++)
				{
					for ( int j = 0; j < height; j++)
					{
						input[j*width + i] = 255 - input[j*width + i];
					}
				}
			}

			int countOnlyTextColumns = 0;
			for ( int i = 0; i < width; i++)
			{
				bool wholeColumnIsTextColor = true;
				for ( int j = 0; j < height; j++)
				{
					byte color = input[j*width + i];
					if (color == 0x00)
						wholeColumnIsTextColor = false;
				}
				if (wholeColumnIsTextColor)
				{
					countOnlyTextColumns++;
					for ( int j = 0; j < height; j++)
					{
						input[j*width + i] = 0;
					}
				}
			}


			for ( int j = 0; j < height; j++)
			{
				int numberTextColorInWholeRow = 0;
				for ( int i = 0; i < width; i++)
				{
					byte color = input[j*width + i];
					if (color != 0x00)
						numberTextColorInWholeRow++ ;
				}
				if (numberTextColorInWholeRow > width-countOnlyTextColumns-2)
				{
					for ( int i = 0; i < width; i++)
					{
						input[j*width + i] = 0;
					}
				}
			}
			for ( int i = 0; i < width; i++)
			{
				for ( int j = 0; j < height; j++)
				{
					output[3*j*width+ 3*i + 0]  = input[j*width+ i];
					output[3*j*width+ 3*i + 1]  = input[j*width+ i];
					output[3*j*width+ 3*i + 2]  = input[j*width+ i];
				}
			}



			int w_jerky = width;
			int h_jerky = height;

			byte * input_jerky= new byte[w_jerky*h_jerky];
			memset(input_jerky, 0,  sizeof(byte)*w_jerky*h_jerky);
			for ( int i = 0; i < w_jerky; i++)
			{
				for ( int j = 0; j < h_jerky; j++)
				{
					input_jerky[j*w_jerky+ i]  = input[j*w_jerky+ i];
				}
			}

               
			


/*
			FILE *fp1 = fopen("C://Temp//pinfo_pic.bmp", "wb");
			WriteBMP(fp1,input, .type,width,height,pinfo.r , pinfo.g, pinfo.b, 1 << pinfo.biBitCount,pinfo.colType);
			fclose(fp1);
*/

			

			byte * output1 = new byte[width*10*3];
			memset(output1, 0,  sizeof(byte)*width*10*3);
			
#ifdef _SCANMARKER_WINDOWS_
			SetCurrentDirectoryA( currentDirectory);
#endif
			if (language == 221 || language == 222)
				strcpy(dirName, "Chineese_CFG");
  			else
				strcpy(dirName, "English_CFG");

			int bodyHeight = 0;
			int time1 = getTimeMillisecond(); 
			bool firstWordBad = false;
			bool isTable = false;
			{
			COneLine_4 OneLine;
			OneLine.Init(width, height, input, output, NULL);
			if (outputSnakes && maxNumberOutputSnakes > 0)
				isTable = true;
			OneLine.m_outputSnakes = outputSnakes;
			OneLine.m_maxNumberOutputSnakes = maxNumberOutputSnakes;
			OneLine.SetParameters(1,25,20, 40, 1000);
			OneLine.m_isChineseVertical = isChineseVertical;
			//OneLine.ReadCfgFile(dirName);
			OneLine.Calculate(isTable,language);
			bodyHeight = OneLine.m_bodyHeight;
			firstWordBad = OneLine.m_firstWordBad;
    //--shimon
			}
			


	/*		char fileName1[1024];
			sprintf(fileName1, "%s_ol.bmp", fileName);
			FILE *fp = fopen(fileName1, "wb");
			
			WriteBMP(fp,output,PIC24,width,height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
	*/		
			int w = width;
			int h = height;
			byte * input1= new byte[width*height];
			memset(input1, 0, width*height*sizeof(byte));
			byte * input2= new byte[width*height];
			memset(input2, 0, width*height*sizeof(byte));
			for ( int i = 0; i < w; i++)
			{
				for ( int j = 0; j < h; j++)
				{
					byte value = output[j*w*3 + i*3 + 1];
					imageOut1[j*w*3 + i*3 + 0] = 255-output[j*w*3 + i*3 + 1];
					imageOut1[j*w*3 + i*3 + 1] = 255-output[j*w*3 + i*3 + 1];
					imageOut1[j*w*3 + i*3 + 2] = 255-output[j*w*3 + i*3 + 1];
					input1[j*w + i] = value;
					input2[j*w + i] = value;
				}

			}
			if ( width < 200 || isTable)
			{
				sprintf	(g_resultString, "%s %s %s", "slow_good" ,"jerky_good" ,"textout_good");
				if (input)
				{
					delete [] input;
					input = NULL;
				}
				if (input1)
				{
					delete [] input1;
					input1 = NULL;
				}
				if (input2)
				{
					delete [] input2;
					input2 = NULL;
				}
				
				if (image)
				{
					delete [] image;
					image = NULL;
				}
				if (imageOut)
				{
					delete [] imageOut;
					imageOut = NULL;
				}
				if (input_jerky)
				{
					delete [] input_jerky;
					input_jerky = NULL;
				}
			

			
				delete[] fileName;
	//			delete[] fileNameOut;
				delete[] output;
	//			delete[] fileNameOut1;
				delete[] output1;
				//delete[] pinfo.pic;
				delete[] dirName;
				return g_resultString;
		 
			}
			/*{
			FILE *fp1 = fopen("/Users/jonathan/Developer/Scanmaker/MultiThread/DerivedData/MultiThread/Build/Products/Debug/dbg1.bmp", "wb");
			//WriteBMP(fp1,input1, PIC8,width,height,pinfo.r , pinfo.g, pinfo.b, 1 << pinfo.biBitCount,pinfo.colType);
			//WriteBMP(fp1,input1,PIC8,width,height,NULL,NULL, NULL,8,F_FULLCOLOR);
                if (fp1)
                {
                    WriteBMP(fp1,output, PIC24,width,height,NULL,NULL, NULL,8,F_FULLCOLOR);
                    fclose(fp1);
                }
			}*/
			
			memset(output, 0, w*h*3*sizeof(byte));
			time1 = getTimeMillisecond();
			bool slowLengthResults = true;
			bool fastLengthResults = true;
			{
			SlowLength slowLength;
			slowLength.Init(width, height, input1, output, output1);
			slowLength.SetParameters(5,25,20, 40, 1000, 25, NULL);
			slowLength.ReadCfgFile(dirName);
			slowLength.m_language = language;
			slowLength.m_bodyHeight = bodyHeight;
			slowLength.CalculateSlow();
			fastLengthResults = slowLength.m_resultTooFast; 
			slowLengthResults = slowLength.m_resultTooSlow; 
			}



			memset(output, 0,  sizeof(byte)*width*height*3);
			memset(output1, 0,  sizeof(byte)*width*10*3);

			time1 = getTimeMillisecond();
			
			bool jerkyResults = true;
			bool checkJerky = true;
			//if (language == 3)
				//checkJerky = false;
			/*else */if (IsShortImage(input_jerky, w_jerky,h_jerky))
				checkJerky = false;

			if (checkJerky)
			{
			Jerky_new jerky;
//			fnOut = fnOutSrc +"_jerk_1.bmp";
//			fnOut1 = fnOutSrc +"_jerk_2.bmp";

			jerky.Init(w_jerky, h_jerky, input_jerky, output, output1);
			jerky.SetParameters(5,25,20, 40, 1000, 25, NULL);
			jerky.ReadCfgFile(dirName);
			jerky.CalculateJerky();
			jerkyResults = jerky.m_result;
			}
//			System::String^ resultString;

			memset(output, 0,  sizeof(byte)*width*height*3);
			memset(output1, 0,  sizeof(byte)*width*10*3);




			time1 = getTimeMillisecond();
			bool textoutResult = true;
			{
			TextIsOut textout;
//			fnOut = fnOutSrc +"_out_1.bmp";
//			fnOut1 = fnOutSrc +"_out_2.bmp";
			textout.Init(width, height, input2, output, output1);
			textout.SetParameters(1,25,20, 40, 1000);
			textout.ReadCfgFile(dirName);
			textout.CalculateTextIsOut();
			textoutResult = textout.m_result;
			}
			
			if (isTable) jerkyResults = true; // disable jerky for tables

			sprintf	(g_resultString, "%s %s %s", 
			(slowLengthResults ? "slow_good" : "slow_bad"),
			(jerkyResults ? "jerky_good" : "jerky_bad"),
			(textoutResult? "textout_good" : "textout_bad"));
		
			



/*			char* fileNameOut = new char[fn->Length +100];
			for ( i = 0; i < fnOut->Length; i++)
			{
				fileNameOut[i] = (char) fnOut->Substring(i, 1)->ToCharArray()[0];
			}
			fileNameOut[i] = 0;
			FILE *fp = fopen(fileNameOut, "wb");
			
			WriteBMP(fp,output,PIC24,width,height,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp);
			pictureBox2->Width = width;
			pictureBox2->Height= height;
			pictureBox2->ImageLocation =fnOut;



			char* fileNameOut1 = new char[fn->Length +100];
			for ( i = 0; i < fnOut1->Length; i++)
			{
				fileNameOut1[i] = (char) fnOut1->Substring(i, 1)->ToCharArray()[0];
			}
			fileNameOut1[i] = 0;
			FILE *fp1 = fopen(fileNameOut1, "wb");
			
			WriteBMP(fp1,output1,PIC24,width,10,NULL,NULL, NULL,8,F_FULLCOLOR);
			fclose(fp1);
			pictureBox3->Width = width;
			pictureBox3->Height= 10;
			pictureBox3->ImageLocation =fnOut1;
*/
			if (input1)
			{
				delete [] input1;
				input1 = NULL;
			}
			if (input)
			{
				delete [] input;
				input = NULL;
			}
			if (input2)
			{
				delete [] input2;
				input2 = NULL;
			}

			if (image)
			{
				delete [] image;
				image = NULL;
			}
			if (imageOut)
			{
				delete [] imageOut;
				imageOut = NULL;
			}
			if (input_jerky)
			{
				delete [] input_jerky;
				input_jerky = NULL;
			}
			

			
			delete[] fileName;
//			delete[] fileNameOut;
			delete[] output;
//			delete[] fileNameOut1;
			delete[] output1;
			//delete[] pinfo.pic;
			delete[] dirName;


		return g_resultString;
	
}
