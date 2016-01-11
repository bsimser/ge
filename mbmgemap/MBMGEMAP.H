 /**************************************************************************
  * This program is free software; you can redistribute it and/or modify   *
  * under the terms of the GNU General Public License as published by the  *
  * Free Software Foundation; either version 2 of the License, or (at your *
  * option) any later version.                                             *
  *                                                                        *
  * This program is distributed in the hope that it will be useful,        *
  * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU       *
  * General Public License for more details.                               *
  *                                                                        *
  * You should have received a copy of the GNU General Public License      *
  * along with this program; if not, write to the Free Software Foundation,*
  * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA            *
  *************************************************************************/
			     
#define TSC_C 1

#define SEGMAX  24            /* max number of key segs per file           */

struct btvblk {                    /* btrieve file data block definition   */
     long posblk[128/4];           /*   position block                     */
     char *filnam;                 /*   file name                          */
     int reclen;                   /*   maximum record length              */
     char *key;                    /*   key for searching, etc.            */
     char *data;                   /*   actual record contents             */
     int lastkn;                   /*   last key number used               */
     int keylns[SEGMAX];           /*   lengths of all possible keys       */
#ifdef ECLIPSE
     int realseg;                  /*   real segment of this data block    */
     int keyseg;                   /*   real segment of the key area       */
     int dataseg;                  /*   real segment of the data area      */
#endif
};

#define BTVFILE struct btvblk      /* shorthand for btrieve file structure */

BTVFILE *opnbtv();                 /* non-int btvstf routine returns       */
long absbtv(),
     cntrbtv();

#define PRIMBV  0                  /* normal pre-image btvopn() mode       */
#define ACCLBV -1                  /* "accelerated" btvopn() mode          */
#define RONLBV -2                  /* read-only btvopn() mode              */
#define VERFBV -3                  /* verify (read-after-write) btvopn mode*/
#define EXCLBV -4                  /* "exclusive" btvopn() mode            */

#define qeqbtv(key,keynum) qrybtv(key, keynum,55)
#define qnxbtv()           qnpbtv(56)
#define qprbtv()           qnpbtv(57)
#define qgtbtv(key,keynum) qrybtv(key, keynum,58)
#define qgebtv(key,keynum) qrybtv(key, keynum,59)
#define qltbtv(key,keynum) qrybtv(key, keynum,60)
#define qlebtv(key,keynum) qrybtv(key, keynum,61)
#define qlobtv(keynum)     qrybtv(NULL,keynum,62)
#define qhibtv(keynum)     qrybtv(NULL,keynum,63)

#define geqbtv(recp,key,keynum) getbtv(recp,key, keynum,5)
#define gnxbtv(recp)            getbtv(recp,NULL,    -1,6)
#define gprbtv(recp)            getbtv(recp,NULL,    -1,7)
#define ggtbtv(recp,key,keynum) getbtv(recp,key, keynum,8)
#define ggebtv(recp,key,keynum) getbtv(recp,key, keynum,9)
#define gltbtv(recp,key,keynum) getbtv(recp,key, keynum,10)
#define glebtv(recp,key,keynum) getbtv(recp,key, keynum,11)
#define globtv(recp,keynum)     getbtv(recp,NULL,keynum,12)
#define ghibtv(recp,keynum)     getbtv(recp,NULL,keynum,13)

#define acqbtv(recp,key,keynum) obtbtv(recp,key, keynum,5)
#define aqnbtv(recp)            anpbtv(recp,            6)
#define aqpbtv(recp)            anpbtv(recp,            7)
#define agtbtv(recp,key,keynum) obtbtv(recp,key, keynum,8)
#define agebtv(recp,key,keynum) obtbtv(recp,key, keynum,9)
#define altbtv(recp,key,keynum) obtbtv(recp,key, keynum,10)
#define alebtv(recp,key,keynum) obtbtv(recp,key, keynum,11)
#define alobtv(recp,keynum)     obtbtv(recp,NULL,keynum,12)
#define ahibtv(recp,keynum)     obtbtv(recp,NULL,keynum,13)

#define gcrbtv(recp,keynum)     gabbtv(recp,absbtv(),keynum)

#ifdef MSC_C
#define XREG                WORDREGS
#endif

