#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#ifndef NO_CHANGE_TOP
#define NO_CHANGE_TOP 32
#endif

#ifndef NO_CHANGE_RIGHT
#define NO_CHANGE_RIGHT 42
#endif

#ifndef WM_SUB_MSG
#define WM_SUB_MSG 3000
#endif


HINSTANCE gHinstance;
HWND mHwnd;