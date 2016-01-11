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
int getreg(char *seed,char *mbbs,int method);
void main(void)
{
char number[2];
char seed[9];
char mbbs[9];
int method=-1;
strcpy(seed,'\0');
while(strlen(seed)!=8){
  printf("The Seed for your module must be 8 characters long!\n\n");
  printf("Enter the SEED for your module : ");
  gets(seed);
  }
while((method<0)||(method>9)){
  printf("The Method number must be between 0 and 9!\n\n");
  printf("Enter the Method Your module uses : ");
  gets(number);
  method=atoi(number);
  }
while (getreg(seed,mbbs,method));

return;
}

int getreg(char *seed,char *mbbs,int method)
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
sprintf(chkrno,"%8ld",(atol(seed))^(atol(mbbs)));

if(method==1){
	  for(i=0; i<8; ++i){
		 tmpc[0]=chkrno[i];
		 tmpc[1]=0;
		 val1=atoi(tmpc);
		 tmpc[0]=seed[i];
		 tmpc[1]=0;
		 val2=atoi(tmpc);
		 itoa(val1+val2,tmpc,10);
		 chkrno[i]=tmpc[strlen(tmpc)-1];
		 }
	  }
printf("\n\n The registration number for this module is : %s\n",chkrno);
return(1);
}
