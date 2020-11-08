/*** Copyright (C) 2019-2020 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#ifndef HWA_C_INCLUDED
#define HWA_C_INCLUDED

#include "HWA.h"

#include <math.h> // for simple orbit calculation
#define HWA_PI 3.1415926535897932384626433832795
#define HWA_DBL_PI (HWA_PI * 2.0)

ZT_HWA_HOST* gMain = NULL;

void gHWA_Main(void) {
    if (!ZTK_Timeout(&(gMain->timestamp.draw), HWA_FRAMETIME)) { // this frame is not overdue, calculate if we can idle for at least 1ms until next scheduled draw
        ZT_TIME lDelay = (gMain->timestamp.draw + HWA_FRAMETIME) - ZTK_GetTicks(); // how much time remains ?
        if (lDelay > 1) {ZTK_Idle(lDelay - 1);} // set wake timer to -1ms in regards to next scheduled draw, also do not pass "ZTK_Idle(0)", this is timeout-less waiting, not disabling!
    }
    while (ZTK_PollMessages(&(gMain->event))) {
        switch (gMain->event->id.H) { // note: there is also a simpler messaging system...
            case ZTK_HID_WINDOW:
                switch (gMain->event->id.S) {
                    case ZTK_SID_WINDOW_SIZE:
                        gHWA_SpritesFree(); // upon resize, discard sprites and fall-through to below case
                    case ZTK_SID_WINDOW_CREATION:
                        ZTK_GetRect(&(gMain->rect.window)); // get current window size
                        gHWA_SpritesLoad(); // load sprites
                        break;
                    default:
                        break;
                }
                break;
            case ZTK_HID_KEY:
                if (gMain->event->id.E == ZTK_EID_KEYBOARD_KEY_UP) {
                    switch (gMain->event->id.S) {
                        case ZTK_SID_KEY_H:
                        case ZTK_SID_KEY_F1:
                            gMain->flag ^= HWA_FLAG_SHOW_HELP;
                            break;
                        default: break;
                    }
                }
                break;
            default: break;
        }
        //printf("%x %x %u %u %u %u\n", gMain->event->id.device, gMain->event->id.event, gMain->event->data.d0, gMain->event->data.d1, gMain->event->data.d2, gMain->event->data.d3); // uncomment to print event messages
    }
    gHWA_FPS();
    gHWA_Draw();
}

void gHWA_FPS(void) {
    if (ZTK_Timeout(&(gMain->timestamp.fps), 999)) {
        ZTK_TimeoutReset(&(gMain->timestamp.fps), 999);
        gMain->stat.fps = gMain->counter.fps;
        gMain->counter.fps = 0;
    }
}
void gHWA_Draw(void) {
    if (ZTK_Timeout(&(gMain->timestamp.draw), HWA_FRAMETIME)) {
        ZTK_TimeoutReset(&(gMain->timestamp.draw), HWA_FRAMETIME);
        ZTK_DrawClear();
        ZTK_DrawSprite(gMain->sprite.space, NULL);
        ZTK_DrawSprite(gMain->sprite.sun, &(gMain->rect.sun));
        gHWA_Orbit();
        ZTK_DrawSprite(gMain->sprite.world, &(gMain->rect.world));
        ZTK_DrawSprite(gMain->sprite.moon, &(gMain->rect.moon));
        if (!ZTK_HostTimeout(&(gMain->timestamp.greet), gMain->timestamp.draw, HWA_GREETING_PERIOD)) {
            gMain->rect.helloWorld.y = (gMain->rect.world.y > (gMain->rect.window.h / 2)) ? (gMain->rect.sun.y + gMain->rect.sun.h) : (gMain->rect.sun.y - gMain->rect.helloWorld.h);
            gMain->rect.helloSun.x = gMain->rect.world.x + ((gMain->rect.world.w - gMain->rect.helloSun.w) / 2);
            gMain->rect.helloSun.y = (gMain->rect.world.y <= (gMain->rect.window.h / 2)) ? (gMain->rect.world.y + gMain->rect.world.h) : (gMain->rect.world.y - gMain->rect.helloSun.h);
            if (!ZTK_HostTimeout(&(gMain->timestamp.greet), gMain->timestamp.draw, HWA_GREETING_PERIOD * 3 / 4)) {ZTK_DrawSprite(gMain->sprite.helloWorld, &(gMain->rect.helloWorld));}
            if (ZTK_HostTimeout(&(gMain->timestamp.greet), gMain->timestamp.draw, HWA_GREETING_PERIOD / 4) && !ZTK_HostTimeout(&(gMain->timestamp.greet), gMain->timestamp.draw, HWA_GREETING_PERIOD)) {ZTK_DrawSprite(gMain->sprite.helloSun, &(gMain->rect.helloSun));}
        } else {
            if (gHWA_GreetingDistance()) {gMain->timestamp.greet = gMain->timestamp.draw;}
        }
        ZTK_PrintPosFromInteger(0, 0);
        ZTK_Print("FPS: %u", gMain->stat.fps);
        ZTK_PrintPosFromInteger(0, HWA_TEXT_SIZE);
		if (gMain->flag & HWA_FLAG_SHOW_HELP) {ZTK_Print(HWA_TEXT_HELP);} else {ZTK_Print(HWA_TEXT_INFO);}
        ZTK_DrawPresent();
        gMain->counter.fps++;
    }
}
void gHWA_Exit(void) {
    gHWA_SpritesFree();
    ZTK_FontFree(gMain->font);
    ZTK_PrinterQuit();
    ZTK_Free();
    ZTM8_Free(gMain);
}
void gHWA_Init(void) {
    gMain = (ZT_HWA_HOST*)ZTM8_Init(sizeof(ZT_HWA_HOST), 0x0);
    gMain->path.sun = (const ZT_CHAR*)HWA_RES_SUN;
    gMain->path.world = (const ZT_CHAR*)HWA_RES_WORLD;
    gMain->path.moon = (const ZT_CHAR*)HWA_RES_MOON;
    gMain->text.helloWorld = (const ZT_CHAR*)HWA_LABEL_HELLO_WORLD;
    gMain->text.helloSun = (const ZT_CHAR*)HWA_LABEL_HELLO_SUN;
    gMain->text.windowTitle = (const ZT_CHAR*)(HWA_TEXT_TITLE);
    #ifdef HWA_SCREEN_SIZE_AUTO
    ZT_I lScreenX = ZTK_HostGetScreenWidth();
    ZT_I lScreenY = ZTK_HostGetScreenHeight();
    #else
    ZT_I lScreenX = HWA_VIRTUAL_SCREEN_SIZE_X;
    ZT_I lScreenY = HWA_VIRTUAL_SCREEN_SIZE_Y;
    #endif // HWA_SCREEN_SIZE_AUTO
    ZTM_Rect(&(gMain->rect.window), (lScreenX / 6), (lScreenY / 6), ((2 * lScreenX) / 3), ((2 * lScreenY) / 3)); // centered window 2/3rds of screen size
    ZTK_New(gMain->text.windowTitle, &(gMain->rect.window), NULL); // define the window title, position and size, and without a custom icon
    ZTK_IdleForbid(ZT_FALSE); // explicitly disable forbidding of idling to enable it
    ZTK_Heartbeat(0); // disable heartbeat (done manually with wake timers instead)
    ZTK_IdleHoldOff(4); // forbid idling for 4ms after waking
    ZTK_Open(); // mandatory call to ZTK subsystems
    gMain->font = ZTK_FontNew(NULL, (ZT_CHAR*)HWA_TEXT_FONT, HWA_TEXT_SIZE, ZTM_FONT_STYLE_NORMAL, HWA_TEXT_COLOR);
    ZTK_PrinterInit(gMain->font);
}
void gHWA_SpritesFree(void) {
    ZTK_SpriteFree(gMain->sprite.space);
    ZTK_SpriteFree(gMain->sprite.helloSun);
    ZTK_SpriteFree(gMain->sprite.helloWorld);
    ZTK_SpriteFree(gMain->sprite.moon);
    ZTK_SpriteFree(gMain->sprite.sun);
    ZTK_SpriteFree(gMain->sprite.world);
}
void gHWA_SpritesLoad(void) {
    ZTM_Seed(ZTK_GetTicks());
    ZT_SURFACE* lSurface = ZTM_SurfaceNewFillFromIntegers(gMain->rect.window.w, gMain->rect.window.h, 0x000000ff);
    for (ZT_I y = 0; y < lSurface->block.y; y++) {
        for (ZT_I x = 0; x < lSurface->block.x; x++) {
            if (!(ZTM_Random(500))) { // one in 500 chance per pixel for (a wild ;) star to appear
                ZT_COLOR lRed = 0xff - ZTM_Random(0x60);
                ZT_COLOR lGreen = 0xff - ZTM_Random(0x40);
                ZT_COLOR lBlue = 0xff - ZTM_Random(0x20);
                ZT_I lSize = (ZTM_Random(3)) % 2; // double modulo decreases probability of larger star
                ZT_I lSizeX = ((lSize) < x) ? (lSize + 1) : (x + 1);
                lSize = (ZTM_Random(3)) % 2; // rectangle stars? rectangle stars!
                ZT_I lSizeY = ((lSize) < y) ? (lSize + 1) : (y + 1);
                for (ZT_I sY = 0; sY < lSizeY; sY++) {
                    for (ZT_I sX = 0; sX < lSizeX; sX++) {
                        ZT_I lIndex = (x - sX) + ((y - sY) * lSurface->block.x);
                        lSurface->pixels[lIndex] = ((lRed << 24) | (lGreen << 16) | (lBlue << 8) | 0xff);
                    }
                }
            }
        }
    }
    gMain->sprite.space = ZTK_SpriteFromSurface(lSurface, ZTK_GetPalette()); // when creating...
    ZTM_SurfaceFree(lSurface); // ...sprites from surfaces, the source must be deleted manually!,
    gMain->sprite.sun = ZTK_SpriteFromBitmapWithTransparent(gMain->path.sun, 0x000000ff, 0xff); // ...direct-to-sprite functions,
    gMain->sprite.moon = ZTK_SpriteFromBitmapWithTransparent(gMain->path.moon, 0x000000ff, 0xff); // ...like these three bitmap...
    gMain->sprite.world = ZTK_SpriteFromBitmapWithTransparent(gMain->path.world, 0x000000ff, 0xff); // ...and two text-
    gMain->sprite.helloSun = ZTK_SpriteFromText(gMain->text.helloSun); // generated sprites, automatically delete...
    gMain->sprite.helloWorld = ZTK_SpriteFromText(gMain->text.helloWorld); // ...their temporary surfaces during creation.
    if (gMain->sprite.sun != NULL && gMain->sprite.world != NULL && gMain->sprite.moon != NULL) { // if bitmaps are not found, sprites are NULL
        ZTM_RectSize(&(gMain->rect.sun), &(gMain->sprite.sun->block)); // get the sprites' sizes for processing animation in loop
        ZTM_RectSize(&(gMain->rect.moon), &(gMain->sprite.moon->block));
        ZTM_RectSize(&(gMain->rect.world), &(gMain->sprite.world->block));
        ZTM_RectPosFromIntegers(&(gMain->rect.sun), (gMain->rect.window.w - gMain->rect.sun.w) / 2, (gMain->rect.window.h - gMain->rect.sun.h) / 2);
        ZTM_RectSize(&(gMain->rect.helloSun), &(gMain->sprite.helloSun->block));
        ZTM_RectSize(&(gMain->rect.helloWorld), &(gMain->sprite.helloWorld->block));
        ZTM_RectPosFromIntegers(&(gMain->rect.helloWorld), (gMain->rect.sun.x + ((gMain->rect.sun.w - gMain->rect.helloWorld.w) / 2)), 0);
    }
}
void gHWA_Orbit(void) {
    ZT_DBL lDutyWorld = (ZT_DBL)(gMain->timestamp.draw % HWA_ORBIT_PERIOD_WORLD) / (ZT_DBL)HWA_ORBIT_PERIOD_WORLD;
    ZT_DBL lDutyMoon = (ZT_DBL)(gMain->timestamp.draw % HWA_ORBIT_PERIOD_MOON) / (ZT_DBL)HWA_ORBIT_PERIOD_MOON;
    gDist_Sun.x = cos(lDutyWorld * HWA_DBL_PI) * gMain->rect.sun.w * HWA_ORBIT_SIZE_WORLD / 0xff; // ...because bit-shift! ;)
    gDist_Sun.y = sin(lDutyWorld * HWA_DBL_PI) * gMain->rect.sun.h * HWA_ORBIT_SIZE_WORLD / 0xff;
    gMain->rect.world.x = ((gMain->rect.window.w - gMain->rect.world.w) / 2) + gDist_Sun.x;
    gMain->rect.world.y = ((gMain->rect.window.h - gMain->rect.world.h) / 2) + ((gDist_Sun.y * HWA_ORBIT_ELIPSIS_WORLD) / 0xff);
    gDist_Sun.x = ZTM_Abs(gDist_Sun.x);
    gDist_Sun.y = ZTM_Abs(gDist_Sun.y);
    gDist_Moon.x = (cos(lDutyMoon * HWA_DBL_PI) * (gMain->rect.world.w * HWA_ORBIT_SIZE_MOON)) / 0xff;
    gDist_Moon.y = (sin(lDutyMoon * HWA_DBL_PI) * (gMain->rect.world.h * HWA_ORBIT_SIZE_MOON)) / 0xff;
    gMain->rect.moon.x = (gMain->rect.world.x - ((gMain->rect.moon.w - gMain->rect.world.w) / 2)) + gDist_Moon.x;
    gMain->rect.moon.y = (gMain->rect.world.y - ((gMain->rect.moon.h - gMain->rect.world.h) / 2)) + ((gDist_Moon.y * HWA_ORBIT_ELIPSIS_MOON) / 0xff);
    gDist_Moon.x = ZTM_Abs(gDist_Moon.x);
    gDist_Moon.y = ZTM_Abs(gDist_Moon.y);
}
ZT_BOOL gHWA_GreetingDistance(void) {
    ZT_BOOL lGreet = ZT_FALSE;
    if (gMain->rect.sun.w && gMain->rect.sun.h) {
        lGreet = ((((gDist_Sun.x * 0x7f * HWA_ORBIT_ELIPSIS_WORLD) / gMain->rect.sun.w) + ((gDist_Sun.y * 0x7f) / gMain->rect.sun.h)) / 0xfe) < HWA_ORBIT_GREETING;
    }
    return lGreet;
}

#endif // HWA_C_INCLUDED
