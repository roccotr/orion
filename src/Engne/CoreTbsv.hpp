#ifndef CORETBSV
#define CORETBSV
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"
#include "CoreSdmg.hpp"
#include "CoreMetb.hpp"
#include "CoreMeot.hpp"
#include "CoreSstb.hpp"
#include "CoreRvrs.hpp"
#include "CoreFsys.hpp"
#include "CorePart.hpp"
#include "CoreLstc.hpp"
#include "PrtoAlog.pb.h"
#include "PrtoSstb.pb.h"


namespace orion {

using namespace google::protobuf;

class CoreTbsv {

public:
	const static int iCredotimestampsize;
	const static AnsiString sCchar0;
	const static AnsiString sCchar1;
	const static AnsiString sCchar255;
	const static AnsiString sCcleanredotimestamp;
	const static AnsiString sCfastkeyupsertdelimiters; 
	const static AnsiString sCfastkeydeletedelimiters;

private:

  enum bCflagtypes { 
		bCdelete = 0x10,
    bCupsert = 0xFF
	};

	typedef struct CoreMetl {
		bool bVvalid;
		bool bVdeadmanwalking;
		BASE_CUSTOMMEMTABLE::PTR cVmemtable;
		bool bVneedcompaction;
		AnsiString sVredotimestamp;

		/*INITIALIZE*/
		CoreMetl () {
			bVvalid = false;
			bVneedcompaction = false;
			bVdeadmanwalking = false;
			sVredotimestamp = sCcleanredotimestamp;
			cVmemtable = boost::shared_ptr<BASE_CUSTOMMEMTABLE> (new BASE_CUSTOMMEMTABLE ());
		}
	} MEMTABLELOC;

	typedef struct CoreOstl {
		AnsiString sVindex;

		AnsiString sVtempfilenamenoext;
	} ORDEREDSSTABLELOC;

	typedef boost::shared_ptr<MEMTABLELOC> PTR_MEMTABLELOC;

	typedef struct CoreSstl {
		bool bVvalid;
		bool bVvalidasmemorytable;
		bool bVdeadmanwalking;
		int iVcompactionlevel;
		CoreHsrx::iCsstabletypes iVsstabletype;
		CoreSstb::PTR cVsstable;
		AnsiString sVredotimestamp;
		PTR_MEMTABLELOC cVlinkedmemtableloc;

		/*INITIALIZE*/
		CoreSstl () {

			bVvalid = false;
			bVvalidasmemorytable = false;
			bVdeadmanwalking = false;
			iVcompactionlevel = 0;
			sVredotimestamp = sCcleanredotimestamp;
			cVsstable = boost::shared_ptr<CoreSstb> (new CoreSstb ());
		}
	} SSTABLELOC;

	typedef struct CoreTabl {
		AnsiString sVindex;
	} TABLELOC;

	typedef boost::shared_ptr<SSTABLELOC> PTR_SSTABLELOC;

	typedef struct CorePmtl {
		PTR_MEMTABLELOC cVptr;

		/*INITIALIZE*/
		CorePmtl () {
			cVptr = boost::shared_ptr<MEMTABLELOC> (new MEMTABLELOC);
		}
	} PTRMEMTABLELOC;

	typedef struct CorePstl {
		PTR_SSTABLELOC cVptr;

		/*INITIALIZE*/
		CorePstl () {
			cVptr = boost::shared_ptr<SSTABLELOC> (new SSTABLELOC);
		}
	} PTRSSTABLELOC;

	int iVsstablecounter;
	int iVmemtabledim;
	int iVmemtablesize;
	int iVmemtablechunk;
	int64vcl iVgracetimestamp;
	CorePart::iCpartitiontype iVpartitiontype;
	bool bVmemorytable;
	bool bVvolatiletable;
	bool bVfastkey;
	bool bVtabletsystem;
	bool bVabort;
	AnsiString sVpath;
	AnsiString sVfilename;
	AnsiString sVnamespace;
	AnsiString sVtable;
	AnsiString sVredotimestamp;
	AnsiString sVsystemkey;
	CoreSmph cVdbopmutex;
	CoreSmph cVcompactionmutex;
	PTR_POOL cVpool; 
	PTR_SEDAMANAGER cVsedamanager;
	PTR_REVERSE cVreverse;
	PTR_FILESYSTEM cVserializer;
	boost::shared_ptr<CoreTbsv>  cVtabletsystem;
	PTR_MEMTABLELOC cVactivememloc;
	CoreList<PTRMEMTABLELOC> cVmemtablepool;
	CoreList<PTRSSTABLELOC> cVsstablepool;

