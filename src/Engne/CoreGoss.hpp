#ifndef COREGOSS
#define COREGOSS
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreEnpo.hpp"
#include "CoreAlog.hpp"
#include "CoreSdmg.hpp"
#include "PrtoAlog.pb.h"

namespace orion{

class CoreGoss {

public:

	typedef struct CoreWrlc {
		AnsiString sVindex;

	} WRITERREADERLOC;

	typedef struct CoreAddr {

		bool bVissuerefreshstructures;
		CoreEnpo::GOSSIPLOC* cVgossiploc;
	} ADDGOSSIPLOCRETURN;

	const static double dCalivethreshold;
	const static AnsiString sCobjectmanager;

private:

	const static AnsiString sCgossiperkey;
	const static AnsiString sCmaxmd5bound;
	const static double dCmovingaveragedimphi;
	const static double dCmovingaveragedimtmean;
	const static double dCphifactor;
	const static int iCwgoldms;

	int iVcurrentwriter;
	int iVcurrentreader;	
	CoreList<WRITERREADERLOC> cVwriterpool;
	CoreList<WRITERREADERLOC> cVreaderpool;
	CoreList<CoreEnpo::GOSSIPLOC> cVgossippool;
	CoreSmph cVmutex;
	CoreEnpo::GOSSIPLOC* cVthisgossiploc;
	PTR_ENDPOINT cVendpoint;
	PTR_REDOLOG cVredolog;
	PTR_SEDAMANAGER cVsedamanager;

	bool __fastcall wgold ();
	void __fastcall togos (PrtoGoss& cGossiptoserialize);
	void __fastcall updpa (CoreEnpo::GOSSIPLOC* cGossiploc, double dValue, double dVelapsed);

public:
  __fastcall CoreGoss ();
  __fastcall ~CoreGoss ();
	void __fastcall init_ (PTR_ENDPOINT cEndpoint, PTR_REDOLOG cRedolog);
	void __fastcall init_ (PTR_SEDAMANAGER cSedamanager);

	void __fastcall addwr (AnsiString sWriter);
	void __fastcall addrd (AnsiString sReader);

	bool __fastcall getge (AnsiString sNodeid, CoreEnpo::GOSSIPLOC& cGossipelement, int iOffset = 0);
	ADDGOSSIPLOCRETURN addgl (	PrtoGoel* cGossipelement, 
															bool bLocal = false, bool bWritetodb = true, bool bForceringupdate = false);
	bool __fastcall check (AnsiString sAddress, int iPort);
	bool __fastcall rgold ();
	void __fastcall state (CoreEnpo::iCstategossipnode iState);
	void __fastcall chnge (AnsiString sAddress, int iPort);
	void __fastcall newtk (AnsiString sNewtoken);
	AnsiString __fastcall getnt ();

	void __fastcall getgp (PrtoGoel* cGossipresult);
	void __fastcall getgp (PrtoGoss* cGossipresult);

	bool __fastcall gossp (PrtoGoss* cGossip, PrtoGosr* cGossipresult, bool& bIssuerefreshstructures);
	bool __fastcall getrn (CoreEnpo::GOSSIPLOC& cGossiploc);
	bool __fastcall setrn (CoreEnpo::GOSSIPLOC& cGossiploc, double dValue, double dVelapsed);
	bool __fastcall setrn (PrtoGoel* cGossipelement, double dValue, double dVelapsed);

	AnsiString __fastcall debug ();
};

typedef boost::shared_ptr<CoreGoss> PTR_GOSSIPER;

};

#endif