#ifndef COREENPO
#define COREENPO
#include "CoreCofa.hpp"
#include "CoreVcl_.hpp"
#include "CoreSmph.hpp"
#include "CoreList.hpp"

namespace orion{

class CoreEnpo {

private:
	const static int iCconnectiontimeoutms;

public:

  enum iCconsistencylevel {
		iCany = 0,
		iCone = 1,
		iCtwo = 2,
		iCthree = 3,
		iCquorum = 10, 
		iClocalquorum = 11,
		iCall = 12
  };

  enum iCdatacenterlevel {
		iCsimplestrategy = 0,
		iCnetworktopologystrategy = 1
  };

#define iCtypeconsistencyleveldim 2

	enum iCtypeconsistencylevel {
		iCwrite = 0,
		iCread = 1
	};

	typedef struct CoreThdt {
		AnsiString sVtoken;
		AnsiString sVnodeid;
		AnsiString sVdatacenterid;

		AnsiString sVpath;
		AnsiString sVfilename;
		AnsiString sVdatapath;
		AnsiString sVtabletpath;
		AnsiString sVredologpath;
		int iVredologdim;
		int iVmaxcompactionlevel;
		int iVreplicationfactor;
		int iVwriteconsistencylevel;
		int iVreadconsistencylevel;
		int iVdatacenterlevel;

		AnsiString sVthriftaddress;
		AnsiString sVthriftgossiperaddress;
		AnsiString sVordbcaddress;
		int iVthriftport;
		int iVthriftthreadedport;
		int iVthriftgossiperport;
		int iVthriftlisteners;
		int iVordbcport;

		int iVconcurrentwrites;
		int iVconcurrentreads;

		int iVgracetime;
	} NODEDATA;

	enum iCstategossipnode {
		iCrunning = 0,
		iCjoining = 1,
		iCleaving = 2,
		iCstopped = 3,
		iCloading = 4
	};

	typedef struct CoreGplc {
		PTR_CONNECTIONFACTORY cVconnectionfactory;

		AnsiString sVindex; 		//node
		AnsiString sVtoken;

		AnsiString sVaddress;
		int iVport;

		AnsiString sVdatacenterid;
		
		double dVphiaccrual;
		double dVtmean;
		bool bValive;
		bool bVlocal;

		AnsiString sVwriter;
		AnsiString sVreader;

		iCstategossipnode iVstate;
		int64vcl iVtimestamp;



		CoreGplc () {
			dVphiaccrual = 0;
			dVtmean = 0;
			bValive = false;
			bVlocal = false;
			iVtimestamp = -1;
			cVconnectionfactory = boost::shared_ptr<CoreCofa> (
				new CoreCofa (CoreEnpo::iCconnectiontimeoutms));
		}
	} GOSSIPLOC;

	typedef struct CoreRglc {
		AnsiString sVindex;

		GOSSIPLOC* cVgossiploc;
	} RINGGOSSIPLOC;

	typedef struct CorePglc {

		int iVelement;
		bool bVcallback;
	} POINTERGOSSIPLOC;

	typedef struct CoreRpdt {

		int iVremotereplicas;
		int iVlocalreplicas;
		bool bVnotsorted;
		CoreList<CoreEnpo::POINTERGOSSIPLOC> cVpointergossippool;
	} REPLICATIONDATA;

private:
	NODEDATA cVnodedata;
	int iVw [iCtypeconsistencyleveldim];
	CoreSmph cVmutex;
	CoreList<CoreEnpo::RINGGOSSIPLOC> cVringgossippool;
	bool bVreadrepair;

	int __fastcall calcw (int iConsistencylevel);

public:
  __fastcall CoreEnpo ();
  __fastcall ~CoreEnpo ();

	void __fastcall init_ (	NODEDATA& cNodedata);
	bool __fastcall getrp (	iCtypeconsistencylevel iType,
													int iSeed, 
													REPLICATIONDATA& cReplicationdata,
													bool& bOffset, int iOffset, int iCustomreplicationfactor);
	bool __fastcall getsg (	int iSeed, bool& bLocal, CoreEnpo::GOSSIPLOC& cGossipelement);
	int get_w (iCtypeconsistencylevel iType);
	bool __fastcall getpr ( CoreList<RINGGOSSIPLOC>& cRinggossippoolresult);
	bool __fastcall joind ();
	int __fastcall next_ (int iCursor, int iDirection = 1);
	int __fastcall tk2po (AnsiString sToken);
	GOSSIPLOC* __fastcall getrg (int iPosition);
	void __fastcall setrg (AnsiString sToken, GOSSIPLOC* cGossiploc);
	void __fastcall ustrg (AnsiString sToken);
	bool __fastcall getrn (CoreEnpo::GOSSIPLOC& cGossiploc);
	NODEDATA* __fastcall getnd ();
	int __fastcall getrc ();
	bool __fastcall getrr ();
	int __fastcall getrf ();
	AnsiString __fastcall getni ();
	AnsiString __fastcall gettk ();
	void __fastcall settk (AnsiString sNewtoken);
	AnsiString __fastcall rngtk (AnsiString sNodeid);
	static PTR_CONNECTIONFACTORY __fastcall getcf (GOSSIPLOC* cVgossiploc);

	AnsiString __fastcall debug ();
};

typedef boost::shared_ptr<CoreEnpo> PTR_ENDPOINT;

};

#endif