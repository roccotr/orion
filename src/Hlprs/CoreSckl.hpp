#ifndef CORESCKL
#define CORESCKL
#include "CoreScpl.hpp"

namespace orion {

class CoreSckl
{
protected:

	void* cVsocket;
	void* cVwsadata;

	int iVport;
	int iVsendnum;
	int iVrecvnum;
	int iVsendbytes;
	int iVrecvbytes;

	bool bVsocket;
	bool bVwsa;


public:

	const static AnsiString sCmagic;
  #define iCmaxdir 256

  __fastcall CoreSckl ();
  __fastcall CoreSckl (void* cSocket);
	__fastcall ~CoreSckl ();

	virtual bool __fastcall open_ (int iPort) {return false;};
	virtual void __fastcall cls () {};

	virtual bool __fastcall lflsh () {return true;};
	virtual void __fastcall lsetb (int iDim = 1024) {};
	void __fastcall setno ();

	static __fastcall AnsiString gethi (AnsiString sHostname = NULL);
};

};

#endif




