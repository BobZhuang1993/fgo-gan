#include "pic.h"

int CmpImage(char * source,char * target,MyPos * ret){
    IplImage * sourceTmp,* targetTmp,* result;
    int sw,sh;
    int tw,th;

    double minVal,maxVal;
    CvPoint minLoc,maxLoc;

    sourceTmp = (IplImage *)cvLoadImage(source,1);
    if(sourceTmp == NULL){
        return 0;
    }
    targetTmp = (IplImage *)cvLoadImage(target,1);
    if(targetTmp == NULL){
        return 0;
    }
    //
    sw = sourceTmp->width;
    sh = sourceTmp->height;

    tw = targetTmp->width;
    th = targetTmp->height;

    //
    result = cvCreateImage(cvSize(sw - tw + 1,sh - th + 1),32,1);

    //
    cvMatchTemplate(sourceTmp,targetTmp,result,CV_TM_CCOEFF_NORMED);
    cvNormalize(result,result,1,0,CV_MINMAX,NULL);
    cvMinMaxLoc(result,&minVal,&maxVal,&minLoc,&maxLoc,NULL);

    ret->x = maxLoc.x + tw / 2;
    ret->y = maxLoc.y + th / 2;
    cvReleaseImage(&sourceTmp);
    cvReleaseImage(&targetTmp);
    cvReleaseImage(&result);
    // cvNamedWindow("test",1);
    // cvShowImage("test",sourceTmp);
    // cvWaitKey(0);
    
    // cvDestroyWindow("test");
    return 1;
}

