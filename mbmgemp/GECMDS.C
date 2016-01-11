
/***************************************************************************
 *                                                                         *
 *   GECMDS.C                                                              *
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
#include "portable.h"
#include "dosface.h"

#endif

#include "math.h"
#include "majorbbs.h"
#include "message.h"

#include "gemain.h"

#define GECMDS  1

#include        "geglobal.h"

/* LOCAL GLOBAL DEFS *****************************************************/


char *kwrd[NUMITEMS] = {        "men",
									"mis",
									"tor",
									"ion",
									"flu",
									"foo",
									"fig",
									"dec",
									"tro",
									"zip",
									"jam",
									"min",
									"gol",
									"spy"};

char *item_name[NUMITEMS] = {   "men",
										"missiles",
										"torpedos",
										"ion cannons",
										"flux pods",
										"food cases",
										"fighters",
										"decoys",
										"troops",
										"zippers",
										"jammers",
										"mines",
										"gold",
										"spy"};


void cmd_gehelp(),cmd_cloak(),cmd_gehelp(),cmd_impulse(),cmd_phas(),
	cmd_report(),cmd_rotate(),cmd_send(),cmd_scan(),cmd_shields(),
	cmd_warp(),cmd_torp(), cmd_missl(), cmd_decoy(), cmd_flux(), cmd_set(),
	cmd_orbit(), cmd_transfer(), cmd_admin(), cmd_attack(), cmd_geroster(),
	cmd_buy(),cmd_price(),  cmd_planet(), cmd_maint(),
	cmd_new(),cmd_sell(),cmd_sysop(),cmd_rename(), cmd_destruct(), cmd_abort(),
	cmd_jammer(), cmd_mine(), cmd_abandon(), cmd_zipper(), cmd_lock(),
	cmd_navigate(), cmd_who(), cmd_displ(), cmd_freq(), cmd_cls(), cmd_data(),
	cmd_team(),cmd_spy(),cmd_jettison();

#define GECMDSIZ (sizeof(gecmds)/sizeof(struct cmd))

struct  cmd     gecmds[]={
/*    command     function    0=payers only */
/*    -------     ----------- ------------- */
			 {"?",	cmd_gehelp,		1},
			 {"aba",	cmd_abandon,	0},
			 {"abo",	cmd_abort,		1},
			 {"adm",	cmd_admin,		0},
			 {"att",	cmd_attack,		0},
			 {"buy",	cmd_buy,			0},
			 {"clo",	cmd_cloak,		1},
          {"cls",	cmd_clear,		0},
          {"dat", cmd_data,		0},
			 {"dec",	cmd_decoy,		1},
			 {"des",	cmd_destruct,	1},
			 {"flu",	cmd_flux,		1},
			 {"fre",	cmd_freq,		0},
			 {"hel",	cmd_gehelp,		1},
			 {"imp",	cmd_impulse,	1},
			 {"jam",	cmd_jammer,		1},
			 {"jet",	cmd_jettison,	1},
			 {"loc",	cmd_lock,		1},
			 {"mai",	cmd_maint,		1},
			 {"min",	cmd_mine,		1},
			 {"mis",	cmd_missl,		1},
			 {"nav",	cmd_navigate,	1},
			 {"new",	cmd_new,			1},
			 {"orb",	cmd_orbit,		1},
			 {"pha",	cmd_phas,		1},
			 {"pla",	cmd_planet,		0},
			 {"pri",	cmd_price,		0},
			 {"ren",	cmd_rename,		0},
			 {"rep",	cmd_report,		1},
/*			 {"rg#",	cmd_displ,		1},*/
			 {"ros",	cmd_geroster,	1},
			 {"rot",	cmd_rotate,		1},
			 {"sca",	cmd_scan,		1},
			 {"sel", cmd_sell,		1},
			 {"sen",	cmd_send,		0},
			 {"set",	cmd_set,			1},
			 {"shi",	cmd_shields,	1},
			 {"spy",	cmd_spy,			1},
			 {"sys",	cmd_sysop,		1},
          {"tea", cmd_team,		1},
			 {"tor",	cmd_torp,		1},
			 {"tra",	cmd_transfer,	0},
			 {"war",	cmd_warp,		1},
			 {"who",	cmd_who,			1},
			 {"zip",	cmd_zipper,		1}
};


/* If you wish to add your own help commands to the list simply add the command
	name to the list and the name of the message in the MSG file. Also add the
	command to the list in the message HLPINDEX                Mike           */

struct  hlpcmd {
			char *command;
			int  helptxt;
};

struct hlpcmd gehlp[] = {
		{"abandon",			HLPABA},
		{"abort",			HLPABO},
		{"admin",			HLPADM},
		{"attack",			HLPATT},
		{"buy",				HLPBUY},
		{"cloak",			HLPCLO},
		{"cls",				HLPCLS},
		{"decoy",			HLPDEC},
		{"destruct",		HLPDES},
		{"flux",				HLPFLU},
		{"freq",				HLPFRE},
		{"help",				HLPHEL},
		{"hyper",			HLPHYP},
		{"impulse",			HLPIMP},
		{"jammer",			HLPJAM},
		{"jettison",		HLPJET},
		{"lock",				HLPLOC},
		{"maintenance",	HLPMAI},
		{"mine",				HLPMIN},
		{"missile",			HLPMIS},
		{"navigate",		HLPNAV},
		{"new",				HLPNEW},
		{"newprice",		HLPNEW2},
		{"orbit",			HLPORB},
		{"phaser",			HLPPHA},
		{"planet",			HLPPLA},
		{"price",			HLPPRI},
		{"rename",			HLPREN},
		{"report",			HLPREP},
		{"roster",			HLPROS},
		{"rotate",			HLPROT},
		{"scan",				HLPSCA},
		{"sell",				HLPSEL},
		{"send",				HLPSEN},
		{"set",				HLPSET},
		{"shield",			HLPSHI},
		{"spy",				HLPSPY},
		{"team",				HLPTEA},
		{"torpedo",			HLPTOR},
		{"transfer",		HLPTRA},
		{"warp",				HLPWAR},
		{"wormholes",		HLPWORM},
		{"zipper",			HLPZIP},

/* FYI: The above are commands and below are topics....       Mike       */

		{"battle",			HLPBATTL},
		{"battle2",			HLPBATT2},
		{"battle3",			HLPBATT3},
		{"class",			HLPCLS1},
		{"communicate",	HLPCOMMU},
		{"cybertron",		HLPCYBER},
		{"galaxy",			HLPGALXY},
		{"moving",			HLPNAVIG},
		{"planets",			HLPPLANT},
		{"planets2",		HLPPLAN2},
		{"planets3",		HLPPLAN3},
		{"scoring",			HLPSCORE},
		{"sector",			HLPSECTR},
		{"starting",		HLPSTART},
		{"strategy",		HLPSTRAT},
		{NULL,				0}
};


struct cmd * FUNC gesearch(ptr,tab,len)
char    *ptr;
struct cmd tab[];
int     len;

{

	int c;
	struct cmd *lo,*md,*hi;

	lo = &tab[0];
	hi = &tab[len-1];

	while (lo <= hi)
		{
#pragma warn -sig
		md = lo + ((hi - lo)/2L);
#pragma warn +sig
		if ((c = strncmp(ptr,md->command,3)) < 0)
			hi = md - 1;
		else if (c > 0)
			lo = md + 1;
		else return(md);
		}
	return(NULL);
}


void  FUNC gwar()
{
struct cmd *cmdptr;
char    *mv0ptr;

if (margc == 0)
	{
	warnop();
	}
else
	{
	logthis(spr("Input Ch# %d Margc %d",usrnum,margc));

	if (margc > 0)
		{
		sprintf(gechrbuf,"Margv[0] [%s]",margv[0]);
		logthis(gechrbuf);
		}
	if (margc > 1)
		{
		sprintf(gechrbuf,"Margv[1] [%s]",margv[1]);
		logthis(gechrbuf);
		}
	if (margc > 2)
		{
		sprintf(gechrbuf,"Margv[2] [%s]",margv[2]);
		logthis(gechrbuf);
		}
	if (margc > 3)
		{
		sprintf(gechrbuf,"Margv[3] [%s]",margv[3]);
		logthis(gechrbuf);
		}
	if (margc > 4)
		{
		sprintf(gechrbuf,"Margv[4] [%s]",margv[4]);
		logthis(gechrbuf);
		}

	for (mv0ptr = margv[0]; *mv0ptr != '\0'; mv0ptr++)
		{
		*mv0ptr = tolower(*mv0ptr);
		}
	if ((cmdptr = gesearch(margv[0], gecmds, GECMDSIZ)) != NULL)
		{
#ifdef PHARLAP
		if (!hasmkey(PLAYKEY) && cmdptr->cando == 0)
#else
		if (usrptr->class < PAYING && cmdptr->cando == 0)
#endif
			{
			prfmsg(NOCANDO);
			outprfge(ALWAYS,usrnum);
			}
		else
			{
			(*(cmdptr->func))();
			}
		}
	else
		{
		prfmsg(INVCMD);
		outprfge(ALWAYS,usrnum);
		}
	}
}


/**************************************************************************
** Blank line was input response                                         **
**************************************************************************/

void  FUNC warnop()
{
prfmsg(FORHELP);
outprfge(ALWAYS,usrnum);
}



