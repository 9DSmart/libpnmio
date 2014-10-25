/*
 * File       : pnmio.c                                                          
 * Description: I/O facilities for PBM, PGM, PPM (PNM) ASCII images.
 * Author     : Nikolaos Kavvadias <nikolaos.kavvadias@gmail.com>                
 * Copyright  : (C) Nikolaos Kavvadias 2012, 2013, 2014
 * Website    : http://www.nkavvadias.com                            
 *                                                                          
 * This file is part of libpnmio, and is distributed under the terms of the  
 * Modified BSD License.
 *
 * A copy of the Modified BSD License is included with this distribution 
 * in the file LICENSE.
 * libpnmio is free software: you can redistribute it and/or modify it under the
 * terms of the Modified BSD License. 
 * libpnmio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Modified BSD License for more details.
 * 
 * You should have received a copy of the Modified BSD License along with 
 * libpnmio. If not, see <http://www.gnu.org/licenses/>. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "pnmio.h"

#define  MAXLINE         1024
#define  LITTLE_ENDIAN     -1
#define  BIG_ENDIAN         1
#define  GREYSCALE_TYPE     0 /* used for PFM */
#define  RGB_TYPE           1 /* used for PFM */ 


/* read_pbm_header:
 * Read the header contents of a PBM (portable bit map) file.
 * An ASCII PBM image file follows the format:
 * P1
 * <X> <Y>
 * <I1> <I2> ... <IMAX>
 * A binary PBM image file uses P4 instead of P1 and 
 * the data values are represented in binary. 
 * NOTE1: Comment lines start with '#'.
 # NOTE2: < > denote integer values (in decimal).
 */
void read_pbm_header(FILE *f, int *img_xdim, int *img_ydim, int is_ascii)
{
  int c;
  int x_val, y_val;
  char dummy_string[48];
  char line[MAXLINE];

  /* Read the magic number string. */
  fscanf(f, "%s", dummy_string);
  if (((strcmp(dummy_string, "P1") != 0) && (is_ascii == 1)) ||
      ((strcmp(dummy_string, "P4") != 0) && (is_ascii == 0))) {
    fprintf(stderr, "Error: Input file not in PBM format!\n");
    exit(1);
  }
   
  /* Read the rest of the PBM file header. */
  while ((c = fgetc(f)) != EOF) {
    /* Detect a comment line. */
    if (c == '#') {
      /* Parse and omit the contents of the comment line. */
      if (fgets(line, MAXLINE, f) != NULL) {
        ;  
	  }
      break;
    } else if (isdigit(c)) {
      ungetc(c, f);
      break;
    }
  }
  fscanf(f, "%d %d", &x_val, &y_val);
  *img_xdim = x_val;
  *img_ydim = y_val;
}

/* read_pgm_header:
 * Read the header contents of a PGM (portable grey map) file.
 * An ASCII PGM image file follows the format:
 * P2
 * <X> <Y> 
 * <levels>
 * <I1> <I2> ... <IMAX>
 * A binary PGM image file uses P5 instead of P2 and 
 * the data values are represented in binary. 
 * NOTE1: Comment lines start with '#'.
 # NOTE2: < > denote integer values (in decimal).
 */
void read_pgm_header(FILE *f, int *img_xdim, int *img_ydim, int *img_colors, int is_ascii)
{
  int c;
  int x_val, y_val, maxcolors_val;
  char dummy_string[48];
  char line[MAXLINE];

  /* Read the magic number string. */
  fscanf(f, "%s", dummy_string);
  if (((strcmp(dummy_string, "P2") != 0) && (is_ascii == 1)) ||
      ((strcmp(dummy_string, "P5") != 0) && (is_ascii == 0))) {
    fprintf(stderr, "Error: Input file not in PGM format!\n");
    exit(1);
  }
  
  /* Read the rest of the PPM file header. */
  while ((c = fgetc(f)) != EOF) {
    /* Detect a comment line. */
    if (c == '#') {
      /* Parse and omit the contents of the comment line. */
      if (fgets(line, MAXLINE, f) != NULL) {
        ;  
	  }
      break;
    } else if (isdigit(c)) {
      ungetc(c, f);
      break;
    }
  }
  fscanf(f, "%d %d %d", &x_val, &y_val, &maxcolors_val);
  *img_xdim   = x_val;
  *img_ydim   = y_val;
  *img_colors = maxcolors_val;
}

