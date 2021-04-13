#include "BitMap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
//#define	TEST
#define	BUFFSIZE	30
#define	OBUFFSIZE	2

int	openBitStream(struct BitMapStream *bs,const char *fileName)
{
	if(bs == NULL) return -1;
	
	//bs->fp	 = fopen("mode.bmp","rb");
	bs->fp	 = fopen(fileName,"rb");
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
		fread((void *)fHdr, 1, 14, bs->fp);
#ifdef TEST
		printf("----------------hdr------------\n");
		printf("bm:\t\t%0.2s\n",fHdr->bfType);
		printf("size:\t\t%x %x %x %x\n",fHdr->bfSize[0],\
		fHdr->bfSize[1],fHdr->bfSize[2],fHdr->bfSize[3]);
		printf("private:\t%x %x %x %x\n",fHdr->bfReserved[0],\
		fHdr->bfReserved[1],fHdr->bfReserved[2],fHdr->bfReserved[3]);
		printf("offset:\t\t%x %x %x %x\n",fHdr->bfOffBits[0],\
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
		printf("info size:\t%x %x %x %x\n",bi->infoSize[0],\
		bi->infoSize[1],bi->infoSize[2],bi->infoSize[3]);
		printf("map width:\t%x %x %x %x\n",bi->mapWidth[0],\
		bi->mapWidth[1],bi->mapWidth[2],bi->mapWidth[3]);
		printf("map height:\t%x %x %x %x\n",bi->mapHeight[0],\
		bi->mapHeight[1],bi->mapHeight[2],bi->mapHeight[3]);
		
		printf("map wm:\t\t%x %x\n",bi->mapPlanes[0],bi->mapPlanes[1]);
		printf("map bit/pixel:\t%x %x\n",bi->pixelBitCount[0],bi->pixelBitCount[1]);
		
		printf("map compress:\t%x %x %x %x\n",bi->mapcompress[0],\
		bi->mapcompress[1],bi->mapcompress[2],bi->mapcompress[3]);
/*		压缩说明： 
		0 - 不压缩 (使用BI_RGB表示) 
		1 - RLE 8-使用8位RLE压缩方式(用BI_RLE8表示) 
		2 - RLE 4-使用4位RLE压缩方式(用BI_RLE4表示) 
		3 - Bitfields-位域存放方式(用BI_BITFIELDS表示)	
*/
		printf("map size:\t%x %x %x %x\n",bi->mapSizeImage[0],\
		bi->mapSizeImage[1],bi->mapSizeImage[2],bi->mapSizeImage[3]);
		
		printf("map x:\t\t%x %x %x %x\n",bi->mapXPelsPerMeter[0],\
		bi->mapXPelsPerMeter[1],bi->mapXPelsPerMeter[2],bi->mapXPelsPerMeter[3]);
		printf("map y:\t\t%x %x %x %x\n",bi->mapYPelsPerMeter[0],\
		bi->mapYPelsPerMeter[1],bi->mapYPelsPerMeter[2],bi->mapYPelsPerMeter[3]);		
		printf("clrusd:\t\t%x %x %x %x\n",bi->mapClrUsed[0],\
		bi->mapClrUsed[1],bi->mapClrUsed[2],bi->mapClrUsed[3]);	
		printf("clrimp:\t\t%x %x %x %x\n",bi->mapClrImportant[0],\
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


int _RGB888_RGB565(struct BitMapStream *bs)
{
	FILE 	*Out;
	char	buff[BUFFSIZE];
	char	bout[OBUFFSIZE];

	int ret = 0,i,j,matrix;

	Out = fopen("image.h","wb");
	if(Out == NULL) return -1;
	
	
	fseek(bs->fp, bs->BitMapOffset, SEEK_SET);

	fprintf(Out,"const unsigned char gImage[25600] = {\n");
	matrix	=	bs->FileSize/(bs->BitMapWidth*bs->BitMapHeight);
	for(i = 0; i < 25600; i+=24)
	{
		memset((void*)buff,'\0', 24);
		if(!feof(bs->fp))
			ret = fread((void *)buff, 1, 24, bs->fp);
		for(j = 0; j < 24; j+=matrix)
		{
			bout[0]	=	((buff[j])  &0xf8) |  ((buff[j+1])&0xff) >>5;
			bout[1]	=	((buff[j+1]<<3)&0xe0)| ((buff[j+2])&0x1f);

			fprintf(Out,"0x%.2x,0x%.2x,",bout[j]&0xff,bout[j+1]&0xff);
			bout[0]	=	0;
			bout[1]	=	0;
		}
		fprintf(Out,"\n");
	}
	fseek(bs->fp,-1,SEEK_CUR); 
	fprintf(Out,"\n};\n");
	return 0;
}

int _RGB888_RGB666(struct BitMapStream *bs)
{
	FILE 	*Out;
	char	buff[OBUFFSIZE];

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
			if(buff[j]&0xff > 0xf)
				buff[j]		=	((buff[j])  &0xff) >>2 ;
			else
				buff[j]		=	((buff[j])  &0x3f);
			if(buff[j+1]&0xff > 0xf)
				buff[j+1]	=	((buff[j+1])&0xff) >>2 ;
			else
				buff[j+1]	=	((buff[j+1])&0x3f);
			if(buff[j+1]&0xff > 0xf)
				buff[j+2]	=	((buff[j+2])&0xff) >>2 ;
			else
				buff[j+2]	=	((buff[j+2])&0x3f);
			if(matrix == 4)
				buff[j+3]	=	buff[j+3] >>2;
			fprintf(Out,"0x%.2x,0x%.2x,0x%.2x,",buff[j+2]&0xff,buff[j+1]&0xff,buff[j]&0xff);
		}
		fprintf(Out,"\n");
	}
	fseek(bs->fp,-1,SEEK_CUR); 
	fprintf(Out,"\n};\n");
	return 0;
}
