
/***************************************************************************
 *                                                                         *
 *   GEPLANET.C                                                            *
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
#include "mem.h"
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


#define  GEPLANET 1

#include "geglobal.h"

/* LOCAL GLOBAL DEFS *****************************************************/

WORMTAB	wormtab[MAXPLANETS];
int		wormnum;



void	 FUNC check_spy()

{
int	spycnt,odds,i,j;

long	int	itemcnt;

double		d_itemcnt, d_rptcnt, d_odds;

/* is the spy owned by the planet owner? - if so kill him */
if (sameas(plptr->spyowner,plptr->userid))
	plptr->spyowner[0] = 0;

if	(plptr->spyowner[0] != 0)
	{
/*	if(!uidxst(plptr->spyowner))
		{
		plptr->spyowner[0]=0;
		}
	else */
		{
		setmbk(gemb);
		xsect = coord1(plptr->coord.xcoord);
		ysect = coord1(plptr->coord.ycoord);
		logthis(spr("Spy on %s in %d %d present",plptr->name,xsect,ysect));
		/* there is a spy on this planet */
		/* calculate his odds of getting caught */
		spycnt = (int)plptr->items[I_SPY].qty;

		/*DEBUG
		spycnt = 1;*/

		if (spycnt > 0)
			{
			odds = (50/spycnt)+1;

			if (gernd()%odds == 0)
				/* caught him */
				{
				logthis(spr("Spy on %s in %d %d caught",plptr->name,xsect,ysect));
				/* mail message to spy owner informing him of the death of his spy */
				clrprf();
				prfmsg(SPYC1,plptr->name,xsect,ysect);
				strcpy(mail.userid,plptr->spyowner);
				strcpy(mail.topic,"** Official Protest **");
				mail.class = MAIL_CLASS_DISTRESS;
				sendit();

				/* mail message to the planet owner informing him that a spy was caught */
				clrprf();
				prfmsg(SPYC2,plptr->name,xsect,ysect,plptr->spyowner,plptr->spyowner);
				strcpy(mail.userid,plptr->userid);
				strcpy(mail.topic,"** Official Protest **");
				mail.class = MAIL_CLASS_DISTRESS;
				sendit();
				clrprf();

				/* kill the spy */
				plptr->spyowner[0]=0;

				return;
				}
			}

		/* calculate the odds of the spy finding some usefull information */

		if (gernd()%10 == 0) 
			{
			logthis(spr("Spy on %s in %d %d found stuff",plptr->name,xsect,ysect));

			/* pick a topic to report */
			itemcnt = -1;
			for (j=0;j<10;++j)
				{
				i = gernd()%NUMITEMS;
				itemcnt = plptr->items[i].qty;
				if (itemcnt > 0)
					break;
				}

			if (itemcnt > 0) 
				{

				/* calculate the accuracy of the information */
				d_odds = 50.0+rndm(48.0);
				odds = d_odds;

				d_itemcnt = itemcnt;

				/* convert it to a factor */
				d_odds = (100.0 - d_odds)/100.0;

				d_rptcnt = d_itemcnt - (d_itemcnt*rndm(d_odds)) + (d_itemcnt*rndm(d_odds)); /* calc the deviation */

				itemcnt = d_rptcnt;

				clrprf();
				prfmsg(SPYM2,plptr->name,xsect,ysect,odds,item_name[i],spr("%ld",itemcnt));
				strcpy(mail.userid,plptr->spyowner);
				strcpy(mail.topic,"Intelligence Report");
				mail.class = MAIL_CLASS_DISTRESS;
	
				sendit();
				clrprf();
				}
			}
		}
	}
}