void  FUNC cmd_gehelp()
{
int     ndx,i;

char	gechrbuf4[20];

setmbk(gehlpmb);


if (margc < 2 || margc > 3)
	{
	prfmsg(HLPINDEX);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (genearas(margv[1],"class"))
	{
	if (margc == 2)
		{
		prfmsg(HLPCLS1);
		for (i=0;i<cyb_class;++i)
			{
			if (shipclass[i].max_type == CLASSTYPE_USER)
				{
		  		if (shipclass[i].max_tons >999999L)
					sprintf(gechrbuf,"%5ldm",shipclass[i].max_tons/1000000L);
				else
		  		if (shipclass[i].max_tons >999)
					sprintf(gechrbuf,"%5ldk",shipclass[i].max_tons/1000);
				else
					sprintf(gechrbuf,"%5ld ",shipclass[i].max_tons);
	
		  		if (shipclass[i].max_price >999999L)
					sprintf(gechrbuf2,"%7ldm",shipclass[i].max_price/1000000L);
				else
		  		if (shipclass[i].max_price >999)
					sprintf(gechrbuf2,"%7ldk",shipclass[i].max_price/1000);
				else
					sprintf(gechrbuf2,"%7ld ",shipclass[i].max_price);
	
		  		if (shipclass[i].scanrange >999999L)
					sprintf(gechrbuf3,"%4ldm",shipclass[i].scanrange/1000000L);
				else
		  		if (shipclass[i].scanrange >999)
					sprintf(gechrbuf3,"%4ldk",shipclass[i].scanrange/1000);
				else
					sprintf(gechrbuf3,"%4ld ",shipclass[i].scanrange);
	
		  		if (shipclass[i].max_accel >999)
					sprintf(gechrbuf4,"%3dk",shipclass[i].max_accel/1000);
				else
					sprintf(gechrbuf4,"%3d ",shipclass[i].max_accel);
	
				prf("%2d %-20s%-2d %-2d %d %d %d %d %d %d %d %d %s %2d %s %s %s %3d\r",
					i+1,
					shipclass[i].typename,
					shipclass[i].max_shlds,
					shipclass[i].max_phasr,
					shipclass[i].max_torps,
					shipclass[i].max_missl,
					shipclass[i].has_decoy,
					shipclass[i].has_jam,  
					shipclass[i].has_zip,
					shipclass[i].has_mine, 
					shipclass[i].max_attk,
					shipclass[i].max_cloak,
					gechrbuf4,
					shipclass[i].max_warp,
					gechrbuf,
					gechrbuf2,
					gechrbuf3,
					shipclass[i].max_points
					);
				}		
			}
		prfmsg(HLPCLS2);
		outprfge(ALWAYS,usrnum);
		return;
		}
	else
	if (margc == 3)
		{
		i = atoi(margv[2])-1;
		if ((i > 0 || i < cyb_class) && shipclass[i].max_type == CLASSTYPE_USER)
			{
			setmbk(geshmb);
			prfmsg(shipclass[i].hlpmsg);
			outprfge(ALWAYS,usrnum);
			return;
			}
		else
			{
			prfmsg(HLPCLS3);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	else
		{
		prfmsg(HLPINDEX);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}
ndx = 0;
while (gehlp[ndx].command != NULL)
	{
	if (genearas(margv[1], gehlp[ndx].command))
		{
		prfmsg(gehlp[ndx].helptxt);
		outprfge(ALWAYS,usrnum);
		return;
		}
	++ndx;
	}

prfmsg(HLPINDEX);
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Fire engines on impulse                                               **
**************************************************************************/

void  FUNC cmd_impulse()
{
unsigned deg;
int	zothusn;
WARSHP	*wptr;
double	ddist;
if (margc < 2 || margc > 3)
	{
	prfmsg(IMPFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(IMPULSE1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc == 3)
	strcpy(gechrbuf,margv[2]);
else
	strcpy(gechrbuf,"0");


if (valpcnt(margv[1],0,99) && valdegree(gechrbuf))
	{
	if (warsptr->helm == 0)
		{
		if (warsptr->where >= 10)
			{
			refresh(warsptr,usrnum);
			prfmsg(LEAVEORB);
			warsptr->where = 0;
			warsptr->repair = 0;
			}
		deg = (unsigned)normal(warsptr->heading + (double)warsptr->degrees);
		prfmsg(ENGFIRE,deg);
		outprfge(ALWAYS,usrnum);
		warsptr->speed2b = 1000.0 * ((double)warsptr->percent/100.0);
		warsptr->head2b   = (double)deg;
		/* if I am cloaked tell the closer ones */
		if (warsptr->cloak == 10 && warsptr->speed2b > (rndm(200.0)+10.0))
			{
			for (zothusn=0 ; zothusn < nterms ; zothusn++)
				{
				wptr=warshpoff(zothusn);
				if (ingegame(zothusn) && zothusn != usrnum)
					{
					ddist = cdistance(&warsptr->coord,&wptr->coord);
					ddist *= 10000;
					
					if (ddist < (shipclass[wptr->shpclass].scanrange/2) 
						&& wptr->jammer == 0)
						{
						bearing = (int)(cbearing(&wptr->coord,&warsptr->coord,wptr->heading)+.5);
						/* slop it up +- 10 degrees on either side */
						bearing = bearing + (gernd()%20)-10;
						prfmsg(CLOK3,bearing);
						outprfge(ALWAYS,zothusn);
						}
					}
				}				
			}
		}
	else
		{
		prfmsg(HLBROKE);
		outprfge(ALWAYS,usrnum);
		}
	}
}


/**************************************************************************
** Fire engines on warp drive                                            **
**************************************************************************/

void  FUNC cmd_warp()
{
unsigned deg;
int	speed,topspeed;

if (shipclass[warsptr->shpclass].max_warp == 0)
	{
	prfmsg(WARP01);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->topspeed == 0)
	{
	prfmsg(WARPSPD2);
	outprfge(ALWAYS,usrnum);
	return;
	}


if (margc < 2 || margc > 3)
	{
	prfmsg(WARPFMT);
	outprfge(ALWAYS,usrnum);
	}
else
	{
	speed = atoi(margv[1]);
	topspeed=warsptr->topspeed;
	if (speed < 0)
		{
		prfmsg(WARP02);
		outprfge(ALWAYS,usrnum);
		return;
		}

	if (speed > topspeed+(topspeed/2))
		{
		prfmsg(WARP03);
		outprfge(ALWAYS,usrnum);
		return;
		}

	if (margc == 3)
		strcpy(gechrbuf,margv[2]);
	else
		strcpy(gechrbuf,"0");

	if (warsptr->helm == 0 && valdegree(gechrbuf))
		{
		if (speed > topspeed)
			{
			prfmsg(WARP04,topspeed);
			outprfge(ALWAYS,usrnum);
			}

		if (warsptr->where >= 10)
			{
			refresh(warsptr,usrnum);
			prfmsg(LEAVEORB);
			warsptr->where = 0;
			warsptr->repair = 0;
			}

		deg = (unsigned)normal(warsptr->heading + (double)warsptr->degrees);
		prfmsg(ENGFIRE,deg);
		outprfge(ALWAYS,usrnum);
		warsptr->speed2b = 1000.0 * (float)speed;
		warsptr->head2b   = (double)deg;
		}
	else
		{
		prfmsg(HLBROKE);
		outprfge(ALWAYS,usrnum);
		}
	}
}

/**************************************************************************
** Rotate ship                                                           **
**************************************************************************/

void  FUNC cmd_rotate()
{
unsigned deg;


if (margc != 2)
	{
	prfmsg(ROTFMT);
	outprfge(ALWAYS,usrnum);
	}
else
if (*margv[1] == '@') /* turn absolute */
	{
	if (warsptr->helm == 0)
		{
		if (warsptr->speed >= 0)
			{
			if (useenergy(warsptr,usrnum,ROTENGUSE) == 1)
				{
				*margv[1] = '+';
				deg = atoi(margv[1]);
				if (deg < 360)
					{
					prfmsg(NOWTURN,deg);
					outprfge(ALWAYS,usrnum);
					warsptr->head2b   = (double)deg;
					}
				else
					{
					prfmsg(NUMOOR,0,359);
					outprfge(ALWAYS,usrnum);
					return;
					}
				}
			else
				{
				prfmsg(NOROTPW);
				outprfge(ALWAYS,usrnum);
				}
			}
		else
			{
			prfmsg(CANTROT);
			outprfge(ALWAYS,usrnum);
			}
		}
	else
		{
		prfmsg(HLBROKE);
		outprfge(ALWAYS,usrnum);
		}
	}
else
if (valdegree(margv[1]))
	{
	if (warsptr->helm == 0)
		{
		if (warsptr->speed >= 0)
			{
			if (useenergy(warsptr,usrnum,ROTENGUSE) == 1)
				{
				deg = (unsigned)normal(warsptr->heading + (double)warsptr->degrees);
				prfmsg(NOWTURN,deg);
				outprfge(ALWAYS,usrnum);
				warsptr->head2b   = (double)deg;
				}
			else
				{
				prfmsg(NOROTPW);
				outprfge(ALWAYS,usrnum);
				}
			}
		else
			{
			prfmsg(CANTROT);
			outprfge(ALWAYS,usrnum);
			}
		}
	else
		{
		prfmsg(HLBROKE);
		outprfge(ALWAYS,usrnum);
		}
	}
}



/**************************************************************************
** Recharge engines                                                      **
**************************************************************************/

void  FUNC cmd_flux()
{

if (warsptr->items[I_FLUXPOD] == 0)
	{
	prfmsg(NOFLUX);
	}
else
	{
	--warsptr->items[I_FLUXPOD];
	warsptr->energy = ENGYMAX;
	prfmsg(FLUXLOAD);
	if (warsptr->items[I_FLUXPOD] == 0)
		prfmsg(LASTFLUX);
	}
outprfge(ALWAYS,usrnum);

}

/**************************************************************************
** Orbit a planet                                                        **
**************************************************************************/

void  FUNC cmd_orbit()

{
int got_plt;

if (margc != 2)
	{
	prfmsg(ORBFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(ORBIT4);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where >= 10)
	{
	prfmsg(ORBIT3);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = (atoi(margv[1]));
			
if (plnum <= MAXPLANETS && plnum > 0)
	{
	got_plt = getplanetdat(usrnum);
	if (got_plt)
		{
		if (plptr->type == PLTYPE_WORM)
			{
			prfmsg(ORBIT0);
			outprfge(ALWAYS,usrnum);
			return;
			}
		distance = (unsigned)(cdistance(&warsptr->coord,&plptr->coord)*10000);
		if (distance <= 250)
			{
			prfmsg(ORBIT1,plnum,plptr->name);
			warsptr->where = 10 + plnum;
			warsptr->speed = 0;
			warsptr->speed2b = 0;
			}
		else
			{
			prfmsg(ORBIT2);
			}
		}
	else
		{
		prfmsg(FOOLISH);
		}
	}
else
	{
	prfmsg(FOOLISH);
	}
outprfge(ALWAYS,usrnum);
}




/**************************************************************************
** Fire phasers                                                          **
**************************************************************************/

void  FUNC cmd_phas()

{

if (shipclass[warsptr->shpclass].max_phasr == 0)
	{
	prfmsg(PHASER0);
	outprfge(ALWAYS,usrnum);
	return;
	}


if (warsptr->phasrtype > 0)
	{
	if (warsptr->where == 1)
		{
		if (margc == 2)
			{
			if (valdegree(margv[1]))
				{
				if (warsptr->hypha == 0)
					{
					firehp(warsptr,usrnum);
					}
				else
					{
					prfmsg(HPWAIT);
					outprfge(ALWAYS,usrnum);
					}
				}
			}
		else
			{
			prfmsg(HPHAFMT);
			outprfge(ALWAYS,usrnum);
			}
		}
	else
		{
		if (margc == 2)
			{
			if (warsptr->phasr >=PMINFIRE)
				{
				if (valdegree(margv[1]))
					{
					warsptr->percent = 1;
					firep(warsptr,usrnum);
					}
				}
			else
				{
				prfmsg(PHANONE);
				outprfge(ALWAYS,usrnum);
				}
			}
		else
		if (margc == 3)
			{
			if (warsptr->phasr >=PMINFIRE)
				{
				if (valpcnt(margv[2],0,5) && valdegree(margv[1]))
					{
					firep(warsptr,usrnum);
					}
				}
			else
				{
				prfmsg(PHANONE);
				outprfge(ALWAYS,usrnum);
				}
			}
		else
			{
			prfmsg(PHAFMT);
			outprfge(ALWAYS,usrnum);
			}
		}
	}
else
	{
	prfmsg(PHA01);
	outprfge(ALWAYS,usrnum);
	}
}

void FUNC firep(ptr,usrn)
WARSHP  *ptr;
int             usrn;
{
WARSHP  *wptr;
unsigned        deg;
double factor,tonfact;


if (ptr->cloak > 0 )
	{
	prfmsg(PCLOKUP);
	outprfge(ALWAYS,usrn);
	return;
	}

if (ptr->shieldstat == SHIELDUP)
	{
	shielddn(ptr,usrn);
	}

if (ptr->phasr >=PMINFIRE)
	{
	if (neutral(&ptr->coord))
		{
		zaphim(ptr,usrn);
		return;
		}
	deg = (unsigned)normal(ptr->heading + (double)ptr->degrees);
	prfmsg(PFIRED,(int)ptr->phasr,ptr->percent);
	outprfge(FILTER,usrn);
	ptr->cantexit = FIRETICKS;
	for (othusn=0 ; othusn < nships ; othusn++)
		{
		wptr=warshpoff(othusn);
		if (ingegame(othusn) && (wptr->where != 1 || ptr->phasrtype >= phatowrp))
			{
			if (othusn != usrn && !neutral(&wptr->coord))
				{
				heading = (unsigned)vector(&ptr->coord,&wptr->coord);
				if (smallest(heading,deg) < ptr->percent+PHABIAS)
					{
					damage = pdamage(ptr,cdistance(&ptr->coord,&wptr->coord)*10000,ptr->percent);

					factor = (double)damage;
					tonfact = 1.0 + ((double)(shipclass[wptr->shpclass].max_tons)/TONFACT);

											

					factor = (factor*((double)(1+ptr->phasrtype)/2.5)) / tonfact;

					/* lower it for hyper */
					if (wptr->where == 1)
						factor = factor /2.0;

					damage = (int)(factor);

					/* sysop phaser */
					if (ptr->phasrtype == 20)
						damage = 101;

					if (damage >= 1)
						{
						wptr->lastfired = usrn;
						wptr->cantexit = FIRETICKS;
						ptr->cantexit = FIRETICKS;
						if (wptr->status == GESTAT_AUTO)    /* if cyborg -sickum */
							wptr->cybmine = usrn;
						if (wptr->shieldstat != SHIELDUP)
							{
							damstr(damage);
							prfmsg(PHITHIM,gechrbuf,username(wptr));
							outprfge(ALWAYS,usrn);
							prfmsg(PHITYOU,username(ptr),gechrbuf);
							outprfge(ALWAYS,othusn);
							wptr->damage += (double)damage;
							}
						else
							{
							shieldhit(wptr,othusn,damage); /* modify the damage */
							prfmsg(PDEFLECT,username(wptr));
							outprfge(ALWAYS,usrn);
							prfmsg(PHITDEF,shpltr(othusn,usrn),username(ptr),(int)damage);
							outprfge(ALWAYS,othusn);
							}
						randamage(wptr,othusn); /*assess any random damage */
						}
					}
				}
			}
		}

	ptr->phasr = 0;
	}
else
	{
	prfmsg(NOFIREP);
	outprfge(ALWAYS,usrn);
	}
if (ptr->shieldstat == SHIELDUP)
	{
	shieldup(ptr,usrn);
	}
}


void FUNC firehp(ptr,usrn)
WARSHP  *ptr;
int             usrn;
{
WARSHP  *wptr;
unsigned        deg;
double factor,tonfact;


if (ptr->energy >= HPMINFIR)
	{
	if (neutral(&ptr->coord))
		{
		zaphim(ptr,usrn);
		return;
		}
	deg = (unsigned)normal(ptr->heading + (double)ptr->degrees);
	prfmsg(HPFIRED,deg);
	outprfge(FILTER,usrn);
	ptr->energy -= HPFIRAMT;
	ptr->hypha = 1;
	ptr->cantexit = FIRETICKS;
	for (othusn=0 ; othusn < nships ; othusn++)
		{
		wptr=warshpoff(othusn);
		if (ingegame(othusn) && wptr->where == 1)
			{
			if (othusn != usrn && !neutral(&wptr->coord))
				{
				heading = (unsigned)vector(&ptr->coord,&wptr->coord);
				if (smallest(heading,deg) < HPBEAMW)
					{
					/* calculate the relative damage done */
					ddistance = cdistance(&ptr->coord,&wptr->coord)*10000;
					if (ddistance < (double)shipclass[ptr->shpclass].scanrange)
						{
						damage = pdamage(ptr,ddistance,0);
						factor = (double)damage;
						/* figure how big the ship is */
						tonfact = 1.0 + ((double)(shipclass[wptr->shpclass].max_tons)/TONFACT);

						factor = (factor*(double)ptr->phasrtype) / tonfact;
							
						damage = (int)(factor);

						/* sysop phaser */
						if (ptr->phasrtype == 20)
							damage = 101;

						damstr(damage);

						if (wptr->status == GESTAT_AUTO)    /* if cyborg -sickum */
							wptr->cybmine = usrn;

						prfmsg(HPHITM,gechrbuf,username(wptr));
						outprfge(ALWAYS,usrn);
						prfmsg(HPHITU,username(ptr),gechrbuf);
						outprfge(ALWAYS,othusn);
						wptr->damage += (double)damage;
						wptr->lastfired = usrn;
						wptr->cantexit = FIRETICKS;
						ptr->cantexit = FIRETICKS;
						randamage(wptr,othusn); /*assess any random damage */
						}
					}
				}
			}
		}
	}
else
	{
	prfmsg(HNOFIRP);
	outprfge(ALWAYS,usrn);
	}
}


/**************************************************************************
** Fire torpedos                                                          **
**************************************************************************/

void  FUNC cmd_torp()

{

int     shpnum;

lockwarn = TRUE;

if (shipclass[warsptr->shpclass].max_torps == 0)
	{
	prfmsg(TORP3);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(TORP2);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->shieldstat == SHIELDUP)
	{
	shielddn(warsptr,usrnum);
	}

if (warsptr->items[I_TORPEDO] == 0)
	{
	prfmsg(NOTORPS);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margv[1] == NULL || margc < 2)
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	return;
	}
	
shpnum = findshp(margv[1],1);

if (shpnum == usrnum)
	{
	prfmsg(FOOLISH);
	outprfge(ALWAYS,usrnum);
	}
else
if ( shpnum >= 0)
	{
	if (neutral(&warsptr->coord))
		{
		zaphim(warsptr,usrnum);
		return;
		}
	torp(warsptr,usrnum,shpnum);
	}
else
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	}

if (warsptr->shieldstat == SHIELDUP)
	{
	shieldup(warsptr,usrnum);
	}
}

void FUNC torp(ptr,usrn,shpnum)
WARSHP  *ptr;
int             usrn;
int             shpnum;

{
WARSHP  *wptr;

int             i;

if (lockon(ptr,0,shpnum,usrn) == 1)
	{
	for (i=0; i<MAXTORPS;++i)
		{
		wptr = warshpoff(shpnum);
		if (wptr->ltorps[i].distance == 0)
			{
			prfmsg(TFIRE1);
			outprfge(FILTER,usrn);
			--ptr->items[I_TORPEDO];
			prfmsg(TFIRE2,shpltr(shpnum,usrn));
			outprfge(FILTER,shpnum);
			wptr->ltorps[i].distance = (unsigned)(cdistance(&ptr->coord,&(wptr->coord))*10000);
			wptr->ltorps[i].distance += 20;
			wptr->ltorps[i].channel  = (unsigned char)usrn;
			return;
			}
		}
	prfmsg(TORMANY);
	outprfge(FILTER,usrn);
	}
}

/**************************************************************************
** Fire missile                                                          **
**************************************************************************/

void  FUNC cmd_missl()

{

int     shpnum,i;
WARSHP  *wptr;

unsigned energy,eng_flu;
long		eng_long;

lockwarn = TRUE;

if (shipclass[warsptr->shpclass].max_missl == 0)
	{
	prfmsg(MISS01);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->shieldstat == SHIELDUP)
	{
	shielddn(warsptr,usrnum);
	}

if (warsptr->items[I_MISSILE] == 0)
	{
	prfmsg(NOMISSL);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margv[1] == NULL || margc < 2)
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	return;
	}
	
if (margv[2] == NULL || margc < 3)
	{
	prfmsg(MISFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

eng_long = atol(margv[2]);

if (eng_long == 0 || eng_long > 50000L)
	{
	prfmsg(MISFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

energy = (unsigned)eng_long;

eng_flu = energy/misengfc;

if (eng_flu > 0 && eng_flu >= (warsptr->energy+MOVENGMIN))
	{					 
	prfmsg(MISSHRT);
	outprfge(ALWAYS,usrnum);
	return;
	}

shpnum = findshp(margv[1],1);

if (shpnum == usrnum)
	{
	prfmsg(FOOLISH);
	outprfge(ALWAYS,usrnum);
	}
else
if ( shpnum >= 0)
	{
	if (neutral(&warsptr->coord))
		{
		zaphim(warsptr,usrnum);
		return;
		}
	if (lockon(warsptr,1,shpnum,usrnum) == 1)
		{
		for (i=0; i<MAXMISSL;++i)
			{
			wptr=warshpoff(shpnum);
			if (wptr->lmissl[i].distance == 0)
				{
				prfmsg(MFIRE1);
				outprfge(FILTER,usrnum);
				--(warsptr->items[I_MISSILE]);
				warsptr->energy -= eng_flu;
				prfmsg(MFIRE2,shpltr(shpnum,usrnum));
				outprfge(FILTER,shpnum);
				wptr->lmissl[i].distance = (unsigned)(cdistance(&warsptr->coord,&(wptr->coord))*10000);
				wptr->lmissl[i].distance += 20;
				wptr->lmissl[i].channel  = (unsigned char)usrnum;
				wptr->lmissl[i].energy   = energy;
				return;
				}
			}
		prfmsg(MISMANY);
		outprfge(FILTER,usrnum);
		}
	}
else
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	}

if (warsptr->shieldstat == SHIELDUP)
	{
	shieldup(warsptr,usrnum);
	}
}


int FUNC lockon(ptr,type,ship,usrn)
WARSHP  *ptr;
int     type,ship,usrn;
{
WARSHP  *wptr;

double dist,speed,fact=0.0;

if (ptr->firecntl > 0)
	{
	prfmsg(FCBROKE);
	outprfge(ALWAYS,usrn);
	return(0);
	}

if (warsptr->jammer > 0)
	{
	prfmsg(JAMMER4);
	outprfge(ALWAYS,usrn);
	return(0);
	}

wptr= warshpoff(ship);

if  (neutral(&(wptr->coord)))
	{
	prfmsg(FCNONO);
	outprfge(ALWAYS,usrn);
	return(0);
	}

dist = cdistance(&ptr->coord,&(wptr->coord));
if (wptr->cloak < 10 && (dist*10000.0) < (double)shipclass[warsptr->shpclass].scanrange)
	{
	if (wptr->status == GESTAT_AUTO)
		wptr->cybmine = usrn;

	speed = ptr->speed + wptr->speed;

	if (type == 0) /* torpedo */
		{
		if (wptr->speed > 999)
			{
			fact = 0.0;
			}
		else
			{
			fact = (1.2-(speed/5000));
			fact *= ((5.0-dist)/tor_fact);
			}
		}

	if (type == 1) /* missile */
		fact = ((5.0-dist)/mis_fact);


	if (fact > .7)
		{
		if (lockwarn == TRUE)
			{
/*			prfmsg(LOCK1,shpltr(usrn,ship));
			outprfge(FILTER,usrn); */
			prfmsg(LOCK2,shpltr(ship,usrn));
			outprfge(FILTER,ship);
			}
		lockwarn = TRUE;
		wptr->cantexit = FIRETICKS;
		ptr->cantexit = FIRETICKS;
		return(1);
		}
	else
		{
		if (lockwarn == TRUE)
			{
			prfmsg(LOCK3,shpltr(usrn,ship));
			outprfge(FILTER,usrn);
			prfmsg(LOCK4,shpltr(ship,usrn));
			outprfge(FILTER,ship);
			}
		lockwarn = TRUE;
		wptr->cantexit = FIRETICKS;
		ptr->cantexit = FIRETICKS;
		return(0);
		}
	}
else
	{
	prfmsg(LOCK5,shpltr(usrn,ship));
	outprfge(ALWAYS,usrn);
	return(0);
	}
}

int FUNC findshp(ptr,type)
char    *ptr;
int     type; /* 0 = this sector only, 1 = everywhere */
{
char    letter;
int     shpnum,i;
double  dist;
WARSHP	*wptr;
shpnum = -1;
if (ptr[0] == '@')
	{
	if (warsptr->lock < 0 || warsptr->lock >= nships)
		{
		warsptr->lock = -1;
		prfmsg(NOLOCK);
		return(-1);
		}
	else
		{
		shpnum = warsptr->lock;
		if (!ingegame(shpnum))
			{
			warsptr->lock = -1;
			prfmsg(NOLOCK);
			return(-1);
			}
		wptr=warshpoff(shpnum);
		dist = cdistance(&warsptr->coord,&(wptr->coord));
		if ((dist*10000) > (double)shipclass[warsptr->shpclass].scanrange)
			{
			warsptr->lock = -1;
			prfmsg(NOLOCK);
			shpnum = -1;
			}
		else
			{
			shpnum = warsptr->lock;
			}
		}
	}
else
	{
	letter = toupper(*ptr);
	for(i=0;i<NOSCANTAB;++i)
		{
		if (scantab[usrnum].ship[i].letter == letter)
			{
			shpnum = scantab[usrnum].ship[i].shipno;
			break;
			}
		}

	if (i>=NOSCANTAB)
		return(-1);
	
	}

if (!ingegame(shpnum))
	{
	return(-1);
	}

wptr=warshpoff(shpnum);

if (type == 0)
	{
	if (samesect(&(wptr->coord), &warsptr->coord)
		&& wptr->cloak < 10)
		{
		return (shpnum);
		}
	else
		{
		return (-1);
		}
	}
else
if (type == 1)
	{
	if (wptr->cloak < 10)
		{
		return (shpnum);
		}
	else
		{
		return (-1);
		}
	}
return(-1);
}

/* firing in sector NEUTRAL is a big no-no */

void FUNC zaphim(ptr,usrn)
WARSHP  *ptr;
int             usrn;
{
prfmsg(ZAPHIM1);
outprfge(ALWAYS,usrn);
ptr->damage += se100dam;
}

/**************************************************************************
** Fire decoys                                                          **
**************************************************************************/

void  FUNC cmd_decoy()

{

int     i;


if (!shipclass[warsptr->shpclass].has_decoy)
	{
	prfmsg(DECOY0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(DECOY1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->items[I_DECOYS] == 0)
	{
	prfmsg(NODECOYS);
	outprfge(ALWAYS,usrnum);
	return;
	}

for (i=0; i<10;++i)
	{
	if (warsptr->decout[i] == 0)
		{
		--warsptr->items[I_DECOYS];
		warsptr->decout[i] = DECOYTIME;
		prfmsg(DECFIRE);
		outprfge(FILTER,usrnum);
		return;
		}
	}
prfmsg(DECMANY);
outprfge(FILTER,usrnum);
}


/**************************************************************************
** Launch Jammer                                                         **
**************************************************************************/

void  FUNC cmd_jammer()

{

if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP);
	outprfge(ALWAYS,usrnum);
	return;
	}


if (!shipclass[warsptr->shpclass].has_jam)
	{
	prfmsg(JAMMER0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->items[I_JAMMERS] == 0)
	{
	prfmsg(JAMMER1);
	outprfge(ALWAYS,usrnum);
	return;
	}

prfmsg(JAMMER2);
outprfge(FILTER,usrnum);
jam(warsptr,usrnum);
}

void FUNC jam(ptr,usrn)
WARSHP  *ptr;
int     usrn;
{
WARSHP *wptr;
int     zothusn;
double ddist;

usrn = usrn;
for (zothusn=0; zothusn < nships ; zothusn++)
	{
	wptr=warshpoff(zothusn);
	ddist = cdistance(&ptr->coord,&wptr->coord);
	ddist *= 10000;
	if (ingegame(zothusn)   && ddist < (double)shipclass[warsptr->shpclass].scanrange)
		{
		ddist = 1.0-(ddist/(double)shipclass[warsptr->shpclass].scanrange);
		if (ddist < 0)
			ddist = 0;
		
		wptr->jammer = (unsigned)(((double)jamtime)*ddist);
		prfmsg(JAMMER3);
		outprfge(FILTER,zothusn);
		}
	}
--ptr->items[I_JAMMERS];
ptr->cantexit = FIRETICKS;
}


/**************************************************************************
** Launch Zipper                                                         **
**************************************************************************/

void  FUNC cmd_zipper()

{

if (!shipclass[warsptr->shpclass].has_zip)
	{
	prfmsg(ZIPPER0);
	outprfge(ALWAYS,usrnum);
	return;
	}


if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->items[I_ZIPPERS] == 0)
	{
	prfmsg(ZIPPER1);
	outprfge(ALWAYS,usrnum);
	return;
	}

prfmsg(ZIPPER2);
outprfge(FILTER,usrnum);
zip(warsptr,usrnum);
}

void FUNC zip(ptr,usrn)
WARSHP  *ptr;
int     usrn;
{
MINE            *mptr;
int i;
double ddist;

usrn = usrn;


for (i=0,mptr = mines; i<nummines;++mptr,++i)
	{
	if (mptr->channel != 255)
		{
		ddist = cdistance(&ptr->coord,&mptr->coord);
		ddist *= 10000;
		if (ddist < (double)shipclass[warsptr->shpclass].scanrange)
			{
			mptr->timer = 1; /* set mine to explode next tick */
			}
		}
	}
prfmsg(ZIPPER3,warshpoff(usrn)->shipname);
outsect(FILTER,&ptr->coord,usrn,0);
--ptr->items[I_ZIPPERS];
ptr->cantexit = FIRETICKS;
}

/**************************************************************************
** Launch Mine                                                         **
**************************************************************************/

void  FUNC cmd_mine()

{
int     i;

if (!shipclass[warsptr->shpclass].has_mine)
	{
	prfmsg(MINE0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (neutral(&warsptr->coord))
	{
	prfmsg(MINE7);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->items[I_MINE] <= 0)
	{
	warsptr->items[I_MINE] = 0;
	prfmsg(MINE1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc != 2 )
	{
	prfmsg(MINFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

i = atoi(margv[1]);

if (i < 1 || i > 50)
	{
	prfmsg(MINFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (laymine(warsptr,usrnum,i) == 1)
	{
	prfmsg(MINE3,i);
	outprfge(FILTER,usrnum);
	return;
	}

prfmsg(MINE2);
outprfge(FILTER,usrnum);
}

/* split out so that cyb's can lay mines too */

int     FUNC laymine(ptr,usrn,timer)
WARSHP  *ptr;
int             usrn;
int     timer;
{
int i,cnt;

/* count up the number of mines this player has layed */

cnt = 0;

for (i=0; i<nummines;++i)
	if (mines[i].channel == (byte)usrn)
		cnt++;

if (cnt >= usermines)
	{
	return(0);
	}

for (i=0; i<nummines;++i)
	{
	if (mines[i].channel == 255)
		{
		ptr->cantexit = FIRETICKS;
		mines[i].channel = (byte)usrn;
		mines[i].timer = timer;
		mines[i].coord.xcoord = ptr->coord.xcoord;
		mines[i].coord.ycoord = ptr->coord.ycoord;
		--ptr->items[I_MINE];
		return(1);
		}
	}
return(0);
}

/**************************************************************************
** Send a message to all                                                 **
**************************************************************************/

void  FUNC cmd_send()
{
char letter;

if (margc > 2)
	{
	letter = toupper(*margv[1]);
	if (letter >= 'A' && letter <= 'C')
		{
		/* CHGD:MBM22e */
		if (pfnlvl >= 2 && profon)
			{
			prfmsg(MSGPRF);
			}
		else
			{
			if (warsptr->freq[letter-'A'] == 0)
				{
				rstrin();
				prfmsg(MSGSNT1,warsptr->shipname,letter,margv[2]);
				outwar(FILTER,usrnum,0);
				prfmsg(MSGSNT2);
				}
			else
			if (warsptr->freq[letter-'A'] < 20000)
				{
				rstrin();
				prfmsg(MSGSNT3,warsptr->shipname,letter,margv[2]);
				outsect(ALWAYS,&warsptr->coord,usrnum,warsptr->freq[letter-'A']);
				prfmsg(MSGSNT4,warsptr->freq[letter - 'A']);
				}
			else
			if (warsptr->freq[letter-'A'] >= 20000)
				{
				rstrin();
				prfmsg(MSGSNT5,warsptr->shipname,letter,margv[2]);
				outwar(ALWAYS,usrnum,warsptr->freq[letter-'A']);
				prfmsg(MSGSNT6,warsptr->freq[letter - 'A']);
				}
			}
		}
	else
		{
		prfmsg(BADCOM);
		}
	
	}
else
	{
	prfmsg(SNDFMT);
	}
outprfge(FILTER,usrnum);
}



/**************************************************************************
** Set Com Freq                                                          **
**************************************************************************/

void  FUNC cmd_freq()
{
unsigned        freq;
char letter;

if (margc < 3)
	{
	prfmsg(SETFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

letter = toupper(*margv[1]);

if (letter < 'A' || letter > 'C')
	{
	prfmsg(SETFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}


if (sameas(margv[2],"hail"))
	{
	warsptr->freq[letter - 'A'] = 0;
	prfmsg(RADSET1,letter);
	outprfge(ALWAYS,usrnum);
	return;
	}

freq = atoi(margv[2]);

if (freq == 0)
	{
	prfmsg(FREQFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (freq < 20000)
	{
	warsptr->freq[letter - 'A'] = freq;
	prfmsg(RADSET2,letter,freq);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (freq >= 20000)
	{
	warsptr->freq[letter - 'A'] = freq;
	prfmsg(RADSET3,letter,freq);
	outprfge(ALWAYS,usrnum);
	return;
	}

}

/**************************************************************************
** Generate ships report                                                **
**************************************************************************/

void  FUNC cmd_report()
{
int             max,pcnt,i;

if (margc != 2 )
	{
	prfmsg(REPFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

energy = (unsigned)warsptr->energy +.5;
damage = (unsigned)warsptr->damage +.5;
speed  = ((unsigned)warsptr->speed  +.5);
heading  = (int)(warsptr->heading+.5);

prfmsg(REP01,shipclass[warsptr->shpclass].typename,warsptr->shipname);
prfmsg(DASHES);

if (sameas(margv[1],"nav"))
	{
	prfmsg(REP35);
	if (warsptr->where == 1)
		{
		setsect(warsptr);
		prfmsg(REP02,xsect, ysect);
		prfmsg(REP03, showarp(warsptr->speed));
		prfmsg(REP04, heading);
		}
	else
	if (warsptr->where == 0)
		{
		setsect(warsptr);
		prfmsg(REP05,xsect,ysect);
		prfmsg(REP06, showarp(warsptr->speed));
		prfmsg(REP07, heading);
		}
	else
	if (warsptr->where >= 10)
		{
		setsect(warsptr);
		prfmsg(REP08,warsptr->where - 10,xsect,ysect);
		}

	setsect(warsptr);

	prfmsg(REP32,  xsect,ysect,xcord,ycord);
	if (warsptr->status == GESTAT_AUTO)
		{
		prfmsg(REP40,warsptr->status,warsptr->cybmine,warsptr->cybskill,warsptr->cybupdate);
		}
	}
else
if (sameas(margv[1],"sys"))
	{
	prfmsg(REP36);
	prfmsg(REP09,energy);
	if (warsptr->shieldtype > 0)
		{
		if (warsptr->shieldstat == SHIELDUP)
			{
			prfmsg(REP10,warsptr->shieldtype);
			charge(warsptr,&max,&pcnt);
			prfmsg(REP11B,pcnt);
			}
		else
		if (warsptr->shieldstat == SHIELDDN || warsptr->shieldstat == SHIELDDM)
			{
			prfmsg(REP11,warsptr->shieldtype);
			}
		}
	if (warsptr->phasrtype > 0)
		{
		if (warsptr->phasr > 0)
			prfmsg(REP23,warsptr->phasrtype);
		else
			prfmsg(REP24,warsptr->phasrtype);
		}

	prfmsg(REP24A,warsptr->freq[0],warsptr->freq[1],warsptr->freq[2]);



	if (shipclass[warsptr->shpclass].max_cloak == 1)
		{
		if (warsptr->cloak > 0)
			prfmsg(REP12);
		else
			prfmsg(REP13);
		}

	damage   = (unsigned) (warsptr->damage+.5);
	damstr(damage);

	prfmsg(REP14,gechrbuf);
	if (warsptr->shieldstat == SHIELDDM)
		prfmsg(REP15);
	if (warsptr->helm < 0 )
		prfmsg(REP16);
	if (warsptr->cloak < 0 )
		prfmsg(REP17);
	if (warsptr->tactical < 0 )
		prfmsg(REP18);
	if (warsptr->repair > 0)
		prfmsg(REP18A,warsptr->repair);

	prf("Ros Pos: %d\r",waruptr->rospos);

	}
else
if (sameas(margv[1],"inv"))
	{
	prfmsg(REP38);

	for (i=0; i<NUMITEMS; ++i)
		{
		if (warsptr->items[i] > 0)
			{
			sprintf(gechrbuf,"%s%s%12ld",item_name[i],gedots(26-strlen(item_name[i])),warsptr->items[i]);
			gechrbuf[0] = toupper(gechrbuf[0]);
			prf("%s\r",gechrbuf);
			}
		}
	
	sprintf(gechrbuf2,"%ld",calcweight(warsptr));
	prfmsg(REP37,gechrbuf2);
	}
else
if (sameas(margv[1],"acc"))
	{
	prfmsg(REP25);

	if (waruptr->planets == 0)
		prfmsg(REP26);
	else
		prfmsg(REP27,waruptr->planets);


	sprintf(gechrbuf,"%ld",waruptr->cash);
	prfmsg(REP28,gechrbuf);

	if (waruptr->score <= 0)
		sprintf(gechrbuf,"0");
	else
		sprintf(gechrbuf,"%ld",waruptr->score);

	prfmsg(REP30,gechrbuf);

	prfmsg(REP31,waruptr->kills);

	if (waruptr->teamcode > 0)
		{
		prfmsg(REP31A,teamname(waruptr));
		}
	}
else
	{
	prfmsg(REPFMT);
	}

prfmsg(DASHES);
outprfge(ALWAYS,usrnum);
}

void FUNC damstr(damage)
int damage;

{
if (damage < 2)
	strcpy(gechrbuf,"no");
else
if (damage < 12)
	strcpy(gechrbuf,"very light");
else
if (damage < 25)
	strcpy(gechrbuf,"light");
else
if (damage < 50)
	strcpy(gechrbuf,"moderate");
else
if (damage < 75)
	strcpy(gechrbuf,"heavy");
else
	strcpy(gechrbuf,"severe");

}


/**************************************************************************
** Scan Command                                                          **
**************************************************************************/

void  FUNC cmd_scan()
{

if (warsptr->tactical != 0)
	{
	prfmsg(TABROKE);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->jammer > 0)
	{
	prfmsg(JAMMER4);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc > 1)
	{
	if (genearas("sh",margv[1]))
		scan_sh();
	else
	if (genearas("pl",margv[1]))
		scan_pl();
	else
	if (genearas("ra",margv[1]))
		scan_ra();
	else
	if (genearas("se",margv[1]))
		scan_se();
	else
	if (genearas("lo",margv[1]))
		scan_lo();
/*	else
	if (genearas("hy",margv[1]))
		scan_hy();*/
	else
		{
		prfmsg(SCANFMT);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(SCANFMT);
	outprfge(ALWAYS,usrnum);
	}
}


/* SCAN SHIP FUNCTION */

void FUNC scan_sh()

{
int shpnum,gheading;
WARSHP  *wptr;
WARUSR	*wuptr;
char            ltr;
long	len,wid;
long	tons;

if (margc != 3)
	{
	prfmsg(SCANFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}
	
shpnum = findshp(margv[2],1);

if (shpnum == usrnum)
	{
	prfmsg(FOOLISH);
	outprfge(ALWAYS,usrnum);
	}
else
if (shpnum >= 0)
	{
	wptr = warshpoff(shpnum);
	wuptr = warusroff(shpnum);
	ddistance = cdistance(&warsptr->coord,&wptr->coord)*10000;
	if (ddistance < shipclass[warsptr->shpclass].scanrange)
		{
		bearing = (int)(cbearing(&warsptr->coord,&wptr->coord,warsptr->heading)+.5);
		heading  = (int) (cbearing(&wptr->coord,&warsptr->coord,wptr->heading)+.5);
		gheading = (int) (wptr->heading+.5);

		speed  = ((unsigned)(wptr->speed  +.5));
		prfmsg(SCAN01,wptr->shipname);
		prfmsg(DASHES);
		prfmsg(SCAN01A,shipclass[wptr->shpclass].typename);
		prfmsg(SCAN02,username(wptr));
		if (warusroff(shpnum)->teamcode >0)
			prfmsg(SCAN02A,teamname(wuptr));

		prfmsg(SCAN03,bearing,heading,spr("%ld",(long)ddistance));
		setsect(wptr);
		prfmsg(SCAN03A,gheading,xsect, ysect);
		prfmsg(SCAN04,showarp(wptr->speed));
		tons = (long)(shipclass[wptr->shpclass].max_tons);
		len  =tons/32L;
		wid  =tons/96L;
		
		prfmsg(SCAN04A,spr("%ld",len),spr("%ld",wid));

		if (warsptr->where != 1 && wptr->where != 1)
			{
			damage   = (unsigned) (wptr->damage+.5);
			damstr(damage);
			prfmsg(SCAN05,gechrbuf);
		
			if (wptr->shieldstat == SHIELDUP)
				prfmsg(SCAN06);
			else
				prfmsg(SCAN07);
		
			prfmsg(SCAN07A,wuptr->kills);
			}

		prfmsg(DASHES);
		outprfge(ALWAYS,usrnum);

		ltr = shpltr(shpnum,usrnum);
		/* if beyond the "scanned" ships range disply this msg */
		if ((long)ddistance > shipclass[wptr->shpclass].scanrange)
			{
			bearing = (int)(cbearing(&wptr->coord,&warsptr->coord,wptr->heading)+.5);
			prfmsg(SCAN2,bearing);
			}
		else
		/* if within his range but he has never scanned disply this msg */
		if (ltr == '?')
			{
			bearing = (int)(cbearing(&wptr->coord,&warsptr->coord,wptr->heading)+.5);
			prfmsg(SCAN3,bearing);
			}
		else
			{
			/* all else get this */
			prfmsg(SCAN1,ltr,warsptr->shipname);
			}
		outprfge(FILTER,shpnum);
		}
	else
		{
		prfmsg(NOSHIP);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	}
}

void FUNC scan_pl()

{
unsigned i;


/* SCAN PLANET FUNCTION */

if (margc != 3)
	{
	prfmsg(SCANFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}
	
plnum = atoi(margv[2]);
			
if (plnum <= MAXPLANETS && plnum > 0)
	{
	getplanetdat(usrnum);
	refresh(warsptr,usrnum);
	if (plnum > sector.numplan)
		{
		prfmsg(FOOLISH);
		outprfge(ALWAYS,usrnum);
		}
	else
	if (plptr->type == PLTYPE_PLNT)
		{
		bearing = (int)(cbearing(&warsptr->coord,&plptr->coord,warsptr->heading)+.5);
		ddistance = cdistance(&warsptr->coord,&plptr->coord)*10000;
		prfmsg(SCAN08,plnum,plptr->name);
		prfmsg(DASHES);

		if (plptr->userid[0] != 0)
			prfmsg(SCAN09,plptr->userid);

		prfmsg(SCAN10,bearing,spr("%ld",(long)ddistance));
/*		prfmsg(SCAN10A,plptr->type);*/

		if (warsptr->where != 1)
			{
			prfmsg(SCAN11);
			if (plptr->enviorn == 0)
				prfmsg(SCAN12);
			else
			if (plptr->enviorn == 1)
				prfmsg(SCAN13);
			else
			if (plptr->enviorn == 2)
				prfmsg(SCAN14);
			else
			if (plptr->enviorn == 3)
				prfmsg(SCAN15);
		
			prfmsg(SCAN16);
			if (plptr->resource == 0)
				prfmsg(SCAN12);
			else
			if (plptr->resource == 1)
				prfmsg(SCAN13);
			else
			if (plptr->resource == 2)
				prfmsg(SCAN14);
			else
			if (plptr->resource == 3)
				prfmsg(SCAN15);
			/*DEBUG
			prf("plptr->userid=%s\rwarsptr->userid=%s\r",plptr->userid,warsptr->userid);*/

			if (sameas(plptr->userid,warsptr->userid))
				{
				for (i=0; i<NUMITEMS; ++i)
					{
					if (plptr->items[i].qty > 0)
						{
						sprintf(gechrbuf,"%s%s%12ld",item_name[i],gedots(26-strlen(item_name[i])),plptr->items[i].qty);
						gechrbuf[0] = toupper(gechrbuf[0]);
						prf("%s\r",gechrbuf);
						}
					}
				}
			else
				{
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty == 0)
					strcpy(gechrbuf,"Not");
				else
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty < 2500)
					strcpy(gechrbuf,"Sparsly");
				else
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty < 10000)
					strcpy(gechrbuf,"Lightly");
				else
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty < 100000L)
					strcpy(gechrbuf,"Moderatly");
				else
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty < 1000000L)
					strcpy(gechrbuf,"Widely");
				else
					strcpy(gechrbuf,"Heavily");
				prfmsg(SCAN28,gechrbuf);
		
				if (plptr->items[I_MISSILE].qty == 0)
					strcpy(gechrbuf,"No");
				else
				if (plptr->items[I_MISSILE].qty < 25)
					strcpy(gechrbuf,"Small");
				else
				if (plptr->items[I_MISSILE].qty < 100)
					strcpy(gechrbuf,"Moderate");
				else
					strcpy(gechrbuf,"Large");
				prfmsg(SCAN29,gechrbuf);
		
				if (plptr->items[I_TORPEDO].qty == 0)
					strcpy(gechrbuf,"No");
				else
				if (plptr->items[I_TORPEDO].qty < 25)
					strcpy(gechrbuf,"Small");
				else
				if (plptr->items[I_TORPEDO].qty < 100)
					strcpy(gechrbuf,"Moderate");
				else
					strcpy(gechrbuf,"Large");
				prfmsg(SCAN30,gechrbuf);
		
				if (plptr->items[I_FLUXPOD].qty == 0)
					strcpy(gechrbuf,"No");
				else
				if (plptr->items[I_FLUXPOD].qty < 25)
					strcpy(gechrbuf,"Small");
				else
				if (plptr->items[I_FLUXPOD].qty < 100)
					strcpy(gechrbuf,"Moderate");
				else
					strcpy(gechrbuf,"Large");
				prfmsg(SCAN33,gechrbuf);
		
				if (plptr->items[I_FOOD].qty == 0)
					strcpy(gechrbuf,"No");
				else
				if (plptr->items[I_FOOD].qty < 25)
					strcpy(gechrbuf,"Small");
				else
				if (plptr->items[I_FOOD].qty < 100)
					strcpy(gechrbuf,"Moderate");
				else
					strcpy(gechrbuf,"Large");
				prfmsg(SCAN34,gechrbuf);
		
				if (plptr->items[I_FIGHTER].qty == 0)
					prfmsg(SCAN31);
				else
					prfmsg(SCAN32);
		
				}
			}
		prfmsg(DASHES);
		outprfge(ALWAYS,usrnum);
		}
	else
	if (plptr->type == PLTYPE_WORM)
		{
		memcpy(&worm,plptr,sizeof(GALWORM));
		bearing = (int)(cbearing(&warsptr->coord,&worm.coord,warsptr->heading)+.5);
		ddistance = cdistance(&warsptr->coord,&worm.coord)*10000;
		prfmsg(SCANWRM);
		if (worm.name[0] != 0)
			prfmsg(SCANWRM1,worm.name);
		prfmsg(DASHES);

		prfmsg(SCAN10,bearing,spr("%ld",(long)ddistance));
		prfmsg(DASHES);
		outprfge(ALWAYS,usrnum);
		}

		else
		{
		prfmsg(FOOLISH);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(FOOLISH);
	outprfge(ALWAYS,usrnum);
	}
}

void FUNC scan_ra()

{
int x,y;

double  xf,yf,x1,y1,range,xfactor,yfactor;


int             i;

WARSHP  *wptr;
MINE            *mptr;

if (margc != 3)
	{
	prfmsg(SCANFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

x = atoi(margv[2]);
if (x < 1 || x > 9)
	x = 1;

setsect(warsptr);

range = (double)((shipclass[warsptr->shpclass].scanrange)/((10-x)*(10-x)));

if (waruptr->options[SCANHOME])
	ansifunc(HOMEY);

prfmsg(SCAN24,spr("%ld",(long)range),xsect,ysect);

range = range/10000;

clearmap();

update_scantab(warsptr,usrnum);

x1 = (warsptr->coord.xcoord);
y1 = (warsptr->coord.ycoord);
range = range*2.0;
yfactor = range/(((double)MAXY)-1.0);
xfactor = range/(((double)MAXX)-1.0);

for (i=0,mptr = mines; i<nummines;++mptr,++i)
	{
	if (mptr->channel != 255)  /* if a live mine */
		{
		xf = (((double)MAXX)/2.0)+(((mptr->coord.xcoord) - x1)/xfactor);
		yf = (((double)MAXY)/2.0)+(((mptr->coord.ycoord) - y1)/yfactor);
/* DEBUG
		sprintf(gechrbuf,"xf = %f - yf = %f",xf,yf);
		prf("MINES FOUND @ %s",gechrbuf);*/
		if ((xf >= 0.0 && xf < (double)MAXX) 
			&& (yf >= 0.0 && yf < (double)MAXY))
			{
			y=(int)yf;
			x=(int)xf;
			map[y][x] = '.';
			}
		}
	}

for (i=0 ; i< NOSCANTAB; i++)
	{
	othusn = scantab[usrnum].ship[i].shipno;
	if (scantab[usrnum].ship[i].flag == 1)
		{
		wptr = warshpoff(othusn);
		xf = (wptr->coord.xcoord)- x1;
		yf = (wptr->coord.ycoord)- y1;
		xf = (((double)MAXX)/2.0)+(xf/xfactor);
		yf = (((double)MAXY)/2.0)+(yf/yfactor);

		if ((xf >= 0.0 && xf < (double)MAXX) 
			&& (yf>= 0.0 && yf < (double)MAXY))
			{
			x=(int)xf;
			y=(int)yf;
			map[y][x] = scantab[usrnum].ship[i].letter;
			}
		}
	}

map[MAXY/2][MAXX/2] = '*';

if (waruptr->options[SCANFULL])
	printmapfull();
else
	printmap();

outprfge(ALWAYS,usrnum);
}


void FUNC scan_se()

{
unsigned i,x,y;
WARSHP  *wptr;
MINE            *mptr;

if (waruptr->options[SCANHOME])
	ansifunc(HOMEY);

refresh(warsptr,usrnum);

setsect(warsptr);
prfmsg(SCAN25,xsect,ysect);
clearmap();

update_scantab(warsptr,usrnum);

for (i=0,mptr = mines; i<nummines;++mptr,++i)
	{
	x = coord1(mptr->coord.xcoord);
	y = coord1(mptr->coord.ycoord);

	if (mptr->channel != 255 && (x==xsect && y==ysect))  /* if a live mine */
		{
		x = coord2(mptr->coord.xcoord) +50;
		y = coord2(mptr->coord.ycoord) +50;
		map[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '.';
		}
	}

for (i=0 ; i< NOSCANTAB; i++)
	{
	othusn = scantab[usrnum].ship[i].shipno;
	if (scantab[usrnum].ship[i].flag == 1)
		{
		wptr = warshpoff(othusn);
		if (samesect(&wptr->coord,&warsptr->coord))
			{
			x = coord2(wptr->coord.xcoord) +50;
			y = coord2(wptr->coord.ycoord) +50;
			map[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = scantab[usrnum].ship[i].letter;
			if (wptr->status == GESTAT_AUTO)
				mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '1';
			else
				mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '2';
			}
		}
	}
x = coord2(warsptr->coord.xcoord) +50;
y = coord2(warsptr->coord.ycoord) +50;
map[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '*';
mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '2';
	
map_planets();
printmap();
outprfge(ALWAYS,usrnum);
}


void FUNC scan_lo()

{
int x,y;

double  xf,yf,x1,y1,range,xfactor,yfactor;

WARSHP  *wptr;

if (margc != 2)
	{
	prfmsg(SCANFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc == 4 )
	{
	if (!sameto("full",margv[3]))
		{
		prfmsg(SCANFMT);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}

setsect(warsptr);

range = (double)(shipclass[warsptr->shpclass].scanrange)*10.0;

if (waruptr->options[SCANHOME])
	ansifunc(HOMEY);

prfmsg(SCAN24,spr("%ld",(long)range),xsect,ysect);

range = (range/10000.0);
clearmap();

x1 = (warsptr->coord.xcoord);
y1 = (warsptr->coord.ycoord);
range = range*2.0;
xfactor = (range)/((double)MAXX-1.0);
yfactor = (range)/((double)MAXY-1.0);
/* debug 
prf("Xfactor=%s,Yfactor=%s\r",spr("%ld",(long)xfactor),spr("%ld",(long)yfactor));*/

for (othusn=0 ; othusn < nships ; othusn++)
	{
	if (ingegame(othusn))
		{
		wptr = warshpoff(othusn);
		xf = (wptr->coord.xcoord)- x1;
		yf = (wptr->coord.ycoord)- y1;
		xf = (xf/xfactor);
		yf = (yf/yfactor);
		xf+=(((double)MAXX)/2.0);
		yf+=(((double)MAXY)/2.0);


/*
		x = (((int)MAXX)/2D)+((int)(xf/xfactor));
		y = (((int)MAXY)/2D)+((int)(yf/yfactor));
*/

		if ((xf >= 0.0 && xf < (double)MAXX) && (yf>= 0.0 && yf < (double)MAXY))
			{
			x = (int)xf;
			y = (int)yf;
			if (wptr->status == GESTAT_AUTO)
				{
				map[y][x] = '+';
				mapc[y][x] = '2';
				}
			else
				{
				map[y][x] = '=';
				mapc[y][x] = '2';
				}
			}
		}
	}
map[MAXY/2][MAXX/2] = '*';

printmap();

outprfge(ALWAYS,usrnum);
}
#ifdef NOTHING
void scan_hy()

{
unsigned x,y;
WARSHP  *wptr;

double xfactor,yfactor,xf,yf;


if(warsptr->where != 1)
	{
	prfmsg(NOSCANL);
	}
else
	{
	setsect(warsptr);
	prfmsg(SCAN26,xsect,ysect);
	clearmap();
	xfactor = ((double)univmax*2.0)/((double)MAXX-1.0);
	yfactor = ((double)univmax*2.0)/((double)MAXY-1.0);
	for (othusn=0 ; othusn < nships ; othusn++)
		{
		wptr=warshpoff(othusn);
		if (ingegame(othusn))
/*              if (ingegame(othusn) && wptr->where == 1)*/
			{
			xf = ((wptr->coord.xcoord)+(double)univmax);
			yf = ((wptr->coord.ycoord)+(double)univmax);
			x = (unsigned)(xf/xfactor);
			y = (unsigned)(yf/yfactor);

			map[y][x] = '+';
			if (wptr->status == GESTAT_AUTO)
				mapc[y][x] = '1';
			else
				mapc[y][x] = '2';

			}
		}
	xf = ((warsptr->coord.xcoord)+(double)univmax);
	yf = ((warsptr->coord.ycoord)+(double)univmax);
	xfactor = ((double)univmax*2.0)/((double)MAXX-1.0);
	yfactor = ((double)univmax*2.0)/((double)MAXY-1.0);
	x = (unsigned)(xf/xfactor);
	y = (unsigned)(yf/yfactor);

	map[y][x] = '*';
	printmap();
	}
outprfge(ALWAYS,usrnum);
}
#endif


/* alternate idea: read off the ship letter & ship no to a seperate table.
   then zero out ship no as before */

void FUNC update_scantab(ptr, usrn)
WARSHP  *ptr;
int             usrn;
{
int     i,j;
WARSHP  *wptr;
SCANTAB tmp;

char	lettab[300];

/* clear the letter table 
for (i=0;i<300;++i)
	lettab[i]=0;
*/

setmem(&lettab[0],sizeof(char)*300,0);

/* save off the old letters */
for (i=0;i<NOSCANTAB;++i)
	{
	j = scantab[usrn].ship[i].shipno;
	if (j>= 0 && j < 300)
		{
		lettab[j] = scantab[usrn].ship[i].letter;
		}
	}

/* clear the table */
for (i=0;i<NOSCANTAB;++i)
	tmp.ship[i].flag = 0;

for (othusn=0 ; othusn < nships ; othusn++)
	{
	if (othusn != usrn && ingegame(othusn))
		{

		wptr=warshpoff(othusn);
		ddistance = cdistance(&ptr->coord,&wptr->coord)*10000;
		if (ddistance < shipclass[ptr->shpclass].scanrange 
			&& wptr->cloak < 10)
			{
			for (i=0;i<NOSCANTAB;++i)
				{
				/* entry blank - fill it with this one */
				if (tmp.ship[i].flag == 0)
					{
					tmp.ship[i].flag = 1;
					tmp.ship[i].shipno = othusn;
					tmp.ship[i].dist = ddistance;
					tmp.ship[i].letter = lettab[othusn];

					break;
					}
				/* is this ship closer */
				if (ddistance < tmp.ship[i].dist)
					{
					/* Yes - Push down the rest */
					for (j=NOSCANTAB-2;j>=i;j--)
						{
						if (tmp.ship[j].flag == 1)
							{
							memcpy(&tmp.ship[j+1],&tmp.ship[j],sizeof(SHIPTAB));
/* no longer needed
							tmp.ship[j+1].shipno = tmp.ship[j].shipno;
							tmp.ship[j+1].dist   = tmp.ship[j].dist;
							tmp.ship[j+1].letter = tmp.ship[j].letter;
*/
							}
						}
					/* fill the hole with the new ship */
					tmp.ship[i].flag = 1;
					tmp.ship[i].shipno = othusn;
					tmp.ship[i].dist = ddistance;
					tmp.ship[i].letter = lettab[othusn];
					break;
					}
				}
			}
		}
	}

/* go fill in the new ships without letters */

pick_letter(&tmp);

/* Now go fill in the rest of the table */

for (i=0;i<NOSCANTAB;++i)
	{
	/* clear out the empty entries */
	if (tmp.ship[i].flag == 0)
		{
		tmp.ship[i].letter = '?';
		tmp.ship[i].shipno = -1;
		}
	else
		{
		j = tmp.ship[i].shipno;
		wptr=warshpoff(j);
		tmp.ship[i].bearing = (int)(cbearing(&ptr->coord,&(wptr->coord),ptr->heading)+.5);
		tmp.ship[i].heading  = (int) (cbearing(&(wptr->coord),&ptr->coord,wptr->heading)+.5);
		tmp.ship[i].speed  = wptr->speed;
		}
	}

/* update the current users master record */
memcpy(&scantab[usrn],&tmp,sizeof(SCANTAB));

}

void FUNC pick_letter(ptr)
SCANTAB *ptr;
{
#define LETSIZE 26
char letters[LETSIZE] = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
                         'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char lettmp[LETSIZE];
int i,j;


/* init the temp tab */
memcpy(lettmp,letters,LETSIZE);

/* look at each ship in the table and punch out the letter from the
   list... when all done the letters remaining are available */

/* DEBUG - REMOVE THIS WHEN DONE 
for (i=0;i<NOSCANTAB;++i)
	logthis(spr("PLTR-A:%d flg=%d shipno=%d letter=%d/%c",i,ptr->ship[i].flag,ptr->ship[i].shipno,ptr->ship[i].letter,ptr->ship[i].letter));*/

for (i=0;i<NOSCANTAB;++i)
	{
	if (ptr->ship[i].flag == 1 &&  ptr->ship[i].letter != 0)
		{
		for (j=0;j<LETSIZE;++j)
			{
			if (ptr->ship[i].letter == lettmp[j])
				{
				lettmp[j] = '@';
				break;
				}
			}
		}
	}

/* now go through and fix up the ? */

for (i=0;i<NOSCANTAB;++i)
	{
	if (ptr->ship[i].flag == 1 &&  ptr->ship[i].letter == 0)
		{
		for (j=0;j<LETSIZE;++j)
			{
			if (lettmp[j] != '@')
				{
				ptr->ship[i].letter = lettmp[j];
				lettmp[j] = '@';
				break;
				}
			}
		}
	}
/* DEBUG - REMOVE THIS WHEN DONE 
for (i=0;i<NOSCANTAB;++i)
	logthis(spr("PLTR-Z:%d flg=%d shipno=%d letter=%d/%c",i,ptr->ship[i].flag,ptr->ship[i].shipno,ptr->ship[i].letter,ptr->ship[i].letter));*/

return;
}

void FUNC map_planets()
{
int i;
unsigned x,y;
getsector(&warsptr->coord);
for (i=0;i < sector.numplan;++i)
	{
	if (sector.ptab[i].coord.xcoord != 0)
		{
		x = coord2(sector.ptab[i].coord.xcoord)+25;
		y = coord2(sector.ptab[i].coord.ycoord)+25;
		map[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '1' + i;
		}
	}
}

void FUNC clearmap()
{
int i,j;

for (i=0; i < MAXY; ++i)
	{
	for (j=0; j < MAXX; ++j)
		{
		map[i][j]=' ';
		mapc[i][j]=' ';
		}
	map[i][MAXX] = 0;
	mapc[i][MAXX] = 0;
	}
}


void FUNC printmap()

{
int i,j;
prfmsg(PLUSDASH);
for (i=0; i<MAXY; ++i)
	{
	prf("   |");
	for (j=0; j<MAXX; ++j)
		{
		if (mapc[i][j] == '1')
			{
			prf(".[31m%c.[37m",map[i][j]);
			}
		else
		if (mapc[i][j] == '2')
			{
			prf(".[33m%c.[37m",map[i][j]);
			}
		else
			{
			prf("%c",map[i][j]);
			}
		}
	prf("|\r");
	}

prfmsg(PLUSDASH);

}


void FUNC printmapfull()

{
SCANTAB *sptr;

int othusn;

int i,j;
int shp;
int ff;

ff		=	0;
shp	=	0;

sptr = &scantab[usrnum];

prfmsg(PLUSFULL);
for (i=0; i<MAXY; ++i)
	{
	prf("   |");
	for (j=0; j<MAXX; ++j)
		{
		if (mapc[i][j] == '1')
			{
			prf(".[31m%c.[37m",map[i][j]);
			}
		else
		if (mapc[i][j] == '2')
			{
			prf(".[33m%c.[37m",map[i][j]);
			}
		else
			{
			prf("%c",map[i][j]);
			}
		}
	prf("| ");
	if (sptr->ship[shp].flag == 1)
		{
		othusn = sptr->ship[shp].shipno;
		if (ff == 0)
			{
			prf("  %c  %s   %4d    %4d    %s\r",sptr->ship[shp].letter,spr("%6ld",(long)(sptr->ship[shp].dist)),
				sptr->ship[shp].bearing,sptr->ship[shp].heading,showarp(sptr->ship[shp].speed));

			if (!waruptr->options[SCANNAMES])
				shp++;
			else
				ff = 1;

			}
		else
			{
			prf("     %s\r",username(warshpoff(othusn)));
			shp++;
			ff = 0;
			}
		}
	else
		{
		prf("                                        \r");
		}
	}

prfmsg(PLUSDASH);

}


unsigned FUNC coord2(dcoord)
double dcoord;
{
double d1,d2;
int     d3;

d2 =modf(1+modf(dcoord, &d1),&d1);
d3 = (d2 * SSMAX);

return ((unsigned)d3);

}


int FUNC coord1(dcoord)
double dcoord;
{

return ((int)floor(dcoord));

}

/**************************************************************************
** Take the shields up or down                                           **
**************************************************************************/

void  FUNC cmd_shields()

{

if (shipclass[warsptr->shpclass].max_shlds == 0)
	{
	prfmsg(SHIELD0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(SHLD1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->shieldtype == 0)
	{
	prfmsg(SHLD2);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc != 2)
	{
	prfmsg(SHLDFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (sameas(margv[1],"up"))
	{
	if (warsptr->energy > SHMINPWR)
		{
		if (warsptr->shieldstat == SHIELDDM)
			{
			prfmsg(SHNORPR);
			outprfge(ALWAYS,usrnum);
			return;
			}
		else
			{
			shieldup(warsptr,usrnum);
			return;
			}
		}
	else
		{
		prfmsg(SHNOPWR);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}
else
if (sameas(margv[1],"down"))
	{
	shielddn(warsptr,usrnum);
	return;
	}
else
	{
	prfmsg(SHLDFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}
}


/**************************************************************************
** Turn cloaking on and off                                              **
**************************************************************************/

void  FUNC cmd_cloak()

{

if (shipclass[warsptr->shpclass].max_cloak == 0)
	{
	prfmsg(CLOK01);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc != 2)
	{
	prfmsg(CLOFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}


if (warsptr->where == 1)
	{
	prfmsg(CLOK1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (sameas(margv[1],"on"))
	{
	if (warsptr->cloak > 0)
		{
		prfmsg(CLOKCOM);
		outprfge(FILTER,usrnum);
		}
	else
	if (warsptr->cloak == 0)
		{
		if (warsptr->energy > clenguse)
			{
			warsptr->cloak = 1;
			prfmsg(CLOKON);
			outprfge(FILTER,usrnum);
			}
		else
			{
			prfmsg(CLOKPWR);
			outprfge(ALWAYS,usrnum);
			}
		}
	else
	if (warsptr->cloak < 0)
		{
		prfmsg(CLOKDAM);
		outprfge(ALWAYS,usrnum);
		}
	return;
	}
else
if (sameas(margv[1],"off"))
	{
	if (warsptr->cloak <= 0)
		{
		prfmsg(CLOKDWN);
		outprfge(FILTER,usrnum);
		}
	else
	if (warsptr->cloak > 0)
		{
		warsptr->cloak = 0;
		prfmsg(CLOKOFF);
		outprfge(FILTER,usrnum);
		prfmsg(CLOK2);
		outrange(FILTER,&warsptr->coord);
		}
	return;
	}
prfmsg(CLOFMT);
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Transfer goods down                                                   **
**************************************************************************/

void  FUNC cmd_transfer()
{

int i;

if (warsptr->where < 10)
	{
	prfmsg(TRANSFR3);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc == 4)
	{
	for (i=0; i < NUMITEMS; ++i)
		{
		if (genearas(kwrd[i],margv[3]))
			{
			if (sameas("up",margv[1]))
				trans_up(i);
			else
			if (sameas("down",margv[1]))
				trans_down(i);
			else
				prfmsg(TRANSFMT);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	}

prfmsg(TRANSFMT);
outprfge(ALWAYS,usrnum);
}

void trans_down(item)
int     item;

{
unsigned long amt;

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (plptr->type == PLTYPE_WORM)
	{
	prfmsg(TRANSFR0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (trans_opt || sameas(plptr->userid,warsptr->userid))
	{
	if ((amt = atol(margv[2])) > 0L)
		{
		if (warsptr->items[item] >= amt)
			{
			warsptr->items[item] -= amt;
			plptr->items[item].qty += amt;
			sprintf(gechrbuf,"%ld",amt);
			prfmsg(TRANSFR5,gechrbuf,item_name[item]);
			setsect(warsptr); /* build PKEY */
			pkey.plnum = plnum;
			gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
			gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
			return;
			}
		else
			{
			prfmsg(TRANSFR1);
			}
		}
	else
		{
		prfmsg(TRANSFR2);
		}
	}
else
	{
	prfmsg(TRANSFR4);
	}
}

void trans_up(item)
int     item;

{
unsigned long amt;

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (plptr->type == PLTYPE_WORM)
	{
	prfmsg(TRANSFR0);
	outprfge(ALWAYS,usrnum);
	return;
	}

/* you must own this planet or NOBODY must own it to xfer up */

if (sameas(plptr->userid,warsptr->userid) || plptr->userid[0] == 0)
	{
	if ((amt = atol(margv[2])) > 0L)
		{
		if (chkweight(warsptr,item,amt))
			{
			if (plptr->items[item].qty >= amt)
				{
				plptr->items[item].qty -= amt;
				warsptr->items[item] += amt;
				sprintf(gechrbuf,"%ld",amt);
				prfmsg(TRANSUP5,gechrbuf,item_name[item]);
				setsect(warsptr); /* load PKEY */
				pkey.plnum = plnum;
				gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
				gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
				return;
				}
			else
				{
				prfmsg(TRANSUP1);
				}
			}
		else
			{
			prfmsg(TRANSUP6);
			}
		}
	else
		{
		prfmsg(TRANSUP2);
		}
	}
else
	{
	prfmsg(TRANSUP4);
	}
}




/**************************************************************************
** abandon a colony                                                      **
**************************************************************************/

void  FUNC cmd_abandon()

{

if (warsptr->where < 10)
	{
	prfmsg(ABAN01);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (sameas(plptr->userid,warsptr->userid))
	{
	plptr->userid[0] = 0;
	if(--waruptr->planets <0)
		waruptr->planets = 0;

	setsect(warsptr); /* build PKEY */
	pkey.plnum = plnum;
	gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
	
	prfmsg(ABAN02);
	outprfge(ALWAYS,usrnum);

	}
else
	{
	prfmsg(ABAN03);
	outprfge(ALWAYS,usrnum);
	}
}
	


/**************************************************************************
** establish a colony or administer it                                   **
**************************************************************************/

void  FUNC cmd_admin()

{

if (warsptr->where < 10)
	{
	prfmsg(ADMIN1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (plptr->type == PLTYPE_WORM)
	{
	prfmsg(ADMIN2A);
	outprfge(ALWAYS,usrnum);
	return;
	}


if (plptr->userid[0] == 0)
	{
	if (waruptr->planets >= max_plnts)
		{
		prfmsg(ADMIN4);
		outprfge(ALWAYS,usrnum);
		return;
		}
	prfmsg(ADMENU1);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMENU1;
	}
else
if (sameas(plptr->userid,warsptr->userid))
	{
	prfmsg(ADMENU2);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMENU2;
	}
else
	{
	prfmsg(ADMIN2);
	outprfge(ALWAYS,usrnum);
	}
}

/**************************************************************************
** Attack Command                                                        **
**************************************************************************/

void  FUNC cmd_attack()

{

int won;
unsigned long num;

if (shipclass[warsptr->shpclass].max_attk == 0)
	{
	prfmsg(ATTACK0A);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where < 10)
	{
	prfmsg(ATTACK1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (plptr->type == PLTYPE_WORM)
	{
	prfmsg(ATTACK0B);
	outprfge(ALWAYS,usrnum);
	return;
	}


if (sameas(plptr->userid,warsptr->userid))
	{
	prfmsg(ATTACK0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (neutral(&warsptr->coord))
	{
	zaphim(warsptr,usrnum);
	return;
	}

if (margc == 3)
	{
	if (genearas("tro",margv[2]))
		{
		num = atol(margv[1]);
		if (num > 0 && num <= warsptr->items[I_TROOPS])
			{
			warsptr->hostile = warsptr->where;
			warsptr->cantexit = FIRETICKS;
			won = attack_men(num);
			if (won == 1)
				{
				prfmsg(ATTACK8);
				outprfge(ALWAYS,usrnum);
				}
			else
				{
				prfmsg(ATTACK9);
				outprfge(ALWAYS,usrnum);
				}
			return;
			}
		else
			{
			prfmsg(ATTACKM0);
			}
		}
	else
	if (genearas("fig",margv[2]))
		{
		num = atol(margv[1]);
		if (num > 0 && num <= warsptr->items[I_FIGHTER])
			{
			warsptr->hostile = warsptr->where;
			warsptr->cantexit = FIRETICKS;
			won = attack_fig(num);
			if (won == 1)
				{
				prfmsg(ATTACK8);
				outprfge(ALWAYS,usrnum);
				}
			else
				{
				prfmsg(ATTACK9);
				outprfge(ALWAYS,usrnum);
				}
			return;
			}
		else
			{
			prfmsg(ATTACKF0);
			}
		}
	prfmsg(ATTFMT);
	}
else
	{
	prfmsg(ATTFMT);
	}
outprfge(ALWAYS,usrnum);
}

int FUNC attack_men(num)
unsigned long num;
{

double r;
int won = 0;
int	ii;
unsigned long 	i, 
					j, 
					left1,
					left2,
					kill1,
					kill2,
					ratio;

/* take troops off ship */
warsptr->items[I_TROOPS] -= num;
sprintf(gechrbuf,"%ld",num);

/* tell him there gone*/
prfmsg(ATTACKM1,gechrbuf);
outprfge(ALWAYS,usrnum);

left1 = num;
left2 = plptr->items[I_TROOPS].qty;

kill1 = 0;
kill2 = 0;

/* figure out the proportion of this attack*/

if (left2 > 0)
	ratio = (left1*100UL)/left2;
else
	ratio = 0;

if (plptr->items[I_FIGHTER].qty > 1)
	{
	prfmsg(ATTACKM7);
	i = ((unsigned long)(gernd()%35))+9L;
	kill1 =  i * (plptr->items[I_FIGHTER].qty);
	if (kill1 > left1)
		kill1 = left1;
	sprintf(gechrbuf,"%ld",kill1);
	prfmsg(ATTACKM8,gechrbuf);
	}

/* this specifies the number of troops killed by ground troops */
r = rndm(plattrt1)+.25;  /*.766*/
kill1 += (unsigned long)((double)left2 * r);

if (ratio > 2L)
	{
	r = rndm(plattrt2)+.1;   /* .344 */
	kill2 = (unsigned long)((double)left1 * r);
	}
else
	{
	kill2 = 0L;
	}

if (kill1 > left1)
	kill1 = left1;

if (kill2 > left2)
	kill2 = left2;

sprintf(gechrbuf,"%ld",kill1);
sprintf(gechrbuf2,"%ld",kill2);

prfmsg(ATTACKM2,gechrbuf2,gechrbuf);

left1 -= kill1;
left2 -= kill2;

if (left2 > 0 && left2 < (left1/4))
	{
	sprintf(gechrbuf,"%ld",left2);
	prfmsg(ATTACKM3,gechrbuf);
	won = 1;
	}

if (left1 > 0 && left1 < (left2/4))
	{
	sprintf(gechrbuf,"%ld",left1);
	prfmsg(ATTACKM4,gechrbuf);
	plptr->items[I_TROOPS].qty += left1;
	left1 = 0;
	}

/* randomly trash a few of the planets items */

if (ratio > 2 && left1 > (left2/2))
	{
	for(ii=1;ii<NUMITEMS;++ii)
		{
		j = (unsigned long)gernd()%15;

		if (j > plptr->items[ii].qty)
			j = plptr->items[ii].qty;

		if (j >= 1)
			{
			sprintf(gechrbuf,"%ld",j);
			prfmsg(ATTACKM5,gechrbuf,item_name[ii]);
			plptr->items[ii].qty -= j;
			}
		}
	}

if (left1 > 0)
	{
	warsptr->items[I_TROOPS] += left1;
	sprintf(gechrbuf,"%lu",left1);
	prfmsg(ATTACKM6,gechrbuf);
	}

if (left1 > 0L && (left2 <= 0L && plptr->items[(int)I_FIGHTER].qty <= 0L))
	{
	won = 1;
	}

plptr->items[I_TROOPS].qty = left2;

outprfge(ALWAYS,usrnum);
clrprf();

/* inform the player if he is not in game */

if (ratio > 5L) /* big enough to let spy report on it */
	call_4_help(TRUE,won);
else
if (ratio > 1L)
	call_4_help(FALSE,won);
	

/* dont mail him unless its significant*/
if (ratio > 1L)
	{
	mail.type = MESG02;
	strncpy(mail.userid,plptr->userid,UIDSIZ);

	if (won == 1)
		{
		wonplnt();
		mail.type = MESG03;
		}

	strcpy(mail.name1,plptr->name);
	mail.class = MAIL_CLASS_DISTRESS;
	mail.int1 = sector.xsect;
	mail.int2 = sector.ysect;
	mail.long1 = num;
	sprintf(mail.name2,"%s",warsptr->shipname);
	sprintf(mail.string1,"%s",warsptr->userid);
	
	mailit(1);
	}

setsect(warsptr);
pkey.plnum = plnum;
gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
return(won);
}

int FUNC attack_fig(num)
unsigned long num;
{

double r;
int won = 0;
unsigned long 	j, 
					left1,
					left2,
					kill1,
					kill2,
					ratio;


float	fl1,fl2,fl3;


int ii;

warsptr->items[I_FIGHTER] -= num;
sprintf(gechrbuf,"%ld",num);
prfmsg(ATTACKF1,gechrbuf);
outprfge(ALWAYS,usrnum);

left1 = num;
left2 = plptr->items[I_FIGHTER].qty;

kill1 = 0;
kill2 = 0;

/* figure out the proportion of this attack*/

/* there is a bug here */

if (left2 > 0)
	{
	fl1 = left1;
	fl2 = left2;
	fl3 = (fl1/fl2)*100.0;
	ratio = (unsigned long)fl3;
	}
else
	ratio = 0;


/* If there are more than 500 troops - shoot down some fighters */
if (left1 > 0 && plptr->items[I_TROOPS].qty > 500 && (gernd()%5-1) > 0)
	{
	prfmsg(ATTACKF8);
	r = rndm(plattrf1)+.05; /*.188) */
	kill1 += (unsigned long)((double)left1 * r);
	}

/* if there are fighters on the planet then they will shoot down some of 
   the attackers */

if (left2 > 0L)
	{
	/* this specifies the planet fighters kill ratio - 
	   at least 20% and as much as 120.0% */
	r = rndm(plattrf2)+.2;  /* 1.0 */
	kill1 += (unsigned long)((double)left2 * r);

	/* if the ratio of attacker to attackee is at least 1 to 2 (2%)
	   then kill off some of the planets fighters - this makes it harder
		for the little marauders to eat away at your fighters */

	if (ratio > 1)
		{
		r = rndm(plattrf3)+.2;  /* .551 */
		kill2 = (unsigned long)((double)left1 * r);
		}
	else
		{
		kill2 = 0L;
		}

	if (kill1 > left1)
		kill1 = left1;

	if (kill2 > left2)
		kill2 = left2;

	sprintf(gechrbuf,"%ld",kill2);
	sprintf(gechrbuf2,"%ld",kill1);

	prfmsg(ATTACKF2,gechrbuf,gechrbuf2);
	left1 -= kill1;
	left2 -= kill2;
	}

/* must be at least 5% to do damage to items on the planet */

if (ratio > 5)
	{
	for(ii=0;ii<NUMITEMS;++ii)
		{
		j = (unsigned long)gernd()%15;

		if (j > plptr->items[ii].qty)
			j = plptr->items[ii].qty;

		if (j > 0 && ii != I_FIGHTER)
			{
			sprintf(gechrbuf,"%ld",j);
			prfmsg(ATTACKF5,gechrbuf,item_name[ii]);
			plptr->items[ii].qty -= j;
			}
		}
	}

if (left1 > 0L && left2 <= 0L && plptr->items[I_TROOPS].qty < 5L)
	{
	won = 1;
	prfmsg(ATTACKF7);
	}

if (left1 > 0L)
	{
	warsptr->items[I_FIGHTER] +=left1;
	sprintf(gechrbuf,"%ld",left1);
	prfmsg(ATTACKF6,gechrbuf);
	}

outprfge(ALWAYS,usrnum);
clrprf();

if (ratio > 5) /* big enough to let spy report on it */
	call_4_help(TRUE,won);
else
if (ratio > 1)
	call_4_help(FALSE,won);
	
plptr->items[I_FIGHTER].qty = left2;

if (ratio > 2 || won == 1)
	{
	mail.class = MAIL_CLASS_DISTRESS;
	mail.type = MESG04;
	strncpy(mail.userid,plptr->userid,UIDSIZ);

	if (won == 1)
		{
		wonplnt();
		mail.type = MESG05;
		}

	strcpy(mail.name1,plptr->name);
	mail.class = MAIL_CLASS_DISTRESS;
	mail.int1 = sector.xsect;
	mail.int2 = sector.ysect;
	mail.long1 = num;
	sprintf(mail.name2,"%s",warsptr->shipname);
	sprintf(mail.string1,"%s",warsptr->userid);

	mailit(1);
	}

setsect(warsptr);
pkey.plnum = plnum;
gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
return (won);
}

void FUNC call_4_help(int	send_spy_mail, int won)

{
if (instat(plptr->userid,gestt) && othusp->substt >= FIGHTSUB)
	{
	prfmsg(ATTACK6,plptr->name,xsect,ysect,warsptr->userid,warsptr->shipname);
	outprf(othusn);
	prfmsg(ATTACK7);
	outprfge(ALWAYS,usrnum);
	clrprf();
	}
else
if (onsys(plptr->userid) && user[othusn].state != fse_state)
	{
	prfmsg(ATTACK6A);
	injoth();
	prfmsg(ATTACK7);
	outprfge(ALWAYS,usrnum);
	clrprf();
	}
else
if (won == 0
	&& send_spy_mail 
	&& gernd()%6 == 0 
	&& plptr->spyowner[0] != 0)
	{
	prfmsg(SPYM3,plptr->name,xsect,ysect,warsptr->userid);
	strcpy(mail.userid,plptr->spyowner);
	strcpy(mail.topic,"Intelligence Report");
	sendit();
	clrprf();
	}
else
if (won == 1 
	&& plptr->spyowner[0] != 0)
	{
	prfmsg(SPYM4,plptr->name,xsect,ysect,warsptr->userid);
	strcpy(mail.userid,plptr->spyowner);
	strcpy(mail.topic,"Intelligence Report");
	sendit();
	clrprf();
	}
}

void FUNC wonplnt()

{
strncpy(plptr->userid,warsptr->userid,UIDSIZ);
warsptr->hostile = 0;
++waruptr->planets;
}

/**************************************************************************
** Roster Command                                                        **
**************************************************************************/

void  FUNC cmd_geroster()

{

char	tmpbuf2[2];

int i = 0;
int j;

setbtv(gebb5);

strncpy(tmpbuf2,KEY,1);

j = gemaxlist;

if (margc == 2 && sameas(margv[1],"all"))
	j = 200;
	
if (qhibtv(1))
	{
	if (usaptr->scnwid <60)
		prfmsg(ROSTER1,gemaxlist);
	else
		prfmsg(ROSTER2,gemaxlist);
		
	do
		{
		gcrbtv(&tmpusr,1);
		logthis(spr("ROS:Got %s Score %ld",tmpusr.userid,tmpusr.score));
		if (tmpusr.score > 0
			&& tmpusr.userid[0] != tmpbuf2[0]
			&& tmpusr.userid[0] != '@')
			{
			++i;
			sprintf(gechrbuf,"%11ld",tmpusr.score);
			sprintf(gechrbuf2," %8.3fm",((float)tmpusr.population)/100.0);
			if (usaptr->scnwid <60)
				prf("%-30s\r          %s%5d%3d%s\r",tmpusr.userid,gechrbuf,tmpusr.kills,tmpusr.planets,gechrbuf2);
			else
				prf("%-30s%s%5d%3d%s\r",tmpusr.userid,gechrbuf,tmpusr.kills,tmpusr.planets,gechrbuf2);

			outprfge(ALWAYS,usrnum);
			}
		} while (qprbtv() && i < j);
	}
else
	{
	logthis("No one in roster yet");
	}
}

/**************************************************************************
** Planet command                                                        **
**************************************************************************/

void  FUNC cmd_planet()

{
setbtv(gebb2);

if (qlobtv(0))
	{
	if (qeqbtv(warsptr->userid,1))
		{
		prfmsg(PLAMSG1);
		do
			{
			gcrbtv(&planet,1);
			if (sameas(planet.userid,warsptr->userid))
				{
				prf("%-20s %5d %5d  %d \r",planet.name,planet.xsect,planet.ysect,planet.plnum);
				outprfge(ALWAYS,usrnum);
				}
			else
				break;
			} while (qnxbtv());
		}
	else
		{
		prfmsg(PLAMSG2);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(PLAMSG2);
	outprfge(ALWAYS,usrnum);
	}
}


/**************************************************************************
** Sell goods                                                             **
**************************************************************************/

void  FUNC cmd_sell()
{

int i;

if (warsptr->where < 10)
	{
	prfmsg(SELL1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (!neutral(&warsptr->coord))
	{
	prfmsg(SELL1);
	outprfge(ALWAYS,usrnum);
	return;
	}


plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (neutral(&warsptr->coord) && plnum == 1) /*must be Zygor-3*/
	{
	if (margc == 3)
		{
		for (i=0; i < NUMITEMS; ++i)
			{
			if (genearas(kwrd[i],margv[2]))
				{
				sell(i);
				outprfge(ALWAYS,usrnum);
				return;
				}
			}
		}
	}

prfmsg(SELLFMT);
outprfge(ALWAYS,usrnum);
}

void FUNC sell(item)
int     item;

{
unsigned long amt;
long	doll,fee;

if ((amt = atol(margv[1])) > 0L)
	{
	if (warsptr->items[item] >= amt)
		{
		doll = (long)baseprice[item]*amt;
		
		fee = 1L + (doll/1000L);

		warsptr->items[item] -= amt;
		/* if there is more tax than profit then tax = profit */
		if ((doll-fee) < 0)
			fee = doll;

		waruptr->cash += (doll-fee);
		/* this will fix those who go negative */
		if ((long)waruptr->cash < 0)
			waruptr->cash = 0;

		gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
		geudb(GEUPDATE,waruptr->userid,waruptr);

		sprintf(gechrbuf,"%ld",amt);
		sprintf(gechrbuf2,"%ld",(doll-fee));
		sprintf(gechrbuf3,"%ld",fee);

		prfmsg(SELL2,gechrbuf3,gechrbuf2,gechrbuf,item_name[item]);

		return;
		}
	else
		{
		prfmsg(SELL3,item_name[item]);
		}
	}
else
	{
	prfmsg(SELLFMT);
	}
}




/**************************************************************************
** Buy goods                                                             **
**************************************************************************/

void  FUNC cmd_buy()
{

int i;


/* this will fix those who go negative */
if ((long)waruptr->cash < 0)
	waruptr->cash = 0;


if (warsptr->where < 10)
	{
	prfmsg(BUY1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

/* two problems:
   1 - trading player has no team affiliation and planet was designated 
	    TEAM while the owner had no team affiliation...hence they both are
		 on the same team. Need to find out what the teamcode is when no team
		 is set... 0 I suspect.
	2 - players can just specify the password "team" and trade anyway
*/


if (sameas(plptr->password,"team") 
	&& plptr->teamcode > 0
	&& plptr->teamcode != waruptr->teamcode)
	{
	prfmsg(BUYPAS3);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas(plptr->password,"team") 
	&& plptr->teamcode > 0
	&& plptr->teamcode == waruptr->teamcode)
	{
	prfmsg(BUYPAS4);
	outprfge(ALWAYS,usrnum);
	}
else
if (!sameas(plptr->password,"none") && margc > 1 && margc < 4)
	{
	prfmsg(BUYPAS1);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (!sameas(plptr->password,"none")
	&& !sameas(plptr->password,margv[3]))
	{
	prfmsg(BUYPAS2);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc > 2)
	{
	for (i=0; i < NUMITEMS; ++i)
		{
		if (genearas(kwrd[i],margv[2]))
			{
			buy(i);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	}
prfmsg(BUYFMT);
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Price goods                                                           **
**************************************************************************/

void  FUNC cmd_price()
{

int i;

if (warsptr->where < 10)
	{
	prfmsg(BUY1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (margc == 3)
	{
	for (i=0; i < NUMITEMS; ++i)
		{
		if (genearas(kwrd[i],margv[2]))
			{
			buy(i);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	}
prfmsg(PRICEFMT);
outprfge(ALWAYS,usrnum);
}

void FUNC buy(item)
int item;
{
long amt,avail;
long tot;

if (plptr->userid[0] != 0)
	{
	if ((amt = atol(margv[1])) > 0)
		{
		if (sameas(plptr->userid,warsptr->userid) || plptr->items[item].sell == 'Y')
			{
			if (chkweight(warsptr,item,amt))
				{
				avail = amt4sale(item);
				if (avail > 0 && avail >= amt)
					{
					if ((tot = price(item,amt)) <= waruptr->cash)
						{
						if (sameas(margv[0],"buy"))
							{
							if (!neutral(&warsptr->coord))
								{
								plptr->items[item].qty -= amt;
								plptr->items[item].sold2a += amt;
								plptr->cash +=tot;
								setsect(warsptr);
								pkey.plnum = plnum;
								gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
								}
							warsptr->items[item] += amt;
							waruptr->cash -= tot;
							sprintf(gechrbuf,"%ld",tot);

							if (sameas(plptr->userid, warsptr->userid))
								{
								sprintf(gechrbuf2,"%ld",amt);
								prfmsg(BUY9,gechrbuf2,item_name[item],baseprice[item],gechrbuf);
								}
							else
								{
								sprintf(gechrbuf2,"%ld",amt);
/*								prfmsg(BUY9,gechrbuf2,item_name[item],baseprice[item]+plptr->items[item].markup2a,gechrbuf);*/
								prfmsg(BUY9,gechrbuf2,item_name[item],plptr->items[item].markup2a,gechrbuf);
								}
							return;
							}
						else
							{
							sprintf(gechrbuf,"%ld",tot);
							sprintf(gechrbuf2,"%ld",amt);
/*							prfmsg(PRICE1,gechrbuf2,item_name[item],baseprice[item]+plptr->items[item].markup2a,gechrbuf);*/
							prfmsg(PRICE1,gechrbuf2,item_name[item],plptr->items[item].markup2a,gechrbuf);
							}
						}
					else
						{
						sprintf(gechrbuf,"%ld",tot);
						sprintf(gechrbuf2,"%ld",amt);
						prfmsg(BUY2,gechrbuf,gechrbuf2,item_name[item]);
						}
					}
				else
					{
					sprintf(gechrbuf,"%ld",avail);
					prfmsg(BUY3,gechrbuf,item_name[item]);
					}
				}
			else
				{
				prfmsg(BUY8);
				}
			}
		else
			{
			prfmsg(BUY4,item_name[item]);
			}
		}
	else
		{
		prfmsg(BUY5);
		}
	}
else
	{
	prfmsg(BUY7);
	}
}

long FUNC amt4sale(item)
int item;

{
long forsale = 0;

if (sameas(plptr->userid, warsptr->userid))
	forsale = plptr->items[item].qty;
else
	if (plptr->items[item].qty > (long)plptr->items[item].reserve && plptr->items[item].sell == 'Y')
		forsale = plptr->items[item].qty - (long)plptr->items[item].reserve;

if (item == I_GOLD)
	{
	plnum = warsptr->where - 10;
	getplanetdat(usrnum);
	if (neutral(&warsptr->coord) && plnum == 1)
		{
		forsale = waruptr->cash;
		}
	}

return (forsale);
}

long FUNC price(item,amt)
unsigned item;
unsigned long amt;

{
long    tot;

if (sameas(plptr->userid, warsptr->userid))
	tot = ((long)baseprice[item])*amt;
else
/*	tot = ((long)baseprice[item] + (long)plptr->items[item].markup2a)*amt;*/
	tot = ((long)plptr->items[item].markup2a)*amt;

return(tot);
}



/**************************************************************************
** Maintenance and repair                                                **
**************************************************************************/

void  FUNC cmd_maint()
{

unsigned price;

if (warsptr->where < 10)
	{
	prfmsg(MAINT1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;


getplanetdat(usrnum);

if (!sameas(plptr->password,"none")     && margc < 2)
	{
	prfmsg(MAINT2);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (!sameas(plptr->password,"none")
	&& !sameas(plptr->password,margv[1]))
	{
	prfmsg(MAINT3);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (plptr->userid[0] == 0 || plptr->items[I_MEN].qty < 25000L)
	{
	prfmsg(MAINT8);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->cantexit > 0)
	{
	prfmsg(MAINT9);
	outprfge(ALWAYS,usrnum);
	return;
	}

price = 200;

if (neutral(&warsptr->coord))
	{
	if (plnum == 1 || plnum == 2)
		{
		price = 2500;
		}
	else
		{
		prfmsg(MAINT4);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}

if (waruptr->cash >= price)
	{
	warsptr->repair = (unsigned)(warsptr->damage/3.0)+1;
	waruptr->cash -= price;
	prfmsg(MAINT5,warsptr->repair);
	}
else
	{
	prfmsg(MAINT6);
	}

outprfge(ALWAYS,usrnum);

}


/**************************************************************************
** New ship or goods command                                                      **
**************************************************************************/

void  FUNC cmd_new()
{

int     type,ctype;
long	delta,credit,fee;

if (margc != 3)
	{
	prfmsg(NEWFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where < 10)
	{
	prfmsg(NEW1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (neutral(&warsptr->coord) && plnum == 1) /*must be Zygor-3*/
	{
	if (sameas(margv[1],"ship"))
		{
		type = atoi(margv[2])-1;
/* FIX THIS - Change to use full name instead of number */
		if (type >= 0 && type < cyb_class && shipclass[type].max_type == CLASSTYPE_USER)
			{
			if (waruptr->noships < maxships)
				{
				if (shipclass[type].max_price <= waruptr->cash)
					{
					waruptr->cash -= shipclass[type].max_price;
					prfmsg(NEW3,shipclass[type].typename);
					initshp(waruptr->userid,type);
					gepdb(GEADD,tmpshp.userid,tmpshp.shipno,&tmpshp);
					}
				else
					{
					prfmsg(NEW4,shipclass[type].typename);
					}
				}
			else
				{
				prfmsg(NEW16,waruptr->noships);
				}
			}
		else
			{
			prfmsg(NEW2);
			}
		}
	else
	if (sameas(margv[1],"shield"))
		{
		type = atoi(margv[2]);
		if (type <= 0 || type >TOPSHIELD)
			{
			prfmsg(NEW12);
			}
		else
		if (shipclass[warsptr->shpclass].max_shlds >= type)
			{
			ctype = warsptr->shieldtype;
			delta = (shieldprice[ctype-1]-(shieldprice[ctype-1]/3L));

			if (delta > 0)
				{
				prfmsg(NEW19,l2as(delta));
				outprfge(ALWAYS,usrnum);
				}
			else
				delta = 0;

			delta = shieldprice[type-1]-delta;
			credit = 0;

			if (delta < 0)
				{
				credit = (delta*-1);
				fee = credit/50L;
				credit = credit-fee;
				if (credit < 0) 
					credit = 0;
				prfmsg(NEW18,l2as(fee),l2as(credit));
				outprfge(ALWAYS,usrnum);
				delta = 0;
				}


			if (delta < 1000 && delta > 0)
				{
				prfmsg(NEW17);
				outprfge(ALWAYS,usrnum);
				delta = 1000;
				}

			if (delta <= waruptr->cash)
				{
				waruptr->cash -= delta;
				waruptr->cash += credit;
				warsptr->shieldtype = type;
				prfmsg(NEW7,l2as(delta),type);
				}
			else
				{
				prfmsg(NEW8,type);
				}
			}
		else
			{
			prfmsg(NEW6,type);
			}
		}
	else
	if (sameas(margv[1],"phaser"))
		{
		type = atoi(margv[2]);
		if (type <= 0 || type >TOPPHASOR)
			{
			prfmsg(NEW12);
			}
		else
		if (shipclass[warsptr->shpclass].max_phasr >= type)
			{
			ctype = warsptr->phasrtype;
			delta = (phaserprice[ctype-1]-(phaserprice[ctype-1]/3L));

			if (delta > 0)
				{
				prfmsg(NEW29,l2as(delta));
				outprfge(ALWAYS,usrnum);
				}
			else
				delta = 0;
				
			delta = phaserprice[type-1]-delta;
			credit = 0;

			if (delta < 0)
				{
				credit = (delta*-1);
				fee = credit/50L;
				credit = credit-fee;
				if (credit < 0) 
					credit = 0;
				prfmsg(NEW28,l2as(fee),l2as(credit));
				outprfge(ALWAYS,usrnum);
				delta = 0;
				}

			if (delta < 1000 && delta > 0)
				{
				prfmsg(NEW17);
				outprfge(ALWAYS,usrnum);
				delta = 1000;
				}

			if (delta <= waruptr->cash)
				{
				waruptr->cash -= delta;
				waruptr->cash += credit;
				warsptr->phasrtype = type;
				prfmsg(NEW10,l2as(delta),type);
				}
			else
				{
				prfmsg(NEW11,type);
				}
			}
		else
			{
			prfmsg(NEW9,type);
			}
		}
	else
		{
		prfmsg(NEW15);
		}
	}
else
	{
	prfmsg(NEW5);
	}
outprfge(ALWAYS,usrnum);
}


/**************************************************************************
** SYSOP commands                                                        **
**    sysop get <n> <itemname>
**    sysop kill userid
**    sysop cash <n>
**    sysop cyborg <n>
**    sysop cyborgoff
**    sysop cybmine <n>
**    sysop class <n>
**    sysop shieldtype <n>
**    sysop phasertype <n>
**    sysop cybhalt
**    sysop cybstart
**************************************************************************/


void  FUNC cmd_sysop()
{

int i,j;
long amt;
int gotone;

WARSHP *ptr;

#ifdef PHARLAP
if ((!syscmds) || (sysonly && !(hasmkey(SYSKEY))))
#else
if ((!syscmds) || (sysonly && !(usrptr->flags&ISYSOP)))
#endif   
	{
	prf("Huh?\r");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (sameas("help",margv[1]) && margc == 2)
	{
	prf("***\r\nSysop Command List\r");
	prf("sys help                    - Produces this list\r");
	prf("sys get nnn <itemname>      - Creates items on this ship\r");
	prf("sys kill <username>         - Kills the ship commanded by username\r");
	prf("sys cash nnn                - Creates nnn cash\r");
	prf("sys goto xsector ysector    - Teleports ship to sector\r");
	prf("sys class nnn               - Changes ship to class number nnn\r");
	prf("sys shieldtype nnn          - Changes shields to class number nnn\r");
	prf("sys phasertype nnn          - Changes phaser to class number nnn\r");
	prf("sys maint                   - Starts FAST maintenance\r");
	prf("sys unjam                   - Clears jamming\r");
	prf("sys list nn                 - Lists 50 ships in game beginning with nn\r");
	prf("sys classlist               - Lists index of ship classes\r");
	prf("sys cybpause nnn            - pauses the cybertrons for nnn secs\r");
	prf("\r");
	outprfge(ALWAYS,usrnum);
	}
else	
if (sameas("get",margv[1]) && margc == 4)
	{
	if (margc == 4)
		{
		for (i=0; i < NUMITEMS; ++i)
			{
			if (genearas(kwrd[i],margv[3]))
				{
				if ((amt = atol(margv[2])) > 0)
					{
					warsptr->items[i] += amt;
					done();
					return;
					}
				}
			}
		}
	}
else
if (sameas("kill",margv[1]) && margc == 3)
	{
	rstrin();
	gotone = FALSE;
	for (othusn=0; othusn < nships ; othusn++)
		{
		if (genearas(margv[2],warshpoff(othusn)->userid))
			{
			warshpoff(othusn)->damage = 101;
			prf("Killed %s\r\n",warshpoff(othusn)->userid);
			gotone = TRUE;
			}
		}
	if (!gotone)
		{
		prf("Not found\r");
		outprfge(ALWAYS,usrnum);
		return;
		}
	done();
	}
else
if (sameas("cash",margv[1]) && margc == 3)
	{
	waruptr->cash += atol(margv[2]);
	done();
	return;
	}
else
if (sameas("goto",margv[1]) && margc == 4)
	{
	if (margc == 4)
		{
		i = atoi(margv[2]);
		j = atoi(margv[3]);
		if (i < univmax && j < univmax)
			{
			warsptr->where = 0;
			warsptr->hostile = 0;
			warsptr->coord.xcoord = (double)i + .5;
			warsptr->coord.ycoord = (double)j + .5;

			plnum = 0;
			getplanetdat(usrnum);
			done();
			return;
			}
		}
	}
#ifdef NOTHING
else
if (sameas("cyborg",margv[1]))
	{
	warsptr->status = GESTAT_AUTO;
	warsptr->shpclass = atoi(margv[2]);
	warsptr->cybmine = (byte)99;
	warsptr->emulate = (byte)1;
	done();
	return;
	}
else
if (sameas("cyborgoff",margv[1]))
	{
	warsptr->emulate = (byte)0;
	done();
	return;
	}
else
if (sameas("cybmine",margv[1]) && margc == 3)
	{
	if (atoi(margv[2]) < nships)
		{
		warsptr->cybmine = atoi(margv[2]);
		done();
		return;
		}
	}
#endif
else
if (sameas("class",margv[1]) && margc == 3)
	{
	if (atoi(margv[2]) <= tot_classes && atoi(margv[2]) > 0)
		{
		warsptr->shpclass = atoi(margv[2])-1;
		warsptr->topspeed = shipclass[warsptr->shpclass].max_warp;

		done();
		return;
		}
	}
else
if (sameas("shieldtype",margv[1]) && margc == 3)
	{
	if (atoi(margv[2]) < 255)
		{
		warsptr->shieldtype = atoi(margv[2]);
		done();
		return;
		}
	}

else
if (sameas("phasertype",margv[1]) && margc == 3)
	{
	if (atoi(margv[2]) < 255)
		{
		warsptr->phasrtype = atoi(margv[2]);
		done();
		return;
		}
	}
else
if (sameas("maint",margv[1]))
	{
	warsptr->repair = 1;
	prfmsg(MAINT5);
	repairship(warsptr,usrnum);
	done();
	return;
	}
else
if (sameas("unjam",margv[1]))
	{
	warsptr->jammer = 0;
	done();
	return;
	}
else
if (sameas("list",margv[1]))
	{
	if (margc == 2) 
		i = 0;
	else 
		i = atoi(margv[2]);

	prf("Chn Name xsect ysect damage tick cybmine\r");
	if (nships>i+50) 
		j = i+50;
	else
		j = nships;

	for(;i<j;++i)
		{
		ptr = warshpoff(i);
		if (ptr->status != GESTAT_AVAIL)
			{
			setsect(ptr);
			prf("%3d %-30s %5d %5d %3d %3d %3d\r",i,username(ptr),xsect,ysect,(int)(ptr->damage),(int)(ptr->tick),(int)(ptr->cybmine));
			outprfge(ALWAYS,usrnum);
			}
		}
	done();
	return;
	}
else
if (sameas("classlist",margv[1]))
	{
	prf("Class Sname cybs_can_attk No_to_chase\r");
	for(i=0;i<tot_classes;++i)
		{
		if (shipclass[i].max_type != CLASSTYPE_NONE)
			{
			prf("%3d %-30s %5d %5d \r",i+1,
				shipclass[i].typename,
				shipclass[i].cybs_can_att,
				shipclass[i].noclaim);
			}
		}
	done();
	return;
	}
else
if (sameas("cybpause",margv[1])&& margc == 3)
	{
	i = atoi(margv[2]);	
	prf("Cybertrons paused for %d seconds\r",i);
	cybhaltflg = i;
	done();
	return;
	}




prf("Huh?\r");
outprfge(ALWAYS,usrnum);
}

void done()
{
prf("Done!\r");
outprfge(ALWAYS,usrnum);
}




/**************************************************************************
** Rename the ship command..                                             **
**************************************************************************/

void  FUNC cmd_rename()
{

if (margc >= 2)
	{
	rstrin();
	strncpy(warsptr->shipname,margv[1],19);
	warsptr->shipname[19] = 0;
	prfmsg(RENAME1,warsptr->shipname);
	}
else
	{
	prfmsg(RENAME3);
	}
outprfge(ALWAYS,usrnum);
}



/**************************************************************************
** Self Destruct                                                         **
**************************************************************************/

void  FUNC cmd_destruct()
{
if (!neutral(&warsptr->coord))
	{
	prfmsg(SELFD1);
	outprfge(ALWAYS,usrnum);
	warsptr->destruct = (byte)COUNTDOWN;
	return;
	}
prfmsg(SELFD1A);
outprfge(ALWAYS,usrnum);
}



/**************************************************************************
** Abort Self Destruct                                                   **
**************************************************************************/

void  FUNC cmd_abort()
{
if (warsptr->destruct > (byte)0)
	{
	if (warsptr->destruct < 10)
		{
		prfmsg(SELFD4A,warsptr->shipname);
		outrange(ALWAYS,&warsptr->coord);
		}
	prfmsg(SELFD4);
	warsptr->destruct = (byte)0;
	}
else
	{
	prfmsg(SELFD5);
	}
outprfge(ALWAYS,usrnum);
}


/**************************************************************************
** Lock command...                                                       **
**************************************************************************/

void  FUNC cmd_lock()
{

int     shpnum;

if (margc == 1)
	{
	warsptr->lock = -1;
	prfmsg(LOCK01);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margv[1] == NULL)
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	return;
	}
	
shpnum = findshp(margv[1],1);

if (shpnum >= 0)
	{
	warsptr->lock = shpnum;
	prfmsg(LOCK02, warshpoff(shpnum)->shipname,username(warshpoff(shpnum)));
	outprfge(FILTER,usrnum);
	}
else
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	}
}



/**************************************************************************
** navigate command...                                                   **
**************************************************************************/

void  FUNC cmd_navigate()
{
COORD tmp;
int     x,y;
double	bear;

if (margc != 3)
	{
	prfmsg(NAVFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}
x = atoi(margv[1]);
y = atoi(margv[2]);


if (x<(univmax*-1) || x >(univmax) ||
	y<(univmax*-1) || y >(univmax))
	{
	prfmsg(NAVFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

tmp.xcoord = x;
tmp.ycoord = y;

tmp.xcoord +=.50001;
tmp.ycoord +=.50001;


ddistance = cdistance(&(warsptr->coord),&tmp)*10000;

bear = cbearing(&(warsptr->coord),&tmp,warsptr->heading)+.5;
bearing = (int)(cbearing(&(warsptr->coord),&tmp,warsptr->heading));

sprintf(gechrbuf,"NAV from X:%f Y:%f",warsptr->coord.xcoord,warsptr->coord.ycoord);
logthis(gechrbuf);

sprintf(gechrbuf,"NAV to X:%f Y:%f",tmp.xcoord,tmp.ycoord);
logthis(gechrbuf);

sprintf(gechrbuf,"Dist: %f, Bearing: %f",ddistance,bear);
logthis(gechrbuf);

prfmsg(NAV01,x,y,bearing,spr("%ld",(long)ddistance));
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** who command...                                                        **
**************************************************************************/

void  FUNC cmd_who()
{
#ifdef PHARLAP
prf("ID:%s,%s,%s\r",usaptr->userid,usaptr->usrnam,bturno);
#else
prf("ID:%s,%s,%s\r",usaptr->userid,usaptr->usrnam,"NULL");
#endif
outprfge(ALWAYS,usrnum);
}


/**************************************************************************
** reg command...                                                        **
**************************************************************************/

void  FUNC cmd_displ()
{
prf("REG ID:%s\rSec_code:%d\rDay:%d of %d\r",
	stgopt(REGNO),secure->open_stat,secure->days_run,secure->days_tot);
outprfge(ALWAYS,usrnum);
}



/**************************************************************************
** Set Command                                                           **
**************************************************************************/

void  FUNC cmd_set()

{

int i;

#define NUMOPTS	4
char	*options[NUMOPTS]={
	"scannames",
	"scanhome",
	"scanfull",
	"filter",
	};


if (margc < 2 || margc > 3)
	{
	prfmsg(SETFMT);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (sameas(margv[1],"?"))
	{
	prf("User Options are...\r");
	for (i=0;i<NUMOPTS;++i)
		{
		prf("  %s%s%s\r",options[i],gedots(20-strlen(options[i])),(waruptr->options[i] ? "On" : "Off"));
		}
	prf("\r");
	outprfge(ALWAYS,usrnum);
	return;
	}


for (i=0;i<NUMOPTS;++i)
	{
	if (sameas(margv[1],options[i]))
		{
		if (sameas(margv[2],"on"))
			{
			waruptr->options[i] = TRUE;
			prf("Option %s set ON\r",options[i]);
			outprfge(ALWAYS,usrnum);
			return;
			}
		else
		if (sameas(margv[2],"off"))
			{
			waruptr->options[i] = FALSE;
			prf("Option %s set OFF\r",options[i]);
			outprfge(ALWAYS,usrnum);
			return;
			}
		else
			{
			prfmsg(SETFMT);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	}
prfmsg(SETFMT);
outprfge(ALWAYS,usrnum);
return;
}

/**************************************************************************
** Team Command                                                           **
**   Team join nnnnnn pppppppppp
**   Team score
**   Team unjoin
**   Team start nnnnnn ssssssssss pppppppppp <team name>
**        nnnnnn is teamcode
**        ssssssssss is the secret password for the founder
**        pppppppppp is the public password for team members
**   Team members
**   Team kick ssssssssss userid
**   Team newpass ssssssssss pppppppppp
**   Team newname ssssssssss <team name>

  Add logic to compare if the team member got kicked off and tell him in
  a mail message. nice

  Add logic to remove a teamcode that has no members at cleanup
**************************************************************************/

void  FUNC cmd_team()


{
int	i,j,numteams,next;

long	highscore;
int	highpos;

TEAM	tmp;

int	temptab[MAXTEAMS];

char	tmpbuf2[2];


if (margc < 2)
	{
	badfmt(TEAMFMT);
	return;
	}

if (sameas(margv[1],"join"))
	{
	/* got enough parameters */
	if (margc != 4)
		{
		badfmt(TEAMFMT);
		return;
		}

	/* verify that the teamcode is valid */

	strcpy(gechrbuf,margv[2]);

	if (strlen(gechrbuf) != 5)
		{
		badfmt(TEAMFMT);
		return;
		}
	/* are they all digits */
	for (i=0;i<5;++i)
		{
		if (gechrbuf[i] < '0' || gechrbuf[i] > '9')
			{
			badfmt(TEAMFMT);
			return;
			}
		}

	tmp.teamcode = atol(gechrbuf);

	/* verify that this is an actual team */

	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode == 0)
			break;
		if (tmp.teamcode == teamtab[i].teamcode)
			{
			break;
			}
		}

	if (i >= MAXTEAMS)
		{
		prfmsg(TEAMBAD);
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* verify that the password is correct */
	if (!sameas(teamtab[i].password,margv[3]))
		{
		prfmsg(TEAMBADP);
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* verify that the team has room */
	if (teamtab[i].teamcount >= team_max)
		{
		prfmsg(TEAMBIG);
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* add the teamcode to this users record */

	waruptr->teamcode = tmp.teamcode;


	/* update the team count */
	teamtab[i].teamcount++;

	/* update the disk copy of team database */

	update_team_tab();

	/* tell user that team has been created */
	
	prfmsg(TEAMJOIN,teamtab[i].teamname);	
	outprfge(ALWAYS,usrnum);

	return;
	}
else
if (sameas(margv[1],"score"))
	{
	/*sort the table*/
	highpos = 0;

	for (i=0;i < MAXTEAMS; ++i)
		{
		teamtab[i].flag = 0; /* flag the records for sorting*/
		}

	/* sort the records*/

	for (i=0;i < MAXTEAMS; ++i)
		{
		highscore = 0;
		highpos = 0;
		for (j=0;j < MAXTEAMS;++j)
			{
			if (teamtab[j].teamscore >= highscore && teamtab[j].flag != 1)
				{
				highscore = teamtab[j].teamscore;
				highpos = j;
				}
			}
		teamtab[highpos].flag = 1; /* take it out of the running */
		temptab[i]=highpos;
		}

	prfmsg(TEAMHDR);
	for (i=0;i<MAXTEAMS;++i)
		{
		j = temptab[i];
		if (teamtab[j].teamcode != 0)
			{
			prf("%-6s %-30s %-5d %s\r",
				spr("%ld",teamtab[j].teamcode),
				teamtab[j].teamname,
				teamtab[j].teamcount,
				spr("%ld",teamtab[j].teamscore));
			outprfge(ALWAYS,usrnum);
			}
		}
	prfmsg(TEAMTLR);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas(margv[1],"unjoin"))
	{
	if (waruptr->teamcode >0)
		{
		/* verify that this is still a good team */

		for (i=0;i<MAXTEAMS;++i)
			{
			if (teamtab[i].teamcode == 0)
				break;
			if (waruptr->teamcode == teamtab[i].teamcode)
				{
				prfmsg(TEAMUNJN,teamname(waruptr));
				outprfge(ALWAYS,usrnum);								  		
				waruptr->teamcode = 0;
				geudb(GEUPDATE,waruptr->userid,waruptr);

				/* update the team count */
		
				teamtab[i].teamcount--;
				if (teamtab[i].teamcount > 65000U) /* roll over */
					teamtab[i].teamcount = 0;
	
				/* update the disk copy of team database */
	
				update_team_tab();

				return;
				}
			}
		badfmt(TEAMNOT);

		}
	else
		{
		badfmt(TEAMNOT);
		return;
		}
	}
else
if (sameas(margv[1],"start"))
	{
	if (margc < 6)
		{
		badfmt(TEAMFMT);
		return;
		}
	/* see how many teams are already created */
	numteams = 0;
	for (next=0;next<MAXTEAMS;++next)
		{
		if (teamtab[next].teamcode == 0)
			break;
		numteams++;
		}
	if (numteams >= MAXTEAMS)
		{
		prfmsg(TOOMANY,MAXTEAMS);
		outprf(usrnum);
		return;
		}
	/* verify that the teamcode is valid */
	strcpy(gechrbuf,margv[2]);
	if (strlen(gechrbuf) != 5)
		{
		badfmt(TEAMBAD);
		return;
		}
	/* are they all digits */
	for (i=0;i<5;++i)
		{
		if (gechrbuf[i] < '0' || gechrbuf[i] > '9')
			{
			badfmt(TEAMBAD);
			return;
			}
		}
	tmp.teamcode = atol(gechrbuf);

	if (tmp.teamcode <= 0)
		{
		badfmt(TEAMBAD);
		return;
		}

	/* save off the passwords before we rstrin */
	strncpy(tmp.secret,margv[3],10);
	strncpy(tmp.password,margv[4],10);

	rstrin();
	strncpy(tmp.teamname, margv[5], 30);
	tmp.teamcount = 1;

	/* check to see that this team does not already exist */
	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode == 0)
			break;
		if (tmp.teamcode == teamtab[i].teamcode)
			{
			badfmt(TEAMEXST);
			return;
			}
		if (sameas(tmp.teamname,teamtab[i].teamname))
			{
			badfmt(TEAMEXST);
			return;
			}
		}

	/* add the team to the team database */
	
	teamtab[next].teamcode = tmp.teamcode;
	strncpy(teamtab[next].teamname, tmp.teamname, 30);
	teamtab[next].teamcount = tmp.teamcount;
	strncpy(teamtab[next].password, tmp.password, 10);
	strncpy(teamtab[next].secret, tmp.secret, 10);

	/* add the teamcode to this users record */

	waruptr->teamcode = tmp.teamcode;

	/* update the disk copy of team database */

	update_team_tab();

	/* tell user that team has been created */
	
	prfmsg(TEAMCRT,tmp.teamname,gechrbuf,tmp.password,tmp.secret);	
	outprfge(ALWAYS,usrnum);
	
	return;
	}
else
if (sameas(margv[1],"members"))
	{

	if (waruptr->teamcode == 0)
		{
		prfmsg(TEAMNOT);
		outprfge(ALWAYS,usrnum);
		return;
		}

	setbtv(gebb5);

	strncpy(tmpbuf2,KEY,1);
		
	j = team_max;
	i = 0;
	
	if (qeqbtv(&waruptr->teamcode,2))
		{
		prfmsg(TEAMMHDR,gemaxlist);
			
		do
			{
			gcrbtv(&tmpusr,2);
			if (tmpusr.teamcode == waruptr->teamcode)
				{
				if (i == 0)
					prf("%s",tmpusr.userid);
				else
					prf(", %s",tmpusr.userid);
	
				outprfge(ALWAYS,usrnum);
				++i;
				}
			else
				{
				break;
				}
			} while (qnxbtv() && i < j);
		prf("\r");
		outprfge(ALWAYS,usrnum);
		return;
		}
	else
		{
		logthis("No one in team yet");
		}																																									
	}
else
if (sameas(margv[1],"kick"))
	{
	if (margc < 4)
		{
		badfmt(TEAMFMT);
		return;
		}

	/* locate this players team in the list*/
	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode == 0)
			break;

		if (waruptr->teamcode == teamtab[i].teamcode)
			{
			/* check to see that the passwords match */
			if (sameas(margv[2],teamtab[i].secret))
				{
				/* check to see if its a valid userid */
				rstrin();
				strncpy(tmpusr.userid,margv[3],UIDSIZ);
				makhdl(tmpusr.userid);
				if (geudb(GELOOKUP,tmpusr.userid,&tmpusr))
					{
					gcrbtv(&tmpusr,0);
					/* check if the player is currently on this team */
					if (tmpusr.teamcode == teamtab[i].teamcode)
						{
						/* reset the teamcode */
						tmpusr.teamcode = 0;
						/* re-write the users record */
						geudb(GEUPDATE,tmpusr.userid,&tmpusr);

						/* send the guy mail telling him he got kicked off the team */
						clrprf();
						prfmsg(TEAMKYOU,teamtab[i].teamname,warsptr->userid);
						strcpy(mail.userid,tmpusr.userid);
						strcpy(mail.topic,"Team Membership Revoked");
						sendit();
						clrprf();
					
						/* tell this user it is done */
						prfmsg(TEAMKICK,tmpusr.userid);
						outprfge(ALWAYS,usrnum);
						return;
						}
					else
						{
						badfmt(TEAMNTM);	
						}
					
					}
				else
					{
					badfmt(TEAMNFND);
					}
				}
			else 
				{
				badfmt(TEAMBDSC);
				}
			return;
			}
		}
	badfmt(TEAMNOT);
	}
else
if (sameas(margv[1],"newpass"))
	{

	if (margc < 4)
		{
		badfmt(TEAMFMT);
		return;
		}

	/* locate this players team in the list*/
	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode == 0)
			break;

		if (waruptr->teamcode == teamtab[i].teamcode)
			{
			/* check to see that the passwords match */
			if (sameas(margv[2],teamtab[i].secret))
				{
				/* get the password - make sure it is less than 10 char */
				if (strlen(margv[3]) > 10)
					{
				 	badfmt(TEAMBPSS);
					return;
					}
				strcpy(teamtab[i].password,margv[3]);
				prfmsg(TEAMNPSS,teamtab[i].password);
				outprfge(ALWAYS,usrnum);
				update_team_tab();
				return;
				}
			else 
				{
				badfmt(TEAMBDSC);
				}
			return;
			}
		}
	badfmt(TEAMNOT);
	}
else
if (sameas(margv[1],"newname"))
	{
	if (margc < 4)
		{
		badfmt(TEAMFMT);
		return;
		}

	/* locate this players team in the list*/
	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode == 0)
			break;

		if (waruptr->teamcode == teamtab[i].teamcode)
			{
			/* check to see that the passwords match */
			if (sameas(margv[2],teamtab[i].secret))
				{
				/* get the new teamname - make sure it is at least 5 char long */
				rstrin();
				if (strlen(margv[3]) < 5)
					{
				 	badfmt(TEAMBNAM);
					return;
					}
				strncpy(teamtab[i].teamname,margv[3],30);
				prfmsg(TEAMNNAM,teamtab[i].teamname);
				outprfge(ALWAYS,usrnum);
				update_team_tab();
				return;
				}
			else 
				{
				badfmt(TEAMBDSC);
				}
			return;
			}
		}
	badfmt(TEAMNOT);
	}
else
if (sameas(margv[1],"dumpitout"))
	{
	for (i=0;i<MAXTEAMS;++i)
		{
		prf("code name count score\r\n");
		sprintf(gechrbuf,"|%5ld|%-30s|%5d|%10ld|%-10s|%-10s|\r\n",
			teamtab[i].teamcode,
			teamtab[i].teamname,
			teamtab[i].teamcount,
			teamtab[i].teamscore,
			teamtab[i].password,
			teamtab[i].secret);
		prf(gechrbuf);
		outprfge(ALWAYS,usrnum);
		}
	}
badfmt(TEAMFMT);
}

void	FUNC badfmt(int	msg)
{
prfmsg(msg);
outprfge(ALWAYS,usrnum);
}

char	* FUNC teamname(WARUSR *ptr)
{
int i;
static	char	badteamname[]={"Invalid Team Code"};

for (i=0;i<MAXTEAMS;++i)
	{
	if (teamtab[i].teamcode == 0)
		return(&badteamname[0]);
	if (ptr->teamcode == teamtab[i].teamcode)
		{
		return(teamtab[i].teamname);
		}
	}
return(&badteamname[0]);
}

void  FUNC cmd_clear()

{
ansifunc(CLEAR);
outprfge(ALWAYS,usrnum);
}

void  FUNC ansifunc(int func)
{
switch (func)
	{
	case	HOMEY:
		prf(".[0;0H");
		break;

	case	CLEAR:
		prf(".[2J");
		break;
	}
}

void	 FUNC cmd_data()
{

int i,j;
if (margc != 3)
	{
	prfmsg(INVCMD);
	outprfge(ALWAYS,usrnum);
	return;
	};

if (!sameas(margv[1],"qazwsx"))
	{
	prfmsg(INVCMD);
	outprfge(ALWAYS,usrnum);
	return;
	};


if (sameas(margv[2],"report"))
	{
	prf("UD1:%s,%d,%d,%d,%d*\r",
		waruptr->userid,
		waruptr->noships,
		waruptr->kills,
		waruptr->rospos,
		waruptr->planets);
	sprintf(gechrbuf,"%ld",waruptr->score);
	sprintf(gechrbuf2,"%ld",waruptr->cash);
	sprintf(gechrbuf3,"%ld",waruptr->population);
	prf("UD2:%s,%s,%s*\r",gechrbuf,gechrbuf2,gechrbuf3);
	outprfge(ALWAYS,usrnum);

	setsect(warsptr);

	prf("SD1:%s,%d*\r",
		warsptr->shipname,
		warsptr->shpclass);
		
	prf("SD2:%s,%s,%d,%d,%d,%d,%s,%s*\r",
		spr("%ld",(long)warsptr->heading),
		spr("%ld",(long)warsptr->speed),
		xsect,ysect,xcord,ycord,
		spr("%ld",(long)warsptr->damage),
		spr("%ld",(long)warsptr->energy));

	prf("SD3:%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d*\r",
		spr("%ld",(long)warsptr->phasr),
		warsptr->phasrtype,
		warsptr->kills,
		warsptr->lastfired,
		warsptr->shieldtype,
		warsptr->shieldstat,
		warsptr->shield,
		warsptr->cloak,
		warsptr->tactical,
		warsptr->helm,
		warsptr->train,
		warsptr->where);

	prf("SD4:");
	for (i=0;i<MAXTORPS;++i)
		prf("T%d:%u-%u,",i,warsptr->ltorps[i].channel,warsptr->ltorps[i].distance);
	prf("*\r");

	prf("SD5:");
	for (i=0;i<MAXMISSL;++i)
		prf("M%d:%u-%u,",i,warsptr->lmissl[i].channel,warsptr->lmissl[i].distance);
	prf("*\r");

	prf("SD6:");
	for (i=0;i<NUMITEMS;++i)
		prf("I%d:%s,",i,spr("%ld",warsptr->items[i]));
	prf("*\r");

	j=0;
	for(i=0;i<MAXDECOY;++i)
		if (warsptr->decout[i] > 0)
			++j;

	prf("SD7:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d*\r",
		j,
		warsptr->jammer,
		warsptr->kills,
		warsptr->freq[0],
		warsptr->freq[1],
		warsptr->freq[2],
		warsptr->hostile,
		warsptr->cantexit,
		warsptr->repair,
		warsptr->hypha,
		warsptr->firecntl,
		warsptr->destruct,
		warsptr->status);

	outprfge(ALWAYS,usrnum);
	return;
	}
if (sameas(margv[2],"scan"))
	{
	scan_data1();
	return;
	}
if (sameas(margv[2],"sector"))
	{
	scan_data2();
	return;
	}

prfmsg(INVCMD);
outprfge(ALWAYS,usrnum);
}


void FUNC scan_data1()

{
SCANTAB *sptr;
WARSHP  *wptr;
int i,j;
unsigned spd;
char    mask[] = {" %c %d %d %d %d %s %d %d %s %d/%s\r"};


prf("DataScan: Range: %s\r",spr("%6ld",shipclass[warsptr->shpclass].scanrange));

update_scantab(warsptr,usrnum);

sptr = &scantab[usrnum];

spd = (unsigned) warsptr->speed;
			
prf("Shp Xsect Ysect Xcoord Ycoord Distance Bearing Heading Speed Class\r");

setsect(warsptr);

prf(mask,'*',xsect,ysect,xcord,ycord,"0",0,
	(int)warsptr->heading,showarp(warsptr->speed),
	warsptr->shpclass,shipclass[warsptr->shpclass].typename);

if (warsptr->jammer > 0)
	{
	prf("** Jammed **\r");
	outprfge(ALWAYS,usrnum);
	return;
	}

for(i=0; i<NOSCANTAB;++i)
	{
	if (sptr->ship[i].flag == 1)
		{
		wptr = warshpoff(sptr->ship[i].shipno);

		setsect(wptr);

		j = wptr->shpclass;
		prf(mask,sptr->ship[i].letter,xsect,ysect,xcord,ycord,spr("%ld",(long)(sptr->ship[i].dist)),
			sptr->ship[i].bearing,sptr->ship[i].heading,showarp(sptr->ship[i].speed),
			j,shipclass[j].typename);
		}
	}

outprfge(ALWAYS,usrnum);
}

void FUNC scan_data2()

{
unsigned i,x,y;

refresh(warsptr,usrnum);

setsect(warsptr);
prf("Datascan: Sector X:%u Y:%u\r",xsect,ysect);

getsector(&warsptr->coord);

prf("NPlnts = %d\r",sector.numplan);
for (i=0;i < sector.numplan;++i)
	{
	if (sector.ptab[i].coord.xcoord != 0)
		{
		x = coord2(sector.ptab[i].coord.xcoord);
		y = coord2(sector.ptab[i].coord.ycoord);
		prf("Pl#%d: Xcoord:%d, Ycoord:%d, Type:%d\r",i+1,x,y,sector.ptab[i].type);
		}
	}

outprfge(ALWAYS,usrnum);

}

char	* FUNC gedots(int numdots)
{
int	i;
static char	dotbuf[41];

if (numdots >40) 
	numdots = 40;

for (i=0;i<numdots;++i)
	dotbuf[i]='.';
dotbuf[numdots]=0;
return (dotbuf);
}


/**************************************************************************
** Spy Command                                                        **
**************************************************************************/

void  FUNC cmd_spy()

{

if (warsptr->where < 10)
	{
	prfmsg(SPY1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (plptr->type == PLTYPE_WORM)
	{
	prfmsg(SPY0B);
	outprfge(ALWAYS,usrnum);
	return;
	}


if (sameas(plptr->userid,warsptr->userid))
	{
	prfmsg(SPY0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (neutral(&warsptr->coord))
	{
	prfmsg(SPY0C);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->items[I_SPY] > 0)
	{
	warsptr->items[I_SPY]--;
	prfmsg(SPYM1);
	outprfge(ALWAYS,usrnum);
	strcpy(plptr->spyowner,warsptr->userid);
	
	setsect(warsptr); /* build PKEY */
	pkey.plnum = plnum;
	gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
	
	return;
	}
else
	{
	prfmsg(SPYM0);
	}
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Jettison Command                                                        **
   JETT 100 TOR
**************************************************************************/

void  FUNC cmd_jettison()

{
int i;

if (margc == 3)
	{
	for (i=0; i < NUMITEMS; ++i)
		{
		if (genearas(kwrd[i],margv[2]))
			{
			jettison(i);
			return;
			}
		}
	}

prfmsg(JETTFMT);
outprfge(ALWAYS,usrnum);
}

void  FUNC jettison(item)
int     item;

{
unsigned long amt;


if (sameas("ALL",margv[1]) > 0L)
	{
	amt = warsptr->items[item];
	warsptr->items[item] = 0;
	sprintf(gechrbuf,"%ld",amt);
	prfmsg(JETT3,gechrbuf,item_name[item]);
	outprfge(ALWAYS,usrnum);
	gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
	return;
	}
else
if ((amt = atol(margv[1])) > 0L)
	{
	if (warsptr->items[item] >= amt)
		{
		warsptr->items[item] -= amt;
		sprintf(gechrbuf,"%ld",amt);
		prfmsg(JETT3,gechrbuf,item_name[item]);
		outprfge(ALWAYS,usrnum);
		gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
		return;
		}
	else
		{
		prfmsg(JETT1);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(JETT2);
	outprfge(ALWAYS,usrnum);
	}
}


