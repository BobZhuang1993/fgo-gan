#include "add.h"

LRESULT WINAPI AddDialogProc(HWND hwnd,unsigned int msg,WPARAM wParam,LPARAM lParam){
    int saf;
    switch(msg){
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_CREATE:
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case ID_ADD_BTN:
                    saf = SaveAction(hwnd);
                    if(saf == 1){
                        SendMessage(mHwnd,WM_SUB_MSG,ID_ADD_ACTION_FINISH,0);
                        SendMessage(hwnd,WM_CLOSE,0,0);
                    }
                    break;
            }
            break;
    }
    return 0;
    // return DefWindowProc(hwnd,msg,wParam,lParam);
}
int SaveAction(HWND hwnd){
    char skill[3][50];
    char card[3][50];
    // char firstSkill[50],secondSkill[50],thirdSkill[50];
    // char firstCard[50],secondCard[50],thirdCard[50];
    char actionName[10];
    char fPath[20];

    int b;

    int skillId[] = {ID_FIRST_SKILL,ID_SECOND_SKILL,ID_THIRD_SKILL};
    int cardId[] = {ID_FIRST_CARD,ID_SECOND_CARD,ID_THIRD_CARD};
    FILE * fp;

    for(int i = 0;i < (sizeof(skillId) / sizeof(int));i++){
        GetDlgItemTextA(hwnd,skillId[i],skill[i],50);
        if((b = CheckAction(skill[i])) == 0){
            MessageBoxA(hwnd,"填写不符合要求","提示",MB_OK);
            return 0;
        }
    }
    for(int i = 0;i < (sizeof(cardId) / sizeof(int));i++){
        GetDlgItemTextA(hwnd,cardId[i],card[i],50);
        if((b = CheckAction(card[i])) == 0){
            MessageBoxA(hwnd,"填写不符合要求","提示",MB_OK);
            return 0;
        }
    }
    
    GetDlgItemTextA(hwnd,ID_ADD_NAME,actionName,10);
    
    if(strcmp(actionName,"") == 0){
        MessageBoxA(hwnd,"行动名不能为空","提示",MB_OK);
        return 0;
    }
    wsprintf(fPath,"./action/%s.txt",actionName);

    fp = fopen(fPath,"w+");
    for(int i = 0;i < (sizeof(skillId) / sizeof(int));i++){
        fprintf(fp,"&\n");
        fprintf(fp,"+%s\n",skill[i]);
        fprintf(fp,"-%s\n",card[i]);
    }
    fclose(fp);
    return 1;
}

int CheckAction(char * action){
    int ai;
    for(int i = 0;i < sizeof(action);i++){
        ai = (int)action[i];
        if(ai != 32 && ai != 44 && ai != 35 && (ai < 48 || ai >57)){
            return 0;
        }
    }
    return 1;
}

int ShowAddMain(){
    aHwnd = CreateDialog(gHinstance,MAKEINTRESOURCEA(ID_ADD_MAIN),mHwnd,AddDialogProc);
    if(aHwnd == NULL){
        printf("error:%d\n",GetLastError());
    }
    printf("aHwnd:%x\n",aHwnd);
    // STM_SETICON
    // HICON icon = LoadImageA(gHinstance,"./main.ico",IMAGE_ICON,20,20,LR_LOADFROMFILE);
    HICON icon = LoadIconA(gHinstance,MAKEINTRESOURCEA(ID_ADD_ICON));
    SendMessageA(aHwnd,WM_SETICON,ICON_SMALL,(long)icon);
    long style = GetWindowLongA(aHwnd,GWL_STYLE);
    style = style & (~WS_MAXIMIZEBOX);
    SetWindowLongA(aHwnd,GWL_STYLE,style);
    ShowWindow(aHwnd,SW_SHOWDEFAULT);
    UpdateWindow(aHwnd);
    return 1;
}