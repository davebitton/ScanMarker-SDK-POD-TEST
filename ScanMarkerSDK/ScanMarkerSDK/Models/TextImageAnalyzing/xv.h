/*
 *  xv.h  -  header file for xv, but you probably guessed as much
 * 
 *  Author:    John Bradley  (bradley@cis.upenn.edu)
 */

#include "copyright.h"
#include "config.h"


#define REVDATE   "Version 3.10a  Rev: 12/29/94 (PNG patch 1.2)"
#define VERSTR    "3.10a(PNG)"

/*
 * uncomment the following, and modify for your site, but only if you've
 * actually registered your copy of XV...
 */
#define REGSTR "Registered for use at the Massachussetts Institute of Technology"


/*
#ifndef VMS
#  define THUMBDIR     ".xvpics"  /* name of thumbnail file subdirectories * /
#  define THUMBDIRNAME ".xvpics"  /* same as THUMBDIR, unlike VMS case... * /
#  define CLIPFILE     ".xvclip"  /* name of clipboard file in home directory * /
#else
#  define THUMBDIR     "XVPICS"       /* name to use in building paths... * /
#  define THUMBDIRNAME "XVPICS.DIR"   /* name from readdir() & stat() * /
#  define CLIPFILE     "xvclipbd.dat"
#endif


#undef PARM
#ifdef __STDC__
#  define PARM(a) a
#else
#  define PARM(a) ()
#  define const
#endif



/*************************************************/
/* START OF MACHINE-DEPENDENT CONFIGURATION INFO */
/*************************************************/

/* Things to make xv more likely to just build, without the user tweaking
   the makefile * /

#ifdef hpux        /* HPUX machines (SVR3, (not SVR4) NO_RANDOM) * /
#  undef  SVR4
#  undef  SYSV
#  define SYSV
#  undef  NO_RANDOM
#  define NO_RANDOM
#  define USE_GETCWD
#endif


#ifdef sgi         /* SGI machines (SVR4) * /
#  undef  SVR4
#  define SVR4
#endif


#ifdef LINUX
#  ifndef _LINUX_LIMITS_H
#    include <linux/limits.h>
#  endif
#endif


//#include <X11/Xos.h>     /* need type declarations immediately */

/*********************************************************/


/* The BSD typedefs are used throughout.
 * If your system doesn't have them in <sys/types.h>,
 * then define BSDTYPES in your Makefile.
 * /
#if defined(BSDTYPES) || defined(VMS)
  typedef unsigned char  u_char;
  typedef unsigned short u_short;
  typedef unsigned int   u_int;
  typedef unsigned long  u_long;
#endif


#ifdef __UMAXV__              /* for Encore Computers UMAXV * /
#  include <sys/fs/b4param.h>   /* Get bsd fast file system params* /
#endif


/* things that *DON'T* have dirent.  Hopefully a very short list * /
#if defined(__UMAXV__)
#  ifndef NODIRENT
#    define NODIRENT
#  endif
#endif


/* include files */
#include <stdio.h>
#include <math.h>
#include <ctype.h>/*

#ifdef __STDC__
#  include <stddef.h>
#  include <stdlib.h>
#endif

/* note: 'string.h' or 'strings.h' is included by Xos.h, and it
   guarantees index() and rindex() will be available * /

#ifndef VMS
#  include <errno.h>
   extern int   errno;             /* SHOULD be in errno.h, but often isn't * /
#  if (! (defined(__NetBSD__) || defined(linux)))
     extern char *sys_errlist[];     /* this too... * /
#  endif
#endif


/* not everyone has the strerror() function, or so I'm told * /
#ifndef VMS
#  define ERRSTR(x) sys_errlist[x]
#else
#  define ERRSTR(x) strerror(x, vaxc$errno)
#endif




#ifdef VMS   /* VMS config, hacks & kludges * /
#  define MAXPATHLEN    512
#  define popUp xv_popup
#  define qsort xv_qsort
#  define random rand
#  define srandom srand
#  define cols xv_cols
#  define gmap xv_gmap
#  define index  strchr
#  define rindex strrchr
#  include <errno.h>
#  include <perror.h>
#endif


/* lots of things don't have <malloc.h> */
/* A/UX systems include it from stdlib, from Xos.h * /
#ifndef VMS   /* VMS hates multi-line '#if's * /
# if !defined(ibm032)                    && \
     !defined(__convex__)                && \
     !(defined(vax) && !defined(ultrix)) && \
     !defined(mips)                      && \
     !defined(apollo)                    && \
     !defined(pyr)                       && \
     !defined(__UMAXV__)                 && \
     !defined(bsd43)                     && \
     !defined(aux)                       && \
     !defined(__bsdi__)                  && \
     !defined(sequent)

#  if defined(hp300) || defined(hp800) || defined(NeXT)
#   include <sys/malloc.h>                /* it's in 'sys' on HPs and NeXT * /
#  else
#   include <malloc.h>
#  endif
# endif
#endif /* !VMS */



