#ifndef MODULEWALKER_HPP
#define MODULEWALKER_HPP

#include "core.hpp"

class ModuleWalker{
public:
	BOOL ListModules( DWORD dwPID ) ;
	void printError( TCHAR* msg );
protected:
	
	//continues walking while this returns true
	virtual bool walkModule(TCHAR* moduleName){
		return true;
	}
};

BOOL ModuleWalker::ListModules( DWORD dwPID ) 
{ 
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE; 
	MODULEENTRY32 me32; 

	// Take a snapshot of the system
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID ); 
	if( hModuleSnap == INVALID_HANDLE_VALUE ) { 
//		printError(TEXT( "CreateToolhelp32Snapshot (of modules)") ); 
		return( FALSE ); 
	} 

	me32.dwSize = sizeof( MODULEENTRY32 ); 

	//prime the walk
	if( !Module32First( hModuleSnap, &me32 ) ){ 
//		printError( TEXT("Module32First") );  // Show cause of failure 
		CloseHandle( hModuleSnap );     // Must clean up the snapshot object! 
		return( FALSE ); 
	} 

	//walk the list
	do 
	{ 
		if(!walkModule( me32.szModule ) ){
			break;
		}
		//printf( "\n\n     MODULE NAME:     %s",             me32.szModule ); 
		//printf( "\n     executable     = %s",             me32.szExePath ); 
		//printf( "\n     process ID     = 0x%08X",         me32.th32ProcessID ); 
		//printf( "\n     ref count (g)  =     0x%04X",     me32.GlblcntUsage ); 
		//printf( "\n     ref count (p)  =     0x%04X",     me32.ProccntUsage ); 
		//printf( "\n     base address   = 0x%08X", (DWORD) me32.modBaseAddr ); 
		//printf( "\n     base size      = %d",             me32.modBaseSize ); 
	} while( Module32Next( hModuleSnap, &me32 ) ); 
	
	CloseHandle( hModuleSnap ); 
	return TRUE ; 
} 

void ModuleWalker::printError( TCHAR* msg )
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError( );
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		 NULL, eNum,
		 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		 sysMsg, 256, NULL );

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while( ( *p > 31 ) || ( *p == 9 ) )
	++p;
	do { *p-- = 0; } while( ( p >= sysMsg ) &&
						  ( ( *p == '.' ) || ( *p < 33 ) ) );

	// Display the message
	printf( "\n  WARNING: %s failed with error %d (%s)", msg, eNum, sysMsg );
}

#endif