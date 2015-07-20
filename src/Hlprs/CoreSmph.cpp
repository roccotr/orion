#include "CoreSmph.hpp"

using namespace orion;

  const long int CoreSmph::iCsphinit = 1;
  const long int CoreSmph::iCsmphprecision = 10;
	const int CoreSmph::iCspincount = 1000;

  __fastcall CoreSmph::CoreSmph () {

    iVinterlock= iCsphinit;
		iVptrinterlock = &iVinterlock;
		cVptrevent = &cVevent;
    iVerrors = 0;
		iVspincount = 0;
		bVcreatedevent = false;

#ifdef iCdebugsemaphore
		iVdebugsemaphore = 0;
#endif

  }

  __fastcall CoreSmph::~CoreSmph () {

		if (bVcreatedevent) delete cVptrevent;
  }

	__fastcall CoreSmph::CoreSmph (CoreSmph* cSemaphore, unsigned int iProcessid) {

		iVptrinterlock = &cSemaphore->iVinterlock;
		cVptrevent = new TEvent (&cSemaphore->cVevent, iProcessid);
		iVerrors = 0;
		bVcreatedevent = true;
	}

	/*SLeeP EVent*/
	void CoreSmph::slpev (int iMs) {

#ifdef iCdebugsemaphore
		if (iVspincount > 0) {
			iVdebugsemaphore++;
			iVdebugsemaphore %= 10000;
			if (iVdebugsemaphore == 9999) printf ("#");
		}
#endif

		if (cVptrevent->WaitFor (iMs) == wrSignaled) cVptrevent->ResetEvent ();
	}

  /*LOCK Aquire*/
  bool __fastcall CoreSmph::locka () {
    l64 iVresult;
    bool bVreturn;

    bVreturn = false;
		if (*iVptrinterlock <= 0) return false;
    iVresult = __InterlockedDecrement (iVptrinterlock);
    if (iVresult >= 0) bVreturn = true;
    else __InterlockedIncrement (iVptrinterlock);
    return bVreturn;
  }


  /*LOCK Sleep*/
  void __fastcall CoreSmph::locks () {
		
		if (iVspincount == 0) while (!locka ()) slpev (iCsmphprecision);
		else {
			int iVcounter = iVspincount;

			while (!locka ()) {
				if (iVcounter == 0) slpev (iCsmphprecision);
				else iVcounter--;
			}
		}
  }

  /*LOCK Sleep*/
  void __fastcall CoreSmph::locks (int iMswatchdog) {
    double dVlasttickcount;
    double dVmswatchdog;
    double dVmstimelag;

    dVmswatchdog = iMswatchdog;
    dVlasttickcount = timeGetTime ();
    while (!locka ()) {
			slpev (iCsmphprecision);
      dVmstimelag = timeGetTime () - dVlasttickcount;
      if (dVmstimelag > dVmswatchdog) {
        *iVptrinterlock = iCsphinit -1;
        iVerrors++;
        break;
      }
    }
  }

  /*RELeaSE*/
  void __fastcall CoreSmph::relse () {
    __InterlockedIncrement (iVptrinterlock);
		cVptrevent->SetEvent ();
  }

	/*SET Spin Count*/
	void __fastcall CoreSmph::setsc (int iSpincount) {

		iVspincount = iSpincount;
	}
	
  /*DEBuG Lock*/
  int __fastcall CoreSmph::debgl () {
    int iVreturn;

		iVreturn = 0;
    while (!locka ()) {
      iVreturn++;
      slpev (iCsmphprecision);
    }
    return iVreturn;
  }

  /*GET ERrors*/
  int __fastcall CoreSmph::geter () {
    return iVerrors;
  }

	/*GET EVent*/
	TEvent* __fastcall CoreSmph::getev () {
		return cVptrevent;
	}

