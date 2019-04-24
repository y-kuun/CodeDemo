#ifndef __HANDMADE_HERO_DEBUG_H__
#define __HANDMADE_HERO_DEBUG_H__

#include <stdio.h>

#define OutputDebugStringAFormat(...) {char cad[512]; sprintf(cad, __VA_ARGS__);  OutputDebugStringA(cad);}

#endif
