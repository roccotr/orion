#ifndef COREPOOL
#define COREPOOL
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreSmph.hpp"

namespace orion {

class CorePool {
private:

  const static int iCattempts;
	

  typedef struct {
    short int iVtype;
    void* vVaddress;
    int iVdim;
		int iVbytes;
    char bVcomment [5];

#ifdef iCdebugpool
		char* bVdescription;
#endif

  } POOLINFO;

	long int iValloclimit;
  long int iVallocatedmemory;
  long int iVmaxallocatedmemory;
  long int iVmaxrequestedmemory;
	bool bValloclimit;
  TList cVauditpool;
	CoreSmph cVmutex;

public:

  enum iCdatatypes
  { iCdouble = 0,
		iCfloat,
    iCint,
    iClongint,
    iCbool,
    iCchar,
    iCshortint,

		iCansistring 
  };

  __fastcall CorePool ();
  __fastcall ~CorePool ();

	static int __fastcall getdm (short int iType);
  void* __fastcall mallc (short int iType, int iDim, const char* bComment, 
													AnsiString sDescription = "");
  void __fastcall free_ (void* vMemory);
  long int __fastcall getma ();
  long int __fastcall getxa ();
  long int __fastcall getxr ();
  void __fastcall logst ();
  void __fastcall logdp ();  
	void __fastcall setal (long int iAlloclimit);
  
};

typedef boost::shared_ptr<CorePool> PTR_POOL;

};

#endif