#include <windows.h>

#pragma data_seg ( "SHAREDDATA" )
	HANDLE hMutex;
#pragma data_seg ()
#pragma comment(linker, "/section:SHAREDDATA,rws")

int WINAPI DllMain(HINSTANCE hInstance,DWORD reason,LPVOID reserved){

	switch(reason){
		case DLL_PROCESS_ATTACH:
			//TODO: process attach
			MessageBox(0,L"DONG",L"DONG",MB_OK);
			hMutex = CreateMutex(NULL,TRUE,"zomgmtx");

			if(WaitForSingleObject(hMutex,1)){

			}
		break;

		case DLL_PROCESS_DETACH:

		break;
	}

	return 0;
}