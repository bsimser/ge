
/***************************************************************************
 *                                                                         *
 *   GEMAIN.H                                                              *
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
 

#define TRUE            1
#define FALSE           0

/* if this is the trainer version this define needs to be made */
/*
#define	GETRAINER 1
*/

#ifdef GETRAINER
#include "mbmg2msg.h"
#include "mbmg2hlp.h"
#include "mbmgeshp.h"
#else
#include "mbmgemsg.h"
#include "mbmgehlp.h"
#include "mbmgeshp.h"
#endif

/***************************************************************************/
/* The following defines have been carefully chosen and refined to provide */
/* a well balanced and entertaining game play. Should they be modified,    */
/* do so with caution and consideration                                    */
/***************************************************************************/

#define NEUTRAL_X               0
#define NEUTRAL_Y               0

#define ROTENGUSE 30          /* Energy used to rotate                     */
#define ROTAMT   20           /* Amount of degres rotation each tick       */

#define ACCENGAMT 120          /* Acceleration energy usage                 */
#define MOVENGMIN 3000
#define MOVENGUSE 10

#define PRELOAD  10           /* Phasor reload rate / tick                 */
#define PMINFIRE 60           /* Minimum amount to fire phasor             */
#define PENGUSE  57           /* Amount of energy used by phasors charging */
#define PMINENG  500          /* minimum energy to fire                    */
#define PHABIAS 2          /* Degrees to bias "spread" to better aim    */

#define HPMINFIR 6000        /* Minimum energy in flux to fire Hyper phasr*/
#define HPFIRAMT 5000
#define HPBEAMW  5

#define ENGYMAX  65000U        /* Maximum energy one can have               */
#define ENGRECHG 1           /* Energy recharge amount / tick             */
#define ENGYMIN  5000           /* Minimum Energy in FLUX to autoload        */

#define SHMINPWR 200          /* Minimum power fore shields                */
#define SHENGUSE 100          /* Energy used every tick for shields        */
#define SHHITENG 1000         /* Energy drained when hit                   */
#define SHMAXCHG 10           /* Max (full) charge in the shields          */
#define SHMINCHG 5            /* Minimum charge to be effective            */

#define SCANADJ   40           /* Adjustment factor for range scanner       */

#define TONFACT	15000.0      /* divisor for ton factor */

#define SSMAX    10000

#define SNMSIZ  25

#define PI 3.1415927

#define GELOOKUP     1
#define GEADD        2
#define GEDELETE     3
#define GEUPDATE     4
#define GEGET        5
#define GENEXT  		6
#define GELOOKUPNAME 7
#define GEGETNOW 		8

#define MAXPLANETS 9

#define MAXX 30
#define MAXY 15

/* do not change these as they effect structure sizes */
#define MAXTORPS 3
#define MAXMISSL 3
#define MAXDECOY 10

/* how far does the hyper scanner look */
#define HYSCANRANGE 5

#define DECOYTIME 15             /* times TICKTIME how long a decoy lives */
#define TICKTIME  6             /* how often does RTKICK fire    6   */
#define TICKTIME2 1              /* how often does RTKICK fire       */
#define CYBTICKTIME 6           /* Cyborgs do stuff this fast       */
#define PLANTIME  55                                    /* how often do planets get updated */

#define FIRETICKS 10           /* how many ticks to hold a player  */
/* when involved in battle          */

#define  NUMITEMS       14

#define I_MEN				0
#define I_MISSILE			1
#define I_TORPEDO			2
#define I_IONCANNON		3

#define I_FLUXPOD			4
#define I_FOOD				5
#define I_FIGHTER			6
#define I_DECOYS			7
#define I_TROOPS			8
#define I_ZIPPERS			9
#define I_JAMMERS			10
#define I_MINE				11
#define I_GOLD				12
#define I_SPY				13

#define SHIELDUP			1
#define SHIELDDN			2
#define SHIELDDM			3

#define SHIELD_FACTOR   4

#define COUNTDOWN			20   /* self destruct countdown */

#define CYB_GO_WARP             1
#define CYB_MINCLASS    3   /* Minimum class a Cybertron will pick on */
#define CYBSLO                          3   /* slows down the cyborgs reaction time */
#define CYB_ALLOW                       35  /* how much allowance per tick          */
#define CYB_MAXCASH     2000000L  /* maximum cash cybertrons can hold */
#define CYB_BE_NICE             30            /* 30 kills grace before we get tough */
#define CYB_BE_EASY             60       /* 60 kills till we get really mean */
#define CYB_BREAKOFF            500     /* 1 in x chance that cyb will break off attk */
#define CYB_MINDAM              75    /* % of damage when cyb jams and mines */
#define CYBS_FIGHT              FALSE /* TRUE if Cybs fight each other */
#define CYB_BADBAD              FALSE /* TRUE if cybs attack anyone once they */
										/* have been fired upon. FALSE if they  */
										/* only attack their attacker */
