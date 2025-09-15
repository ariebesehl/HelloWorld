/*** Copyright (C) 2019-2021 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#ifndef HELLOUI_C_INCLUDED
#define HELLOUI_C_INCLUDED

#include "HelloUI.h"

HUI_HOST gMain;

void gHUI_Size(void){
    ZTK_FontSize(gMain.font.printer, ZTK_GetHeight() * 0.03333);
    ZUI_RectCenterRelative(gMain.gui.splash, 0.5, 0.5, 0.875, 0.875);
    ZUI_RectRelative(gMain.gui.toggle, 0.25, 0.75, 0.125, 0.06125);
    ZUI_RectRelative(gMain.gui.exit, 0.625, 0.75, 0.125, 0.06125);
	ZUI_Size(gMain.gui.title, NULL);
	ZUI_PosCenterRelative(gMain.gui.title, 0.5, 0.25);
}
void gHUI_Main(void) {
    if (ZTK_TimeoutReset(&(gMain.fps.timestamp), 1000)) {
        gMain.fps.stat = gMain.fps.counter;
        gMain.fps.counter = 0;
    }
    ZT_EVENT* lEvent;
    ZT_EVENT* lEventGUI;
    while (ZTK_PollMessages(&lEvent)) {
        switch (lEvent->id.H) {
            case ZTK_HID_WINDOW: if (lEvent->id.E == ZTK_EID_WINDOW_RESIZE) {gHUI_Size();} break;
            default: break;
        }
        while (ZUI_HostPoll(lEvent, &lEventGUI)) { // event may trigger multiple GUI events, e.g. overlapping buttons
            if (lEventGUI->id.S == HUI_ID_TOGGLE && lEventGUI->id.E == ZTK_EID_GUI_RELEASE) {
                ZUI_IsDisabled(gMain.gui.exit) ? ZUI_Enable(gMain.gui.exit) : ZUI_Disable(gMain.gui.exit);
            }
            if (lEventGUI->id.S == HUI_ID_EXIT && lEventGUI->id.E == ZTK_EID_GUI_RELEASE) {
                ZTK_Quit();
            }
        }
    }
    ZT_COLOR lBackground = ZUI_HostGetJuicyColor();
	ZTK_BackgroundColor(lBackground);
    ZTK_DrawClear();
    ZTK_PrinterSetBounds(NULL);
    ZTK_PrintPosFromInteger(0, 0);
    ZTK_Print("FPS: %u", gMain.fps.stat);
    ZT_FLAG lPalette = ZTK_GetPalette();
    ZT_COLOR lR = ZTM_ColorR(lBackground, lPalette);
    ZT_COLOR lG = ZTM_ColorG(lBackground, lPalette);
    ZT_COLOR lB = ZTM_ColorB(lBackground, lPalette);
    ZT_I lWidthWindow = ZTK_GetWidth();
    ZT_I lHeightWindow = ZTK_GetHeight();
    ZT_I lPosYPrint = lHeightWindow * 0.4;
    ZT_I lWidthPrint = lWidthWindow * 0.2;
    ZT_I lHeightPrint = lHeightWindow * 0.2;
    ZT_RECT lRect;
    ZTM_Rect(&lRect, lWidthWindow * 0.30, lPosYPrint, lWidthPrint, lHeightPrint);
    ZTK_PrinterSetBounds(&lRect);
    ZTK_Print("R: %#2x\nG: %#2x\nB: %#2x", lR, lG, lB);
    ZTM_Rect(&lRect, lWidthWindow * 0.45, lPosYPrint, lWidthPrint, lHeightPrint);
    ZTK_PrinterSetBounds(&lRect);
    ZTK_Print("R: %3u\nG: %3u\nB: %3u", lR, lG, lB);
    ZTM_Rect(&lRect, lWidthWindow * 0.60, lPosYPrint, lWidthPrint, lHeightPrint);
    ZTK_PrinterSetBounds(&lRect);
    ZTK_Print("R: %2.2f%%\nG: %2.2f%%\nB: %2.2f%%", lR / 2.55, lG / 2.55, lB / 2.55);
	ZUI_HostDraw();
    ZTK_DrawPresent();
    gMain.fps.counter++;
}
void gHUI_Init(void) {
    ZTM8_Zero(&gMain, sizeof(gMain));
    ZT_RECT lRectWindow;
    ZTM_Rect(&lRectWindow, ZTL_ScreenWidth() / 4, ZTL_ScreenHeight() / 4, ZTL_ScreenWidth() / 2, ZTL_ScreenHeight() / 2);
    ZTK_New(&gHUI_Main, HUI_TEXT_WINDOW_TITLE, &lRectWindow, NULL);
    //ZTK_Renderer(ZTK_RENDERER_OPENGL);
    gMain.font.window = ZTK_FontNew(NULL, HUI_FONT_WINDOW, ZTK_DEFAULT_FONT_SIZE, ZTK_FONT_STYLE_NORMAL, ZTK_DEFAULT_FONT_COLOR);
    gMain.font.printer = ZTK_FontNew(NULL, HUI_FONT_PRINTER, ZTK_GetHeight() * 0.03333, ZTK_FONT_STYLE_NORMAL, ZTK_DEFAULT_FONT_COLOR);
    gMain.font.title = ZTK_FontNew(NULL, HUI_FONT_TITLE, ZTK_DEFAULT_FONT_SIZE * 3, ZTK_FONT_STYLE_NORMAL, ZTK_DEFAULT_FONT_COLOR);
    ZTK_FontSet(gMain.font.window);
    ZTK_PrinterInit(gMain.font.printer);
    ZUI_HostNew();
    gMain.gui.splash = ZUI_BoxNew(HUI_ID_OTHER, NULL, 0x1010101f, 0xffffffff);
    gMain.gui.toggle = ZUI_ButtonNew(HUI_ID_TOGGLE, NULL, HUI_TEXT_BUTTON_TOGGLE);
    gMain.gui.exit = ZUI_ButtonNew(HUI_ID_EXIT, NULL, HUI_TEXT_BUTTON_EXIT);
    gMain.gui.title = ZUI_LabelNewFromFont(HUI_ID_OTHER, NULL, HUI_TEXT_LABEL_TITLE, gMain.font.title);
    gHUI_Size();
    ZTK_Open();
}
void gHUI_Exit(void) {
    ZUI_HostFree();
    ZTK_FontFree(gMain.font.title);
    ZTK_FontFree(gMain.font.printer);
    ZTK_FontFree(gMain.font.window);
    ZTK_Free();
}
int main(void) {
	gHUI_Init();
    while (ZTK_Main());
    gHUI_Exit();
    return 0;
}

#endif // HELLOUI_C_INCLUDED
