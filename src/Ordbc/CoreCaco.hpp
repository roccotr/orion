#ifndef CORECACO
#define CORECACO
#include "CoreCocf.hpp"
#include "CoreCwrk.hpp"
#include "CoreList.hpp"


class CoreCaco {

public:
	typedef struct CoreBrlc {
		AnsiString sVindex;
		int iVworker;
		CoreCwrk* cVworker;
	} BROKERLOC;

private:
	int iVsubcounter;
	int iVbulksize;
	int iVworkers;
	int iVsimplerule;
	int iVrefcount;
	int iVassignworker;
	CoreSmph cVmutex;
	CoreList<CoreCwrk> cVworkerpool;
	CoreList<BROKERLOC> cVbrokerpool;
	PTR_ORIONCONNECTIONFACTORY cVorionconnectionfactory;

	bool __fastcall addjb (CoreCwrk::CLIENTJOBLOC* cJob, int iWorker = -1);

public:
  __fastcall CoreCaco	(PTR_ORIONCONNECTIONFACTORY cOrionconnectionfactory, int iWorkers, int iBulksize);
  __fastcall ~CoreCaco ();

	bool __fastcall addrf ();
	bool __fastcall subrf ();

	bool __fastcall valid ();
	bool __fastcall run__ (PrtoSrvr& cReturn, PrtoSrvc& cService, bool bOptimistic, int iWorker = -1);

	BROKERLOC getbl (AnsiString sIndex);
	bool __fastcall run__ (PrtoSrvr& cReturn, PrtoSrvc& cService, bool bOptimistic, CoreCwrk* cWorker);
};

typedef boost::shared_ptr<CoreCaco> PTR_ARIESCONNECTION;

#endif