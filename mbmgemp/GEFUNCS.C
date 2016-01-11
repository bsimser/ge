
/***************************************************************************
 *                                                                         *
 *   GEFUNCS.C                                                             *
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
#include "stdio.h"

#else
#include "stdio.h"
#include "ctype.h"
#include "dos.h"
#include "usracc.h"
#include "btvstf.h"
#include "stdlib.h"
#include "portable.h"
#include "dosface.h"

#endif
#include "math.h"
#include "majorbbs.h"
#include "message.h"

#include "gemain.h"

#define GEFUNCS 1

#include        "geglobal.h"

/* LOCAL GLOBAL DEFS *****************************************************/



/**************************************************************************
** Look up the ships this player has                                     **
**************************************************************************/

void  FUNC lookupshp()

{
int     noships,shpno;

/* get the user record from GEuser.dat */

if (!(geudb(GELOOKUP,usaptr->userid, waruptr)))
	{
	/* Not found.... Better make up something */
	initusr(usaptr->userid); /* create his account */
	geudb(GEADD,tmpusr.userid,&tmpusr);
	memcpy(waruptr,&tmpusr,sizeof(WARUSR));  /* make it the current user */
	}
else
	{
	/* Got it! ... Dang are we lucky */
	geudb(GEGET,usaptr->userid, waruptr);
	}

/* go tell the poor sap what ships he still has */
noships = findships();

if (noships == 0)
	{
	initshp(usaptr->userid,0); /* give the dude a light freighter */
	gepdb(GEADD,tmpshp.userid,tmpshp.shipno,&tmpshp);
	memcpy(warsptr,&tmpshp,sizeof(WARSHP));  /* make is the current ship */
	prfmsg(FIRSTIME);
	outprfge(ALWAYS,usrnum);
	}
else
if (noships > 1)
	{
	prfmsg(FLEET3);
	usrptr->substt = CHOOSESH;
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (noships == 1)
	{
	shpno = scantab[usrnum].ship[0].shipno;

	setbtv(gebb1);

	if (gepdb(GEGET,usaptr->userid,shpno,warsptr))
		{
		tossingegame(); /* into the game you go bud! */
		return;
		}
	else
		{
		/* somehow lost the ship... make one anyway */
		geshocst(0,spr("GE:DBG:Ship Load Err %s",usaptr->userid));
		initshp(usaptr->userid,0); /* give the dude a light freighter */
		gepdb(GEADD,tmpshp.userid,tmpshp.shipno,&tmpshp);
		memcpy(warsptr,&tmpshp,sizeof(WARSHP));  /* make is the current ship */
		prfmsg(FIRSTIME);
		outprfge(ALWAYS,usrnum);
		}
	}
tossingegame();
}


/**************************************************************************
** Toss this yokel into the arena - God rest his soul                    **
**************************************************************************/

void  FUNC tossingegame()
{

if (warsptr->cloak != 10)
	{
	prfmsg(ANNOUN,shipclass[warsptr->shpclass].typename, warsptr->shipname);
	outwar(FILTER,usrnum,0);
	}

prfmsg(ENTSHP,waruptr->userid);
outprfge(ALWAYS,usrnum);

if (warsptr->cloak != 10)
	{
	prfmsg(ENTWAR, shipclass[warsptr->shpclass].typename, warsptr->shipname);
	outsect(FILTER,&warsptr->coord,usrnum,0);
	}

btupmt(usrnum,'>');
prfmsg(WELCOM,waruptr->userid);
outprfge(ALWAYS,usrnum);
usrptr->substt = FIGHTSUB;
warsptr->status = GESTAT_USER;
}

/**************************************************************************
** Initialize all the ship data                                          **
** NOTE: waruptr MUST be set to this channel first
**************************************************************************/
	
int  FUNC initshp(userid,type)

char    *userid;
int     type;
{
double  ddistance;
int     i,flag;

logthis(spr("GE:DBG:initship %d",type));
logthis(spr("%s",userid));
strncpy(tmpshp.userid,userid,UIDSIZ);
strncpy(tmpshp.shipname," <NO NAME> ",20);

tmpshp.coord.xcoord     = NEUTRAL_X + rndm(.9999);
tmpshp.coord.ycoord     = NEUTRAL_Y + rndm(.9999);
	
getsector(&tmpshp.coord);
flag = 1;

while (flag == 1)
	{
	tmpshp.coord.xcoord     = NEUTRAL_X + rndm(.9999);
	tmpshp.coord.ycoord     = NEUTRAL_Y + rndm(.9999);
	flag = 0;
	for (i=0;i<sector.numplan;++i)
		{
		if (sector.ptab[i].coord.xcoord != 0)
			{
			ddistance = cdistance(&tmpshp.coord,&sector.ptab[i].coord)*10000;
			if (ddistance < 1000)
				flag = 1;
			}
		}
	}
tmpshp.heading  = rndm(359.99);
tmpshp.head2b   = tmpshp.heading;

tmpshp.shpclass = type;
tmpshp.speed    = 0;
tmpshp.phasr    = 100;
tmpshp.speed    = 0;
tmpshp.speed2b  = 0;
tmpshp.damage   = 0.0;
tmpshp.lastfired = -1;
tmpshp.energy   = 50000L;
tmpshp.tactical = 0;
tmpshp.helm             = 0;
tmpshp.cloak    = 0;
tmpshp.shieldstat       = SHIELDDN;
tmpshp.shield   = 0;
tmpshp.shieldtype = 1;
tmpshp.phasrtype = 1;
tmpshp.train    = 0;
tmpshp.where    = 0;
tmpshp.hostile          = 0;
tmpshp.repair           = 0;
tmpshp.hypha                    = 0;
tmpshp.firecntl         = 0;
tmpshp.cantexit         = 0;
tmpshp.items[I_TORPEDO] = 0;
tmpshp.items[I_MISSILE] = 0;
tmpshp.items[I_FLUXPOD] = 3;
tmpshp.items[I_FOOD]            = 0;
tmpshp.items[I_DECOYS]  = 0;
tmpshp.items[I_FIGHTER] = 0;
tmpshp.items[I_MEN]      = 0;
tmpshp.items[I_IONCANNON]= 0;
tmpshp.items[I_TROOPS]  = 0;
tmpshp.items[I_ZIPPERS] = 0;
tmpshp.items[I_JAMMERS] = 0;
tmpshp.items[I_MINE]            = 0;
tmpshp.items[I_GOLD]   = 0;

tmpshp.destruct         = 0;
tmpshp.status                   = 0;
tmpshp.cybmine                  = 0;
tmpshp.cybskill         = 0;
tmpshp.cybupdate                = 0;
tmpshp.emulate                  = 0;


tmpshp.shipno = waruptr->topshipno+1;

++waruptr->topshipno;
++waruptr->noships;

for (i=0;i<MAXTORPS;++i)
	tmpshp.ltorps[i].distance = 0;

for (i=0;i<MAXMISSL;++i)
	tmpshp.lmissl[i].distance = 0;

for (i=0;i<MAXDECOY;++i)
	tmpshp.decout[i] = 0;

tmpshp.topspeed = shipclass[tmpshp.shpclass].max_warp;
logthis(spr("Created ship - topspeed = %d",tmpshp.topspeed));
return(0);
}

int  FUNC initusr(userid)
char    *userid;
{

strncpy(tmpusr.userid,userid,UIDSIZ); /* BJ CHANGED TO UIDSIZ */
tmpusr.kills    	= 0;
tmpusr.planets  	= 0;
tmpusr.debt     	= 0;
tmpusr.noships 	= 0;
tmpusr.score 		= 0;
tmpusr.plscore 	= 0;
tmpusr.klscore 	= 0;
tmpusr.rospos  	= 0;
tmpusr.teamcode	= 0;

tmpusr.cash 		= startcash;

tmpusr.topshipno 	= 0;


return(0);
}

/**************************************************************************
** find and list all the ships a single user has                         **
**************************************************************************/

int  FUNC findships()
{

int     found;
SCANTAB *sptr;
setbtv(gebb1);

found = 0;

if (qlobtv(0))
	{
	if (gepdb(GELOOKUPNAME,usaptr->userid,0,warsptr))
		{
		sptr = &scantab[usrnum];
		prf("\r#  XSect YSect Class                           Name:\r");
		do
			{
			gcrbtv(warsptr,0);
			if (sameas(usaptr->userid,warsptr->userid))
				{
				setsect(warsptr);
				prf("%-2d %-5d %-5d %-30s %s\r",found+1,
											 xsect,ysect,
				  shipclass[warsptr->shpclass].typename,
				  warsptr->shipname);
				sptr->ship[found].shipno = warsptr->shipno;
				++found;
				}
			else
				break;
			} while (qnxbtv());
		}
	}
/* added 06/17/89 to prevent cyborg code from thinking this user is a
	cybertron incase the next alpha record was indeed Cyborg-1. */

waruptr->noships = found;
warsptr->status = 0;

if (found == 1)
	clrprf();
else
	outprfge(ALWAYS,usrnum);
return (found);
}

/**************************************************************************
** Select the ship to board from the list                                **
**************************************************************************/

void  FUNC selectship()
{
int selection;
int     shpno;

selection = (atoi(margv[0]))-1;
if (selection >= 0)
	{
	shpno = scantab[usrnum].ship[selection].shipno;

	setbtv(gebb1);

	if (gepdb(GEGET,usaptr->userid,shpno,warsptr))
		{
		tossingegame(); /* into the game you go bud! */
		return;
		}
	}

prfmsg(FLEET4);
findships();
prfmsg(FLEET3);
usrptr->substt = CHOOSESH;
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Repair the ship                                                       **
**************************************************************************/
	
void  FUNC repairship(ptr,usrn)

WARSHP *ptr;
int           usrn;
{
if (ptr->repair > 0)
	{
	if (ptr->cantexit > 0)
		{
		prfmsg(MAINT10);
		ptr->repair = 0;
		outprfge(ALWAYS,usrn);
		return;
		}

	if (ptr->damage > 3.0)
		ptr->damage -= 3.0;
	else
		ptr->damage = 0.0;

	ptr->repair = (int)(ptr->damage/3.0);
	if (ptr->repair <= 1)
		{
		ptr->repair = 0;
		ptr->damage = 0.0;
		ptr->phasr = 100;
		ptr->tactical = 0;
		ptr->helm = 0;
		ptr->firecntl = 0;
		ptr->shieldstat = SHIELDDN;
		ptr->shield = 0;
		ptr->topspeed = shipclass[ptr->shpclass].max_warp;
		prfmsg(MAINT7);
		outprfge(ALWAYS,usrn);
		}
	}
}


/**************************************************************************
** Rotate the ship                                                       **
**************************************************************************/

void  FUNC rotateship(ptr,usrn)

WARSHP *ptr;
int           usrn;
{
int  angle;
double  rotamt;

rotamt = (double)(shipclass[ptr->shpclass].max_accel/10.0);

if (ptr->heading != ptr->head2b)
	{
	if (absol(normal(ptr->heading - ptr->head2b)) >= (360.0-rotamt) ||
		absol(normal(ptr->heading - ptr->head2b)) <= rotamt)
		{
		ptr->heading = ptr->head2b;
		angle = (int)ptr->heading;
		prfmsg(NOWTHER,angle);
		outprfge(FILTER,usrn);
		}
	else
		{
		angle = (int)normal(ptr->heading - ptr->head2b);
		if (angle < 180)           /* rotate left */
			ptr->heading = normal(ptr->heading - rotamt);
		else                      /* rotate right */
			ptr->heading = normal(ptr->heading + rotamt);
		}
	}
}


/**************************************************************************
** Accelerate the ship                                                   **
**************************************************************************/

void  FUNC accel(ptr,usrn)

WARSHP *ptr;
int           usrn;
{
int i,flag,usage;
double absol();
double  accelrate,decelrate;


if (ptr->speed < ptr->speed2b)
	{
	accelrate = (double)shipclass[ptr->shpclass].max_accel;
	if ((ptr->speed2b >= 1000) && (ptr->speed/1000 < 1) && ((ptr->speed+accelrate)/1000 >=1))
		hyperspace(ptr,usrn,1);
	if (absol(ptr->speed - ptr->speed2b) <= accelrate)
		{
		ptr->speed = ptr->speed2b;
		
		prfmsg(SPEEDIS, showarp(ptr->speed));
		outprfge(FILTER,usrn);
		}
	else
		{
		if (ptr->speed < 1000)
			usage = 0;
		else
			usage = ACCENGAMT;
		if (useenergy(ptr,usrn,usage) == 1)
			{
			if ((int)(ptr->speed/1000) != (int)((ptr->speed + accelrate)/1000))
				{
				prfmsg(WARP,(int)((ptr->speed + accelrate)/1000));
				outprfge(FILTER,usrn);

				/* if we passed warp 4-8 kill any missiles */

				if ((ptr->speed + accelrate)/1000 >= (4 + gernd()%4))
					{
					flag = 0;
					for (i=0;i<MAXMISSL;++i)
						{
						if (ptr->lmissl[i].distance > 0)
							{
							ptr->lmissl[i].distance = 0;
							flag = 1;
							}
						}
					if (flag == 1)
						{
						prfmsg(MISSL2);
						outprfge(FILTER,usrn);
						}
					}
				}
			ptr->speed += accelrate;
			}
		else
			{
			prfmsg(NOACCEL,(int)ptr->speed);
			outprfge(ALWAYS,usrn);
			ptr->speed2b = 0;
			}
		}
	}
else
if (ptr->speed > ptr->speed2b)
	{
	decelrate = (double)shipclass[ptr->shpclass].max_accel * 2.0;
	if ((ptr->speed2b < 1000) && (ptr->speed/1000 >=1) && ((ptr->speed-decelrate)/1000 <1))
		hyperspace(ptr,usrn,0);

	if (absol(ptr->speed - ptr->speed2b) <= decelrate)
		{
		ptr->speed = ptr->speed2b;
		if (ptr->speed > 0)
			{
			prfmsg(SPEEDIS, showarp(ptr->speed));
			outprfge(FILTER,usrn);
			}
		else
			{
			prfmsg(SPEED0);
			outprfge(FILTER,usrn);
			}
		}
	else
		{
		if ((int)(ptr->speed/1000) != (int)((ptr->speed - decelrate)/1000))
			{
			if (ptr->speed > 0 )
				{
				prfmsg(WARP,(int)((ptr->speed-decelrate)/1000)+1);
				outprfge(FILTER,usrn);
				}
			else
				{
				prfmsg(DEADSTOP);
				outprfge(FILTER,usrn);
				}
			}
		ptr->speed -= decelrate;
		}
	}
}


/**************************************************************************
** Make the jump to or from hyperspace                                   **
**************************************************************************/

void  FUNC hyperspace(ptr,usrn,flag)

WARSHP *ptr;
int           usrn;
{

int i;

if (flag == 1)
	{
	if (ptr->shieldstat == SHIELDUP)
		{
		prfmsg(HYSHDN);
		ptr->shieldstat = SHIELDDN;
		}
	if (ptr->cloak > 0)
		{
		prfmsg(HYCLDN);
		ptr->cloak = 0;
		}
	prfmsg(HYPERIN);
	outprfge(FILTER,usrn);
	
	ptr->where = 1;

	prfmsg(HYPERIN2,ptr->shipname);
	outsect(FILTER,&(warshpoff(usrn)->coord),usrn,0);

	for(i=0;i<MAXTORPS;++i)
		ptr->ltorps[i].distance = 0;

	for(i=0;i<MAXDECOY;++i)
		ptr->decout[i] = 0;
	}
else
	{
	prfmsg(HYPEROUT);
	outprfge(FILTER,usrn);
	
	ptr->where = 0;

	prfmsg(HYPEROU2,ptr->shipname);
	outsect(FILTER,&(warshpoff(usrn)->coord),usrn,0);

	}

}

/**************************************************************************
** Move the ship                                                         **
**************************************************************************/

void  FUNC moveship(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

COORD oldsect,newsect;
int	diff,intspeed;

if (ptr->speed > 0)
	{

	movecoord(&oldsect, &ptr->coord);
/*   sprintf(gechrbuf,"usrn = %d, x=%g, y=%g",usrn,ptr->coord.xcoord,ptr->coord.ycoord);
	prf("%s",gechrbuf);
	outprf(usrn);            */
	ptr->coord.xcoord = ptr->coord.xcoord + ((ptr->speed * sin(degtorad(ptr->heading)))/65000.0);
	ptr->coord.ycoord = ptr->coord.ycoord - ((ptr->speed * cos(degtorad(ptr->heading)))/65000.0);

	if (ptr->where <= 1)
		{
		if (ptr->coord.xcoord > univmax)
			{
			if (univwrap)
				{
				ptr->coord.xcoord       -= (double)(univmax*2);
				}
			else
				{
				ptr->coord.xcoord       = (double)(univmax-2);
				telezip(ptr,usrn);
				}
			}
		else
		if (ptr->coord.xcoord < (univmax*-1))
			{
			if (univwrap)
				{
				ptr->coord.xcoord       += (double)(univmax*2);
				}
			else
				{
				ptr->coord.xcoord       = (double)((univmax-2)*-1);
				telezip(ptr,usrn);
				}
			}


		if (ptr->coord.ycoord > univmax)
			{
			if (univwrap)
				{
				ptr->coord.ycoord       -= (double)(univmax*2);
				}
			else
				{
				ptr->coord.ycoord       = (double)(univmax-2);
				telezip(ptr,usrn);
				}
			}
		else
		if (ptr->coord.ycoord < (univmax*-1))
			{
			if (univwrap)
				{
				ptr->coord.ycoord       += (double)(univmax*2);
				}
			else
				{
				ptr->coord.ycoord       = (double)((univmax-2)*-1);
				telezip(ptr,usrn);
				}
			}
		}

	movecoord(&newsect, &ptr->coord);

	if (!samesect(&oldsect, &newsect))
		{
		prfmsg(MOVE1,coord1(oldsect.xcoord),coord1(oldsect.ycoord),
			coord1(newsect.xcoord),coord1(newsect.ycoord));
		outprfge(FILTER,usrn);
		if (ptr->speed < 21000.0)
			{
			prfmsg(MOVE2,ptr->shipname);
			outsect(FILTER,&oldsect,usrn,0);
			}
		if (ptr->speed < 21000.0)
			{
			prfmsg(MOVE3,ptr->shipname);
			outsect(FILTER,&newsect,usrn,0);
			}
		ptr->hostile = 0;
		if (ptr->destruct > 0 && neutral(&newsect))
			{
			prfmsg(SELFD4);
			outprfge(ALWAYS,usrn);
			ptr->destruct = 0;
			}
		}

	if (ptr->speed > 1000.0 && ptr->status == GESTAT_USER)
		{
		intspeed = ptr->speed/1000.0;

		/* if this ship is exceeding the safety zone and not decelerating */
		if (intspeed > ptr->topspeed && ptr->speed <= ptr->speed2b)
			{

			/* the following function yealds a factor from 10 to 60 which
            is lower the faster the player is going */
			diff = intspeed - ptr->topspeed;
			diff = (diff*100)/intspeed;
			diff = 60 - diff;
			if (diff < 0) 
				diff = 5;

			if (gernd()%diff==0)
				{
				if (ptr->warncntr > 4)
					{
					/* blow up the engines */
					prfmsg(WARPBRK);
					outprfge(ALWAYS,usrn);
					ptr->topspeed = 0;
					ptr->speed2b = 0;
					ptr->damage += gernd()%20;
					}
				else
					{
					prfmsg(WARPFAST+ptr->warncntr);
					outprfge(FILTER,usrn);
					ptr->warncntr++;
					}
				}			

			}
		else
			{
			if (ptr->warncntr > 0)
				{
				/* speed is normal but he was going to fast before... limit the
            engine maximum speed */

				ptr->topspeed = ptr->topspeed/ptr->warncntr;
				ptr->speed2b = ptr->topspeed*1000.0;
				prfmsg(WARPSPD,ptr->topspeed);
				outprfge(FILTER,usrn);
				/* reset the warning counter */
				ptr->warncntr = 0;
				}
			}
		useenergy(ptr,usrn,MOVENGUSE);

		if (ptr->energy < MOVENGMIN)
			{
			ptr->speed2b = 0;
			prfmsg(MOVE4);
			outprfge(FILTER,usrn);
			}
		}
	/* Cybertrons ignore gravity */
	if (ptr->where == 0 && ptr->status == GESTAT_USER)
		gravity(ptr,usrn);

	if (ptr->hostile > 0)
		checkdist(ptr,usrn);
	}
else
	{
	if (ptr->where == 1)  /* fix stuck users */
		ptr->where = 0;
	}



/* if there is a beacon message and same sector display it */
if (samesect(&beacon[usrn].coord,&ptr->coord))
	{
	if (beacon[usrn].beacon[0] != 0 && gernd()%10 == 0)
		{
		prfmsg(BEAC01,beacon[usrn].plnum,beacon[usrn].beacon);
		outprfge(FILTER,usrn);
		}
	}
}

void  FUNC telezip(ptr,usrn)

WARSHP *ptr;
int           usrn;
{
/*
ptr->coord.xcoord       = (rndm((double)(univmax-2)))+1;
ptr->coord.ycoord       = (rndm((double)(univmax-2)))+1;
*/
ptr->speed = 0.0;
ptr->speed2b = 0.0;
ptr->damage += TELEDAM;
prfmsg(TELEPORT);
outprfge(ALWAYS,usrn);
}


void  FUNC gravity(ptr,usrn)

WARSHP *ptr;
int           usrn;
{
int i;
double    cdistance(),cbearing();
unsigned dist;

refresh(ptr, usrn);

/* check distances to plantets */

for (i=0; i<MAXPLANETS;++i)
	{
	if (ptab[usrn].planets[i].type != 0)
		{
		dist = (unsigned)(cdistance(&ptr->coord,&ptab[usrn].planets[i].coord)*10000);
	/*      prf("dist to planet %u is %d\r",i,dist);
		outprfge(ALWAYS,usrn);*/
		if (dist < 250)
			{
			if (dist >= 50)
				{
				if (ptab[usrn].planets[i].type == PLTYPE_PLNT)
					prfmsg(GRAVITY1,i+1);
				else
					prfmsg(GRAVWRM1,i+1);

				outprfge(ALWAYS,usrn);
				}
			else
			if (dist >= 25)
				{
				if (ptab[usrn].planets[i].type == PLTYPE_PLNT)
					prfmsg(GRAVITY2,i+1);
				else
					prfmsg(GRAVWRM2,i+1);

				outprfge(ALWAYS,usrn);
				}
			else
			if (dist < 25 )
				{
				if (ptab[usrn].planets[i].type == PLTYPE_PLNT)
					prfmsg(GRAVITY3,i+1);
				else
					prfmsg(GRAVWRM3,i+1);

				outprfge(ALWAYS,usrn);
				if (ptab[usrn].planets[i].type == PLTYPE_PLNT)
					ptr->damage = 101.0;
				else
					{
					setsect(ptr); /* build PKEY */
					pkey.plnum = i+1;
					gesdb(GEGETNOW,&pkey,(GALSECT *)&worm);
					ptr->coord.xcoord = worm.destination.xcoord;
					ptr->coord.ycoord = worm.destination.ycoord;
					ptr->damage+= 5.5;
					cleartm(usrn);	/* clear the tors and missiles */
					}
				}
			}
		}
	}
}

/* If player is far from the planet they were attacking then they are not
	being hostile */

void  FUNC checkdist(ptr,usrn)

WARSHP *ptr;
int           usrn;
{
int i;
double    cdistance(),cbearing();
unsigned dist;

refresh(ptr, usrn);

i = ptr->hostile - 10;
if (ptab[usrn].planets[i].type != 0)
	{
	dist = (unsigned)(cdistance(&ptr->coord,&ptab[usrn].planets[i].coord)*10000);
/*      prf("dist to planet %u is %d\r",i,dist);
	outprfge(ALWAYS,usrn);*/
	if (dist > 1000)
		{
		ptr->hostile = 0;
		}
	}
}


void  FUNC refresh(ptr, usrn)

WARSHP *ptr;
int           usrn;
{
int i;

COORD   ss,tmpcoord;

movecoord(&ss,&ptr->coord);

/* need to refresh planet coords? */

tmpcoord.xcoord = 32767.000;
tmpcoord.ycoord = 32767.000;

for (i = 0; i < MAXPLANETS; ++i)
	{
	if (ptab[usrn].planets[i].type != 0)
		{
		movecoord(&tmpcoord,&ptab[usrn].planets[i].coord);
		break;
		}
	}

if (!samesect(&tmpcoord, &ss))
	{
	logthis(spr("GEFUNCS:refreshing sector for usrn %d",usrn));
	getsector(&ss);

	movecoord(&ptab[usrn].planets[i].coord, &ss);
	memcpy(&ptab[usrn],&sector.ptab,sizeof(PLANETAB));
	}
}

/**************************************************************************
** Check the damage and repair any - Also service weapons                **
**************************************************************************/

void  FUNC checkdam(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

double  preload;
logthis(spr("GE:Chn %d checkdam %s",usrn,ptr->userid)); 

if (ptr->damage >= 100.0)
	{
	ptr->damage = 0.0;   /* reset damage so he can get back on */
	prfmsg(YOURDEAD);
	outprfge(ALWAYS,usrn);

/* DEBUG
	prf ("lastfired = %u\r",ptr->lastfired);
	outprfge(ALWAYS,usrn); */

	killem(ptr,usrn);

	/* only reset btupmt on "real" users */
	if (usrn < nterms)
		btupmt(usrn,'\0');

	if (ptr->status == GESTAT_AUTO || ptr->userid[0] == '@')
		ptr->status = GESTAT_AVAIL;

	if (ptr->status == GESTAT_USER || ptr->emulate == 1)
		{
/*		user[usrn].state = 0;*/
		user[usrn].substt = 0;
		--numwar;
		ptr->where = -1;
		}
	return;
	}

if (ptr->damage > 0.0)
	ptr->damage = ptr->damage - repairrate;
else
	ptr->damage = 0.0;
	

/* repair & recharge phasers */
if (ptr->phasr < 0)
	{
	ptr->phasr += 1;
	if (ptr->phasr == 0)
		{
		prfmsg(PHREPR);
		outprfge(ALWAYS,usrn);
		}
	}
else
if (ptr->phasr < 100)
	{
	if(useenergy(ptr,usrn,PENGUSE) == 1)
		{
		/* If phasers get to minimum fire power tell captain */
		preload = (double)(ptr->phasrtype * PRELOAD);
		/* if inteceptor class double phaser recharge speed 
		if (ptr->shpclass == 2)
			preload *=2; */
		if ((ptr->phasr < PMINFIRE) && (ptr->phasr + preload >= PMINFIRE))
			{
			prfmsg(PHSRUP);
			outprfge(ALWAYS,usrn);
			}
		
		ptr->phasr = ptr->phasr + preload;

		/* if phasers get to 100% tell captain, and set to 100% */
		if (ptr->phasr >= 100)
			{
			prfmsg(PHSRMAX);
			outprfge(ALWAYS,usrn);
			ptr->phasr = 100;
			}
		}
	}

/* repair tactical display */
if (ptr->tactical < 0)
	{
	++ptr->tactical;
	if (ptr->tactical == 0)
		{
		prfmsg(TAREPR);
		outprfge(ALWAYS,usrn);
		}
	}

/* repair helm */
if (ptr->helm < 0)
	{
	++ptr->helm;
	if (ptr->helm == 0)
		{
		prfmsg(HLREPR);
		outprfge(ALWAYS,usrn);
		}
	}
/* repair fire control systems */
if (ptr->firecntl > 0)
	{
	--ptr->firecntl;
	if (ptr->firecntl == 0)
		{
		prfmsg(FCREPR);
		outprfge(ALWAYS,usrn);
		}
	}
return;
}

void  FUNC killem(ptr,usrn)
WARSHP *ptr;
int           usrn;
{
WARSHP	*wptr;
WARUSR	*wuptr;

unsigned i;
int     who;
long scr,amt,bonus,ded_amt;

/* 12/19/91 fix to prevent a player from being awarded points for killing */
/* himself */

waruptr=warusroff(usrn);

who = ptr->lastfired;

if (who >= 0 && who < nships && who != usrn)
	{
	wptr= warshpoff(who);
	wuptr= warusroff(who);
	logthis(spr("Killed by %s",wptr->userid));
	if (wptr->status == GESTAT_AUTO)
		{
		if (shipclass[wptr->shpclass].won_func != NULL)
			shipclass[wptr->shpclass].won_func(wptr,who,ptr);
		}

	prfmsg(KILLEDBY,username(ptr),username(wptr));
	outwar(FILTER,usrn,0);
	++(wuptr->kills);

	prfmsg(KILLGOT1,ptr->shipname);
	
	/* no men or troops can be collected */
	for (i=1;i<NUMITEMS;++i)
		{
		if (i != I_MEN && i != I_TROOPS)
			{
			amt = ptr->items[i] / (gernd()%5 +1);
			/* only collect as much as we can hold */
			if (amt > 0  && chkweight(wptr,i,amt))
				{
				wptr->items[i] += amt;
				sprintf(gechrbuf2,"%ld",amt);
				prf(", %s %s",gechrbuf2,item_name[i]);
				}
			}
		}
/*	k = ptr->cash / (long)((gernd()%15) +5);
	waruptr->cash -= k;
	wuptr->cash += k;
	sprintf(gechrbuf,"%ld",k);*/
	prf(".\r");

	/* grant points for the kill */

	scr = (long)shipclass[ptr->shpclass].max_points;
	bonus = 0;

/* 12/19/91 NEW SCORING METHOD */

	if (waruptr->rospos > 0)
		{
		bonus = (long)(score_bonus/(waruptr->rospos));
		}

	amt = scr + bonus;

	ded_amt = (amt/100L)*score_f2;

	/* if no one shot me or a cyb shot me - don't deduct the score */

	if (who< 0 || who >= nterms)
		ded_amt = ded_amt/10;


	if (waruptr->score < ded_amt)
		{
		waruptr->score = 0;
		waruptr->klscore = 0;
		}
	else
		{
		(waruptr->score) -= ded_amt;
		}

	if (waruptr->klscore < ded_amt)
		{
		waruptr->klscore = 0;
		}
	else
		{
		waruptr->klscore -= ded_amt;
		}

	(wuptr->score) += amt;
	(wuptr->klscore) += amt;

	sprintf(gechrbuf,"%ld",scr);
	prfmsg(KILLPNTS,gechrbuf,shipclass[ptr->shpclass].typename);

	if (bonus > 0)
		{
		sprintf(gechrbuf,"%ld",bonus);
		prfmsg(KILLBON,gechrbuf);
		}

	outprfge(ALWAYS,who);

	if (chgloser > 0 
		&& ptr->status == GESTAT_USER 
		&& wptr->status == GESTAT_USER)
		{
		amt = (waruptr->cash/100L)*(long)chgloser;
		if (amt > waruptr->cash)
			amt = waruptr->cash;


		if (amt > 0)
			{
			waruptr->cash -= amt;
			wuptr->cash += amt;
			sprintf(gechrbuf,"%ld",amt);
			prfmsg(CHGLSR1,gechrbuf);
			outprfge(ALWAYS,usrn);
			prfmsg(CHGLSR2,gechrbuf,ptr->userid);
			outprfge(ALWAYS,who);
			}
		
		}

	/* if the clown just killed was the last to fire on me clean out
		my last fired flag so as not to award him with any points should
		I end up getting killed */

	if (wptr->lastfired == usrn)
		wptr->lastfired = -1;
	
#ifdef SHOWDOC
	if (gernd()%RNDDOC == 0)
		{
		setbtv(gebb2);

		if (qlobtv(0))
			{
			if (qeqbtv(ptr->userid,1))
				{
				prfmsg(CAPTDOC);
				i = 0;
				do
					{
					gcrbtv(&planet,1);
					if (sameas(planet.userid,ptr->userid))
						{
						prf("%-20s %d %d   %d \r",planet.name,planet.xsect,planet.ysect,planet.plnum);
						outprfge(ALWAYS,who);
						}
					else
						break;
					} while (qnxbtv() && (++i < 20));
				}
			}
		}
#endif
	if (shipclass[wptr->shpclass].max_type != CLASSTYPE_DROID)
		{
		geudb(GEUPDATE,wuptr->userid,wuptr);
		}

	if (shipclass[ptr->shpclass].kill_func != NULL)
		shipclass[ptr->shpclass].kill_func(ptr,usrn,wptr);
	}
else
	{
	prfmsg(DIED,ptr->shipname,username(ptr));
	outwar(ALWAYS,usrn,0);
	if (shipclass[ptr->shpclass].kill_func != NULL)
		shipclass[ptr->shpclass].kill_func(ptr,usrn,NULL);
	}


cleartm(usrn);

--(waruptr->noships);
/* fix any wrap problem */
if (waruptr->noships == 65535U)
	waruptr->noships = 0;

if (shipclass[ptr->shpclass].max_type != CLASSTYPE_DROID)
	{
	gepdb(GEDELETE,ptr->userid,ptr->shipno,ptr);
	geudb(GEUPDATE,waruptr->userid,waruptr);
	}

logthis(spr("GE:INF:%s died!",waruptr->userid));
}

/**************************************************************************
** Recharge energy pool                                                  **
**************************************************************************/

void  FUNC recharge(ptr,usrn)

WARSHP *ptr;
int           usrn;
{
	usrn = usrn; /* avoid the warning */
	if (ptr->energy < ENGYMAX)
		ptr->energy = ptr->energy + ENGRECHG;
	else
		ptr->energy = ENGYMAX;
}


/**************************************************************************
** Check  flux status                                                  **
**************************************************************************/

void  FUNC fluxstat(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

if (ptr->energy < ENGYMIN)
	{
	if (ptr->items[I_FLUXPOD] > 0)
		{
		ptr->energy = ENGYMAX;
		--ptr->items[I_FLUXPOD];
		prfmsg(FLUXLOAD);
		if (ptr->items[I_FLUXPOD] == 0)
			{
			prfmsg(LASTFLUX);
			}
		outprfge(ALWAYS,usrn);
		}
	}
}


/**************************************************************************
** Check  shield status                                                  **
**************************************************************************/

void  FUNC shieldstat(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

if (ptr->shieldstat == SHIELDUP)
	{
	if (ptr->energy < SHMINPWR)
		{
		ptr->shieldstat = SHIELDDN;
		ptr->shield = 0;
		prfmsg(SHDNNOP);
		outprfge(ALWAYS,usrn);
		}
	 else
		{
		shieldchg(ptr,usrn);
		}
	}
else
if (ptr->shieldstat == SHIELDDM)
	{
	shieldrep(ptr,usrn);
	}
}


/**************************************************************************
** Check  cloak  status                                                  **
**************************************************************************/

void  FUNC cloakstat(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

if (ptr->cloak > 0)
	{
	if (ptr->energy < clenguse)
		{
		ptr->cloak = 0;
		prfmsg(CLOKNOP);
		outprfge(ALWAYS,usrn);
		prfmsg(CLOK2);
		outrange(FILTER,&ptr->coord);
		}
	 else
		{
		ptr->energy = ptr->energy - clenguse;
		}
	 }
else
if (ptr->cloak < 0)
	{
	++ptr->cloak;
	if (ptr->cloak == 0)
		{
		prfmsg(CLREPR);
		outprfge(ALWAYS,usrn);
		}
	}
}

/**************************************************************************
** Check  mine  status                                                  **
**************************************************************************/

void  FUNC checkmines()
{
int i;
int zothusn;            /* general purpose other-user channel number */
WARSHP  *wptr;
double  ddist, cdistance(), cbearing(), damfact;
unsigned udist;
MINE            *mptr;
setmbk(gemb);

/*DEBUG
printf("checking mines\r\n");*/

for (i=0,mptr = mines; i<nummines;++mptr,++i)
	{
	if (mptr->channel != 255)  /* if a live mine */
		{
		--mptr->timer;
		if (mptr->timer%5 == 0)
			{
			for (zothusn=0 ; zothusn < nships ; zothusn++)
				{
				wptr=warshpoff(zothusn);
				if (ingegame(zothusn))
					{
					ddist = cdistance(&mptr->coord,&wptr->coord);
					ddist *= 10000;
					bearing = (int)(cbearing(&wptr->coord,&mptr->coord,wptr->heading)+.5);
					setsect(wptr);
					if (ddist < ((double)MINERANGE) && (xsect != 0 || ysect != 0))
						{
						udist = (unsigned)ddist;
						if (mptr->timer == 0)
							{
							ddist = 1.0-(ddist/((double)MINERANGE));
							if (ddist < 0)
								ddist = 0;
							ddist = ddist*ddist*ddist;
							if (wptr->shieldstat == SHIELDUP)
								{
								damfact = (double)(ddist*minedammax);
								damfact = ton_fact(wptr,damfact); /* adjust for weight */
								damage = damfact;

								damage = damage/(gernd()%5+wptr->shieldtype);
								damstr(damage);
								prfmsg(MINE4,bearing,udist,gechrbuf);
								outprfge(FILTER,zothusn);
								shieldhit(wptr,zothusn,damage+20);
								}
							else
								{
								damfact = (double)(ddist*minedammax);
								damfact = ton_fact(wptr,damfact); /* adjust for weight */
								damage = damfact;

								damstr(damage);
								prfmsg(MINE4,bearing,udist,gechrbuf);
								outprfge(FILTER,zothusn);
								}
							wptr->damage += (double)damage;
							wptr->lastfired = (int)mptr->channel;
							wptr->minesnear = FALSE;
							/*DEBUG
							prf("MINE: chn # %d gets credit\r",wptr->lastfired);
							outprfge(ALWAYS,zothusn);*/
							}
						else
							{
							if (wptr->jammer == 0)
								{
								prfmsg(MINE6,bearing,udist);
								outprfge(FILTER,zothusn);
								if (wptr->status == GESTAT_AUTO)
									wptr->minesnear = TRUE;
								}
							}
						}
					else
					if (mptr->timer == 0 && wptr->jammer == 0)
						{
						prfmsg(MINE5,bearing);
						outprfge(FILTER,zothusn);
						}
					}
				}
			}
		}
	if (mptr->timer == 0)
		mptr->channel = 255; /* stomp on it - HARD */
	}
}

/**************************************************************************
** Use up energy and check functions                                     **
**************************************************************************/

int  FUNC useenergy(ptr,usrn,amount)

WARSHP *ptr;
int           usrn, amount;

{
usrn = usrn; /* avoid the warning */
if (ptr->energy >= amount+500) /* fudge a bit */
	{
	ptr->energy -= amount;
	return(1);
	}
else
	return(0);
}



/**************************************************************************
** Check  for incoming torpedoes or missiles & track decoys               **
**************************************************************************/

void  FUNC checktm(ptr,usrn)

WARSHP *ptr;
int           usrn;
{
int i,j,flag,power;

MISSILE 			*mptr;
TORPEDO 			*tptr;
unsigned			*dptr;
char 				tmpbuf[20];
double			damfact;

/* flag hyper-phasers ready again */
if (ptr->hypha > 0)
	--(ptr->hypha);

/* knock down battle lock ticks */
if (ptr->cantexit > 0)
	--(ptr->cantexit);

/* torpedoes first */

flag = 0;
for (i=0,tptr=ptr->ltorps;i<MAXTORPS;++i,++tptr)
	{
	if (tptr->distance > 1)
		{
		if (tptr->distance <= torpsped)
			{
			tptr->distance = 0;
			if (ptr->shieldstat == SHIELDUP)
				{
				damfact = tdammax * rndm(.5);
				damfact = ton_fact(ptr,damfact); /* adjust for weight */

				ptr->damage += damfact;
				ptr->lastfired = tptr->channel;
				prfmsg(THIT1);
				outprfge(ALWAYS,usrn);
				acctm(ptr,usrn,0,tptr->channel);
				shieldhit(ptr,usrn,(gernd()%20)+10);
				}
			else
				{
				prfmsg(THIT2);
				outprfge(ALWAYS,usrn);
				damfact = rndm(.5)+.5;
				damfact = tdammax * damfact;
				
				damfact = ton_fact(ptr,damfact); /* adjust for weight */

				ptr->damage += damfact;
				acctm(ptr,usrn,0,tptr->channel);
				}
			randamage(ptr,usrn); /*assess any random damage */
			}
		else  /* still flying */
			{
			for (j=0,dptr=ptr->decout; j<MAXDECOY; ++j)
				{
				if (dptr[j] > 0)
					{
					if (tptr->distance < 5000 && (gernd()%decodds == 0))
						{
						prfmsg(TORDEST);
						outprfge(FILTER,usrn);
						dptr[j] = 0;
						tptr->distance = 0;
						break;
						}
					}
				}
			if (tptr->distance > 0)  /* torp still here? */
				{
				tptr->distance -= torpsped;
				if (flag == 0)
					{
					prfmsg(TORP1);
					outprfge(FILTER,usrn);
					flag = 1;
					}
				}
			}
		}
	}

/* missiles second */
flag = 0;
for (i=0,mptr=ptr->lmissl;i<MAXMISSL;++i,++mptr)
	{
	if (mptr->distance > 0)
		{
		if (mptr->distance < mislsped)
			{
			mptr->distance = 0;
			/* reduce the energy by the weight of this ship */
			damfact = mptr->energy;
			damfact = ton_fact(ptr,damfact); /* adjust for weight */
			mptr->energy = damfact;

			if (mptr->energy < 1000)
				strcpy(tmpbuf,"very small");
			else
			if (mptr->energy < 5000)
				strcpy(tmpbuf,"light");
			else
			if (mptr->energy < 20000U)
				strcpy(tmpbuf,"moderate");
			else
			if (mptr->energy < 40000U)
				strcpy(tmpbuf,"strong");
			else
				strcpy(tmpbuf,"devastating");


			if (ptr->shieldstat == SHIELDUP)
				{
/*				damfact = (double)mptr->energy;*/
				damfact = damfact/50000.0;
				damfact = damfact * rndm(.1);
				ptr->damage += mdammax*damfact;
				prfmsg(MHIT1,tmpbuf);
				outprfge(ALWAYS,usrn);
				acctm(ptr,usrn,1,mptr->channel);
				
				power = mptr->energy/999;
				power = power * (rndm(.5)+.5);
				shieldhit(ptr,usrn,power);
				}
			else
				{
				prfmsg(MHIT2,tmpbuf);
				outprfge(ALWAYS,usrn);
/*				damfact = (double)mptr->energy;*/
				damfact = damfact/50000.0;
				damfact = damfact * (rndm(.5)+.5);
				ptr->damage += mdammax*damfact;
				acctm(ptr,usrn,1,mptr->channel);
				}
			randamage(ptr,usrn); /*assess any random damage */
			}
		else
			{
			for (j=0,dptr=ptr->decout; j<MAXDECOY; ++j)
				{
				if (dptr[j] > 0)
					{
					if (mptr->distance < 3000 && (gernd()%decodds == 0))
						{
						prfmsg(MISDEST);
						outprfge(FILTER,usrn);
						dptr[j] = 0;
						mptr->distance = 0;
						break;
						}
					}
				}
			if (mptr->distance > 1)  /* missl still here? */
				{
				mptr->distance -= mislsped;
				if (flag == 0)
					{
					prfmsg(MISSL1);
					outprfge(FILTER,usrn);
					flag = 1;
					}
				}
			}
		}
	}
/* finally decoys */

for (i=0,dptr=ptr->decout;i<MAXDECOY;++i)
	{
	if (dptr[i] > 0)
		{
		if (--dptr[i] == 0)
			{
			prfmsg(DECGONE);
			outprfge(FILTER,usrn);
			}
		}
	}
/* and Jammers too */
if (ptr->jammer > 0)
	{
	--ptr->jammer;
	if (ptr->jammer == 0)
		{
		prfmsg(JAMMER5);
		outprfge(FILTER,usrn);
		}
	}
/* and cloak too */
if (ptr->cloak == 1)
	{
	ptr->cloak = 2;
	}
else
if (ptr->cloak == 2)
	{
	ptr->cloak = 10;
	prfmsg(CLOKUP);
	outprfge(ALWAYS,usrn);
	}
}

void  FUNC acctm(ptr,usrn,mt,channel)
WARSHP *ptr;
int     usrn;
int     mt;
unsigned char channel;

{

if (channel != 255)
	{
	prfmsg(MTACC1+mt,shpltr(channel,usrn),ptr->shipname);
	outprfge(ALWAYS,channel);
	ptr->lastfired = channel;
	}
else
	{
	ptr->lastfired = -1;
	}
}

void  FUNC cleartm(channel)
int channel;
{
WARSHP	*wptr;
int     j;
int zothusn;

for (zothusn=0; zothusn < nships ; zothusn++)
	{
	if (zothusn != usrnum && ingegame(zothusn))
		{
		wptr=warshpoff(zothusn);
		for (j=0;j<MAXTORPS;++j)
			{
			if (wptr->ltorps[j].channel == (unsigned char)channel)
				{
				wptr->ltorps[j].channel = 255;
				}
			}
		for (j=0;j<MAXMISSL;++j)
			{
			if (wptr->lmissl[j].channel == (unsigned char)channel)
				{
				wptr->lmissl[j].channel = 255;
				}
			}
		}
	}
}


/**************************************************************************
** Check if the ION cannons need to be fired                             **
**************************************************************************/

void  FUNC fireion(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

if (ptr->hostile > 1)
	{
	plnum = ptr->hostile - 10;
	getplanetdat(usrn);
	if (plptr->items[I_IONCANNON].qty > 0)
		{
		ptr->lastfired = -1;
		if (ptr->shieldstat == SHIELDUP)
			{
			ptr->damage += (idammax * rndm(.15));
			prfmsg(IHIT1);
			outprfge(ALWAYS,usrn);
			shieldhit(ptr,usrn,(gernd()%50)+40);
			}
		else
			{
			prfmsg(IHIT2);
			outprfge(ALWAYS,usrn);
			ptr->damage += (idammax * (rndm(.50) + .50));
			}
		}
	}
}


/**************************************************************************
** Self Destruct countdown                                               **
**************************************************************************/

void  FUNC destruct(ptr,usrn)

WARSHP *ptr;
int           usrn;
{
WARSHP *wptr;
int	zothusn;
double	ddist;

if (ptr->destruct > (byte)0)
	{
	if (--ptr->destruct > (byte)0)
		{
		if (ptr->destruct==10)
			{
			prfmsg(SELFD2A,ptr->shipname);
			outrange(ALWAYS,&ptr->coord);
			}

		if (ptr->destruct==5)
			{
			prfmsg(SELFD2B,ptr->shipname);
			outrange(ALWAYS,&ptr->coord);
			}

		if (ptr->destruct==2)
			{
			prfmsg(SELFD2C,ptr->shipname);
			outrange(ALWAYS,&ptr->coord);
			}

		prfmsg(SELFD2,ptr->destruct);
		outprfge(ALWAYS,usrn);
		}
	else
		{
		prfmsg(SELFD3);
		ptr->damage = 101;
		outprfge(ALWAYS,usrn);
		prfmsg(SELFD3A,ptr->shipname);
		outrange(ALWAYS,&ptr->coord);
		for (zothusn=0 ; zothusn < nships ; zothusn++)
			{
			wptr=warshpoff(zothusn);
			if (ingegame(zothusn))
				{
				ddist = cdistance(&ptr->coord,&wptr->coord);
				ddist *= 10000;
				setsect(wptr);
				if (ddist < ((double)MINERANGE) && (xsect != 0 || ysect != 0))
					{
					ddist = 1.0-(ddist/((double)DESTRUCTRANGE));
					if (ddist < 0)
						ddist = 0;
					ddist = ddist*ddist*ddist;
					if (wptr->shieldstat == SHIELDUP)
						{
						damage = (unsigned)(ddist*minedammax);
						damage = damage*((ptr->shpclass/2)+1);
						damage = damage/(gernd()%5+wptr->shieldtype);
						damstr(damage);
						prfmsg(SELFD6,gechrbuf);
						outprfge(ALWAYS,zothusn);
						shieldhit(wptr,zothusn,damage+20);
						}
					else
						{
						damage = (unsigned)(ddist*minedammax);
						damage = damage*((ptr->shpclass/2)+1);
						damstr(damage);
						prfmsg(SELFD7,gechrbuf);
						outprfge(ALWAYS,zothusn);
						}
					wptr->damage += (double)damage;
					wptr->lastfired = -1;
					}
				}
			}
		}
	}
}

/**************************************************************************
** Verify percent for validaty                                           **
**************************************************************************/

int  FUNC valpcnt(ptr,minnum,maxnum)
char *ptr;
unsigned  minnum,maxnum;
{
int  val;
char *inpptr;

	  stripb(ptr);  /* BJ Changed */
	  if (inplen != 0) {
	  for (inpptr = ptr; isdigit(*inpptr) ; inpptr++) {
	  }
	  if (*inpptr == '\0' || *inpptr == ' ') {
			 if ((val=atoi(ptr)) >= minnum && val <= maxnum) {
			 warsptr->percent = val;
			 return(1);
			 }
	  }
	  }
	  prfmsg(NUMOOR,minnum,maxnum);
	  outprfge(ALWAYS,usrnum);
	  return(0);
}

/**************************************************************************
** Verify degree for validity                                            **
**************************************************************************/

int  FUNC valdegree(ptr)
char *ptr;
{
int  val;

if (strlen(ptr) != 0)
	{
	val=atoi(ptr);
	if (val >= -180 && val <= 180)
		{
		warsptr->degrees = val;
		return(1);
		}
	}
prfmsg(NUMOOR,-180,180);
outprfge(ALWAYS,usrnum);
return(0);
}

/**************************************************************************
** Assess any random damage                                              **
**************************************************************************/

void  FUNC randamage(ptr,usrn)

WARSHP *ptr;
int           usrn;
{
int    a;

if (ptr->shieldtype == 20)
	return;

if (ptr->damage > 20.0)
	{
	/* toss dice to see if a system has been damaged */

	a = (int)rndm((101.0 - ptr->damage)/1.5);
	if (a == 0)
		{
		/* ok so there is damage - what system is it */
		a = gernd()%6;
		switch (a)
			{
			case 0:  /* shields */
				if (shipclass[ptr->shpclass].max_shlds > 0)
					{
					prfmsg(RNDSHLD);
					outprfge(ALWAYS,usrn);
					ptr->shield = 0 - (int)rndm((ptr->damage+10.0));
					ptr->shieldstat = SHIELDDM;
					}
				else
					{
					prfmsg(RNDXX1);
					outprfge(ALWAYS,usrn);
					}
				break;

			case 1:  /* phasers */
				if (shipclass[ptr->shpclass].max_phasr > 0)
					{
					prfmsg(RNDPHSR);
					outprfge(ALWAYS,usrn);
					ptr->phasr = 0 - (int)rndm((ptr->damage+10.0));
					}
				else
					{
					prfmsg(RNDXX2);
					outprfge(ALWAYS,usrn);
					}
				break;

			case 2:  /* dummy next */
				if (shipclass[ptr->shpclass].max_torps > 0 ||
					 shipclass[ptr->shpclass].max_missl > 0)
					{
					prfmsg(RNDFCNT);
					outprfge(ALWAYS,usrn);
					ptr->firecntl = gernd()%20;
					}
				else
					{
					prfmsg(RNDXX3);
					outprfge(ALWAYS,usrn);
					}
				break;

			case 3:  /* cloak next */
				if (shipclass[ptr->shpclass].max_cloak > 0)
					{
					prfmsg(RNDCLOK);
					outprfge(ALWAYS,usrn);
					ptr->cloak = 0 - (int)rndm((ptr->damage+10.0));
					}
				else
					{
					prfmsg(RNDXX4);
					outprfge(ALWAYS,usrn);
					}
				break;

			case 4:  /* tactical display next */

				prfmsg(RNDTACT);
				outprfge(ALWAYS,usrn);
				ptr->tactical = 0 - (int)rndm((ptr->damage+10.0));
				break;

			case 5:  /* navigational */
				prfmsg(RNDNAVG);
				outprfge(ALWAYS,usrn);
				ptr->helm = 0 - (int)rndm((ptr->damage+10.0));
				break;

			case 6:  /* not used */
				break;
		 }
	 }
  }
}


/**************************************************************************
** Determine the damage amount                                           **
**************************************************************************/

unsigned   FUNC pdamage(wptr,dist,foc)
WARSHP	*wptr;
double dist;
int    foc;

{
double dd,fd,dp,factor,disfact;
unsigned dam;

if (wptr->where == 1)
	{
	factor = hpfirdst;
	dd = 1-(dist/40000.0);
	if (dd<0)
		dd = 0;
	dp = pow(dd,factor);
	dam = hpdammax * dp;
	}
else
	{
	factor = pfirdist;
	disfact = 20000.0+((double)wptr->phasrtype*4000.0);
	dd = 1-(dist/disfact);
	if (dd<0)
		dd=0;
	fd = 1 - ((double)foc/11);
	dp = (pow(dd,factor)) * (fd*fd) * (wptr->phasr/100);
	dam = pdammax * dp;
	}

logthis(spr("Pdamage %s %ld %d",wptr->userid,(long)dist,dam));
return (dam);

}


/**************************************************************************
** set the xsect and ysect coordinates given a ship pntr                 **
**************************************************************************/

void  FUNC setsect(ptr)
WARSHP *ptr;
{
	xsect = coord1(ptr->coord.xcoord);
	ysect = coord1(ptr->coord.ycoord);
	xcord = coord2(ptr->coord.xcoord);
	ycord = coord2(ptr->coord.ycoord);
	pkey.xsect = xsect;
	pkey.ysect = ysect;
	pkey.plnum = 0;
}

/**************************************************************************
** move one coord to another (direction is <-- )                **
**************************************************************************/

void  FUNC movecoord(pointb, pointa)
COORD   *pointb, *pointa;
{

pointb->xcoord = pointa->xcoord;
pointb->ycoord = pointa->ycoord;

}

/**************************************************************************
** Compare two coords to determine if they are equal                     **
**************************************************************************/

int  FUNC samesect(pointb, pointa)
COORD   *pointb, *pointa;
{
int     ax,ay,bx,by;
ax = coord1(pointa->xcoord);
ay = coord1(pointa->ycoord);
bx = coord1(pointb->xcoord);
by = coord1(pointb->ycoord);

return ((ax == bx) && (ay == by));
}




/**************************************************************************
** genearas function. Compare for length of element 1 only                 **
**************************************************************************/
int  FUNC genearas(str1,str2)
char *str1,*str2;

{
return(sameto(str1,str2));
/*
return(!strnicmp(str1,str2,strlen(str1)));
 BJ CHANGED */
}


/**************************************************************************
** MAIL functions                                                        **
**************************************************************************/

int  FUNC mailscan(userid,class)
char    *userid;
int	class;
{

strncpy(mailkey.userid,userid,UIDSIZ);
mailkey.class = class;

setbtv(gebb4);

/* if class = 0 scan if user has ANY mail */
if (class == 0)
	{
	if (qeqbtv(userid,0))
		{
		/* DEBUG
		prf("mail.userid=%s\rmail.class=%d\rmail.type=%d\r",mail.userid,mail.class,mail.type);*/
		return(TRUE);
		}
	}
else
/* otherwize see if he has this class of mail */
if (qeqbtv(&mailkey,1))
	{
	return(TRUE);
	}
return(FALSE);
}

int mailread(userid,class)
char    *userid;
{

strncpy(mailkey.userid,userid,UIDSIZ);
mailkey.class = class;
mailkey.msgno = 0;

setbtv(gebb4);

setmem(gemsg,FIXEDMSGSIZ,0);

if (qeqbtv(&mailkey,1))
	{
	gcrbtv(gemsg,1);
	prf(".[1;34m------------------------------------------------------------------------------.[1;36m\r");
	prf(gemsg->text);
	prf(".[1;34m------------------------------------------------------------------------------.[1;0m");
	outprfge(ALWAYS,usrnum);
	
	delbtv();

	return(TRUE);
	}


prfmsg(MAIL1);
outprfge(ALWAYS,usrnum);
return(FALSE);
}


void  FUNC mailit(flag)
int     flag;
{
int i;

setmbk(gemb);
clrprf();

if (flag == 1)
	{
	if (instat(mail.userid,gestt))
		{
		if (othusp->substt >= FIGHTSUB)
			{
			return;
			}
		}
	}

mail.stamp = cofdat(today());
sprintf(mail.dtime,"%s - %.5s",ncedat(today()),nctime(now()));

prfmsg(MAIL2,mail.dtime,mail.userid);

switch (mail.type)
	{
	case MESG02:
	case MESG03:
	case MESG04:
	case MESG05:
		strcpy(mail.topic,"Distress Message");
		sprintf(gechrbuf,"%ld",mail.long1);
		prfmsg(mail.type,mail.name1,mail.int1,mail.int2,mail.string1,mail.name2,gechrbuf);
		sendit();
		break;

	case MESG06:
	case MESG07:
		strcpy(mail.topic,"Distress Message");
		sprintf(gechrbuf,"%ld",mail.long1);
		prfmsg(mail.type,mail.name1,mail.int1,mail.int2,gechrbuf);
		sendit();
		break;
	
	case MESG08:
	case MESG09:
	case MESG10:
	case MESG11:
	case MESG12:
	case MESG13:
	case MESG14:
	case MESG15:
	case MESG16:
	case MESG17:
	case MESG18:
	case MESG19:
	case MESG19A:
	case MESG19B:
	case MESG30:
		strcpy(mail.topic,"Status Message");
		sprintf(gechrbuf,"%ld",mail.long1);
		prfmsg(mail.type,mail.name1,mail.int1,mail.int2,gechrbuf);
		sendit();
		break;
	
	case MESG20:

		strcpy(mail.topic,"Production Report");
		memcpy(&tmpstat,&mail,sizeof(MAILSTAT));
		prfmsg(tmpstat.type,tmpstat.name1,tmpstat.int1,tmpstat.int2);
		sprintf(gechrbuf2,"%ld",tmpstat.cash);
		prf("Cash %s  ",gechrbuf2);			
		sprintf(gechrbuf2,"%ld",tmpstat.debt);
		prf("Debt %s  ",gechrbuf2);			
		sprintf(gechrbuf2,"%ld",tmpstat.tax);
		prf("Tax %s  \r",gechrbuf2);			
		for(i=0;i<NUMITEMS;++i)
			{
			setmem(gechrbuf,20,'.');
			gechrbuf[20-strlen(item_name[i])]=0;
			sprintf(gechrbuf2,"%ld",tmpstat.itemqty[i]);
			prf("%s%s%14s\r",item_name[i],gechrbuf,gechrbuf2);
			}
		sendit();
		break;

	default:
		prfmsg(MAIL3,mail.type);
		sendit();
		break;
	
	}

rstmbk();
}

int	 FUNC sendit()
{

/* don't send mail to non-live players */

if (mail.userid[0] == '*')
	return(FALSE);

setmem(gemsg,FIXEDMSGSIZ,0);

/* only increment if using MBBS Mail */
if (!usegemsg)
	gemsg->msgno=++sv.msgtot;
else
	gemsg->msgno = 0;

strcpy(gemsg->userto,mail.userid);
strcpy(gemsg->from,"** Galactic Empire **");
strcpy(gemsg->to,mail.userid);
strcpy(gemsg->topic,mail.topic);
gemsg->auxtpc[0] = 0;

if (usegemsg)
	gemsg->flags = mail.class;
else
	gemsg->flags=0;

gemsg->crdate=today();
gemsg->crtime=now();

/* robbed nreply for time stamp */
if (usegemsg)
	gemsg->nreply = cofdat(today());
else
	gemsg->nreply=0;

prf2tx();

/*if user has selected to use the GE mail system*/

if (usegemsg)
	{
	return(sendgemsg(gemsg,mail.userid));
	}

return(sendmsg(gemsg,mail.userid));

}

void	FUNC prf2tx(void)                /* xfer prfbuf contents to message text area */
{
char *cp;

stpans(prfbuf);
if (strlen(prfbuf) >= GEMSGSIZ) 
	{
	prfbuf[GEMSGSIZ-1]='\0';
	}
for (cp=prfbuf ; *cp != '\0' ; cp++) 
	{
	if (*cp == '\n') 
		{
		*cp='\r';
		}
	}
strcpy(gemsg->text,prfbuf);
clrprf();
}


int	 FUNC sendgemsg(struct message *msgptr,char *usrid)

{

usrid=usrid;

setbtv(gebb4);

dinsbtv(msgptr);

rstbtv();

return(TRUE);

}

/**************************************************************************
** Shield functions                                                      **
**************************************************************************/

void  FUNC shieldup(wptr,usrn)
WARSHP  *wptr;
int             usrn;
{
prfmsg(SHLDCHP);
outprfge(FILTER,usrn);
wptr->shieldstat = SHIELDUP;
}


void  FUNC shielddn(wptr,usrn)
WARSHP  *wptr;
int             usrn;
{
usrn = usrn; /* avoid the warning */
prfmsg(SHLDDN);
outprfge(FILTER,usrn);
wptr->shieldstat = SHIELDDN;
}


int      FUNC shieldhit(wptr,usrn,dam)
WARSHP  *wptr;
int             usrn;
int             dam;   /* 0% to 100% damage */
{
int             knock;

double  dmax,
			ddam;

/* There are 20 kinds of shields Mark-1 to Mark-20 */
usrn = usrn; /* avoid the warning */

dmax = (double)( 80 - ((int)wptr->shieldtype * SHIELD_FACTOR));

if (wptr->shieldtype == 20)
	dmax = 0;		

if (dmax < 0) dmax = 0;

ddam = dam;
ddam /=100;   /* make it a percentile */

knock = (int)(dmax * ddam);

wptr->shield -= knock;
if (wptr->shield <=2 )
	{
	prfmsg(SHDAMAG);
	outprfge(ALWAYS,usrn);
	wptr->shieldstat = SHIELDDM;
	wptr->shield -= (knock*3);
	}
else
if (wptr->shield < SHMINCHG )
	{
	prfmsg(SHKNKDN);
	outprfge(ALWAYS,usrn);
	}
return (knock);
}


void FUNC shieldrep(wptr,usrn)
WARSHP  *wptr;
int             usrn;
{
wptr->shield += (int)(wptr->shieldtype);

if (wptr->shieldtype == 20)
	wptr->shield = 1;

if (wptr->shield > 0)
	{
	wptr->shieldstat = SHIELDDN;
	wptr->shield = 0;
	prfmsg(SHREPR);
	outprfge(ALWAYS,usrn);
	}
}

void  FUNC shieldchg(wptr,usrn)
WARSHP  *wptr;
int             usrn;
{
/*STATIC*/
static  int     maxcharge;
static  int     pcnt;
int                     type;

type = wptr->shieldtype;

if (type < 20)
	wptr->energy -=  (type*SHENGUSE);


charge(wptr,&maxcharge,&pcnt); /* go figure maxcharge and percent */

if (wptr->shieldtype == 20 && wptr->shield < maxcharge)
	wptr->shield = maxcharge -1;

if (wptr->shield < maxcharge)
	{
	wptr->shield += (type*3);
	if (wptr->shield >= maxcharge)
		{
		wptr->shield = maxcharge;
		prfmsg(SHLDUP);
		}
	else
		{
		charge(wptr,&maxcharge,&pcnt); /* go figure maxcharge and percent */
		prfmsg(SHLDAT,pcnt);
		}
	outprfge(FILTER,usrn);
	}
}

/* calculate the relative charge the shields are at */

void  FUNC charge(wptr,max,pct)
WARSHP  *wptr;
int             *max;
int             *pct;
{
*max = 40 + (wptr->shieldtype*10);
*pct = (wptr->shield*100)/(*max);
}

/* check if the goods to be added will cause wieght to be exceeded */

int  FUNC chkweight(wptr,itm,amt)
WARSHP  *wptr;
int             itm;
long            amt;

{
int     i;
long    total = 0;

for (i=0; i<NUMITEMS; ++i)
	{
	total += (wptr->items[i]*weight[i])/100L;
	}
total += (amt*weight[itm])/100;

return ((total <= shipclass[wptr->shpclass].max_tons)
		&& (wptr->items[itm]+amt < 0xEFFFFFFFL));
}

/* tell the total weight on board */

long  FUNC calcweight(wptr)
WARSHP  *wptr;
{
int     i;
long    total = 0;

for (i=0; i<NUMITEMS; ++i)
	{
	total += (wptr->items[i]*weight[i])/100L;
	}
return (total);
}


/* Figure the ship letter for this user */

char  FUNC shpltr(usrn,ship)
int     usrn,ship;
{
int i;
SCANTAB *sptr;

sptr = &scantab[usrn];

for (i=0; i<NOSCANTAB; ++i)
	{
	if (sptr->ship[i].shipno == ship)
		return(sptr->ship[i].letter);
	}
return('?');
}

/* return the proper name for this user given a pointer to the ship */

char    * FUNC username(ptr)
WARSHP  *ptr;
{
if (shipclass[ptr->shpclass].max_type == CLASSTYPE_CYBORG)  /* CYBORG */
	return(ptr->shipname);
if (shipclass[ptr->shpclass].max_type == CLASSTYPE_DROID)  /* DROID */
	return(ptr->shipname);
return(ptr->userid);
}

/* data logger */

void	 FUNC logthis(str)
char	*str;

{

FILE	*hdl;
int	idate,itime;
char	*c_date,*c_time;



if (!logflag)
	return;

hdl = fopen("geout.log","at+");

if(hdl != (FILE *)0)
	{
	idate = today();
	itime = now();
	c_date = ncdate(idate);
	c_time = nctime(itime);

	fprintf(hdl,"[%s %s] %s\r",c_date,c_time,str);
	fclose(hdl);
	}

return;
}

WARUSR	* FUNC warusroff(int	usrn)
{
if(usrn >= 0 && usrn < nships)
	return((WARUSR *)((long)(warusr_ecl+(usrn<<3))<<16));
else
	{
	geshocst(0,spr("GE:WARUSROFF:bad usrn [%d]",usrn));
	return((WARUSR *)((long)0));
	}
}

WARSHP	* FUNC warshpoff(int usrn)
{
if(usrn >= 0 && usrn < nships)
	return((WARSHP *)((long)(warshp_ecl+(usrn<<3))<<16));
else
	{
	geshocst(0,"GE:BAD WARSHPOFF CALL");
	logthis(spr("WARSHPOFF:bad usrn [%d]",usrn));
	return((WARSHP *)((long)0));
	}
}

double  FUNC ton_fact(ptr,damfact)
WARSHP  	*ptr;
double	damfact;

{

double 	temp;

temp = damfact / ((double)(shipclass[ptr->shpclass].damfact)/100.0);

return(temp);
}

char * FUNC showarp(double speed)
{
if (speed == 0.0)
	sprintf(warpbuf,"0.00");
else
if ((speed/1000.0) > 99.999)
	sprintf(warpbuf,"Hyper");
else
	sprintf(warpbuf,"%.2f",speed/1000.0);
return(warpbuf);
}

