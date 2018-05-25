#include <stdlib.h>
#include <string.h>
#include "image.h"

typedef unsigned char matrice[IMAGE_HEIGH][IMAGE_WIDTH];
static matrice mat1;

void erase(unsigned char * img_in)
{
	for (int i = 0; i<IMAGE_SIZE;i++)
	{
		img_in[i]=255;
	}
}

int max(int a,int b)
{
	if (a<b)
	{
		return b;
	}
	return a;
}

void square(unsigned char * img_in, int size, int i, int j)
{
	int pos;
	for (int k=0; k<size; k++)
	{
		for(int l=0; l<size; l++)
		{
			pos = (i-k)*IMAGE_WIDTH+j-l;
			if(img_in[pos]!=255)
			{
				if (l==0)
				{
					k=size;
				}
				l=size;
			}
			else
			{
				img_in[pos]=0;
			}
		}
	}
}

void zone_detecting(unsigned char * img_in,unsigned char * img_out, int size)
{
	erase(img_out);
	int prec;
	int v;
	int nouv;
	for (int i=0; i< IMAGE_HEIGH;i++)
	{
		prec = 0;
		for (int j=0; j< IMAGE_WIDTH;j++)
		{
			if(img_in[i*IMAGE_WIDTH+j]==0)
			{
				v=0;
			}
			else
			{
				v=1;
			}
			nouv=max(prec-1,size*v);
			mat1[i][j]=nouv;
			prec=nouv;
		}
	}
	for (int j=0; j< IMAGE_WIDTH;j++)
	{
		prec = 0;
		for (int i=0; i< IMAGE_HEIGH;i++)
		{
			if(mat1[i][j]==0)
			{
				v=0;
			}
			else
			{
				v=1;
			}
			nouv=max(prec-1,size*v);
			prec=nouv;
			if ((nouv==0)&&(i>=size-1)&&(j>=size-1))
			{
				square(img_out, size, i, j);
			}
		}
	}
}