void  FUNC multiply()
{


int i,j;
double qty,men,rate,hrs,fact,tfact,taxfact,maxf;
long max,cnt;
unsigned long temp;

/* kill off half the troops if no food  (troops eat first)*/

if ((plptr->items[I_TROOPS].qty/100)> plptr->items[I_FOOD].qty)
	{
	cnt = plptr->items[I_TROOPS].qty / 8;
	plptr->items[I_TROOPS].qty -= cnt;

	mail.class = MAIL_CLASS_DISTRESS;
	mail.type = MESG06;
	strncpy(mail.userid,plptr->userid,UIDSIZ);
	strcpy(mail.name1,plptr->name);
	mail.int1 = plptr->xsect;
	mail.int2 = plptr->ysect;
	mail.long1 = cnt;
	mailit(0);
	}

/* eat the food */
temp = plptr->items[I_FOOD].qty;
if (temp > plptr->items[I_TROOPS].qty/100)
	{
	temp -= plptr->items[I_TROOPS].qty/100;
	
	plptr->items[I_FOOD].qty = temp;
	}
else
	plptr->items[I_FOOD].qty = 0;

/* kill off half the men if no food */

if ((plptr->items[I_MEN].qty/100)> plptr->items[I_FOOD].qty)
	{
	cnt = plptr->items[I_MEN].qty / 8;
	
	temp = plptr->items[I_MEN].qty;

	if (temp > cnt)
		temp -= cnt;
	else
		temp = 0;

	plptr->items[I_MEN].qty = temp;

	mail.class = MAIL_CLASS_DISTRESS;
	mail.type = MESG07;
	strncpy(mail.userid,plptr->userid,UIDSIZ);
	strcpy(mail.name1,plptr->name);
	mail.int1 = plptr->xsect;
	mail.int2 = plptr->ysect;
	mail.long1 = cnt;
	mailit(0);
	}

taxfact = 1-((float)plptr->taxrate/120.0);

/* take any donated cash */

temp = (long)(baseprice[I_GOLD]);
temp = (plptr->items[I_GOLD].qty)*temp;
plptr->cash += temp;

plptr->items[I_GOLD].qty = 0;


for (i=0;i<NUMITEMS;++i)
	{
/* qty = plptr->items[i].qty;*/
	men = (float)plptr->items[I_MEN].qty;
	rate= (float)plptr->items[i].rate;
	hrs = (float)manhours[i];
	hrs = hrs/10000.0;

	fact = 1.0;
	
	qty = (men * (rate/100.0)*(hrs/6.0))/7.0;

	fact *= ((float)(plptr->enviorn+plptr->resource+2) * .25);

	fact *= taxfact;

	j = (6 - (plptr->resource+plptr->enviorn))*10;
	tfact = .95-((float)j/100);
	plptr->cash = (float)plptr->cash*tfact;

	if (plptr->cash > 0) /* if plenty of cash increase production */
		fact *= 1.5;
	temp = plptr->items[i].qty;
	temp += (long)(qty * fact);

/* cap out planet maximums */

	maxf = maxpl[i];

	max = (long)(maxf * fact);

	sprintf(gechrbuf,"Pl prod %d %d %s %s max:%ld f:%g hrs:%f qty:%f maxf:%f cnt:%ld",
		plptr->xsect,
		plptr->ysect,
		plptr->userid,
		item_name[i],
		max,
		fact,
		hrs,
		qty,
		maxf,
		temp);

	logthis(gechrbuf);

	if (plptr->items[i].qty <= max && temp >= max)
		{
		logthis("Mailing production report");
		temp = max;
		mail.class = MAIL_CLASS_PRODRPT;
		mail.type = MESG08+i;
		strncpy(mail.userid,plptr->userid,UIDSIZ);
		strcpy(mail.name1,plptr->name);
		mail.int1 = plptr->xsect;
		mail.int2 = plptr->ysect;
		mail.long1 = max;
		mailit(0);
		logthis("Mail sent");
		}

	if (temp > max)
		{
		temp = max;
		}
	plptr->items[i].qty = temp;
	}

fact = (float)plptr->taxrate/1200;

qty = fact * (float)plptr->items[I_MEN].qty;
plptr->tax += (long)qty;


/* see if there is a revolt */

if (!sameas(plptr->userid,"**Free**"))
	{

	/* the higher the taxes the greater the chance of revolt */
	
	tfact = ((float)plptr->taxrate/120.0)* .35;
	
	qty = (float)plptr->items[I_TROOPS].qty;
	men = (float)plptr->items[I_MEN].qty;
	
	men *=tfact;
	
	/* if there are not enough troops the odds are really high */
	
	if (men > qty)
		{
		if (gernd()%10 == 0) 
			{
	
			logthis("Revolt!!");
			cnt = plptr->items[I_TROOPS].qty / ((gernd()%8)+2);
			plptr->items[I_TROOPS].qty = cnt;
	
	
			mail.class = MAIL_CLASS_DISTRESS;
			mail.type = MESG30;
			strncpy(mail.userid,plptr->userid,UIDSIZ);
			strcpy(mail.name1,plptr->name);
			mail.int1 = plptr->xsect;
			mail.int2 = plptr->ysect;
			mail.long1 = cnt;
			mailit(0);
	
			logthis("back from mailing it");				
			strcpy(plptr->userid,"**Free**");
			}
		}
	}
}

