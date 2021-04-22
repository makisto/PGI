#pragma pack (1)
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

int main()
{	
    FILE * pFile = fopen("cat.bmp", "rb");
 	FILE *fp1 = fopen("cat2.bmp", "wb+");
 	 	
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
 
 	unsigned char *mas = new unsigned char[bmiHeader.biSizeImage];	  	
  	fseek(pFile, header.bfOffBits, SEEK_SET);
 	fread(mas, bmiHeader.biSizeImage, 1, pFile);
 	
 	for(int i = 0; i < bmiHeader.biSizeImage; i += bmiHeader.biBitCount / 8)
 	{
 		unsigned char r = mas[i];
		unsigned char g = mas[i + 1];
		unsigned char b = mas[i + 2];
		
		int col = i / 3 % bmiHeader.biWidth;	
		int row = i / 3 / bmiHeader.biWidth;
		
		if(col < 10 || col > bmiHeader.biWidth - 10)
		{
			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;	
			
			mas[i] = r;
			mas[i + 1] = g;
			mas[i + 2] = b;
		}
		
		if(row < 10 || row > bmiHeader.biHeight - 10)
		{
			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;	
			
			mas[i] = r;
			mas[i + 1] = g;
			mas[i + 2] = b;
		}			
	}	
	fwrite(mas, bmiHeader.biSizeImage, 1, fp1);
    
    fclose(pFile);  	
	fclose(fp1);
    
    return 0;
}