#define CYBMAXPERTICK   2               /* the number of cybs processed each tick */
#define QUADMAXPERTICK  5               /* the number of quads processed each tick */


#define TELEDAM                 17      /* amount of damage done when edge reached */


/* the following define if included will cause the captured planet map
	to be displayed to the victor of a battle - The odds of it being
	captured are 1 in RNDDOC (set below) - To remove this feature from
	the game simple comment out the following two defines */

#define SHOWDOC                 1
#define RNDDOC                          6

#define MINERANGE 10000        /* maximum Mine range                     */

#define DESTRUCTRANGE 10000

#define NUM_MINES                       20   /* 20 mines in the game max */

#define TOPSHIELD 19

#define TOPPHASOR 19

#define SECTYPE_NORMAL  1
#define PLTYPE_PLNT     2
#define PLTYPE_WORM     3

#define GESTAT_AVAIL 0
#define GESTAT_USER  1
#define GESTAT_AUTO  2
#define GESTAT_IDLE  3

#define CLASSTYPE_USER          1
#define CLASSTYPE_CYBORG        2
#define CLASSTYPE_DROID         3
#define CLASSTYPE_NONE          4


#define MAIL_CLASS_DISTRESS	1
#define MAIL_CLASS_MAXOUT		2
#define MAIL_CLASS_PRODRPT		3
#define MAIL_CLASS_GAMESTATS	4
#define MAIL_CLASS_PLSTATS		5

/* ansi display function codes*/

#define 	HOMEY	1
#define	CLEAR	2

/* user options */

#define	SCANNAMES	0
#define	SCANHOME		1
#define	SCANFULL		2
#define	MSG_FILTER	3
#define  DUMMY1		4
#define  DUMMY2		5

#define	MAXTEAMS		50

#define BEACONMSGSZ	75

#define	GEMSGSIZ		784
#define FIXEDMSGSIZ   sizeof(struct message)+GEMSGSIZ


#define 	ALWAYS		1
#define	FILTER		2
#define	BLOCK			3

/*****************************************************************************/
/* End of Defines                                                            */
/*****************************************************************************/

#ifndef byte
#define byte unsigned char
#endif

#define COORD struct _coord
extern
COORD {
	  double     xcoord;
	  double ycoord;
		};

#define MINE struct _mine
MINE            {
			byte            channel;
			unsigned timer;
			COORD   coord;
			};

#define MISSILE struct _missile
MISSILE         {
			unsigned char channel;
			unsigned distance;
			unsigned energy;
			};

#define TORPEDO struct _torpedo
TORPEDO {
			unsigned char channel;
			unsigned distance;
			};


#define WARUSR struct warusr
extern


WARUSR {                           /* user data blocks                       */
		char				userid[UIDSIZ];     /* userid of player                       */
		unsigned long	score;              /*   Your score               31     */
		char				waste[40];
		unsigned			noships;            /*   Number of ships                      */
		int				topshipno;          /*   Top ship number used                 */
		int				kills;              /*   No kills                             */
		unsigned			rospos;
		int				planets;            /*   Planets owned                        */
		unsigned long	cash;               /*   cash on hand                         */
		unsigned long 	debt;               /*   I owe, I owe, its off to work I go   */
		unsigned long	plscore;
		unsigned long 	klscore;
		unsigned long 	population;
		byte				options[30];
		unsigned long	teamcode;
		char    filler[256-109-UIDSIZ];
};


#define WARSHP struct warshp
extern


