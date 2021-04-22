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
 	bmiHeader.biWidth = bmiHeader.biHeight;
 	bmiHeader.biHeight = width;
    
 	fwrite(&header, sizeof(BITMAPFILEHEADER), 1, fp1); 	
 	printf("%d %d %d %d %d\n", header.bfType, header.bfSize, header.bfReserved1, header.bfReserved2, header.bfOffBits);    
 	
 	fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp1);
 	printf("%d %d %d %d %d %d %d %d %d %d %d\n", bmiHeader.biSize, bmiHeader.biWidth, bmiHeader.biHeight, bmiHeader.biPlanes, bmiHeader.biBitCount, 
	 bmiHeader.biCompression, bmiHeader.biSizeImage, bmiHeader.biXPelsPerMeter, bmiHeader.biYPelsPerMeter, bmiHeader.biClrUsed, bmiHeader.biClrImportant);
 
 	unsigned char *mas = new unsigned char[bmiHeader.biSizeImage];	  
	unsigned char *new_mas = new unsigned char[bmiHeader.biSizeImage];	
  	fseek(pFile, header.bfOffBits, SEEK_SET);
 	fread(mas, bmiHeader.biSizeImage, 1, pFile);
 	
 	for(int i = 0; i < bmiHeader.biSizeImage; i += bmiHeader.biBitCount / 8)
 	{		
		int col = i / 3 % bmiHeader.biWidth;	
		int row = i / 3 / bmiHeader.biWidth;
		
 		unsigned char r = mas[row * 3 + col * bmiHeader.biHeight * 3];
		unsigned char g = mas[row * 3 + col * bmiHeader.biHeight * 3 + 1];
		unsigned char b = mas[row * 3 + col * bmiHeader.biHeight * 3 + 2];	
		
		new_mas[col * 3 + row * bmiHeader.biHeight * 3] = r;
		new_mas[col * 3 + row * bmiHeader.biHeight * 3 + 1] = g;
		new_mas[col * 3 + row * bmiHeader.biHeight * 3 + 2] = b;			
	}	
	fwrite(new_mas, bmiHeader.biSizeImage, 1, fp1);
    
    fclose(pFile);  	
	fclose(fp1);
    
    return 0;
}
