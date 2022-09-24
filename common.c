#include "common.h"
//
char * GetPartFromStr(char * str,char * part){
    char * sp;
    char * strTmp = malloc(strlen(str));
    strcpy(strTmp,str);
    sp = strstr(strTmp,part);
    if(sp != NULL){
        memset(sp,0,strlen(sp));
    }
    return strTmp;
}
//截取指定的字符
char * GetPartFromCh(char * str,char ch){
    char * sp;
    char * strTmp = malloc(strlen(str));
    strcpy(strTmp,str);
    sp = strchr(strTmp,ch);
    if(sp != NULL){
        memset(sp,0,strlen(sp));
    }
    return strTmp;
}
// //获取进程id
int GetPIdFromPName(char * pName,int * pid){
    PROCESSENTRY32 pe32;
    int more;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE tlh = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if(tlh == INVALID_HANDLE_VALUE){
        return 1;
    }

    more = Process32First(tlh,&pe32);
    while(more){
        if(strcmp(pe32.szExeFile,pName) == 0){
            *pid = pe32.th32ProcessID;
            return 0;
        }
        more = Process32Next(tlh,&pe32);
    }
    return 1;
}

int callback(HWND hwnd,LPARAM lparam,int pid){
    int nPid;
    // char title[1024];
    char * title = malloc(1024);
    CBPARAM * p = (CBPARAM *)lparam;
    GetWindowThreadProcessId(hwnd,(PDWORD)&nPid);
    GetWindowTextA(hwnd,title,1024);
    if(nPid == p->pid && IsWindowVisible(hwnd) && title != ""){
        p->hwnd = hwnd;
        free(title);
        return 0;
    }
    free(title);
    return 1;
}

int GetAbsolutePos(HWND wh,int x,int y,MyAbsPos * ret){
    RECT wRect;
    int sw= GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    GetWindowRect(wh,&wRect);
    ret->x = (x + wRect.left) * 65536 / sw;
    ret->y = (y + NO_CHANGE_TOP + wRect.top) * 65536 / sh; 
    return 1;
}

//根据pid获取进程句柄
HWND GetHandleFromPid(int pid){
    HWND nwh;
    CBPARAM cbparam;
    cbparam.pid = pid;
    cbparam.hwnd = NULL;
    EnumWindows((WNDENUMPROC)callback,(LPARAM)&cbparam);
    return cbparam.hwnd;
}

