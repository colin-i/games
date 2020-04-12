
#include<Windows.h>
#include<stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char* keys[]={"do","re","mi","fa","sol","la","si","doHigh",0};
char key_data[8][14000];
short channels;int sample_rate;short bit_rate;
//void main(){
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
	char file_fullpath[MAX_PATH];
	for(int i=0;keys[i]!=0;i++){
		sprintf(file_fullpath,"C:/Desktop/universe/solfege/%s.wav",keys[i]);
		FILE* f=fopen(file_fullpath,"rb");
		if(i==0){
			fseek(f,0x14,SEEK_SET);fread(&channels,2,1,f);
			fseek(f,0x18,SEEK_SET);fread(&sample_rate,4,1,f);
			fseek(f,0x22,SEEK_SET);fread(&bit_rate,2,1,f);
		}
		fseek(f,0x28,SEEK_SET);
		int soundSz;fread(&soundSz,4,1,f);
		fread(key_data[i],1,soundSz,f);
		fclose(f);
	}
	//
	static TCHAR szWindowClass[] = "win32app";
	WNDCLASSEX wcex={sizeof(WNDCLASSEX),CS_HREDRAW | CS_VREDRAW,WndProc,0,0,hInstance,0,LoadCursor(NULL, IDC_ARROW),
					CreateSolidBrush(RGB(223,223,223)),0,szWindowClass,0};
	if(!RegisterClassEx(&wcex)){
		MessageBox(NULL,"Call to RegisterClassEx failed!","SS Recorder",MB_ICONERROR);
		return 0;
	}
	HWND hWnd = CreateWindowEx
	(
		0, szWindowClass, "Sound", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
		960,640,
			//720
		NULL, 0, hInstance, NULL
	);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	// Main message loop:
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(!IsDialogMessage(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 1;
}
int win_command;int win_y=10;
//
#define key_wd 60
int key_buttons_x=10;
//
#define key_com 0
int key_com_max;
int keyStyles=WS_TABSTOP|WS_GROUP;
void add_key(char* key,HWND hwndMain){
	CreateWindowEx(0,"BUTTON",key,
		WS_CHILD|WS_VISIBLE|keyStyles
		,key_buttons_x,win_y,key_wd,20,hwndMain,(HMENU)win_command,0,0);
	key_buttons_x+=key_wd+10;
	keyStyles=0;
	win_command++;
}
#define msec_com 100
int msec_com_max;
#define sound_view 200
#define sound_fromFile 300
#define sound_reset 350
#define save_file 400
#define export_wav 500
HWND hwnd_allSz;
void saveName(void (*pt2Func)(char*));
void export_to_wav(char*);
void save_to_file(char*);
#define nr_of_rows 6
HWND hwnd_rows_sel[nr_of_rows];
HWND hwnd_rows_data[nr_of_rows];
HWND hwnd_rows_sz[nr_of_rows];
int sound_fromFile_cursor=sound_fromFile;int sound_fromFile_max;
int sound_reset_cursor=sound_reset;int sound_reset_max;
int nr_of_secRadio=0;
#define cam10butoaneDarPun100 100
HWND hwnd_msec[cam10butoaneDarPun100];
void add_time(HWND,char*);
#define text_data_sz 100*1000
char text_data[text_data_sz];
#define inter_rows_nuNumaiSpatiul 50
void allSize_time_add(HWND);
void allSize_time_subtract(HWND);
LRESULT CALLBACK WndProc(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
	case WM_CREATE:{
			win_command=key_com;
			for(int i=0;keys[i]!=0;i++){
				add_key(keys[i],hwndMain);
			}
			key_com_max=win_command-1;
			//
			int msec_wd=50;int msec_x=10;int msec_nr=200;char nrStr[10];
			win_command=msec_com;win_y+=inter_rows_nuNumaiSpatiul;
			int radioHead=WS_GROUP;
			for(int nr=200;nr<=1200;nr+=100){
				sprintf(nrStr,"%u",nr);
				hwnd_msec[nr_of_secRadio]=CreateWindowEx(0,"BUTTON",nrStr,WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|radioHead,msec_x,win_y,msec_wd,20,hwndMain,(HMENU)win_command,0,0);
				if(nr==1000)SendMessage(hwnd_msec[nr_of_secRadio],BM_SETCHECK,BST_CHECKED,0);
				nr_of_secRadio++;
				win_command++;
				msec_x+=msec_wd+10;
				radioHead=0;
			}
			msec_com_max=win_command-1;
			//
			int viewX=10;int viewSz=500;int small_sz=100;int selRow_wd=50;int button_sz=60;
			radioHead=WS_GROUP;
			win_command=sound_view;
			for(int i=0;i<nr_of_rows;i++){
				win_y+=inter_rows_nuNumaiSpatiul;
				int xPos=viewX;
				hwnd_rows_sel[i]=CreateWindowEx(0,"BUTTON","",WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|radioHead,xPos,win_y,selRow_wd,20,hwndMain,(HMENU)win_command,0,0);win_command++;
				radioHead=0;
				//
				xPos+=selRow_wd+10;
				hwnd_rows_data[i]=CreateWindowEx(0,"EDIT",0,
					WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL
					,xPos,win_y,viewSz,40,hwndMain,(HMENU)win_command,0,0);win_command++;
				//
				xPos+=viewSz+viewX;
				hwnd_rows_sz[i]=CreateWindowEx(0,"EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,xPos,win_y,small_sz,20,hwndMain,(HMENU)win_command,0,0);win_command++;
				//
				xPos+=small_sz+viewX;
				CreateWindowEx(0,"BUTTON","Add File",WS_CHILD|WS_VISIBLE,xPos,win_y,button_sz,20,hwndMain,(HMENU)sound_fromFile_cursor,0,0);sound_fromFile_cursor++;
				//
				xPos+=button_sz+viewX;
				CreateWindowEx(0,"BUTTON","Reset",WS_CHILD|WS_VISIBLE,xPos,win_y,button_sz,20,hwndMain,(HMENU)sound_reset_cursor,0,0);sound_reset_cursor++;
			}
			sound_fromFile_max=sound_fromFile_cursor-1;
			sound_reset_max=sound_reset_cursor-1;
			SendMessage(hwnd_rows_sel[0],BM_SETCHECK,BST_CHECKED,0);
			//all size
			win_y+=inter_rows_nuNumaiSpatiul;
			hwnd_allSz=CreateWindowEx(0,"EDIT","0",WS_CHILD|WS_VISIBLE|WS_BORDER,10,win_y,60,20,hwndMain,(HMENU)win_command,0,0);win_command++;
			//
			win_y+=inter_rows_nuNumaiSpatiul;
			int xPos=10;int save_wd=100;
			CreateWindowEx(0,"BUTTON","Save File",WS_CHILD|WS_VISIBLE,xPos,win_y,save_wd,20,hwndMain,(HMENU)save_file,0,0);
			xPos+=save_wd+10;
			CreateWindowEx(0,"BUTTON","Export WAV",WS_CHILD|WS_VISIBLE,xPos,win_y,save_wd,20,hwndMain,(HMENU)export_wav,0,0);
			break;
	}
	case WM_COMMAND:{
			int wPara=LOWORD(wParam);
			if((key_com<=wPara)&&(wPara<=key_com_max)){
				int rowNr=0;
				HWND texthwnd;
				for(;rowNr<nr_of_rows;rowNr++){
					if(SendMessage(hwnd_rows_sel[rowNr],BM_GETCHECK,0,0)==BST_CHECKED){texthwnd=hwnd_rows_data[rowNr];break;}
				}
				HWND radioHwndMsec;
				for(int i=0;i<nr_of_secRadio;i++){
					if(SendMessage(hwnd_msec[i],BM_GETCHECK,0,0)==BST_CHECKED){radioHwndMsec=hwnd_msec[i];break;}
				}
				#define msecStr_sz 100
				char msecStr[msecStr_sz];
				SendMessage(radioHwndMsec,WM_GETTEXT,msecStr_sz,(LPARAM)msecStr);
				//
				char soundAdd[100];sprintf(soundAdd,"%u,%s;",wPara-key_com,msecStr);
				SendMessage(texthwnd,EM_REPLACESEL,FALSE,(LPARAM)soundAdd);
				//
				add_time(hwnd_rows_sz[rowNr],msecStr);
				add_time(hwnd_allSz,msecStr);
				break;
			}
			if((sound_fromFile<=wPara)&&(wPara<=sound_fromFile_max)){
				int rowNr=wPara-sound_fromFile;
				char pathname[MAX_PATH];pathname[0]=0;
				OPENFILENAME ofn;
				ZeroMemory(&ofn,sizeof(OPENFILENAME));
				ofn.lStructSize=0x58;
				ofn.lpstrFilter="All Files\0*.*\0\0";
				ofn.lpstrFile=pathname;
				ofn.nMaxFile=MAX_PATH;
				ofn.Flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
				if(GetOpenFileName(&ofn)==TRUE){
					//remove current time from all size
					allSize_time_subtract(hwnd_rows_sz[rowNr]);
					//
					FILE* f=fopen(pathname,"rb");
					fseek(f,0,SEEK_END);
					int sz=ftell(f);fseek(f,0,SEEK_SET);
					fread(text_data,1,sz,f);
					text_data[sz]=0;
					SendMessage(hwnd_rows_data[rowNr],WM_SETTEXT,strlen(text_data),(LPARAM)text_data);
					char* cursor=text_data;
					int harvested_size=0;
					while(cursor[0]!=0){
						while(cursor[0]!=',')cursor++;
						cursor++;
						char* pointer=cursor;
						while(cursor[0]!=';')cursor++;
						cursor[0]=0;cursor++;
						harvested_size+=atoi(pointer);
					}
					char toStr[100];sprintf(toStr,"%u",harvested_size);
					SendMessage(hwnd_rows_sz[rowNr],WM_SETTEXT,strlen(toStr),LPARAM(toStr));
					fclose(f);
					//add time to all size
					allSize_time_add(hwnd_rows_sz[rowNr]);
				}
				break;
			}
			if((sound_reset<=wPara)&&(wPara<=sound_reset_max)){
				int rowNr=wPara-sound_reset;
				SendMessage(hwnd_rows_data[rowNr],WM_SETTEXT,0,(LPARAM)"");
				allSize_time_subtract(hwnd_rows_sz[rowNr]);
				SendMessage(hwnd_rows_sz[rowNr],WM_SETTEXT,1,(LPARAM)"0");
				break;
			}
			switch(wPara)
			{
				case save_file:
					saveName(save_to_file);
					break;
				case export_wav:
					saveName(export_to_wav);
					break;
			}
			break;
		}
	case WM_DESTROY:
            PostQuitMessage(0);
            break;
	default:
		    return DefWindowProc(hwndMain, uMsg, wParam, lParam);
    }
    return 0;
}
#define msecStr_sz 100
void add_time(HWND hw,char* str){
	char msecStr[msecStr_sz];
	SendMessage(hw,WM_GETTEXT,msecStr_sz,(LPARAM)msecStr);
	int current_sz=atoi(msecStr);
	int add_sz=atoi(str);
	sprintf(msecStr,"%u",current_sz+add_sz);
	SendMessage(hw,WM_SETTEXT,0,(LPARAM)msecStr);
}
void allSize_time_add(HWND hw){
	char msecStr[msecStr_sz];
	SendMessage(hw,WM_GETTEXT,msecStr_sz,(LPARAM)msecStr);
	add_time(hwnd_allSz,msecStr);
}
void allSize_time_subtract(HWND hw){
	char msecStr[msecStr_sz];msecStr[0]='-';
#define ofset_pozitiv 1
	SendMessage(hw,WM_GETTEXT,msecStr_sz-ofset_pozitiv,LPARAM(msecStr+ofset_pozitiv));
	add_time(hwnd_allSz,msecStr);
}
void saveName(void (*pt2Func)(char*)){
	char pathname[MAX_PATH];pathname[0]=0;
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize=0x58;
	ofn.lpstrFilter="All Files\0*.*\0\0";
	ofn.lpstrFile=pathname;
	ofn.nMaxFile=MAX_PATH;
	ofn.Flags=OFN_OVERWRITEPROMPT;
	//OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST
	if(GetSaveFileName(&ofn)==TRUE){
		pt2Func(pathname);
	}
}
void export_to_wav(char* pathname){
		FILE* f=fopen(pathname,"wb");
			WAVEFORMATEX wfmt;
			wfmt.wFormatTag=WAVE_FORMAT_PCM;
			wfmt.nChannels=channels;
			wfmt.nSamplesPerSec=sample_rate;
			wfmt.wBitsPerSample=bit_rate;
			wfmt.nBlockAlign=(wfmt.nChannels*wfmt.wBitsPerSample)/8;
			wfmt.nAvgBytesPerSec=wfmt.nSamplesPerSec*wfmt.nBlockAlign;
			wfmt.cbSize=0;
		char* outData;int dataSz=0;
		outData=(char*)malloc(1000*1000);
		char* pointer;char* cursor;
		for(int i=0;i<nr_of_rows;i++){
			SendMessage(hwnd_rows_data[i],WM_GETTEXT,text_data_sz,(LPARAM)text_data);
			pointer=text_data;
			while(pointer[0]!=0){
				cursor=pointer;
				while(cursor[0]!=',')cursor++;
				cursor[0]=0;cursor++;
				int keyNr=atoi(pointer);
				pointer=cursor;
				while(cursor[0]!=';')cursor++;
				cursor[0]=0;cursor++;
				int msecNr=atoi(pointer);
				pointer=cursor;
				//
				int time_to_bytes=wfmt.nAvgBytesPerSec*msecNr/1000;
				memcpy(outData+dataSz,key_data[keyNr],time_to_bytes);
				dataSz+=time_to_bytes;
			}
		}
		//
		int all_size=4+4+sizeof(WAVEFORMATEX)+4+4+dataSz;
		fwrite("RIFF",1,4,f);fwrite(&all_size,4,1,f);
		fwrite("WAVE",1,4,f);
		int wv_size=sizeof(WAVEFORMATEX);
		fwrite("fmt ",1,4,f);fwrite(&wv_size,4,1,f);
		fwrite(&wfmt,sizeof(WAVEFORMATEX),1,f);
		fwrite("data",1,4,f);fwrite(&dataSz,4,1,f);
		fwrite(outData,1,dataSz,f);
			fclose(f);
}
void save_to_file(char* pathname){
	FILE* f=fopen(pathname,"wb");
	int rowNr=0;
	HWND texthwnd;
	for(;rowNr<nr_of_rows;rowNr++){
		if(SendMessage(hwnd_rows_sel[rowNr],BM_GETCHECK,0,0)==BST_CHECKED){texthwnd=hwnd_rows_data[rowNr];break;}
	}
	int nr_of_chars=SendMessage(texthwnd,WM_GETTEXT,text_data_sz,(LPARAM)text_data);
	fwrite(text_data,1,nr_of_chars,f);
	fclose(f);
}





/*LRESULT CALLBACK text_WndProc(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg)
    {
		case WM_HSCROLL:
		{
			int currentPos=GetScrollPos(hwndMain,SB_HORZ);
			switch (LOWORD(wParam))
			{
				case SB_TOP:
					currentPos= 0;
					break;
				case SB_LINEUP:
					if (currentPos> 0)currentPos--;
					break;
				case SB_THUMBPOSITION:
					currentPos= HIWORD(wParam);
					break;
				case SB_THUMBTRACK:
					currentPos= HIWORD(wParam);
					break;
				case SB_LINEDOWN:
					if (currentPos<si_nMax)currentPos++;
					break;
				case SB_BOTTOM:
					currentPos=si_nMax;
					break;
				//case SB_ENDSCROLL:
				//	break;
			}
			SetScrollPos(hwndMain, SB_HORZ, currentPos, TRUE);
			break;
		}
	default:
		return CallWindowProc(OldWndProc,hwndMain,uMsg,wParam,lParam);
		    //return DefWindowProc(hwndMain, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK text_WndProc(HWND,UINT,WPARAM,LPARAM);WNDPROC OldWndProc;
#define si_nMax 240
#define si_nPage 10
			SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.fMask  = SIF_RANGE | SIF_PAGE|SIF_POS;
			si.nMin   = 0;
			si.nMax   = si_nMax;
			si.nPage  = si_nPage;
			si.nPos   = 0;
			//si.nTrackPos
			|WS_HSCROLL
				SetScrollInfo(hwnd_rows_data[i], SB_HORZ, &si, TRUE);
				OldWndProc = (WNDPROC)SetWindowLongPtr(hwnd_rows_data[i],GWLP_WNDPROC,(LONG_PTR)text_WndProc);

*/


///////////////////////

//fwrite("/",1,1,f);
	//nr_of_chars=SendMessage(hwnd_rows_sz[rowNr],WM_GETTEXT,text_data_sz,(LPARAM)text_data);
	//fwrite(text_data,1,nr_of_chars,f);

//while(text_data[sz]!='/')sz--;
					//text_data[sz]=0;int timeSz=sz+1;
					//SendMessage(hwnd_rows_sz[rowNr],WM_SETTEXT,strlen(text_data+timeSz),LPARAM(text_data+timeSz));