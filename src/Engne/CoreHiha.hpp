#ifndef COREHIHA
#define COREHIHA
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"
#include "CoreAlog.hpp"
#include "CoreEnpo.hpp"
#include "CoreGoss.hpp"

namespace orion {

class CoreHiha {
	const static AnsiString sChintedhandofftable;
	const static char bCdelimiter;
	const static int iCdefaultprocess;

	PTR_REDOLOG cVredolog;
	PTR_GOSSIPER cVgossiper;
	CoreRvrs cVreverse;
	CoreSmph cVmutex;
	PrtoLmtb cVtemplatemutable;

public:
  __fastcall CoreHiha(PTR_REDOLOG cRedolog, PTR_GOSSIPER cGossiper);
  __fastcall ~CoreHiha ();

	bool __fastcall put__ (AnsiString sNode, PrtoLsts* cStatements, AnsiString sRedotimestamp = NULL);
	bool __fastcall prcss (int iDim = iCdefaultprocess);
};

typedef boost::shared_ptr<CoreHiha> PTR_HINTEDHANDOFF;

};

#endif