WARSHP {                        /* user data blocks         */
		char            userid[UIDSIZ];     /* userid of player                          */
		int             shipno;             /*   Ship id number                          */
		char            shipname[35];       /*   Ship name                               */
/*32*/
		int     shpclass;           /*   Ship Class   display                    */
		double  heading;            /*   Heading of this ship                    */
/*42*/
		double  head2b;             /*   direction rotating to                   */
		double  speed;              /*   Speed                                   */
/*58*/
		double  speed2b;            /*   Speed to be                             */
/*66*/
/*70*/
		COORD   coord;
/*86*/
		double  damage;             /*   Damage on this ship (0 - 100%)          */
/*94*/
		double  energy;             /*   Amount of energy available              */
/*102*/
		double  phasr;              /*   Phasor strength (0 - 100%)              */
/*110*/
		byte            phasrtype;          /*   Type of Phasor system                   */
		int     kills;              /*   No kills                                */
		int     lastfired;          /*   usernumber of last user to fire on you  */
		byte            shieldtype;         /*   shield type                             */
		byte            shieldstat;         /*   shield status (supposed to be up/dn)    */
		int             shield;             /*   shield charge                           */
		int     cloak;              /*   cloak flag  -1 = up 0 = down + = time   */
		int     degrees;            /*   work fields for calculating degrees     */
/*123*/
		int     percent;            /*   Work field for calculating percentages  */
		int     tactical;           /*   Tactical Display damage indicator       */
		int     helm;               /*   Helm Control damage indicator           */
		int     train;              /*   Training screen counter                 */
		int             where;              /*   Hyperspace flag                         */
/*133*/
		TORPEDO         ltorps[MAXTORPS];  /*   Locked on torpedoes                     */
/*142*/
		MISSILE         lmissl[MAXMISSL];   /*   Locked on missiles                       */
/*157*/
		unsigned        decout[MAXDECOY]; /*   decoys out                              */
/*177*/
		unsigned        jammer;          /*   supper time?                            */
		unsigned        freq[3];          /*   sub space, hyper space, planetary       */
/*185*/
		unsigned long   items[NUMITEMS]; /*   all the items                           */
/*237*/
		unsigned        titem;           /*   establishing a colony mode temp item    */
		unsigned hostile;         /*   being hostile flag                      */
		unsigned cantexit;        /*   cannot exit counter                     */
		unsigned repair;          /*   repairs underway flag                   */
		unsigned hypha;           /*   hyper-phasers cooling flag              */
/*247*/
		unsigned firecntl;        /*   fire-control damage flag                */
		byte            destruct;        /*   self destruct countdown                 */
		byte            status;          /*   Status of this record                   */
		byte            cybmine;         /*   I have found me a ship to seek          */
		byte            cybskill;        /*   Cyborg skill level                      */
		byte            cybupdate;       /*   update cyborg counter                   */
		byte            tick;            /* Ship Ticker */
		byte            emulate;         /* permits cybertron emulation */
		byte            minesnear;       /* set when mines are near */
		int             lock;            /* set to the ship we are locked onto*/
		byte            holdcourse;      /* Hold course ticker */
		int		topspeed;
		byte		warncntr;
/*256 ---------------------v  */
		char    filler[512-266-UIDSIZ]; /*   fill to 512 BJ added 20 for name   */
};

/*
#define X1 ptr1->xcoord
#define X2 ptr2->xcoord
#define Y1 ptr1->ycoord
#define Y2 ptr2->ycoord
*/

