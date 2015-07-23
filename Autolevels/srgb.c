/*
 * (C) 2012, Roland Baudin <roland65@free.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* 04/05/2012 */
/* Pre-computation of the SRGB transform (direct and inverse) */
/* For 16 bits images (levels between 0 and 65535) */

#include <stdio.h>
#include <stdlib.h>

#include <math.h>



int main(void)
{
	int i;
	double C, L;
	unsigned int Li;
	FILE *fp1, *fp2;
	
	/* SRGB */
	
	fp1=fopen("srgb.txt","w");
	
	for (i=0; i<=65535; i++)
	{
		
		C =i/65535.0;
		
		if (C <= 0.0031308)
			L = 65535.0*(12.92*C);
		
		else
			L=65535.0*((1.0+0.055)*pow(C,1.0/2.4)-0.055);
			
		Li=(unsigned int)round(L);
		if (Li<0)
			Li=0;
		if (Li>65535)
			Li=65535;
			
		fprintf(fp1,"%u,",Li);
		if (fmod(i,32)==0)
			fprintf(fp1,"\n");
		//fprintf(fp1,"%u\n",Li);	
	}
	fclose(fp1);

	/* Inverse SRGB */

	fp2=fopen("invsrgb.txt","w");
	
	for (i=0; i<=65535; i++)
	{
		
		C =i/65535.0;
		
		if (C <= 0.04045)
			L = 65535.0*(C/12.92);
		
		else
			L= 65535.0*pow((C+0.055)/(1.0+0.055),2.4);
			
		Li=(unsigned int)round(L);
		if (Li<0)
			Li=0;
		if (Li>65535)
			Li=65535;
			
		fprintf(fp2,"%u,",Li);
		if (fmod(i,32)==0)
			fprintf(fp2,"\n");
		//fprintf(fp2,"%u\n",Li);	
	}
	fclose(fp2);


}	

