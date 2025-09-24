/*** Copyright (C) 2019-2021 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#ifndef HELLOWORLD_C_INCLUDED
#define HELLOWORLD_C_INCLUDED

#include "HelloWorld.h"

#include <math.h>
#define HW_2PI (2.0 * 3.141593)

HW_RUNTIME gMain;

ZT_BOOL gHW_Greeting(void) {
	ZT_POINT lDistance;
	lDistance.x = (gMain.rect.world.x + gMain.rect.world.w / 2) - (gMain.rect.sun.x + gMain.rect.sun.w / 2);
	lDistance.y = (gMain.rect.world.y + gMain.rect.world.h / 2) - (gMain.rect.sun.y + gMain.rect.sun.h / 2);
	ZT_DBL lVector = sqrt(lDistance.x * lDistance.x + lDistance.y * lDistance.y) / (HW_ORBIT_SIZE_WORLD * (ZT_DBL)gMain.rect.sun.w);
	return lVector < HW_ORBIT_GREETING;
}
void gHW_Orbit(void) {
	ZT_POINT lDistanceWorld;
	ZT_POINT lDistanceMoon;
	ZT_DBL lDutyWorld = (ZT_DBL)(gMain.timestamp.draw % HW_ORBIT_PERIOD_WORLD) / (ZT_DBL)HW_ORBIT_PERIOD_WORLD;
	ZT_DBL lDutyMoon = (ZT_DBL)(gMain.timestamp.draw % HW_ORBIT_PERIOD_MOON) / (ZT_DBL)HW_ORBIT_PERIOD_MOON;
	lDistanceWorld.x = cos(lDutyWorld * HW_2PI) * HW_ORBIT_SIZE_WORLD * gMain.rect.sun.w;
	lDistanceWorld.y = sin(lDutyWorld * HW_2PI) * HW_ORBIT_SIZE_WORLD * HW_ORBIT_ELIPSIS_WORLD * gMain.rect.sun.h;
	lDistanceMoon.x = cos(lDutyMoon * HW_2PI) * HW_ORBIT_SIZE_MOON * gMain.rect.world.w;
	lDistanceMoon.y = sin(lDutyMoon * HW_2PI) * HW_ORBIT_SIZE_MOON * HW_ORBIT_ELIPSIS_MOON * gMain.rect.world.h;
	gMain.rect.world.x = (gMain.rect.window.w - gMain.rect.world.w) / 2 + lDistanceWorld.x;
	gMain.rect.world.y = (gMain.rect.window.h - gMain.rect.world.h) / 2 + lDistanceWorld.y;
	gMain.rect.moon.x = gMain.rect.world.x - (gMain.rect.moon.w - gMain.rect.world.w) / 2 + lDistanceMoon.x;
	gMain.rect.moon.y = gMain.rect.world.y - (gMain.rect.moon.h - gMain.rect.world.h) / 2 + lDistanceMoon.y;
}
void gHW_FPS(void) {
	if (ZTK_TimeoutReset(&(gMain.fps.timestamp), 1000)) {
		gMain.fps.stat = gMain.fps.counter;
		gMain.fps.counter = 0;
	}
}
void gHW_Draw(void) {
	gMain.timestamp.draw = ZTK_GetTicks();
	gHW_Orbit();
	ZTK_DrawClear();
	ZTK_DrawSprite(gMain.sprite.space, NULL);
	ZTK_DrawSprite(gMain.sprite.sun, &gMain.rect.sun);
	ZTK_DrawSprite(gMain.sprite.world, &gMain.rect.world);
	ZTK_DrawSprite(gMain.sprite.moon, &gMain.rect.moon);
	if (!ZTM_Timeout(gMain.timestamp.draw, gMain.timestamp.greet, HW_GREETING_PERIOD)) {
		if (!ZTM_Timeout(gMain.timestamp.draw, gMain.timestamp.greet, HW_GREETING_PERIOD * 3 / 4)) {
			gMain.rect.helloWorld.y = (gMain.rect.world.y > (gMain.rect.window.h / 2)) ? (gMain.rect.sun.y + gMain.rect.sun.h) : (gMain.rect.sun.y - gMain.rect.helloWorld.h);
			ZTK_DrawSprite(gMain.sprite.helloWorld, &(gMain.rect.helloWorld));
		}
		if (ZTM_Timeout(gMain.timestamp.draw, gMain.timestamp.greet, HW_GREETING_PERIOD / 4) && !ZTM_Timeout(gMain.timestamp.draw, gMain.timestamp.greet, HW_GREETING_PERIOD)) {
			gMain.rect.helloSun.x = gMain.rect.world.x + ((gMain.rect.world.w - gMain.rect.helloSun.w) / 2);
			gMain.rect.helloSun.y = (gMain.rect.world.y <= (gMain.rect.window.h / 2)) ? (gMain.rect.world.y + gMain.rect.world.h) : (gMain.rect.world.y - gMain.rect.helloSun.h);
			ZTK_DrawSprite(gMain.sprite.helloSun, &(gMain.rect.helloSun));
		}
	} else if (gHW_Greeting()) {
		gMain.timestamp.greet = gMain.timestamp.draw;
	}
	ZTK_PrintPosFromInteger(0, 0);
	ZTK_Print("FPS: %u", gMain.fps.stat);
	ZTK_PrintPosFromInteger(0, HW_TEXT_SIZE);
	if (gMain.flag & HW_FLAG_SHOW_HELP) {ZTK_Print(HW_TEXT_HELP);} else {ZTK_Print(HW_TEXT_INFO);}
	ZTK_DrawPresent();
	++gMain.fps.counter;
}
void gHW_SpritesFree(void) {
	ZTK_SpriteFree(gMain.sprite.space);
	ZTK_SpriteFree(gMain.sprite.helloSun);
	ZTK_SpriteFree(gMain.sprite.helloWorld);
	ZTK_SpriteFree(gMain.sprite.moon);
	ZTK_SpriteFree(gMain.sprite.sun);
	ZTK_SpriteFree(gMain.sprite.world);
}
void gHW_SpritesLoad(void) {
	ZTM_Seed(ZTK_GetTicks());
	ZT_SURFACE* lSurface = ZTM_SurfaceNewFillFromIntegers(gMain.rect.window.w, gMain.rect.window.h, 0x000000ff);
	for (ZT_I y = 0; y < lSurface->block.y; y++) {
		for (ZT_I x = 0; x < lSurface->block.x; x++) {
			if (!(ZTM_Random(500))) {
				ZT_COLOR lRed = 0xff - ZTM_Random(0x60);
				ZT_COLOR lGreen = 0xff - ZTM_Random(0x40);
				ZT_COLOR lBlue = 0xff - ZTM_Random(0x20);
				ZT_I lSize = (ZTM_Random(3)) % 2;
				ZT_I lSizeX = ((lSize) < x) ? (lSize + 1) : (x + 1);
				lSize = (ZTM_Random(3)) % 2;
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
	gMain.sprite.space = ZTK_SpriteFromSurface(lSurface, ZTK_GetPalette()); // when creating...
	ZTM_SurfaceFree(lSurface); // ...sprites from surfaces, the source must be deleted manually!,
	gMain.sprite.sun = ZTK_SpriteFromBitmapWithTransparent(HW_PATH_SUN, 0x000000ff, 0x0, 0xff); // ...direct-to-sprite functions,
	gMain.sprite.moon = ZTK_SpriteFromBitmapWithTransparent(HW_PATH_MOON, 0x000000ff, 0x0, 0xff); // ...like these three bitmap...
	gMain.sprite.world = ZTK_SpriteFromBitmapWithTransparent(HW_PATH_WORLD, 0x000000ff, 0x0, 0xff); // ...and two text-
	gMain.sprite.helloSun = ZTK_SpriteFromText(HW_LABEL_HELLO_SUN); // generated sprites, automatically delete...
	gMain.sprite.helloWorld = ZTK_SpriteFromText(HW_LABEL_HELLO_WORLD); // ...their temporary surfaces during creation.
	ZTK_SpriteSize(gMain.sprite.sun, &gMain.rect.sun.size);
	ZTK_SpriteSize(gMain.sprite.moon, &gMain.rect.moon.size);
	ZTK_SpriteSize(gMain.sprite.world, &gMain.rect.world.size);
	ZTK_SpriteSize(gMain.sprite.helloSun, &gMain.rect.helloSun.size);
	ZTK_SpriteSize(gMain.sprite.helloWorld, &gMain.rect.helloWorld.size);
	ZTM_RectPosFromIntegers(&gMain.rect.sun, (gMain.rect.window.w - gMain.rect.sun.w) / 2, (gMain.rect.window.h - gMain.rect.sun.h) / 2);
	ZTM_RectPosFromIntegers(&gMain.rect.helloWorld, gMain.rect.sun.x + (gMain.rect.sun.w - gMain.rect.helloWorld.w) / 2, 0);
}
void gHW_Main(void) {
	ZT_EVENT* lEvent;
	while (ZTK_PollMessages(&lEvent)) {
		///uncomment to print event messages to console
		//printf("%x %x %u %u %u %u\n", lEvent->id.device, lEvent->id.event, lEvent->data.d0, lEvent->data.d1, lEvent->data.d2, lEvent->data.d3);
		switch (lEvent->id.H) {
			case ZTK_HID_WINDOW:
				switch (lEvent->id.S) {
					case ZTK_SID_WINDOW_SIZE: gHW_SpritesFree(); // deliberate fall-through
					case ZTK_SID_WINDOW_CREATION: ZTK_GetRect(&(gMain.rect.window)); gHW_SpritesLoad(); break;
					default: break;
				}
				break;
			case ZTK_HID_KEY:
				if (lEvent->id.E == ZTK_EID_KEYBOARD_KEY_UP) {
					switch (lEvent->id.S) {
						case ZTK_SID_KEY_F1: case ZTK_SID_KEY_H: gMain.flag ^= HW_FLAG_SHOW_HELP; break;
						case ZTK_SID_KEY_F: gMain.flag ^= HW_FLAG_FRAMELIMIT; break;
						default: break;
					}
				}
				break;
			default:
				break;
		}
	}
	gHW_FPS();
	if (ZTL_Timer(gMain.timer) || !(gMain.flag & HW_FLAG_FRAMELIMIT)) {gHW_Draw();} else {ZTK_Sleep(1);}
	//gHW_Draw();
}
void gHW_Init(void) {
	ZTM8_Zero(&gMain, sizeof(gMain));
	gMain.timer = ZTL_TimerNew(HW_FPS_LIMIT);
	ZT_I lScreenX = ZTL_ScreenWidth();
	ZT_I lScreenY = ZTL_ScreenHeight();
	ZTM_Rect(&gMain.rect.window, lScreenX / 6, lScreenY / 6, 2 * lScreenX / 3, 2 * lScreenY / 3);
	ZTK_New(&gHW_Main, HW_TEXT_TITLE, &gMain.rect.window, NULL);
	ZTK_Renderer(ZTK_RENDERER_OPENGL);
	ZTK_Open();
	gMain.font = ZTK_FontNew(NULL, HW_TEXT_FONT, HW_TEXT_SIZE, ZTK_FONT_STYLE_NORMAL, HW_TEXT_COLOR);
	ZTK_PrinterInit(gMain.font);
}
void gHW_Exit(void) {
	gHW_SpritesFree();
	ZTK_FontFree(gMain.font);
	ZTK_PrinterQuit();
	ZTK_Free();
	ZTL_TimerFree(gMain.timer);
}
int main() {
	gHW_Init();
	while (ZTK_Main());
	gHW_Exit();
	return 0;
}

#endif // HELLOWORLD_C_INCLUDED
