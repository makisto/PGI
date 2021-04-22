#pragma pack (1)
#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SCALE 1.5
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

#pragma pack (1)
typedef struct RGBquad 
{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQ;

RGBQ pal[256];

int main()
{	
 	FILE * pFile = fopen("cat256.bmp", "rb");
 	
 	fread(&header, sizeof(header), 1, pFile); 	
	fread(pal, sizeof(pal), 1, pFile);
	
	int x_center = 900 / 2;
	int y_center = 900 / 2;
	int x_start = x_center - (int)(header.biWidth / 2 * SCALE);
	int y_start = y_center + (int)(header.biHeight / 2 * SCALE);	
	unsigned char rastr[header.biHeight * header.biWidth];
	for (int i = 0; fread(&rastr[i], 1, 1024, pFile) == 1024; i += 1024);
	
 	initwindow(header.biWidth, header.biHeight);
 	unsigned char col; 
	unsigned int color;
	 	
 	int x = 0;
 	int y = 0;
 	for(double i = 0.0; i < header.biHeight; i += 1.0 / SCALE)
 	{		
 		for(double j = 0.0; j < header.biWidth; j += 1.0 / SCALE)
 		{	
		 	RGBQ pix = pal[rastr[((int)i) * header.biWidth + ((int)j)]];							 
			color = COLOR(pix.rgbRed, pix.rgbGreen, pix.rgbBlue);
			putpixel(x_start + x, y_start - y, color);				
			x++;	
		}
		y++;
		x = 0;
	}	
    fclose(pFile);  	
    getchar();
    
    return 0;
}
