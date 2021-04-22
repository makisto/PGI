#define MAXCOLORTC
#include <math.h>
#include "graphics.h"
#define COLOR(r, g, b) (r|(g<<8)|(b<<16)|(3<<24))

#define pallete_size 16

//0xC0 - первые два старших бита равны 1
//0x3F - 63

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

typedef struct TPaletteStruct
{
  unsigned char Red;
  unsigned char Green;
  unsigned char Blue;
} TPalette;

int delta(TPalette c1, TPalette c2)
{
	return (c1.Red - c2.Red) * (c1.Red - c2.Red) + (c1.Green - c2.Green) * (c1.Green - c2.Green) + (c1.Blue - c2.Blue) * (c1.Blue - c2.Blue);
}

bool is_same_color(TPalette c1, TPalette c2)
{
	c1.Red &= 0xC0;
  	c2.Red &= 0xC0;
  	c1.Blue &= 0xC0;
  	c2.Blue &= 0xC0;
  	c1.Green &= 0xC0;
  	c2.Green &= 0xC0;
  	
 	return c1.Red == c2.Red && c1.Green == c2.Green && c1.Blue == c2.Blue;
}

void decoding(char *buf, int bufSize, char **pImg, int& imgSize)
{
	TPCXHeader *header = (TPCXHeader *) buf;
	buf += sizeof(TPCXHeader);
  	bufSize -= sizeof(TPCXHeader);	
	
	char c;  	  	
  	int i = 0; 
  	int j = 0;
  	int repeater = 0; 
  	int begin_line = 0;
  	int plane_number = 0;    
	   		
  	int planes = header->Planes;
  	int p = header->BytePerLine; 
 	int x_size = header->XMax - header->XMin + 1;
  	int y_size = header->YMax - header->YMin + 1;
  	int total_bytes = planes * header->BytePerLine;
  	
  	imgSize = y_size * total_bytes;
  	*pImg = new char[imgSize];

  	while (j < imgSize) 
  	{
    	if (repeater > 0) 
		{ 
    		(*pImg)[j] = c;
     		j += planes;
      		p--;
      		repeater--;
    	} 
		else 
		{
     		if ((buf[i] & 0xC0) != 0xC0) 
	  		{ 
        		(*pImg)[j] = buf[i];
       			j += planes;
        		i++;
        		p--;
      		} 
			else 
			{
        		repeater = buf[i] & 0x3F; 
        		i++;
        		c = buf[i];
        		i++;
      		}
    	}
    	
    	if (p == 0) 
		{ 
      		p = header->BytePerLine;
      		plane_number++; 
      		if (plane_number < planes)
      		{
        		j = begin_line + plane_number;      			
			}
      		else 
		  	{
        		j -= planes - 1;
        		begin_line = j;
        		plane_number = 0;
        		repeater = 0;
      		}
    	}
 	}
}

void show_true_picture(char *buf, int w, int h, int x, int y)
{
	int k = 0;
	int color = 0;
	
  	for (int i = 0; i < h; i++)
 	{
     	for (int j = 0; j < w; j++) 
		{
    		k = (i * w + j) * 3;
    		color = RGB(buf[k], buf[k + 1], buf[k + 2]);
            putpixel(x + j, y + i, color);  
    	} 	
	}
}

int repeat_byte(char *img, int width, int pos)
{
	int rep = 1;
  	int i = pos + 1;
  	while (i % width != 0 && rep < 63 && img[i] == img[pos]) 
  	{
		rep++;
		i++;
 	}
  
  	return rep;
}

