#ifndef CORESMPW
#define CORESMPW
#include "CoreVcl_.hpp"

namespace orion {

//#define iCdebugsemaphore

class CoreSmph {
private:
  const static long int iCsphinit;
  const static long int iCsmphprecision;


	bool bVcreatedevent;
  l64 iVinterlock;
	l64* iVptrinterlock;
  int iVerrors;
	TEvent cVevent; 
	TEvent* cVptrevent;
	int iVspincount;

	inline void slpev (int iMs);

#ifdef iCdebugsemaphore
	int iVdebugsemaphore;
#endif

public:

	const static int iCspincount;

  __fastcall CoreSmph ();
	__fastcall CoreSmph (CoreSmph* cSemaphore, unsigned int iProcessid = 0);
  __fastcall ~CoreSmph ();
  bool __fastcall locka ();
  void __fastcall locks ();
  void __fastcall locks (int iMswatchdog);
  void __fastcall relse ();
  int __fastcall debgl ();
  int __fastcall geter ();
	void __fastcall setsc (int iSpincount);

	TEvent* __fastcall getev ();
};

};



#endif