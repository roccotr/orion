#include "CoreTevv.hpp"
#include "CoreDbug.hpp"

using namespace orion;

  CoreTevn::CoreTevn (LPSECURITY_ATTRIBUTES lpEventAttributes, 
											bool bManualReset, bool bInitialState, LPCTSTR lpName) {
		
		cVevent = CreateEvent (lpEventAttributes, bManualReset, bInitialState, NULL);
		bVcreated = true;
	}

  CoreTevn::~CoreTevn () {
		if (bVcreated) CloseHandle(cVevent);
  }

  CoreTevn::CoreTevn (CoreTevn* cEvent, unsigned int iProcessid) {
		if (iProcessid == 0) {
			cVevent = cEvent->cVevent;
			bVcreated = false;
		} else {
			HANDLE hVprocess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, iProcessid);
			DuplicateHandle(hVprocess, cEvent->cVevent, GetCurrentProcess(), &cVevent, 0, FALSE, DUPLICATE_SAME_ACCESS);
			CloseHandle (hVprocess);
			bVcreated = true;

		}
	}
	
	/*RESET EVENT*/
	void CoreTevn::ResetEvent () {
		::ResetEvent (cVevent);
	}
	
	/*WAIT FOR*/
	int CoreTevn::WaitFor (int iMsec) {
		return WaitForSingleObject (cVevent, iMsec);
	}
	
	/*SET EVENT*/
	void CoreTevn::SetEvent () {
		::SetEvent(cVevent);
	}
	


