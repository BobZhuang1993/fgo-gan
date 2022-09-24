#include "main.h"

LRESULT WINAPI WinProc(HWND hwnd,unsigned int msg,WPARAM wParam,LPARAM lParam){
    HDC cHdc;
    switch(msg){
        case WM_DESTROY:
            DeleteObject(mBrush);
            ReleaseDC(NULL,cHdc);
            PostQuitMessage(0);
            InsertToListbox("�رմ���");
            break;
        case WM_LBUTTONDOWN:
            // printf("�������\n");
            break;
        case WM_CREATE:
            gHinstance = ((LPCREATESTRUCT)lParam)->hInstance;
            CreateControls(hwnd,wParam,lParam);
            CreateActionList(hwnd);
            ShowMainPersonImg();
            EnableWindow(hControls[ID_STOP],0);
            mBrush = CreateSolidBrush(RGB(255,255,255));
            // AjustWinPos();
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case ID_JIE_BUTTON:
                    GetScreenImg(gHwnd,"./img","now");
                    JpgToBmp("./img/now.jpg","./img/now.bmp");
                    ShowDialog();
                    break;
                case ID_CB_WORK:
                    if(HIWORD(wParam) == CBN_SELCHANGE){
                        occupation = SendMessage(hControls[ID_CB_WORK],CB_GETCURSEL,0,0);
                        // char * sName = calloc(1,20);
                        // SendMessage(hControls[ID_CB_WORK],CB_GETLBTEXT,iIndex,sName);
                        // printf("iIndex:%s\n",sName);
                        // printf("ѡ��\n");
                        // free(sName);
                        //�л�ʱѡ������任
                        DeletePersonOption(hControls[ID_CB_PERSON]);
                        if(occupation == 0){
                            //caster
                            ChangePersonOption(hControls[ID_CB_PERSON],caster);
                        }else if(occupation == 1){
                            //rider
                            ChangePersonOption(hControls[ID_CB_PERSON],assassin);
                        }else if(occupation == 2){
                            //rider
                            ChangePersonOption(hControls[ID_CB_PERSON],rider);
                        }
                        occupation++;
                    }
                    break;
                case ID_CB_PERSON:
                    if(HIWORD(wParam) == CBN_SELCHANGE){
                        pType = SendMessage(hControls[ID_CB_PERSON],CB_GETCURSEL,0,0);
                        pType++;
                    }
                    break;
                case ID_EDIT_NUM:
                    break;
                case ID_ADD_ACTION:
                    ShowAddMain();
                    break;
                case ID_GO:
                    //��ʼ
                    Go();
                    //��ȡ�ж��ļ�
                    break;
                case ID_STOP:
                    Stop();
                    break;
                case ID_ACTION_DELETE:
                case ID_ACTION_DELETE+1:
                case ID_ACTION_DELETE+2:
                case ID_ACTION_DELETE+3:
                case ID_ACTION_DELETE+4:
                    DeleteAction(hwnd,LOWORD(wParam)-ID_ACTION_DELETE);
                    break;
            }
            break;
        case WM_SUB_MSG:
            switch(LOWORD(wParam)){
                case ID_SHOW_CUT:
                    ShowMainPersonImg();
                    break;
                case ID_ADD_ACTION_FINISH:
                    DeleteNowActionList();
                    CreateActionList(hwnd);
                    break;
            }
        case WM_CTLCOLORSTATIC:
            cHdc = (HDC)wParam; 
            SetTextColor(cHdc,RGB(0,0,0));
            SetBkColor(cHdc,RGB(255,255,255));
            return mBrush;
            break;
        
    }
    return DefWindowProc(hwnd,msg,wParam,lParam);
}

//��ͼ���Ա�
int getAndCompare(HWND hwnd,char * target,MyPos * loc){
    GetScreenImg(hwnd,"./img","now");
    CmpImage("./img/now.jpg",target,loc);
    return 1;
}

int waitForImg(HWND hwnd,char * target,int type,int time,int timeout){
    MyPos * locTmp = malloc(sizeof(MyPos));
    int timeTmp = 0;
    while(1){
        getAndCompare(hwnd,target,locTmp);
        if(CmpMark(hwnd,type,locTmp->x,locTmp->y) == 1){
            break;
        }
        Sleep(time);
        timeTmp += time;
        if(timeout != 0 && timeTmp >= timeout){
            //��ʱ
            return 0;
        }
    }
    free(locTmp);
    return 1;
}

