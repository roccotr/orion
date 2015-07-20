#ifndef CORETTHR
#define CORETTHR
#include <glog/logging.h>
#include <windows.h>


class CoreTthr {
public:
	typedef struct {
		CoreTthr*	cVinstance;
		HANDLE cVpthread;
		DWORD cVpthreadkey;
	} TTHREADLOC;
		
	
	bool Terminated;
	int Priority;	

private:
	
	bool bVsuspend;
	bool bVcansetpriority;
	bool bVdeadmanwalking;
	TTHREADLOC cVthreaddata;

	static DWORD __stdcall ptsta (void* cHandle);	
public:

  CoreTthr (bool bSuspend);
  ~CoreTthr ();
  virtual void Terminate ();
  virtual void Execute ();
	void Resume ();
	void WaitFor ();

	void exepr ();
};

#endif