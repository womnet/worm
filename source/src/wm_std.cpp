
#ifdef WIN32
#include <windows.h>
#undef ERROR
#else
#include <sys/time.h>
#include <string.h>
#include <sys/sysctl.h>
#endif //

#include <stdio.h>
#include <assert.h>
#include "wormnet/wm_types.h"

char* ip2str(const unsigned int& ip, char* szIp)
{
	assert(NULL != szIp);
	sprintf(szIp, "%u.%u.%u.%u", ip&0xff, (ip&0x0000ff00)>>8, (ip&0x00ff0000)>>16, (ip&0xff000000)>>24);

	return szIp;
}

#ifndef WIN32
#ifdef KERN_PROC
uint32 GetModuleFileName(void* p, char* szFileName, int nSize)
{
    int mib[4];
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;
    size_t cb = nSize;
    sysctl(mib, 4, szFileName, &cb, NULL, 0);
    
    return 0;
}
#else
uint32 GetModuleFileName(void* p, char* szFileName, int nSize)
{
	int ret = -1;
	char sLine[1024] = { 0 };
	void* pSymbol = (void*)"";
	FILE *fp = NULL;
	char *pPath = NULL;

	fp = fopen ("/proc/self/maps", "r");
	if (NULL == fp)
	{
		return ret;
	}
	while (!feof (fp))
	{
		unsigned long start, end;

		if ( !fgets (sLine, sizeof (sLine), fp))
		{
			continue;
		}
		if ( !strstr (sLine, " r-xp ") || !strchr (sLine, '/'))
		{
			continue;
		}

		sscanf (sLine, "%lx-%lx ", &start, &end);
		if (pSymbol >= (void *) start && pSymbol < (void *) end)
		{
			char *tmp;
			size_t len;

			/* Extract the filename; it is always an absolute path */
			pPath = strchr (sLine, '/');

			/* Get rid of the newline */
			tmp = strrchr (pPath, '\n');
			if (tmp) *tmp = 0;


			ret = 0;
			strcpy(szFileName, pPath );
		}
	}
	fclose (fp);

	return ret;
}
#endif // ifdef _FREEBSD
#endif //ifndef WIN32