//
int GetScreenImg(HWND wh,char * dirPath,char * fileName){
    HANDLE hDIB;
    BITMAP bmpScreen;
    BITMAPINFOHEADER bi;
    BITMAPFILEHEADER bmHeader;
    DWORD dwBmpSize;
    char filePath[MAX_PATH];
    char * lpbitmap;
    HANDLE fh;
    DWORD dwSizeDIB;
    RECT wRect;
    //40 30
    // int sw= GetSystemMetrics(SM_CXSCREEN);
    // int sh = GetSystemMetrics(SM_CYSCREEN);
    GetWindowRect(wh,&wRect);
    int sw = wRect.right - wRect.left;
    int sh = wRect.bottom - wRect.top;
    HDC asDc = GetDC(wh); //
    HDC memDc = CreateCompatibleDC(asDc); //
    
    if(!memDc){
        DeleteObject(memDc);
        ReleaseDC(NULL,asDc);
        return 0;
    }
    //
    HBITMAP hbmScreen = CreateCompatibleBitmap(asDc,sw-NO_CHANGE_RIGHT,sh-NO_CHANGE_TOP);
    if(!hbmScreen){
        DeleteObject(memDc);
        ReleaseDC(NULL,asDc);
        return 0;
    }
    //
    SelectObject(memDc,hbmScreen);
    int res = BitBlt(memDc,0,0,sw-NO_CHANGE_RIGHT,sh-NO_CHANGE_TOP,asDc,0,NO_CHANGE_TOP,SRCCOPY);
    if(!res){
        DeleteObject(hbmScreen);
        DeleteObject(memDc);
        ReleaseDC(NULL,asDc);
        return 0;
    }
    //
    GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 16;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    dwBmpSize = bmpScreen.bmWidth * bi.biBitCount * bmpScreen.bmHeight;

    hDIB = GlobalAlloc(GHND,dwBmpSize);
    lpbitmap = (char *)GlobalLock(hDIB);

    //

    GetDIBits(
        asDc,
        hbmScreen,
        0,
        (UINT)bmpScreen.bmHeight,
        lpbitmap,
        (BITMAPINFO *)&bi,
        DIB_RGB_COLORS
    );

    wsprintf(filePath,"%s/%s.jpg",dirPath,fileName);

    fh = CreateFileA(filePath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if(fh == NULL){
        DeleteObject(hbmScreen);
        DeleteObject(memDc);
        ReleaseDC(wh,asDc);
        return 0;
    }

    dwSizeDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    bmHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    bmHeader.bfSize = dwSizeDIB;

    bmHeader.bfType = 0x4D42;

    WriteFile(fh,&bmHeader,sizeof(BITMAPFILEHEADER),NULL,NULL);
    WriteFile(fh,&bi,sizeof(BITMAPINFOHEADER),NULL,NULL);
    WriteFile(fh,lpbitmap,dwBmpSize,NULL,NULL);

    //
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    //
    CloseHandle(fh);

    //
    DeleteObject(hbmScreen);
    DeleteObject(memDc);
    ReleaseDC(NULL,asDc);

    // printf("%d",sw);
    // CaptureImage(NULL,"./img","test");
    return 1;
}

LRESULT WINAPI GialogProc(HWND hwnd,unsigned int msg,WPARAM wParam,LPARAM lParam){
    switch(msg){
        case WM_COMMAND:
            switch((WORD)wParam){
                case ID_JIE_SURE:
                    printf("截图按钮\n");
                    // ShowImg("./img/now.jpg");
                    CutImage("./img/now.jpg");
                    //
                    SendMessageA(mHwnd,WM_SUB_MSG,ID_SHOW_CUT,0);
                    PostMessageA(hwnd,WM_CLOSE,0,0);
                    break;
                case ID_JIE_CANCEL:
                    printf("消失截图\n");
                    PostMessageA(hwnd,WM_CLOSE,0,0);
                    break;
            } 
            break;
        case WM_CLOSE:
            printf("关闭窗口\n");
            DestroyWindow(hwnd);
            break;
        case WM_INITDIALOG:
            printf("对话框初始\n");
            LoadNowImg(hwnd);
        case WM_LBUTTONDOWN:
            GetCursorPos(&mPoint);
            GetWindowRect(hwnd,&wcRect);
            InvalidateRect(hwnd,&clearRect,0);
            UpdateWindow(hwnd);
            pFlag = 1;
            cRect.left = mPoint.x - wcRect.left;
            cRect.top = mPoint.y - wcRect.top;
            cRect.right = mPoint.x - wcRect.left;
            cRect.bottom = mPoint.y - wcRect.top;

            clearRect.left = cRect.left - 1;
            clearRect.top = cRect.top - 1;
            clearRect.right = cRect.right + 1;
            clearRect.bottom = cRect.bottom + 1;
            break;
        case WM_MOUSEMOVE:
            // printf("移动\n");
            GetCursorPos(&mPoint);
            if(pFlag == 1 && ((mPoint.x - wcRect.left <= 960) && (mPoint.y - wcRect.top <= 542))){
                InvalidateRect(hwnd,&clearRect,0);
                UpdateWindow(hwnd);
                cRect.right = mPoint.x - wcRect.left;
                cRect.bottom = mPoint.y - wcRect.top;
                
                clearRect.right = cRect.right + 1;
                clearRect.bottom = cRect.bottom + 1;

                DrawRect(hwnd,cRect);
            }
            break;
        case WM_LBUTTONUP:
            pFlag = 0;
            if(cRect.left == cRect.right){
                InvalidateRect(hwnd,&cRect,0);
            }
            printf("左起\n");
            break;
        // case WM_PAINT:
        //     printf("窗口重绘\n");
        //     break;
        // case :
        //     printf("no\n");
        //     break;
    }
    return 0;
}
int DrawRect(HWND hwnd,RECT rect){
    HDC hdc = GetWindowDC(hwnd);
    HBRUSH brush = CreateSolidBrush(RGB(247,63,98));
    // HPEN pen = CreatePen(PS_SOLID,2,RGB(0,0,0));
    FrameRect(hdc,&rect,brush);
    ReleaseDC(hwnd,hdc);
    DeleteObject(brush);
    return 0;
}
//加载图片
int LoadNowImg(HWND hwnd){
    //加载bmp资源
    // HBITMAP nowBmp = LoadBitmapA(gHinstance,MAKEINTRESOURCEA(SOU_BMP_NOW));
    HANDLE nowBmp = LoadImageA(gHinstance,"./img/now.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    HWND nowBmpH = GetDlgItem(hwnd,ID_BMP_NOW);
    SendMessage(nowBmpH,STM_SETIMAGE,IMAGE_BITMAP,(long)nowBmp);
    return 1;
}
//剪裁图片
int CutImage(char * source){
    IplImage * sourceImg,* pDest;
    CvSize newSize;
    sourceImg = cvLoadImage(source,1);
    if(sourceImg == NULL){
        printf("图片创建失败\n");
        return 0;
    }
    newSize = cvSize(cRect.right-cRect.left,cRect.bottom-cRect.top);
    cvSetImageROI(sourceImg,cvRect(cRect.left,cRect.top,newSize.width,newSize.height));
    pDest = cvCreateImage(newSize,sourceImg->depth,sourceImg->nChannels);
    cvCopy(sourceImg,pDest,0);
    cvResetImageROI(sourceImg);
    cvSaveImage("./img/main_person.jpg",pDest);
    cvSaveImage("./img/main_person.bmp",pDest);
    return 1;
}
//jpg转bmp
int JpgToBmp(char * source,char * out){
    IplImage * sourceImg;
    sourceImg = cvLoadImage(source,1);
    cvSaveImage(out,sourceImg);
    return 1;
}
//创建弹窗
int ShowDialog(){
    LONG style;
    wHwnd = CreateDialog(gHinstance,MAKEINTRESOURCEA(DLG_MAIN),mHwnd,GialogProc);
    if(wHwnd == NULL){
        printf("error:%d\n",GetLastError());
    }
    style = GetWindowLongA(wHwnd,GWL_STYLE);
    style = style&(~WS_CAPTION)&(~WS_VSCROLL);
    SetWindowLongA(wHwnd,GWL_STYLE,style);
    ShowWindow(wHwnd,SW_SHOWDEFAULT);
    UpdateWindow(wHwnd);
    return 1;
}

