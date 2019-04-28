class CFonts;
class HebrewFull
{
public:
	int m_widthRaw;
	int m_heightRaw;
	int m_width;
	int m_height;
	int m_aveHeightDefault;
	byte* m_dataRaw;
	byte* m_dataIn;
	byte* m_dataOut;
	byte* m_dataOut_1;
	int m_dataInMode;
	int m_dataOutMode;
	bool m_isDebugMode;
	char m_debugFileBasicName[1000];
	char m_dirName[1000];
	int m_width_2;
	int m_height_2;
	bool m_isWhiteBackground;
	bool m_directionFromLeftToRight;
	bool m_corierActive;
	int m_heightOfOutputBody;
	int m_language_In;
	float m_compressFactor_In;
	int m_italicStep;
	bool m_isVertical;
	bool m_isTableWithLines;
	CFonts *m_fonts;
	wchar_t m_hebrewResultStr[1000];
	HebrewFull();
	~HebrewFull();
	int Init(bool corierActive, int width, int height, byte* pDataIn, byte* pDataOut, byte* pDataOut1, bool debugMode,
		char* dirName, char* debugFileBasicName, bool directionFromLeftToRight);
	int Calculate( bool isOldStraight, int* param);
	void EnlargeImage(byte* dataIn, byte* dataOut, int width, int height, int &newWidth, int &newHeight);
	void CropImage(byte* dataIn, byte* dataOut, int width, int height, int &newWidth, int &newHeight);
	void UnSplit();
	void PrintDebugStr(char* str, bool append=true);
	void PrintTimeStr(char* str, int ms, bool append=true);
	void KoreanThick();
	void OneLineTry(int widthHist);
	void WriteDebugStr(char * str, bool isAppend=true);
	bool CheckIfShortScan();
	void FixWidthChina(byte* dataAfterStraight , int widthAfterStraight, int heightAfterStraight  );
	float CalculateLettersRatio();
	float ThresholdByLinesThickness();

};
