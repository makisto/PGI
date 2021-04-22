#pragma pack (1)
#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXCOLOR256
#define COLOR(r, g, b) (r|(g<<8)|(b<<16)|(3<<24))

struct head
{
    unsigned short   bfType;
    unsigned int   	 bfSize;
    unsigned short   bfReserved1;
    unsigned short   bfReserved2;
   	unsigned int  	 bfOffBits;

    unsigned int   biSize;
    unsigned int   biWidth;
    unsigned int   biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    unsigned int   biXPelsPerMeter;
    unsigned int   biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} header;

unsigned char pal[256][4];

int main()
{	
 	FILE *pFile = fopen("cat256.bmp", "rb");
 	
 	fread(&header, sizeof(header), 1, pFile); 
	 
	int width = header.biWidth;
 	int height = header.biHeight;
 	int width1 = width;
 	
 	while(width1 % 4 != 0)
 	{
 		width1++;	
	}

	fread(pal, sizeof(pal), 1, pFile);
		
 	initwindow(width, height);	
 	unsigned char col; 
	unsigned int color;
	 	
 	for(int i = height - 1; i >= 0; i--)
 	{		
 		for(int j = 0; j < width1; j++)
 		{
 			fread(&col, 1, 1, pFile);

			color = COLOR(pal[col][2], pal[col][1], pal[col][0]);
			putpixel(j, i, color);		
		}
	}	
    fclose(pFile);  	
    getchar();
    
    return 0;
}
