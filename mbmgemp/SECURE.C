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
			     
#include "gcomm.h"
#include "dos.h"
#include "majorbbs.h"
#include "string.h"
#include "secure.h"

/* Begin btrieve includes */
char *fbuf;
int fkeylen;
struct btvblk *fopnbtv(char *filnam, int maxlen);
void btverr(char *who);
int fclckln(void);
void maksur(int size);
void eclalcrem(int size, int *seg, char **sel);
int btvu(int funcno, int datbufseg, int keyseg, int keyno, int rlen);
void eclint(int inum, REGS16 *regs);
void DosBlockIntr(void);
void DosUnblockIntr(void);
int protect(int, char *);
int securit(void);
static char *secptr;
static int secseg=0;

struct filspc {               /* Btrieve STAT command file spec            */
	  int reclen;
	  int pagsiz;
	  int numofx;
	  long numofr;
	  int flags;
	  int reserved;
	  int unupag;
};
static struct btvblk *bb;               /* current btvu file pointer set   */
static int bbomode=PRIMBV;              /* opnbtv() file-open mode         */
static char *gpbptr;                    /* general purpose buffer pointer  */
static int gpbseg;                      /* real-mode segment of the above  */
static int *statusptr;                  /* status code returned by btvu()  */
static int statusseg;                   /* real-mode segment of the above  */
static int lastlen;                     /* length of last record read in   */

static
struct btvdat {   /* btrieve parameter block structure for use with INT 0x7B */
	  int datbufoff;
	  int datbufseg;
	  int dbflen;
	  int posp38off;
	  int posp38seg;
	  int posblkoff;
	  int posblkseg;
	  int funcno;
	  int keyoff;
	  int keyseg;
	  char keylen;
	  char keyno;
	  int statptoff;
	  int statptseg;
	  int magic;
} *btvdatptr;
static int btvdatseg;              /* real-mode segment of the above       */

struct keyspc {               /* Btrieve STAT command key specs            */
	  int keypos;
	  int keylen;
	  int flags;
	  long numofk;
	  int dontcare;
	  long reserved;
};

#define ANOSEG  0x10          /* keyspc.flags bit for key has another seg  */

struct statbf {               /* STAT command ret buf layout               */
	  struct filspc fs;
	  struct keyspc ks[SEGMAX];
	  char altcol[265];
};

unsigned int oldimsk;         /* old 8259 interrupt mask                   */

/* end btrieve includes */

/* begin security stuff */

char *regpnt;
static int fpend, tdover, fpcnt;
static long encode, fpdays;
static char regrno[9], chkrno[9];
static char *securn="00000000\0";

/* end security stuff */

struct btvblk *sec_opnbtv(	char 	*datfilnam, 
									int 	sizostruct, 
									char 	*seed, 
									int 	regnum, 
									char 	*key, 
									int 	method)

