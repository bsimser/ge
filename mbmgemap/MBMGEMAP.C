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

/***************************************************************************
 *                                                                         *
 *   MBMGEMAP.C                                                            *
 *                                                                         *
 *   Copyright (C) 1988, 89, 90, 91, 92 Michael B. Murdock                 *
 *                                                                         *
 *   This is the source for the Galactic Empire game module                *
 *                                                                         *
 *                                         M. Murdock     03/17/92         *
 *                                                                         *
 ***************************************************************************/

 /**************************************************************************
 * Authors Note: The source code to this program is being made available   *
 *               to the general public in the hopes that it will continue  *
 *               to live on and evolve. While I cannot provide the time    *
 *               to personally participate in its development any longer,  *
 *               I am eager to assist in the design discussions, testing   *
 *               and project discussions.                                  *
 *                                                                         *
 *               To foster a community for Galactic Empire developers I    *
 *               have created a project home page at...                    *
 *               http://mbbs.sitesage.net                                  *
 *                                                                         *
 *               I may be contacted via email at mmurdock@starphire.com    *
 ***************************************************************************/
 
#include "string.h"
#include "dos.h"
#include "conio.h"
#include "stdio.h"
#include "mem.h"
#include <math.h>
#include "mbmgemap.h"
#include "mbmgemap.hp"
#include "endecode.h"

#define VERSION	"1.0B"

/************************** P R O T O T Y P E S *****************************/

void 	clrscrx(void);
void 	clrscr1(void);
void 	clrscr2(void);
struct btvblk *opnbtv(char *filnam,int maxlen);
int	clckln(void);
int	omdbtv(int mode);
int	setbtv(struct btvblk *bbptr);
int	qrybtv(char *key, int keynum, int qryopt);
int	qnpbtv(int getopt);
int	getbtv(char *recptr, char *key, int keynum, int getopt);
int	obtbtv(char *recptr, char *key, int keynum, int obtopt);
int	anpbtv(char *recptr, int anpopt);
long 	absbtv(void);
int	gabbtv(char *recptr, long abspos, int keynum);
int	aabbtv(char *recptr, long abspos, int keynum);
int	sabbtv(char *recptr);
int	updbtv(char *recptr);
int	upvbtv(char *recptr, int length);
int	insbtv(char *recptr);
int	invbtv(char *recptr, int length);
int	delbtv(void);
int	clsbtv(struct btvblk *bbp);
int	btverr(char *who);
int	btvu(int funcno, char *datbuf, char *key, int keyno, int rlen);
int	btvup(void);
char 	*alcmem(unsigned size);
char 	*secure(void);
int	honk(void);
int	free(char *);
void	file_write(FILE *hdl, char *buf, int flg, char *key);

/**************************** S A M E   S T U F F ***************************/

#define  NUMITEMS       13
#define	UIDSIZ	30
#define MAXPLANETS 9
#define BEACONMSGSZ	75

#define COORD struct _coord
extern
COORD {
	  double	xcoord;
	  double ycoord;
		};

#define PLNTCOORD struct _plntcoord
extern
PLNTCOORD {
	int     type;                  /* planet type */
	COORD coord;                 /* location */
};


#define PLANETAB struct planetab

extern
PLANETAB {
	PLNTCOORD       planets[MAXPLANETS];
	};



/****************************** N E W   S T U F F ****************************/



#define N_ITEM    struct n_item
extern
N_ITEM    {
	unsigned long	qty;                            /* quantity on hand                                                     */
	unsigned			rate;                                           /* rate of manufacture                                          */
	char				sell;                                           /* sell to allies?                                                      */
	unsigned			reserve;                                        /* qty to reserve                                                       */
	unsigned			markup2a;                               /* mark up to allies                                                    */
	unsigned	long	sold2a;                    /* # sold to allies                                                     */
	};

#define GALPLNT_NEW struct _galplnt_new

extern
GALPLNT_NEW {                /* star system data record               */
	int 	xsect;                                  /* sector x coord                       */
	int	ysect;                                  /* sector y coord                       */
	int	plnum;               /* planet number */
	int	type;                /* type of sector */
	COORD	coord;
	char	userid[UIDSIZ];
	char	name[20];
	char	enviorn;             /* enviornment factor                                              */
	char	resource;                               /* resources                                                                    */
	unsigned	long cash;                                           /* cash on hand                                                         */
	unsigned long	debt;                /* amount you owe                      */
	unsigned long	tax;                 /* amount of tax collected             */
	int	taxrate;             /* rate to tax population              */
	char	warnings;            /* warnings to intruders               */
	char	password[10];                   /* password for alliance               */
	char	lastattack[UIDSIZ];     /* userid of last attacker             */
	N_ITEM	items[NUMITEMS]; /* 195 */
	char	beacon[BEACONMSGSZ];
	char	spyowner[UIDSIZ];
	int	technology;
	unsigned long	teamcode;
	char	filler[512-182-(UIDSIZ*2)-(sizeof(N_ITEM)*NUMITEMS)];         


};

