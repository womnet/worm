
#ifndef _WORMNET_TYPES_H
#define _WORMNET_TYPES_H

#ifdef WORMNET_DLL
#ifdef WIN32
#define WORM_DLL_EXPORT __declspec(dllexport)
#else
#define WORM_DLL_EXPORT 
#endif // ifdef WIN32
#else
#define WORM_DLL_EXPORT 
#endif // ifdef WORMNET_DLL

// 为了可读性更好再定义一下关键字
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;


#endif // ifndef _WORMNET_TYPES_H
