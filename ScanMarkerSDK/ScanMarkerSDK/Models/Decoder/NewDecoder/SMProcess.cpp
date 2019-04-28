/*
 *  pbmtojbg85 - Portable Bitmap to JBIG converter (T.85 version)
 *
 *  Markus Kuhn - http://www.cl.cam.ac.uk/~mgk25/jbigkit/
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "jbig85.h"
#include "SMProcess.h"
#ifdef _SIMULATE_SCANNER_COMPRESS__
#include "data_processing.h"
#endif
char *progname;                  /* global pointer to argv[0] */
static int currentJBig85BufferLength = 0;
static int maxJBig85BufferLength = 0;
static int numberBytesSentPerColumn = 8;

#ifdef _SIMULATE_SCANNER_COMPRESS__
FILE* fSimulateScannerCompresFile = NULL;
void ByteOutCallback(uint8_t byte)
{
	if ( !fSimulateScannerCompresFile);
	fwrite(&byte, 1, 1, fSimulateScannerCompresFile);
	int len = 1;
	currentJBig85BufferLength += len*8;
	if ( maxJBig85BufferLength < currentJBig85BufferLength)
	  maxJBig85BufferLength = currentJBig85BufferLength;

}
void BufferOutCallback(uint8_t* buffer, uint32_t size)
{
	if ( !fSimulateScannerCompresFile);
	fwrite(buffer, 1, size, fSimulateScannerCompresFile);
	int len = size;
	currentJBig85BufferLength += len*8;
	if ( maxJBig85BufferLength < currentJBig85BufferLength)
	  maxJBig85BufferLength = currentJBig85BufferLength;
}
void SimulateScannerCompress(byte * image, int width)
{
	DpsInit(ByteOutCallback, BufferOutCallback);
	DpsStart();
	for ( int i = 0; i < width; i++)
	{
		DpsProcessNewSample((uint8_t * )image, width);
		currentJBig85BufferLength -= numberBytesSentPerColumn;
		if (currentJBig85BufferLength < 0) 
			currentJBig85BufferLength = 0;
   }
}
#endif
/*
 * Print usage message and abort
 */



/*
 * malloc() with exception handler
 */
void *checkedmalloc(size_t n)
{
  void *p;
  
  if ((p = malloc(n)) == NULL) {
    fprintf(stderr, "Sorry, not enough memory available!\n");
    //exit(1);
  }
  
  return p;
}


/* 
 * Read an ASCII integer number from file f and skip any PBM
 * comments which are encountered.
 */
static unsigned long getint(FILE *f)
{
  int c;
  unsigned long i;

  while ((c = getc(f)) != EOF && !isdigit(c))
    if (c == '#')
      while ((c = getc(f)) != EOF && !(c == 13 || c == 10)) ;
  if (c != EOF) {
    ungetc(c, f);
    fscanf(f, "%lu", &i);
  }

  return i;
}


/*
 * Callback procedure which is used by JBIG encoder to deliver the
 * encoded data. It simply sends the bytes to the output file.
 */
int get_currentJBig85BufferLength()
{
	return currentJBig85BufferLength;
}
int get_maxJBig85BufferLength()
{
	return maxJBig85BufferLength;
}

static void data_out(unsigned char *start, size_t len, void *file)
{

  fwrite(start, len, 1, (FILE *) file);
#ifdef _SIMULATE_SCANNER_COMPRESS__
  currentJBig85BufferLength += len*8;
  if ( maxJBig85BufferLength < currentJBig85BufferLength)
	  maxJBig85BufferLength = currentJBig85BufferLength;
#endif
  return;
}

