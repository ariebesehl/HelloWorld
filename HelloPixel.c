/*** Copyright (C) 2019-2021 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#ifndef HELLOPIXEL_C_INCLUDED
#define HELLOPIXEL_C_INCLUDED

#include "HelloPixel.h"

HP_RUNTIME gMain;

void gHP_FPS(void) {
	if (ZTK_TimeoutReset(&gMain.fps.timestamp, 1000)) {
		gMain.fps.stat = gMain.fps.counter;
		gMain.fps.counter = 0;
	}
}
void gHP_SurfaceRandomize(void) {
	ZT_COLOR lAlphaMask = ZTM_ColorMaskA(ZTK_GetPaletteSystem());
	for (ZT_I y = 0; y < gMain.pixels->block.y; y++) {
		ZT_I lOffset = y * gMain.pixels->block.x;
		for (ZT_I x = 0; x < gMain.pixels->block.x; x++) {
			gMain.pixels->pixels[lOffset + x] = ZTM_Random(0) | lAlphaMask;
		}
	}
}
void gHP_SurfaceFree(void) {
	ZTM_SurfaceFree(gMain.pixels);
}
void gHP_SurfaceLoad(void) {
	gMain.pixels = ZTM_SurfaceNewFillFromIntegers(gMain.window.w, gMain.window.h, ZTM_ColorConvert(0x000000ff, ZTM_PALETTE_RGBA, ZTK_GetPaletteSystem()));
}
void gHP_Draw(void) {
	ZTK_DrawClear();
	gHP_SurfaceRandomize();
	ZTK_DrawSurface(gMain.pixels, NULL);
	ZTK_PrintPosFromInteger(0, 0);
	ZTK_Print("FPS: %u", gMain.fps.stat);
	ZTK_PrintPosFromInteger(0, HP_TEXT_SIZE);
	if (gMain.flag & HP_FLAG_SHOW_HELP) {ZTK_Print(HP_TEXT_HELP);} else {ZTK_Print(HP_TEXT_INFO);}
	ZTK_DrawPresent();
	gMain.fps.counter++;
}
void gHP_Main(void) {
	ZT_EVENT* lEvent;
	while (ZTK_PollMessages(&lEvent)) {
		///uncomment to print event messages to console
		//printf("%x %x %u %u %u %u\n", lEvent->id.device, lEvent->id.event, lEvent->data.d0, lEvent->data.d1, lEvent->data.d2, lEvent->data.d3);
		switch (lEvent->id.H) {
			case ZTK_HID_WINDOW:
				switch (lEvent->id.S) {
					case ZTK_SID_WINDOW_SIZE: gHP_SurfaceFree(); // deliberate fall-through
					case ZTK_SID_WINDOW_CREATION: ZTK_GetRect(&gMain.window); gHP_SurfaceLoad(); break;
					default: break;
				}
				break;
			case ZTK_HID_KEY:
				if (lEvent->id.E == ZTK_EID_KEYBOARD_KEY_UP) {
					switch (lEvent->id.S) {
						case ZTK_SID_KEY_F1: case ZTK_SID_KEY_H: gMain.flag ^= HP_FLAG_SHOW_HELP; break;
						default: break;
					}
				}
				break;
			default:
				break;
		}
	}
	gHP_FPS();
	gHP_Draw();
}
void gHP_Init(void) {
	ZTM_Seed(ZTM_Time());
	ZTM8_Zero(&gMain, sizeof(gMain));
	ZT_I lScreenX = ZTL_ScreenWidth();
	ZT_I lScreenY = ZTL_ScreenHeight();
	ZTM_RectSizeFromIntegers(&gMain.window, 2 * lScreenX / 3, 2 * lScreenY / 3);
	ZTM_RectPosFromIntegers(&gMain.window, (lScreenX - gMain.window.w) / 2, (lScreenY - gMain.window.h) / 2);
	ZTK_New(&gHP_Main, HP_TEXT_TITLE, &gMain.window, NULL);
	ZTK_Renderer(ZTK_RENDERER_OPENGL);
	gMain.font = ZTK_FontNew(NULL, HP_TEXT_FONT, HP_TEXT_SIZE, ZTK_FONT_STYLE_NORMAL, HP_TEXT_COLOR);
	ZTK_PrinterInit(gMain.font);
	ZTK_Open();
}
void gHP_Exit(void) {
	gHP_SurfaceFree();
	ZTK_FontFree(gMain.font);
	ZTK_PrinterQuit();
	ZTK_Free();
}
int main() {
	gHP_Init();
	while (ZTK_Main());
	gHP_Exit();
	return 0;
}

#endif // HELLOPIXEL_C_INCLUDED