/* read_ppm_header:
 * Read the header contents of a PPM (portable pix map) file.
 * An ASCII PPM image file follows the format:
 * P3
 * <X> <Y> 
 * <colors>
 * <R1> <G1> <B1> ... <RMAX> <GMAX> <BMAX>
 * A binary PPM image file uses P6 instead of P3 and 
 * the data values are represented in binary. 
 * NOTE1: Comment lines start with '#'.
 # NOTE2: < > denote integer values (in decimal).
 */
void read_ppm_header(FILE *f, int *img_xdim, int *img_ydim, int *img_colors, int is_ascii)
{
  int c;
  int x_val, y_val, maxcolors_val;
  char dummy_string[48];
  char line[MAXLINE];

  /* Read the magic number string. */
  fscanf(f, "%s", dummy_string);
  if (((strcmp(dummy_string, "P3") != 0) && (is_ascii == 1)) ||
      ((strcmp(dummy_string, "P6") != 0) && (is_ascii == 0))) {
    fprintf(stderr, "Error: Input file not in PPM format!\n");
    exit(1);
  }
  
  /* Read the rest of the PPM file header. */
  while ((c = fgetc(f)) != EOF) {
    /* Detect a comment line. */
    if (c == '#') {
      /* Parse and omit the contents of the comment line. */
      if (fgets(line, MAXLINE, f) != NULL) {
        ;  
	  }
      break;
    } else if (isdigit(c)) {
      ungetc(c, f);
      break;
    }
  }
  fscanf(f, "%d %d %d", &x_val, &y_val, &maxcolors_val);
  *img_xdim   = x_val;
  *img_ydim   = y_val;
  *img_colors = maxcolors_val;
}

/* read_pfm_header:
 * Read the header contents of a PFM (portable float map) file.
 * A PFM image file follows the format:
 * [PF|Pf]
 * <X> <Y> 
 * (endianess)
 * {R1}{G1}{B1} ... {RMAX}{GMAX}{BMAX} 
 * NOTE1: Comment lines, if allowed, start with '#'.
 # NOTE2: < > denote integer values (in decimal).
 # NOTE3: ( ) denote floating-point values (in decimal).
 # NOTE4: { } denote floating-point values (coded in binary).
 */
void read_pfm_header(FILE *f, int *img_xdim, int *img_ydim, int *img_type, int *endianess)
{
  /* Union definition for extracting s|m|e of a float. */
  typedef union {
    float f;
    struct {
      unsigned int m : 23; /* mantissa */
      unsigned int e :  8; /* exponent */
      unsigned int s :  1; /* sign     */
    } field;
  } float_type;
  int c;
  int x_val, y_val;
  int is_rgb=0, is_greyscale=0;
  float_type aspect_ratio;
  char dummy_string[48];
  char line[MAXLINE];

  /* Read the magic number string. */
  fscanf(f, "%s", dummy_string);
  if ((strcmp(dummy_string, "PF") != 0) ||
      (strcmp(dummy_string, "Pf") != 0)) {
    fprintf(stderr, "Error: Input file not in PFM format!\n");
    exit(1);
  }

  if (dummy_string[1] == 'F') {
    is_rgb = 1;
  } else if (dummy_string[1] == 'f') {
    is_greyscale = 1;
  } else {
    fprintf(stderr, "Error: Invalid identifier line: %s\n", dummy_string);
    exit(1);
  }        

  /* Read the rest of the PFM file header. */
  while ((c = fgetc(f)) != EOF) {
    /* Detect a comment line. */
    if (c == '#') {
      /* Parse and omit the contents of the comment line. */
      if (fgets(line, MAXLINE, f) != NULL) {
        ;  
      }
      break;
    } else if (isdigit(c)) {
      ungetc(c, f);
      break;
    }
  }
  fscanf(f, "%d", &x_val);
  fscanf(f, "%d", &y_val);
  fscanf(f, "%f", &aspect_ratio.f);

  /* FIXME: Aspect ratio different to 1.0 is not yet supported. */
  if (!floatEqualComparison(aspect_ratio.f, 1.0, 1E-06)) {
    fprintf(stderr, "Error: Aspect ration different to 1.0 is unsupported!\n");
    exit(1);
  }

  *img_xdim   = x_val;
  *img_ydim   = y_val;
  *img_type   = is_rgb & ~is_greyscale;
  if (aspect_ratio.field.s > 0) {
    *endianess = 1;
  } else {
    *endianess = -1;
  }
}

