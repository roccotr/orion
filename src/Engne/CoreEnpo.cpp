#include "CoreEnpo.hpp"
#include "CoreGoss.hpp"
#include "PrtoAlog.pb.h"
#include "TmplList.cpp"

using namespace orion;

	const int CoreEnpo::iCconnectiontimeoutms = 30000;

  __fastcall CoreEnpo::CoreEnpo() {  
	}

  __fastcall CoreEnpo::~CoreEnpo () {
	}

	/*CALCulate W*/
	int __fastcall CoreEnpo::calcw (int iConsistencylevel) {
		int iVw;

		switch (iConsistencylevel) {
			case iCany: iVw = 0; break;
			case iCall: iVw = cVnodedata.iVreplicationfactor; break;
			case iCone: iVw = Min (cVnodedata.iVreplicationfactor, 1); break;
			case iCtwo: iVw = Min (cVnodedata.iVreplicationfactor, 2); break;
			case iCthree: iVw = Min (cVnodedata.iVreplicationfactor, 3); break;
			case iCquorum: iVw = (cVnodedata.iVreplicationfactor + 1)/2; break;
			case iClocalquorum: iVw = cVnodedata.iVreplicationfactor/2; break;
			default: iVw = 0;
		}
		return iVw;
	}

	void __fastcall CoreEnpo::init_ (NODEDATA& cNodedata) {
		
		cVnodedata = cNodedata;
		iVw [iCwrite] = calcw (cVnodedata.iVwriteconsistencylevel);
		iVw [iCread] = calcw (cVnodedata.iVreadconsistencylevel);
		bVreadrepair = (cVnodedata.iVreadconsistencylevel == CoreEnpo::iCone) && (cVnodedata.iVreplicationfactor > 1);
	}

	/*GET Replication Pool*/
	bool __fastcall CoreEnpo::getrp (	iCtypeconsistencylevel iType, int iSeed, 
																		CoreEnpo::REPLICATIONDATA& cReplicationdata,
																		bool& bOffset, int iOffset, int iCustomreplicationfactor
																		) {
		int iV;
		int iVreplicationfactor;
		int iVseed;
		int iVcount;
		POINTERGOSSIPLOC* cVpointergossiploc;

		cReplicationdata.cVpointergossippool.clear ();
		cVmutex.locks ();
		iVcount = cVringgossippool.count ();
		switch (cVnodedata.iVdatacenterlevel) {
			case iCsimplestrategy:
				if (iSeed < 0) {
					iVseed = 0;
					iVreplicationfactor = iVcount;
					cReplicationdata.bVnotsorted = true;
				} else {
					iVseed = iSeed;
					iVreplicationfactor = iCustomreplicationfactor > 0 ? iCustomreplicationfactor : cVnodedata.iVreplicationfactor;
					cReplicationdata.bVnotsorted = false;
				}
				cReplicationdata.iVlocalreplicas = 0;
				bOffset = false;
				for (iV = 0; iV < iVreplicationfactor; iV++) {
					cVpointergossiploc = cReplicationdata.cVpointergossippool.set__ ();						
					cVpointergossiploc->iVelement = (iV + iVseed + iOffset) % iVcount;
					if ((cVpointergossiploc->iVelement == (iVcount - 1)) && (iV == 0)) bOffset = true;
					if (cVringgossippool.get__ (cVpointergossiploc->iVelement)->cVgossiploc->bVlocal) cReplicationdata.iVlocalreplicas++;				
					cVpointergossiploc->bVcallback = iVw [iType] > 0;
				}
				cReplicationdata.iVremotereplicas = Max ((cReplicationdata.bVnotsorted ? iVcount : iVw [iType]) - cReplicationdata.iVlocalreplicas, 0);
				break;
			default:
				cReplicationdata.iVremotereplicas = 0;
		}
		cVmutex.relse ();
		return true;
	}

	/*GET Single Gossip element*/
	bool __fastcall CoreEnpo::getsg (	int iSeed, bool& bLocal, CoreEnpo::GOSSIPLOC& cGossipelement) {
		int iVcount;
		bool bVreturn;
		double dVthreshold;
		CoreEnpo::GOSSIPLOC* cVgossiploc;

		if (iSeed < 0) return false;
		bVreturn = bLocal = false;
		dVthreshold = CoreGoss::dCalivethreshold;
		cVmutex.locks ();
		iVcount = cVringgossippool.count ();
		switch (cVnodedata.iVdatacenterlevel) {
			case iCsimplestrategy:
				for (int iV = 0; iV < cVnodedata.iVreplicationfactor; iV++) {				
					cVgossiploc = cVringgossippool.get__ ((iV + iSeed) % iVcount)->cVgossiploc;
					if (cVgossiploc->bVlocal){
						bLocal = true;
						bVreturn = true;
						break;
					}
					if (cVgossiploc->dVphiaccrual > dVthreshold) {
						cGossipelement = *cVgossiploc;
						dVthreshold = cVgossiploc->dVphiaccrual;
						bVreturn = true;
					}
				}
				break;
		}
		cVmutex.relse ();
		return bVreturn;
	}

	/*GET W*/
	int CoreEnpo::get_w (CoreEnpo::iCtypeconsistencylevel iType) {
		return iVw [iType];
	}

	/*GET Previous Replication pool*/
	bool __fastcall CoreEnpo::getpr ( CoreList<RINGGOSSIPLOC>& cRinggossippoolresult) {
		int iVcount;
		int iVseed;
		int iVtemp;
		RINGGOSSIPLOC* cVringgossiploc;

		cRinggossippoolresult.clear ();
		cVmutex.locks ();
		iVcount = cVringgossippool.count ();
		iVseed = cVringgossippool.bnsrc (cVnodedata.sVtoken) - 1;
		switch (cVnodedata.iVdatacenterlevel) {
			case iCsimplestrategy:
				cVringgossiploc = cRinggossippoolresult.set__ ();		
				iVtemp = (iVcount + iVseed - 1) % iVcount;
				cVringgossiploc->cVgossiploc = cVringgossippool.get__ (iVtemp)->cVgossiploc;
				break;
		}
		cVmutex.relse ();
		return true;
	}

	/*JOINeD*/
	bool __fastcall CoreEnpo::joind () {
		bool bVreturn;

		cVmutex.locks ();
		bVreturn = cVringgossippool.count () > 1;
		cVmutex.relse ();
		return bVreturn;
	}

	/*NEXT*/
	int __fastcall CoreEnpo::next_ (int iCursor, int iDirection) {
		int iVreturn;
		int iVsize;

		cVmutex.locks ();
		iVsize = cVringgossippool.count ();
		iVreturn = (iCursor + iDirection + iVsize) % iVsize;
		cVmutex.relse ();
		return iVreturn;
	}

	/*ToKen 2 POsition*/
	int __fastcall CoreEnpo::tk2po (AnsiString sToken) {
		int iVreturn;
		int iVcount;

		cVmutex.locks ();
		iVcount = cVringgossippool.count ();
		iVreturn = (cVringgossippool.bnsrc (sToken) + iVcount - 1) % iVcount;
		cVmutex.relse ();
		return iVreturn;
	}

	/*GET Ring Gossip loc*/
	CoreEnpo::GOSSIPLOC* __fastcall CoreEnpo::getrg (int iPosition) {
		GOSSIPLOC* cVreturn;

		cVreturn = NULL;
		cVmutex.locks ();
		if (iPosition < cVringgossippool.count ()) 
			cVreturn = cVringgossippool.get__ (iPosition)->cVgossiploc;
		cVmutex.relse ();
		return cVreturn;
	}

	/*SET Ring Gossip loc*/
	void __fastcall CoreEnpo::setrg (AnsiString sToken, GOSSIPLOC* cGossiploc) {

		cVmutex.locks ();
		if (cGossiploc->sVindex == cVnodedata.sVnodeid) cVringgossippool.bnuns (cVnodedata.sVtoken);
		if (cVringgossippool.bnget (sToken) == NULL) cVringgossippool.bnset (sToken)->cVgossiploc = cGossiploc;
		cVmutex.relse ();
	}

	/*UnSeT Ring Gossip loc*/
	void __fastcall CoreEnpo::ustrg (AnsiString sToken) {

		cVmutex.locks ();
		if (cVringgossippool.bnget (sToken) != NULL) cVringgossippool.bnuns (sToken);
		cVmutex.relse ();
	}

	/*GET Random neightbour*/
	bool __fastcall CoreEnpo::getrn (CoreEnpo::GOSSIPLOC& cGossiploc) {
		int iVneightbour;
		CoreEnpo::RINGGOSSIPLOC* cVringgossiploc;

		cVmutex.locks ();
		if (cVringgossippool.count () == 1) {
			if (cVnodedata.iVthriftgossiperport == 0) {
				cVmutex.relse ();
				return false;
			}
			cGossiploc.sVindex = NULL;
			cGossiploc.sVaddress = cVnodedata.sVthriftgossiperaddress;
			cGossiploc.iVport = cVnodedata.iVthriftgossiperport;	
		} else {
			do {
				iVneightbour = int (((double) rand() / RAND_MAX)* (cVringgossippool.count ()));
				cVringgossiploc = cVringgossippool.get__ (iVneightbour);
			} while (cVringgossiploc->cVgossiploc->sVindex == cVnodedata.sVnodeid);
			cGossiploc = *cVringgossiploc->cVgossiploc;
		}
		cVmutex.relse ();
		return true;
	}

	/*GET Node Data*/
	CoreEnpo::NODEDATA* __fastcall CoreEnpo::getnd () {

		return &cVnodedata;
	}

	/*GET Read Consistency level*/
	int __fastcall CoreEnpo::getrc () {

		return cVnodedata.iVreadconsistencylevel;
	}

	/*GET Read Repair*/
	bool __fastcall CoreEnpo::getrr () {

		return bVreadrepair;
	}

	/*GET Replication Factor*/
	int __fastcall CoreEnpo::getrf () {

		return cVnodedata.iVreplicationfactor;
	}

	/*GET Node Id*/
	AnsiString __fastcall CoreEnpo::getni () {

		return cVnodedata.sVnodeid;
	}

	/*GET ToKen*/
	AnsiString __fastcall CoreEnpo::gettk () {

		return cVnodedata.sVtoken;
	}

	/*SET ToKen*/
	void __fastcall CoreEnpo::settk (AnsiString sNewtoken) {
		AnsiString sVoldtoken;
		CoreEnpo::RINGGOSSIPLOC* cVringgossiploc;
		CoreEnpo::GOSSIPLOC* cVgossiploc;

		cVmutex.locks ();
		sVoldtoken = cVnodedata.sVtoken;
		cVringgossiploc = cVringgossippool.bnget (sVoldtoken);
		if (cVringgossiploc != NULL) {
			cVgossiploc = cVringgossiploc->cVgossiploc;
			cVringgossippool.bnuns (sVoldtoken);
			cVringgossippool.bnset (sNewtoken)->cVgossiploc = cVgossiploc;
		}
		cVnodedata.sVtoken = sNewtoken;
		cVmutex.relse ();
	}

	/*RiNG ToKen slow version*/ 
	AnsiString __fastcall CoreEnpo::rngtk (AnsiString sNodeid) {
		CoreEnpo::GOSSIPLOC* cVgossiploc;
		AnsiString sVreturn;

		sVreturn = NULL;
		cVmutex.locks ();
		for (int iV = 0; iV < cVringgossippool.count (); iV++) {
			cVgossiploc = cVringgossippool.get__ (iV)->cVgossiploc;
			if (cVgossiploc->sVindex == sNodeid) {
				sVreturn = cVgossiploc->sVtoken;
				break;
			}
		}
		cVmutex.relse ();
		return sVreturn;
	}

	/*GET Connection Factory*/
	PTR_CONNECTIONFACTORY __fastcall CoreEnpo::getcf (GOSSIPLOC* cVgossiploc) {
		PTR_CONNECTIONFACTORY cVreturn;

		cVreturn = cVgossiploc->cVconnectionfactory;
		if (!cVreturn->check (cVgossiploc->sVaddress, cVgossiploc->iVport)) 
			cVreturn->init_ (cVgossiploc->sVaddress, cVgossiploc->iVport);
		return cVreturn;
	}

	/*DEBUG*/
	AnsiString __fastcall CoreEnpo::debug () {
		AnsiString sVreturn = "";
		CoreEnpo::RINGGOSSIPLOC* cVringgossiploc;

		for (int iV = 0; iV < cVringgossippool.count (); iV++) {
			cVringgossiploc = cVringgossippool.get__ (iV);
			sVreturn +=		(AnsiString) iV + (AnsiString) ": " +
										cVringgossiploc->sVindex + 
										(AnsiString) " = " + cVringgossiploc->cVgossiploc->sVtoken +  
										(AnsiString) " = " + cVringgossiploc->cVgossiploc->sVdatacenterid + 
										(AnsiString) ":" + cVringgossiploc->cVgossiploc->sVindex + 
										(AnsiString) "-" + cVringgossiploc->cVgossiploc->sVaddress + 
										(AnsiString) ":" + cVringgossiploc->cVgossiploc->iVport + 
										bCcarriagereturn;
		}
		return sVreturn;
	}