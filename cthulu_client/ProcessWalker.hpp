#ifndef PROCESSWALKER_HPP
#define PROCESSWALKER_HPP

class ProcessWalker{
public:
	ProcessWalker();
	~ProcessWalker();
	BOOL ListProcesses();
protected:
	//Continues the walk while this returns true
	virtual bool walkProcess(DWORD pid,TCHAR * processName){
		return true;
	}
};

ProcessWalker::ProcessWalker(){

}

ProcessWalker::~ProcessWalker(){

}

BOOL ProcessWalker::ListProcesses(){
	//TODO: create snapshot
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE; 
	PROCESSENTRY32 pe32; 

	// Take a snapshot of the system
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPALL, 0 ); 
	if( hProcessSnap == INVALID_HANDLE_VALUE ) { 
//		printError(TEXT( "CreateToolhelp32Snapshot (of process)") ); 
		return( FALSE ); 
	} 

	pe32.dwSize = sizeof( PROCESSENTRY32 ); 

	//prime the walk
	if( !Process32First( hProcessSnap, &pe32 ) ){ 
//		printError( TEXT("Module32First") );  // Show cause of failure 
		CloseHandle( hProcessSnap );     // Must clean up the snapshot object! 
		return( FALSE ); 
	} 

	//walk the list
	do 
	{ 
		if(!walkProcess( pe32.th32ProcessID,pe32.szExeFile ) ){
			break;
		}
		//printf( "\n\n     MODULE NAME:     %s",             me32.szModule ); 
		//printf( "\n     executable     = %s",             me32.szExePath ); 
		//printf( "\n     process ID     = 0x%08X",         me32.th32ProcessID ); 
		//printf( "\n     ref count (g)  =     0x%04X",     me32.GlblcntUsage ); 
		//printf( "\n     ref count (p)  =     0x%04X",     me32.ProccntUsage ); 
		//printf( "\n     base address   = 0x%08X", (DWORD) me32.modBaseAddr ); 
		//printf( "\n     base size      = %d",             me32.modBaseSize ); 
	} while( Process32Next( hProcessSnap, &pe32 ) ); 
	
	CloseHandle( hProcessSnap ); 
	return TRUE ; 
}

#endif