GALPLNT_NEW	new;



#define N_GALSECT struct n_galsect

extern
N_GALSECT {                /* star system data record               */
	int             xsect;                                  /* sector x coord                       */
	int             ysect;                                  /* sector y coord                       */
	int             plnum;                                  /* always 0 */
	int             type;                /* type of sector */

	int             numplan;             /* number of planetary objects */
	PLNTCOORD       ptab[MAXPLANETS];
	char            filler[512-10-(sizeof(PLNTCOORD)*MAXPLANETS)];          /* fill to 250*/
};


N_GALSECT	newsect;


#define N_GALWORM struct n_galworm

extern
N_GALWORM {                /* star system data record               */
	int             xsect;                                  /* sector x coord                       */
	int             ysect;                                  /* sector y coord                       */
	int             plnum;               /* planet number */
	int             type;                /* type of sector */
	COORD           coord;
	int             visible;             /* is the wormhole visible flag */
	COORD           destination;
	char            filler[512-10-(sizeof(COORD)*2)];         /* fill to 250*/
};


N_GALWORM	newworm;


static char	buffer[2000];

static struct btvblk *bb;               /* current btvu file pointer set   */
static int status;                      /* status code returned by btvu()  */
static int bbomode=PRIMBV;              /* opnbtv() file-open mode         */

struct filspc {               /* Btrieve STAT command file spec            */
     int reclen;
     int pagsiz;
     int numofx;
     long numofr;
     int flags;
     int reserved;
     int unupag;
};

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

char *alcmem();               /* memory allocation utility in GENUTL.C     */

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

char	chrbuf[80];
int	seedrnd;
char	ageyn[2];
char	detail;

BTVFILE *gebbold;
BTVFILE *gebbnew;

char	record[512];

char 	regno[10];

FILE	*hdl;

void 	main(int argc, char **argv)

{

int	i;
static long     fpos = 0;
int	bad;
int	xtsect,ytsect;
int	planets = 0;
int	sectors = 0;
int	worms   = 0;
char	flipper[]="-\|/";
int	cnt;

clrscrx();

if (!btvup())
	{
	printf("\r\nError! BTRIEVE not loaded. Please Run Btrieve first!\r\n");
	return;
	}

/* read in args */

for (i=1;i<argc;++i)
	{
	if (strlen(argv[i]) > 1 && argv[i][0] == '-')
		{
		if (argv[i][1] == 'N' || argv[i][1] == 'n')
			{
			strcpy(regno,&argv[i][2]);
			if (strlen(regno) != 8)
				{
				printf("\r\nError! Registration Number not 8 digits\r\n");
				return;
				}
			}

		if (argv[i][1] == '?')
			{
			printf("USAGE:\r\n");
			printf("   MBMGEMAP [-options]\r\n");
			printf("    -nxxxxxxxx      8 Digit GE Registration #\r\n");

			printf("\r\n");
			printf("Examples:\r\n");
			printf("   mbmgemap -n12345678\r\n");
			printf("\r\n");
			return;
			}
		}
	}


if (strlen(regno) != 8)
	{
	printf("\r\nError! Registration Number not specified\r\n   Type MBMGEMAP -? for Help!\r\n");
	return;
	}
	




printf("Begin Processing Planet Database.\r\n");

gebbnew=opnbtv("MBMGEPLT.DAT",sizeof(new));

if (gebbnew == (BTVFILE *)0)
	{
	printf("\r\nMBMGEPLT.DAT file open error...please check!\r\n");
	return;
	}


hdl = fopen("mbmgeplt.map","wb");

setbtv(gebbnew);

if (qlobtv(0))
	{
	gotoxy(1,10);
	printf("Processing....");
	cnt = 0;

	fprintf(hdl,"Galactic Empire Planet Extract File\n");
	fprintf(hdl,"Copyright(c) 1994 M.B. Murdock & Associates, All Rights Reserved!\n");
	fprintf(hdl,"Version:%s\n",VERSION);

	strcpy(buffer,regno);

	file_write(hdl,buffer,1,"10293847");
	strcpy(buffer,"MaRkEr");
	file_write(hdl,buffer,1,regno);
	do
		{
		gotoxy(14,10);
		printf("%c",flipper[cnt%4]);
		cnt++;
		gcrbtv((char *)&newsect,0);

		fpos=absbtv();

		if (newsect.type == 1) /* if sector record */
			{
/*			fprintf(hdl,"SECTOR|X:%d|Y:%d|XF:%f|YF:%f|P:%d|*\n",*/
			sprintf(buffer,"SECTOR|%d|%d|%d|*\n",
				newsect.xsect,
				newsect.ysect,
				newsect.numplan);
			file_write(hdl,buffer,1,regno);
			
			gotoxy(1,12);
			printf("Sectors....... %d",++sectors);
			}
		else
		if (newsect.type == 2) /* if planet record */
			{
			gcrbtv((char *)&new,0);
/*			fprintf(hdl,"PLANET|X:%d|Y:%d|XF:%f|YF:%f|P:%d|S:%ld|O:%s|N:%s|\n",*/
			sprintf(buffer,"PLANET|%d|%d|%f|%f|%d|%ld|%s|%s|*\n",
				new.xsect,
				new.ysect,
				new.coord.xcoord,
				new.coord.ycoord,
				new.plnum,
				new.items[0].qty,
				new.userid,
				new.name);
			file_write(hdl,buffer,1,regno);
			gotoxy(1,13);
			printf("Planets....... %d",++planets);
			}
		else
		if (newsect.type == 3) /* if worm record */
			{
			gcrbtv((char *)&newworm,0);

			xtsect = (int)floor(newworm.destination.xcoord);
			ytsect = (int)floor(newworm.destination.ycoord);

/*			fprintf(hdl,"WORM|X:%d|Y:%d|XF:%f|YF:%f|XT:%d|YT:%d|XTF:%f|YTF:%f|\n",*/
			sprintf(buffer,"WORM|%d|%d|%f|%f|%d|%d|%f|%f|*\n",
				newworm.xsect,
				newworm.ysect,
				newworm.coord.xcoord,
				newworm.coord.ycoord,
				xtsect,
				ytsect,
				newworm.destination.xcoord,
				newworm.destination.ycoord);
			file_write(hdl,buffer,1,regno);
			gotoxy(1,14);
			printf("Wormholes..... %d",++worms);

			}

		gabbtv((char *)&newsect,fpos,0);
		} while (qnxbtv());
	}

gotoxy(1,16);

printf("Extract Complete\r\n");

clsbtv(gebbnew);

fclose(hdl);

return;
}