/**************************************************************************
** look up sector. If not fnd make one... xsect and ysect are used       **
**************************************************************************/

int  FUNC getsector(sect)
COORD    *sect;
{
int	i;

/* I know when you look at this later Mike you will never remember why
   you did this. So before you go tear it apart or go crazy trying to
   figure it out I'll explain. 

   We call the getsector routine the first time to either get a previously
   created sector or to create one. If one is created and there is a
   wormhole (or more) in it the function Xgetsect returns TRUE. If so then
   we must call it again to get the sectors that are the destination points 
   of the wormhole(s). Since we don't want it to create more wormholes in
   the new sector (risking a runaway cascade effect) a flag is tripped on
   telling Xgetsector to NOT create any wormholes. 

   If on the second pass Xgetsector gets a sector that already exists then
   we have to insert a wormhole into the next planet slot and create
   the planet record. If there are already 9 planets then too bad, this
   wormhole is a one way bugger.
*/

wormnum = 0;

if (xgetsector(sect,FALSE))
	{
	logthis("GE:DBG:Getsector-had worms");
	for (i=0;i<wormnum;++i)
		{
		logthis("GE:DBG:Getsector-get worm dest sect");

		xgetsector(&wormtab[i].coord,TRUE);
		if (sector.numplan < MAXPLANETS)
			{
			/* insert new wormhole */
			setmem(&worm,sizeof(GALWORM),0);
			worm.xsect = pkey.xsect;
			worm.ysect = pkey.ysect;
			worm.plnum = sector.numplan+1;
			worm.coord.xcoord =  wormtab[i].coord.xcoord;
			worm.coord.ycoord =  wormtab[i].coord.ycoord;
			worm.visible = 1;
			worm.destination.xcoord    = wormtab[i].dest.xcoord;
			worm.destination.ycoord    = wormtab[i].dest.ycoord;
			worm.type   =  PLTYPE_WORM;

		logthis("GE:DBG:Getsector-add dest worm record");
			/* write the database record */
			gesdb(GEADD,(PKEY *)&worm,(GALSECT *)&worm);

		logthis("GE:DBG:Getsector-update sector record");

			/* now update the sector record */
			sector.ptab[sector.numplan].type = worm.type;
			sector.ptab[sector.numplan].coord.xcoord = worm.coord.xcoord;
			sector.ptab[sector.numplan].coord.ycoord = worm.coord.ycoord;
			sector.numplan++;
			gesdb(GEUPDATE,(PKEY *)&sector,&sector);
			}
		}
	/* if we had to add another sector get the original again */
	logthis("GE:DBG:Getsector-get orig record");
	xgetsector(sect,FALSE);
	}
return(0);
}