//����������
void scrollAndFind(HWND hwnd,char * pImg,FARPROC mouseEvent){
    MyPos * locTmp = malloc(sizeof(MyPos));
    MyAbsPos * myAbsLocTmp = malloc(sizeof(MyAbsPos));
    while(1){
        getAndCompare(hwnd,pImg,locTmp);
        if(CmpMark(hwnd,9,locTmp->x,locTmp->y) == 1){
            GetAbsolutePos(hwnd,locTmp->x,locTmp->y,myAbsLocTmp);
            ClickRole(myAbsLocTmp->x,myAbsLocTmp->y,mouseEvent);
            break;
        }
        //����
        getAndCompare(hwnd,"./img/scroll.png",locTmp);
        GetAbsolutePos(hwnd,locTmp->x,locTmp->y+30,myAbsLocTmp);
        if(locTmp->y >= NORMAL_ZHU_SCROLL_BOTTOM){
            GetAbsolutePos(hwnd,NORMAL_ZHU_SCROLL_LEFT,NORMAL_ZHU_SCROLL_TOP,myAbsLocTmp);
            ClickRole(myAbsLocTmp->x,myAbsLocTmp->y,mouseEvent);
            Sleep(1000);
            GetAbsolutePos(hwnd,NORMAL_ZHU_X,NORMAL_ZHU_Y,myAbsLocTmp);
            ClickRole(myAbsLocTmp->x,myAbsLocTmp->y,mouseEvent);
            break;
        }else{
            // printf("moveY:%d\n",NORMAL_ZHU_MOVE_Y);
            ScrollRole(myAbsLocTmp->x,myAbsLocTmp->y,NORMAL_ZHU_MOVE_Y,mouseEvent);
            
        }
    }
    free(locTmp);
    free(myAbsLocTmp);
}

//��ȡ�����
int getRandCard(int * numL){
    int num_ = 0;
    while(1){
        num_ = rand() % 5 + 1;
        if(num_ == *numL){
            continue;
        }
        *numL = num_;
        break;
    }
    return num_;
}

//����
int chargeCart(HWND hwnd,int * numL,FARPROC mouseEvent){
    MyPos * locTmp = malloc(sizeof(MyPos));
    MyAbsPos * myAbsLocTmp = malloc(sizeof(MyAbsPos));
    while(1){
        Sleep(1000);
        getAndCompare(hwnd,"./img/back.png",locTmp);
        if(CmpMark(hwnd,10,locTmp->x,locTmp->y) == 0){
            break;
        }
        InsertToListbox("ȱ�������в���");
        GetCardAbsPosition(hwnd,getRandCard(numL),2,myAbsLocTmp);
        ClickRole(myAbsLocTmp->x,myAbsLocTmp->y,mouseEvent);
    }
    free(locTmp);
    free(myAbsLocTmp);
}