/*#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Intrinsic.h>
#include <X11/Xatom.h>
#include <X11/Xmd.h>

* /
#undef SIGCHLD           /* defined in both Xos.h and signal.h * /
#include <signal.h>      /* for interrupt handling * /

#include <sys/types.h>






#ifdef NEEDSTIME
#  include <sys/time.h>

#  ifdef _AIX
#    include <sys/select.h>   /* needed for select() call in Timer() * /
#  endif

#  ifdef SVR4
#    include <poll.h>      /* used in SVR4 version of Timer() * /
#  endif

#  ifdef sgi               /* need 'CLK_TCK' value for sginap() call * /
#    include <limits.h>
#  endif

/*** for select() call *** /
#  ifdef __hpux
#    define XV_FDTYPE (int *)
#  else
#    define XV_FDTYPE (fd_set *)
#  endif

#endif  /* NEEDSTIME * /



#ifdef NEEDSDIR
#  ifdef VMS
#    include <descrip.h>
#    include <stat.h>
#    include "dirent.h"
#  else
#    ifdef NODIRENT
#      include <sys/dir.h>
#    else
#      include <dirent.h>
#    endif

#    if defined(SVR4) || defined(SYSV)
#      include <fcntl.h>
#    endif

#    include <sys/param.h>
#    include <sys/stat.h>

#    if defined(__convex__) && defined (__STDC__)
#      define S_IFMT  _S_IFMT
#      define S_IFDIR _S_IFDIR
#      define S_IFCHR _S_IFCHR
#      define S_IFBLK _S_IFBLK
#    endif
#  endif
#endif


#ifdef NEEDSARGS
#  if defined(__STDC__) && !defined(NOSTDHDRS)
#    include <stdarg.h>
#  else
#    include <varargs.h>
#  endif
#endif



/* Use S_ISxxx macros in stat-related stuff
 * make them if missing, along with a few fictitious ones
 *      Cameron Simpson  (cameron@cse.unsw.edu.au)
 * /
 
#ifndef         S_ISDIR         /* missing POSIX-type macros * /
#  define       S_ISDIR(mode)   (((mode)&S_IFMT) == S_IFDIR)
#  define       S_ISBLK(mode)   (((mode)&S_IFMT) == S_IFBLK)
#  define       S_ISCHR(mode)   (((mode)&S_IFMT) == S_IFCHR)
#  define       S_ISREG(mode)   (((mode)&S_IFMT) == S_IFREG)
#endif
#ifndef         S_ISFIFO
#  ifdef        S_IFIFO
#    define     S_ISFIFO(mode)  (((mode)&S_IFMT) == S_IFIFO)
#  else
#    define     S_ISFIFO(mode)  0
#  endif
#endif
#ifndef         S_ISLINK
#  ifdef        S_IFLNK
#    define     S_ISLINK(mode)  (((mode)&S_IFMT) == S_IFLNK)
#  else
#    define     S_ISLINK(mode)  0
#  endif
#endif
#ifndef         S_ISSOCK
#  ifdef        S_IFSOCK
#    define     S_ISSOCK(mode)  (((mode)&S_IFMT) == S_IFSOCK)
#  else
#    define     S_ISSOCK(mode)  0
#  endif
#endif



#ifndef MAXPATHLEN
#  define MAXPATHLEN 256
#endif


#ifdef SVR4
#  define random lrand48
#  define srandom srand48
#else
#  if defined(NO_RANDOM) || (defined(sun) && defined(SYSV))
#    define random()   rand()
#    define srandom(x) srand(x)
#  endif
#endif


#ifndef VMS       /* VMS hates multi-line definitions * /
#  if defined(SVR4)  || defined(SYSV) || defined(sco) || \
      defined(XENIX) || defined(__osf__) 
#    undef  USE_GETCWD
#    define USE_GETCWD          /* use 'getcwd()' instead of 'getwd()' * /
#  endif
#endif


/*****************************/
/* END OF CONFIGURATION INFO */
/***************************** /

#ifdef DOJPEG
#define HAVE_JPEG
#endif

#ifdef DOTIFF
#define HAVE_TIFF
#endif

#ifdef DOPNG
#define HAVE_PNG
#endif

#ifdef DOPDS
#define HAVE_PDS
#endif



#define PROGNAME  "xv"             /* used in resource database * /

#define MAXNAMES 4096              /* max # of files in ctrlW list * /

#define MAXBRWIN   4               /* max # of vis browser windows * /

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


/* buttons in the ctrl window */
#define BNEXT    0
#define BPREV    1
#define BLOAD    2
#define BSAVE    3
#define BDELETE  4
#define BPRINT   5

