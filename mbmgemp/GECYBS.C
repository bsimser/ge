
/***************************************************************************
 *                                                                         *
 *   GECYBS.C                                                              *
 *                                                                         *
 *   Copyright (C) 1988, 89, 90, 91, 92 Michael B. Murdock                 *
 *                                                                         *
 *   This is the source for the Galactic Empire game module                *
 *                                                                         *
 *                                         M. Murdock     03/17/92         *
 *                                                                         *
 ***************************************************************************/

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
 
/* NOTE: I like my tabs at 3 so if the code looks "messed up" try changing */
/*       your tabs to 3....    MBM                                         */

#ifdef PHARLAP

#include "gcomm.h"
#include "string.h"

#else

#include "stdio.h"
#include "ctype.h"
#include "dos.h"
#include "usracc.h"
#include "btvstf.h"
#include "stdlib.h"
#include "math.h"
#include "portable.h"
#include "dosface.h"
#endif

#include "majorbbs.h"
#include "message.h"

#include "gemain.h"


#define  GECYBS   1

#include "geglobal.h"

/* LOCAL GLOBAL DEFS ****************************************************/

char        cybname[UIDSIZ]; /*BJ REMOVED CYBORG- name*/
int			cybhaltflg = 0;


double		d_topspeed;

/**************************************************************************
** Cyborg functions                                                      **
**************************************************************************/

void  FUNC cyb_init(ptr, usrn, class)
WARSHP *ptr;
int   usrn;
int   class;

{

logthis(spr("@Cyb_init usrn=%d,class=%d",usrn,class));


if (usrn < 0 || usrn >= nships)
	{
	logthis(spr("CYB_INIT:bad usrn [%d]",usrn));
	return;
	}

strncpy(cybname,"@Cybrg-",UIDSIZ);/* Bj Added name here */
sprintf(&cybname[7],"%d",usrn);

if (!(geudb(GELOOKUP,cybname, &tmpusr)))
	{
	initusr(cybname);
	geudb(GEADD,tmpusr.userid,&tmpusr);
	logthis(spr("GE:INF:Adding %s user",tmpusr.userid)); 
	}

waruptr = warusroff(usrn);
warsptr = warshpoff(usrn);

if (geudb(GELOOKUP,cybname, waruptr))
	{
	geudb(GEGET,cybname, waruptr);

	if (waruptr->cash > CYB_MAXCASH)
		waruptr->cash = CYB_MAXCASH;

	logthis(spr("GE:INF:Load %s user",waruptr->userid)); 

	if (gepdb(GELOOKUPNAME,cybname,0,ptr))
		{
		gcrbtv(ptr,0);
		logthis(spr("GE:INF:Load %s ship",ptr->userid)); 

		ptr->status = GESTAT_AUTO;
		ptr->phasr = 100;
		ptr->cybmine = (byte)255;
		ptr->speed2b = (double)(ptr->topspeed)*500.0;
		ptr->cybupdate = 100 + gernd()%20;
		ptr->holdcourse = 0;
		ptr->tick = CYBTICKTIME + gernd()%(CYBTICKTIME*5);

		/* SANITY CHECK */
		if (shipclass[ptr->shpclass].max_type != CLASSTYPE_CYBORG)
			{
			geshocst(0,spr("GE:ERR:NOTCYBCLS %d",ptr->shpclass));
			}
		}
	else
		{
		/* make me a Cybertron or a Cyberquad */
		logthis(spr("GE:INF:Adding %s ship - %d",ptr->userid,class));

		initshp(cybname,class);
		gepdb(GEADD,tmpshp.userid,tmpshp.shipno,&tmpshp);
		memcpy(ptr,&tmpshp,sizeof(WARSHP));  /* make is the current ship */

		logthis(spr("GE:INF:Add shp,cls=%d/%d",class,ptr->shpclass));
		sprintf(ptr->shipname,"%s%u\0",shipclass[class].shipname,usrn*usrn+gernd()%100);
		logthis(spr("  Named: %s",ptr->shipname));

		ptr->coord.xcoord    = rndm((double)univmax*2.0)-(double)univmax;
		ptr->coord.ycoord    = rndm((double)univmax*2.0)-(double)univmax;
		ptr->phasrtype = shipclass[class].max_phasr;
		ptr->shieldtype = shipclass[class].max_shlds;
		ptr->cybmine = (byte)255;


		ptr->items[I_FLUXPOD] = gernd()%5;
		ptr->items[I_DECOYS] = gernd()%25;
		ptr->items[I_TORPEDO] = gernd()%25;
		ptr->items[I_MINE] = gernd()%100;
		ptr->items[I_JAMMERS] = gernd()%100;
		ptr->items[I_GOLD] = gernd()%cyb_gold;

		ptr->holdcourse = 0;

/* cyborg skill level is his propensity to make an error, the higher the #
	the less chance he will make an error                                   */

		ptr->cybskill = (byte)gernd()%15+3;
/*		
		ptr->cybskill = 100;*/

		ptr->status = GESTAT_AUTO;
		ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME;
		/* DEBUG */

		gepdb(GEUPDATE,ptr->userid,ptr->shipno,ptr);
		prfmsg(CYBNEW,gernd()%359);
		outwar(FILTER,usrn,0);
		}
	}
else
	{
	/* DEBUG */
	geshocst(0,spr("GE:ERR:NO FIND %s user",cybname));
	}
}


