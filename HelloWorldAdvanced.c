/*** Copyright (C) 2019-2020 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: This work is presently unlicensed.
****
***/

#include "HWA.h"

int main() {
    gHWA_Init();
    while (ZTK_Main(&gHWA_Main));
    gHWA_Exit();
    return 0;
}
