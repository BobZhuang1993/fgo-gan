#include "head.h"
#include <tlhelp32.h>
#include <time.h>
#include <string.h>

#include "config.h"

typedef struct
{
    int pid;
    HWND hwnd;
} CBPARAM;

typedef struct
{
    int x;
    int y;
} MyAbsPos;

typedef struct
{
    int person;     //���4Ϊmaster
    int num;        //�ڼ�����
    int target;     //�����ͷŶ���
    int change;     //����ר��
} Skill;

typedef struct
{
    int num;        //Ϊ�ڼ���
    int type;       //1���߿���2��ͨ��
} Attack;

int GetPIdFromPName(char * pName,int * pid);
HWND GetHandleFromPid(int pid);
int GetAbsolutePos(HWND wh,int x,int y,MyAbsPos * ret);
void ClickRole(int x,int y,FARPROC func);
int GetSkillAbsPosition(HWND hwnd,int person,int num,MyAbsPos * ret);
int GetChangeAbsPosition(HWND hwnd,int num,MyAbsPos * ret);
int GetSkillSelectAbsPosition(HWND hwnd,int target,MyAbsPos * ret);
int CmpMark(HWND hwnd,int type,int x,int y);
int GetCardAbsPosition(HWND hwnd,int num,int type,MyAbsPos * ret);
void ScrollRole(int x,int y,int l,FARPROC func);
char * GetPartFromCh(char * str,char ch);
char * GetPartFromStr(char * str,char * part);