void  FUNC cyb_lives(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

WARSHP   *wptr;
int      zothusn;
int		i;

double   ddist; 


if (!sameas(ptr->userid,warusroff(usrn)->userid))
	geshocst(0,"GE:ERR:Cyb Names !=");
	
i = usrn;
++i;

sprintf(&cybname[7],"%d",i);

logthis(spr("@cyb_lives %s",cybname)); 


/* reset the ticker to 255 to cause it to recalc */
ptr->tick = 255;

/* save off the topspeed in 1000's */
d_topspeed = (double)ptr->topspeed*1000.0;

/* allowance money for cybertrons */
warusroff(usrn)->cash += CYB_ALLOW;

/* countdown to database update */

db_update(ptr,usrn);

/* am I being jammed ? */
if (ptr->jammer == 0)
	{
	/* look at all the other ships */
	for (zothusn=0 ; zothusn < nterms ; zothusn++)
		{
		wptr=warshpoff(zothusn);
		/* if not me, and playing, and not cyborg, go getem */
		if (user[zothusn].state == gestt 
			&& user[zothusn].substt >= FIGHTSUB
			&& wptr->cloak != 10)
			{

			ddist = cdistance(&ptr->coord,&wptr->coord);
			ddist *= 10000;

			if (!neutral(&ptr->coord) 
				&& ddist < (double)shipclass[ptr->shpclass].scanrange)
				{
				/* if ordinary cyb see if this is his lucky day */
				if (isquad(ptr) && gernd()%CYB_BREAKOFF == 0)
					{
					prfmsg(CYBLUCK,ptr->shipname);
					outprfge(FILTER,zothusn);
					ptr->cybmine = (byte)255; /* take a break */

					ptr->speed2b = d_topspeed;
					}
	
				/* fire phasers at the fool */
	
				if (ptr->where == 1 
					&& wptr->where == 1 
					&& gebemean(ptr,zothusn))
					{
					if (ddist < (tooclose+rndm(tooclose))
						|| shipclass[wptr->shpclass].cybs_can_att 
						|| wptr->cantexit > 0 
						|| ptr->cantexit > 0)
						{
						if (ddist < 30000.0)
							{
							ptr->degrees = (int)(cbearing(&ptr->coord,&wptr->coord,ptr->heading)+.5);
							firehp(ptr,usrn);
							}
						}
					}
				else
				if (ptr->where == 0 
					&& wptr->where != 1)
					{
					ptr->degrees = (int)(cbearing(&ptr->coord,&wptr->coord,ptr->heading)+.5);
					ptr->percent = 2;
	
					/* if the guy gets closer then 2000 or is not little guy or has fired */
					if (ddist < (tooclose + rndm(tooclose))
						|| shipclass[wptr->shpclass].cybs_can_att 
						|| wptr->cantexit > 0)
						{
						cyb_attack(ptr,usrn,wptr,zothusn);
						cyb_annoy(ptr,zothusn,20,13,16);
						cyb_lay_decoys(ptr,zothusn);
						}
					else
						{
						cyb_annoy(ptr,zothusn,20,9,12);
						}
					}
				}
			}
		}
	}
else
	{
	/* as long as they can't see ... the other player must be trying to get
		away.... might as well mine the area */

	if (shipclass[ptr->shpclass].has_mine 
		&& ptr->items[I_MINE] > 0 
		&& gernd()%5 == 0)
		laymine(ptr,usrn,10);

	ptr->speed2b = d_topspeed + rndm(3000.0);
	ptr->holdcourse = gernd()%7 + 2;
	}

cyb_check_damage(ptr,usrn);

cyb_check_lockon(ptr,usrn);

ptr->energy = 50000L;

/*DEBUG
geshocst(0,spr("GE:cm %d",(int)ptr->cybmine)); */

/* check for jammer */
if (ptr->jammer > 0)
	{

	ptr->speed2b = d_topspeed + rndm(3000.0);
	ptr->holdcourse = gernd()%10 + 5;
	}

if (ptr->tick == 255)
	{
	/* if just cruising around don't get back to me for some time */
	if (ptr->cantexit == 0)
		{
		ptr->tick = (CYBTICKTIME + gernd()%CYBTICKTIME)*5;
		}
	else
		{
		if (!isquad(ptr))
			ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME;
		else
			ptr->tick = 2 + gernd()%CYBTICKTIME;
		}
	}
}

