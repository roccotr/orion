#ifndef COREPLAN
#define COREPLAN
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"
#include "CorePeer.hpp"
#include "CoreSmph.hpp"
#include "CoreSdmg.hpp"
#include "CorePart.hpp"
#include "CorePars.hpp"
#include "CoreLiss.hpp"
#include "CoreLssc.hpp"
#include "PrtoAlog.pb.h"
#include "PrtoPlan.pb.h"

namespace orion {

class CorePlan {

#define iCcachetablepooldim 4

public:

	typedef bool (CorePlan::*POINTERFUNCPROTOMUTABLE)(PrtoLmtb*);

  enum iCpartitiontype {
		iCnopartition = 0,
		iCrandompartition = 1,
		iCbyteorderpartition = 2,
		iCcustompartition = 3,
		iCbalancedcustompartition = 4
  };

	enum iCstatstype {
		iCwritestats = 0,
		iCreadstats = 1,
		
		iCstatstypedim = 2
	};

	enum iCtabletype {
		iCstandardtable = 0,
		iCmemorytable = 1,
		iCvolatiletable = 2,

		iCtabletypedim = 3
	};

	enum iCindextype {
		iCdistributedindex = 0,
		iClocalindex = 1,

		iCindextypedim = 2
	};

	typedef struct CoreCllc {
		AnsiString sVindex;
		int iVtype;
		int iVaccessgroup;
		int iVaccessobjectitem;

		/*INITIALIZE*/
		CoreCllc () {
			
			iVaccessgroup = 0;
			iVaccessobjectitem = -1;
		}
	} COLUMNLOC;	

private:
	const static AnsiString sCdefaultaccessgroup;
	const static AnsiString sCindexseparator;
	const static double dCmovingaveragedimdelay;
	const static AnsiString sCpromptupdates;

	typedef struct CoreGslc {
		int64vcl iVstartuptime;

	} GLOBALSTATSLOC;

	typedef struct CoreIxcl {
		COLUMNLOC* cVcolumnloc;
		bool bVasc;

	} INDEXCOLUMNLOC;

	typedef struct CoreIxlc {
		AnsiString sVindex;
		CoreList<INDEXCOLUMNLOC> cVindexcolumnpool;

	} INDEXLOC;

	typedef struct CoreIxpl {
		INDEXLOC* cVindexloc;

	} INDEXPOINTERLOC;

	typedef struct CoreAglc {
		AnsiString sVindex;
		CoreList<INDEXPOINTERLOC> cVindexpointerpool;
		PrtoL2ao cVaccessobject;
		PrtoL2ao cVupdateaccessobject;
		bool bVupdateaccessobject;
		int64vcl iVupdatetimestamp;
		
	} ACCESSGROUPLOC;

	typedef struct CoreStlc {
		int64vcl iVcount;
		double dVmeandelay;
		double dVworstdelay;
		double dVtotaldelay;
		int64vcl iVsize;

		/*INITIALIZE*/
		CoreStlc () {
			
			iVcount = 0;
			dVmeandelay = 0; 
			dVworstdelay = 0; 
			dVtotaldelay = 0; 
			iVsize = 0;
		}
	} STATSLOC;

		typedef struct {
			AnsiString sVindex;
			PrtoL2cv cVprotoupdate;
		} UPDATELOC;

public:

	typedef struct CoreKylc {
		AnsiString sVmain;
		PrtoLkey cVkey;
		PrtoL2iv cVindexvalue;
		bool bVvalid;

		/*INITIALIZE*/
		CoreKylc() {
			
			sVmain = "";
			bVvalid = true;
		}
	} KEYLOC;

	typedef struct CorePklc {
		AnsiString sVindex;
		KEYLOC* cVkeyloc;

	} POINTERKEYLOC;

	typedef struct CoreCplc {
		COLUMNLOC* cVcolumnpointer;

	} COLUMNPOINTERLOC;

	typedef struct CoreOblc {
		AnsiString sVindex;

		AnsiString sVreferencefullindex;
		AnsiString sViteratorindex;
		AnsiString sVoriginalindex;

		int iVsortedkeyreference; 
	} ORDERBYKEYLOC;