//��ʾ��ͼ
int ShowMainPersonImg(){
    HANDLE personImg = LoadImageA(gHinstance,"./img/main_person.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    SendMessageA(hControls[ID_S_MIAN_PERSON],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)personImg);
    UpdateWindow(hControls[ID_S_MIAN_PERSON]);
    return 1;
}

//��listbox ��� ��Ϣ
int InsertToListbox(char * msg){
    SendMessageA(hControls[ID_MSG_BOX],LB_INSERTSTRING,0,msg);
    UpdateWindow(hControls[ID_MSG_BOX]);
    return 1;
}

//�г�Ŀ¼
char ** ListActionDir(){
    static char * fList[5];
    char * dirPath = "./action/";
    DIR * dp;
    int index = 0;
    struct dirent * fp;
    dp = opendir(dirPath);
    
    for(int i = 0;i < 5;i++){
        // memset(,0,sizeof(fList[i]));
        fList[i] = NULL;
    }

    while((fp = readdir(dp)) != NULL){
        if(strcmp(fp->d_name,".") != 0 && strcmp(fp->d_name,"..") != 0){
            fList[index] = GetPartFromCh(fp->d_name,'.');
            index++;
        }
    }
    closedir(dp);
    return fList;
}

//���������б�
int CreateActionList(HWND hwnd){
    char ** actionList = ListActionDir();
    int y = 10;
    long bs = BS_AUTORADIOBUTTON|WS_CHILD|WS_VISIBLE;
    int ln = 0;
    for(int i = 0;i < 5;i++){
        if(*(actionList+i) != NULL){
            if(i == 0){
                aControls[ID_ACTION_START+i] = CreateWindowEx(NULL,"BUTTON",*(actionList+i),bs|WS_GROUP,560,y + i * 25,100,20,hwnd,(HMENU)(ID_ACTION_START+i),gHinstance,0);
            }else{
                aControls[ID_ACTION_START+i] = CreateWindowEx(NULL,"BUTTON",*(actionList+i),bs,560,y + i * 25,100,20,hwnd,(HMENU)(ID_ACTION_START+i),gHinstance,0);
            }
            
            ShowWindow(aControls[ID_ACTION_START+i],SW_SHOW);
            bControls[ID_ACTION_DELETE+i] = CreateWindowEx(NULL,"STATIC","",SS_NOTIFY|SS_BITMAP|SS_CENTERIMAGE|WS_CHILD|WS_VISIBLE,670,y + i * 25,20,20,hwnd,(HMENU)(ID_ACTION_DELETE+i),gHinstance,0);
            ShowWindow(bControls[ID_ACTION_DELETE+i],SW_SHOW);
            // free(*(actionList[i]));
            ln++;
        }
    }
    if(ln >= 5){
        EnableWindow(hControls[ID_ADD_ACTION],0);
    }else{
        EnableWindow(hControls[ID_ADD_ACTION],1);
    }
    InsertDeleteImg();
    return 1;
}

//���ɾ��ͼƬ
int InsertDeleteImg(){
    HBITMAP delBtn = LoadImageA(gHinstance,"./img/delete.bmp",IMAGE_BITMAP,16,16,LR_LOADFROMFILE);
    for(int i = 0;i < 5;i++){
        SendMessage(bControls[ID_ACTION_DELETE+i],STM_SETIMAGE,IMAGE_BITMAP,(long)delBtn);
    }
    return 1;
}

int CreateControls(HWND hwnd,WPARAM wParam,LPARAM lParam){
    int num = sizeof(controls) / sizeof(CONTROL);
    HINSTANCE hinstance = ((LPCREATESTRUCT)lParam)->hInstance;
    for(int i = 0;i < num;i++){
        hControls[controls[i].wId] = CreateWindowEx(NULL,controls[i].widget,controls[i].wName,controls[i].wStyle|WS_CHILD|WS_VISIBLE|WS_GROUP,controls[i].x,controls[i].y,controls[i].width,controls[i].height,hwnd,(HMENU)controls[i].wId,gHinstance,0);
        ShowWindow(hControls[controls[i].wId],SW_SHOW);
        if(strcmp(controls[i].widget,"COMBOBOX") == 0){
            // SendMessage(hControls[controls[i].wId], CB_ADDSTRING, 0, (LPARAM)TEXT("caster"));
            // SendMessage(hControls[controls[i].wId], CB_ADDSTRING, 1, (LPARAM)TEXT("rider"));
            for(int z = 0;z < 10;z++){
                if(controls[i].data[z] == NULL){
                    break;
                }
                SendMessage(hControls[controls[i].wId], CB_ADDSTRING, 0, (LPARAM)controls[i].data[z]);
            }
        }
        
    }
    return 1;
}

//�任�����������ֵ
int ChangePersonOption(HWND hwnd,char ** arr){
    
    for(int i = 0;i < 10;i++){
        if(arr[i] == NULL){
            break;
        }
        SendMessage(hwnd,CB_ADDSTRING,0,arr[i]);
    }
    UpdateWindow(hwnd);
    return 1;
}

int DeletePersonOption(HWND hwnd){
    char * zeroText = calloc(1,50);
    for(int i = 0;i < 10;i++){
        SendMessage(hwnd,CB_GETLBTEXT,0,zeroText);
        if(strcmp(zeroText,"") == 0){
            break;
        }
        SendMessage(hwnd,CB_DELETESTRING,0,0);
        memset(zeroText, 0, sizeof(zeroText));
    }
    free(zeroText);
    return 1;
}

int DeleteNowActionList(){
    for(int i = 0;i < sizeof(aControls) / sizeof(HWND);i++){
        DestroyWindow(aControls[ID_ACTION_START+i]);
    }
    for(int i = 0;i < sizeof(bControls) / sizeof(HWND);i++){
        DestroyWindow(bControls[ID_ACTION_DELETE+i]);
    }
    return 1;
}

//�����ж��ַ���
int SetActionFromStr(char * action,int turn){
    Skill * skillP;
    Attack * attackP;

    int elList[4];
    int index = 0;
    int group = 0;

    int type = 0;

    char one;

    while(one = *action){
        if(one == ','){
            index++;
        }else if(one == '+'){
            type = 1;
            // skillP = malloc(sizeof(Skill));
           
        }else if(one == '-'){
            type = 2;
        }else if(one >= '0' && one <= '9'){
            elList[index] = (int)(one - '0');
        }else if(one == '#'){
            if(type == 1){
                skillP = malloc(sizeof(Skill));
                skillP->person = elList[0];
                skillP->num = elList[1];
                skillP->target = elList[2];
                skillP->change = elList[3];
                skillList[turn][group] = *skillP;
                
            }else if(type == 2){
                attackP = malloc(sizeof(Attack));
                attackP->num = elList[0];
                attackP->type = elList[1];
                attackList[turn][group] = *attackP;
            }
            group++;
            index = 0;
        }
        action++;
    }
    return 1;
}

//
int DeleteAction(HWND hwnd,int id){
    char radioText[50];
    char fp[50];
    SendMessage(aControls[ID_ACTION_START+id],WM_GETTEXT,50,radioText);
    wsprintf(fp,"./action/%s.txt",radioText);
    remove(fp);
    //
    DeleteNowActionList();
    CreateActionList(hwnd);
    return 1;
}

//
int ClearSkillAttack(){
    for(int i = 0;i < 3;i++){
        for(int z =0;z < 12;z++){
            skillList[i][z].person = 0;
            skillList[i][z].num = 0;
            skillList[i][z].target = 0;
            skillList[i][z].change = 0;
        }
        for(int z =0;z < 3;z++){
            attackList[i][z].num = 0;
            attackList[i][z].type = 0;
        }
    }
    return 1;
}

//
int ReadAction(){
    int rCheck;
    int i;
    char nowAction[50];
    char fp[50];
    FILE * f;
    int fr;
    char buf[50];
    char * bufNoN;
    int turn = -1;
    //��ȡѡ�е��ж�
    for(i = 0;i < sizeof(aControls)/sizeof(HWND);i++){
        rCheck = SendMessage(aControls[ID_ACTION_START+i],BM_GETCHECK,0,0);
        if(rCheck == 1){
            break;
        }
    }
    SendMessage(aControls[ID_ACTION_START+i],WM_GETTEXT,50,nowAction);
    wsprintf(fp,"./action/%s.txt",nowAction);
    //
    f = fopen(fp,"r");
    if(f == NULL){
        // MessageBoxA(NULL,"ѡ�񷽰�������","��ʾ",MB_OK);
        return 0;
    }

    while(!feof(f)){
        fgets(buf,50,f);
        if(strcmp(buf,"") == 0){
            continue;
        }
        bufNoN = GetPartFromStr(buf,"\n");
        if(strcmp(bufNoN,"&") == 0){
            turn++;
        }else{
            SetActionFromStr(bufNoN,turn);
            //���ַ���������������
        }
        
    }
    fclose(f);
    return 1;
}

//��֤
int VerifyInput(){
    int r;
    char goStr[3];

    SendMessage(hControls[ID_EDIT_NUM],WM_GETTEXT,3,goStr);
    goNum = atoi(goStr);
    r = ReadAction();

    if(r == 1 && goNum > 0 && occupation > 0 && pType > 0){
        return 1;
    }
    return 0;
}
//
int SetInitState(){
    EnableWindow(hControls[ID_JIE_BUTTON],1);
    EnableWindow(hControls[ID_GO],1);
    EnableWindow(hControls[ID_EDIT_NUM],1);
    EnableWindow(hControls[ID_CB_WORK],1);
    EnableWindow(hControls[ID_CB_PERSON],1);
    SendMessage(hControls[ID_GO],WM_SETTEXT,50,"����");
}
//
int Stop(){
    if(state == 1){
        state = 0;
        InsertToListbox("���ֽ�����ֹͣ�ж�");
        EnableWindow(hControls[ID_STOP],0);
    }
    return 1;
}
//
int Go(){
    int r;
    pthread_t g_t;

    r = VerifyInput();

    if(r == 1){
        // SendMessage(hControls[ID_GO],BM_SETSTATE,1,0);
        state = 1;
        EnableWindow(hControls[ID_JIE_BUTTON],0);
        EnableWindow(hControls[ID_GO],0);
        EnableWindow(hControls[ID_EDIT_NUM],0);
        EnableWindow(hControls[ID_CB_WORK],0);
        EnableWindow(hControls[ID_CB_PERSON],0);
        EnableWindow(hControls[ID_STOP],1);
        SendMessage(hControls[ID_GO],WM_SETTEXT,50,"������");
        pthread_create(&g_t,NULL,Gan,NULL);
    }else{
        MessageBox(mHwnd,"��Ϣ��д��ȫ","��ʾ",MB_OK);
        return 0;
    }
    return 1;
}
//
int Gan(){
    MyPos * loc = malloc(sizeof(MyPos));
    MyAbsPos * myAbsLoc = malloc(sizeof(MyAbsPos));;

    char go[2];
    int scrollTop = 0;
    char pImg[30];
    int numL,num_;

    if(occupation == 1){
        sprintf(pImg,"./img/person/caster/%d.png",pType);
    }else if(occupation == 2){
        sprintf(pImg,"./img/person/assassin/%d.png",pType);
    }else if(occupation == 3){
        sprintf(pImg,"./img/gift_dress/1.png");
    }
    //��ʼ
    //���ж��Ƿ���ѡ�˽���
    InsertToListbox("�ȴ�ѡ�˽���...");
    while(1){
        getAndCompare(gHwnd,"./img/start.png",loc);
        if(CmpMark(gHwnd,1,loc->x,loc->y) == 1){
            break;
        }
        Sleep(2000);
    }
    InsertToListbox("��ǰλ��ѡ�˽���");
    //ѡ��ս

    //��ʼ
    GetAbsolutePos(gHwnd,loc->x,loc->y,myAbsLoc);
    ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);

    for(int n = 0;n < goNum;n++){
        if(state == 0){
            InsertToListbox("�����ֶ�ֹͣ");
            break;
        }
        if(n > 0){
            InsertToListbox("��������");
            GetAbsolutePos(gHwnd,NORMAL_NO_STOP_X,NORMAL_NO_STOP_Y,myAbsLoc);
            ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
            Sleep(1000);
            //ѡ����ս
            InsertToListbox("�ȴ�������ս����");
            if(waitForImg(gHwnd,"./img/zhuzhan.png",8,1000,5000) == 0){
                //��������������㻭��
                getAndCompare(gHwnd,"./img/shi.png",loc);
                if(CmpMark(gHwnd,7,loc->x,loc->y) == 1){
                    InsertToListbox("ȱ��������׼����ƻ��");
                    //��ƻ��
                    GetAbsolutePos(gHwnd,NORMAL_APPLE_X,NORMAL_APPLE_Y,myAbsLoc);
                    ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                    Sleep(1000);
                    //ȷ��ƻ��
                    GetAbsolutePos(gHwnd,NORMAL_APPLE_SURE_X,NORMAL_APPLE_SURE_Y,myAbsLoc);
                    ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                    Sleep(2000);
                }
                waitForImg(gHwnd,"./img/zhuzhan.png",8,1000,0);
            };
            InsertToListbox("������ս����");
            Sleep(2000);
            //
            if(occupation == 3){
                //ѡ��qp��װ
                ////�л���rider
                GetAbsolutePos(gHwnd,RIDER_POS_X,OCCUPATION_POS_Y,myAbsLoc);
            }else if(occupation == 1){
                GetAbsolutePos(gHwnd,CASTER_POS_X,OCCUPATION_POS_Y,myAbsLoc); 
            }else if(occupation == 2){
                GetAbsolutePos(gHwnd,ASSASSIN_POS_X,OCCUPATION_POS_Y,myAbsLoc);
            }
            ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
            Sleep(1000);
            scrollAndFind(gHwnd,pImg,mouseEvent);
        }
        // ����ս������
        InsertToListbox("�ȴ�ս������...");
        Sleep(2000);
        for(int i = 0;i < TURN_NUM;i++){
            while(1){
                // loc = getAndCompare(hwnd,"./img/master.png");
                // if(loc == NULL){
                //     printf("ս������ͼ��ʶ�������־ʧ��,886\n"); 
                //     free(loc);
                //     return 1;
                // }
                // if(CmpMark(hwnd,5,loc->x,loc->y) == 0){
                //     free(loc);
                //     Sleep(1000);
                //     continue;
                // }
                // free(loc);
                getAndCompare(gHwnd,"./img/next_pliase.png",loc);
                if(CmpMark(gHwnd,2,loc->x,loc->y) == 1){
                    break;
                }
                Sleep(1000);
            }
            wsprintf(lbMsg,"��ǰ���ڵ�%d�غ�ս������",i+1);
            InsertToListbox(lbMsg);
            Sleep(1000);
            //�ͷ���Ӧ����
            for(int z = 0;z < 12;z++){
                if(skillList[i][z].person == 0){
                    break;
                }
                if(skillList[i][z].person == 4){
                    //�ͷ�master����
                    GetAbsolutePos(gHwnd,NORMAL_MASTER_X,NORMAL_MASTER_Y,myAbsLoc);
                    ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                }
                //�����ͷż���
                GetSkillAbsPosition(gHwnd,skillList[i][z].person,skillList[i][z].num,myAbsLoc);
                ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                if(skillList[i][z].change != 0){
                    InsertToListbox("������");
                    //���˼���
                    Sleep(1000);
                    GetChangeAbsPosition(gHwnd,skillList[i][z].target,myAbsLoc);
                    ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                    GetChangeAbsPosition(gHwnd,skillList[i][z].change,myAbsLoc);
                    ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                    //ȷ��
                    GetAbsolutePos(gHwnd,NORMAL_CHNAGE_SURE_X,NORMAL_CHNAGE_SURE_Y,myAbsLoc);
                    ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                }if(skillList[i][z].target != 0){
                    //ѡ�˼���
                    GetSkillSelectAbsPosition(gHwnd,skillList[i][z].target,myAbsLoc);
                    ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                }
                Sleep(2000);
                waitForImg(gHwnd,"./img/next_pliase.png",2,1000,0);
            }
            //�������
            GetAbsolutePos(gHwnd,loc->x,loc->y,myAbsLoc);
            ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
            //�����
            InsertToListbox("��ʼѡ��...");
            numL = 0;
            for(int z = 0;z < 3;z++){
                if(attackList[i][z].num == 0){
                    attackList[i][z].num = getRandCard(&numL);
                }
                GetCardAbsPosition(gHwnd,attackList[i][z].num,attackList[i][z].type,myAbsLoc);
                ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
            }
            //�ж��Ƿ���3�ſ�
            chargeCart(gHwnd,&numL,mouseEvent);
            InsertToListbox("ս����ʼ...");
            Sleep(3000);
        }
        //�ж��Ƿ���Ҫ����
        
        while(1){
            Sleep(2000);
            InsertToListbox("�м��Ƿ�ս������...");
            //�Ƿ���ս������
            getAndCompare(gHwnd,"./img/next_pliase.png",loc);
            if(CmpMark(gHwnd,2,loc->x,loc->y) == 1){
                InsertToListbox("�˺����㣬���в���...");
                GetAbsolutePos(gHwnd,loc->x,loc->y,myAbsLoc);
                ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                Sleep(1000);
                for(int z = 0;z < 3;z++){
                    if(z == 0){
                        GetCardAbsPosition(gHwnd,1,1,myAbsLoc);
                        ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                    }
                    GetCardAbsPosition(gHwnd,getRandCard(&numL),2,myAbsLoc);
                    ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
                }
                chargeCart(gHwnd,&numL,mouseEvent);
            }
            //�Ƿ��ڽ�������
            getAndCompare(gHwnd,"./img/main_person.jpg",loc);
            // printf("loc->x:%d\n",loc->x);
            // printf("loc->y:%d\n",loc->y);
            if(CmpMark(gHwnd,6,loc->x,loc->y) == 1){
                break;
            }
        }
        InsertToListbox("ս������");
        Sleep(2000);
        GetAbsolutePos(gHwnd,NORMAL_CLICK_X,NORMAL_CLICK_Y,myAbsLoc);
        ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
        Sleep(1000);
        //
        InsertToListbox("ս�����㻭��");
        GetAbsolutePos(gHwnd,NORMAL_CLICK_X,NORMAL_CLICK_Y,myAbsLoc);
        ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
        Sleep(1000);

        //
        InsertToListbox("ս����������");
        GetAbsolutePos(gHwnd,NORMAL_NEXT_X,NORMAL_NEXT_Y,myAbsLoc);
        ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
        Sleep(2000);

        //
        getAndCompare(gHwnd,"./img/zhuzhan.png",loc);
        // printf("loc->x:%d\n",loc->x);
        // printf("loc->y:%d\n",loc->y);
        if(CmpMark(gHwnd,11,loc->x,loc->y) == 1){
            InsertToListbox("������ѽ���");
            GetAbsolutePos(gHwnd,FORFRIEND_BTN_X,FORFRIEND_BTN_Y,myAbsLoc);
            ClickRole(myAbsLoc->x,myAbsLoc->y,mouseEvent);
            Sleep(1000);
        }
        wsprintf(lbMsg,"����ȫ%d�֣���ʣ%d��\n",n+1,goNum-n-1);
        InsertToListbox(lbMsg);
    }
    CloseHandle(gHwnd);
    free(loc);
    free(myAbsLoc);
    SetInitState();
    return 0;
}

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hprevInstance,LPSTR lpCmdLine,int nShowCmd){
    char * cName = "FGO����Ƭ";
    char * wName = "FGO����Ƭ";

    int ww = GetSystemMetrics(SM_CXFULLSCREEN);
    int wh = GetSystemMetrics(SM_CYFULLSCREEN);

    WNDCLASSEX wc;
    int mwx,mwy;
    MSG msg;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.style = CS_HREDRAW|CS_VREDRAW;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = NULL;
    wc.hIcon = LoadIconA(hinstance,MAKEINTRESOURCEA(ID_ICON));
    wc.hIconSm = NULL;
    wc.hInstance = hinstance;
    wc.lpfnWndProc = WinProc;
    wc.lpszClassName = cName;
    wc.lpszMenuName = MAKEINTRESOURCEA(MY_MENU);

    RegisterClassEx(&wc);

    mwx = (ww - W_WIDTH) / 2;
    mwy = (wh - W_HEIGHT) / 2;

    // mHwnd = CreateDialogParamA(hinstance,MAKEINTRESOURCEA(DLG_MAIN),NULL,GialogProc,0);
    mHwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,cName,wName,WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX&~WS_THICKFRAME,mwx,mwy,W_WIDTH,W_HEIGHT,NULL,NULL,hinstance,0);
    if(mHwnd == NULL){
        MessageBoxA(NULL,"��������ʧ��","��ʾ",MB_OK);
    }
    printf("mHwnd:%x\n",mHwnd);
    ShowWindow(mHwnd,nShowCmd);
    UpdateWindow(mHwnd);

    //�������ģ��
    userDll = LoadLibraryA("user32.dll");
    mouseEvent = GetProcAddress(userDll,"mouse_event");

    srand((unsigned int)time(0));

    res = GetPIdFromPName("Nox.exe",&pid);
    if(res == 1){
        MessageBoxA(NULL,"��Ӧ����û�д�!","��ʾ",MB_OK);
        return 1;
    }
    printf("ָ�����ڽ���ID:%d\n",pid);
    gHwnd = GetHandleFromPid(pid);
    // WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE),"hello",strlen("hello"),NULL,NULL);
    printf("ָ�����ھ��:%x\n",gHwnd);

    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.message;
}
