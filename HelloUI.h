/*** Copyright (C) 2019-2021 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#ifndef HELLOUI_H_INCLUDED
#define HELLOUI_H_INCLUDED

#include <ZTK.h>
#include <ZUI.h>

#ifdef ZTK_BUILD_WINDOWS
#define HUI_FONT_WINDOW (const ZT_CHAR*)"MS Sans Serif"
#define HUI_FONT_TITLE (const ZT_CHAR*)"Verdana"
#define HUI_FONT_PRINTER (const ZT_CHAR*)"Courier New"
#else
#define HUI_FONT_WINDOW ZTK_DEFAULT_FONT
#define HUI_FONT_TITLE ZTK_DEFAULT_FONT
#define HUI_FONT_PRINTER ZTK_DEFAULT_FONT
#endif // ZTK_BUILD_WINDOWS

#define HUI_ID_TOGGLE 0x1
#define HUI_ID_EXIT 0x2
#define HUI_ID_OTHER 0x3
#define HUI_TEXT_WINDOW_TITLE (const ZT_CHAR*)"Hello UI - by ZaidaTek & Andreas Riebesehl - Compiled: " __DATE__
#define HUI_TEXT_LABEL_TITLE (const ZT_CHAR*)"Hello World!"
#define HUI_TEXT_BUTTON_TOGGLE (const ZT_CHAR*)"Toggle"
#define HUI_TEXT_BUTTON_EXIT (const ZT_CHAR*)"Exit"

typedef struct {
    struct {
        ZT_INDEX counter;
        ZT_INDEX stat;
        ZT_TIME timestamp;
    } fps;
    struct {
        ZT_FONT* window;
        ZT_FONT* title;
        ZT_FONT* printer;
    } font;
    struct {
        ZUI_BOX* splash;
        ZUI_LABEL* title;
        ZUI_BUTTON* toggle;
        ZUI_BUTTON* exit;
    } gui;
} HUI_HOST;

HUI_HOST gMain;

#endif // HELLOUI_H_INCLUDED
