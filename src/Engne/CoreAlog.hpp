#ifndef COREALOG
#define COREALOG
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"
#include "CoreSdmg.hpp"
#include "CoreRvrs.hpp"
#include "CoreList.hpp"
#include "CoreTbsv.hpp"
#include "CoreStix.hpp"
#include "PrtoAlog.pb.h"
#include "CoreLstc.hpp"

namespace orion {

#define iCcacheredologpooldim 4

using namespace google::protobuf;

class CoreAlog {

	const static AnsiString sCredologprefix;
	const static AnsiString sCredologdelimiter;
	const static AnsiString sCredologext;
	const static int iCmaxactivelogdim;

	typedef struct CoreOstl {
		AnsiString sVindex;

		int64vcl iVredologorderingcounter;
		AnsiString sVtempfilename;
	} ORDEREDREDOLOGFILELOC;

	typedef struct CoreLogt {
		AnsiString sVindex;
		PTR_TABLETSERVER cVtabletserver;
		AnsiString sVtimestamp;

	} REDOLOGTABLET;

	typedef struct CoreLogf {
		bool bVvalid;
		AnsiString sVfilename;
		CoreLstc<REDOLOGTABLET, iCcacheredologpooldim> cVredologtabletpool;

		/*INITIALIZE*/
		CoreLogf () {
			bVvalid = true;
		}
	} REDOLOGFILELOC;

public:

	const static AnsiString sCsystemnamespace;
	const static AnsiString sCglobalsystemtable;
	const static AnsiString sClocalsystemtable;

	typedef struct CoreTbsl {
		AnsiString sVindex;
		bool bVsystem;
		PTR_TABLETSERVER cVtablet;
		int iVrecyclecounter;

		int64vcl iVwritecount;
		int64vcl iVreadcount;

		/*INITIALIZE*/
		CoreTbsl () {
			iVrecyclecounter = 0;
			iVwritecount = iVreadcount = 0;
			cVtablet = boost::shared_ptr<CoreTbsv> (new CoreTbsv ());
		}

	} TABLETLOC;

private:
	AnsiString sVdatapath;
	AnsiString sVtabletspath;
	AnsiString sVredologspath;
	int iVindexcount;
	int iVactivelogdim;
	int iVredologdim;
	int iVgracetime;
	int64vcl iVredologcounter;
	bool bVabort;
	bool bVresetlocal;
	PTR_POOL cVpool;
	PTR_SEDAMANAGER cVsedamanager;
	PTR_REVERSE cVreverse;
	CoreLstc<REDOLOGFILELOC, iCcacheredologpooldim> cVredologfilepool;
	REDOLOGFILELOC* cVactiveredologfile;
	CoreLstc<TABLETLOC, iCcacheredologpooldim> cVtabletpool;
	PTR_FILESYSTEM cVredologwriter;
	CoreSmph cVtabletpoolmutex;
	CoreSmph cVredologfilepoolmutex;
	CoreSmph cVredologwritermutex;
	CoreSmph cVredoixmutex;
	TABLETLOC* cVtabletsystemloc;

	void __fastcall open_ ();
	AnsiString __fastcall getix ();
	REDOLOGFILELOC* __fastcall newri (bool bActive);
	REDOLOGFILELOC* __fastcall newrl (bool bActive = true);
	bool __fastcall wrtrl (PrtoLstm* cStatement, TABLETLOC* cTabletloc, AnsiString& sRedotimestamp);
	REDOLOGTABLET* getrt (REDOLOGFILELOC* cRedologfileloc, TABLETLOC* cTabletloc);
	void __fastcall remve (REDOLOGFILELOC* cRedologfileloc);
	bool __fastcall rcvry (bool bPrintf = false);
	bool __fastcall rcvry (REDOLOGFILELOC* cRedologfileloc, bool bPrintf = false);
	bool __fastcall loada (bool bPrintf = false);
	bool __fastcall load_ (AnsiString sFilename);

	bool __fastcall setsg (PrtoLmtb* cMutable);
	void __fastcall rcycl ();
	AnsiString __fastcall newrn ();

public:

	const static AnsiString sCsystemlogictable;
	const static AnsiString sCitableprefix;
	const static AnsiString sCtabletdelimiter;
	const static AnsiString sCtablekeydelimiter;

  __fastcall CoreAlog(	PTR_POOL cPool, PTR_SEDAMANAGER cSedamanager,
												AnsiString sRedologspath, AnsiString sDatapath, AnsiString sTabletspath, 
												int iRedologdim, bool bResetlocal = false, int iGracetime = 0);
  __fastcall ~CoreAlog ();

	void __fastcall close ();
	void __fastcall kill_ ();

	bool __fastcall statm (PrtoLstm* cStatement, TABLETLOC** cCachedtabletloc, AnsiString sRedotimestamp = NULL);

	bool __fastcall query (	PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, 
		CoreTble::iCquerytype iQuerytype, TABLETLOC* cCachedtabletloc);
	bool __fastcall compa		(PrtoLmtb* cMutable, CoreTbsv::iCcompactions iCompaction, 
													int iCompactionlevel, int iMaxcompactionlevel);
	bool __fastcall swtch ();
	void __fastcall setab (bool bAbort = true);

	bool __fastcall iftbl (AnsiString sTabletfilename, PrtoTabl* cTablet, bool bDiagnostic = false);
	PTR_TABLETSERVER __fastcall gettb (PrtoLmtb* cMutable);

	static bool __fastcall index (PrtoLmtb* cMutable);
	static bool __fastcall ix2tb (PrtoLmtb* cMutable);
	static void __fastcall tb2ix (PrtoLmtb* cMutable);

	static AnsiString __fastcall gettk (PrtoLmtb* cMutable) ;
	static AnsiString __fastcall getix (PTR_REVERSE cReverse, int& iCounter);
	static void __fastcall getrx (PTR_REVERSE cReverse, AnsiString sIndex, int64vcl& iTimestamp, int& iCounter);
	static AnsiString __fastcall logrx (PTR_REVERSE cReverse, AnsiString sIndex);
	static AnsiString __fastcall logrs (PTR_REVERSE cReverse, AnsiString sIndex);

	static AnsiString __fastcall gettk (PrtoLmtb* cMutable, AnsiString& sNamespace, AnsiString& sTable) ;
	static void __fastcall settk (AnsiString sIndex, PrtoLmtb* cMutable);

	TABLETLOC* __fastcall gettb (PrtoLmtb* cMutable, bool bTabletsystem, bool bCreate = true, bool bRepair = false);

	int64vcl __fastcall getsz ();

	AnsiString __fastcall debug ();
};

typedef boost::shared_ptr<CoreAlog> PTR_REDOLOG;

};

#endif