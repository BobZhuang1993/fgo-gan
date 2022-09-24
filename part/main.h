#include "head.h"
#include "resource.h"
#include "common.h"
#include "pic.h"
#include "add.h"
#include <pthread.h>
#include <dirent.h>
#include <string.h>

typedef struct {
    char * widget;
    char * wName;
    int wStyle;
    int wId;
    int x;
    int y;
    int width;
    int height;
    char * data[10];
} CONTROL;

#ifndef ID_JIE_BUTTON
#define ID_JIE_BUTTON 1002
#endif

#ifndef ID_EDIT_NUM
#define ID_EDIT_NUM 1003
#endif

#ifndef ID_CB_WORK
#define ID_CB_WORK 1004
#endif

#ifndef ID_CB_PERSON
#define ID_CB_PERSON 1005
#endif

#ifndef ID_GO
#define ID_GO 1006
#endif

#ifndef ID_S_MIAN_PERSON
#define ID_S_MIAN_PERSON 1007
#endif

#ifndef ID_MSG_BOX
#define ID_MSG_BOX 1008
#endif

#ifndef ID_ADD_ACTION
#define ID_ADD_ACTION 1009
#endif

#ifndef ID_STOP
#define ID_STOP 1010
#endif

#ifndef ID_ACTION_START
#define ID_ACTION_START 9000
#endif

#ifndef ID_ACTION_DELETE 
#define ID_ACTION_DELETE 9010
#endif

#ifndef W_WIDTH
#define W_WIDTH 720
#endif

#ifndef W_HEIGHT
#define W_HEIGHT 360
#endif

HWND hControls[5];
HWND aControls[5];
HWND bControls[5];
HBRUSH mBrush;

int res;
HMODULE userDll;
FARPROC mouseEvent;
HWND gHwnd;
int goNum;
int occupation;
int pType;
int pid;
char lbMsg[30];
int state = 0;


Skill skillList[TURN_NUM][12];
// = {
//     //caster 饼干 狂兰
//     // {{2,1,1,0},{3,1,1,0}},
//     // {{2,3,1,0},{1,3,0,0},{4,2,1,0}},
//     // {{3,3,1,0},{2,2,0,0},{3,2,0,0}}
//     //qp
//     // {{2,1,0,0},{2,3,0,0}},
//     // {{2,2,0,0},{3,3,2,0}},
//     // {{1,2,1,0},{1,3,0,0}}

//     //火种 弓凛
//     // {{1,1,0,0},{1,3,0,0},{2,3,1,0},{3,3,1,0}},
//     // {{1,2,0,0},{2,1,1,0},{1,3,0,0}},
//     // {{3,1,1,0},{1,1,0,0},{4,3,3,4},{3,1,0,0},{3,2,1,0},{3,3,1,0}}

//     //qp 弓凛
//     // {{1,1,0,0},{1,3,0,0},{2,3,1,0}},
//     // {{1,2,0,0},{2,1,1,0},{1,3,0,0}},
//     // {{3,1,0,0},{3,2,1,0},{3,3,1,0},{4,3,3,4},{3,1,0,0},{3,2,1,0},{4,1,0,0}}
// };

Attack attackList[TURN_NUM][3];
// = {
//     //caster 饼干 狂兰
//     // {{1,1},{0,2},{0,2}},
//     // {{1,1},{0,2},{0,2}},
//     // {{1,1},{0,2},{0,2}}
//     //qp
//     // {{2,1},{0,2},{0,2}},
//     // {{2,1},{0,2},{0,2}},
//     // {{1,1},{0,2},{0,2}}

//     //火种 弓凛
//     // {{1,1},{0,2},{0,2}},
//     // {{1,1},{0,2},{0,2}},
//     // {{1,1},{0,2},{0,2}}
// };

char * caster[10] = {
    "阿尔托莉亚",
    "斯卡哈"
};

char * assassin[10] = {
    "光之高扬斯卡娅",
};

char * rider[10] = {
    "随机",
};

CONTROL controls[] = {
    {"STATIC","打手图片",SS_CENTERIMAGE|SS_CENTER,NULL,10,30,80,30},
    {"STATIC","",SS_BITMAP|SS_NOTIFY|WS_BORDER|SS_CENTERIMAGE,ID_S_MIAN_PERSON,90,10,80,80},
    {"BUTTON","截图",BS_CENTER|BS_VCENTER,ID_JIE_BUTTON,10,95,80,30},
    {"STATIC","运行次数",SS_CENTERIMAGE|SS_CENTER,NULL,10,130,80,30},
    {"EDIT","",ES_NUMBER|ES_LEFT|WS_BORDER,ID_EDIT_NUM,100,135,140,20},
    {"STATIC","选择职介",SS_CENTERIMAGE|SS_CENTER,NULL,10,160,80,30},
    {"COMBOBOX","请选择",CBS_DROPDOWNLIST|CBS_DROPDOWN,ID_CB_WORK,100,165,140,100,{"caster","assassin","rider"}},
    {"STATIC","选择人物",SS_CENTERIMAGE|SS_CENTER,NULL,10,200,80,30},
    {"COMBOBOX","请选择",CBS_DROPDOWNLIST|CBS_DROPDOWN,ID_CB_PERSON,100,204,140,100,{}},
    {"LISTBOX","",WS_VSCROLL|WS_BORDER|LBS_DISABLENOSCROLL|LBS_NOSEL,ID_MSG_BOX,250,10,300,280},
    {"BUTTON","添加行动",BS_CENTER|BS_VCENTER,ID_ADD_ACTION,570,254,100,30},
    {"BUTTON","开肝",BS_CENTER|BS_VCENTER,ID_GO,10,245,80,30},
    {"BUTTON","收皮",BS_CENTER|BS_VCENTER,ID_STOP,100,245,80,30}
};

int CreateControls(HWND hwnd,WPARAM wParam,LPARAM lParam);
int ChangePersonOption(HWND hwnd,char ** arr);
int DeletePersonOption(HWND hwnd);
int Gan();
int ShowMainPersonImg();
int InsertToListbox(char * msg);
char ** ListActionDir();
int CreateActionList(HWND hwnd);
int InsertDeleteImg();
int DeleteAction(HWND hwnd,int id);
int ReadAction();
int SetActionFromStr(char * action,int turn);
int ClearSkillAttack();
int Go();
int Stop();
int SetInitState();
int VerifyInput();