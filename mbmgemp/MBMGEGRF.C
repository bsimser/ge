/***************************************************************************
 *   MBMGEGRF.C                                                            *
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
 

#include <fastgraf.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <mem.h>
#include	<time.h>
#include <ctype.h>
#include "endecode.h"

/* define this option to create the evaluation version of this program
#define	EVALUATION	1*/

/* define this option on to create the registration encoder for this program
#define	REGCODER		1*/

#define	VERSION	"1.0B"
#define	EVERSION "1.0"


/************************** P R O T O T Y P E S *****************************/

int	file_read(FILE *hdl, char *buf, int flg, char *key);
void	parse_input(char *);
void main(int,char **);
int	trans_x(float);
int	trans_y(float);
void	clrscrx(void);
int	delay(int d);
int	regcheck(void);
void	sparkle(int x, int y, int color,int size);
void	spark2(void);

/************************** G L O B A L S *****************************/

FILE	*hdl;
#define	MAX_BUFSZ 1024
char buffer[MAX_BUFSZ];
char	chrbuf[256];

char	gifname[40];
char	pcxname[40];

int	margc;
char	*margv[32];

/*globals */
int	worm_flg = 0;
int	sect_flg = 1;
int	gif_flg	=	0;
int	pcx_flg	=	0;
int	line_flg	=  0;
int	bypass_flg	=  0;

fpos_t	pos;

float fl_x, fl_y, fl_xt, fl_yt;

int	min_x_sect = 0;
int	max_x_sect = 0;
int	min_y_sect = 0;
int	max_y_sect = 0;

float	fl_x_width, fl_y_width;
float	fl_x_div,	fl_y_div;
float fl_shift_x,	fl_shift_y;

float	fl_x_select= 0.0, fl_y_select=0.0;

#define REGMARK struct _regmark

REGMARK
	{
	long	cksum1;
	char	username[41];
	char	serialno[9];
	long	cksum2;
	};

REGMARK	regmark;

