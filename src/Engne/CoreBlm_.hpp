#ifndef COREBLM_
#define COREBLM_
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"

#define iCbloomdefaultk 67

using namespace orion;

class CoreBlm_
{
private:
	CorePool* cVpool;
	int iVdim;
	int iVk;
	int iVm;
	char* bVbuffer;

	__fastcall void write (unsigned int iHash);
	__fastcall bool read_ (unsigned int iHash);

public:
  __fastcall CoreBlm_ (CorePool* cPool, int iM, int iK = iCbloomdefaultk);
  __fastcall ~CoreBlm_ ();

	__fastcall void add__ (AnsiString sKey);
	__fastcall bool exst_ (AnsiString sKey);
	__fastcall void clear ();

	static __fastcall int getdm (int iMaxindex);

};

#endif