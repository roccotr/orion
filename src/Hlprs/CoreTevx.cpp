#include "CoreTevx.hpp"
#include "CoreKutl.hpp"

using namespace orion;


  const int CoreTevn::iCwrsignaled = 1;
  const int CoreTevn::iCwrnotsignaled = 0;

  CoreTevn::CoreTevn (void*, bool, bool, char*) {
		
		bVsignaled = false;
		pthread_mutex_init (&cVhandle, NULL);
		pthread_cond_init (&cVcondition, NULL);		
	}

  CoreTevn::~CoreTevn () {
		
		pthread_mutex_destroy (&cVhandle);
		pthread_cond_destroy (&cVcondition);
  }
	
	/*RESET EVENT*/
	void CoreTevn::ResetEvent () {
	
		pthread_mutex_lock (&cVhandle);
		bVsignaled = false;
		pthread_mutex_unlock (&cVhandle);
	}
	
	/*WAIT FOR*/
	int CoreTevn::WaitFor (int iMsec) {
		int iVreturn;
		struct timespec cVtimeforcondition;
		 
		 pthread_mutex_lock (&cVhandle);	
		 if (!bVsignaled) {
			 CoreKutl::cnmpd (iMsec, &cVtimeforcondition);
			 pthread_cond_timedwait (&cVcondition, &cVhandle, &cVtimeforcondition);
		 }	 
		if (bVsignaled) {
			iVreturn = iCwrsignaled;
			bVsignaled = false;
		} else iVreturn = iCwrnotsignaled;
		pthread_mutex_unlock (&cVhandle);			 
		return iVreturn; 
	}
	
	/*SET EVENT*/
	void CoreTevn::SetEvent () {
	
		pthread_mutex_lock (&cVhandle);
		bVsignaled = true;
		pthread_cond_signal(&cVcondition);
		pthread_mutex_unlock (&cVhandle);
	}
	


