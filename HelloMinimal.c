/*** Copyright (C) 2019-2021 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#include <ZTK.h>
void gMainFunction(void) {
    while (ZTK_PollMessages(NULL));
    ZTK_DrawClear();
	ZTK_DrawText((const ZT_CHAR*)"Hello World!", NULL);
    ZTK_DrawPresent();
}
int main(void) {
	ZTK_New(&gMainFunction, (const ZT_CHAR*)"Hello Minimal - by ZaidaTek & Andreas Riebesehl - Compiled: " __DATE__, NULL, NULL);
	ZTK_Open();
    while (ZTK_Main());
	ZTK_Free();
    return 0;
}
