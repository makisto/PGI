#include <stdio.h>
#include <graphics.h>
#define COLOR(r, g, b) (r|(g<<8)|(b<<16)|(3<<24))
#pragma pack(1)

typedef struct TPCXHeaderStruct
{
	unsigned char ID;
	unsigned char Version;
	unsigned char Coding;
	unsigned char BitPerPixel;
	unsigned short XMin;
	unsigned short YMin;
	unsigned short XMax;
	unsigned short YMax;
	unsigned short HRes;
	unsigned short VRes;
	unsigned char Palette[48];
	unsigned char Reserved;
	unsigned char Planes;
	unsigned short BytePerLine;
	unsigned short PaletteInfo;
	unsigned short HScreenSize;
	unsigned short VScreenSize;
	unsigned char Filler[54];
} TPCXHeader;

int main(void)
{
	TPCXHeader header;
	FILE *pFile = fopen("cat256.pcx", "rb");
	unsigned char pal[256][3];
	unsigned char id;
	
	fseek(pFile, -256 * 3 - 1, SEEK_END);
	fread(&id, 1, 1, pFile);
	
	if(12 != id)
	{
		printf("FAIL\n");
		exit(1);
	}
	
	fread(pal, sizeof(pal), 1, pFile);
	fseek(pFile, 0, SEEK_SET);
	
	fread(&header, sizeof(header), 1, pFile);
	int x_size = header.XMax - header.XMin + 1;
	int y_size = header.YMax - header.YMin + 1;
	
	printf("BitPerPixel - %d\nPlanes - %d\n", header.BitPerPixel, header.Planes);
	printf("id = %d x_size = %d y_size = %d\n", id, x_size, y_size);
	initwindow(x_size, y_size);
	int total_bytes = header.Planes * header.BytePerLine;
	
	int color;
	byte c, t;
	
	for(int i = 0; i < y_size; i++)
	{
		int j = 0;
		while(j < total_bytes)
		{
			fread(&c, 1, 1, pFile);
			t = c & 0xC0;
			if(t == 0xC0)
			{
				t = c & 0x3F;
				fread(&c, 1, 1, pFile);
				for(int k = 0; k < t; k++)
				{
					if(j < x_size)
					{
						color = RGB(pal[c][0], pal[c][1], pal[c][2]);
						putpixel(j, i, color);
						j++;
					}
				}
			}
			else if(j < x_size)
			{
				color = RGB(pal[c][0], pal[c][1], pal[c][2]);
				putpixel(j, i, color);
				j++;	
			}
		}
	}
	getchar();
	fclose(pFile);
}
