#ifndef PROCESSDISCRIMINATOR_HPP
#define PROCESSDISCRIMINATOR_HPP

#include "ModuleWalker.hpp"
#include "ProcessWalker.hpp"

/*
	The ProcessDiscriminator checks to see if the target process has our DLL loaded
 */
class ProcessDiscriminator : ModuleWalker,ProcessWalker{
public:
	ProcessDiscriminator(TCHAR * targetProcess,TCHAR * dllName);
	~ProcessDiscriminator();

	DWORD	getPid(){return targetPid;};
	bool	getContainsDll(){ return containsDll;};
protected:
	bool walkModule(TCHAR* moduleName);
	bool walkProcess(DWORD pid,TCHAR * processName);
private:
	TCHAR * dllName;
	TCHAR * targetProcess;

	DWORD targetPid;
	bool  containsDll;
};

ProcessDiscriminator::ProcessDiscriminator(TCHAR * targetProcess,TCHAR * dllName){
	this->targetProcess = targetProcess;
	this->dllName = dllName;
	targetPid = 0;
	containsDll= false;
	this->ListProcesses();
}

ProcessDiscriminator::~ProcessDiscriminator(){

}

bool ProcessDiscriminator::walkModule(TCHAR *moduleName){
	//TODO: decloak dllName
	int chk = _wcsicmp(moduleName,dllName);
	//TODO: recloak dllName
	if(chk == 0){
		containsDll = true;
	}
	return !(chk == 0);
}

bool ProcessDiscriminator::walkProcess(DWORD pid,TCHAR * processName){
	//TODO: decloak targetProcess
	int chk = _wcsicmp(targetProcess,processName);
	//TODO: recloak targetProcess

	//walk the process if it's the one we're looking for
	if(chk == 0){
		targetPid = pid;
		this->ListModules(pid);
	}

	return !(chk == 0);	
}

#endif