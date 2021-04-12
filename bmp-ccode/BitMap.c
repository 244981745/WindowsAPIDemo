#include "BitMap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#define	TEST
#define	BUFFSIZE	3
#define	OBUFFSIZE	2

int	openBitStream(struct BitMapStream *bs)
{
	if(bs == NULL) return -1;
	
	bs->fp	 = fopen("test.bmp","rb");
	if(bs->fp == NULL) return 1;
	
	bs->FileSize		= 0;
	bs->BitMapOffset	= 0;
	bs->BitMapWidth		= 0;
	bs->BitMapHeight	= 0;

	return 0;
}

int closeBitStream(struct BitMapStream *bs)
{
	fclose(bs->fp);
	return 0;
}

int	getBitMapHdr(struct BitMapStream *bs, struct BitMapHdr *fHdr)
{
	assert(bs->fp);
	if(bs->fp != NULL)
	{
		memset((void*)fHdr,'\0',sizeof(struct BitMapHdr));		
		fread((void *)fHdr, 1, 14, bs->fp);
#ifdef TEST
		printf("----------------hdr------------\n");
		printf("bm:\t\t%0.2s\n",fHdr->bfType);
		printf("size:\t\t%2x %2x %2x %2x\n",fHdr->bfSize[0],\
		fHdr->bfSize[1],fHdr->bfSize[2],fHdr->bfSize[3]);
		printf("private:\t%.2x %.2x %.2x %.2x\n",fHdr->bfReserved[0],\
		fHdr->bfReserved[1],fHdr->bfReserved[2],fHdr->bfReserved[3]);
		printf("offset:\t\t%.2x %.2x %.2x %.2x\n",fHdr->bfOffBits[0],\
		fHdr->bfOffBits[1],fHdr->bfOffBits[2],fHdr->bfOffBits[3]);
		printf("-------------------------------\n\n");
#endif

		bs->FileSize		= c2i_32(fHdr->bfSize[0],\
		fHdr->bfSize[1],fHdr->bfSize[2],fHdr->bfSize[3]);	
		bs->BitMapOffset	= c2i_32(fHdr->bfOffBits[0],\
		fHdr->bfOffBits[1],fHdr->bfOffBits[2],fHdr->bfOffBits[3]);
		
		return 0;
	}
	
	return -1;
}

int	getBitMapInfo(struct BitMapStream *bs, struct	BitMapInfo *bi)
{
	if(bs->fp == NULL)return -1;
	fseek(bs->fp,14,SEEK_SET);
	
	fread((void *)bi, 1, 40, bs->fp);

#ifdef	TEST
		printf("----------------info-----------\n");
		printf("info size:\t%.2x %.2x %.2x %.2x\n",bi->infoSize[0],\
		bi->infoSize[1],bi->infoSize[2],bi->infoSize[3]);
		printf("map width:\t%.2x %.2x %.2x %.2x\n",bi->mapWidth[0],\
		bi->mapWidth[1],bi->mapWidth[2],bi->mapWidth[3]);
		printf("map height:\t%.2x %.2x %.2x %.2x\n",bi->mapHeight[0],\
		bi->mapHeight[1],bi->mapHeight[2],bi->mapHeight[3]);
		
		printf("map wm:\t\t%.2x %.2x\n",bi->mapPlanes[0],bi->mapPlanes[1]);
		printf("map bit/pixel:\t%.2x %.2x\n",bi->pixelBitCount[0],bi->pixelBitCount[1]);
		
		printf("map compress:\t%.2x %.2x %.2x %.2x\n",bi->mapcompress[0],\
		bi->mapcompress[1],bi->mapcompress[2],bi->mapcompress[3]);
/*		压缩说明： 
		0 - 不压缩 (使用BI_RGB表示) 
		1 - RLE 8-使用8位RLE压缩方式(用BI_RLE8表示) 
		2 - RLE 4-使用4位RLE压缩方式(用BI_RLE4表示) 
		3 - Bitfields-位域存放方式(用BI_BITFIELDS表示)	
*/
		printf("map size:\t%.2x %.2x %.2x %.2x\n",bi->mapSizeImage[0],\
		bi->mapSizeImage[1],bi->mapSizeImage[2],bi->mapSizeImage[3]);
		
		printf("map x:\t\t%.2x %.2x %.2x %.2x\n",bi->mapXPelsPerMeter[0],\
		bi->mapXPelsPerMeter[1],bi->mapXPelsPerMeter[2],bi->mapXPelsPerMeter[3]);
		printf("map y:\t\t%.2x %.2x %.2x %.2x\n",bi->mapYPelsPerMeter[0],\
		bi->mapYPelsPerMeter[1],bi->mapYPelsPerMeter[2],bi->mapYPelsPerMeter[3]);		
		printf("clrusd:\t\t%.2x %.2x %.2x %.2x\n",bi->mapClrUsed[0],\
		bi->mapClrUsed[1],bi->mapClrUsed[2],bi->mapClrUsed[3]);	
		printf("clrimp:\t\t%.2x %.2x %.2x %.2x\n",bi->mapClrImportant[0],\
		bi->mapClrImportant[1],bi->mapClrImportant[2],bi->mapClrImportant[3]);		

		printf("-------------------------------\n");
#endif

	bs->BitMapWidth		= c2i_32(bi->mapWidth[0],\
		bi->mapWidth[1],bi->mapWidth[2],bi->mapWidth[3]);
	bs->BitMapHeight	= c2i_32(bi->mapHeight[0],\
		bi->mapHeight[1],bi->mapHeight[2],bi->mapHeight[3]);
	bs->BitPerPixel		= c2i_16(bi->pixelBitCount[0],bi->pixelBitCount[1]);
	bs->MapDataSize		= c2i_32(bi->mapSizeImage[0],\
		bi->mapSizeImage[1],bi->mapSizeImage[2],bi->mapSizeImage[3]);

	return 0;
}

