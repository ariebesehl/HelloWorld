/*** Copyright (C) 2019-2021 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#ifndef HELLOWORLD_H_INCLUDED
#define HELLOWORLD_H_INCLUDED

#include <ZTK.h>

#define HW_FPS_LIMIT 120
#define HW_PATH_SUN (const ZT_CHAR*)"res/sun.bmp"
#define HW_PATH_WORLD (const ZT_CHAR*)"res/world.bmp"
#define HW_PATH_MOON (const ZT_CHAR*)"res/moon.bmp"
#define HW_TEXT_TITLE (const ZT_CHAR*)"Hello World - by ZaidaTek & Andreas Riebesehl - Compiled: " __DATE__
#define HW_TEXT_HELP (const ZT_CHAR*)"Help:\nPress F1 or H again to disable help."
#define HW_TEXT_INFO (const ZT_CHAR*)"Help: Press F1 or H."
#define HW_TEXT_SIZE 18
#define HW_TEXT_COLOR 0xffef00bf
#if defined(ZTK_BUILD_WINDOWS) && (ZTK_BUILD_WINDOWS)
	#define HW_TEXT_FONT (const ZT_CHAR*)"Comic Sans MS"
#else // ZTK_BUILD_WINDOWS
	#define HW_TEXT_FONT ZTK_DEFAULT_FONT
#endif // ZTK_BUILD_WINDOWS
#define HW_LABEL_SIZE 24
#define HW_LABEL_HELLO_WORLD (const ZT_CHAR*)"Hello, world!"
#define HW_LABEL_HELLO_SUN (const ZT_CHAR*)"Hello, sun!"
#define HW_ORBIT_ELIPSIS_MOON 0.5
#define HW_ORBIT_ELIPSIS_WORLD 0.5
#define HW_ORBIT_SIZE_MOON 2.0
#define HW_ORBIT_SIZE_WORLD 3.0
#define HW_ORBIT_GREETING 0.51
#define HW_ORBIT_PERIOD_MOON 1217 // in ms, default 2^21th of actual (29.53 days)
#define HW_ORBIT_PERIOD_WORLD 15048 // in ms, default 2^21th of actual (365.25 days)
#define HW_GREETING_PERIOD 1500 // in ms

#define HW_FLAG_SHOW_HELP 0x1
#define HW_FLAG_FRAMELIMIT 0x2

typedef struct {
    ZT_FLAG flag;
    ZT_FONT* font;
    ZT_TIMER* timer;
    struct {
        ZT_TIME draw;
        ZT_TIME greet;
    } timestamp;
    struct {
        ZT_INDEX stat;
        ZT_INDEX counter;
        ZT_TIME timestamp;
    } fps;
    struct {
        ZT_RECT window;
        ZT_RECT helloSun;
        ZT_RECT helloWorld;
        ZT_RECT moon;
        ZT_RECT sun;
        ZT_RECT world;
    } rect;
    struct {
        ZT_SPRITE* moon;
        ZT_SPRITE* sun;
        ZT_SPRITE* world;
        ZT_SPRITE* helloSun;
        ZT_SPRITE* helloWorld;
        ZT_SPRITE* space;
    } sprite;
} HW_HOST;
/*
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void gHW_Main(void);
void gHW_Init(void);
void gHW_Exit(void);
void gHW_FPS(void);
void gHW_Draw(void);
void gHW_SpritesLoad(void);
void gHW_SpritesFree(void);
void gHW_Orbit(void);
ZT_BOOL gHW_GreetingDistance(void);
#ifdef __cplusplus
}
#endif // __cplusplus
*/
#endif // HELLOWORLD_H_INCLUDED