	typedef struct CoreTblc {
		bool bVvalid;
		AnsiString sVindex;

		CoreList<COLUMNLOC> cVcolumnpool;
		CoreList<ACCESSGROUPLOC> cVaccessgrouppool;
		CoreList<INDEXLOC> cVindexpool;

		PrtoLstr cVstorage;
		PrtoLmtb cVmutable;
		
		CoreAlog::TABLETLOC* cVcachedlocaltablet;
		CoreAlog::TABLETLOC* cVcachedlocalindextablet;

		STATSLOC cVstatsloc [iCstatstypedim];
		CorePart::PARTITIONLOC cVpartitionloc;
		iCpartitiontype iVpartitiontype;
		bool bVlocalindex;
		CoreSmph cVlocalmutex;

		/*HELPERS*/
		CoreList<KEYLOC> cVkeysstm;
		CoreList<COLUMNLOC> cVcolumnsstm;
		CoreList<POINTERKEYLOC> cVpointerkeysstm;
		CoreList<COLUMNPOINTERLOC> cVcolumnpointerpoolql1;
		CoreList<KEYLOC> cVstatickeysql2;
		CoreList<POINTERKEYLOC> cVstaticsortedkeysql2;
		CoreList<ORDERBYKEYLOC> cVorderbykeysql2;
		CoreList<COLUMNPOINTERLOC> cVcolumnpointerpoolql2;

		/*INITIALIZE*/
		CoreTblc () {
			
			bVvalid = false;
			bVlocalindex = false;
			cVcachedlocaltablet = NULL;
			cVcachedlocalindextablet = NULL;
		}
	} TABLELOC;

	typedef boost::shared_ptr<TABLELOC> PTR_TABLELOC;

private:

	typedef struct {
		AnsiString sVindex;
	} ANSISTRINGLOC;


	CoreEnpo::NODEDATA cVnodedata;
	PTR_REVERSE cVreverse;
	PTR_POOL cVpool;
	PTR_PEER cVpeer;
	PTR_REDOLOG cVredolog;
	PTR_SEDAMANAGER cVsedamanager;
	PTR_PARTITIONER cVpartitioner;
	PTR_GOSSIPER cVgossiper;
	PTR_PARSER cVparser;
	CoreLssc<PTR_TABLELOC,TABLELOC, iCcachetablepooldim> cVtablepool;
	GLOBALSTATSLOC cVglobalstats;
	CoreSmph cVglobalmutex;

	bool __fastcall savet (PrtoL2ct* cCreatetable);
	bool __fastcall loadt (PrtoLmtb* cMutable, PTR_TABLELOC cTableloc, bool bRepair, AnsiString sXml = NULL);