void main(int argc,char **argv)
{

char c;
int	correct;
unsigned long	serial;

int area;
int color, old_color;
int max_x, max_y;
float fl_max_x, fl_max_y;

int new_mode, old_mode;
int x, y, xt,yt;

float zoom = .9;

int	i;

char	*tc;

char regno[10];

time_t bintime;

clrscrx();

#ifdef REGCODER

correct = 0;

randomize();

for(i=0;i<8;++i)
	{
	regmark.serialno[i] = '0'+random(10);
	}
regmark.serialno[9] = 0;

while(!correct)
	{
	gotoxy(1,5);
	printf("                                                                   ");
	printf("                                                                   ");
	gotoxy(1,5);
	printf("Enter the Users full name:[                                       ]");
	gotoxy(28,5);
	gets(buffer);
	strncpy(regmark.username,buffer,sizeof(regmark.username));
	regmark.username[40] = 0;
	gotoxy(1,5);
	printf("                                                                   ");
	gotoxy(1,5);
	printf("Register to %s\r\nCorrect (Y/N)? ",regmark.username);
	c = 0;
	while (c != 'Y' && c != 'N')
		{
		c = getche();
		c = toupper(c);
		}
	if (c == 'Y')
		{
		correct = 1;
		}
	}

printf("\r\nSerial # %s registered to %s\r\n",regmark.serialno,regmark.username);

hdl = fopen("mbmgegrf.log","at+");
	
if(hdl != (FILE *)0)
	{
	time(&bintime);
	strcpy(chrbuf,ctime(&bintime));
	chrbuf[24]=0;
	fprintf(hdl,"[%s] %s %s\n",chrbuf,regmark.serialno,regmark.username);
	fclose(hdl);
	}

if (makeregmark())
	printf("\r\nRegistration Mark Successfull!\r\n");
else
	printf("\r\nRegistration Mark FAILED!!!!\r\n");


return;

#endif

#ifndef EVALUATION

if (regcheck() && regmark.cksum1 == regmark.cksum2)
	{
	gotoxy(1,5);
	printf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\r\n");
	printf("³Serial # %8s                                                       ³\r\n",regmark.serialno);
	printf("³Registered to: %-40s                 ³\r\n",regmark.username);
	printf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\r\n");
	delay(4000);
	}
else
	{
	printf("\r\nError 1: Installation Error\r\n");
	printf("Please Re-install.\r\n");
	return;
	}



#endif

#ifdef EVALUATION

gotoxy(1,5);
printf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\r\n");
printf("³Please Read!                                                            ³\r\n");
printf("³This is an Evaluation Version of the Galactic Empire Starmap Utility.   ³\r\n");
printf("³You are granted a limited license to use this software for 30 days for  ³\r\n");
printf("³evaluation purposes only. You may give unmodified copies to others for  ³\r\n");
printf("³their evaluation. After the evaluation period you must either purchase  ³\r\n");
printf("³a registered copy of this software or cease using it. The registration  ³\r\n");
printf("³price is only $15.00. Upon registering you will receive updates and     ³\r\n");
printf("³access to additional features. Besides, its the right thing to do!      ³\r\n");
printf("³                                                                        ³\r\n");
printf("³To order print the document ORDER.FRM included in the ZIP or contact us.³\r\n");
printf("³                                                                        ³\r\n");
printf("³M.B.Murdock & Associates            Office: (813) 545-8050              ³\r\n");
printf("³P.O. Box 2194                       Fax:    (813) 541-5876              ³\r\n");
printf("³Pinellas Park, Fl. 34665            BBS:    (813) 544-3372              ³\r\n");
printf("³                                            (813) 541-5876              ³\r\n");
printf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\r\n");


y = 23;
for (i=0;i<15;++i)
	{
	gotoxy(1,y);
	printf("This delay in EVALUATION VERSION only... %d - Register it Soon!",i);
	delay(1000);
	}

for(i=0;i<80;i+=4)
	{
	delay(50);
	gotoxy(i,y);
	printf(" ");
	}
for(i=1;i<80;i+=4)
	{
	delay(50);
	gotoxy(i,y);
	printf(" ");
	}

for(i=2;i<80;i+=4)
	{
	delay(50);
	gotoxy(i,y);
	printf(" ");
	}

for(i=3;i<80;i+=4)
	{
	delay(50);
	gotoxy(i,y);
	printf(" ");
	}

gotoxy(0,23);

#endif

min_x_sect = 0;
max_x_sect = 0;
min_y_sect = 0;
max_y_sect = 0;


/* get args */
/*
for (i=0;i<argc;++i)
	printf("Argv#%d [%s]\r\n",i,argv[i]);
*/

strcpy(gifname,"MBMGEGRF.GIF");
strcpy(pcxname,"MBMGEGRF.PCX");

for (i=1;i<argc;++i)
	{
	if (strlen(argv[i]) > 1 && argv[i][0] == '-')
		{
		argv[i][1] = toupper(argv[i][1]);
		if (argv[i][1] == 'Z')
			{
			zoom = atof(&argv[i][2]);
			if (zoom > 100.0 || zoom <= 0.0)
				zoom = 1.0;
			}
#ifndef EVALUATION

		if (argv[i][1] == 'X')
			{
			fl_x_select = atof(&argv[i][2]);
			}

		if (argv[i][1] == 'Y')
			{
			fl_y_select = atof(&argv[i][2]);
			}
#endif

		if (argv[i][1] == 'W')
			{
			worm_flg = 1;
			}

		if (argv[i][1] == 'S')
			{
			sect_flg = 0;
			}

		if (argv[i][1] == 'P')
			{
			pcx_flg = 1;
			}

		if (argv[i][1] == 'L')
			{
			line_flg = 1;
			}

		if (argv[i][1] == 'G')
			{
			gif_flg = 1;
			}

		if (argv[i][1] == 'B')
			{
			bypass_flg = 1;
			}

		if (argv[i][1] == 'N')
			{
			if (strlen(&argv[i][2]) > 0)
				{
				strcpy(gifname,&argv[i][2]);
				strcat(gifname,".GIF");
				strcpy(pcxname,&argv[i][2]);
				strcat(pcxname,".PCX");
				}
			}

		if (argv[i][1] == '?')
			{
			printf("\r\nUSAGE:\r\n");
			printf("   MBMGEGRF [-options]\r\n");
			printf("    -znn.nn         Adjust Zoom Factor\r\n");
			printf("    -w              Turn wormhole traces on\r\n");
			printf("    -s              Force Sector grids off\r\n");
			printf("    -p              Generate PCX output (registered ver only)\r\n");
			printf("    -g              Generate GIF output (registered ver only)\r\n");
			printf("    -b              Bypass keystroke pauses\r\n");
			printf("    -l              Draw Scale Lines\r\n");
			printf("    -x[-]nnnn.nn    Center on X sector (registered ver only)\r\n");
			printf("    -y[-]nnnn.nn    Center on Y sector (registered ver only)\r\n");
			printf("    -nFILENAME      Alternate Name of Output File (8 char max)\r\n");

			printf("\r\n");
			printf("Examples:\r\n");
			printf("   mbmgegrf -z40 -w -g\r\n");
			printf("    generates a good zoom of Sector 0 0\r\n");
			printf("   mbmgegrf -z.9 -g\r\n");
			printf("    generates a total map of the galaxy\r\n");
			printf("   mbmgegrf -z.9 -g -nstarmap\r\n");
			printf("    generates GIF to STARMAP.GIF\r\n");
			return;
			}
		}
	}
	
hdl = fopen("MBMGEPLT.MAP","rb");
if (hdl == (FILE *)0)
	{
	printf("\r\nError 2: MBMGEPLT.MAP not found!\r\n");
	return;
	}

fgets(buffer,sizeof(buffer),hdl);
fgets(buffer,sizeof(buffer),hdl);
fgets(buffer,sizeof(buffer),hdl);
if (strstr(buffer,"Version:"))
	{
	if (strstr(buffer,EVERSION)==NULL)
		{
		printf("\r\nError 3: Incompatible Version of MBMGEGRF.EXE!\r\n");
		printf("Please ask your Sysop for the correct version.\r\n");
		return;
		}
	}
else
	{
	printf("\r\nError 4: Corrupted MBMGEPLT.MAP file!\r\n");
	printf("Please inform your Sysop of the problem.\r\n");
	return;
	}		

file_read(hdl,buffer, 1, "10293847");

strcpy(regno,buffer);

file_read(hdl,buffer,1,regno);

if(strstr(buffer,"MaRkEr")==NULL)
	{
	printf("\r\nError 5: Corrupted MBMGEPLT.MAP file!\r\n");
	printf("Please inform your Sysop of the problem.\r\n");
	return;
	}

fgetpos(hdl,&pos);

while (file_read(hdl, buffer, 1, regno))
	{
	parse_input(buffer);

	if (strstr(margv[0],"SECTOR"))
		{
		x = atoi(margv[1]);
		y = atoi(margv[2]);

/*		if (x < min_x_sect)
			min_x_sect = x;*/

		if (abs(x) > max_x_sect)
			max_x_sect = abs(x);

/*		if (y < min_y_sect)
			min_y_sect = y; */

		if (abs(y) > max_y_sect)
			max_y_sect = abs(y);
		}
	}


min_x_sect = max_x_sect*-1;
min_y_sect = max_y_sect*-1;

/* make sure our x and y conform */
if (fl_x_select > (float)max_x_sect 
	|| fl_x_select < (float)min_x_sect)
	fl_x_select = 0.0;

if (fl_y_select > (float)max_y_sect 
	|| fl_y_select < (float)min_y_sect)
	fl_y_select = 0.0;

/*
sprintf(chrbuf,"DEBUG: maxx:%d minx:%d maxy:%d miny:%d\r\n",
	max_x_sect,
	min_x_sect,
	max_y_sect,
	min_y_sect);

printf(chrbuf);


i = getchar();
*/


/***************************** BEGIN GRAPHICS ****************************/

old_mode = fg_getmode();
new_mode = fg_bestmode(640,480,1);
if (new_mode == -1)
	{
	printf("\r\nError 6: VGA Adaptor not detected!\r\n");
	printf("MBMGEGRF.EXE requires a VGA adaptor to be used.\r\n");
	return;
	}

fg_setmode(new_mode);

max_x = fg_getmaxx()-1;
max_y = fg_getmaxy()-1;

max_x = max_x - 0;
max_y = max_y - 0;

fl_x_width = (float)((min_x_sect*-1) + max_x_sect);
fl_y_width = (float)((min_y_sect*-1) + max_y_sect);

/*fl_shift_x = fl_x_width/2.0;
fl_shift_y = fl_y_width/2.0;*/

fl_shift_x = min_x_sect*-1;
fl_shift_y = min_y_sect*-1;

fl_shift_x /= zoom;
fl_shift_y /= zoom;

fl_shift_x += (fl_x_select*-1);
fl_shift_y += (fl_y_select*-1);

fl_x_div = (fl_x_width/(float)max_x);
fl_y_div = (fl_y_width/(float)max_y);

fl_x_div /= zoom;
fl_y_div /= zoom;

/* load and draw PCX logo */
if (!bypass_flg)
	{
	fg_showpcx("mbmgegrf.x01",0);

	while (!kbhit())
		{
		if (color == 15)
			color = 1;
		else
			color = 15;
		fg_setcolor(color);
		if (!random(3))
			{
			sparkle(78,41,color,2);
			delay(random(100));
			}
		if (!random(3))
			{
			sparkle(366,289,color,2);
			delay(random(100));
			}
		if (!random(3))
			{
			sparkle(602,241,color,2);
			delay(random(100));
			}
		};
	
	fg_setcolor(0);
	fg_fadeout(5);
	}

/*draw box*/
fg_setcolor(15);

fg_move(0,0);
fg_draw(0,max_y);
fg_draw(max_x,max_y);
fg_draw(max_x,0);
fg_draw(0,0);

fsetpos(hdl, &pos);

while (file_read(hdl, buffer, 1, regno))
	{
	parse_input(buffer);
	if (strstr(margv[0],"SECTOR"))
		{
		fl_x = (float)atoi(margv[1]);
		fl_y = (float)atoi(margv[2]);

		x = trans_x(fl_x);
		y = trans_y(fl_y);

		fl_x += 1.0;
		fl_y += 1.0;

		xt = trans_x(fl_x);
		yt = trans_y(fl_y);

		color = 8;
		
		fg_setcolor(color);

		if (sect_flg && (xt-x)>5)
			{
/*			fg_move(x,y);
			fg_dash(xt,y,0xaaaa);
			fg_dash(xt,yt,0xaaaa);
			fg_dash(x,yt,0xaaaa);
			fg_dash(x,y,0xaaaa);*/

			fg_box(x,xt,y,yt);
			}
		}
	else
	if (strstr(margv[0],"PLANET"))
		{

		fl_x = (float)atof(margv[3]);
		fl_y = (float)atof(margv[4]);

		x = trans_x(fl_x);
		y = trans_y(fl_y);

		color = 11;
		fg_setcolor(color);
		fg_point(x,y);

		}
	else
	if (strstr(margv[0],"WORM"))
		{
		fl_x = (float)atof(margv[3]);
		fl_y = (float)atof(margv[4]);

		x = trans_x(fl_x);
		y = trans_y(fl_y);

		fl_x = (float)atof(margv[7]);
		fl_y = (float)atof(margv[8]);

		xt = trans_x(fl_x);
		yt = trans_y(fl_y);

      color = 13;
      fg_setcolor(color);
      fg_point(x,y);
		fg_move(x,y);

		fg_setcolor(8);
		
		if (worm_flg)
			{
			/* if either end is on the visible screen */
			if (((xt >= 0 && xt <=max_x) && (yt >= 0 && yt <=max_y)) 
			||  ((x >= 0 && x <=max_x) && (y >= 0 && y <=max_y)))
				{
				fg_dash(xt,yt,0x1111);
				}
			}

		}

/*
   x = rand() % max_x + 1;
   y = rand() % max_y + 1;

   area = fg_getpixel(x-1,y-1) + fg_getpixel(x,y-1) + fg_getpixel(x+1,y-1)
        + fg_getpixel(x-1,y)   + fg_getpixel(x,y)   + fg_getpixel(x+1,y)
        + fg_getpixel(x-1,y+1) + fg_getpixel(x,y+1) + fg_getpixel(x+1,y+1);

   if (area == 0) {
      color = rand() % max_color + 1;
      fg_setcolor(color);
      fg_point(x,y);
      left--;
      }
*/
   }




/* redraw box 
fg_setcolor(15);
fg_move(0,0);
fg_draw(0,max_y);
fg_draw(max_x,max_y);
fg_draw(max_x,0);
fg_draw(0,0); */

/* draw sector 0 0 */

fl_x = fl_x_select;
fl_y = fl_y_select;

x = trans_x(fl_x);
y = trans_y(fl_y);

fg_move(x,y);

#ifdef NOTHING

fg_setcolor(15);

fg_drawrel(40,40);
fg_drawrel(80,0);
fg_moverel(-80,-2);
fg_print("Sector 0 0",10);

#endif


if (line_flg)
	{
	fg_setcolor(1);

	/*right*/
	fg_move(x,y);
	fg_drawrel(max_x,0);

	for (i = x;i<max_x;i+=10)
		{
		fg_setcolor(7);
		fg_point(i,y);
		}

	/*left*/
	fg_setcolor(1);
	fg_move(x,y);
	fg_drawrel(0-(max_x),0);

	for (i = x;i>0;i-=10)
		{
		fg_setcolor(7);
		fg_point(i,y);
		}

	/*down*/
	fg_setcolor(1);
	fg_move(x,y);
	fg_drawrel(0,max_y);

	for (i = y;i<max_y;i+=10)
		{
		fg_setcolor(7);
		fg_point(x,i);
		}

	/*up*/
	fg_setcolor(1);
	fg_move(x,y);
	fg_drawrel(0,0-(max_y));

	for (i = y;i>0;i-=10)
		{
		fg_setcolor(7);
		fg_point(x,i);
		}

	/* draw references */
	fg_move(25,y-2);
	sprintf(chrbuf,"Y:%.2f",fl_y_select);
	fg_print(chrbuf,strlen(chrbuf));
	fg_move(x+5,max_y-20);
	sprintf(chrbuf,"X:%.2f",fl_x_select);
	fg_print(chrbuf,strlen(chrbuf));


	}

/* draw title box */
fg_setcolor(1);
fg_boxdepth(20,20);
fg_box(0,max_x,0,max_y);	

fg_setcolor(15);
fg_boxdepth(1,1);
fg_box(0,max_x,0,max_y);	

fg_box(0,200,0,20);
fg_move(3,19);
sprintf(chrbuf,"GALACTIC EMPIRE STARMAP");
fg_print(chrbuf,strlen(chrbuf));

/* display scale */

fg_box(max_x-200,max_x,0,20);
fg_move(max_x-197,19);
sprintf(chrbuf,"Scale: %3.3f %3.3f",fl_x_div*10.0,fl_y_div*10.0);
fg_print(chrbuf,strlen(chrbuf));

/*display copyright */

fg_box(max_x-200,max_x,max_y-20,max_y);
fg_move(max_x-197,max_y-2);

sprintf(chrbuf,"(c) Copyright 1993 MBM&A");
fg_print(chrbuf,strlen(chrbuf));

/* display zoom */

fg_box(0,200,max_y-20, max_y);
fg_move(3,max_y-2);

sprintf(chrbuf,"Zoom Factor %3.2f",zoom);
fg_print(chrbuf,strlen(chrbuf));

#ifndef EVALUATION
if (pcx_flg)
	fg_makepcx(0,max_x,0,max_y,pcxname);

if (gif_flg)
	fg_makegif(0,max_x+1,0,max_y,gifname);
#endif

if (!bypass_flg)
	fg_waitkey();

fg_setmode(old_mode);
fg_reset();
}


