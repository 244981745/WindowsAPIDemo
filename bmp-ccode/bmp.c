#ifndef	__linux__
#include "BitMap.c"
#else
#include "BitMap.h"
#endif
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
	struct BitMapStream bs;
	struct BitMapHdr hdr;
	struct	BitMapInfo bi;
	int ret=0;
	
	if(argc < 3)
	{
		printf("usage: \n\tbmp -[16/18] filename\n");
		return -1;	
	}
	else
	{
		if(!strcmp("-16",argv[1])|| !strcmp("-18",argv[1]))
		{
			if(openBitStream(&bs,argv[2]))
			{
				printf("file: %s open error.\n",argv[2]);
			}
			else
			{
				getBitMapHdr(&bs, &hdr);
				getBitMapInfo(&bs, &bi);
				
				if(!strcmp("-16",argv[1]))
				{
					if(_RGB888_RGB565(&bs))
					{
						printf("RGB888-RGB565 ERROR.\n");
					}
					else
					{
						printf("RGB88-RGB565 Finish.\n");
					}
				}
				else if(!strcmp("-18",argv[1]))
				{
					if(_RGB888_RGB666(&bs))
					{
						printf("RGB888-RGB666 ERROR.\n");
					}
					else
					{
						printf("RGB88-RGB666 Finish.\n");
					}
				}
			}
		}
		else
		{
			printf("\tParameters:%s not supported\n",argv[1]);
			return -1;
		}
	}

	closeBitStream(&bs);
	return 0;
}
