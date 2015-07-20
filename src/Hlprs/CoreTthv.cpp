#include "CoreTthv.hpp"
#include "CoreDbug.hpp"
#include <stdio.h>

using namespace google;

  CoreTthr::CoreTthr ( bool bSuspend)  {
 		bVsuspend = true;
		cVthreaddata.cVinstance = this;   
    cVthreaddata.cVpthread = CreateThread (
            0, 0, ptsta, &cVthreaddata, 
						CREATE_SUSPENDED, &cVthreaddata.cVpthreadkey);

		Terminated = false;
		bVcansetpriority = false;
		bVdeadmanwalking = false;
		Priority = 0;
		if (!bSuspend) Resume ();
  }

  CoreTthr::~CoreTthr () {

		Terminate ();
		Resume ();
		WaitFor ();
		CloseHandle (cVthreaddata.cVpthread);
		FlushLogFiles(GLOG_INFO);
  }

	/*PThreads STArt routine*/
	DWORD __stdcall  CoreTthr::ptsta (void* cHandle) {
		TTHREADLOC* cVthreaddata;

		cVthreaddata = (TTHREADLOC*) cHandle;
		cVthreaddata->cVinstance->Execute ();
		return 0;
	}

  /*EXECUTE*/
  void CoreTthr::Execute () {}

  /*TERMINATE*/
  void CoreTthr::Terminate () {
		Terminated = true;
  }
	
  /*RESUME*/
  void CoreTthr::Resume () {
		
		if (bVsuspend && !bVdeadmanwalking) {
			ResumeThread (cVthreaddata.cVpthread); 
			bVsuspend = false;
		}
  }	
	
	/*WAITFOR*/
	void CoreTthr::WaitFor () {
		if (!bVdeadmanwalking) {
			bVdeadmanwalking = true;
			WaitForSingleObject (cVthreaddata.cVpthread, INFINITE);
		}
	}
	
	/*EXEcute PRiority*/
	void CoreTthr::exepr () {
		if (!bVdeadmanwalking)
			SetThreadPriority(cVthreaddata.cVpthread, Priority);
	}


