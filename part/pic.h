#include "head.h"
#include "pic_r.h"
#include <highgui.h>
#include <cxcore.h>
#include <cv.h>

typedef struct {
    int x;
    int y;
} MyPos;

#ifndef ID_SHOW_CUT
#define ID_SHOW_CUT 3001
#endif

HWND wHwnd;
RECT cRect;
RECT clearRect;
int pFlag;
RECT wcRect;
POINT mPoint;

int ShowDialog();
int LoadNowImg(HWND hwnd);
int DrawRect(HWND hwnd,RECT rect);
int CutImage(char * source);
int JpgToBmp(char * source,char * out);
int GetScreenImg(HWND wh,char * dirPath,char * fileName);

