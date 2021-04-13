#ifndef	_BITMAP_H
#define	_BITMAP_H
#include <stdio.h>

struct BitMapStream
{
	FILE	*fp;			//位图文件指针 
	int		FileSize;		//文件大小 
	int		BitMapOffset;	//位图数据区偏移量 
	int		BitMapWidth;	//图宽 
	int		BitMapHeight;	//图高
	int		BitPerPixel;	//像素数据大小
	int 	MapDataSize;	//数据区大小 
};

struct BitMapHdr
{
	char	bfType[2];			//位图文件的类型，必须为BM(1-2字节）
	char	bfSize[4];			//文件大小
	char	bfReserved[4];		//保留位
	char	bfOffBits[4];		//位图信息偏移地址 	
};

struct	BitMapInfo
{
	char	infoSize[4];		//该部分大小
	char	mapWidth[4];		//图宽
	char	mapHeight[4];		//图高
	char	mapPlanes[2];		//位面数 恒为1
	char	pixelBitCount[2];	//单像素位数 
	char	mapcompress[4];
/*	 0 - 不压缩 (使用BI_RGB表示) 
	1 - RLE 8-使用8位RLE压缩方式(用BI_RLE8表示) 
	2 - RLE 4-使用4位RLE压缩方式(用BI_RLE4表示) 
	3 - Bitfields-位域存放方式(用BI_BITFIELDS表示)
*/
	char	mapSizeImage[4];			//位数据大小
	char	mapXPelsPerMeter[4];		//用象素/米表示的水平分辨率
	char	mapYPelsPerMeter[4];		//用象素/米表示的垂直分辨率。
	char	mapClrUsed[4];				//文件使用的颜色数。如果为0，则表示使用了全部可能的颜色。如8-比特/象素表示为0x100或者 256。
	char	mapClrImportant[4];			//指定重要的颜色数。当该域的值等于颜色数时（或者等于0时），表示所有颜色都一样重要。
};


int	openBitStream(struct BitMapStream *bs,const char *fileName);
int closeBitStream(struct BitMapStream *bs);
int	getBitMapHdr(struct BitMapStream *bs, struct BitMapHdr *fHdr);
int	getBitMapInfo(struct BitMapStream *bs, struct	BitMapInfo *bi);
int _RGB888_RGB565(struct BitMapStream *bs);
int _RGB888_RGB666(struct BitMapStream *bs);

#endif
