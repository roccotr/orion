#ifndef COREAENP
#define COREAENP
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePlan.hpp" 
 
namespace orion{

class CoreAenp {
	CorePlan* cVplanner;
	PTR_PEER cVpeer;
	PTR_REDOLOG cVredolog;
	PTR_GOSSIPER cVgossiper;

	const static int iCchunkdim;
	const static int iCchunksize;

public:
  __fastcall CoreAenp(CorePlan* cPlan);
  __fastcall ~CoreAenp ();

	bool __fastcall rpair (	AnsiString sNodeid, bool Table, bool bIndex, AnsiString sIndextable = NULL, 
													CoreEnpo::GOSSIPLOC* cGossipelement = NULL);
	bool __fastcall ppmch (PrtoAenp* cAntientrophy);
	bool __fastcall pprep (PrtoAenp* cAntientrophy);

	bool __fastcall purge (PrtoLmtb* cMutable, CorePart::PARTITIONLOC* cPartitionloc);

};

};

#endif