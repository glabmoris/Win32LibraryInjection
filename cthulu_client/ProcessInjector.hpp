#ifndef PROCESSINJECTOR_HPP
#define PROCESSINJECTOR_HPP

class ProcessInjector{
public:
	ProcessInjector(DWORD pid);
	~ProcessInjector();

	bool injectRemoteThread(TCHAR* dllName);
	bool injectHook(TCHAR* dllName);
private:
	DWORD pid;
};

ProcessInjector::ProcessInjector(DWORD pid){
	this->pid=pid;
}

ProcessInjector::~ProcessInjector(){

}

//Standard DLL injection
bool ProcessInjector::injectRemoteThread(TCHAR * dllName){
	HANDLE Proc;
	//char buf[50]={0};
	LPVOID RemoteString, LoadLibAddy;

	if (pid != NULL){
		//TODO: powerup with AdjustTokenPrivileges
		if( Proc = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE, pid) ){
			LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");

			RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, wcslen(dllName)*2, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
			WriteProcessMemory(Proc, (LPVOID)RemoteString, dllName,wcslen(dllName)*2, NULL);
			CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);   
		   
			CloseHandle(Proc);

			return true;
		}
	}

	return false;
}

// This baby will need a function named CBTProc in our dll
// and will inject it in every process
bool ProcessInjector::injectHook(TCHAR * dllName){

    HMODULE hDll;
    HOOKPROC cbtProcAddr;

    hDll        = LoadLibrary(dllName);
    cbtProcAddr = (HOOKPROC) GetProcAddress(hDll, "CBTProc");

    SetWindowsHookEx(WH_CBT, cbtProcAddr, hDll, 0);
   
    return true;
}

#endif