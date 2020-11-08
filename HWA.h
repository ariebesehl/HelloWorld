/*** Copyright (C) 2019-2020 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#ifndef HWA_H_INCLUDED
#define HWA_H_INCLUDED

#include <ZTK.h>

#define HWA_DESIRED_FPS 120
#define HWA_VIRTUAL_SCREEN_SIZE_X 1366
#define HWA_VIRTUAL_SCREEN_SIZE_Y 768
#define HWA_SCREEN_SIZE_AUTO // uncomment to make values above effective
#define HWA_RES_SUN "res/sun.bmp"
#define HWA_RES_WORLD "res/world.bmp"
#define HWA_RES_MOON "res/moon.bmp"
#define HWA_TEXT_TITLE "Concept demo: \"Hello World Advanced\" using \"ZTK\" - by \"ZaidaTek\" & \"Andreas Riebesehl\" - Compiled: " __DATE__
#define HWA_TEXT_HELP "Help:\nIf no sun, world or moon is being rendered,\ncheck that the bitmaps are in \"res\" folder.\n\nPress F1 or H again to disable help."
#define HWA_TEXT_INFO "Help: Press F1 or H."
#define HWA_TEXT_SIZE 18
#define HWA_TEXT_COLOR 0xffef00bf
#define HWA_TEXT_FONT "Comic Sans MS"
#define HWA_LABEL_SIZE 24
#define HWA_LABEL_HELLO_WORLD "Hello, world!"
#define HWA_LABEL_HELLO_SUN "Hello, sun!"
#define HWA_ORBIT_PERIOD_MOON 1217 // in ms, default 2^21th of actual (29.53 days)
#define HWA_ORBIT_PERIOD_WORLD 15048 // in ms, default 2^21th of actual (365.25 days)
#define HWA_ORBIT_ELIPSIS_MOON 0x7f // 0x0 - 0xff = 0.0 - 1.0
#define HWA_ORBIT_ELIPSIS_WORLD 0x7f // 0x0 - 0xff = 0.0 - 1.0
#define HWA_ORBIT_SIZE_MOON 0x200 // 0x0 - 0xff = 0.0 - 1.0, of double the width/-height of earth
#define HWA_ORBIT_SIZE_WORLD 0x300 // 0x0 - 0xff = 0.0 - 1.0, of double the width/-height of sun
#define HWA_ORBIT_GREETING 0x1f // 0x0 - 0xff factor of orbit in which a greeting is generated
#define HWA_GREETING_PERIOD 1500 // in ms

#define HWA_FLAG_SHOW_HELP 0x1
#define HWA_FRAMETIME ((1000 / HWA_DESIRED_FPS) - 1)

typedef struct {
    ZT_FLAG flag;
    ZT_EVENT* event;
    ZT_FONT* font;
    ZT_PRINTER* printer;
    struct {
        ZT_TIME draw;
        ZT_TIME greet;
        ZT_TIME fps;
    } timestamp;
    struct {
        ZT_INDEX fps;
    } stat;
    struct {
        ZT_INDEX fps;
    } counter;
    struct {
        ZT_RECT window;
        ZT_RECT helloSun;
        ZT_RECT helloWorld;
        ZT_RECT moon;
        ZT_RECT sun;
        ZT_RECT world;
    } rect;
    struct {
        const ZT_CHAR* helloSun;
        const ZT_CHAR* helloWorld;
        const ZT_CHAR* windowTitle;
    } text;
    struct {
        const ZT_CHAR* moon;
        const ZT_CHAR* sun;
        const ZT_CHAR* world;
    } path;
    struct {
        ZT_SPRITE* moon;
        ZT_SPRITE* sun;
        ZT_SPRITE* world;
        ZT_SPRITE* helloSun;
        ZT_SPRITE* helloWorld;
        ZT_SPRITE* space;
    } sprite;
} ZT_HWA_HOST;

ZT_HWA_HOST* gMain;

ZT_POINT gDist_Sun;
ZT_POINT gDist_Moon;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void gHWA_Main(void);
void gHWA_Init(void);
void gHWA_Exit(void);
void gHWA_FPS(void);
void gHWA_Draw(void);
void gHWA_SpritesLoad(void);
void gHWA_SpritesFree(void);
void gHWA_Orbit(void);
ZT_BOOL gHWA_GreetingDistance(void);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HWA_H_INCLUDED