void getBitsLineFromImage(unsigned char* next_line, int y, int w, byte* image)
{
	for (int j = 0; j < 16; j++)
		next_line[j] = 0;

	for ( int j = 0; j < 128; j++)
	{
		byte bit = (image[j*w+y] == 255) ? 1 : 0;
		int jBit = j % 8;
		int jByte = j / 8;
		next_line[jByte] |= bit << ( 7 - jBit);
	}
}
int JBig85Compress(int w, int h, byte *image, char* fnout)
{
	currentJBig85BufferLength = 0;
	maxJBig85BufferLength = 0;

	FILE *fin = stdin, *fout = stdout;
  const char *fnin = NULL;//, *fnout = NULL;
  int i, j, c;
  int all_args = 0, files = 0;
  unsigned long x, y;
  unsigned long width, height;
  size_t bpl;
  char type;
  unsigned char *p, *lines, *next_line;
  unsigned char *prev_line = NULL, *prevprev_line = NULL;
  struct jbg85_enc_state s;
  int mx = -1;
  unsigned long l0 = 0, yi = 0, yr = 0;
  char *comment = NULL;
  int options = JBG_TPBON;
/*
  /* parse command line arguments * /
  progname = argv[0];
  for (i = 1; i < argc; i++) {
    if (!all_args && argv[i][0] == '-')
      if (argv[i][1] == 0) {
	if (files++) usage();
      } else
	for (j = 1; j > 0 && argv[i][j]; j++)
	  switch(argv[i][j]) {
	  case '-' :
	    all_args = 1;
	    break;
	  case 0 :
	    if (files++) usage();
	    break;
	  case 'Y':
	    if (i+2 >= argc) usage();
	    j = -1;
	    yi = atol(argv[++i]);
	    yr = atol(argv[++i]);
	    break;
	  case 'p':
	    if (++i >= argc) usage();
	    j = -1;
	    options = atoi(argv[i]);
	    break;
	  case 's':
	    if (++i >= argc) usage();
	    j = -1;
	    l0 = atol(argv[i]);
	    break;
	  case 'm':
	    if (++i >= argc) usage();
	    j = -1;
	    mx = atoi(argv[i]);
	    break;
	  case 'C':
	    if (++i >= argc) usage();
	    j = -1;
	    comment = argv[i];
	    break;
	  default:
	    usage();
	  }
    else
      switch (files++) {
      case 0: fnin  = argv[i]; break;
      case 1: fnout = argv[i]; break;
      default:
	usage();
      }
  }
  
  /* open input file * /
  if (fnin) {
    fin = fopen(fnin, "rb");
    if (!fin) {
      fprintf(stderr, "Can't open input file '%s", fnin);
      perror("'");
      exit(1);
    }
  } else
    fnin  = "<stdin>";

  /* read PBM header * /
  while ((c = getc(fin)) != EOF && (isspace(c) || c == '#'))
    if (c == '#')
      while ((c = getc(fin)) != EOF && !(c == 13 || c == 10)) ;
  type = getc(fin);
  if (c != 'P' || (type != '1' && type != '4')) {
    fprintf(stderr, "Input file '%s' does not look like a PBM file!\n", fnin);
    exit(1);
  }
  width = getint(fin);
  height = getint(fin);
  fgetc(fin);    /* skip line feed * /

  /* Test for valid parameters * /
  if (width < 1 || height < 1) {
    fprintf(stderr, "Image dimensions must be positive!\n");
    exit(1);
  }
  */
  /* allocate buffer for a single image line */
  width = h;
  height = w;
  l0 = 35;
  mx = 127;
  bpl = (width >> 3) + !!(width & 7);     /* bytes per line */
  lines = (unsigned char *) checkedmalloc(bpl * 3);
  if (!lines) return 0;
  /* open output file */
  if (fnout) {
    fout = fopen(fnout, "wb");
    if (!fout) {
      fprintf(stderr, "Can't open input file '%s", fnout);
      perror("'");
        free(lines);
      //exit(1);
      return 1;
    }
  } else
    fnout = "<stdout>";
  
  /* initialize parameter struct for JBIG encoder*/
  jbg85_enc_init(&s, width, yi ? yi : height, data_out, fout);

  /* Specify a few other options (each is ignored if negative) */
 /* if (yi)
    options |= JBG_VLENGTH;
  if (comment) {
    s.comment_len = strlen(comment);
    s.comment = (unsigned char *) comment;
  }
  */
  jbg85_enc_options(&s, options, l0, mx);

  for (y = 0; y < height; y++) {

    /* Use a 3-line ring buffer, because the encoder requires that the two
     * previously supplied lines are still in memory when the next line is
     * processed. */
    next_line = lines + (y%3)*bpl;
	getBitsLineFromImage(next_line, y, w, image);
	currentJBig85BufferLength -= numberBytesSentPerColumn;
	if (currentJBig85BufferLength < 0) 
		currentJBig85BufferLength = 0;
    /*switch (type) {
    case '1':
      /* PBM text format * /
      p = next_line;
      for (x = 0; x <= ((width-1) | 7); x++) {
	*p <<= 1;
	if (x < width)
	  *p |= getint(fin) & 1;
	if ((x & 7) == 7)
	  ++p;
      }
      break;
    case '4':
      /* PBM raw binary format  * /
      fread(next_line, bpl, 1, fin);
      break;
    default:
      fprintf(stderr, "Unsupported PBM type P%c!\n", type);
      exit(1);
    }
    if (ferror(fin)) {
      fprintf(stderr, "Problem while reading input file '%s", fnin);
      perror("'");
      exit(1);
    }
    if (feof(fin)) {
      fprintf(stderr, "Unexpected end of input file '%s'!\n", fnin);
      exit(1);
    }
*/
    /* JBIG compress another line and write out result via callback */
    jbg85_enc_lineout(&s, next_line, prev_line, prevprev_line);
    prevprev_line = prev_line;
    prev_line = next_line;

    /* adjust final image height via NEWLEN */
    if (yi && y == yr)
      jbg85_enc_newlen(&s, height);
  }

  /* check for file errors and close fout */
 if (ferror(fout) || fclose(fout)) {
    fprintf(stderr, "Problem while writing output file '%s", fnout);
    perror("'");
     free(lines);
     //exit(1);
     return 1;
 }

  return 0;
}
//decoder
//char *progname;                  /* global pointer to argv[0] */
unsigned long y_0;
fpos_t ypos;
int ypos_error = 1;
unsigned long ymax = 0;

