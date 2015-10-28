#ifndef CORETHTH
#define CORETHTH
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"
#include "CoreSckt.hpp"
#include "CoreShst.hpp"
#include "CoreTsrl.hpp"
#include "ThrfOrn_.h"

namespace orion {

class CoreThth : TThread {

	bool bValive;
	bool bVsharedmemory;
	CoreSckt* cVsocket;
	void* cVthriftserver;
	PTR_POOL cVpool;
	AnsiString sVsharedmemoryname;
	boost::shared_ptr<CoreShst<ThrfBsrr> > cVsharedsend;
	boost::shared_ptr<CoreShst<ThrfBsrc> > cVsharedreceive;

public:

  __fastcall CoreThth (	PTR_POOL cPool, CoreSckt* cSocket, void* cThriftserver,
												bool bSharedmemory, AnsiString sSharedmemoryprefix, int iSharedmemorysuffix);
  __fastcall ~CoreThth ();

	void __fastcall Execute ();
	void __fastcall loop_ ();
	void __fastcall loopp (); 
	bool __fastcall isalv ();

};

typedef boost::shared_ptr<CoreThth> PTR_SERVERWORKER;

};

#endif