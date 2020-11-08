/*** Copyright (C) 2019-2020 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/

#include <ZTK.h>

#define ZTK_HWUI_FONT_WINDOW ZTK_DEFAULT_WINDOW_TEXT_FONT
#define ZTK_HWUI_FONT_TITLE ZTK_DEFAULT_WINDOW_TEXT_FONT
#define ZTK_HWUI_FONT_PRINTER ZTK_DEFAULT_WINDOW_TEXT_FONT
#if (defined(_WIN32) || defined(_WIN64))
#undef ZTK_HWUI_FONT_PRINTER
#define ZTK_HWUI_FONT_PRINTER "Courier New"
#endif

typedef struct {
    ZT_FLAG flag;
    struct {
        ZT_RECT window;
    } rect;
    struct {
        ZT_TIME fps;
    } timestamp;
    struct {
        ZT_INDEX fps;
    } count;
    struct {
        ZT_INDEX fps;
    } stat;
    struct {
        ZT_EVENT* window;
        ZT_EVENT* gui;
    } event;
    struct {
        const ZT_CHAR* windowTitle;
    } text;
    struct {
        ZT_FONT* window;
        ZT_FONT* title;
        ZT_FONT* printer;
    } font;
    ZT_PRINTER* printer;
    struct {
        ZUI_BOX* splash;
    } box;
    struct {
        ZUI_LABEL* title;
    } label;
    struct {
        ZUI_BUTTON* toggle;
        ZUI_BUTTON* exit;
    } button;
} ZT_HWUI_HOST;

ZT_HWUI_HOST* gMain = NULL;

void gHWUI_Size(void){
    ZTK_FontSize(gMain->font.printer, ZTK_GetHeight() * 0.03333);
    ZUI_RectCenterRelative(gMain->box.splash, 0.5, 0.5, 0.875, 0.875);
    ZUI_RectRelative(gMain->button.toggle, 0.25, 0.75, 0.125, 0.06125);
    ZUI_RectRelative(gMain->button.exit, 0.625, 0.75, 0.125, 0.06125);
	ZUI_Size(gMain->label.title, NULL);
	ZUI_PosCenterRelative(gMain->label.title, 0.5, 0.25);
}
void gHWUI_Init(void) {
    if (gMain == NULL) {
        if ((gMain = ZTM8_Init(sizeof(ZT_HWUI_HOST), 0x0)) != NULL) {
            ZTM_Rect(&(gMain->rect.window), ZTK_HostGetScreenWidth() / 4, ZTK_HostGetScreenHeight() / 4, ZTK_HostGetScreenWidth() / 2, ZTK_HostGetScreenHeight() / 2);
            ZTK_New((gMain->text.windowTitle = (const ZT_CHAR*)("Concept demo: \"Hello World UI\" using \"ZTK\" - by \"ZaidaTek\" & \"Andreas Riebesehl\" - Compiled: " __DATE__)), &(gMain->rect.window), NULL); // declare the window with the title, the declared rect and no custom icon
            ZTK_Background(ZT_TRUE); // enable background
            ZTK_BackgroundAuto(ZT_TRUE); // immediately auto-redraw background after every draw clear
            ZTK_Heartbeat(4); // enable heartbeat every 4ms
            ZTK_IdleForbid(ZT_FALSE); // allow idling to reduce idle loop processes
            ZTK_IdleAuto(ZT_TRUE); // automatically enter idle state after message processing
            gMain->font.window = ZTK_FontNew(NULL, (ZT_CHAR*)ZTK_HWUI_FONT_WINDOW, ZTK_DEFAULT_WINDOW_TEXT_SIZE, ZTM_FONT_STYLE_NORMAL, ZTK_DEFAULT_WINDOW_TEXT_COLOR);
            gMain->font.printer = ZTK_FontNew(NULL, (ZT_CHAR*)ZTK_HWUI_FONT_PRINTER, ZTK_GetHeight() * 0.03333, ZTM_FONT_STYLE_NORMAL, ZTK_DEFAULT_WINDOW_TEXT_COLOR);
            gMain->font.title = ZTK_FontNew(NULL, (ZT_CHAR*)ZTK_HWUI_FONT_TITLE, ZTK_DEFAULT_WINDOW_TEXT_SIZE * 3, ZTM_FONT_STYLE_NORMAL, ZTK_DEFAULT_WINDOW_TEXT_COLOR);
            ZTK_FontSet(gMain->font.window);
            ZTK_PrinterInit(gMain->font.printer);
            ZUI_HostNew(); // declare a GUI-host, mandatory
            gMain->box.splash = ZUI_BoxNew(0x8, NULL, 0x1010101f, 0xffffffff); // create a box with user-specified S(pecific-)ID (shared with all UI elements), dimensions according to lRect, with fill-color and frame-color
            gMain->button.toggle = ZUI_ButtonNew(0x10, NULL, (const ZT_CHAR*)"Toggle"); // create first button with user SID, label, and NULL-rect to be later specified (or not?)
            gMain->button.exit = ZUI_ButtonNew(0x11, NULL, (const ZT_CHAR*)"Exit");
            gMain->label.title = ZUI_LabelNewFromFont(0x13, NULL, (const ZT_CHAR*)"Hello World!", gMain->font.title);
            gHWUI_Size();
            ZTK_Open(); // open the declared window
        }
    }
}
void gHWUI_Exit(void) {
    if (gMain != NULL) {
        ZUI_HostFree();
        ZTK_PrinterQuit();
        ZTK_FontFree(gMain->font.title);
        ZTK_FontFree(gMain->font.printer);
        ZTK_FontFree(gMain->font.window);
        ZTK_Free(); // clean up
        ZTM8_Free(gMain);
        gMain = NULL;
    }
}
void gHWUI_Main(void) { // a user-specified main function to run while the window is open
    gMain->count.fps++;
    if (ZTK_Timeout(&(gMain->timestamp.fps), 999)) {
        ZTK_TimeoutReset(&(gMain->timestamp.fps), 999);
        gMain->stat.fps = gMain->count.fps;
        gMain->count.fps = 0;
    }
    while (ZTK_PollMessages(&(gMain->event.window))) {
        ZT_EVENT* lEvent = gMain->event.window;
        switch (lEvent->id.H) {
            case ZTK_HID_WINDOW:
                if (lEvent->id.E == ZTK_EID_WINDOW_RESIZE) {gHWUI_Size();}
                break;
            default:
                break;
        }
        //printf("%x %x %u %u %u %u\n", lEvent->id.device, lEvent->id.event, lEvent->data.d0, lEvent->data.d1, lEvent->data.d2, lEvent->data.d3); // uncomment to print event messages
        while (ZUI_HostPoll(lEvent, &(gMain->event.gui))) { // event may trigger multiple GUI events, e.g. overlapping buttons
            ZT_EVENT* lGUIEvent = gMain->event.gui;
            if (lGUIEvent->id.S == 0x10) {
                if (lGUIEvent->id.E == ZTK_EID_GUI_RELEASE) {ZUI_IsDisabled(gMain->button.exit) ? ZUI_Enable(gMain->button.exit) : ZUI_Disable(gMain->button.exit);}
            }
            if (lGUIEvent->id.S == 0x11) {
                if (lGUIEvent->id.E == ZTK_EID_GUI_RELEASE) {ZTK_Quit();}
            }
            //printf("%x %x %u %u %u %u\n", lGUIEvent->id.device, lGUIEvent->id.event, lGUIEvent->data.d0, lGUIEvent->data.d1, lGUIEvent->data.d2, lGUIEvent->data.d3); // uncomment to print event messages
        }
    }
    ZT_FLAG lPalette = ZTK_GetPalette();
    ZT_COLOR lBackground = ZUI_HostGetJuicyColor();
    ZT_COLOR lR = ZTM_ColorRed(lBackground, lPalette);
    ZT_COLOR lG = ZTM_ColorGreen(lBackground, lPalette);
    ZT_COLOR lB = ZTM_ColorBlue(lBackground, lPalette);
	ZTK_BackgroundColor(lBackground); // set the background color to the color returned by the random internal color modulator
    ZTK_DrawClear(); // clear user area of window
    ZTK_PrinterSetBounds(NULL);
    ZTK_PrintPosFromInteger(0, 0);
    ZTK_Print("FPS: %u", gMain->stat.fps);
    ZT_RECT lRect;
    ZTM_Rect(&lRect, ZTK_GetWidth() * 0.3, ZTK_GetHeight() * 0.4, ZTK_GetWidth() * 0.2 , ZTK_GetHeight() * 0.2);
    ZTK_PrinterSetBounds(&lRect);
    ZTK_Print("R: %#2x\nG: %#2x\nB: %#2x", lR, lG, lB);
    ZTM_Rect(&lRect, ZTK_GetWidth() * 0.45, ZTK_GetHeight() * 0.4, ZTK_GetWidth() * 0.2 , ZTK_GetHeight() * 0.2);
    ZTK_PrinterSetBounds(&lRect);
    ZTK_Print("R: %3u\nG: %3u\nB: %3u", lR, lG, lB);
    ZTM_Rect(&lRect, ZTK_GetWidth() * 0.6, ZTK_GetHeight() * 0.4, ZTK_GetWidth() * 0.2 , ZTK_GetHeight() * 0.2);
    ZTK_PrinterSetBounds(&lRect);
    ZTK_Print("R: %2.2f%%\nG: %2.2f%%\nB: %2.2f%%", lR / 2.55, lG / 2.55, lB / 2.55);
	ZUI_HostDraw(); // draw UI elements onto draw buffer
    ZTK_DrawPresent(); // present draw buffer
}

int main(void) {
	gHWUI_Init(); // run UI setup
    while (ZTK_Main(&gHWUI_Main)) {}; // while user doesn't terminate the window, run a user-specified main function
    gHWUI_Exit(); // clean up UI
    return 0; // terminate program, return OK
}
