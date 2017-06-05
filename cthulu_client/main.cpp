#include "ProcessDiscriminator.hpp"
#include "ProcessInjector.hpp"

int WINAPI WinMain(HINSTANCE hProcess,HINSTANCE hPrev,LPSTR CmdLine,int ShowFlags){
	TCHAR * targetProcess = 	TEXT("notepad.exe");
	TCHAR * targetDll = TEXT("payload_dll.dll");

	//Find target process and check it if contains our dll
	ProcessDiscriminator pd(targetProcess,targetDll);

	if( pd.getPid() && !pd.getContainsDll()){
		//inject our DLL through
		ProcessInjector pi(pd.getPid());
		pi.injectRemoteThread(targetDll);
	}

	return 0;
}