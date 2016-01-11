
/***************************************************************************
 *                                                                         *
 *   GEGLOBAL.H                                                            *
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


/* GEMAIN Global Defs ****************************************************/

#ifndef GEMAIN

extern  int			gestt;

extern  BTVFILE	*gebb1,  /* GEship.dat   */
						*gebb2,  /* GEplanet.dat */
						*gebb4,  /* GEmail.dat   */
						*gebb5;  /* GEuser.dat   */

extern  FILE		*gemb, *gehlpmb, *geshmb;
extern  FILE		*opnmsg();

extern  int			numwar;                 /* number of users in game  */

extern  WARSHP		tmpshp;  /* used to temporarly set up a new ship */
extern  WARSHP		*warshp,*warsptr;
extern  WARUSR		*warusr,*waruptr;
extern  int			warusr_ecl,warshp_ecl;
extern  WARUSR		tmpusr;

extern  GALSECT	sector,*sptr1;

extern  GALPLNT	planet;

extern  GALWORM	worm;

extern  PLANETAB	*ptab;

extern  char		map[MAXY][MAXX+1];          /* global scan map array */
extern  char		mapc[MAXY][MAXX+1];          /* global scan map array color map */

extern  MINE		*mines;       /* place to stuff mines */


extern  MAIL		mail;
extern  MAILSTAT			tmpstat;

extern  BEACONTAB	*beacon;

extern  struct message *gemsg;

extern  int			nships;
extern  int			heading;
extern  unsigned	speed;
extern  int			lockwarn;
extern  int			xsect,ysect;
extern  unsigned	xcord,ycord, coord2();
extern  PKEY		pkey;
extern  unsigned	distance;
extern  double		ddistance;
extern  int			bearing;
extern  unsigned	energy;
extern  unsigned	damage;
extern  char		*gechrbuf,*gechrbuf2,*gechrbuf3;
extern  char 		*warpbuf;
extern  int			sectnum,plnum;
extern  GALPLNT	*plptr;
extern  ITEM		*titems;
extern  TEAM		*teamtab;

extern  int			su;
extern  int			tmp_usrnum;

extern  unsigned	plantime;

extern  long		max_plrec,
						teambonus,
						pltvcash,
						pltvdiv,
						startcash;

extern  int			gemaxplrs,
						gefreebies,
						gemaxlist,
						maxships,
						se100dam,
						profon,
						showopt,
						syscmds,
						sysonly,
						max_plnts,
						trans_opt,
						numships,
						univmax,
						plodds,
						wormodds,
						hpfirdst,
						hpdammax,
						pfirdist,
						pdammax,
						jamtime,
						maildays,
						torpsped,
						mislsped,
						decodds,
						nummines,
						usermines,
						maxdroids,
						cyb_class,
						clenguse,
						logflag,
						optmenu,
						cyb_gold,
						tot_classes,
						team_max,
						usegemsg,
						fse_state,
						phatowrp,
						misengfc,
						score_bonus,
						score_f1,
						score_f2,
						chgloser,
						univwrap,
						maxplanets;

extern	char		*opttxt,
						optchr;

extern 	long		*opttbl;

extern	double	tor_fact,
						tdammax,
						mis_fact,
						mdammax,
						idammax,
						minedammax,
						repairrate,
						tooclose,
						hyperdist1,
						hyperdist2,
						plattrf1,
						plattrf2,
						plattrf3,
						plattrt1,
						plattrt2,
						plattrt3;



extern  long	plantock;

#ifdef  TESTDRIVE
extern  int			endtd,
						nowtd,
						lefttd;
#endif

extern  SHPKEY		shpkey;
extern  MAILKEY	mailkey;

extern  SHIP		*shipclass;

extern  S00			*s00;
extern  int			s00plnum;

extern  SCANTAB	*scantab;

extern  long		shieldprice[TOPSHIELD];
extern  long		phaserprice[TOPPHASOR];
extern  unsigned	baseprice[NUMITEMS];
extern  double		maxpl[NUMITEMS];
extern  long		weight[NUMITEMS];
extern  long		value[NUMITEMS];
extern  long	   manhours[NUMITEMS];

#endif

/* GECMDS Global Defs ****************************************************/

#ifndef GECMDS

extern  char		*kwrd[NUMITEMS];
extern  char		*item_name[NUMITEMS];

#endif

/* GEFUNCS Global Defs ***************************************************/

#ifndef GEFUNCS
#endif

/* GEPLANET Global Defs **************************************************/

#ifndef GEPLANET
#endif

/* GECYBS Global Defs ****************************************************/

#ifndef GECYBS
extern  char		*cybname;
extern  int			cybhaltflg;
#endif

#ifdef PHARLAP
#else
/* MAJORBBS Global Defs **************************************** */

extern
struct usracc 	*usracc, 	/* user accounting block array               */
	      		*usacptr;	/* user accounting block pointer for usrnum  */
extern
int 				usrnum;		/* global user-number (channel) in effect    */

extern
struct user 	*usrptr, 	/* global pointer to user data in effect     */
					*user;		/* user volatile-data structure array        */
extern
char input[INPSIZ],   		/* raw user input data buffer                */
     *margv[INPSIZ/2];		/* array of ptrs to word starts, a la argv[] */

extern
int margc,						/* number of words in margv[], a la argc     */
    status;						/* raw status from btusts, where appropriate */


extern
struct module *module[NMODS]; /* module access block pointer table         */


extern
int inplen,						/* overall raw input string length           */
    pfnlvl;						/* profanity level of current input (0-3)    */

extern
char prfbuf[];					/* 2K buffer used by prfutl, used for staging*/

extern
struct sysvbl sv;				/* sys-variables record instance for updates */

char *curtim(),*curdat();	/* time and date formatter routines          */


extern int othusn;
extern struct user *othusp;   /* gen purp other-user user structure ptr    */
extern struct usracc *othuap; /* gen purp other-user accounting data ptr   */

#endif

#include "geproto.h"