void	parse_input(char *buf)
{
int i,j,l;

margv[0] = &buf[0];

l = strlen(buf);

for (i=0,j=0; i<l; ++i)
	{
	if (buf[i] == '|')
		{
		buf[i] = 0;
		j++;
		margv[j] = &buf[i+1];
		}
	}
margc = j;
}

int trans_x(float x)
{
return ((int)(((x+fl_shift_x)/fl_x_div)));
}

int trans_y(float y)
{
return ((int)(((y+fl_shift_y)/fl_y_div)));
}


void clrscrx(void)
{
clrscr();
gotoxy(1,1);
#ifdef EVALUATION
	printf(" MBMGEGRF: Galactic Empire Galaxy Graphing Utility V%s \r\n",VERSION);
#else
	printf(" MBMGEGRF: Galactic Empire Galaxy Graphing Utility V%s(r) \r\n",VERSION);
#endif
printf("           (c) Copyright 1994 M.B. Murdock & Associates\r\n");

#ifdef EVALUATION
printf("           EVALUATION VERSION\r\n");
#else
#ifndef REGCODER
printf("           Registered Version\r\n");
#endif
#endif

#ifdef REGCODER
printf("           REGISTRATION CODER VERSION\r\n");
#endif

printf("\r\n\r\n\r\n");
return;
}