/*
 * Print usage message and abort
 */
/*static void usage(void)
{
  fprintf(stderr, "JBIGtoPBM converter " JBG85_VERSION " (T.85 version) --\n"
	  "reads a bi-level image entity (BIE) as input file\n\n"
	  "usage: %s [<input-file> | -  [<output-file>]]\n\n", progname);
  fprintf(stderr, "options:\n\n"
	  "  -x number\tmaximum number of pixels per line for which memory\n"
	  "\t\tis allocated (default: 8192)\n"
          "  -y number\tinterrupt decoder after this number of lines\n"
	  "  -B number\tinput buffer size\n\n");
  exit(1);
}
*/

/*
 * Call-back routine for merged image output
 */
int line_out(const struct jbg85_dec_state *s,
	     unsigned char *start, size_t len, unsigned long y, void *file)
{
	
  fwrite(start, len, 1, (FILE *) file);
  return y == ymax - 1;
}


int JBig85Expand( char* fnin, char* fnout)
{
  FILE *fin = NULL, *fout = NULL;
//  const char /* *fnin = NULL,*/ *fnout = NULL;
  int i, j, result;
  int all_args = 0, files = 0;
  struct jbg85_dec_state s;
  unsigned char *inbuf, *outbuf;
  size_t inbuflen = 8192, outbuflen, len, cnt, cnt2;
  unsigned long xmax = 8192;
  size_t bytes_read = 0;

  
  inbuf = (unsigned char *) malloc(inbuflen);
  outbuflen = ((xmax >> 3) + !!(xmax & 7)) * 3;
  outbuf = (unsigned char *) malloc(outbuflen);
  if (!inbuf || !outbuf) {
    printf("Sorry, not enough memory available!\n");
      if (inbuf)
      {
          free(inbuf);
          inbuf = NULL;
      }
      if (outbuf)
      {
          free(outbuf);
          outbuf = NULL;
      }
      //exit(1);
      return 1;
  }

  if (fnin) {
    fin = fopen(fnin, "rb");
    if (!fin) {
      fprintf(stderr, "Can't open input file '%s", fnin);
      perror("'");
        if (inbuf)
        {
            free(inbuf);
            inbuf = NULL;
        }
        if (outbuf)
        {
            free(outbuf);
            outbuf = NULL;
        }
        //exit(1);
        return 1;
    }
  } else
    fnin  = "<stdin>";
  
  if (fnout) {
    fout = fopen(fnout, "wb");
    if (!fout) {
      fprintf(stderr, "Can't open input file '%s", fnout);
      perror("'");
        if (inbuf)
        {
            free(inbuf);
            inbuf = NULL;
        }
        if (outbuf)
        {
            free(outbuf);
            outbuf = NULL;
        }
        if (fin)
        {
            fclose(fin);
            fin = NULL;
        }
        //exit(1);
        return 1;
    }
  } else
    fnout = "<stdout>";
	
  /* send input file to decoder */
  jbg85_dec_init(&s, outbuf, outbuflen, line_out, fout);
  result = JBG_EAGAIN;
  while ((len = fread(inbuf, 1, inbuflen, fin))) {
    result = jbg85_dec_in(&s, inbuf, len, &cnt);
    bytes_read += cnt;
    while (result == JBG_EOK_INTR) {
      /* demonstrate decoder interrupt at given line number */
      printf("Decoding interrupted after %lu lines and %lu BIE bytes "
	     "... continuing ...\n", s.y, (unsigned long) bytes_read);
      /* and now continue decoding */
      result = jbg85_dec_in(&s, inbuf + cnt, len - cnt, &cnt2);
      bytes_read += cnt2;
      cnt += cnt2;
    }
    if (result != JBG_EAGAIN)
      break;
  }
  if (ferror(fin)) {
    fprintf(stderr, "Problem while reading input file '%s", fnin);
    perror("'");
    if (fout )
    {
        fclose(fout);
        fout = NULL;
    }
    if (fin)
    {
        fclose(fin);
        fin = NULL;
    }
    if (inbuf)
    {
        free(inbuf);
        inbuf = NULL;
    }
    if (outbuf)
    {
        free(outbuf);
        outbuf = NULL;
    }
      //exit(1);
    return 1;
  }
  if (result == JBG_EAGAIN || result == JBG_EOK_INTR) {
    /* signal end-of-BIE explicitely */
    result = jbg85_dec_end(&s);
    while (result == JBG_EOK_INTR) {
      /* demonstrate decoder interrupt at given line number */
      printf("Decoding interrupted after %lu lines and %lu BIE bytes "
	     "... continuing ...\n", s.y, (unsigned long) bytes_read);
      result = jbg85_dec_end(&s);
    }
  }
  if (result != JBG_EOK) {
    fprintf(stderr, "Problem with input file '%s': %s\n"
            "(error code 0x%02x, %lu = 0x%04lx BIE bytes "
	    "and %lu pixel rows processed)\n",
	    fnin, jbg85_strerror(result), result,
	    (unsigned long) bytes_read, (unsigned long) bytes_read, s.y);
      if (fout )
      {
          fclose(fout);
          fout = NULL;
      }
      if (fin)
      {
          fclose(fin);
          fin = NULL;
      }
      if (inbuf)
      {
          free(inbuf);
          inbuf = NULL;
      }
      if (outbuf)
      {
          free(outbuf);
          outbuf = NULL;
      }
      //exit(1);
      return 1;
  }
/*
  /* do we have to update the image height in the PBM header? * /
  if (!ypos_error && y_0 != jbg85_dec_getheight(&s)) {
    if (fsetpos(fout, &ypos) == 0) {
      fprintf(fout, "%10lu", jbg85_dec_getheight(&s)); /* pad to 10 bytes * /
    } else {
      fprintf(stderr, "Problem while updating height in output file '%s",
	      fnout);
      perror("'");
      exit(1);
    }
  }
  */
  /* check for file errors and close fout */

    if (fout )
    {
        fclose(fout);
        fout = NULL;
    }
    if (fin)
    {
        fclose(fin);
        fin = NULL;
    }
    if (inbuf)
    {
        free(inbuf);
        inbuf = NULL;
    }
    if (outbuf)
    {
        free(outbuf);
        outbuf = NULL;
    }
  return 0;
}
int GetImageFromFile( char* fnin, byte* image, int &width, int&height)
{
    int widthMax = 15000;
    int maxSize = widthMax*height/8;
    int widthRight = 0;
    FILE* fout = fopen(fnin, "rb");
    if (!fout)
    {
        width = 0;
        height = 0;
        return 0;
    }
    
    byte* imageTmp = new byte[maxSize];
    byte* currentLine = imageTmp;
    int size = 0;
    int rb = -1;
    while (rb != 0)
    {
        rb = fread(currentLine, 16, 1, fout);
        size += rb*16;
        currentLine += 16;
    }
    fclose(fout);
    widthRight = size / 16;
    for (int i = 0; i < widthRight; i++)
    {
        for ( int j = 0; j < height; j++)
        {
            image[j*widthRight+i] = 0;
        }
    }
    currentLine = imageTmp;
    for (int i = 0; i < widthRight; i++)
    {
        if ( i >= width) break;
        for ( int j = 0; j < height; j++)
        {
            int jj = height - j - 1;
            int jByte = j / 8;
            int jBit = j % 8;
            byte bit = (currentLine[jByte] >> (7-jBit)) & 0x01;
            image[jj*widthRight+i] = bit ? 255 : 0;
        }
        currentLine += 16;
    }
    width = widthRight;
    delete [] imageTmp;
    return true;
}
