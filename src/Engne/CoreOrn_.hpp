#ifndef COREORN_
#define COREORN_
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"
#include "CorePlan.hpp"
#include "CoreSckt.hpp"
#include "CorePeer.hpp" 

#define iCversion "1.1.04 (2015)"  

namespace orion {

class CoreOrn_ {

private:
	PTR_POOL cVpool;
	PTR_PLANNER cVplanner;	
	CoreEnpo::NODEDATA cVnodedata;	

public:

	const static AnsiString sCnodeid;
	const static AnsiString sClocaladdress;
	const static AnsiString sCdefault;
	const static int iCthriftport;
	const static int iCredologdim;
	const static short int iCthriftlisteners;
	const static short int iCreplicationfactor;
	const static short int iCmaxcompactionlevel;

  __fastcall CoreOrn_(	PTR_POOL cPool, AnsiString sPath, AnsiString sFilename, 
												CoreEnpo::iCstategossipnode iState, bool bResetlocal = false);
  __fastcall ~CoreOrn_ ();

	PTR_PLANNER __fastcall getpl ();
	void __fastcall getnd (	CoreEnpo::NODEDATA& cNodedata);
};

typedef boost::shared_ptr<CoreOrn_> PTR_ORION;

}

#endif