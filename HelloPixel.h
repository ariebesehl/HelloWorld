/*** Copyright (C) 2019-2021 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#ifndef HELLOPIXEL_H_INCLUDED
#define HELLOPIXEL_H_INCLUDED

#include <ZTK.h>

#define HP_TEXT_TITLE (const ZT_CHAR*)"Hello Pixel - by ZaidaTek & Andreas Riebesehl - Compiled: " __DATE__
#define HP_TEXT_INFO (const ZT_CHAR*)"Help: Press F1 or H."
#define HP_TEXT_HELP (const ZT_CHAR*)"Help: Need a bag?"
#define HP_TEXT_FONT (const ZT_CHAR*)"MS Sans Serif"
#define HP_TEXT_SIZE 18
#define HP_TEXT_COLOR 0xffffffff
#define HP_FLAG_SHOW_HELP 0x1

typedef struct {
    ZT_SURFACE* pixels;
    ZT_FONT* font;
    ZT_RECT window;
    ZT_FLAG flag;
    struct {
        ZT_INDEX stat;
        ZT_INDEX counter;
        ZT_TIME timestamp;
    } fps;
} HP_HOST;

#endif // HELLOPIXEL_H_INCLUDED