{
int x;
char pos1[9];
char pos2[9];
char pos3[9];
char pos4[9];

struct btvblk* secdb;
extern char bturno[];
char fpendt[3];

tdover = 0;
regpnt = stgopt(regnum);
strcpy(regrno, regpnt);

secure = (struct secure *)alcmem(sizeof(struct secure));
secure->open_stat = 0;
secure->days_run = 0;
secure->days_tot = 0;
setmem(secure->reg_num, 9, '\0');
strcpy(secure->reg_num, regrno);
encode = atol(seed);
fpendt[0] = seed[0];
fpendt[1] = seed[6];
fpendt[2] = '\0';
fpend = atoi(fpendt);
protect(method, seed);
strcpy(securn, chkrno);
secdb = fopnbtv(datfilnam, sizostruct);
setbtv(secdb);
btvu(29, securit(), securit(), bbomode, strlen(chkrno));
setmem(securn, 9, '\0');
fbuf = malloc(sizostruct);
setmem(secure->key, fkeylen, '\0');
memcpy(secure->key, key, fkeylen-1);
if(!cntrbtv()){
 setmem(fbuf, sizostruct, '\0');
 memcpy(fbuf, secure->key, fkeylen);
 sprintf(&fbuf[fkeylen], "%8ld", fpdays^encode);
 sprintf(&fbuf[fkeylen+8], "%8ld", fpdays^encode);
 sprintf(&fbuf[fkeylen+16], "%8ld", (atol(bturno))^(1));
 sprintf(&fbuf[fkeylen+24], "%8ld", (atol(bturno))^(fpdays));
 for(x = fkeylen;x<sizostruct;x++){
	if (fbuf[x] ! =  '\0')
		fbuf[x]  =  (fbuf[x]+x);
	}
 insbtv(fbuf);
 }
acqbtv(fbuf, secure->key, 0);
for(x = fkeylen;x<sizostruct;x++)
	{
if(fbuf[x] ! =  '\0')
	fbuf[x]  =  (fbuf[x]-x);
  }
strncpy(pos1, &fbuf[fkeylen], 8);
pos1[8] = '\0';
strncpy(pos2, &fbuf[fkeylen+8], 8);
pos2[8] = '\0';
strncpy(pos3, &fbuf[fkeylen+16], 8);
pos3[8] = '\0';
strncpy(pos4, &fbuf[fkeylen+24], 8);
pos4[8] = '\0';
fpcnt = (int)((atol(pos3))^(atol(bturno)));
if(!protect(method, seed))
	{
	if(((atol(pos2))^(encode))! = fpdays)
		{
		sprintf(&fbuf[fkeylen+8], "%8ld", fpdays^encode);
		fpcnt++;
		sprintf(&fbuf[fkeylen+16], "%8ld", (atol(bturno))^(fpcnt));
		sprintf(&fbuf[fkeylen+24], "%8ld", (atol(pos4)));
		for(x = fkeylen;x<sizostruct;x++)
			{
			if(fbuf[x]! = '\0')
				fbuf[x] = (fbuf[x]+x);
			}
		updbtv(fbuf);
		}
	if(((atol(pos1)^(encode))>fpdays)
	 ||((fpdays-(atol(pos1)^(encode)))>fpend)
	 ||(fpcnt>fpend)
	 ||((atol(pos1)^(encode))! = (atol(pos4)^(atol(bturno)))))
	 	{
		tdover = 1;
		secure->open_stat = 0;
		clsbtv(secdb);
		}
	}
else
	{
	secure->open_stat = 1;
	}

if(!protect(method, seed) && !tdover)
	{
	secure->open_stat = -1;
	}
if((30-fpend)<0){
  secure->open_stat = 0;
  clsbtv(secdb);
  }
secure->days_run = fpcnt;
secure->days_tot = fpend;
return(secdb);
}

int
securit()
{
if(secseg =  = 0)
  eclalcrem(10, &secseg, &secptr);
strcpy(secptr, securn);
return(secseg);
}


int protect(int method, char *seed)
{
char tmpc[2];
char tmpd[3];
char temp[9];
int i, val1, val2, dec;
extern char bturno[];
setmem(chkrno, 9, '\0');
setmem(temp, 9, '\0');
strcpy(temp, ltoa(atol(bturno)^(encode)));
dec = 8;
for(i = strlen(temp);i>-1;i = i-1){
  chkrno[dec] = temp[i];
  dec = dec-1;
  }
fpdays = cofdat(today());
  if(method =  = 1){
	  for(i = 0; i<8; ++i){
		 tmpc[0] = chkrno[i];
		 tmpc[1] = 0;
		 val1 = atoi(tmpc);
		 tmpc[0] = seed[i];
		 tmpc[1] = 0;
		 val2 = atoi(tmpc);
		 setmem(tmpd, 3, '\0');
		 itoa(val1+val2, tmpd, 10);
		 chkrno[i] = tmpd[strlen(tmpd)-1];
		 }
	  }
	  if(sameas(regrno, chkrno)) return(1);
	  return(0);
}




/* Begin btrieve utilities */

struct btvblk *
fopnbtv(                                 /* open a Btrieve file; return ptr.*/
char *filnam,                            /* name of file to open            */
int maxlen)                             /* maximum length of recs in file  */
{
	  int tptr;

	  if (statusptr  =  =  NULL) {
			 eclalcrem(sizeof(int), &statusseg, (char **)&statusptr);
			 eclalcrem(sizeof(struct btvdat), &btvdatseg, (char **)&btvdatptr);
	  }
	  eclalcrem(sizeof(struct btvblk), &tptr, (char **)&bb);
	  bb->realseg = tptr;

	  bb->filnam = malloc(strlen(filnam)+1);
	  strcpy(bb->filnam, filnam);
	  bb->reclen = maxlen;
	  eclalcrem(maxlen, &bb->dataseg, &bb->data);
	maksur(strlen(filnam)+1);
	  strcpy(gpbptr, filnam);
	  if (btvu(0, securit(), gpbseg, bbomode, bb->reclen) ! =  0) {
			 btverr("OPEN");
	  }
	  eclalcrem(fclckln(), &bb->keyseg, &bb->key);
	  return(bb);
}

STATIC void
btverr(                       /* print out any btrieve error messages      */
char *who)
{
	  char buff[40];

	  sprintf(buff, "%s ERROR %d", who, status);
	  catastro("BTRIEVE %s ON FILE \"%s\"", buff, bb->filnam);
}