void clrscrx(void)
{
clrscr();
gotoxy(1,1);
printf(" MBMGEMAP: Galactic Empire Planet Database Extract Utility V%s \r\n",VERSION);
printf("           (c) Copyright 1994 M.B. Murdock & Associates\r\n");
printf("\r\n\r\n\r\n");
return;
}

void clrscr1(void)
{
int i;
for (i=5; i<10; ++i)
	{
	gotoxy(1,i);
	clreol();
	gotoxy(1,5);
	}
return;
}

void clrscr2(void)
{
int i;
for (i=10; i<24; ++i)
	{
	gotoxy(1,i);
	clreol();
	gotoxy(1,10);
	}
return;
}




/***************************************************************************/
/***************************************************************************/
/***************************************************************************/


struct btvblk *
opnbtv(filnam,maxlen)
char *filnam;
int maxlen;
{
char *secure();
bb=(struct btvblk *)alcmem(sizeof(struct btvblk));
bb->filnam=alcmem(strlen(filnam)+1);
strcpy(bb->filnam,filnam);
bb->reclen=maxlen;
bb->data=alcmem(maxlen);
if (btvu(0,secure(),filnam,bbomode,bb->reclen) != 0) 
	{
	btverr("OPEN");
	return((struct btvblk *)0);
	}
bb->key=alcmem(clckln());
return(bb);
}


int	clckln(void)                      /* calculate all key lengths in curr file    */
{                             /*   (and return the max of them all)        */
static char kbf[64];
int i,keyno,klen,maxofa;
struct statbf *tmpbuf;

tmpbuf=(struct statbf *)alcmem(sizeof(struct statbf));
if (btvu(15,(char *)tmpbuf,kbf,0,sizeof(struct statbf)) != 0) 
	{
	btverr("STAT");
	return(0);
	}
maxofa=0;
for (i=0,keyno=0 ; keyno < tmpbuf->fs.numofx ; i++,keyno++) 
	{
	klen=tmpbuf->ks[i].keylen;
	while (tmpbuf->ks[i].flags&ANOSEG) 
		{
		klen+=tmpbuf->ks[++i].keylen;
		}
	bb->keylns[keyno]=klen;
	if (klen > maxofa) 
		{
		maxofa=klen;
		}
	}
free((char *)tmpbuf);
return(maxofa);
}