/* read_pbm_data:
 * Read the data contents of a PBM (portable bit map) file.
 */
void read_pbm_data(FILE *f, int *img_in, int is_ascii)
{
  int i=0, c;
  int lum_val;
  
  /* Read the rest of the PPM file. */
  while ((c = fgetc(f)) != EOF) {
    if (is_ascii == 1) {
      fscanf(f, "%d", &lum_val);
	} else {
	  fscanf(f, "%c", &lum_val);
	}
    img_in[i++] = lum_val;
  } 
  fclose(f);
}

/* read_pgm_data:
 * Read the data contents of a PGM (portable grey map) file.
 */
void read_pgm_data(FILE *f, int *img_in, int is_ascii)
{
  int i=0, c;
  int lum_val;
  
  /* Read the rest of the PPM file. */
  while ((c = fgetc(f)) != EOF) {
    if (is_ascii == 1) {
	  fscanf(f, "%d", &lum_val);
	} else {
	  fscanf(f, "%c", &lum_val);
	}
    img_in[i++] = lum_val;
  } 
  fclose(f);
}

/* read_ppm_data:
 * Read the data contents of a PPM (portable pix map) file.
 */
void read_ppm_data(FILE *f, int *img_in, int is_ascii)
{
  int i=0, c;
  int r_val, g_val, b_val;
    
  /* Read the rest of the PPM file. */
  while ((c = fgetc(f)) != EOF) {
    if (is_ascii == 1) {
      fscanf(f, "%d %d %d", &r_val, &g_val, &b_val);
	} else {
      fscanf(f, "%c%c%c", &r_val, &g_val, &b_val);
	}
    img_in[i++] = r_val;
    img_in[i++] = g_val;
    img_in[i++] = b_val;
  }
  fclose(f);
}

/* read_pfm_data:
 * Read the data contents of a PFM (portable float map) file.
 */
void read_pfm_data(FILE *f, float *img_in, int img_type, int endianess)
{
  int i=0, c;
  int swap = (endianess == 1) ? 1 : 0;
  float r_val, g_val, b_val;
    
  /* Read the rest of the PPM file. */
  while ((c = fgetc(f)) != EOF) {
    /* Read a possibly byte-swapped float. */
    if (img_type == RGB_TYPE) {
      ReadFloat(f, &r_val, swap);
      ReadFloat(f, &g_val, swap);
      ReadFloat(f, &b_val, swap); 
      img_in[i++] = r_val;
      img_in[i++] = g_val;
      img_in[i++] = b_val;
    } else if (img_type == GREYSCALE_TYPE) {
      ReadFloat(f, &g_val, swap);
      img_in[i++] = g_val;
    }
  }
  fclose(f);
}

/* write_pbm_file:
 * Write the contents of a PBM (portable bit map) file.
 */