void create_file(char *img, int imgSize, TPCXHeader *header, const char *fileName)
{
  	int i = 0;
  	int p = 0;
  	int check = 0;
  	int added_colors = 0;
  	bool is_found_same_color; 
  	int same_colors_counter = 0;
	int width = header->XMax - header->XMin + 1;
	 
	TPalette color;
	
	int diff;
	int min_diff; 
		
	int same_colors[pallete_size];
	TPalette palette[pallete_size];
		      	
  	for (int i = 0; i < pallete_size; i++) 
  	{
  		palette[i].Red = 0;
    	palette[i].Green = 0;
    	palette[i].Blue = 0;
    	same_colors[i] = 0;
  	}
  	
  	for (int i = 0; i < imgSize; i += 3) 
	{
    	color = *((TPalette *) &img[i]);
    	is_found_same_color = false; 
    	for (int j = 0; j < pallete_size; j++)
    	{
        	if (is_same_color(color, palette[j])) 
			{
	        	is_found_same_color = true;
	        	break;
      		}  		
		}
   		if (is_found_same_color)
   		{
   		 	continue;
		}
		
		same_colors_counter = 0;
	    for (int k = i + 3; k < imgSize; k += 3)
	    {
	    	if (is_same_color(color, *((TPalette *) &img[k])))
	    	{
	    		same_colors_counter++;
			}	    	
		}
	    if (added_colors < pallete_size)
		{
	    	palette[added_colors] = color;
	      	same_colors[added_colors] = same_colors_counter;
	     	added_colors++;
	    } 
		else 
		{
			check = -1;
	      	min_diff = INT_MAX;
	      	for (int j = 0; j < pallete_size; j++)
	      	{
				if (same_colors_counter > same_colors[j]) 
				{
		        	diff = delta(color, palette[j]);
		          	if (diff < min_diff) 
				  	{
		        		min_diff = diff;
		            	check = j;
		          	}
		    	}
		  	}
		  	
	      	if (check != -1) 
			{
	        	palette[check] = color;
	        	same_colors[check] = same_colors_counter;
	      	}
		}
	}
	
 	memcpy(header->Palette, palette, sizeof(palette));
 	header->BitPerPixel = 4;
  	header->Planes = 1;
	 
  	FILE *f = fopen(fileName, "wb");
  	fwrite(header, sizeof(*header), 1, f);

  	for (int i = 0; i < imgSize; i += 3) 
	{
		color = *((TPalette *) &img[i]);
    	min_diff = INT_MAX;
    	for (int j = 0; j < pallete_size; j++) 
		{
      		diff = delta(color, palette[j]);
      		if (diff < min_diff) 
			{
        		min_diff = diff;
       			check = j;
     		}
    	}
    	img[p] = check;
    	p++;
 	}
  	
 	while (i < imgSize / 3) 
	{
    	check = repeat_byte(img, width, i);
    	if (check == 1) 
		{		
      		fwrite(&img[i], 1, 1, f);
  		}
    	else
		{ 
      		char r = 0xC0 | check;
      		fwrite(&r, 1, 1, f);
      		fwrite(&img[i], 1, 1, f);
    	}
    	i += check;
  	}
  	
  	fclose(f);
}

void show_file(const char *fileName, int x, int y)
{  	
	TPCXHeader header;
	TPalette palette[pallete_size];
  	FILE *f = fopen(fileName, "rb");
  	
  	fseek(f, pallete_size, SEEK_SET);
 	fread(palette, sizeof(palette), 1, f); 
 	fseek(f, 0, SEEK_SET);
  	fread(&header, sizeof(header), 1, f); 
  	int x_size = header.XMax - header.XMin + 1;
  	int y_size = header.YMax - header.YMin + 1;

	int color;
  	int totalBytes = header.Planes * header.BytePerLine;
  	byte b, t;
  	for (int i = 0; i < y_size; i++) 
  	{
		int j = 0;
    	while (j < totalBytes) 
		{
    		fread(&b, 1, 1, f); 
      		t = b & 0xC0;
      		if (t == 0xC0) 
			{
        		t = b & 0x3F;
        		fread(&b, 1, 1, f); 
        		for (int k = 0; k < t; k++) 
				{ 
          			if (j < x_size)
          			{
          				color = RGB(palette[b].Red, palette[b].Green, palette[b].Blue);
             			putpixel(x + j, y + i, color);  
						j++;       			
					}
        		}
      		}
      		else 
	  		{ 
    			if (j < x_size)
    			{
          			color = RGB(palette[b].Red, palette[b].Green, palette[b].Blue);
             		putpixel(x + j, y + i, color);  
		        	j++;    			
				}
      		}
    	}
	}
  	fclose(f);
}

int main(void)
{
	char *img;
  	int imgSize;
  	
  	FILE *inFile = fopen("cat_true.pcx", "rb");
  	fseek(inFile, 0, SEEK_END);
  	int bufSize = ftell(inFile);
  	
  	char *buf = new char[bufSize];
  	fseek(inFile, 0, SEEK_SET);
  	fread(buf, 1, bufSize, inFile);
  	  	
 	decoding(buf, bufSize, &img, imgSize);
 	fclose(inFile);
 	
  	TPCXHeader *header = (TPCXHeader *) buf;
 	int w = header->XMax - header->XMin + 1;
  	int h = header->YMax - header->YMin + 1;
  	
  	initwindow(w * 2 + 20, h + 20);
  	
  	show_true_picture(img, w, h, 5, 5);
  	
 	create_file(img, imgSize, header, "cat_16_new.pcx");
 	show_file("cat_16_new.pcx", w + 15, 5);
 	
 	getchar();
}

