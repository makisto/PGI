#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <graphics.h>

#define MAXCOLOR256
#define COLOR(r, g, b) (r|(g<<8)|(b<<16)|(3<<24))
#define K 0.5

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
} header, logo_header;

#pragma pack (1)
typedef struct RGBquad 
{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
} RGBQ;

RGBQ *logo;

void insert_logo(unsigned char *buf, int i, int j)
{
	RGBQ logo_pixel = logo[i * logo_header.biWidth + j];
	if (logo_pixel.rgbBlue > 200 && logo_pixel.rgbGreen > 200 && logo_pixel.rgbRed > 200) 
	{
		return;
	}
	buf[0] = (buf[0] * K) + (logo_pixel.rgbBlue * (1 - K));
	buf[1] = (buf[1] * K) + (logo_pixel.rgbGreen * (1 - K));
	buf[2] = (buf[2] * K) + (logo_pixel.rgbRed * (1 - K));
}

int main(void)
{	
 	FILE *pFile = fopen("cat_true.bmp", "rb");
 	FILE *logo_file = fopen("logo.bmp", "rb");
 	
 	fread(&header, sizeof(header), 1, pFile); 	
 	fread(&logo_header, sizeof(logo_header), 1, logo_file);
	
	logo = new RGBQ[3 * logo_header.biHeight * logo_header.biWidth];
	
	unsigned char buf[3];
	for(int i = 0; i < logo_header.biHeight; i++)
	{
		for(int j = 0; j < logo_header.biWidth; j++)
		{
			fread(buf, 3, 1, logo_file);
			int index = i * logo_header.biWidth + j;
			logo[index].rgbBlue = buf[0];
			logo[index].rgbGreen = buf[1];
			logo[index].rgbRed = buf[2];
		}
	}
        
    unsigned int color;
    unsigned char col[3];
    
    initwindow(header.biWidth, header.biHeight);	
	for (int i = header.biHeight - 1; i >= 0; i--) 
	{
		for (int j = 0; j < header.biWidth; j++) 
		{
			fread(col, 1, 3, pFile);
			if (i > 100 && i < logo_header.biHeight && j > 100 && j < logo_header.biWidth) 
			{
				insert_logo(col, i, j);
			}
			color = COLOR(col[2], col[1], col[0]);
			putpixel(j, i, color);	
		}
	}
	fclose(pFile);
	fclose(logo_file);
	getchar();
    return 0;
}