/***************************************************************************
 *                                                                         *
 *   PORTABLE.H                                                            *
 *                                                                         *
 *   Copyright (C) 1988,1990 GALACTICOMM, Inc.      All Rights Reserved.   *
 *                                                                         *
 *   This header file supports compatibility of the source code of The     *
 *   Major BBS with multiple C language compilers, specifically, those     *
 *   of Microsoft and Borland (Turbo).                                     *
 *                                                                         *
 *   This declaration:  include "portable.h"  should appear in each        *
 *   source file, immediately following the other include directives.      *
 *                                                                         *
 *                         WARNING!                                        *
 *                                                                         *
 *   Your compiler command line must define one of the symbols:            *
 *   MSC_C or TSC_C.   This identifies the C compiler                      *
 *   (Microsoft or Turbo).  If Microsoft C 5.0 is in use,                  *
 *   you must also define the symbol MS5.  If you plan to use the          *
 *   protected-mode facilities associated with the Eclipse package,        *
 *   you must also define the symbol ECLIPSE.  To use the Turbo C          *
 *   "huge" model without going to protected mode, no special              *
 *   symbol need be defined (other than TSC_C, indicating Turbo C,         *
 *   of course!).                                                          *
 *                                                                         *
 *                                                 - R. Stein   1/24/88    *
 *   Lattice eliminated, "huge" and Eclipse added  - T. Stryker 2/28/90    *
 *                                                                         *
 ***************************************************************************/

#ifdef MSC_C
#define allmem()            ;
#ifdef ECLIPSE
#define sizmem()            eclcoreleft()
#define getml(ln)           eclmalloc(ln)
#define free(p)             eclfree(p)
#define malloc(in)          eclmalloc((long)(in))
char far *eclmalloc(unsigned long size);
long eclcoreleft();
#else
#define getml(ln)           halloc(ln,1)
char huge *halloc();
void *malloc();
#endif
#define ADDR_OFdisaster     disaster
#define ADDR_OFeximod       eximod
#define FOPRB  "rb"     /* fopen() 2nd parameter for read binary mode       */
#define FOPRA  "ra"     /* fopen() 2nd parameter for read ASCII mode        */
#define FOPWB  "wb"     /* fopen() 2nd parameter for write binary mode      */
#define FOPWA  "wa"     /* fopen() 2nd parameter for write ASCII mode       */
#define FOPRWB "r+b"    /* fopen() 2nd parameter for read/write binary mode */
#define FOPRWA "r+a"    /* fopen() 2nd parameter for read/write ASCII mode  */
#define FOPAB  "ab"     /* fopen() 2nd parameter for append binary mode     */
#define FOPAA  "aa"     /* fopen() 2nd parameter for append ASCII mode      */
#define CNTLIT 0        /* does sscanf() count literal matches?             */
#define CFREGS 1        /* REGS has a cflag field (see TurboC dos.h)        */
#define CKNOWN
#define setmem(p,n,c)       memset(p,c,n)
#ifdef MS5
#define movmem(s,d,n)       memmove(d,s,n)   /* microsoft 5.0 memcpy does   */
#else                                        /*   not deal with overlaps    */
#define movmem(s,d,n)       memcpy(d,s,n)
#endif
#endif

#ifdef TSC_C
#define allmem()            ;
#ifdef ECLIPSE
#define sizmem()            eclcoreleft()
#define getml(ln)           eclmalloc(ln)
#define free(p)             eclfree(p)
#define malloc(in)          eclmalloc((long)(in))
char far *eclmalloc(unsigned long size);
long eclcoreleft();
#else
#define getml(ln)           farmalloc(ln)
#define sizmem()            farcoreleft()
char far *farmalloc(unsigned long size);
void *malloc(unsigned size);
long farcoreleft();
#endif
#define ADDR_OFdisaster     disaster
#define ADDR_OFeximod       eximod
#define FOPRB  "rb"     /* fopen() 2nd parameter for read binary mode       */
#define FOPRA  "rt"     /* fopen() 2nd parameter for read ASCII mode        */
#define FOPWB  "wb"     /* fopen() 2nd parameter for write binary mode      */
#define FOPWA  "wt"     /* fopen() 2nd parameter for write ASCII mode       */
#define FOPRWB "r+b"    /* fopen() 2nd parameter for read/write binary mode */
#define FOPRWA "r+t"    /* fopen() 2nd parameter for read/write ASCII mode  */
#define FOPAB  "ab"     /* fopen() 2nd parameter for append binary mode     */
#define FOPAA  "at"     /* fopen() 2nd parameter for append ASCII mode      */
#define CNTLIT 0        /* does sscanf() count literal matches?             */
#define CFREGS 1        /* REGS has a cflag field (see TurboC dos.h)        */
#define CKNOWN
#endif

#ifndef min
#define min(a,b)        (((a) < (b)) ? (a) : (b))
#define max(a,b)        (((a) > (b)) ? (a) : (b))
#define abs(a)          (((a) < 0) ? -(a) : (a))
#endif

#ifdef CKNOWN
#undef CKNOWN
#else
#error YOU SHOULD IDENTIFY THE COMPILER!  (See the warning in PORTABLE.H)
#endif