void write_pbm_file(FILE *f, int *img_out, char *img_out_fname, 
  int x_size, int y_size, int x_scale_val, int y_scale_val, int linevals,
  int is_ascii)
{
  int i, j, x_scaled_size, y_scaled_size;
  int k, v, temp, step;
 
  x_scaled_size = x_size * x_scale_val;
  y_scaled_size = y_size * y_scale_val; 
  /* Write the magic number string. */
  if (is_ascii == 1) {
    fprintf(f, "P1\n");
	step = 1;
  } else {
    fprintf(f, "P4\n");
	step = 8;
  }
  /* Write a comment containing the file name. */
  fprintf(f, "# %s\n", img_out_fname);
  /* Write the image dimensions. */
  fprintf(f, "%d %d\n", x_scaled_size, y_scaled_size);
  
  /* Write the image data. */
  for (i = 0; i < y_scaled_size; i++) {
    for (j = 0; j < x_scaled_size; j+=step) {
	  if (is_ascii == 1) {
        fprintf(f, "%d ", img_out[i*x_scaled_size+j]);
	  } else {
	    temp = 0;
		for (k = 0; k < 8; k++) {
          v = img_out[i*x_scaled_size+j+k];
//		  temp |= (v << k) & 0x1;
		  temp |= (v << (7-k));
		}
//        fprintf(f, "%c", img_out[i*x_scaled_size+j]);
        fprintf(f, "%c", temp);
	  }
      if (((i*x_scaled_size+j) % linevals) == (linevals-1)) {
        fprintf(f, "\n");
      }
    }
  }   
  fclose(f);
}

/* write_pgm_file:
 * Write the contents of a PGM (portable grey map) file.
 */
void write_pgm_file(FILE *f, int *img_out, char *img_out_fname, 
  int x_size, int y_size, int x_scale_val, int y_scale_val, 
  int img_colors, int linevals, int is_ascii)
{
  int i, j, x_scaled_size, y_scaled_size;
 
  x_scaled_size = x_size * x_scale_val;
  y_scaled_size = y_size * y_scale_val; 
  /* Write the magic number string. */
  if (is_ascii == 1) {
    fprintf(f, "P2\n");
  } else {
    fprintf(f, "P5\n");
  }
  /* Write a comment containing the file name. */
  fprintf(f, "# %s\n", img_out_fname);
  /* Write the image dimensions. */
  fprintf(f, "%d %d\n", x_scaled_size, y_scaled_size);
  /* Write the maximum color/grey level allowed. */
  fprintf(f, "%d\n", img_colors);
  
  /* Write the image data. */
  for (i = 0; i < y_scaled_size; i++) {
    for (j = 0; j < x_scaled_size; j++) {
	  if (is_ascii == 1) {
        fprintf(f, "%d ", img_out[i*x_scaled_size+j]);
        if (((i*x_scaled_size+j) % linevals) == (linevals-1)) {
          fprintf(f, "\n");
		}
	  } else {
        fprintf(f, "%c", img_out[i*x_scaled_size+j]);
	  }
    }
  } 
  fclose(f);
}

/* write_ppm_file:
 * Write the contents of a PPM (portable pix map) file.
 */
void write_ppm_file(FILE *f, int *img_out, char *img_out_fname, 
  int x_size, int y_size, int x_scale_val, int y_scale_val, 
  int img_colors, int is_ascii)
{
  int i, j, x_scaled_size, y_scaled_size;
  
  x_scaled_size = x_size * x_scale_val;
  y_scaled_size = y_size * y_scale_val;
  /* Write the magic number string. */
  if (is_ascii == 1) {
    fprintf(f, "P3\n");
  } else {
    fprintf(f, "P6\n");
  }
  /* Write a comment containing the file name. */
  fprintf(f, "# %s\n", img_out_fname);
  /* Write the image dimensions. */
  fprintf(f, "%d %d\n", x_scaled_size, y_scaled_size);
  /* Write the maximum color/grey level allowed. */
  fprintf(f, "%d\n", img_colors);
  
  /* Write the image data. */
  for (i = 0; i < y_scaled_size; i++) {
    for (j = 0; j < x_scaled_size; j++) {
	  if (is_ascii == 1) {
        fprintf(f, "%d %d %d ", 
          img_out[3*(i*x_scaled_size+j)+0], 
          img_out[3*(i*x_scaled_size+j)+1], 
          img_out[3*(i*x_scaled_size+j)+2]);
        if ((j % 4) == 0) {
          fprintf(f, "\n");
        }
      } else {
        fprintf(f, "%c%c%c", 
          img_out[3*(i*x_scaled_size+j)+0], 
          img_out[3*(i*x_scaled_size+j)+1], 
          img_out[3*(i*x_scaled_size+j)+2]);
	  }
    }
  }  
  fclose(f);
}

