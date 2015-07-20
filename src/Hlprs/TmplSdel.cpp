#include "CoreSdel.hpp"

#include "glog/logging.h"
using namespace google;
using namespace orion;

	template<typename T>
	const int CoreSdel<T>::iCidlems = 10;

	template<typename T>
  __fastcall CoreSdel<T>::CoreSdel(	CoreSdmq<T>* cMessagequeue, AnsiString sName, 
																		int iPollingtimems, PTR_SEDAWORKER cWorker, 
																		bool bForceworker) : TThread (true) {
																				
		cVmessagequeue = cMessagequeue;
		cVworker = cWorker;
		bVforceworker = bForceworker;
		iVpollingtimems = iPollingtimems;
		sVname = sName;
		cVworker->setqq (cVmessagequeue);
		cVworker->setid (sVname);
		bVrunning = false;
	}

	template<typename T>
  __fastcall CoreSdel<T>::~CoreSdel () {

	}

	template<typename T>
  void CoreSdel<T>::Terminate () {
    TThread::Terminate ();
	}

	template<typename T>
  void CoreSdel<T>::Execute () {

		LOG(INFO) << "W starts:  " << sVname.c_str ();
		FlushLogFiles(GLOG_INFO);

		bVrunning = true;
		while (!Terminated && cVworker->alive ()) {
			cVmutex.locks ();
			if (bVforceworker) {
				cVworker->work_ ();
				cVmessagequeue->wait_ (iVpollingtimems);
			} else if (cVmessagequeue->wait_ (iVpollingtimems) && !Terminated) cVworker->work_ ();
			cVmutex.relse ();
		}
		bVrunning = false;

		LOG(INFO) << "W ends:    " << sVname.c_str ();
		FlushLogFiles(GLOG_INFO);
	}

	/*START*/
	template<typename T>
	void CoreSdel<T>::start () {
		
		cVmutex.locks ();
		Resume ();
		cVmutex.relse ();
	}

	/*START*/
	template<typename T>
	void CoreSdel<T>::stop_ () {

		Terminate ();
		cVmessagequeue->signl ();

		LOG(INFO) << "W stopping:" << sVname.c_str ();
		FlushLogFiles(GLOG_INFO);
		while (bVrunning) Sleep (iCidlems);

		LOG(INFO) << "W stopped: " << sVname.c_str ();
		FlushLogFiles(GLOG_INFO);
	}