int	omdbtv(mode)                  /* set opnbtv() file-open mode               */
int mode;
{
     bbomode=mode;
return(1);
}

int	setbtv(bbptr)                 /* set a Btrieve data block for use          */
struct btvblk *bbptr;
{
bb=bbptr;
return(1);
}

int	qrybtv(key,keynum,qryopt)     /* query Btrieve operations utility          */
char *key;
int keynum;
int qryopt;
{
if (key != NULL && bb != NULL) 
	{
	movmem(key,bb->key,bb->keylns[keynum]);
	}
if (keynum < 0) 
	{
	keynum=bb->lastkn;
	}
else 
	{
	bb->lastkn=keynum;
	}
if (btvu(qryopt,NULL,bb->key,keynum,bb->reclen) != 0) 
	{
	if (status == 4 || status == 9) 
		{
		return(0);
		}
	btverr("QUERY");
	return(0);
	}
return(1);
}

int	qnpbtv(getopt)                /* query next/previous Btrieve utility       */
int getopt;
{
if (btvu(getopt-50,bb->data,bb->key,bb->lastkn,bb->reclen) != 0) 
	{
	if (status == 4 || status == 9) 
		{
		return(0);
		}
	btverr("QUERY-NP");
	return(0);
	}
return(1);
}

int	getbtv(recptr,key,keynum,getopt)   /* get Btrieve operations utility       */
char *recptr;
char *key;
int keynum;
int getopt;
{
if (recptr == NULL) 
	{
	recptr=bb->data;
	}
if (key != NULL && bb != NULL) 
	{
	movmem(key,bb->key,bb->keylns[keynum]);
	}
if (keynum < 0) 
	{
	keynum=bb->lastkn;
	}
else 
	{
	bb->lastkn=keynum;
	}
if (btvu(getopt,recptr,bb->key,keynum,bb->reclen) != 0) 
	{
	btverr("GET");
	return(0);
	}
return(1);
}

int	obtbtv(recptr,key,keynum,obtopt)   /* "acquire" Btrieve operations utiltiy */
char *recptr;
char *key;
int keynum;
int obtopt;
{
     if (recptr == NULL) {
          recptr=bb->data;
     }
     if (key != NULL && bb != NULL) {
          movmem(key,bb->key,bb->keylns[keynum]);
     }
     if (keynum < 0) {
          keynum=bb->lastkn;
     }
     else {
          bb->lastkn=keynum;
     }
     if (btvu(obtopt,recptr,bb->key,keynum,bb->reclen) != 0) {
          if (status == 4 || status == 9) {
               return(0);
          }
          btverr("OBTAIN");
			 return(0);
     }
     return(1);
}

int	anpbtv(recptr,anpopt)         /* "acquire" next/previous Btrieve utility   */
char *recptr;
int anpopt;
{
     movmem(bb->key,bb->data,bb->keylns[bb->lastkn]);
     if (obtbtv(recptr,NULL,-1,anpopt) && strcmp(bb->data,bb->key) == 0) {
          return(1);
     }
     return(0);
}

long absbtv(void)                      /* return current Btrieve file position      */
{
     long abspos;

     if (btvu(22,(char *)&abspos,NULL,0,sizeof(long)) != 0) {
          btverr("ABSOLUTE");
			 return(0);
     }
     return(abspos);
}

int	gabbtv(recptr,abspos,keynum)  /* get Btrieve record from a file position   */
char *recptr;
long abspos;
int keynum;
{
     if (recptr == NULL) {
          recptr=bb->data;
     }
     *(long *)recptr=abspos;
     bb->lastkn=keynum;
     if (btvu(23,recptr,bb->key,keynum,bb->reclen) != 0) {
          btverr("GET-ABSOLUTE");
			 return(0);
     }
return(1);
}

int	aabbtv(recptr,abspos,keynum)  /* "acquire" a record from a file position   */
char *recptr;
long abspos;
int keynum;
{
     if (recptr == NULL) {
          recptr=bb->data;
     }
     *(long *)recptr=abspos;
     bb->lastkn=keynum;
     return(btvu(23,recptr,bb->key,keynum,bb->reclen) == 0);
}

int	sabbtv(recptr)                /* step "absolute" Btrieve operations        */
char *recptr;
{
     if (recptr == NULL) {
          recptr=bb->data;
     }
     if (btvu(24,recptr,NULL,0,bb->reclen) != 0) {
          btverr("STEP-ABSOLUTE");
			 return(0);
     }
return(1);
}