int	file_read(FILE *hdl, char *buf, int flg, char *key)
{
static int	size;
char	tbuff[1000];

fread(tbuff, sizeof(char), 3, hdl);

if (feof(hdl))
	return(0);

tbuff[3] = 0;

/* find marker - fail if not */
if (strcmp(tbuff,"$$$") !=0)
	return(0);

fread(&size, sizeof(int), 1, hdl);
if (size > 0 && size < MAX_BUFSZ)
	{
	fread(buf,sizeof(char),size,hdl);
	}
else
	{
	return(0);
	}

if (flg)
	{
	bin_decode(buf,size,key);
	buf[size] = 0;
	}

return(1);
}

void	sparkle(int x, int y, int color,int size)
{
int i;

fg_setcolor(color);
for (i=1;i<size;++i)
	{
	fg_point(x+i,y);
	fg_point(x-i,y);
	spark2();
	delay(random(50)+10);
	fg_point(x,y+i);
	fg_point(x,y-i);
	spark2();
	delay(random(50)+10);
	fg_point(x+i,y+i);
	fg_point(x+i,y-i);
	fg_point(x-i,y+i);
	fg_point(x-i,y-i);
	spark2();
	delay(random(50)+10);
	}
spark2();
}

void	spark2(void)
{
int i,col,x,y,c;

static int	first = 1;

#define MAXSTARS 100

static int	xs[MAXSTARS];
static int	ys[MAXSTARS];

#define MAXCOLS 5

int cols[MAXCOLS] = {0,1,9,3,15};

int svcol;

if (first)
	{
	for (i=0;i<MAXSTARS;++i)
		{
		x = random(640);
		y = random(480);
		while (x < 280 && y > 240)
			{
			x = random(640);
			y = random(480);
			if (fg_getpixel(x,y)!=0)
				{
				x = 0;
				}
			}
		xs[i]=x;
		ys[i]=y;
		}
	first = 0;
	}


svcol = fg_getcolor();

for (i=0;i<5;++i)
	{
	x = random(MAXSTARS);
	c = random(MAXCOLS);
	fg_setcolor(cols[c]);
	fg_point(xs[x],ys[x]);
	}
fg_setcolor(svcol);
}

