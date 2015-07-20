#ifndef CORETEVN
#define CORETEVN
#include <glog/logging.h>
#include <windows.h>

namespace orion {

class CoreTevn {

private:
	HANDLE cVevent;
	bool bVcreated;
	
public:

  CoreTevn (LPSECURITY_ATTRIBUTES lpEventAttributes = NULL, 
						bool bManualReset = true, bool bInitialState = false, LPCTSTR lpName = NULL);
  CoreTevn (CoreTevn* cEvent, unsigned int iProcessid = 0);
  ~CoreTevn ();

	void ResetEvent ();
	int WaitFor (int iMsec);
	void SetEvent ();
};

};

#endif