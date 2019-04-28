#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "Include/types_hebrewdll.h"
#endif
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <complex>


#define BgColor 0
#define TextColor 255
#define _MAX_IMAGE_HEIGHT_  300
#define _MAX_NUMBER_LETTERS_ 1000
#define FORIE_FACTOR 13

#pragma once

class CStraightWithPolynom1
{
public:
	CStraightWithPolynom1(void);
	~CStraightWithPolynom1(void);

	void Init(int width, int height, int outputWidth,byte* pDataIn, byte* pDataOut, byte* pDataIn3=NULL);
	void ReadCfgFile(char* dir);
	void AllocateMemory();
	void CalculateImageBoundaries();
	void DrawOutput(void* values, int valuesType, int red, int green, int blue, float minValue = -10000, float maxValue=-10000);
	void PrintDebugStr(char* str, bool append=true);
	void PrintTimeStr(char* str, int ms, bool append=true);
	void PreparePointsForPolynom(int * points);
	void RemoveHorizontalLines();

	bool Calculate(char* filenamePolynom);
	bool Calculate_Old(bool isOldStraight, char* filenamePolynom);
	void ProcessUp();
	void ProcessDown();
	void CalculatePolinom(int * points, int * pointsOut);
	void MinLeastSquare(int * points, int * pointsOut, int size);
	double CalcDeterminant( double  **mat, int order);
	void Clamping();
	bool FindNextWord(int i, int& beginWord, int&endWord);
	int GetMinor(double  **src, double **dest, int row, int col, int order);
	bool CheckIfShortScan();
	int AddHeightsToLetters();
	int AddHeightsToLetters1();
	void RemoveGarbageAtBeginAndFinish();


	int m_clamping[_MAX_NUMBER_LETTERS_];
	//void CalculateBody();
	/*void FindMatrixOpposite(int N, float *matrix1, float *matrix2);
	void FilterLightPoints(int * points, byte* image);
	void MatrixInversion(std::complex<double> ** A, int order, 
										 std::complex<double> ** Y);
	int FilterPointsAfterFouirer( int * points, int * pointsOut);
	int FourierTransform_1( int * points, int * pointsOut, std::complex<double> * Fk);
	int FourierTransform_2( int * points, int * pointsOut, std::complex<double> * Fk);
	int FinishSmoothing(int * points, int * pointsOut, std::complex<double> * Fk);
	*/
	int _LOW_Y_;
	int m_width;
	int m_height;
	int m_widthRaw;
	int m_heightRaw;
	int m_output_width;
	byte* m_pDataRaw;
	byte *m_pDataIn;
	byte* m_pDataIn3;
	byte *m_pDataOut;
	byte *m_pWorkData1;
	int *m_pWorkData2;
	int *m_pWorkData3;
	int *m_pWorkData4;
    int m_pWorkLData[50];
	int *m_pBody;
	int *m_body;
	int *m_bodyHeight;
	int m_begin_x;
	int m_end_x;
	int m_begin_y;
	int m_end_y;
	int m_To;
	int m_start_x;
	float* m_pInterpolationOutput;
	float *m_pInterpolationInput;
	int m_aveBodyHeight;
	int m_begin_x_1;
	int m_end_x_1;
	//int *m_pChangesCoord[_MAX_IMAGE_HEIGHT_];
	//bool _KOREAN_THICK_;
	int m_heightOfOutputBody;
	int m_numberLetters;
	int _REFERENCE_BODY_HEIGHT_;
	int m_aveHeightDefault;
	bool m_isTable;
	int m_language;
	bool m_doNothing;
	bool m_isVertical;

    int _NUMBER_CHANGES_TO_ANALYZE_;
	int _UP_EDGE_;
	int _DOWN_EDGE_;


	int m_letterEdges[_MAX_NUMBER_LETTERS_][4];
	int m_letterPointUp[_MAX_NUMBER_LETTERS_][2];
	int m_letterPointDown[_MAX_NUMBER_LETTERS_][2];
	int m_letterHeight[_MAX_NUMBER_LETTERS_];

	void CalculateLetterEdges();
	int CheckConnectedLetters( int i1, int i2, int j1, int j2);
	void ConnectLetters();
	void CalculateLetterEdges_CMC7();
	void CalculateLetterEdgesVerticalAsia();
	void FixFilledSpaces();



};
