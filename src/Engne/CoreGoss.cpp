#include <math.h>
#include "CoreGoss.hpp"
#include "CorePart.hpp"
#include "CoreBint.hpp"
#include "CoreAutl.hpp"
#include "TmplList.cpp"

using namespace orion;

//#define iCdebuggossiper

	const AnsiString CoreGoss::sCobjectmanager = "ObjectManager";
	const AnsiString CoreGoss::sCgossiperkey = "GOSSIPER";
	const AnsiString CoreGoss::sCmaxmd5bound =  "ffffffffffffffffffffffffffffffff";
	const double CoreGoss::dCalivethreshold = 0.1;
	const double CoreGoss::dCmovingaveragedimphi = 3;
	const double CoreGoss::dCmovingaveragedimtmean = 5;
	const double CoreGoss::dCphifactor = 1.0 / log (10.0);
	const int CoreGoss::iCwgoldms = 1;

  __fastcall CoreGoss::CoreGoss () {
		
		srand (unsigned (double (__Time()) * 10000000));	
		iVcurrentwriter = iVcurrentreader = 0;
	}

  __fastcall CoreGoss::~CoreGoss () {

		cVsedamanager.reset ();;
		cVendpoint.reset ();
		cVredolog.reset ();
	}

	/*INIT*/
	void __fastcall CoreGoss::init_ (PTR_ENDPOINT cEndpoint, PTR_REDOLOG cRedolog) {

		cVendpoint = cEndpoint;
		cVredolog = cRedolog;
	}

	/*INIT*/
	void __fastcall CoreGoss::init_ (PTR_SEDAMANAGER cSedamanager) {

		cVsedamanager = cSedamanager;
	}

	/*TO prtoGOSs*/
	void __fastcall CoreGoss::togos (PrtoGoss& cGossiptoserialize) {
		PrtoGoel* cVgossipelement;
		CoreEnpo::GOSSIPLOC* cVgossiploc;

		for (int iV = 0; iV < cVgossippool.count (); iV++) {
			cVgossiploc = cVgossippool.get__ (iV);
			cVgossipelement = cGossiptoserialize.add_cvgossipelement ();
			cVgossipelement->set_svtoken (cVgossiploc->sVtoken.to_string ());
			cVgossipelement->set_svnodeid (cVgossiploc->sVindex.to_string ());
			cVgossipelement->set_svdatacenterid (cVgossiploc->sVdatacenterid.to_string ());
			cVgossipelement->set_svaddress (cVgossiploc->sVaddress.to_string ());
			cVgossipelement->set_ivport (cVgossiploc->iVport);
			cVgossipelement->set_ivstate ((iCstategossipnode) cVgossiploc->iVstate);
			cVgossipelement->set_ivtimestamp (cVgossiploc->iVtimestamp);
		}
	}

	/*GET Gossip Element*/
	bool __fastcall CoreGoss::getge (AnsiString sNodeid, CoreEnpo::GOSSIPLOC& cGossipelement, int iOffset) {
		CoreEnpo::GOSSIPLOC* cVgossipelement;
		AnsiString sVnodeid;
		int iVcursor;
		int iVcount;

		cVmutex.locks ();
		sVnodeid = sNodeid == NULL ? cVendpoint->getni () : sNodeid;
		iVcursor = cVgossippool.bnsrc (sVnodeid ) - 1;
		iVcount = cVgossippool.count ();
		if (iVcursor >= 0 && iVcursor < iVcount) {
			if (iOffset != 0) {
				iVcursor = (iVcursor + iOffset) % iVcount;
				while (iVcursor < 0) iVcursor += iVcount;
			}
			cVgossipelement = cVgossippool.get__ (iVcursor);
			cGossipelement = *cVgossipelement;
		}
		cVmutex.relse ();
		return cVgossipelement != NULL;
	}

	/*CHECK*/
	bool __fastcall CoreGoss::check (AnsiString sAddress, int iPort) {
		CoreEnpo::GOSSIPLOC* cVgossiploc;
		bool bVreturn;

		bVreturn = false;
		cVmutex.locks ();
		for (int iV = 0; iV < cVgossippool.count (); iV++) {
			cVgossiploc = cVgossippool.get__ (iV);
			if (	cVgossiploc->sVaddress == sAddress && cVgossiploc->iVport == iPort && 
						cVgossiploc->iVstate != CoreEnpo::iCstopped) bVreturn = true;
		}
		cVmutex.relse ();
		return bVreturn;
	}

	/*Write GOssiper to Local Db*/
	bool __fastcall CoreGoss::wgold () {
		PrtoLstm cVstatement;
		PrtoLmtb* cVmutable;
		PrtoLkey* cVkey;
		PrtoLval* cVvalue;
		PrtoGoss cVgossipertoserialize;

#ifdef iCdebuggossiper
		printf ("WGOLD\n");
#endif

		cVmutable = cVstatement.mutable_cvmutable ();
		cVmutable->set_svnamespace (CoreAlog::sCsystemnamespace.to_string ());
		cVmutable->set_svtable (CoreAlog::sCsystemlogictable.to_string ());
		cVkey = cVstatement.mutable_cvkey ();
		CoreAutl::zrkey (cVkey);
		cVkey->set_svmain (sCgossiperkey.to_string ());
		cVkey->set_ivstate (UPSERT);
		Sleep (iCwgoldms);
		cVkey->set_ivtimestamp (CoreButl::tmstl ());
		cVvalue = cVstatement.mutable_cvvalue ();
		togos (cVgossipertoserialize);
		cVvalue->set_svopaquevalue (cVgossipertoserialize.SerializeAsString ());
		cVvalue->set_ivtimestamp (cVkey->ivtimestamp ());
		cVstatement.set_ivopcode (MUTATOR);	
		return cVredolog->statm (&cVstatement, NULL);
	}

	/*Read GOssiper from Local Db*/
	bool __fastcall CoreGoss::rgold () {
		PrtoLmtb cVmutable;
		PrtoLkey cVkey;
		PrtoLval cVvalue;
		PrtoGoss cVgossipertoparse;
		PrtoGoel* cVgossipelement;

		cVmutable.set_svnamespace (CoreAlog::sCsystemnamespace.to_string ());
		cVmutable.set_svtable (CoreAlog::sCsystemlogictable.to_string ());
		cVkey.set_svmain (sCgossiperkey.to_string ());
		if (cVredolog->query (&cVmutable, &cVkey, &cVvalue, CoreTble::iCexactquery, NULL)) {
			cVgossipertoparse.ParseFromString (cVvalue.svopaquevalue ());
			for (int iV = 0; iV < cVgossipertoparse.cvgossipelement_size (); iV++) {
				cVgossipelement = cVgossipertoparse.mutable_cvgossipelement (iV);
				addgl (cVgossipelement, false, false, true);
			}
			return true;
		}
		return false;
	}

	/*ADD Gossip Loc*/
	CoreGoss::ADDGOSSIPLOCRETURN CoreGoss::addgl (PrtoGoel* cGossipelement, 
																								bool bLocal, bool bWritetodb, bool bForceringupdate) {
		ADDGOSSIPLOCRETURN cVreturn;
		AnsiString sVtoken;
		AnsiString sVnodeid;
		AnsiString sVresettoken;
		CoreEnpo::iCstategossipnode iVbackupastate;
		PrtoSeda cVmessage;

		cVreturn.bVissuerefreshstructures = false;;
		sVnodeid = cGossipelement->svnodeid ();
		sVresettoken = CorePart::gettk (sVnodeid);
		sVtoken = cGossipelement->svtoken () == "" ?  sVresettoken: cGossipelement->svtoken ();		
		cVmutex.locks ();
		cVreturn.cVgossiploc = cVgossippool.bnget (sVnodeid);
		if (cVreturn.cVgossiploc == NULL) {
			cVreturn.cVgossiploc = cVgossippool.bnset (sVnodeid);
			cVreturn.cVgossiploc->sVtoken = sVtoken;
			cVreturn.cVgossiploc->sVdatacenterid = cGossipelement->svdatacenterid ();
			cVreturn.cVgossiploc->dVphiaccrual = cGossipelement->dvphiaccrual ();
			cVreturn.cVgossiploc->bValive = cVreturn.cVgossiploc->dVphiaccrual > dCalivethreshold;
			cVreturn.cVgossiploc->sVaddress = cGossipelement->svaddress ();
			cVreturn.cVgossiploc->iVport = cGossipelement->ivport ();
			cVreturn.cVgossiploc->bVlocal = bLocal;
			if (!cVreturn.cVgossiploc->bVlocal) {
				cVreturn.cVgossiploc->cVconnectionfactory->init_ (cVreturn.cVgossiploc->sVaddress, cVreturn.cVgossiploc->iVport); 
				cVreturn.cVgossiploc->sVwriter = cVwriterpool.get__ (iVcurrentwriter)->sVindex;
				iVcurrentwriter = (iVcurrentwriter + 1) % cVwriterpool.count ();
				cVreturn.cVgossiploc->sVreader = cVreaderpool.get__ (iVcurrentreader)->sVindex;
				iVcurrentreader = (iVcurrentreader + 1) % cVreaderpool.count ();
				if (bWritetodb) wgold ();
			} else {
				cVreturn.cVgossiploc->bValive = true;
				cVthisgossiploc = cVreturn.cVgossiploc;
			}
		}	else {
			if (cVreturn.cVgossiploc->sVtoken == sVresettoken) cVreturn.cVgossiploc->sVtoken = sVtoken;
		}
		if (cGossipelement->ivtimestamp () > cVreturn.cVgossiploc->iVtimestamp || bForceringupdate) {
			iVbackupastate = cVreturn.cVgossiploc->iVstate;
			cVreturn.cVgossiploc->iVstate = (CoreEnpo::iCstategossipnode) cGossipelement->ivstate ();
			cVreturn.bVissuerefreshstructures = 
				cVreturn.cVgossiploc->iVstate == CoreEnpo::iCrunning && iVbackupastate != CoreEnpo::iCrunning;

			cVreturn.cVgossiploc->sVaddress = cGossipelement->svaddress ();
			cVreturn.cVgossiploc->iVport = cGossipelement->ivport ();	
			cVreturn.cVgossiploc->iVtimestamp = cGossipelement->ivtimestamp ();
			switch (cVreturn.cVgossiploc->iVstate) {
				case CoreEnpo::iCjoining:
				case CoreEnpo::iCloading:
					cVendpoint->setrg (sVtoken, cVreturn.cVgossiploc);
					break;
				case CoreEnpo::iCrunning:
					cVendpoint->setrg (sVtoken, cVreturn.cVgossiploc);
					if (cVsedamanager != NULL) cVsedamanager->pushs (sCobjectmanager, cVmessage);
					break;
				case CoreEnpo::iCleaving:
					cVendpoint->ustrg (sVtoken);
					break;
				case CoreEnpo::iCstopped:
					cVendpoint->ustrg (sVtoken);
					if (cVsedamanager != NULL) cVsedamanager->pushs (sCobjectmanager, cVmessage);
					break;
			}
			if (!cVreturn.cVgossiploc->bVlocal && bWritetodb) wgold ();
		}
		cVmutex.relse ();
		return cVreturn;
	}

	/*STATE*/
	void __fastcall CoreGoss::state (CoreEnpo::iCstategossipnode iState) {

		cVmutex.locks ();
		cVthisgossiploc->iVstate = iState;
		cVthisgossiploc->iVtimestamp = CoreButl::tmstl ();
		cVmutex.relse ();
	}

	/*CHANGE*/
	void __fastcall CoreGoss::chnge (AnsiString sAddress, int iPort) {

		cVmutex.locks ();
		cVthisgossiploc->sVaddress = sAddress;
		cVthisgossiploc->iVport = iPort;
		cVmutex.relse ();
	}

	/*GET Gossip Pool*/
	void __fastcall CoreGoss::getgp (PrtoGoel* cGossipresult) {

		cVmutex.locks ();
		cGossipresult->set_svtoken (cVthisgossiploc->sVtoken.to_string ());
		cGossipresult->set_svnodeid (cVthisgossiploc->sVindex.to_string ());
		cGossipresult->set_svdatacenterid (cVthisgossiploc->sVdatacenterid.to_string ());
		cGossipresult->set_dvphiaccrual (cVthisgossiploc->dVphiaccrual);
		cGossipresult->set_svaddress (cVthisgossiploc->sVaddress.to_string ());
		cGossipresult->set_ivport (cVthisgossiploc->iVport);
		cGossipresult->set_ivstate ((iCstategossipnode) cVthisgossiploc->iVstate);
		cGossipresult->set_ivtimestamp (cVthisgossiploc->iVtimestamp);
		cVmutex.relse ();
	}

	/*GET Gossip Pool*/
	void __fastcall CoreGoss::getgp (PrtoGoss* cGossipresult) {
		CoreEnpo::GOSSIPLOC* cVgossiplocref;
		PrtoGoel* cVgossipelement;

		cVmutex.locks ();
		cGossipresult->Clear ();
		for (int iV = 0; iV < cVgossippool.count (); iV++) {
			cVgossiplocref = cVgossippool.get__ (iV);
			cVgossipelement = cGossipresult->add_cvgossipelement ();
			cVgossipelement->set_svtoken (cVgossiplocref->sVtoken.to_string ());
			cVgossipelement->set_svnodeid (cVgossiplocref->sVindex.to_string ());
			cVgossipelement->set_svdatacenterid (cVgossiplocref->sVdatacenterid.to_string ());
			cVgossipelement->set_dvphiaccrual (cVgossiplocref->dVphiaccrual);
			cVgossipelement->set_svaddress (cVgossiplocref->sVaddress.to_string ());
			cVgossipelement->set_ivport (cVgossiplocref->iVport);
			cVgossipelement->set_ivstate ((iCstategossipnode) cVgossiplocref->iVstate);
			cVgossipelement->set_ivtimestamp (cVgossiplocref->iVtimestamp);
		}
		cVmutex.relse ();
	}

	/*GET Next node Token*/
	AnsiString __fastcall CoreGoss::getnt () {
		CoreBint cVa;
		CoreBint cVb;
		CoreBint cVf;
		CoreBint cVd;
		int iVthis;
		int iVnext;

		cVmutex.locks ();
		iVthis = cVendpoint->tk2po (cVendpoint->gettk ());
		iVnext = cVendpoint->next_ (iVthis);
		cVa = cVendpoint->gettk ();
		cVb = cVendpoint->getrg (iVnext)->sVtoken;
		if (iVnext > iVthis) {
			cVmutex.relse ();
			return ((cVa + cVb) / 2).write ();
		} else {
			cVf = (AnsiString) sCmaxmd5bound;
			cVd = cVa / 2 + cVb / 2 - cVf / 2;
			if (cVd.isovf ()) cVd = cVa / 2 + cVb / 2 + cVf / 2;
			cVmutex.relse ();
			return cVd.write ();
		}		
	}

	/*NEW ToKen*/
	void __fastcall CoreGoss::newtk (AnsiString sNewtoken) {
		PrtoGoel cVgossipelement;

		getgp (&cVgossipelement);
		cVgossipelement.set_svtoken (sNewtoken.to_string ());
		cVendpoint->settk (sNewtoken);
		addgl (&cVgossipelement, true);
		wgold ();
	}

	/*GOSSiP*/
	bool __fastcall CoreGoss::gossp (PrtoGoss* cGossip, PrtoGosr* cGossipresult, bool& bIssuerefreshstructures) {
		int iVgossipersize;
		PrtoGoel* cVgossipelement;
		CoreEnpo::GOSSIPLOC* cVgossiploc;
		CoreEnpo::NODEDATA* cVnodedata;
		ADDGOSSIPLOCRETURN cVaddgossiplocreturn;

		bIssuerefreshstructures = false;
		iVgossipersize = cGossip->cvgossipelement_size ();
		for (int iV = 0; iV < iVgossipersize; iV++) {
			cVgossipelement = cGossip->mutable_cvgossipelement (iV);
			cVaddgossiplocreturn = addgl (cVgossipelement);
			cVgossiploc = cVaddgossiplocreturn.cVgossiploc;
			bIssuerefreshstructures |= cVaddgossiplocreturn.bVissuerefreshstructures;
			updpa (cVgossiploc, cVgossipelement->dvphiaccrual (), 0);
		}			
		if (iVgossipersize == 0) {
			cVnodedata = cVendpoint->getnd ();
			cVgossipelement = cGossipresult->mutable_cvgossipelement ();
			cVgossipelement->set_svtoken (cVnodedata->sVtoken.to_string ());
			cVgossipelement->set_svnodeid (cVnodedata->sVnodeid.to_string ());
			cVgossipelement->set_svdatacenterid (cVnodedata->sVdatacenterid.to_string ());
			cVgossipelement->set_ivstate ((iCstategossipnode) cVthisgossiploc->iVstate);
			cVgossipelement->set_ivtimestamp (cVthisgossiploc->iVtimestamp);
			cVgossipelement->set_svaddress (cVnodedata->sVthriftaddress.to_string ());
			cVgossipelement->set_ivport (cVnodedata->iVthriftport);
		}
		return true;
	}

	/*GET Random Neightbour*/
	bool __fastcall CoreGoss::getrn (CoreEnpo::GOSSIPLOC& cGossiploc) {

		return cVendpoint->getrn (cGossiploc);
	}

	/*UPDate Phi Accrual*/
	void __fastcall CoreGoss::updpa (CoreEnpo::GOSSIPLOC* cGossiploc, double dValue, double dVelapsed) {
		double dVphiaccrual;
	
		if (dVelapsed > 0 && dValue > 0) {			
			cGossiploc->dVtmean = CoreButl::movav (cGossiploc->dVtmean, dVelapsed, dCmovingaveragedimtmean);
			dVphiaccrual = cGossiploc->dVtmean > 0 ? dCphifactor * dVelapsed / cGossiploc->dVtmean : 0;
		} else dVphiaccrual = dValue;
		cGossiploc->dVphiaccrual = Min (CoreButl::movav (cGossiploc->dVphiaccrual, dVphiaccrual, dCmovingaveragedimphi), 1);
		cGossiploc->bValive = cGossiploc->dVphiaccrual > dCalivethreshold;

#ifdef iCdebuggossiper
		printf ("%s = %s-%s: %f, tmean: %f (%f <-> %f)\n", 
			cGossiploc->sVindex.c_str (), 
			cGossiploc->sVdatacenterid.c_str (),
			cGossiploc->sVnodeid.c_str (),
			cGossiploc->dVphiaccrual, cGossiploc->dVtmean, dVphiaccrual, dValue);
#endif
	}

	/*SET Random Neightbour*/
	bool __fastcall CoreGoss::setrn (CoreEnpo::GOSSIPLOC& cGossiploc, double dValue, double dVelapsed) {
		CoreEnpo::GOSSIPLOC* cVgossiploc;

		cVmutex.locks ();		
		cVgossiploc = cVgossippool.bnget (cGossiploc.sVindex);
		if (cVgossiploc != NULL) {
			updpa (cVgossiploc, dValue, dVelapsed);
			cVmutex.relse ();
			return true;
		}
		cVmutex.relse ();
		return false;
	}

	/*SET Random Neightbour*/
	bool __fastcall CoreGoss::setrn (PrtoGoel* cGossipelement, double dValue, double dVelapsed) {
		CoreEnpo::GOSSIPLOC* cVgossiploc;

		addgl (cGossipelement);
		cVmutex.locks ();	
		cVgossiploc = cVgossippool.bnget (cGossipelement->svnodeid ());
		updpa (cVgossiploc, dValue, dVelapsed);
		cVmutex.relse ();
		return true;
	}

	/*ADD WRiter*/
	void __fastcall CoreGoss::addwr (AnsiString sWriter) {

		cVwriterpool.bnset (sWriter);
	}

	/*ADD ReaDer*/
	void __fastcall CoreGoss::addrd (AnsiString sReader) {

		cVreaderpool.bnset (sReader);
	}

	/*DEBUG*/
	AnsiString __fastcall CoreGoss::debug () {
		AnsiString sVreturn = "";
		CoreEnpo::GOSSIPLOC* cVgossiploc;

		for (int iV = 0; iV < cVgossippool.count (); iV++) {
			cVgossiploc = cVgossippool.get__ (iV);
			if (cVgossiploc->bVlocal) 
				sVreturn += (AnsiString) iV + (AnsiString) ": " +
										cVgossiploc->sVdatacenterid + (AnsiString) ":" + cVgossiploc->sVindex + 
										(AnsiString) " = " + cVgossiploc->sVtoken + 
										(AnsiString) ", " + (AnsiString) cVgossiploc->bValive + 
										(AnsiString) ", " + (AnsiString) cVgossiploc->bVlocal + 									
										(AnsiString) ", " + (AnsiString) cVgossiploc->iVstate + 	
										(AnsiString) " (" + (AnsiString) cVgossiploc->dVphiaccrual + (AnsiString) ")" +	
										(AnsiString) " (" + cVgossiploc->sVaddress + (AnsiString) ":" + (AnsiString) cVgossiploc->iVport + (AnsiString) ")" +	
										bCcarriagereturn;
			else
				sVreturn += (AnsiString) iV + (AnsiString) ": " +
										cVgossiploc->sVdatacenterid + (AnsiString) ":" + cVgossiploc->sVindex + 
										(AnsiString) " = " + cVgossiploc->sVtoken + 
										(AnsiString) ", " + (AnsiString) cVgossiploc->bValive + 
										(AnsiString) ", " + (AnsiString) cVgossiploc->bVlocal + 									
										(AnsiString) ", " + (AnsiString) cVgossiploc->iVstate + 	
										(AnsiString) " (" + cVgossiploc->sVwriter + (AnsiString) ")" +		
										(AnsiString) " (" + cVgossiploc->sVreader + (AnsiString) ")" +	
										(AnsiString) " (" + (AnsiString) cVgossiploc->dVphiaccrual + (AnsiString) ")" +	
										(AnsiString) " (" + cVgossiploc->sVaddress + (AnsiString) ":" + (AnsiString) cVgossiploc->iVport + (AnsiString) ")" +	
										bCcarriagereturn;
		}
		return sVreturn;
	}
