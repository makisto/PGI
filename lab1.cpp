#pragma pack (2)
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    unsigned short   bfType;
    unsigned int   	 bfSize;
    unsigned short   bfReserved1;
    unsigned short   bfReserved2;
   	unsigned int  	 bfOffBits;
} BITMAPFILEHEADER;
 
typedef struct
{
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
} BITMAPINFOHEADER;
 
typedef struct 
{
    unsigned char  rgbBlue;
    unsigned char  rgbGreen;
    unsigned char  rgbRed;
    unsigned char  rgbReserved;
} RGBQUAD;

int main()
{	
    FILE * pFile = fopen("cat.bmp", "rb");
 	FILE *fp1 = fopen("cat1.bmp", "wb+");
 	 	
 	unsigned char middle;
	BITMAPFILEHEADER header;
    BITMAPINFOHEADER bmiHeader;
 
 	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile); 	
 	printf("%d %d %d %d %d\n", header.bfType, header.bfSize, header.bfReserved1, header.bfReserved2, header.bfOffBits);
 	
 	fread(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
 	printf("%d %d %d %d %d %d %d %d %d %d %d\n", bmiHeader.biSize, bmiHeader.biWidth, bmiHeader.biHeight, bmiHeader.biPlanes, bmiHeader.biBitCount, 
	 bmiHeader.biCompression, bmiHeader.biSizeImage, bmiHeader.biXPelsPerMeter, bmiHeader.biYPelsPerMeter, bmiHeader.biClrUsed, bmiHeader.biClrImportant);
  	
 	int width = bmiHeader.biWidth;
 	int height = bmiHeader.biHeight;
    
 	fwrite(&header, sizeof(BITMAPFILEHEADER), 1, fp1); 	
 	printf("%d %d %d %d %d\n", header.bfType, header.bfSize, header.bfReserved1, header.bfReserved2, header.bfOffBits);    
 	
 	fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp1);
 	printf("%d %d %d %d %d %d %d %d %d %d %d\n", bmiHeader.biSize, bmiHeader.biWidth, bmiHeader.biHeight, bmiHeader.biPlanes, bmiHeader.biBitCount, 
	 bmiHeader.biCompression, bmiHeader.biSizeImage, bmiHeader.biXPelsPerMeter, bmiHeader.biYPelsPerMeter, bmiHeader.biClrUsed, bmiHeader.biClrImportant);
    
    RGBQUAD **rgb = new RGBQUAD*[height];
    for (int i = 0; i < width; i++) 
	{
        rgb[i] = new RGBQUAD[width];
    }
 
    for (int i = height - 1; i >= 0; i--) 
	{
        for (int j = 0; j < width; j++) 
		{
			fread(&rgb[i][j], sizeof(RGBQUAD), 1, pFile);
			
			middle = (rgb[i][j].rgbBlue + rgb[i][j].rgbGreen + rgb[i][j].rgbRed) / 3;
			
			rgb[i][j].rgbBlue = middle;
			rgb[i][j].rgbGreen = middle;
			rgb[i][j].rgbRed = middle;
			
			fwrite(&rgb[i][j], sizeof(RGBQUAD), 1, fp1);
        }
    }    
    fclose(pFile);  	
	fclose(fp1);
    
    return 0;
}