/* write_pfm_file:
 * Write the contents of a PFM (portable float map) file.
 */
void write_pfm_file(FILE *f, float *img_out, char *img_out_fname, 
  int x_size, int y_size, 
  int img_type, int endianess)
{
  int i, j, x_scaled_size, y_scaled_size;
//  int swap = (endianess == 1) ? 1 : 0;
  int swap = 0;
  float fendian = endianess;
  unsigned char buffer[sizeof(float)];
  
  x_scaled_size = x_size;
  y_scaled_size = y_size;
  /* Write the magic number string. */
  if (img_type == RGB_TYPE) {
    fprintf(f, "PF\n");
  } else if (img_type == GREYSCALE_TYPE) {
    fprintf(f, "Pf\n");
  } else {
    fprintf(stderr, "Error: Image type invalid for PFM format!\n");
    exit(1);    
  }
  /* Write the image dimensions. */
  fprintf(f, "%d %d\n", x_scaled_size, y_scaled_size);
  /* Write the endianess/scale factor as float. */
  fprintf(f, "%f\n", fendian);
  
  /* Write the image data. */
  for (i = 0; i < y_scaled_size; i++) {
    for (j = 0; j < x_scaled_size; j++) {
      if (img_type == RGB_TYPE) {
        WriteFloat(f, &img_out[3*(i*x_scaled_size+j)+0], swap);
        WriteFloat(f, &img_out[3*(i*x_scaled_size+j)+1], swap);
        WriteFloat(f, &img_out[3*(i*x_scaled_size+j)+2], swap);
      } else if (img_type == GREYSCALE_TYPE) {
        WriteFloat(f, &img_out[i*x_scaled_size+j], swap);
      }
    }
  }  
  fclose(f);
}

/* ReadFloat:
 * Read a possibly byte swapped floating-point number.
 * NOTE: Assume IEEE format.
 * Source: http://paulbourke.net/dataformats/pbmhdr/
 */
int ReadFloat(FILE *fptr, float *f, int swap)
{
  unsigned char *cptr, tmp;

  if (fread(f, 4, 1, fptr) != 1) {
    return (FALSE);
  }   
  if (swap) {
    cptr    = (unsigned char *)f;
    tmp     = cptr[0];
    cptr[0] = cptr[3];
    cptr[3] = tmp;
    tmp     = cptr[1];
    cptr[1] = cptr[2];
    cptr[2] = tmp;
  }
  return (TRUE);
}

/* WriteFloat:
 * Write a possibly byte-swapped floating-point number.
 * NOTE: Assume IEEE format.
 */
int WriteFloat(FILE *fptr, float *f, int swap)
{
  unsigned char *cptr, tmp;

  if (swap) {
    cptr    = (unsigned char*)f;
    tmp     = cptr[0];
    cptr[0] = cptr[3];
    cptr[3] = tmp;
    tmp     = cptr[1];
    cptr[1] = cptr[2];
    cptr[2] = tmp;
  }
  if (fwrite(f, sizeof(float), 1, fptr) != 1) {
    return (FALSE);
  }  
  return (TRUE); 
}

/* floatEqualComparison: 
 * Compare two floats and accept equality if not different than
 * maxRelDiff (a specified maximum relative difference).
 */
int floatEqualComparison(float A, float B, float maxRelDiff)
{
  float largest, diff = fabs(A-B);
  A = fabs(A);
  B = fabs(B);
  largest = (B > A) ? B : A;
  if (diff <= largest * maxRelDiff) {
    return 1;
  }
  return 0;
}

/* frand48:
 * Emulate a floating-point PRNG.
 * Source: http://c-faq.com/lib/rand48.html
 */
float frand(void)
{
  return rand() / (RAND_MAX + 1.0);
}
