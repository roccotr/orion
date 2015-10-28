#ifndef CORECACP
#define CORECACP
#include "CoreCaco.hpp"
#include "CoreLstc.hpp"

class CoreCacp {
	PTR_ARIESCONNECTION cVariesconnection;
	CoreLstc<CoreCaco::BROKERLOC, 8> cVprivatebrokerpool;
	CoreSmph cVmutex;

public:
  __fastcall CoreCacp	(PTR_ARIESCONNECTION cAriesconnection);
  __fastcall ~CoreCacp ();

	bool __fastcall run__ (PrtoSrvr& cReturn, PrtoSrvc& cService, bool bOptimistic);
};

typedef boost::shared_ptr<CoreCacp> PTR_ARIESPIPELINE;

#endif