int  FUNC xgetsector(sect,wormy)
COORD    *sect;
int		wormy;
{

int   p,i,k,j;

double   ddist;
byte     tooclose;

/* Build the integer key for lookup */

pkey.xsect = coord1(sect->xcoord);
pkey.ysect = coord1(sect->ycoord);
pkey.plnum = 0;

if (!gesdb(GEGET,&pkey,&sector))
	{
	/* Didn't find a record - make one */
	logthis("GE:DBG:Getsector-new sect");

	setmem(&sector,sizeof(GALSECT),0);

	sector.xsect = pkey.xsect;
	sector.ysect = pkey.ysect;
	sector.plnum = 0;
	sector.type  = SECTYPE_NORMAL;

	/* figure out how many planets this sector has */
	if (gernd()%plodds == 0)
		p = gernd()%maxplanets;
	else
		p = 0;

   logthis(spr("GE: Ins sect X:%d Y:%d Pl:%d",
		pkey.xsect,pkey.ysect,p));              

	/* Figure out where they are */
	if (neutral(sect))
		{
		for (i=0; i<s00plnum; ++i)
			{
			logthis(spr("GE:DBG:Bld Pl %d %d %s",i,s00[i].type,s00[i].name));

			sector.ptab[i].coord.xcoord = 0.0 + s00[i].xcoord;
			sector.ptab[i].coord.ycoord = 0.0 + s00[i].ycoord;


			if (s00[i].type == 1)
				{
				sector.ptab[i].type = PLTYPE_PLNT;
				build_plan_1(i);
				gesdb(GEADD,&pkey,(GALSECT *)&planet);
				}
			else
			if (s00[i].type == 2)
				{
				sector.ptab[i].type = PLTYPE_PLNT;
				build_plan_2(i);
				gesdb(GEADD,&pkey,(GALSECT *)&planet);
				}
			else
			if (s00[i].type == 3)
				{
				sector.ptab[i].type = PLTYPE_WORM;
				build_worm(i);
				gesdb(GEADD,&pkey,(GALSECT *)&worm);
				}
			else
				{
				sector.ptab[i].type = PLTYPE_PLNT;
				build_other(i);
				gesdb(GEADD,&pkey,(GALSECT *)&planet);
				}

			}
		/* write the sector database record */
	
		sector.plnum = 0;
		sector.type = SECTYPE_NORMAL;
		sector.numplan = s00plnum;

		pkey.plnum = 0;
		pkey.xsect = 0;
		pkey.ysect = 0;

		gesdb(GEADD,&pkey,&sector);
		}
	else
		{
		for (i=0; i<p; ++i)
			{
			/* is it a worm hole */
			if (wormy == FALSE && gernd()%wormodds == 0)
				sector.ptab[i].type = PLTYPE_WORM;
			else
				sector.ptab[i].type = PLTYPE_PLNT;
			/* place it and check distance to other planets */
			do
				{
				sector.ptab[i].coord.xcoord   = (((double)pkey.xsect)+rndm(0.800))+.1;
				sector.ptab[i].coord.ycoord   = (((double)pkey.ysect)+rndm(0.800))+.1;
				tooclose = FALSE;
				for (j=0;j<i;++j)
					{
					ddist = cdistance(&sector.ptab[i].coord,&sector.ptab[j].coord);
					if (ddist < 0.0700)
						tooclose = TRUE;
					}
				}
			while (tooclose == TRUE);

			sector.numplan = p;
			sector.type = SECTYPE_NORMAL;
			}

		logthis("GE:DBG:Getsector-add sect record");

		/* write the sector database record */
		gesdb(GEADD,(PKEY *)&sector,&sector);

		logthis("GE:DBG:Getsector-add planet records");

		/* add the planet records */
		for (i=0; i<p; ++i)
			{
			if (sector.ptab[i].type == PLTYPE_PLNT)
				{
				setmem(&planet,sizeof(GALPLNT),0);
				planet.xsect = pkey.xsect;
				planet.ysect = pkey.ysect;

				planet.plnum = i+1;
				planet.coord.xcoord =  sector.ptab[i].coord.xcoord;
				planet.coord.ycoord =  sector.ptab[i].coord.ycoord;
				planet.type   =  sector.ptab[i].type;
				planet.userid[0] = 0;
				planet.name[0] = 0;
				planet.enviorn = (char)(rndm(3.999));
				planet.resource = (char)(rndm(3.999));

				planet.cash = 0;
				planet.debt = 0;
				planet.tax = 0;
				planet.taxrate = 0;
				planet.debt = 0;
				planet.warnings = 0;
				planet.lastattack[0] = 0;
	
				strcpy(planet.password,"none");
	
				for (k = 0; k < NUMITEMS;++k)
					{

					planet.items[k].qty = 0;

					planet.items[k].rate = 0;

					planet.items[k].reserve = 0;
					planet.items[k].sell = 'N';
					planet.items[k].markup2a = 0;
					planet.items[k].sold2a = 0;
					}
				if (rndm(3.99) > 3)
					{
					for (k = 0; k < NUMITEMS;++k) 
						planet.items[k].rate = (unsigned int)rndm(5.1);
	
					planet.items[I_MEN].qty = (unsigned long)rndm(50000.0);
					planet.items[I_MEN].rate = 5+(unsigned int)rndm(25.0);
					planet.items[I_FOOD].qty = (unsigned long)rndm(3200.0);
					planet.items[I_FOOD].rate = 15+(unsigned int)rndm(15.0);
					}
				logthis("GE:DBG:Getsector-write planet record");
				/* write the database record */
				gesdb(GEADD,(PKEY *)&planet,(GALSECT *)&planet);
				}
			else
			if (sector.ptab[i].type == PLTYPE_WORM)
				{
				setmem(&worm,sizeof(GALWORM),0);
				worm.xsect = pkey.xsect;
				worm.ysect = pkey.ysect;
				worm.plnum = i+1;
				worm.coord.xcoord =  sector.ptab[i].coord.xcoord;
				worm.coord.ycoord =  sector.ptab[i].coord.ycoord;
				worm.visible = 1;
				worm.destination.xcoord    = rndm((double)univmax*2)-(double)univmax;
				worm.destination.ycoord    = rndm((double)univmax*2)-(double)univmax;
				worm.type   =  sector.ptab[i].type;

				logthis("GE:DBG:Getsector-add worm record");
				/* write the database record */
				gesdb(GEADD,(PKEY *)&worm,(GALSECT *)&worm);
				/* add to worm table so we can go build the other sector */

				wormtab[wormnum].coord.xcoord=worm.destination.xcoord;
				wormtab[wormnum].coord.ycoord=worm.destination.ycoord;
				wormtab[wormnum].dest.xcoord = worm.coord.xcoord;
				wormtab[wormnum].dest.ycoord = worm.coord.ycoord;
				++wormnum;
				}
			}
		}
	}
if (wormnum >0)
	return(TRUE);
return(FALSE);
}