int	updbtv(recptr)                /* "update" a Btrieve record                 */
char *recptr;
{
     upvbtv(recptr,bb->reclen);
return(1);
}

int	upvbtv(recptr,length)         /* "update" a variable-length Btrieve record */
char *recptr;
int length;
{
     if (recptr == NULL) {
          recptr=bb->data;
     }
     if (btvu(3,recptr,bb->key,bb->lastkn,length) != 0) {
          btverr("UPDATE");
			 return(0);
     }
return(1);
}

int	insbtv(recptr)                /* insert a new Btrieve record               */
char *recptr;
{
     invbtv(recptr,bb->reclen);
return(1);
}

int	invbtv(recptr,length)         /* insert new variable length Btrieve record */
char *recptr;
int length;
{
     if (recptr == NULL) {
          recptr=bb->data;
     }
     if (btvu(2,recptr,bb->key,0,length) != 0) {
          btverr("INSERT");
			 return(0);
     }
return(1);
}

int	delbtv(void)                      /* delete the current Btrieve record         */
{
     if (btvu(4,NULL,NULL,bb->lastkn,bb->reclen) != 0) {
          btverr("DELETE");
			 return(0);
     }
return(1);
}

int	clsbtv(bbp)                   /* close specified Btrieve file              */
struct btvblk *bbp;
{
     if ((bb=bbp) != NULL && bb->filnam != NULL) {
          if (btvu(1,NULL,NULL,0,0) != 0) {
               printf("BTRIEVE CLOSE ERROR %d ON FILE \"%s\"",status,bb->filnam);
					return(0);
          }
          free(bb->key);
          free(bb->data);
          free(bb->filnam);
          bb->filnam=NULL;
          free((char *)bb);
     }
return(1);
}

int	btverr(who)
char *who;
{
     char buff[40];

     sprintf(buff,"%s ERROR %d",who,status);
     printf("\r\nBTRIEVE %s ON FILE \"%s\"\r\n",buff,bb->filnam);
return(1);
}

struct btvdat {   /* btrieve parameter block structure for use with INT 0x7B */
     char *datbuf;
     int dbflen;
     char *posp38;
     char *posblk;
     int funcno;
     char *key;
     char keylen;
     char keyno;
     int *statpt;
     int magic;
};

int	btvu(funcno,datbuf,key,keyno,rlen) /* underlying Btrieve command interface */
int funcno;
char *datbuf;
char *key;
int keyno;
int rlen;
{
union REGS regs;
struct SREGS sregs;
struct btvdat btvdat;
static int btrvup=0,alrcat=0;

if (!btrvup) 
	{
	regs.x.ax=0x357B;
	int86x(0x21,&regs,&regs,&sregs);
	if (regs.x.bx != 0x33) 
		{
		if (alrcat) 
			{
		  	return(0);
	 	 	}
		alrcat=1;
		printf("\14\n\n\nRun BTRIEVE first!\n");
		return(0);
		}
	btrvup=1;
	}
btvdat.datbuf=datbuf;
btvdat.dbflen=rlen;
btvdat.posp38=((char *)bb->posblk)+38;
btvdat.posblk=(char *)bb->posblk;
btvdat.funcno=funcno;
btvdat.key=key;
btvdat.keylen=(char)255;
btvdat.keyno=(char)keyno;
*(btvdat.statpt=&status)=0;
btvdat.magic=24950;
regs.x.dx=(short)&btvdat;
segread(&sregs);
sregs.ds=sregs.ss;
int86x(0x7B,&regs,&regs,&sregs);
if (status == 22) 
	{                     /* truncate buffer overflows  */
	datbuf[bb->reclen-1]='\0';
	status=0;
	}
return(status);
}


int	btvup(void)
{
union REGS regs;
struct SREGS sregs;

regs.x.ax=0x357B;
int86x(0x21,&regs,&regs,&sregs);
if (regs.x.bx != 0x33) 
	{
	return(0);
	}
return(1);
}

char *alcmem(size)                       /* allocate dynamic memory              */
unsigned size;
{
     char *retval;

     if ((retval=malloc(size)) == NULL) {
          printf("FREE MEMORY POOL EXHAUSTED");
     }
     return(retval);
}

char *secure(void)
{
return(regno);
}

int	honk(void)
{
sound(1000);
delay(2500);
nosound();
return(1);
}

void	file_write(FILE *hdl, char *buf, int flg, char *key)
{
static int	size;

size = strlen(buf);

fwrite("$$$",sizeof(char),3,hdl);

fwrite(&size,sizeof(int),1,hdl);

if (flg)
	bin_encode(buf,strlen(buf),key);

fwrite(buf,sizeof(char),size,hdl);

}
