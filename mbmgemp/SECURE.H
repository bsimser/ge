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
			     
#define METHOD 0

#define KEY "~!@#$%^&*~!@#$%^&*~!@#$%^&*~!@#$%^&*~!@#$%^&*~!@#$%^&*~!@#$"

/* Record Key used in secure process */

#define SEED "19283746"  /*Choose a unique seed for each application */
/*            ^     ^    */
/*            1     4    */
/*               14 = number of day's test drive is to run   */

struct secure {
    int open_stat;  /* 0=not opened, -1=testdrive, 1=registered */
    int days_run;   /* number of days run as test drive (-1=!testdrive) */
    int days_tot;   /* number of days testdrive is good for (-1=!testdrive) */
    char reg_num[9];   /* Registration number */
    char key[UIDSIZ];
    };


extern struct secure *secure;
struct btvblk *sec_opnbtv(char *datfilnam,int sizostruct,char *seed, int regnum,char *key, int method);

