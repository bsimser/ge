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
			     
#include "string.h"
#include "dos.h"
#include "stdio.h"
#include "stdlib.h"


int getreg(char *seed,char *mbbs);

char number[2];
char seed[9];
char bturno[9];

int i;
char k;

static char mask[] ="12332184";

char	tmp_bturno[9];
char	tmp_key[9];


void main(void)
{
printf("REGISTER/2 for Non-Btrieve Methods\n\n");


strcpy(seed,'\0');
while(strlen(seed)!=8){
  printf("The Seed for your module must be 8 characters long!\n\n");
  printf("Enter the SEED for your module : ");
  gets(seed);
  }
while (getreg(seed,bturno));

return;
}

int getreg(char *seed,char *mbbs)
{
char chkrno[9];
char tmpc[3];
int i,val1,val2;
strcpy(mbbs,'\0');
while(strlen(mbbs)!=8){
  printf("The MBBS registration number must be 8 characters long!\n\n");
  printf("Enter the MBBS registration number (or Q to quit): ");
  gets(mbbs);
  if((strlen(mbbs)==1)&&((mbbs[0]=='Q')||(mbbs[0]=='q'))){
    strcpy(mbbs,"QQQQQQQQ");
    }
  }
if(mbbs[0]=='Q')return(0);

strncpy(tmp_bturno,bturno,9);
strncpy(mask,seed,9);

if (strlen(tmp_bturno) != 8)
	{
	printf("\n\nANN:bturno NG %s\n\n",tmp_bturno);
	return(1);
	}

for (i=0;i<8;++i)
	{
	k = tmp_bturno[i]^mask[7-i];
	k = k%10;
	tmp_key[i] = k+48;
	}
tmp_key[8] = 0;

printf("\n\n The registration number for this module is : %s\n",tmp_key);
return(1);
}