#define BCOPY    6
#define BCUT     7
#define BPASTE   8
#define BCLEAR   9
#define BGRAB    10
#define BUP10    11
#define BDN10    12
#define BROTL    13
#define BROTR    14
#define BFLIPH   15
#define BFLIPV   16
#define BPAD     17
#define BANNOT   18
#define BCROP    19
#define BUNCROP  20
#define BACROP   21
#define BABOUT   22
#define BQUIT    23
#define BXV      24
#define NBUTTS   25    /* # of butts */


/* buttons in the load/save window */
#define S_LOAD_NBUTTS  4
#define S_NBUTTS   5
#define S_BOK      0
#define S_BCANC    1
#define S_BRESCAN  2
#define S_BLOADALL 3
#define S_BOLDSET  3
#define S_BOLDNAM  4


/* buttons in the 'gamma' window */
#define G_NBUTTS   24
#define G_BAPPLY   0
#define G_BNOGAM   1
#define G_BRESET   2
#define G_BCLOSE   3
#define G_BUP_BR   4
#define G_BDN_BR   5
#define G_BUP_CN   6
#define G_BDN_CN   7
#define G_B1       8
#define G_B2       9
#define G_B3       10
#define G_B4       11
#define G_BSET     12
#define G_BUNDO    13
#define G_BREDO    14
#define G_BCOLREV  15
#define G_BRNDCOL  16
#define G_BHSVRGB  17
#define G_BCOLUNDO 18
#define G_BRV      19
#define G_BMONO    20
#define G_BMAXCONT 21
#define G_BGETRES  22
#define G_BHISTEQ  23


/* constants for setting default 'save mode' in dirW */
#define F_COLORS    0
#define F_FORMAT    1

/* the following list give indicies into saveColors[] array in xvdir.c */
#define F_FULLCOLOR 0
#define F_GREYSCALE 1
#define F_BWDITHER  2
#define F_REDUCED   3
#define F_MAXCOLORS 4   /* length of saveColors[] array */


/* following list gives indicies into 'saveFormats[]' array in xvdir.c
   note that JPEG and TIFF entries may or may not exist, and following
   constants have to be adjusted accordingly.  Also, don't worry about 
   duplicate cases if JPGINC or TIFINC = 0.  All code that references
   F_JPEG or F_TIFF is #ifdef'd, so it won't be a problem */

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



/* return values from ReadFileType()
 * positive values are *definitely* readable formats (HAVE_*** is defined)
 * negative values are random files that XV can't read, but display as
 *   different icons in the visual browser 
 */