void	 FUNC build_plan_1(idx)
int	idx;
{

setmem(&planet,sizeof(GALPLNT),0);
		
strncpy(planet.userid,s00[idx].owner,UIDSIZ);
strncpy(planet.name,s00[idx].name,20);
strcpy(planet.password,"none");

planet.enviorn = s00[idx].env;
planet.resource = s00[idx].res;

planet.items[I_MEN].qty = 32000;

planet.items[I_MISSILE].qty = 32000;
planet.items[I_MISSILE].sell = 'Y';
planet.items[I_MISSILE].markup2a = baseprice[I_MISSILE]*2;

planet.items[I_TORPEDO].qty = 32000;
planet.items[I_TORPEDO].sell = 'Y';
planet.items[I_TORPEDO].markup2a = baseprice[I_TORPEDO]*2;

planet.items[I_IONCANNON].qty = 32000;
planet.items[I_IONCANNON].sell = 'Y';
planet.items[I_IONCANNON].markup2a = baseprice[I_IONCANNON]*2;

planet.items[I_FLUXPOD].qty = 32000;
planet.items[I_FLUXPOD].sell = 'Y';
planet.items[I_FLUXPOD].markup2a = baseprice[I_FLUXPOD]*2;

planet.items[I_FIGHTER].qty = 32000;
planet.items[I_FIGHTER].sell = 'Y';
planet.items[I_FIGHTER].markup2a = baseprice[I_FIGHTER]*2;

planet.items[I_DECOYS].qty = 32000;
planet.items[I_DECOYS].sell = 'Y';
planet.items[I_DECOYS].markup2a = baseprice[I_DECOYS]*2;

planet.items[I_MINE].qty = 32000;
planet.items[I_MINE].sell = 'Y';
planet.items[I_MINE].markup2a = baseprice[I_MINE]*2;

planet.items[I_JAMMERS].qty = 32000;
planet.items[I_JAMMERS].sell = 'Y';
planet.items[I_JAMMERS].markup2a = baseprice[I_JAMMERS]*2;

planet.items[I_ZIPPERS].qty = 32000;
planet.items[I_ZIPPERS].sell = 'Y';
planet.items[I_ZIPPERS].markup2a = baseprice[I_ZIPPERS]*2;

pkey.plnum = idx+1;
pkey.xsect = 0;
pkey.ysect = 0;

planet.xsect = pkey.xsect;
planet.ysect = pkey.ysect;
planet.plnum = pkey.plnum;
planet.coord.xcoord = ((double)planet.xsect) + s00[idx].xcoord;
planet.coord.ycoord = ((double)planet.ysect) + s00[idx].ycoord;
planet.type = PLTYPE_PLNT;

logthis("Zygor build first time");
}