/* look if another cyborg (or cyborgs) has this ship claimed */

int FUNC notclaimed(ptr,usrn)
WARSHP   *ptr;
int   usrn;
{
WARSHP   *wptr;
int zothusn,nc;

nc=0;
for (zothusn=nterms ; zothusn < nships ;zothusn++)
	{
	wptr=warshpoff(zothusn);
	if (wptr->status == GESTAT_AUTO && wptr->cybmine == (byte)usrn)
		++nc;
	}
/*DEBUG
geshocst(0,spr("nc=%d",nc));*/

logthis(spr("notclaimed: nc = %d, class = %d, class.noclaim = %d",nc,ptr->shpclass,shipclass[ptr->shpclass].noclaim));
return (nc < shipclass[ptr->shpclass].noclaim);
}

/* ptr to sender , usrn = reciever */
void  FUNC cyb_annoy(ptr,usrn,rnd,first,last)
WARSHP   *ptr;
int      usrn;
int      rnd;
int      first;
int      last;
{

int base;
int sel;

if ((gernd()%rnd) == 1)
	{
	base = CYBBASEM;
	base += (ptr->shpclass - cyb_class)*16;

	first = first+base;
	last = last+base;
	sel = first+gernd()%(last-first+1);

	if (sel < CYBLASTM)
		{
		prfmsg(sel,ptr->shipname);

		outprfge(FILTER,usrn);
		}
	sprintf(gechrbuf,"cyb_ann shnm=<%s> usrn=%d base=%d frst=%d lst=%d sel=%d",ptr->shipname,usrn, base, first, last, sel);
	logthis(gechrbuf);
	}
}

int    FUNC cybwhoops(ptr,zothusn)
WARSHP   *ptr;
int      zothusn;
{

unsigned err,i;
err = 0;

for (i=0; i<zothusn;++i)
	gernd();

if ((gernd()%(int)(ptr->cybskill)) == 1)
	{
	err = 1;
	if (gernd()%5 == 1)
		{
/*		cyb_annoy(ptr,zothusn,4,CYBWOOP1,CYBWOOP4);*/
		}
	}
return(err);
}

int  FUNC gebemean(ptr,usrn)
WARSHP   *ptr;
int      usrn;
{

/* cyberquads are ALWAYS mean */
if (isquad(ptr))
	return(1);

/* if this player has accumulated 50 kills play harder */
if (warusroff(usrn)->kills > CYB_BE_NICE)
	return(1);

if (gernd()%CYBSLO == 0)
	return(1);


return(0);
}


