/*** Copyright (C) 2019-2020 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#include <ZTK.h>
void gMainFunction(void) { // a user-specified main function to run while the window is open
    while (ZTK_PollMessages(NULL)); // flush messages (possibly(?) not strictly necessary, but I'd recommend it :)
    ZTK_DrawClear(); // clear user area of window
	ZTK_DrawText((const ZT_CHAR*)"Hello World!", NULL); // draw some text using the (host-OS)-built-in system printer at an unspecified location in client area (defaults to top left)
    ZTK_DrawPresent(); // present draw buffer
}
int main(void) {
	ZTK_New((const ZT_CHAR*)("Concept demo: \"Hello World Minimum\" using \"ZTK\" - by \"ZaidaTek\" & \"Andreas Riebesehl\" - Compiled: " __DATE__), NULL, NULL); // declare the window with title, NULL-rect, NULL-runtime icon
	ZTK_Open(); // open the declared window
    while (ZTK_Main(&gMainFunction)) {}; // while user doesn't terminate the window, run a user-specified main function
	ZTK_Free(); // clean up
    return 0; // terminate program, return OK
}
