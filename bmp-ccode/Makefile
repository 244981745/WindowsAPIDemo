all:bmp.c BitMap.o
	gcc bmp.c BitMap.o -o bmp

BitMap.o:BitMap.c
	gcc	-c BitMap.c

clean:
	rm -f *.o image.h