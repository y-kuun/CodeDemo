#ifndef __HANDMADE_HERO_DEBUG_H__
#define __HANDMADE_HERO_DEBUG_H__

#include <stdio.h>

#if 0
#define OutputDebugStringAFormat(...) {char cad[512]; sprintf(cad, __VA_ARGS__);  OutputDebugStringA(cad);}
#else
#define OutputDebugStringAFormat(...)
#endif

#endif