#define GPBP ((struct statbf *)gpbptr)

int
fclckln(void)                  /* calculate all key lengths in curr file    */
{                             /*   (and return the max of them all)        */
	  static int kbfseg = 0;

	  int i, keyno, klen, maxofa;
	  char *tmp;

	  if (kbfseg  =  =  0) {
			 eclalcrem(64, &kbfseg, &tmp);
	  }
	  maksur(sizeof(struct statbf));
	  if (btvu(15, gpbseg, kbfseg, 0, sizeof(struct statbf)) ! =  0) {
			 btverr("STAT");
	  }
	  maxofa = 0;
	  fkeylen = GPBP->ks[0].keylen;
	  for (i = 0, keyno = 0 ; keyno < GPBP->fs.numofx ; i++, keyno++) {
			 klen = GPBP->ks[i].keylen;
			 while (GPBP->ks[i].flags&ANOSEG) {
					klen+ = GPBP->ks[++i].keylen;
			 }
			 bb->keylns[keyno] = klen;
			 if (klen > maxofa) {
					maxofa = klen;
			 }
	  }
	  return(maxofa+1);
}

void
maksur(                       /* make sure that gen purp buff is big enough*/
int size)
{
	  static int gpbsiz = 0;

	  if (size > gpbsiz) {
			 if (gpbsiz > 0) {
					free(gpbptr);
			 }
			 eclalcrem(size, &gpbseg, &gpbptr);
			 gpbsiz = size;
	  }
}

void
eclalcrem(                         /* allocate real-mode dynamic memory    */
int size,                           /*   (make both real seg and selector)  */
int *seg, 
char **sel)
{
	  void DosBlockIntr(void);
	  void DosUnblockIntr(void);
	  SEL pmsel;
	  int rv;

	  DosBlockIntr();
	  if ((rv = DosAllocRealSeg((ULONG)size, (PUSHORT)seg, &pmsel)) ! =  0) {
			 DosUnblockIntr();
			 catastro("REAL-MODE MEMORY POOL EXHAUSTED (rv = %d,  %d bytes)", rv, size);
	  }
	  DosUnblockIntr();
	  *sel = MK_FP(pmsel, 0);
	  return;
}

int
btvu(                                    /* underlying Btrieve cmd interface */
int funcno, 
int datbufseg, 
int keyseg, 
int keyno, 
int rlen)
{
	  REGS16 eclregs;
	  static int btrvup = 0, alrcat = 0;

	  if (!btrvup) {
			 memset(&eclregs, 0, sizeof(REGS16));
			 eclregs.ax = 0x357B;
			 eclint(0x21, &eclregs);
			 if (eclregs.bx ! =  0x33) {
					if (alrcat) {
						  return(0);
					}
					alrcat = 1;
					catastro("Run BTRIEVE first,  before running The Major BBS!");
			 }
			 btrvup = 1;
	  }
	  btvdatptr->datbufoff = 0;
	  btvdatptr->datbufseg = datbufseg;
	  btvdatptr->dbflen = rlen;
	  btvdatptr->posp38off = 38;
	  btvdatptr->posp38seg = bb->realseg;
	  btvdatptr->posblkoff = 0;
	  btvdatptr->posblkseg = bb->realseg;
	  btvdatptr->funcno = funcno;
	  btvdatptr->keyoff = 0;
	  btvdatptr->keyseg = keyseg;
	  btvdatptr->keylen = (char)255;
	  btvdatptr->keyno = (char)keyno;
	  btvdatptr->statptoff = 0;
	  btvdatptr->statptseg = statusseg;
	  *statusptr = 0;
	  btvdatptr->magic = 24950;
	  memset(&eclregs, 0, sizeof(REGS16));
	  eclregs.dx = 0;
	  eclregs.ds = btvdatseg;
	  eclint(0x7B, &eclregs);
	  lastlen = btvdatptr->dbflen;
	  return(status = *statusptr);
}

void
eclint(                            /* protected to real interrupt          */
int inum, 
REGS16 *regs)
{
	  DosRealIntr(inum, regs, 0L, 0);
}

void
DosBlockIntr(void)            /* Block all interrupts at 8259              */
{
asm  cli
asm  in   al, 0x21
asm  mov  oldimsk, ax
asm  mov  al, 0xFF
asm  out  0x21, al
}

void
DosUnblockIntr(void)          /* Unblock all interrupts at 8259            */
{
asm  mov  ax, oldimsk
asm  out  0x21, al
asm  sti
}

/* end btrieve utilities */