	const static int iCmaxmemtabledim;
	const static int iCmaxmemtablesize;
	const static char bCmaindelimiter;
	const static char bCmajordelimiter;
	const static char bCqualifierdelimiter;

	AnsiString __fastcall getns ();
	int __fastcall gettc ();
	bool __fastcall gette ( int iElement, CoreTble::PTR& cTable);
	PTR_MEMTABLELOC __fastcall newmi (bool bActivememloc, AnsiString sRedotimestamp);
	PTR_MEMTABLELOC __fastcall newmt (bool bActivememloc, AnsiString sRedotimestamp);
	bool __fastcall majci (PTR_SSTABLELOC cS1, PTR_SSTABLELOC cS2, PTR_SSTABLELOC cSresult, int iCompactionlevel, bool bMemorytable);

public:

	static AnsiString __fastcall nstat (AnsiString sKey);
	static AnsiString __fastcall key_i (	
																PTR_REVERSE cReverse, AnsiString sMain, int& iMainindexlength, AnsiString sAccessgroup = NULL, 
																AnsiString sQualifier = NULL, 
																bool bAttributes = false, iCstatetype iState = UPSERT, int64vcl iTimestamp = 0);
	static AnsiString __fastcall key__ (	
																PTR_REVERSE cReverse, PrtoLkey* cKey, int& iMainindexlength, bool bAttributes = true);
	static AnsiString __fastcall fkey_ (PrtoLkey* cKey, int& iMainindexlength); 
	static void __fastcall key__ (PTR_REVERSE cReverse, AnsiString sKey, PrtoLkey* cKey);
	static bool __fastcall istmb (	PTR_REVERSE cReverse, AnsiString sKey);
	static bool __fastcall extmb (	PTR_REVERSE cReverse, AnsiString sOpaquevalue, int64vcl iGracetimestamp);

private:


	AnsiString __fastcall getmj ( AnsiString sKey);
	AnsiString __fastcall getfm ( AnsiString sKey);
	AnsiString __fastcall getmn ( AnsiString sKey);
	bool __fastcall dumps ();
	bool __fastcall rests (CoreList<TABLELOC>* cSystemsstable);
	bool __fastcall invss (PTR_SSTABLELOC cSstableloc, AnsiString sRedotimestamp, bool bAlsolinkedmemtable);
	bool __fastcall sstme (PTR_SSTABLELOC cSstableloc, AnsiString sRedotimestamp, bool bActivememtable = false);
	bool __fastcall loada (bool bResetlocal);
	bool __fastcall minci (bool bForcenew = false);
	bool __fastcall mnici (bool bNotmemory = false);
	bool __fastcall majci (bool& bUplevels, int iLevel, bool bQuick = false, bool bUplevel = false);
	bool __fastcall rcyci (bool bSstables = true, bool bMemtables = true);
	bool __fastcall cmlck (PTR_SSTABLELOC cSstableloc, int iCompactionlevel, bool bExact = true);

public:

	enum iCcompactions {
		iCminor = 0,
		iCmajor = 1,
		iCmerge = 2,
		iCflush = 3
	};

  __fastcall CoreTbsv ();	
  __fastcall ~CoreTbsv ();

	void __fastcall init_ (	PTR_POOL cPool, PTR_SEDAMANAGER cSedamanager,
													AnsiString sNamespace, AnsiString sTable,
													AnsiString sDatapath, AnsiString sTabletspath,
													AnsiString sSystemkey, int iGracetime, 
													boost::shared_ptr<CoreTbsv> cTabletsystem,
													bool bResetlocal = false);
	void __fastcall setsg (	PrtoLstr* cStorage);

	bool __fastcall muttr (PrtoLkey* cKey, PrtoLval* cValue, AnsiString sRedotimestamp);
	bool __fastcall query (PrtoLkey* cKey, PrtoLval* cValue, CoreTble::iCquerytype iQuerytype);
	
	bool __fastcall mincp (bool bForcenew = false);
	bool __fastcall mnicp (bool bNotmemory = false);
	bool __fastcall majcp (bool& bUplevels, int iLevel, bool bQuick = false, bool bUplevel = false);
	bool __fastcall shrnk (bool bSoft);

	bool __fastcall getti (RepeatedPtrField<PrtoTlel>* cTabletinfo, bool bDiagnostic = false);
	void __fastcall setab (bool bAbort = true);
	int64vcl __fastcall getsz ();

	void __fastcall clear ();
	AnsiString __fastcall getcr ();

	bool __fastcall clean ();
	bool __fastcall walog ();

	bool __fastcall rpair ();
	bool __fastcall fcomp ();

	AnsiString __fastcall debug ();


};


typedef boost::shared_ptr<CoreTbsv> PTR_TABLETSERVER;

};

#endif
