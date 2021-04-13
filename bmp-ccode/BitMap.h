#ifndef	_BITMAP_H
#define	_BITMAP_H
#include <stdio.h>

struct BitMapStream
{
	FILE	*fp;			//λͼ�ļ�ָ�� 
	int		FileSize;		//�ļ���С 
	int		BitMapOffset;	//λͼ������ƫ���� 
	int		BitMapWidth;	//ͼ�� 
	int		BitMapHeight;	//ͼ��
	int		BitPerPixel;	//�������ݴ�С
	int 	MapDataSize;	//��������С 
};

struct BitMapHdr
{
	char	bfType[2];			//λͼ�ļ������ͣ�����ΪBM(1-2�ֽڣ�
	char	bfSize[4];			//�ļ���С
	char	bfReserved[4];		//����λ
	char	bfOffBits[4];		//λͼ��Ϣƫ�Ƶ�ַ 	
};

struct	BitMapInfo
{
	char	infoSize[4];		//�ò��ִ�С
	char	mapWidth[4];		//ͼ��
	char	mapHeight[4];		//ͼ��
	char	mapPlanes[2];		//λ���� ��Ϊ1
	char	pixelBitCount[2];	//������λ�� 
	char	mapcompress[4];
/*	 0 - ��ѹ�� (ʹ��BI_RGB��ʾ) 
	1 - RLE 8-ʹ��8λRLEѹ����ʽ(��BI_RLE8��ʾ) 
	2 - RLE 4-ʹ��4λRLEѹ����ʽ(��BI_RLE4��ʾ) 
	3 - Bitfields-λ���ŷ�ʽ(��BI_BITFIELDS��ʾ)
*/
	char	mapSizeImage[4];			//λ���ݴ�С
	char	mapXPelsPerMeter[4];		//������/�ױ�ʾ��ˮƽ�ֱ���
	char	mapYPelsPerMeter[4];		//������/�ױ�ʾ�Ĵ�ֱ�ֱ��ʡ�
	char	mapClrUsed[4];				//�ļ�ʹ�õ���ɫ�������Ϊ0�����ʾʹ����ȫ�����ܵ���ɫ����8-����/���ر�ʾΪ0x100���� 256��
	char	mapClrImportant[4];			//ָ����Ҫ����ɫ�����������ֵ������ɫ��ʱ�����ߵ���0ʱ������ʾ������ɫ��һ����Ҫ��
};


int	openBitStream(struct BitMapStream *bs,const char *fileName);
int closeBitStream(struct BitMapStream *bs);
int	getBitMapHdr(struct BitMapStream *bs, struct BitMapHdr *fHdr);
int	getBitMapInfo(struct BitMapStream *bs, struct	BitMapInfo *bi);
int _RGB888_RGB565(struct BitMapStream *bs);
int _RGB888_RGB666(struct BitMapStream *bs);

#endif