#define ITEM    struct item
extern
ITEM    {
	unsigned long	qty;                            /* quantity on hand                                                     */
	unsigned			rate;                                           /* rate of manufacture                                          */
	char				sell;                                           /* sell to allies?                                                      */
	unsigned			reserve;                                        /* qty to reserve                                                       */
	unsigned			markup2a;                               /* mark up to allies                                                    */
	unsigned	long	sold2a;                    /* # sold to allies                                                     */
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


#define GALSECT struct galsect

extern
GALSECT {                /* star system data record               */
	int             xsect;                                  /* sector x coord                       */
	int             ysect;                                  /* sector y coord                       */
	int             plnum;                                  /* always 0 */
	int             type;                /* type of sector */

	int             numplan;             /* number of planetary objects */
	PLNTCOORD       ptab[MAXPLANETS];
	char            filler[512-10-(sizeof(PLNTCOORD)*MAXPLANETS)];          /* fill to 250*/
};

#define GALPLNT struct _galplnt

extern
GALPLNT {                /* star system data record               */
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
	ITEM	items[NUMITEMS]; /* 195 */
	char	beacon[BEACONMSGSZ];
	char	spyowner[UIDSIZ];
	int	technology;
	unsigned long	teamcode;
	char	filler[512-182-(UIDSIZ*2)-(sizeof(ITEM)*NUMITEMS)];         

};

#define GALWORM struct galworm

extern
GALWORM {                /* star system data record               */
	int		xsect;                                  /* sector x coord                       */
	int		ysect;                                  /* sector y coord                       */
	int		plnum;               /* planet number */
	int		type;                /* type of sector */
	COORD		coord;
	int		visible;             /* is the wormhole visible flag */
	COORD		destination;
	char		name[20];
	char		filler[512-30-(sizeof(COORD)*2)];         /* fill to 250*/
};


#define PKEY struct _pkey

extern
PKEY {                /* star system data record key   */
	int             xsect;                                  /* sector x coord                       */
	int             ysect;                                  /* sector y coord                       */
	int             plnum;                                  /* always 0 */
};

#define WORMTAB	struct _wormtab

extern
WORMTAB	{
	COORD	coord;
	COORD dest;
};


#define MAILKEY struct _mailkey

extern
MAILKEY {
	  char	userid[UIDSIZ];
	  int		class;
	  long	msgno;	
		};

#define MAIL struct _mail

extern
MAIL    {
	char            userid[UIDSIZ];
	int             class;
	int             type;
	int             stamp;
	char            dtime[20];
	char            topic[30];
	char            string1[80];
	char            name1[25];
	char            name2[25];
	int             int1;
	int             int2;
	int             int3;
	long            long1;
	long            long2;
	long            long3;
	char            filler[253-200-UIDSIZ];                /* fill to 256 BJ added 20 for name*/
	};

#define MAILSTAT struct _mailstat
extern
MAILSTAT	{
	char            userid[UIDSIZ];
	int             class;
	int             type;
	int             stamp;
	char            dtime[20];
	char            topic[30];
	char            name1[25];
	int             int1;
	int             int2;
	unsigned long            cash;                /* cash on hand                  */
	unsigned long            debt;                /* amount you owe                      */
	unsigned long            tax;                 /* amount of tax collected             */
	unsigned long            itemqty[NUMITEMS];
	char            filler[253-97-(UIDSIZ)-(sizeof(long)*NUMITEMS)];         /* fill to 250 BJ changed size for name*/
	};



#define SHPKEY struct _shpkey

extern
SHPKEY {
	  char               userid[UIDSIZ];
	  int                        shipno;
		};

#define SHIP    struct _ship

extern
SHIP    {
		char				*typename;
		char				*shipname;
		unsigned	char	max_shlds;
		unsigned char	max_phasr;
		unsigned	char	max_torps;
		unsigned	char	max_missl;
		unsigned char  has_decoy;
		unsigned char  has_jam;
		unsigned char  has_zip;
		unsigned char  has_mine;
		unsigned char	max_attk;
		unsigned char	max_cloak;
		unsigned int	max_accel;
		unsigned	char	max_warp;
		long				max_tons;
		long				max_price;
		unsigned int	max_points;
		unsigned int	max_type;
		long				scanrange;
		unsigned	char	cybs_can_att;
		unsigned char	lowest_to_attk;
		unsigned char  noclaim;
		unsigned int	tot_to_create;
		unsigned char	tough_factor;
		unsigned int	damfact;
		unsigned int	res_flag_2;
		unsigned int	res_flag_3;

		int				hlpmsg;
		void				(*init_func)	(WARSHP *, int, int);
		void				(*tick_func)	(WARSHP *, int);
		void				(*kill_func)	(WARSHP *, int, WARSHP *);
		void				(*won_func)		(WARSHP *, int, WARSHP *);
		};


#define CYB_TOUGH_0	0
#define CYB_TOUGH_1 	1

struct  	cmd 
			{
			char *command;
			void (*func)();
			byte cando;
			};

#define SHIPTAB struct _shiptab

extern
SHIPTAB {
		int		shipno;
		char		letter;
		char		flag;
		double	dist;
		int		bearing;
		int		heading;
		double	speed;
		};

#define NOSCANTAB       15
#define SCANTAB struct _scantab

extern
SCANTAB {
		SHIPTAB ship[NOSCANTAB];
};


#define S00	struct _s00

extern
S00	{
		char		*name;
		char		*owner;
		int		type;
		double	xcoord;
		double	ycoord;
		int		env;
		int		res;
		};

#define TEAM	struct _team
extern
TEAM	{
		long				teamcode;
		char				teamname[31];
		unsigned int	teamcount;
		unsigned long	teamscore;
		char				password[11];
		char				secret[11];
		int				flag;		
		};


		
#define BEACONTAB struct _beacontab
extern
BEACONTAB	{
	COORD	coord;
	int	plnum;
	char	beacon[BEACONMSGSZ];
	};
		
		
#define METHOD 1


#define KEY  "\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255"

#define SEED "10094056"  /*Choose a unique seed for each application */
/*            ^     ^    */
/*            1     4    */
/*               14 = number of day's test drive is to run   */

struct secure {
    int open_stat;  /* 0=not opened, -1=testdrive, 1=registered */
    int days_run;   /* number of days run as test drive (-1=!testdrive) */
    int days_tot;   /* number of days testdrive is good for (-1=!testdrive) */
    char reg_num[9];   /* Registration number */
    char key[60];
    };


extern struct secure *secure;
struct btvblk *sec_opnbtv(char *datfilnam,int sizostruct,char *seed, int regnum,char *key, int method);

#define FUNC _export

