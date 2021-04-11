#ifndef	_COMMON_H
#define	_COMMON_H


//双字节转int 
int c2i_16(char a, char b)
{
	int ret = 0;
	ret = (int)b&0xff;
	ret = ret<<8;
	ret +=(int)a&0xff;		
	return ret;
}

//四字节转int
int c2i_32(char a, char b, char c, char d)
{
	int ret = 0;
	ret = (int)d&0xff;
	ret = ret<<8;
	ret +=(int)c&0xff;
	ret = ret<<8;
	ret +=(int)b&0xff;
	ret = ret<<8;
	ret +=(int)a&0xff;
		
	return ret;
}


#endif
