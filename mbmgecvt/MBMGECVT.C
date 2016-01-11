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
 *   BTVSTF.C                                                              *
 *                                                                         *
 *   Copyright (C) 1987,1988,1990 GALACTICOMM, Inc.   All Rights Reserved. *
 *                                                                         *
 *   This file contains a library of routines for accessing BTRIEVE files  *
 *   with fixed or variable length records.                                *
 *                                                                         *
 *                                            - T. Stryker 9/30/86         *
 *                                                                         *
 ***************************************************************************/

#include "string.h"
#include "dos.h"
#include "conio.h"
/*#include "stdlib.h"*/
#include "stdio.h"
#include "mem.h"
#include <math.h>
#include "mbmgecvt.h"
#include "mbmgecvt.hp"


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


/****************************** O L D   S T U F F ****************************/


#define O_ITEM    struct o_item
extern
O_ITEM    {
	unsigned long	qty;                            /* quantity on hand                                                     */
	unsigned			rate;                                           /* rate of manufacture                                          */
	char				sell;                                           /* sell to allies?                                                      */
	unsigned			reserve;                                        /* qty to reserve                                                       */
	unsigned			markup2a;                               /* mark up to allies                                                    */
	unsigned	long	sold2a;                    /* # sold to allies                                                     */
	};


#define GALPLNT_OLD struct _galplnt_old

GALPLNT_OLD {                /* star system data record               */
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
	O_ITEM	items[NUMITEMS]; /* 195 */
	char	beacon[BEACONMSGSZ];
	char	spyowner[UIDSIZ];
	int	technology;
	unsigned long	teamcode;
	char	filler[512-182-(UIDSIZ*2)-(sizeof(O_ITEM)*NUMITEMS)];         


};


GALPLNT_OLD	old;


#define O_GALSECT struct o_galsect

extern
O_GALSECT {                /* star system data record               */
	int             xsect;                                  /* sector x coord                       */
	int             ysect;                                  /* sector y coord                       */
	int             plnum;                                  /* always 0 */
	int             type;                /* type of sector */

	int             numplan;             /* number of planetary objects */
	PLNTCOORD       ptab[MAXPLANETS];
	char            filler[512-10-(sizeof(PLNTCOORD)*MAXPLANETS)];          /* fill to 250*/
};



O_GALSECT	oldsect;


#define O_GALWORM struct o_galworm

extern
O_GALWORM {                /* star system data record               */
	int             xsect;                                  /* sector x coord                       */
	int             ysect;                                  /* sector y coord                       */
	int             plnum;               /* planet number */
	int             type;                /* type of sector */
	COORD           coord;
	int             visible;             /* is the wormhole visible flag */
	COORD           destination;
	char            filler[512-10-(sizeof(COORD)*2)];         /* fill to 250*/
};


O_GALWORM	oldworm;

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

void 	main()

