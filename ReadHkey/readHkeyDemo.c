/*
*	��ȡע�����ֵdemo
*	1.RegOpenKeyEx��hkey����ȡ�������ɺ�رոþ�� 
*	2.RegEnumValue��ȡ�������� 
*	3.RegQueryValueEx�û�ȡ���ı�������ѯ����ֵ 
*/

#include <windows.h>
#include <winreg.h>
#include <stdio.h>
#include <wchar.h>
#include <winerror.h>

int main(int argc, char **agrv)
{
	int		i;
	HKEY	hkey = NULL;
	long	lRet;
	DWORD	value_total = 0;
	DWORD	dwSize=256;
	LPWSTR	name[MAX_PATH];
	char	data[MAX_PATH];
	
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_QUERY_VALUE, &hkey);
	if(lRet == ERROR_SUCCESS)
	{
		lRet	=	RegQueryInfoKey(hkey, NULL, NULL, NULL, NULL, NULL, NULL, &value_total, NULL, NULL, NULL, NULL);
		if(lRet	!= ERROR_SUCCESS)
		{
			printf("query info error.\n");
		}
		else
		{
			printf("Query Info Success,Total:%d\n",value_total);
			for(i = 0; i < value_total; i++)
			{
				lRet	=	RegEnumValue(hkey, i, name, &dwSize, NULL, NULL, NULL, NULL);
				if(lRet	== ERROR_SUCCESS)
					lRet	=	RegQueryValueEx(hkey,name,NULL,NULL,(LPBYTE)data,&dwSize);
					if(lRet	== ERROR_SUCCESS)
						printf("%s,Len:%d\n",data,dwSize);
			}

		}
	}
	RegCloseKey(hkey);
	return 0;
}