/* countdown to database update */

void  FUNC db_update(ptr,usrn)

WARSHP   *ptr;
int      usrn;

{
WARUSR	*wuptr;

if (ptr->cybupdate > 1)
	{
	--ptr->cybupdate;
	return;
	}
else
if (ptr->cybupdate == 1)
	{
	if (ptr->cybmine == 255)
		{
		ptr->speed2b = rndm(d_topspeed); /* change the direction */
		ptr->head2b = rndm(359.9);
		}
	ptr->cybupdate = 100 + gernd()%100;
	return;
	}
else
/* am I crazy or does this code never ever get executed???????????*/
if (ptr->cybupdate == 0)
	{
	wuptr= warusroff(usrn);
	logthis(spr("GE:DBG:Cyb UUpd %s",wuptr->userid));
	geudb(GEUPDATE,wuptr->userid,wuptr);
	logthis(spr("GE:DBG:Cyb PUpd %s",ptr->userid));
	gepdb(GEUPDATE,ptr->userid,ptr->shipno,ptr);
	ptr->cybupdate = 100 + gernd()%100;
	return;
	}

ptr->cybupdate = 20;
}

/**************************************************************************
** Attack the other player                                               **
**************************************************************************/

void  FUNC cyb_attack(ptr,usrn,wptr,zothusn)

WARSHP   *ptr;    /* ptr to cyb ship*/
int      usrn;    /* cybs ship number*/
WARSHP   *wptr;   /* ptr to users ship*/
int      zothusn;	/* users ship number*/

{


MISSILE  *mptr;
int i,j;

wptr = wptr; /* eliminates warning*/

if (ptr->phasr >= PMINFIRE 
	&& gebemean(ptr,zothusn))
	{
	/* geshocst(0,spr("GE:phaser fired %d",ptr->degrees));*/
	if (!cybwhoops(ptr,zothusn))
		firep(ptr,usrn);
	}

/* fire torpedoes at the fool */
j = gernd()%6;
if (!isquad(ptr) && warusroff(zothusn)->kills < CYB_BE_EASY)
	j = gernd()%2;

if (!gebemean(ptr,zothusn))
	j=0;

/* if this class has no torps don't fire any */
if (shipclass[ptr->shpclass].max_torps == 0)
	j = 0;

for (i=0;i<j;++i)
	{
	ptr->items[I_TORPEDO] = (gernd()%5)+1;
	if (i>0) lockwarn = FALSE;
	torp(ptr,usrn,zothusn);
	}

/* launch Zippers if needed */
if (gernd()%10 == 1 && shipclass[ptr->shpclass].has_zip)
	{
	if(ptr->minesnear == TRUE)
		{
		if (gernd()%3 == 1)
			{
			ptr->items[I_ZIPPERS] = 1;
			zip(ptr,usrn);
			ptr->minesnear = FALSE;
			}
		/* get the hell out of here ...then come back */
		ptr->speed2b = d_topspeed;
		ptr->head2b = rndm(359.9);
		ptr->holdcourse = gernd()%20 + 3;
		}
	}

/* just to confuse them sometimes alter attack vector */
if (gernd()%20 == 1)
	{
	ptr->speed2b = d_topspeed;
	ptr->head2b = rndm(359.9);
	ptr->holdcourse = gernd()%10 + 3;
	}
/* if we are in hyperspace and fighting and missiles detected
	get out of hyperspace and get shields up */

if (ptr->where == 1)
	{
	for (i=0,mptr=ptr->lmissl;i<MAXMISSL;++i,++mptr)
		{
		if (mptr->distance > 0)
			{
			ptr->speed2b = rndm(5000.00)+4500.00;
			ptr->holdcourse = gernd()%5 + 5;
			break;
			}
		}
	}
else
	{
	shieldup(ptr,usrn);
	}
/*
if (!cybwhoops(ptr,zothusn) && gebemean(ptr,zothusn))
	{
	prfmsg(SCAN1,ptr->shipname);
	outprfge(FILTER,zothusn);
	}
EXTRA MESSAGES WE JUST DON'T NEED */
}

