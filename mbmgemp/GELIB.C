
/***************************************************************************
 *                                                                         *
 *   GELIB.C                                                               *
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


#endif

#include "math.h"
#include "majorbbs.h"
#include "message.h"

#include "gemain.h"
#include "geglobal.h"

/**************************************************************************
** Verify system
**************************************************************************/

void c4angle()

{

extern BTVFILE *accbb;
char  nm1[7] = {"XGrlep"};
char  nm2[UIDSIZ];      /*BJ CHANGED TO REFLECT UIDSIZ */
static int hap;
if (hap != 1)
	{
	setbtv(accbb);
	strncpy(nm2,"abcerr",UIDSIZ);
	nm2[0] = nm1[1];
	nm2[1] = nm1[3];
	nm2[2] = nm1[5];
	if (qeqbtv(nm2,0))
		catastro("GE SYSTEM FAULT");
	else
		hap = 1;
	}
}

/**************************************************************************
** Determine the smallest of two complementary angles                    **
**************************************************************************/

unsigned  smallest(a1,a2)
unsigned  a1,a2;

{
int a;

a = abs(a1-a2);

if (a > 180)
	return (360 - a);
else
	return (a);
}


/**************************************************************************
** Generate a random number                                              **
**************************************************************************/

double rndm(mod)
double  mod;
{

static randmax = (double)RAND_MAX;

return(mod*(((double)((unsigned)rand()))/randmax));
}


unsigned int gernd()

{
return(rand());
}

/**************************************************************************
** Calculate ship bearing between two objects                            **
**************************************************************************/

double  cbearing(ptr1, ptr2, heading)
COORD *ptr1,*ptr2;
double heading;
{
double   vector(), absol(), normal();
double   b;

	ptr1->xcoord += .000001;
	ptr1->ycoord += .000001;

	b =  vector(ptr1,ptr2);
/*	sprintf(gechrbuf,"vector = %f",b);
	logthis(gechrbuf);*/

	b =  normal(360 - heading + b);
/*	sprintf(gechrbuf,"normal = %f",b);
	logthis(gechrbuf);*/

	if (b > 180)
		b = b-360;
/*	sprintf(gechrbuf,"+-180 = %f",b);
	logthis(gechrbuf);*/
return (b);

}

/**************************************************************************
** Calculate the distance between two ships                              **
**************************************************************************/

double cdistance(ptr1,ptr2)
COORD *ptr1,*ptr2;
{
double a, b, c;




b = (ptr1->xcoord - ptr2->xcoord);

c = (ptr1->ycoord - ptr2->ycoord);

b = absol(b);

c = absol(c);


a = sqrt((b*b) + (c*c));

return (a);
}

/**************************************************************************
** Calculate the angle from one ship to another                          **
**************************************************************************/

double vector(ptr1,ptr2)
COORD *ptr1, *ptr2;

{
double   a;
double   angleb(),anglec(),normal();

if (ptr1->xcoord >= ptr2->xcoord && ptr1->ycoord <= ptr2->ycoord)
	{
	a = angleb(ptr1,ptr2);
	a = 270.0 - a;
	return(a);
	}
else
if (ptr1->xcoord >= ptr2->xcoord && ptr1->ycoord >= ptr2->ycoord)
	{
	a = angleb(ptr1,ptr2);
	a = 270.0 + a;
	return(a);
	}
else
if (ptr1->xcoord <= ptr2->xcoord && ptr1->ycoord <= ptr2->ycoord)
	{
	a = anglec(ptr1,ptr2);
	a = 180.0 - a;
	return(a);
	}
else
if (ptr1->xcoord <= ptr2->xcoord && ptr1->ycoord >= ptr2->ycoord)
	{
	a = anglec(ptr1,ptr2);
	a = 0.0 + a;
	return(a);
	}


return(99999L);
}

/**************************************************************************
** Calculate the angle from the center to the other ship                 **
**************************************************************************/

double angleb (ptr1, ptr2)
COORD *ptr1, *ptr2;

{

double   da,db,dc, angle;



da = cdistance(ptr1, ptr2);
dc = absol(ptr1->xcoord - ptr2->xcoord);
db = absol(ptr1->ycoord - ptr2->ycoord);

if ((da*dc) > 0)
	angle = (double) acos( ((da*da) + (dc*dc) - (db*db)) / (2*da*dc));
else
	angle = 0;

angle = radtodeg(angle);

return (angle);

}

/**************************************************************************
** Calculate the angle from the center to the other ship                 **
**************************************************************************/

double anglec (ptr1, ptr2)
COORD *ptr1, *ptr2;

{

double   da,db,dc, angle;



da = cdistance(ptr1, ptr2);
dc = absol(ptr1->xcoord - ptr2->xcoord);
db = absol(ptr1->ycoord - ptr2->ycoord);

if ((da*db) > 0)
	angle = (double) acos( ((da*da) + (db*db) - (dc*dc)) / (2*da*db));
else
/* this should not be 0 - figure out what rad(360) is. */
	angle = 0;

angle = radtodeg(angle);

return (angle);

}

/**************************************************************************
** Bring an angle back into the range 0 - 360                            **
**************************************************************************/

double normal (angle)
double   angle;
{


if (angle < 0)
	{
	angle = normal(angle+360);
	}

if (angle >= 360)
	{
	angle = normal(angle-360);
	}
return (angle);
}

/**************************************************************************
** convert degrees to radiuns                                            **
**************************************************************************/

double degtorad(value)
double value;
{
return (value*(PI/180));
}

/**************************************************************************
** convert radiuns to degrees                                            **
**************************************************************************/

double radtodeg(value)
double value;
{
return (value*(180/PI));
}

/**************************************************************************
** double absolute function                                              **
**************************************************************************/

double absol(value)
double value;
{
c4angle();
if (value < 0)
	value = value * -1;

return (value);
}