#define RFT_ERROR    -1    /* couldn't open file, or whatever... */
#define RFT_UNKNOWN   0
#define RFT_GIF       1
#define RFT_PM        2
#define RFT_PBM       3
#define RFT_XBM       4
#define RFT_SUNRAS    5
#define RFT_BMP       6
#define RFT_UTAHRLE   7
#define RFT_IRIS      8
#define RFT_PCX       9
#define RFT_JFIF     10
#define RFT_TIFF     11
#define RFT_PDSVICAR 12
#define RFT_COMPRESS 13
#define RFT_PS       14
#define RFT_IFF      15
#define RFT_TARGA    16
#define RFT_XPM      17
#define RFT_XWD      18
#define RFT_FITS     19
#define RFT_PNG      20

/* definitions for page up/down, arrow up/down list control */
#define LS_PAGEUP   0
#define LS_PAGEDOWN 1
#define LS_LINEUP   2
#define LS_LINEDOWN 3
#define LS_HOME     4
#define LS_END      5


/* values returned by CursorKey() */
#define CK_NONE     0
#define CK_LEFT     1
#define CK_RIGHT    2
#define CK_UP       3
#define CK_DOWN     4
#define CK_PAGEUP   5
#define CK_PAGEDOWN 6
#define CK_HOME     7
#define CK_END      8


/* values 'epicMode' can take */
#define EM_RAW    0
#define EM_DITH   1
#define EM_SMOOTH 2


/* things EventLoop() can return (0 and above reserved for 'goto pic#') */
#define QUIT      -1   /* exit immediately  */
#define NEXTPIC   -2   /* goto next picture */
#define PREVPIC   -3   /* goto prev picture */
#define NEXTQUIT  -4   /* goto next picture, quit if none (used by 'wait') */
#define LOADPIC   -5   /* load 'named' pic (from directory box) */
#define NEXTLOOP  -6   /* load next pic, loop if we're at end */
#define DFLTPIC   -7   /* load the default image */
#define DELETEPIC -8   /* just deleted pic.  load 'right' thing */
#define GRABBED   -9   /* just grabbed a pic.  'load' it up */
#define POLLED    -10  /* polling, and image file has changed... */
#define RELOAD    -11  /* 'reload' interrupt came. be happier about errors */
#define THISNEXT  -12  /* load 'current' selection, Next until success */
#define OP_PAGEUP -13  /* load previous page of multi-page document */
#define OP_PAGEDN -14  /* load next page of multi-page document */
#define PADDED    -15  /* just grabbed a pic.  'load' it up */


/* possible values of 'rootMode' */
#define RM_NORMAL  0     /* default X tiling */
#define RM_TILE    1     /* integer tiling */
#define RM_MIRROR  2     /* mirror tiling */
#define RM_IMIRROR 3     /* integer mirror tiling */
#define RM_CENTER  4     /* modes >= RM_CENTER centered on some sort of bg */
#define RM_CENTILE 4     /* centered and tiled.  NOTE: equals RM_CENTER */
#define RM_CSOLID  5     /* centered on a solid bg */
#define RM_CWARP   6     /* centered on a 'warp-effect' bg */
#define RM_CBRICK  7     /* centered on a 'brick' bg */
#define RM_ECENTER 8     /* symmetrical tiled */
#define RM_ECMIRR  9     /* symmetrical mirror tiled */
#define RM_MAX     RM_ECMIRR


/* values of colorMapMode */
#define CM_NORMAL    0        /* normal RO or RW color allocation */
#define CM_PERFECT   1        /* install own cmap if necessary */
#define CM_OWNCMAP   2        /* install own cmap always */
#define CM_STDCMAP   3        /* use stdcmap */


/* values of haveStdCmap */
#define STD_NONE     0        /* no stdcmap currently defined */
#define STD_111      1        /* 1/1/1 stdcmap is available */
#define STD_222      2        /* 2/2/2 stdcmap is available */
#define STD_232      3        /* 2/3/2 stdcmap is available */
#define STD_666      4        /* 6x6x6 stdcmap is available */
#define STD_332      5        /* 3/3/2 stdcmap is available */


/* values of allocMode */
#define AM_READONLY  0
#define AM_READWRITE 1


/* selections in dispMB */
#define DMB_RAW      0
#define DMB_DITH     1
#define DMB_SMOOTH   2
#define DMB_SEP1     3     /* ---- separator */
#define DMB_COLRW    4
#define DMB_SEP2     5     /* ---- separator */
#define DMB_COLNORM  6
#define DMB_COLPERF  7
#define DMB_COLOWNC  8
#define DMB_COLSTDC  9
#define DMB_MAX      10