	AnsiString __fastcall getxn (AnsiString sBasename);
	AnsiString __fastcall gettn (PrtoLmtb* cMutable);
	AnsiString __fastcall getxn (PrtoLmtb* cMutable);
	COLUMNLOC* __fastcall getcl (AnsiString sColumnname, PTR_TABLELOC cTableloc);
	ACCESSGROUPLOC* __fastcall getal (AnsiString sAccessgroupname, PTR_TABLELOC cTableloc);
	INDEXLOC* __fastcall getil (AnsiString sIndexname, PTR_TABLELOC cTableloc);
	int __fastcall getai (ACCESSGROUPLOC*, PTR_TABLELOC cTableloc);
	bool __fastcall qryl1 (PrtoL2qr* cQuery, PrtoL2mr* cDmlresult, bool bOnlysecondary, PTR_TABLELOC cTablelocpointer, bool bLocal = false);
	bool __fastcall qryl2 (	PrtoL2qr* cQuery, PrtoL2mr* cDmlresult, PTR_TABLELOC cTablelocpointer,
													bool bTeleport = false, bool bIndexloc = false, bool bIndexkey = false, 
													INDEXLOC* cIndexloc = NULL, CoreTble::iCquerytype iQuerytype = CoreTble::iCexactquery, 
													CoreList<POINTERKEYLOC>* cSortedkeys = NULL);	
	bool __fastcall qrl2t (PrtoL2qr* cQuery, PrtoL2mr* cDmlresult, PTR_TABLELOC cTablelocpointer);	
	bool __fastcall getik (	INDEXLOC* cIndexloc, RepeatedPtrField<PrtoL2cl>* cRepeatedvaluesource, 
													bool bAccessobjectitem,	int64vcl iTimestamp, iCstatetype iState, CoreList<KEYLOC>* cReturnkeys,
													bool bIndexsequence, AnsiString sUniquekey);
	bool __fastcall getil (PTR_TABLELOC cTableloc, PrtoL2wh* cWhere, INDEXLOC** cReturnindexloc, CoreTble::iCquerytype& iQuerytype);
	bool __fastcall getms (	int&iMask, RepeatedPtrField<PrtoL2cl>* cRepeatedselectsource, PTR_TABLELOC cTableloc, 
													CoreList<COLUMNPOINTERLOC>* cColumnpointerpool); 
	bool __fastcall qryag (	PrtoLkey* cBasekey, PrtoLmtb* cMutable, int iMask, CoreList<ACCESSGROUPLOC>* cAccessgrouppool, 
													CoreTble::iCquerytype iQuerytype, STATSLOC* cStatsloc, 
													CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc, 
													CoreAlog::TABLETLOC* cCachedtabletloc, PTR_TABLELOC cTableloc);
	bool __fastcall fllks (	PrtoL2qr* cQuery, PrtoL2mr* cDmlresult,
													PrtoLkey* cKey, CoreList<COLUMNPOINTERLOC>* cColumnpointerpool,
													CoreList<ACCESSGROUPLOC>* cAccessgrouppool, int iCount, bool bOnlysecondary);
	bool __fastcall valid ( AnsiString sReferencefullindex, AnsiString sReturnedfullindex, 
													PrtoL2iv* cIndexvalue, POINTERKEYLOC* cPointerkeyloc, PrtoL2wh* cWhere, bool& bForkeyslice);
	bool __fastcall getsk (	CoreList<KEYLOC>* cKeys, CoreList<POINTERKEYLOC>* cSortedkeys, RepeatedPtrField<PrtoL2cl>* cRepeatedcondition);
	bool __fastcall stati (	PrtoL2st* cStatement, PTR_TABLELOC cTablelocpointer, bool bOptimizerewriteindexes, 
													bool bUpdateonlyindexes, bool bLocalfilter);
	bool __fastcall slcti (PrtoL2mr* cDmlresult, PrtoL2qr* cQuery, PTR_TABLELOC cTablelocpointer, bool bLocal, bool bOnlysecondary);
	bool __fastcall slcfi (PrtoL2mr* cDmlresult, PrtoL2qr* cQuery, PTR_TABLELOC cTablelocpointer, bool bLocal, bool bOnlysecondary);
	bool __fastcall insri (PrtoL2st* cStatement, PrtoL2mr* cDmlresult, PTR_TABLELOC cTablelocpointer);
	bool __fastcall selct (PrtoL2mr* cDmlresult, AnsiString sSqlstring, PrtoL2os* cOsql, bool bLocal);
	bool __fastcall insrt (AnsiString sSqlstring, PrtoL2mr* cDmlresult, PrtoL2os* cOsql);
	bool __fastcall updti (PrtoL2st* cStatement, PrtoL2mr* cDmlresult, PTR_TABLELOC cTablelocpointer);
	bool __fastcall delti (PrtoL2st* cStatement, PrtoL2mr* cDmlresult, PTR_TABLELOC cTablelocpointer);
	bool __fastcall updte (PrtoL2mr* cDmlresult, AnsiString sSqlstring, PrtoL2os* cOsql);
	bool __fastcall delte (PrtoL2mr* cDmlresult, AnsiString sSqlstring, PrtoL2os* cOsql);
	static inline int __fastcall ckstr (AnsiString sString1, AnsiString sString2);
	static inline int __fastcall ckint (int iInt1, int iInt2);
	static inline int __fastcall ckdbl (double dDouble1, double dDouble2);
	static inline int __fastcall ckbol (bool bBool1, bool bBool2);
	static inline bool __fastcall ckcmp (iCconditiontype iConditiontype, int iCompared);
	static inline void __fastcall setrs (AnsiString cPrompt, PrtoL2mr* cResult, int iCount);
	static bool __fastcall ckraw (PrtoL2cl* cCondition, PrtoL2cl* cObject);