{

unsigned int cnt;
int	i;
static long     fpos = 0;
int	bad;

clrscrx();

do 
	{
	printf("\rEnter your 8 digit GE registration number:");
	scanf("%8s",regno);
	}
while (strlen(regno) !=8);

#ifdef CONVERT

printf("Begin conversion.\r\n");

gebbold=opnbtv("MBMGEPLT.DAT",sizeof(old));
gebbnew=opnbtv("MBMGEPLT.DA2",sizeof(new));


setbtv(gebbold);

if (qlobtv(0))
	{
	do
		{
		printf("Processing %d\r",cnt++);
		gcrbtv(&old,0);


		validate();

		move_old_to_new();



		setbtv(gebbnew);
		insbtv(&new);

		setbtv(gebbold);
		} while (qnxbtv());
	}

printf("\r\n\r\nConversion Complete\r\n");

clsbtv(gebbold);
clsbtv(gebbnew);
#endif

printf("Begin Audit of New Database.\r\n");


gebbnew=opnbtv("MBMGEPLT.DA2",sizeof(new));


setbtv(gebbnew);

cnt=0;

if (qlobtv(0))
	{
	do
		{
		printf("Processing %d\r\n",cnt++);
		gcrbtv(&newsect,0);

		fpos=absbtv();

		if (newsect.type == 1) /* if sector record */
			{
			printf("Sector %d %d - %d planets\r\n",newsect.xsect,newsect.ysect,newsect.numplan);
			for (i=0;i<newsect.numplan;++i)
				{
				new.xsect=newsect.xsect;
				new.ysect=newsect.ysect;
				new.plnum=i+1;

				bad = 0;
				if (qeqbtv(&new, 0))
					{
					gcrbtv(&new,0);
					if (new.coord.xcoord != newsect.ptab[i].coord.xcoord)
						{
						printf("BAD PTAB XCOORD Pl# %d Plnt is %f - Sect is %f\r\n",i+1,
							new.coord.xcoord,newsect.ptab[i].coord.xcoord);
						bad = 1;
						honk();
						}
					if (new.coord.ycoord != newsect.ptab[i].coord.ycoord)
						{
						printf("BAD PTAB YCOORD Pl# %d Plnt is %f - Sect is %f\r\n",i+1,
							new.coord.ycoord,newsect.ptab[i].coord.ycoord);
						bad = 1;
						honk();
						}
					if (new.type != newsect.ptab[i].type)
						{
						printf("BAD PTAB TYPE Pl# %d Plnt is %d - Sect is %\r\n",i+1,
							new.type,newsect.ptab[i].type);
						bad = 1;
						honk();
						}

					}
				else
					{
					printf("PLANET RECORD MISSING Pl# %d\r\n",i+1);
					bad = 1;
					honk();
					}
				if (bad == 0)
					printf("Planet # %d Good\r\n",i+1);

				}

			}
		else
		if (newsect.type == 2) /* if planet record */
			{
			printf("Planet #%d in sector %d %d\r\n",newsect.plnum,newsect.xsect,newsect.ysect);

			bad = 0;

			/* check out all the planets stuff */

			gcrbtv(&new,0);

			printf("Owner = [%s]\r\n",new.userid);
			printf("Name  = [%s]\r\n",new.name);
			printf("Env   = [%d]\r\n",new.enviorn);
			if (new.enviorn > 3)
				{
				printf("Env BAD!!!\r\n");
				bad = 1;
				}

			printf("Res   = [%d]\r\n",new.resource);

			if (new.resource > 3)
				{
				printf("Res BAD!!!\r\n");
				bad = 1;
				}

			printf("Cash  = [%lu]\r\n",new.cash);
			if (new.cash > 1000000000)
				{
				printf("CASH TOO BIG - %lu\r\n",new.cash);
				bad = 1;
				}

			printf("tax  = [%lu]\r\n",new.tax);
			if (new.tax > 1000000000)
				{
				printf("TAX TOO BIG - %lu\r\n",new.tax);
				bad = 1;
				}

			printf("taxrate  = [%d]\r\n",new.taxrate);
			if (new.taxrate > 100)
				{
				printf("TAXRATE TOO BIG - %d\r\n",new.taxrate);
				bad = 1;
				}

			printf("Password  = [%s]\r\n",new.password);

			printf("Last Attack  = [%s]\r\n",new.lastattack);
			printf("Beacon  = [%s]\r\n",new.beacon);
			printf("Spy Owner  = [%s]\r\n",new.spyowner);
			printf("Teamcode  = [%lu]\r\n",new.teamcode);


			for (i=0;i<NUMITEMS;++i)
				{
				printf("Item # %d ",i+1);
				if (new.items[i].qty > 1000000000)
					{
					printf("ITEM QTY TO BIG %l\r\n",new.items[i].qty);
					bad = 1;
					}
				if (new.items[i].rate > 100 )
					{
					printf("ITEM RATE TO BIG %u\r\n",new.items[i].rate);
					bad = 1;
					}

				if (new.items[i].markup2a > 60000)
					{
					printf("ITEM markup2a TO BIG %u\r\n",new.items[i].markup2a);
					bad = 1;
					}

				if (new.items[i].sold2a > 1000000000)
					{
					printf("ITEM sold2a TO BIG %l\r\n",new.items[i].sold2a);
					bad = 1;
					}
				printf("Qty=%lu, rate=%u, markup=%u, sold=%lu\r\n",
					new.items[i].qty,new.items[i].rate,new.items[i].markup2a,new.items[i].sold2a);
					

				}
						
			newsect.plnum = 0;
						
			if (qeqbtv(&newsect, 0))
				{
				gcrbtv(&newsect,0);
				if (newsect.type != 1)
					{
					printf("SECTOR RECORD NOT TYPE 1 - It is # %d\r\n",newsect.type);
					delbtv();
					printf("***DELETED***\r\n");

					bad = 1;
					honk();
					}
				}
			else
				{
				printf("SECTOR RECORD MISSING Pl# %d\r\n",i+1);
				delbtv();
				printf("***DELETED***\r\n");
				bad = 1;
				honk();
				}

					
			if (bad == 0)
				{
				printf("Planet Good\r\n");
				}

			}
		else
		if (newsect.type == 3) /* if worm record */
			{
			printf("Worm #%d in sector %d %d\r\n",newsect.plnum,newsect.xsect,newsect.ysect);

			movmem(&newsect,&newworm, sizeof(newworm));

			printf("Terminal sector is X:%f Y:%f\r\n",
				newworm.destination.xcoord,newworm.destination.ycoord);

			newsect.xsect = (int)floor(newworm.destination.xcoord);
			newsect.ysect = (int)floor(newworm.destination.ycoord);
			newsect.plnum = 0;

			bad = 0;
			if (qeqbtv(&newsect, 0))
					{
					gcrbtv(&newsect,0);
					}
				else
					{
					printf("TERMINAL RECORD MISSING\r\n");
					bad = 1;
					honk();
					}
			if (bad == 0)
				printf("Worm Good\r\n");

			}
		else
			{
			printf("BAD TYPE %d - %c\r\n",newsect.type,newsect.type);

			movmem(&newsect,&buffer[0], sizeof(newsect));
			printf("--------------------------------------------------------------\r\n");
			for (i=0;i<sizeof(newsect);++i)
				printf("%c",buffer[i]);
			printf("--------------------------------------------------------------\r\n");
			honk();
			delbtv();
			printf("***DELETED***\r\n");
			}

		gabbtv(&newsect,fpos,0);

		} while (qnxbtv());
	}

printf("\r\n\r\nAudit Complete\r\n");

clsbtv(gebbnew);


return;
}


