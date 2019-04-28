//#define _SIMULATE_SCANNER_COMPRESS__
typedef unsigned char byte;
int JBig85Compress(int w, int h, byte *image, char* fnOut);
int JBig85Expand( char* fnin, char* fnout);
int GetImageFromFile( char* fnin, byte* image, int &width, int&height);
int get_currentJBig85BufferLength();
int get_maxJBig85BufferLength();
#ifdef _SIMULATE_SCANNER_COMPRESS__
	extern FILE* fSimulateScannerCompresFile;
	void SimulateScannerCompress(byte * image, int width);
#endif