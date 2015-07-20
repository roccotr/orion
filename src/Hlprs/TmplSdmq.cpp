#include "CoreSdmq.hpp"

using namespace orion;

	template<typename T>
  __fastcall CoreSdmq<T>::CoreSdmq() {
		
		cVevent = NULL;
	}

	template<typename T>
  __fastcall CoreSdmq<T>::~CoreSdmq () {

		delete cVevent;
	}

	/*INIT*/
	template<typename T>
	void __fastcall CoreSdmq<T>::init (AnsiString sName) {

		sVname = sName;
		cVevent = new TEvent (NULL, true, false, sName.c_str ()); 
		cVevent->ResetEvent ();
	}

  /*PUSH*/
	template<typename T>
	bool __fastcall CoreSdmq<T>::push_ (const T& cMessage) {

		cVmutex.locks ();
		*cVqueue.set__ () = cMessage;		
		cVevent->SetEvent ();
		cVmutex.relse ();
		return true;
	}

  /*POP*/
	template<typename T>
	bool __fastcall CoreSdmq<T>::pop__ (T& cMessage) {
		bool bVreturn;

		cVmutex.locks ();
		if (cVqueue.count () > 0) {
			cMessage = *cVqueue.get__ (0);
			cVqueue.unset (0);
			bVreturn = true;
		} else bVreturn = false;
		cVmutex.relse ();
		return bVreturn;
	}

	/*COUNT*/
	template<typename T>
	int __fastcall CoreSdmq<T>::count () {

		return cVqueue.count ();
	}

	/*WAIT*/
	template<typename T>
	bool __fastcall CoreSdmq<T>::wait_ (int iWatchdogms) {
		bool bVreturn;

		bVreturn = cVevent->WaitFor (iWatchdogms) == wrSignaled;
		cVevent->ResetEvent ();
		return bVreturn;
	}

	/*SIGNaL*/
	template<typename T>
	void __fastcall CoreSdmq<T>::signl () {

		cVmutex.locks ();
		cVevent->SetEvent ();
		cVmutex.relse ();
	}