void	validate(void)
{

int bad;

int i;

bad=0;


if (old.plnum > 9) bad = 1;

if (old.type > 3 || old.type == 0 ) bad = 1;

if (old.type == 2)
	{

	if (old.enviorn > 3) bad = 1;
	if (old.resource > 3) bad = 1;

	if (old.taxrate > 100) bad = 1;

	for (i=0;i<NUMITEMS;++i)
		{
		if (old.items[i].rate > 100) bad = 1;
		}
	}
	
if (bad != 0)
	printf("Found bad record\r\n");
	
}


void	move_old_to_new(void)
{

int i;


new.xsect 			= 	old.xsect;
new.ysect 			= 	old.ysect;
new.plnum 			= 	old.plnum;
new.type				= 	old.type;


new.coord.xcoord 	= 	old.coord.xcoord;
new.coord.ycoord 	= 	old.coord.ycoord;


strncpy(new.userid,	old.userid,UIDSIZ);
strncpy(new.name,		old.name,UIDSIZ);


new.enviorn			=	old.enviorn;
new.resource		=	old.resource;
new.cash				=	old.cash;
new.debt				=	old.debt;
new.tax				=	old.tax;
new.taxrate			=	old.taxrate;
new.warnings		=	old.warnings;


strncpy(new.password,	old.password,10);
strncpy(new.lastattack,	old.lastattack,UIDSIZ);


for (i=0;i<NUMITEMS;++i)
	{
	new.items[i].qty			=	old.items[i].qty;
	new.items[i].rate			=	old.items[i].rate;
	new.items[i].sell			=	old.items[i].sell;
	new.items[i].reserve		=	old.items[i].reserve;
	new.items[i].markup2a	=	old.items[i].markup2a;
	new.items[i].sold2a		=	old.items[i].sold2a;
	}


}





void clrscrx()
{
clrscr();
gotoxy(1,1);
printf(" MBMGECV: Galactic Empire Database Conversion Utility V3.0n \r\n");
printf("          (c) Copyright 1992 M.B. Murdock & Associates\r\n");
printf("\r\n\r\n\r\n");
return;
}

void clrscr1()
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

void clrscr2()
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
     if (btvu(0,secure(),filnam,bbomode,bb->reclen) != 0) {
          btverr("OPEN");
     }
     bb->key=alcmem(clckln());
     return(bb);
}


clckln()                      /* calculate all key lengths in curr file    */
{                             /*   (and return the max of them all)        */
     static char kbf[64];
     int i,keyno,klen,maxofa;
     struct statbf *tmpbuf;

     tmpbuf=(struct statbf *)alcmem(sizeof(struct statbf));
     if (btvu(15,tmpbuf,kbf,0,sizeof(struct statbf)) != 0) {
          btverr("STAT");
     }
     maxofa=0;
     for (i=0,keyno=0 ; keyno < tmpbuf->fs.numofx ; i++,keyno++) {
          klen=tmpbuf->ks[i].keylen;
          while (tmpbuf->ks[i].flags&ANOSEG) {
               klen+=tmpbuf->ks[++i].keylen;
          }
          bb->keylns[keyno]=klen;
          if (klen > maxofa) {
               maxofa=klen;
          }
     }
     free(tmpbuf);
     return(maxofa);
}

omdbtv(mode)                  /* set opnbtv() file-open mode               */
int mode;
{
     bbomode=mode;
}

setbtv(bbptr)                 /* set a Btrieve data block for use          */
struct btvblk *bbptr;
{
     bb=bbptr;
}

qrybtv(key,keynum,qryopt)     /* query Btrieve operations utility          */
char *key;
int keynum;
int qryopt;
{
     if (key != NULL && bb != NULL) {
          movmem(key,bb->key,bb->keylns[keynum]);
     }
     if (keynum < 0) {
          keynum=bb->lastkn;
     }
     else {
          bb->lastkn=keynum;
     }
     if (btvu(qryopt,NULL,bb->key,keynum,bb->reclen) != 0) {
          if (status == 4 || status == 9) {
               return(0);
          }
          btverr("QUERY");
     }
     return(1);
}