	bool __fastcall muttr (	PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, 
													CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc,
													AnsiString sKeyforendpoint, bool bLocalfilter, CoreAlog::TABLETLOC** cCachedtabletloc);
	bool __fastcall query (	PrtoLmtb* cMutable, PrtoLkey* cKey, PrtoLval* cValue, CoreTble::iCquerytype iQuerytype, 
													CorePart::iCpartitiontype iPartitiontype, CorePart::PARTITIONLOC* cPartitionloc,
													CoreAlog::TABLETLOC* cCachedtabletloc, CoreSmph* cLockedmutex);
	bool __fastcall setsg (PrtoLmtb* cMutable, PTR_TABLELOC cTableloc);
	bool __fastcall setsg (PrtoLmtb* cMutable);
	bool __fastcall getim (PrtoLmtb* cMutable, PrtoLmtb& cMutableitable);
	bool __fastcall rpaii (PrtoL2st* cStatement);
	void __fastcall stats (PTR_TABLELOC cTableloc, TDateTime tTimestampstart, iCstatstype iStatstype, int iCount);
	double __fastcall getpt (PrtoLmtb* cMutable, bool bIndex, AnsiString& sPartition, double dPartition = 0.5);
	bool __fastcall shrni (PrtoLmtb* cMutable, bool bSoft);
	CorePart::iCpartitiontype getpt (iCpartitiontype iTablepartitiontype, bool bIndex);
	static AnsiString getpl (iCpartitiontype iPartitiontype);

public:

	const static AnsiString sCflushwriter;
	const static AnsiString sCmemtablepostflusher;
	const static AnsiString sCredologswitcher;
	const static AnsiString sCgossipworker;
	const static AnsiString sCremotewriter;
	const static AnsiString sCremotereader;
	const static AnsiString sChhworker;
	const static AnsiString sCmemtablepartial;
	const static AnsiString sCredologrecycler;


  __fastcall CorePlan(	PTR_POOL cPool, CoreEnpo::NODEDATA& cNodedata);
  __fastcall ~CorePlan ();

  void __fastcall init_ ( PTR_VOID cThis, CoreEnpo::iCstategossipnode iState, bool bResetlocal = false);
	void __fastcall end__ ();

	PTR_PEER __fastcall getpr ();
	PTR_SEDAMANAGER __fastcall getsm ();
	bool __fastcall insrt (PrtoLmtb* cMutable, PrtoPval* cInputrow);
	bool __fastcall crttb (PrtoL2ct* cCreatetable, PrtoL2st* cStatement = NULL);
	bool __fastcall crttb (PrtoLmtb* cMutable);

	bool __fastcall query (PrtoL2qr* cQuery, PrtoL2mr* cDmlresult);
	bool __fastcall lcqry (PrtoLqry* cQuery, PrtoLval* cValueresult, bool bGlobalmutex = false);
	bool __fastcall osql_ (PrtoL2os* cOsql, PrtoL2mr* cDmlresult);
	bool __fastcall statm (	PrtoL2st* cStatement, bool bOptimizerewriteindexes = true, 
													bool bUpdateonlyindexes = false, bool bLocalfilter = false,
													PTR_TABLELOC* cTablelocpointer = NULL);
	bool __fastcall lcstm (PrtoLstm* cStatement, bool bNomutex = false, CoreAlog::TABLETLOC** cTabletlocpointer = NULL);
	void __fastcall debgi (	PrtoLmtb* cMutable, AnsiString sKey, AnsiString sReversekey = NULL, 
													CoreTble::iCquerytype iVquerytype = CoreTble::iCtokenquery);