/* selections in rootMB */
#define RMB_WINDOW   0
#define RMB_ROOT     1
#define RMB_TILE     2
#define RMB_MIRROR   3
#define RMB_IMIRROR  4
#define RMB_CENTILE  5
#define RMB_CSOLID   6
#define RMB_CWARP    7
#define RMB_CBRICK   8
#define RMB_ECENTER  9
#define RMB_ECMIRR   10
#define RMB_MAX      11


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

/* indicies into algMB */
#define ALG_NONE      0
#define ALG_SEP1      1  /* separator */
#define ALG_BLUR      2
#define ALG_SHARPEN   3
#define ALG_EDGE      4
#define ALG_TINF      5
#define ALG_OIL       6
#define ALG_BLEND     7
#define ALG_ROTATE    8
#define ALG_ROTATECLR 9
#define ALG_PIXEL     10
#define ALG_SPREAD    11
#define ALG_MEDIAN    12
#define ALG_MAX       13


/* indicies into sizeMB */
#define SZMB_NORM     0
#define SZMB_MAXPIC   1
#define SZMB_MAXPECT  2
#define SZMB_DOUBLE   3
#define SZMB_HALF     4
#define SZMB_P10      5
#define SZMB_M10      6
#define SZMB_SEP      7   /* separator */
#define SZMB_SETSIZE  8
#define SZMB_ASPECT   9
#define SZMB_4BY3     10
#define SZMB_INTEXP   11
#define SZMB_MAX      12

/* indicies into windowMB */
#define WMB_BROWSE    0
#define WMB_COLEDIT   1
#define WMB_INFO      2
#define WMB_COMMENT   3
#define WMB_TEXTVIEW  4
#define WMB_SEP       5  /* separator */
#define WMB_ABOUTXV   6
#define WMB_KEYHELP   7
#define WMB_MAX       8


/* definitions of first char of dirnames[i] (filetype) */
#define C_FIFO  'f'    /* FIFO special file */
#define C_CHR   'c'    /* character special file */
#define C_DIR   'd'    /* directory */
#define C_BLK   'b'    /* block special file */
#define C_LNK   'l'    /* symbolic link */
#define C_SOCK  's'    /* socket */
#define C_REG   ' '    /* regular file */
#define C_EXE   'x'    /* executable file */


/* values used in Draw3dRect() */
#define R3D_OUT 0  /* rect sticks 'out' from screen */
#define R3D_IN  1  /* rect goes 'in' screen */


/* values 'GetSelType()' (in xvcut.c) can return */
#define SEL_RECT 0

/* mode values for PadPopUp() */
#define PAD_SOLID 0
#define PAD_BGGEN 1
#define PAD_LOAD  2

#define PAD_ORGB  0
#define PAD_OINT  1
#define PAD_OHUE  2
#define PAD_OSAT  3
#define PAD_OMAX  4

/* byte offsets into a 'cimg' (clipboard image) array (SaveToClip()) */
#define CIMG_LEN   0              /* offset to 4-byte length of data */
#define CIMG_W     4              /* offset to 2-byte width of image */
#define CIMG_H     6              /* offset to 2-byte height of image */
#define CIMG_24    8              /* offset to 1-byte 'is24bit?' field */
#define CIMG_TRANS 9              /* offset to 1-byte 'has transparent?' */
#define CIMG_TRVAL 10             /* if trans && !24: trans. pixel val */
#define CIMG_TRR   11             /* if trans && 24: red val of trans */
#define CIMG_TRG   12
#define CIMG_TRB   13
#define CIMG_PIC24 14             /* offset to data(24-bit) */
#define CIMG_CMAP  14             /* offset to cmap (8-bit) */
#define CIMG_PIC8 (CIMG_CMAP + 3*256)   /* offset to data (8-bit) */



#define MBSEP "\001"   /* special string for a --- separator in MBUTT */

/* random string-placing definitions */
#define SPACING 3      /* vertical space between strings */
#define ASCENT   (mfinfo->ascent)