void	 FUNC build_plan_2(idx)
int	idx;
{

setmem(&planet,sizeof(GALPLNT),0);
		
		
strncpy(planet.userid,s00[idx].owner,UIDSIZ);
strncpy(planet.name,s00[idx].name,20);
strcpy(planet.password,"none");

planet.enviorn = s00[idx].env;
planet.resource = s00[idx].res;

planet.items[I_TROOPS].qty = 1032000L;
planet.items[I_TROOPS].sell = 'Y';
planet.items[I_TROOPS].markup2a = baseprice[I_TROOPS]*2;

planet.items[I_MEN].qty = 1032000L;
planet.items[I_MEN].sell = 'Y';
planet.items[I_MEN].markup2a = baseprice[I_MEN]*2;
		
planet.items[I_FOOD].qty = 1032000L;
planet.items[I_FOOD].sell = 'Y';
planet.items[I_FOOD].markup2a = baseprice[I_FOOD]*2;
		
pkey.plnum = idx+1;
pkey.xsect = 0;
pkey.ysect = 0;

planet.xsect = pkey.xsect;
planet.ysect = pkey.ysect;
planet.plnum = pkey.plnum;
planet.coord.xcoord = ((double)planet.xsect) + s00[idx].xcoord;
planet.coord.ycoord = ((double)planet.ysect) + s00[idx].ycoord;
planet.type = PLTYPE_PLNT;

logthis("T-Station build first time");

}

void	 FUNC build_other(idx)
int	idx;
{

setmem(&planet,sizeof(GALPLNT),0);
		
		
strncpy(planet.userid,s00[idx].owner,UIDSIZ);
strncpy(planet.name,s00[idx].name,20);
strcpy(planet.password,"none");

planet.enviorn = s00[idx].env;
planet.resource = s00[idx].res;

pkey.plnum = idx+1;
pkey.xsect = 0;
pkey.ysect = 0;

planet.xsect = pkey.xsect;
planet.ysect = pkey.ysect;
planet.plnum = pkey.plnum;
planet.coord.xcoord = ((double)planet.xsect) + s00[idx].xcoord;
planet.coord.ycoord = ((double)planet.ysect) + s00[idx].ycoord;
planet.type = PLTYPE_PLNT;

logthis("Other build first time");

}

void	 FUNC build_worm(idx)
int	idx;
{

setmem(&worm,sizeof(GALWORM),0);
		
strncpy(worm.name,s00[idx].name,20);


pkey.plnum = idx+1;
pkey.xsect = 0;
pkey.ysect = 0;

worm.xsect = pkey.xsect;
worm.ysect = pkey.ysect;
worm.plnum = pkey.plnum;

worm.coord.xcoord =  sector.ptab[idx].coord.xcoord;
worm.coord.ycoord =  sector.ptab[idx].coord.ycoord;

worm.type = PLTYPE_WORM;

worm.visible = 1;
worm.destination.xcoord    = rndm((double)univmax*2)-(double)univmax;
worm.destination.ycoord    = rndm((double)univmax*2)-(double)univmax;


logthis("GE:DBG:Getsector-add worm record");


/* add to worm table so we can go build the other sector */

wormtab[wormnum].coord.xcoord = worm.destination.xcoord;
wormtab[wormnum].coord.ycoord = worm.destination.ycoord;
wormtab[wormnum].dest.xcoord = worm.coord.xcoord;
wormtab[wormnum].dest.ycoord = worm.coord.ycoord;
++wormnum;

logthis("Worm build first time");

}
/**************************************************************************
** look up planet.        **
**************************************************************************/

int  FUNC getplanet(sect,plnt)
COORD    *sect;
int      plnt;
{

pkey.xsect = coord1(sect->xcoord);
pkey.ysect = coord1(sect->ycoord);
pkey.plnum = plnt;

if (!gesdb(GEGET,&pkey,(GALSECT *)&planet))
	{
   geshocst(0,spr("GE: ERROR Missing Pl Sect X:%d Y:%d",xsect,ysect));
	}
return(0);
}


/**************************************************************************
** is this players ship in the neutral zone ?    **
**************************************************************************/

int    FUNC neutral(coord)
COORD *coord;
{
xsect=coord1(coord->xcoord);
ysect=coord1(coord->ycoord);
return (xsect == 0 && ysect == 0);
}