#ifdef NOTHING

int	regcheck(void)
{

int state = 0;

long	pos;

#define BUFFSIZ 512

char	*ptr1,*ptr2;
int	i;
int	patchedit;


patchedit = 0;
hdl = fopen(exename,"r+b");
while (!feof(hdl))
	{
	fread(buffer, sizeof(char), BUFFSIZ, hdl);
	pos = ftell(hdl);
	ptr2 = 0;
	for (i=0,ptr1=buffer;i<BUFFSIZ;++i,++ptr1)
		{
		if (memcmp(ptr1,text,strlen(text))==0)
			{
			ptr2 = ptr1;
			break;
			}
		}
	if (ptr2 != 0)
		{
		pos = pos - (BUFSIZ-i);
		fseek(hdl,pos,SEEK_SET);
		fwrite("MICHAEL B. MURDOCK",sizeof(char),18,hdl);
		patchedit = 1;
		break;
		}
	pos = pos - strlen(text);
	fseek(hdl,pos,SEEK_SET);
	}
fclose(hdl);
return(patchedit);
}

#endif


int	regcheck(void)
{
int i;
long cksum;

hdl = fopen("mbmgegrf.reg","rb");
if (hdl != (FILE *)0)
	{
	fread(&regmark, sizeof(REGMARK), 1, hdl);
	fclose(hdl);
	bin_decode((char *)&regmark,sizeof(REGMARK),"ALPHBETA");
	if (regmark.cksum1 != regmark.cksum2)
		{
		return(0);
		}
	cksum = 0;
	for (i=0;i<(sizeof(regmark.username)+sizeof(regmark.serialno));++i)
		{
		cksum = cksum + (regmark.username[i]*3);
		cksum = cksum ^ 93824763;
		}
	if (cksum != regmark.cksum1)
		{
		regmark.cksum2 = cksum;
		return(0);
		}
	regmark.cksum2 = cksum;
	return(1);
	}
regmark.cksum2 = 79324332;
regmark.cksum1 = 32435567;
return(0);
}

#ifdef	REGCODER

int	makeregmark(void)
{
int i;
long cksum;

hdl = fopen("mbmgegrf.reg","wb");
if (hdl != (FILE *)0)
	{
	cksum = 0;
	for (i=0;i<(sizeof(regmark.username)+sizeof(regmark.serialno));++i)
		{
		cksum = cksum + (regmark.username[i]*3);
		cksum = cksum ^ 93824763;
		}
	regmark.cksum1 = cksum;
	regmark.cksum2 = cksum;
	bin_encode((char *)&regmark,sizeof(REGMARK),"ALPHBETA");
	fwrite(&regmark, sizeof(REGMARK), 1, hdl);
	fclose(hdl);
	return(1);
	}
return(0);
}


#endif

