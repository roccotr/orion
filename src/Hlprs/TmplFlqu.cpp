#include "CoreFlqu.hpp"

	template<typename T, int iVdim>
  const long int CoreFlqu<T, iVdim>::iCwaitprecision = 10;

	template<typename T, int iVdim>
  __fastcall CoreFlqu<T, iVdim>::CoreFlqu () {

		lfds611_queue_new (&cVqs, iVdim);
		cVeventfull.ResetEvent ();
		cVeventempty.ResetEvent ();
		bVterminated = false;
	}

	template<typename T, int iVdim>
  __fastcall CoreFlqu<T, iVdim>::~CoreFlqu () {

		lfds611_queue_delete (cVqs, delte, NULL);
	}

  /*PUSH*/
	template<typename T, int iVdim>
	bool __fastcall CoreFlqu<T, iVdim>::push_ (T* cElement) {

		while (lfds611_queue_enqueue (cVqs, cElement) == 0 || bVterminated)
			if (cVeventfull.WaitFor (iCwaitprecision) == wrSignaled) cVeventfull.ResetEvent ();
		cVeventempty.SetEvent ();
		return !bVterminated;
	}

	/*POP*/
	template<typename T, int iVdim>
	bool __fastcall CoreFlqu<T, iVdim>::pop__ (T** cElement) {

		if (lfds611_queue_dequeue (cVqs, (void**) cElement) == 0) return false;
		else {
			cVeventfull.SetEvent ();
			return true;
		}
	}

	/*ABORT*/
	template<typename T, int iVdim>
	void __fastcall CoreFlqu<T, iVdim>::abort () {
		bVterminated = true;
	}

	/*DELeTE*/
	template<typename T, int iVdim>
	void	CoreFlqu<T, iVdim>::delte (void* cUserdata, void* cDummy) {

		delete (T*) cUserdata;
	}

	/*WAIT*/
	template<typename T, int iVdim>
	bool __fastcall CoreFlqu<T, iVdim>::wait_ (int iWatchdogms) {
		bool bVreturn;

		bVreturn = cVeventempty.WaitFor (iWatchdogms) == wrSignaled;
		cVeventempty.ResetEvent ();
		return bVreturn;
	}

	/*SIGNaL*/
	template<typename T, int iVdim>
	void __fastcall CoreFlqu<T, iVdim>::signl () {

		cVeventempty.SetEvent ();
	}
