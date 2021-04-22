#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXCOLOR16
#define COLOR(r, g, b) (r|(g<<8)|(b<<16)|(3<<24))

#pragma pack (1)
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

unsigned char pal[16][4];

int main()
{	
 	FILE * pFile = fopen("cat16.bmp", "rb");
 	fread(&header, sizeof(header), 1, pFile); 
	 
	int height = header.biHeight;
	int width = header.biWidth;
	int width1 = width;
	int offset = 0;
	
	while(width1 % 4 != 0)
	{
		width1++;
		offset++;
	}
		
	fread(pal, sizeof(pal), 1, pFile);	
	
	unsigned char col; 
	unsigned char col1; 
 	unsigned int color;
	
	initwindow(900, 900);
 	for(int i = height - 1; i >= 0; i--)
 	{		
 		for(int j = 0; j < width1 / 2; j++)
 		{
 			fread(&col, 1, 1, pFile);
			
			col1 = col >> 4;
			color = COLOR(pal[col1][2], pal[col1][1], pal[col1][0]);
			putpixel(j * 2, i, color);	
			
			col1 = col & 0x0F;
			color = COLOR(pal[col1][2], pal[col1][1], pal[col1][0]);
			putpixel(j * 2 + 1, i, color);					
		}
	}	
    fclose(pFile);  	
    getchar();
    
    return 0;
}
