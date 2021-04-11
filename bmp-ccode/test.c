#include "BitMap.c"
#include <stdio.h>


int main(int argc, char **argv)
{
	struct BitMapStream bs;
	struct BitMapHdr hdr;
	struct	BitMapInfo bi;
	
	openBitStream(&bs);
	
	getBitMapHdr(&bs, &hdr);
	getBitMapInfo(&bs, &bi);
	//_RGB888_RGB565(&bs);
	_RGB888_RGB666(&bs);
	closeBitStream(&bs);
	return 0;
}