//点击
void ClickRole(int x,int y,FARPROC func){
    func(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE,x,y,0,0);
    func(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
    func(MOUSEEVENTF_LEFTUP,0,0,0,0);
    Sleep(1000);
}

//滑动
void ScrollRole(int x,int y,int l,FARPROC func){
    func(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE,x,y,0,0);
    Sleep(500);
    func(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
    Sleep(1000);
    func(MOUSEEVENTF_MOVE,0,l,0,0);
    Sleep(1500);
    func(MOUSEEVENTF_LEFTUP,0,0,0,0);
    Sleep(1000);
    // for(int i = 0;i < 10;i++){
    //     func(MOUSEEVENTF_WHEEL|MOUSEEVENTF_ABSOLUTE,0,0,l,0);
    //     Sleep(1000);
    // }
}

//计算画面比率
float GetRatio(HWND hwnd){
    //计算放大或缩少的比例
    RECT wRect;

    GetWindowRect(hwnd,&wRect);
    float wh = wRect.bottom - wRect.top - NO_CHANGE_TOP;
    float ratio = wh / NORMAL_HEIGHT;
    return ratio;
}
//计算技能绝对位置
int GetSkillAbsPosition(HWND hwnd,int person,int num,MyAbsPos * ret){
    float ratio = 1.0;
    int x,y;
    if(person != 4){
        x = (person * PERSOPN_GAP + ((person - 1) * PERSOPN_WIDTH) + (num * SKILL_GAP) + ((num - 1) * SKILL_WIDTH) + SKILL_WIDTH / 2) * ratio;
        y = SKILL_TOP * ratio;
    }else{
        x = ((num * SKILL_GAP) + ((num - 1) * SKILL_WIDTH) + SKILL_WIDTH / 2 + NORMAL_MASTER_SKILL_X) * ratio;
        y = NORMAL_MASTER_Y * ratio;
    }
    GetAbsolutePos(hwnd,x,y,ret);
    return 1;
}
//计算人物替换的位置
int GetChangeAbsPosition(HWND hwnd,int num,MyAbsPos * ret){
    float ratio = 1.0;
    int x,y;
    x = ((num * NORMAL_CHNAGE_GAP) + ((num - 1) * NORMAL_CHNAGE_WIDTH) + NORMAL_CHNAGE_WIDTH / 2 + NORMAL_CHNAGE_X) * ratio;
    y = NORMAL_CHNAGE_Y * ratio;
    GetAbsolutePos(hwnd,x,y,ret);
    return 1;
}
//计算技能选择人物的绝对位置
int GetSkillSelectAbsPosition(HWND hwnd,int target,MyAbsPos * ret){
    float ratio = 1.0;
    int x,y;
    
    x = (target * SKILL_SELECT_GAP + ((target - 1) * SKILL_SELECT_WIDTH) + SKILL_SELECT_WIDTH / 2 + SKILL_SELECT_LEFT) * ratio;
    y = SKILL_SELECT_TOP * ratio;
    GetAbsolutePos(hwnd,x,y,ret);
    return 1;
}
//
//计算人物卡位置
int GetCardAbsPosition(HWND hwnd,int num,int type,MyAbsPos * ret){
    float ratio = 1.0;
    int x,y;
    int num_;
    if(type == 1){
        x = ((num + 1) * CARD_GAP + num * CARD_WIDTH + CARD_WIDTH / 2) * ratio;
        y = CARD_BAO_TOP * ratio;
    }else{
        x = (num * CARD_GAP + (num - 1) * CARD_WIDTH + CARD_WIDTH / 2) * ratio;
        y = CARD_NORMAL_TOP * ratio;
    }
    GetAbsolutePos(hwnd,x,y,ret);
    return 1;
}

//标志位置比较
int CmpMark(HWND hwnd,int type,int x,int y){
    int flag = 0;
    float ratio = 1.0;
    float x_l,x_r,y_t,y_b;
    switch (type)
    {
    case 1:
        //阵容界面
        x_l = START_BTN_POS_X_L * ratio;
        y_t = START_BTN_POS_Y_T * ratio;
        x_r = START_BTN_POS_X_R * ratio;
        y_b = START_BTN_POS_Y_B * ratio;
        break;
    case 2:
        //战斗界面
        x_l = NEPL_BTN_POS_X_L * ratio;
        y_t = NEPL_BTN_POS_Y_T * ratio;
        x_r = NEPL_BTN_POS_X_R * ratio;
        y_b = NEPL_BTN_POS_Y_B * ratio;
        break;
    case 3:
        //战斗界面
        x_l = STAR_POS_X * ratio;
        y_t = STAR_POS_Y * ratio;
        break;
    case 4:
        //战斗界面
        x_l = ZHOU_POS_X * ratio;
        y_t = ZHOU_POS_Y * ratio;
        break;
    case 5:
        //战斗界面
        x_l = MASTER_POS_X * ratio;
        y_t = MASTER_POS_Y * ratio;
        break;
    case 6:
        //战斗结果界面
        x_l = FINISH_POS_X_L * ratio;
        y_t = FINISH_POS_Y_T * ratio;
        x_r = FINISH_POS_X_R * ratio;
        y_b = FINISH_POS_Y_B * ratio;
        break;
    case 7:
        //碎石界面
        x_l = SHI_POS_X_L * ratio;
        y_t = SHI_POS_Y_T * ratio;
        x_r = SHI_POS_X_R * ratio;
        y_b = SHI_POS_Y_B * ratio;
        break;
    case 8:
        //选择助战界面
        x_l = ZHU_POS_X_L * ratio;
        y_t = ZHU_POS_Y_T * ratio;
        x_r = ZHU_POS_X_R * ratio;
        y_b = ZHU_POS_Y_B * ratio;
        break;
    case 9:
        //选择助战界面
        x_l = LI_POS_X_L * ratio;
        y_t = LI_POS_Y_T * ratio;
        x_r = LI_POS_X_R * ratio;
        y_b = LI_POS_Y_B * ratio;
        break;
    case 10:
        //选卡界面
        x_l = NORMAL_BACK_L * ratio;
        y_t = NORMAL_BACK_T * ratio;
        x_r = NORMAL_BACK_R * ratio;
        y_b = NORMAL_BACK_B * ratio;
        break;
    case 11:
        //好友申请界面
        x_l = FORFRIEND_POS_X_L * ratio;
        y_t = FORFRIEND_POS_Y_T * ratio;
        x_r = FORFRIEND_POS_X_R * ratio;
        y_b = FORFRIEND_POS_Y_B * ratio;
        break;
    default:
        x_l = -1;
        y_t = -1;
        x_r = -1;
        y_b = -1;
        break;
    }
    // printf("x:%d\n",x);
    // printf("y:%d\n",y);
    // printf("x_l:%f\n",x_l);
    // printf("y_t:%f\n",y_t);
    // printf("x_r:%f\n",x_r);
    // printf("y_b:%f\n",y_b);
    if(((x >= x_l) && (x <= x_r)) && ((y >= y_t) && (y <= y_b))){
        flag = 1;
    }

    return flag;
}
