#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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

int main(void)
{
	FILE *f1 = fopen("cat_true.bmp", "rb");
	FILE *f2 = fopen("cat_text.bmp", "wb+");
	FILE *text = fopen("text.txt", "r");
	fseek(text, 0L, SEEK_END);
	unsigned long text_file_size = ftell(text);
	rewind(text);

	fread(&header, sizeof(header), 1, f1);
	fwrite(&header, sizeof(header), 1, f2);
			
	unsigned char buf[3];
	unsigned char symbol[3];
	
	unsigned long size = header.biWidth * header.biHeight * 3;
	char text_mas[size / 8];
	
	fread(text_mas, text_file_size, 1, text);	
	
	unsigned char *rastr = new unsigned char[size];
	fseek(f1, header.bfOffBits, SEEK_SET);
	fread(rastr, size, 1, f1);

	char bit = 0;
	int bit_offset = 0;	
	int byte_bit_offset = 0;
	int text_byte_offset = 0;
		
	for(int i = 0; i < size; i++)
	{
		bit = (text_mas[text_byte_offset] >> byte_bit_offset) & 0x1;
		rastr[i] = (rastr[i] & 0xFE) | bit;
		bit_offset++;
		text_byte_offset = bit_offset / 8;
		byte_bit_offset = bit_offset % 8;
	}	
	fwrite(rastr, size, 1, f2);
	
	fclose(f1);
	fclose(text);
	fclose(f2);

	return 0;
}