/**************************************************************************
** Lay down some decoys                                                  **
**************************************************************************/

void  FUNC cyb_lay_decoys(ptr,zothusn)
WARSHP   *ptr;
int   zothusn;

{

int   i;

/* send out a decoy */
if (!cybwhoops(ptr,zothusn))
	{
	for (i=0; i<5;++i)
		if (ptr->decout[i] == 0)
			ptr->decout[i] = DECOYTIME;
	}
}

/**************************************************************************
** if hunting, and badly damaged dump mines, jam, and boogie             **
**************************************************************************/

void  FUNC cyb_check_damage(ptr,usrn)
WARSHP   *ptr;
int      usrn;

{

if (ptr->cybmine < 255 
	&& ptr->damage > CYB_MINDAM 
	&& (gernd()%10 == 0))
	{
	if (shipclass[ptr->shpclass].has_mine 
		&& ptr->items[I_MINE] > 0 
		&& gernd()%5 == 0)
		laymine(ptr,usrn,10);

	if (shipclass[ptr->shpclass].has_jam
		&& ptr->items[I_JAMMERS] > 0 
		&& gernd()%100 == 0)
		jam(ptr,usrn);
	
	ptr->speed2b = d_topspeed;
	ptr->head2b = rndm(359.9);
	ptr->holdcourse = gernd()%10 + 5;
	}
}

/**************************************************************************
** Check lockon status                                                   **
**************************************************************************/

void  FUNC cyb_check_lockon(ptr,usrn)
WARSHP   *ptr;
int      usrn;