qnpbtv(getopt)                /* query next/previous Btrieve utility       */
int getopt;
{
     if (btvu(getopt-50,bb->data,bb->key,bb->lastkn,bb->reclen) != 0) {
          if (status == 4 || status == 9) {
               return(0);
          }
          btverr("QUERY-NP");
     }
     return(1);
}

getbtv(recptr,key,keynum,getopt)   /* get Btrieve operations utility       */
char *recptr;
char *key;
int keynum;
int getopt;
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
     if (btvu(getopt,recptr,bb->key,keynum,bb->reclen) != 0) {
          btverr("GET");
     }
}

obtbtv(recptr,key,keynum,obtopt)   /* "acquire" Btrieve operations utiltiy */
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
     }
     return(1);
}

anpbtv(recptr,anpopt)         /* "acquire" next/previous Btrieve utility   */
char *recptr;
int anpopt;
{
     movmem(bb->key,bb->data,bb->keylns[bb->lastkn]);
     if (obtbtv(recptr,NULL,-1,anpopt) && strcmp(bb->data,bb->key) == 0) {
          return(1);
     }
     return(0);
}

long
absbtv()                      /* return current Btrieve file position      */
{
     long abspos;

     if (btvu(22,&abspos,NULL,0,sizeof(long)) != 0) {
          btverr("ABSOLUTE");
     }
     return(abspos);
}

gabbtv(recptr,abspos,keynum)  /* get Btrieve record from a file position   */
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
     }
}

aabbtv(recptr,abspos,keynum)  /* "acquire" a record from a file position   */
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

sabbtv(recptr)                /* step "absolute" Btrieve operations        */
char *recptr;
{
     if (recptr == NULL) {
          recptr=bb->data;
     }
     if (btvu(24,recptr,NULL,0,bb->reclen) != 0) {
          btverr("STEP-ABSOLUTE");
     }
}

updbtv(recptr)                /* "update" a Btrieve record                 */
char *recptr;
{
     upvbtv(recptr,bb->reclen);
}

upvbtv(recptr,length)         /* "update" a variable-length Btrieve record */
char *recptr;
int length;
{
     if (recptr == NULL) {
          recptr=bb->data;
     }
     if (btvu(3,recptr,bb->key,bb->lastkn,length) != 0) {
          btverr("UPDATE");
     }
}

insbtv(recptr)                /* insert a new Btrieve record               */
char *recptr;
{
     invbtv(recptr,bb->reclen);
}

invbtv(recptr,length)         /* insert new variable length Btrieve record */
char *recptr;
int length;
{
     if (recptr == NULL) {
          recptr=bb->data;
     }
     if (btvu(2,recptr,bb->key,0,length) != 0) {
          btverr("INSERT");
     }
}

delbtv()                      /* delete the current Btrieve record         */
{
     if (btvu(4,NULL,NULL,bb->lastkn,bb->reclen) != 0) {
          btverr("DELETE");
     }
}

clsbtv(bbp)                   /* close specified Btrieve file              */
struct btvblk *bbp;
{
     if ((bb=bbp) != NULL && bb->filnam != NULL) {
          if (btvu(1,NULL,NULL,0,0) != 0) {
               printf("BTRIEVE CLOSE ERROR %d ON FILE \"%s\"",status,bb->filnam);
          }
          free(bb->key);
          free(bb->data);
          free(bb->filnam);
          bb->filnam=NULL;
          free(bb);
     }
}

btverr(who)
char *who;
{
     char buff[40];

     sprintf(buff,"%s ERROR %d",who,status);
     printf("BTRIEVE %s ON FILE \"%s\"",buff,bb->filnam);
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

btvu(funcno,datbuf,key,keyno,rlen) /* underlying Btrieve command interface */
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

     if (!btrvup) {
          regs.x.ax=0x357B;
          int86x(0x21,&regs,&regs,&sregs);
          if (regs.x.bx != 0x33) {
               if (alrcat) {
                    return(0);
               }
               alrcat=1;
               printf(
               "\14\n\n\nRun BTRIEVE /P:2048 first, before running The Major BBS!\n");
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
     if (status == 22) {                     /* truncate buffer overflows  */
          datbuf[bb->reclen-1]='\0';
          status=0;
     }
     return(status);
}


char *
alcmem(size)                       /* allocate dynamic memory              */
unsigned size;
{
     char *retval;

     if ((retval=malloc(size)) == NULL) {
          printf("FREE MEMORY POOL EXHAUSTED");
     }
     return(retval);
}

char *secure()
{
return(regno);
}

int	honk(void)
{
sound(1000);
delay(2500);
nosound();
}