	bool __fastcall compa (	PrtoLmtb* cMutable, CoreTbsv::iCcompactions iCompaction, 
													int iCompactionlevel = 0);
	bool __fastcall swtch ();
	bool __fastcall rcycr ();
	bool __fastcall gossp (PrtoGoss* cGossip, PrtoGosr* cGossipresult);
	int64vcl __fastcall getsz (); 
	void __fastcall getgp (PrtoGoss* cGossipresult, bool bDetails = false);
	void __fastcall getgp (PrtoGoel* cGossipresult);
	bool __fastcall join_ (AnsiString sAddress, int iPort, AnsiString sNewtoken, bool bAlign);
	bool __fastcall rebnd (AnsiString sAddress, int iPort);
	bool __fastcall leave (bool bAlign);
	void __fastcall setab (bool bAbort = true);
	AnsiString __fastcall getnt ();

	bool __fastcall uptbl (AnsiString sTabletfilename, iCstatetype iCstate, PrtoTabl* cTable);
	bool __fastcall dltbl (PrtoLmtb* cMutable); 
	bool __fastcall sttbl (AnsiString sTabletfilename, PTR_TABLELOC cTableloc, AnsiString sXml = "");
	bool __fastcall sttbl (PrtoLmtb* cMutable, AnsiString sXml = ""); 
	bool __fastcall sttbs ();
	bool __fastcall ldtbl (AnsiString sTabletfilename, PrtoTabl* cTable);
	bool __fastcall ldtbl (PrtoLmtb* cMutable, PrtoTabl* cTable); 
	bool __fastcall rltbl (PrtoLmtb* cMutable); 
	bool __fastcall iftbl (PrtoLmtb* cMutable, PrtoTabl* cTable, bool bDiagnostic = false);


	bool __fastcall stats (iCstatstype iStatstype, PrtoLmtb* cMutable, PrtoStel* cStatselement);

	PTR_TABLETSERVER gettb (PrtoLmtb* cMutable);
	PTR_TABLELOC __fastcall gettl (PrtoLmtb* cMutable, bool bRepair = false, AnsiString sXml = NULL);
	PTR_TABLELOC __fastcall gettl (AnsiString sNextindex);

	bool __fastcall lnch_ (PrtoLmtb* cMutable, POINTERFUNCPROTOMUTABLE cPointerfunc);
	bool __fastcall touch (PrtoLmtb* cMutable);
	bool __fastcall rpair (PrtoLmtb* cMutable);
	bool __fastcall rbldi (PrtoLmtb* cMutable);
	bool __fastcall split (PrtoLmtb* cMutable);
	bool __fastcall split (PrtoLmtb* cMutable, AnsiString sPartition, AnsiString sPartitionindex);
	bool __fastcall frzsp (PrtoLmtb* cMutable);
	bool __fastcall rbpar (PrtoLmtb* cMutable);
	bool __fastcall clean (PrtoLmtb* cMutable);
	bool __fastcall purge (PrtoLmtb* cMutable);
	bool __fastcall trunc (PrtoLmtb* cMutable);
	bool __fastcall shrnk (PrtoLmtb* cMutable);
	bool __fastcall soshr (PrtoLmtb* cMutable);
	bool __fastcall fcomp (PrtoLmtb* cMutable); 
	bool __fastcall anlyz (PrtoLmtb* cMutable, int64vcl iTimestamp);
	bool __fastcall clone (PrtoLmtb* cMutablesource, PrtoLmtb* cMutabledestination, int64vcl iTimestamp, int64vcl& iVreturn);

	bool __fastcall pulls ();
	bool __fastcall pulld (AnsiString sNodeid);
	bool __fastcall ppmch (PrtoAenp* cAntientrophy);
	bool __fastcall pprep (PrtoAenp* cAntientrophy);
	
	bool __fastcall setsm (bool bActive);

	bool __fastcall top__ (PrtoTop_& cVtop);

};

typedef boost::shared_ptr<CorePlan> PTR_PLANNER;

};


#endif