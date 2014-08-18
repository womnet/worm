
#ifndef _WORMNET_THREAD_H
#define _WORMNET_THREAD_H

#include <wormnet/wm_types.h>

#ifndef MAX_ALLOCA_STACK_ALLOCATION
#define MAX_ALLOCA_STACK_ALLOCATION 1048576
#endif

#ifdef WIN32
#define WORM_THREAD_DECLARATION(functionName) unsigned __stdcall functionName(void* arguments)
int wm_CreateThread(unsigned __stdcall start_address( void* ), void *arglist, int priority=0);
#else
#define WORM_THREAD_DECLARATION(functionName) void* functionName(void* arguments)
int wm_CreateThread(void* start_address( void* ), void *arglist, int priority=0);
#endif


#endif // ifndef _WORMNET_THREAD_H