{

WARSHP   *wptr;
int      zothusn;

double   ddist;

double   low_dist = 999999999.0;
int		low_ship;
int		lta; /* lowest to attack */

low_dist = 999999999.0;
low_ship = -1;


/* if cyborg not seeking - countdown */

zothusn = ptr->cybmine;

if (ptr->holdcourse > 0)
	{
	--(ptr->holdcourse);
	return;
	}

if (zothusn >= nterms)
	{
	ptr->cybmine = (byte)255;
	}
else
	{	
	if (!ingegame(zothusn))
		{
		ptr->cybmine = (byte)255;
		ptr->speed2b = rndm(d_topspeed); /* let them cruise */
		return;
		}

	wptr=warshpoff(zothusn);
	
	if (wptr->cloak == 10)
		{
		ptr->holdcourse=gernd()%5+5;
		ptr->speed2b = rndm(d_topspeed); /* let them cruise */

		/* if the guy is cloaked then give up after awhile */
		if(gernd()%10 == 0)
			ptr->cybmine = 255;
	
		return;
		}
	
	low_ship = zothusn;
	low_dist = cdistance(&ptr->coord,&(wptr->coord));
	}
	
if (ptr->cybmine == (byte)255)
	{
	lta = shipclass[ptr->shpclass].lowest_to_attk-1;
	
	for (zothusn=0 ; zothusn < nterms ; zothusn++)
		{
		wptr=warshpoff(zothusn);
		/* if playing, and not cloaked, go getem */
		if (ingegame(zothusn) && wptr->cloak != 10)
			{
			if (lta <= wptr->shpclass && notclaimed(wptr,zothusn))
				{
				/* figure out who is closest */
				ddist = cdistance(&ptr->coord,&wptr->coord);
				if (ddist < low_dist)
					{
					low_dist = ddist;
					low_ship = zothusn;
					}
				}
			}
		}
	}

if (low_ship == -1 || low_ship >= nterms)
	{
	ptr->tick = 255; /* no one in game so cool it for awhile */
	ptr->cybmine = 255;
	}
else
	{
	ptr->cybmine = (byte)low_ship;
	wptr=warshpoff(low_ship);
	if (low_dist >= hyperdist1)
		{
		/* if far away invoke hyper-warp 20 X normal speed */
		ptr->speed2b = (double)low_dist*2000.0;
		ptr->speed = ptr->speed2b;
		ptr->head2b = vector(&ptr->coord,&(wptr->coord));
		ptr->where = 1;
		ptr->shieldstat = SHIELDDN;
		/* DEBUG
		logthis(spr("CybSpeed LONG Top=%d S2B=%ld Dist=%ld",
			ptr->topspeed,(long)ptr->speed2b,(long)low_dist));
			prf("***\r<%s>W gonna get you LONG<%d-%d> S2B=%s \r",cybname,(int)ptr->coord.xcoord,(int)ptr->coord.ycoord,spr("%ld",(long)ptr->speed2b));
		outwar(ALWAYS,usrn,0);*/
		}
	else
	if (low_dist >= hyperdist2)
		{
		/* BRAKE!!!!!! */
		if (ptr->speed > 20000.0)
			ptr->speed = 20000.0;
		ptr->speed2b = d_topspeed;
		ptr->head2b = vector(&ptr->coord,&(wptr->coord));
		logthis(spr("CybSpeed MID Top=%d S2B=%ld Dist=%ld",
			ptr->topspeed,(long)ptr->speed2b,(long)low_dist));
		/* DEBUG
		prf("***\r<%s>W gonna get you MID <%d-%d> S2B=%s \r",cybname,(int)ptr->coord.xcoord,(int)ptr->coord.ycoord,spr("%ld",(long)ptr->speed2b));
		outwar(ALWAYS,usrn,0);*/
		cyb_annoy(ptr,low_ship,60,1,4);
		}
	else
	if (low_dist > 3.0)
		{
		if (ptr->speed > d_topspeed)
			ptr->speed = d_topspeed;

		ptr->head2b = vector(&ptr->coord,&(wptr->coord));
		ptr->speed2b = d_topspeed;
		/* DEBUG
		prf("***\r<%s>I gonna get you SHORT <%d-%d> S2B=%s \r",cybname,(int)ptr->coord.xcoord,(int)ptr->coord.ycoord,spr("%ld",(long)ptr->speed2b));
		outwar(ALWAYS,usrn,0); */
		cyb_annoy(ptr,low_ship,30,5,8);
		if (ptr->where == 0)
			shieldup(ptr,usrn);
		}
	else
	if (low_dist <= 3.0)
		{
		if (ptr->speed > d_topspeed)
			ptr->speed = d_topspeed;

		ptr->head2b = vector(&ptr->coord,&(wptr->coord));
		if (wptr->where == 1)
			ptr->speed2b = ((wptr->speed2b > d_topspeed) ? d_topspeed:(wptr->speed2b*1.25));
		else
			ptr->speed2b = ((low_dist > .5) ? 990.0:rndm(500.0));

		/* DEBUG
		prf("***\r<%s>I gonna get you SHORT <%d-%d> S2B=%s \r",cybname,(int)ptr->coord.xcoord,(int)ptr->coord.ycoord,spr("%ld",(long)ptr->speed2b));
		outwar(ALWAYS,usrn,0); */
		cyb_annoy(ptr,low_ship,30,5,8);
		if (ptr->where == 0)
			shieldup(ptr,usrn);
		}
	}
}


void   FUNC cyb_won(ptr,usrn,wptr)
WARSHP	*ptr;				/* ptr to Cyber who won */
int		usrn;				/* usernum of cyber who won */
WARSHP   *wptr;			/* ptr to ship cyber killed */

{
usrn = usrn;
wptr = wptr;
ptr->cybmine = (byte)255;
ptr->speed2b = 2000.0;
ptr->cybupdate = 0;
}

void   FUNC cyb_died(ptr,usrn,wptr)
WARSHP	*ptr;				/* ptr to Cyber who died */
int		usrn;				/* usernum of cyber who died */
WARSHP   *wptr;			/* ptr to ship who killed cyber*/


{
usrn = usrn;
wptr = wptr;
ptr->status = GESTAT_AVAIL;
}

int	 FUNC isquad(ptr)
WARSHP	*ptr;				/* ptr to Cyber */
{
return(shipclass[ptr->shpclass].tough_factor == CYB_TOUGH_1);
}
