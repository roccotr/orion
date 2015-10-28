#ifndef COREPEER
#define COREPEER
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"
#include "CoreSdmg.hpp"
#include "CoreAlog.hpp"
#include "CoreEnpo.hpp"
#include "CorePart.hpp"
#include "CoreHiha.hpp"
#include "CoreGoss.hpp"
#include "PrtoAlog.pb.h"
#include "PrtoPlan.pb.h"

namespace orion {

class CorePeer {

private:

	const static int iCtimeoutsedacallbacksms;

	typedef struct CorePrlc {
		AnsiString sVindex;
		PrtoQres* cVqueryresult;

	} POINTERRESLOC;

	AnsiString sVthisnodeid;
	int64vcl iVticket; 
	PTR_POOL cVpool;
	PTR_SEDAMANAGER cVsedamanager;
	PTR_REDOLOG cVredolog;
	PTR_HINTEDHANDOFF cVhintedhandoff;
	PTR_PARTITIONER cVpartitioner;
	PTR_GOSSIPER cVgossiper;
	PTR_ENDPOINT cVendpoint;
	CoreSmph cVmutexticket;
	PTR_REVERSE cVreverse;

	AnsiString getti ();
	void __fastcall fmgos (PrtoGoss& cGossipparsed);
	bool __fastcall rmqry (	PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, CoreTble::iCquerytype iQuerytype, 
													CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc,
													bool& bNodeoffset, int iNodeoffset, CoreAlog::TABLETLOC* cCachedtabletloc, CoreSmph* cLockedmutex);

public:

	const static AnsiString sCrrworker;
	const static int iCsleepgossiperms;

  __fastcall CorePeer(PTR_POOL cPool, PTR_SEDAMANAGER cSedamanager, CoreEnpo::NODEDATA& cNodedata,
											CoreEnpo::iCstategossipnode iState, bool bResetlocal = false);
  __fastcall ~CorePeer ();

	bool __fastcall muttr (	iCopcodetype iOpcodetype, PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, 
													CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc,
													AnsiString sKeyforendpoint, bool bLocalfilter, CoreAlog::TABLETLOC** cCachedtabletloc);
	bool __fastcall muttr (	PrtoLstm* cStatement, CoreAlog::TABLETLOC** cCachedtabletloc);

	bool __fastcall query (	PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, CoreTble::iCquerytype iQuerytype, 
													CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc, 
													CoreAlog::TABLETLOC* cCachedtabletloc, CoreSmph* cLockedmutex, bool bLocal = false);
	bool __fastcall tlprt ( PrtoLmtb* cMutable, AnsiString sMain, 
													CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc,
													CoreEnpo::GOSSIPLOC& cGossiploc); 

	PTR_REDOLOG __fastcall getal ();
	bool __fastcall compa (	PrtoLmtb* cMutable, CoreTbsv::iCcompactions iCompaction, 
													int iCompactionlevel, int iMaxcompactionlevel);
	bool __fastcall swtch ();

	bool __fastcall joina (AnsiString sAddress, int iPort, AnsiString sNewtoken);
	bool __fastcall joinb (bool bAlign);
	bool __fastcall rebnd (AnsiString sAddress, int iPort);
	bool __fastcall leave (void* cAntientrophy, bool bAlign);
	void __fastcall setab (bool bAbort = true);
	bool __fastcall pull_ ();

	void __fastcall addwr (AnsiString sWriter);
	void __fastcall addrd (AnsiString sReader);
	bool __fastcall puthh (AnsiString sNode, PrtoLsts* cStatements);
	void __fastcall prchh ();

	bool __fastcall iftbl (AnsiString sTabletfilename, PrtoTabl* cTablet, bool bDiagnostic = false);

	PTR_TABLETSERVER __fastcall gettb (PrtoLmtb* cMutable);
	PTR_PARTITIONER getpt ();
	PTR_ENDPOINT getep ();
	PTR_GOSSIPER __fastcall getgo ();

	AnsiString __fastcall getni ();


};

typedef boost::shared_ptr<CorePeer> PTR_PEER;

};

#endif