static unsigned char oxchar[5];
unsigned char mast[]={
	'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'
};

void hex(unsigned char c)
{
	oxchar[0] = '0';
	oxchar[1] = 'x';
 	oxchar[2] = mast[c>>4];
	oxchar[3] = mast[c&0xf];
 	oxchar[4] = ',';
}

int _RGB888_RGB565(struct BitMapStream *bs)
{
	FILE *Out;
	char	buff[BUFFSIZE];
	char	obuff[OBUFFSIZE];

	int ret = 0;

	Out = fopen("image.h","wb");
	if(Out == NULL) return -1;
	
	
	fseek(bs->fp, bs->BitMapOffset, SEEK_SET);

	fputs("const unsigned char gImage[] = {",Out);
	
	while( feof(bs->fp ) == 0 )
	{

		ret = fread((void *)buff, 1, BUFFSIZE, bs->fp);

		memset((void*)obuff,'\0', OBUFFSIZE);
		memset((void *)oxchar,'\0',5);
		obuff[0]	= (buff[0]&0xf8);
		obuff[0]	|= buff[1]>>5;

		obuff[1]	= (buff[1]<<3) &0xe0;
		obuff[1]	|= (buff[2]>> 3) &0x1f;

		hex(obuff[0]);
		fwrite((void*)oxchar,5,1, Out);
		hex(obuff[1]);
		fwrite((void*)oxchar,5,1, Out);		
			
	}
	
	fseek(bs->fp,-1,SEEK_CUR); 
	fputs("};",Out);
	printf("ftell = %d\n",ftell(bs->fp));
	
}

int _RGB888_RGB666(struct BitMapStream *bs)
{
	FILE 	*Out;
	char	buff[30];

	int ret = 0,i,j,matrix;

	Out = fopen("image.h","wb");
	if(Out == NULL) return -1;
	
	
	fseek(bs->fp, bs->BitMapOffset, SEEK_SET);

	fprintf(Out,"const unsigned char gImage[38400] = {\n");
	matrix	=	bs->FileSize/(bs->BitMapWidth*bs->BitMapHeight);
	for(i = 0; i < 38400; i+=24)
	{
		memset((void*)buff,'\0', 24);
		if(!feof(bs->fp))
			ret = fread((void *)buff, 1, 24, bs->fp);
		for(j = 0; j < 24; j+=matrix)
		{
			buff[j]		=	(buff[j]) >>2 &0xff;
			buff[j+1]	=	(buff[j+1]*17/10) >>2 &0xff;
			buff[j+2]	=	(buff[j+2]*7/10) >>2 &0xff;
			if(matrix == 4)
			{
				buff[j+3]	=	buff[j+3] >>2;
				fprintf(Out,"0x%.2x,0x%.2x,0x%.2x,0x%.2x,",buff[j+2]&0xff,buff[j+1]&0xff,buff[j]&0xff,buff[j+3]&0xff);
			}
			else
			{
				fprintf(Out,"0x%.2x,0x%.2x,0x%.2x,",buff[j+2]&0xff,buff[j+1]&0xff,buff[j]&0xff);
			}
		}
		fprintf(Out,"\n");
	}
	fseek(bs->fp,-1,SEEK_CUR); 
	fprintf(Out,"\n};\n");
}

