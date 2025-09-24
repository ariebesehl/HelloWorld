/*** Copyright (C) 2019-2021 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/
#include <ZTK.h>
void gMain(void) {
	while (ZTK_PollMessages(NULL));
	ZTK_DrawClear();
	ZTK_DrawText(ZT_TEXT("Hello, world!"), NULL);
	ZTK_DrawPresent();
}
int main(void) {
	ZTK_New(&gMain, ZT_TEXT("HelloMinimal-v" "250924"), NULL, NULL);
	ZTK_Open();
	while (ZTK_Main());
	ZTK_Free();
	return 0;
}
