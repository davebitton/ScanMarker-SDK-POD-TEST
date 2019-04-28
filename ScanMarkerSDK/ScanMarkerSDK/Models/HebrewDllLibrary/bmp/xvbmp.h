#ifndef _XVBMP_H_
#define _XVBMP_H_
#ifdef _SCANMARKER_WINDOWS_
#include <windows.h>
#else
#include "../Include/types_hebrewdll.h"
#endif
#include <stdio.h>
#define PARM(a)  a



/* MONO returns total intensity of r,g,b triple (i = .33R + .5G + .17B) */
#define MONO(rd,gn,bl) ( ((int)(rd)*11 + (int)(gn)*16 + (int)(bl)*5) >> 5)
/* strings in the INFOBOX (used in SetISTR and GetISTR) */
#define NISTR         10    /* number of ISTRs */
#define ISTR_INFO     0
#define ISTR_WARNING  1
#define ISTR_FILENAME 2
#define ISTR_FORMAT   3
#define ISTR_RES      4
#define ISTR_CROP     5
#define ISTR_EXPAND   6
#define ISTR_SELECT   7
#define ISTR_COLOR    8
#define ISTR_COLOR2   9

/* potential values of 'infomode', used in info box drawing routines */
#define INF_NONE 0    /* empty box */
#define INF_STR  1    /* just ISTR_INFO */
#define INF_PART 2    /* filename, format, size and infostr */
#define INF_FULL 3    /* INF_PART + clipping, expansion, colorinfo */


/*************************** XVINFO.C ***************************/
void  CreateInfo           PARM((char *));
void  InfoBox              PARM((int));
void  RedrawInfo           PARM((int, int, int, int));
void  SetInfoMode          PARM((int));
char *GetISTR              PARM((int));

//#if defined(__STDC__) && !defined(NOSTDHDRS)
//void  SetISTR(int, ...);
//#else
//void  SetISTR();
//#endif

#define SetISTR(a)   printf(a)






/* constants for setting default 'save mode' in dirW */
#define F_COLORS    0
#define F_FORMAT    1

/* the following list give indicies into saveColors[] array in xvdir.c */
#define F_FULLCOLOR 0
#define F_GREYSCALE 1
#define F_BWDITHER  2
#define F_REDUCED   3
#define F_MAXCOLORS 4   /* length of saveColors[] array */


#ifdef HAVE_JPEG
#define F_JPGINC  1
#else
#define F_JPGINC  0
#endif

#ifdef HAVE_TIFF
#define F_TIFINC  1
#else
#define F_TIFINC  0
#endif

#ifdef HAVE_PNG
#define F_PNGINC  1
#else
#define F_PNGINC  0
#endif


#define F_GIF         0
#define F_JPEG      ( 0 + F_JPGINC)
#define F_TIFF      ( 0 + F_JPGINC + F_TIFINC)
#define F_PNG       ( 0 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_PS        ( 1 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_PBMRAW    ( 2 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_PBMASCII  ( 3 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_XBM       ( 4 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_XPM       ( 5 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_BMP       ( 6 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_SUNRAS    ( 7 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_IRIS      ( 8 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_TARGA     ( 9 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_FITS      (10 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_PM        (11 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_DELIM1    (12 + F_JPGINC + F_TIFINC + F_PNGINC)   /* ----- */
#define F_FILELIST  (13 + F_JPGINC + F_TIFINC + F_PNGINC)
#define F_MAXFMTS   (14 + F_JPGINC + F_TIFINC + F_PNGINC)   /* 17, normally */









/* indicies into conv24MB */
#define CONV24_8BIT  0
#define CONV24_24BIT 1
#define CONV24_SEP1  2
#define CONV24_LOCK  3
#define CONV24_SEP2  4
#define CONV24_FAST  5
#define CONV24_SLOW  6
#define CONV24_BEST  7
#define CONV24_MAX   8
#define CONV32_DINA  9

/* values 'picType' can take */
#define PIC8  CONV24_8BIT
#define PIC24 CONV24_24BIT
#define PIC32 CONV32_DINA

typedef struct { byte *pic;                  /* image data */
		 int   w, h;                 /* pic size */
		 int   type;                 /* PIC8 or PIC24 or PIC32*/

		 byte  r[256],g[256],b[256];
		                             /* colormap, if PIC8 */

		 int   normw, normh;         /* 'normal size' of image file
					        (normally eq. w,h, except when
						doing 'quick' load for icons */

		 int   frmType;              /* def. Format type to save in */
		 int   colType;              /* def. Color type to save in */
		 char  fullInfo[128];        /* Format: field in info box */
		 char  shrtInfo[128];        /* short format info */
		 char *comment;              /* comment text */

		 int   numpages;             /* # of page files, if >1 */
		 char  pagebname[64];        /* basename of page files */
		 unsigned int biBitCount;
	       } PICINFO;




int LoadBMP(char    *fname,PICINFO *pinfo);
int WriteBMP(FILE* fp,byte* pic824,int ptype,int w,int h,byte* rmap,byte *gmap,byte* bmap,int numcols,int colorstyle);




/* function declarations for externally-callable functions */

/****************************** XV.C ****************************/
int   ReadFileType      PARM((char *));
int   ReadPicFile       PARM((char *, int, PICINFO *, int));
int   UncompressFile    PARM((char *, char *));
void  KillPageFiles     PARM((char *, int));

void NewPicGetColors    PARM((int, int));
void FixAspect          PARM((int, int *, int *));
void ActivePrevNext     PARM((void));
int  DeleteCmd          PARM((void));
void StickInCtrlList    PARM((int));
void AddFNameToCtrlList PARM((char *, char *));
void ChangedCtrlList    PARM((void));
void HandleDispMode     PARM((void));
char *lower_str         PARM((char *));
int  rd_int             PARM((char *));
int  rd_str             PARM((char *));
int  rd_flag            PARM((char *));
int  rd_str_cl          PARM((char *, char *, int));


char *BaseName              PARM((char *));
FILE *xv_fopen              PARM((char *, char *));

#endif