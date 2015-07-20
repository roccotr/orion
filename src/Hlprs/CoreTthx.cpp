#include "CoreTthx.hpp"
#include "glog/logging.h"
using namespace google;
using namespace orion;


  CoreTthr::CoreTthr ( bool bSuspend)  {
		bVsuspend = true;
		cVthreaddata.cVinstance = this;
		pthread_mutex_init (&cVthreaddata.cVqueuehandle, NULL);
		pthread_cond_init (&cVthreaddata.cVqueuecondition, NULL);		
		cVthreaddata.bVqueuesignaled = false;
		Terminated = false;
		bVdeadmanwalking = false;
		bVcansetpriority = false;
		Priority = 0;
		if (!bSuspend) Resume ();
  }

  CoreTthr::~CoreTthr () {
                Terminate ();
                Resume ();
                WaitFor ();		
		
		pthread_mutex_destroy (&cVthreaddata.cVqueuehandle);
		pthread_cond_destroy (&cVthreaddata.cVqueuecondition);		
		if (!bVsuspend) {
			pthread_key_delete(cVthreaddata.cVpthreadkey);
			//pthread_cancel (cVthreaddata.cVpthread);
		}
  }

	/*PThreads STArt routine*/
	void* CoreTthr::ptsta (void* cHandle) {
		int iVerror;
		TTHREADLOC* cVthreaddata;
		void* retval;	
		cVthreaddata = (TTHREADLOC*) cHandle;
		iVerror = pthread_key_create (&cVthreaddata->cVpthreadkey, ptend);
		if (!iVerror) {
			FlushLogFiles(GLOG_INFO);
			pthread_setspecific (cVthreaddata->cVpthreadkey, cHandle);
			cVthreaddata->cVinstance->start ();
			cVthreaddata->cVinstance->Execute ();
			ptend (cHandle);
		}
		return NULL;
	}

	/*PThreads END routine*/
	void CoreTthr::ptend (void* cHandle) {
		TTHREADLOC* cVthreaddata;
		cVthreaddata = (TTHREADLOC*) cHandle;
		if (cVthreaddata->bVqueuesignaled) return;
		FlushLogFiles(GLOG_INFO);
		pthread_mutex_lock (&cVthreaddata->cVqueuehandle);
		cVthreaddata->bVqueuesignaled = true;
		pthread_cond_signal (&cVthreaddata->cVqueuecondition);
		pthread_mutex_unlock (&cVthreaddata->cVqueuehandle);
	}

	/*START*/
	void CoreTthr::start () {
		int iVspread;
		
		if (pthread_getschedparam (cVthreaddata.cVpthread,&iVtpolicy, &cVtparameters) == 0) {
			iVpriority [tpLowest] = sched_get_priority_min (iVtpolicy);
			iVpriority [tpHighest] = sched_get_priority_max (iVtpolicy);			
			iVspread = iVpriority [tpHighest] - iVpriority [tpLowest];
			iVpriority [tpNormal] = iVspread * 0.5 + iVpriority [tpLowest];
			iVpriority [tpLower] = iVspread * 0.25 + iVpriority [tpLowest];
			iVpriority [tpHigher] = iVspread * 0.75 + iVpriority [tpLowest];	
			bVcansetpriority = true;		
		}
	}

  /*EXECUTE*/
  void CoreTthr::Execute () {}

  /*TERMINATE*/
  void CoreTthr::Terminate () {
		Terminated = true;
  }
	
  /*RESUME*/
  void CoreTthr::Resume () {
    int iVerror;
	if (bVsuspend && !bVdeadmanwalking) {
		iVerror = pthread_create (&cVthreaddata.cVpthread, NULL, ptsta, &cVthreaddata);
		if (!iVerror) {
			pthread_detach (cVthreaddata.cVpthread);
		}
		bVsuspend = false;
	}
  }	
	
	/*WAITFOR*/
	void CoreTthr::WaitFor () {
		if (!bVdeadmanwalking) { 
			bVdeadmanwalking = true; 
			pthread_mutex_lock (&cVthreaddata.cVqueuehandle);	
			while (!cVthreaddata.bVqueuesignaled){ 
				pthread_cond_wait (&cVthreaddata.cVqueuecondition, &cVthreaddata.cVqueuehandle);
			}
			pthread_mutex_unlock (&cVthreaddata.cVqueuehandle);	
		}  
	}
	
	/*EXEcute PRiority*/
	void CoreTthr::exepr () {
		if (bVcansetpriority) {
			cVtparameters.sched_priority = iVpriority [Priority];
			pthread_setschedparam  (cVthreaddata.cVpthread, iVtpolicy, &cVtparameters);
		}
	}


