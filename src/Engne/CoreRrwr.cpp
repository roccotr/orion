#include "CoreTutl.hpp"
#include "CoreRrwr.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"
#include "md5.h"

using namespace orion;

//#define iCdebugreadrepair

	const int CoreRrwr::iCtimeoutsedacallbacksms = 500;

  __fastcall CoreRrwr::CoreRrwr() : CoreSdwk () {}

  __fastcall CoreRrwr::~CoreRrwr () {}

	/*INIT*/
	void __fastcall CoreRrwr::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}


  /*WORK*/
	void CoreRrwr::work_ () {
		PrtoSeda cVmessage;
		PrtoLval cVvalue;
		PTR_SEDAMANAGER cVsedamanager;
		PTR_GOSSIPER cVgossiper;

		cVsedamanager = cVplanner->getsm ();
		cVgossiper = cVplanner->getpr ()->getgo ();
		while (cVmessagequeue->pop__ (cVmessage)) 
			rrepr (cVsedamanager, cVgossiper, &cVmessage, &cVvalue, iCtimeoutsedacallbacksms);			
	}

	PrtoQres* CoreRrwr::getrs (RepeatedPtrField<PrtoQres>* cQueryresult, bool& bFullmatch) {
		CoreList<QRESPOINTERLOC> cVqrespointerloc;
		PrtoQres* cVqres;
		int iVcount;
		CoreRvrs cVreverse;

		bFullmatch = false;
		for (int iV = 0; iV < cQueryresult->size (); iV++) { 
			cVqres = cQueryresult->Mutable (iV);
			if (cVqres->bvresult ())
				cVqrespointerloc.bnset (
					(AnsiString) cVqres->mutable_cvkey ()->svmain () + 
					CoreButl::i642s (cVreverse.descl (cVqres->mutable_cvvalue ()->ivtimestamp ())))->iVpointerloc = iV;
		}
		iVcount = cVqrespointerloc.count ();
		if (iVcount == 0) return NULL;
		bFullmatch = cVqrespointerloc.get__ (0)->sVindex == cVqrespointerloc.get__ (iVcount - 1)->sVindex;
		return cQueryresult->Mutable (cVqrespointerloc.get__ (0)->iVpointerloc);
	}

	/*GET ReSult*/
	bool CoreRrwr::getrs (RepeatedPtrField<PrtoQres>* cQueryresult, PrtoLkey* cKey, PrtoLval* cValue) {
		PrtoQres* cVqres;
		bool bVreturn;

		cVqres= getrs (cQueryresult, bVreturn);
		if (cVqres != NULL) {
			cValue->CopyFrom (cVqres->cvvalue ());
			cKey->CopyFrom (cVqres->cvkey ());
		} 
		return bVreturn;
	}

	/*Read REPaiR*/
	bool CoreRrwr::rrepr (	PTR_SEDAMANAGER cSedamanager, PTR_GOSSIPER cGossiper,
													PrtoSeda* cQueryresultandrepair, PrtoLval* cValue, int iTimeoutsedacallbacksms) {
		bool bVreturn;
		bool bVdummy;
		AnsiString sVcallback;
		AnsiString sVdigestmodel;
		AnsiString sVtempdigest;
		CoreList<QRESPOINTERLOC> cVqrespointerlocpool;
		CoreList<GOELPOINTERLOC> cVgoelpointerlocpool;
		PrtoGoel* cVgossipelement;
		PrtoQres* cVqueryresult;
		PrtoLstm* cVstatement;
		PrtoSeda cVmessageresult;
		ThrfLqry cVquery;
		CoreEnpo::GOSSIPLOC cVgossiploc;
		std::vector<ThrfLqry> cVqueries;
		std::vector<ThrfL1ks> cVqueryresults;
		int iVqueuedim;

		bVreturn = true;
		iVqueuedim = 0;
		sVcallback = cQueryresultandrepair->svcallback ();
		cSedamanager->newqu (sVcallback);
		cValue->Clear ();
		for (int iV = 0; iV < cQueryresultandrepair->cvlistqueryresult_size (); iV++) 
			cVqrespointerlocpool.bnset (cQueryresultandrepair->mutable_cvlistqueryresult (iV)->svnodeid ())->iVpointerloc = iV;
		for (int iV = 0; iV < cQueryresultandrepair->cvlistgossipelement_size (); iV++) {						
			cVgossipelement = cQueryresultandrepair->mutable_cvlistgossipelement (iV);
			cVgoelpointerlocpool.bnset (cVgossipelement->svnodeid ())->iVpointerloc = iV;
			if (cVqrespointerlocpool.bnget (cVgossipelement->svnodeid ()) == NULL) {
				PrtoSeda cVmessage;

				cVmessage.mutable_cvquery ()->CopyFrom (cQueryresultandrepair->cvquery ());
				cVmessage.mutable_cvquery ()->set_bvdigest (true);
				cVmessage.mutable_cvgossipelement ()->CopyFrom (*cVgossipelement);				
				cVmessage.set_svcallback (cQueryresultandrepair->svcallback ());
				cSedamanager->pushs (cVgossipelement->svreader (), cVmessage);	
				iVqueuedim++;
			}
		}
		do {
			if (cSedamanager->popqu (sVcallback, cVmessageresult, iTimeoutsedacallbacksms)) {
				cVqueryresult = cQueryresultandrepair->add_cvlistqueryresult ();
				cVqueryresult->CopyFrom (*cVmessageresult.mutable_cvlistqueryresult (0));	
				iVqueuedim--;
			} else break;
		} while (iVqueuedim > 0);
		cSedamanager->newqu (sVcallback);
		
		bVreturn = cQueryresultandrepair->cvlistqueryresult_size () > 0;
		if (bVreturn) {
			cVqueryresult = getrs (cQueryresultandrepair->mutable_cvlistqueryresult (), bVdummy); 
			if (cVqueryresult == 0) return false;
			if (cVqueryresult->bvdigest ()) {
				sVdigestmodel = cVqueryresult->mutable_cvvalue ()->svopaquevalue ();
				cVgossipelement = 
					cQueryresultandrepair->mutable_cvlistgossipelement (
					cVgoelpointerlocpool.bnget (cVqueryresult->svnodeid ())->iVpointerloc);

#ifdef iCdebugreadrepair
				printf ("READ  %s (%s:%i)\n", 
					cVgossipelement->svnodeid ().c_str (), cVgossipelement->svaddress ().c_str (),
					cVgossipelement->ivport ()); 
#endif

				CoreTutl::cpqry (cQueryresultandrepair->mutable_cvquery (), &cVquery);
				cVqueries.push_back (cVquery);
				if (!cGossiper->getge (cVgossipelement->svnodeid (), cVgossiploc)) return false;
				if (!CoreTutl::sendl (cVgossiploc.cVconnectionfactory , &cVqueries, cVqueryresults)) return false;
				CoreTutl::cpval (&cVqueryresults [0].cVvalue, cValue);					
			} else {
				cValue->CopyFrom (cVqueryresult->cvvalue ());
				sVdigestmodel = md5 (cVqueryresult->cvvalue ().svopaquevalue ());
			}
			for (int iV = 0; iV < cQueryresultandrepair->cvlistqueryresult_size (); iV++) {
				cVqueryresult = cQueryresultandrepair->mutable_cvlistqueryresult (iV);
				if (cVqueryresult->bvdigest ()) sVtempdigest = cVqueryresult->mutable_cvvalue ()->svopaquevalue ();
				else sVtempdigest = md5 (cVqueryresult->mutable_cvvalue ()->svopaquevalue ());
				if (sVtempdigest != sVdigestmodel) {
					PrtoSeda cVmessage;

					cVgossipelement = 
						cQueryresultandrepair->mutable_cvlistgossipelement (
						cVgoelpointerlocpool.bnget (cVqueryresult->svnodeid ())->iVpointerloc);

#ifdef iCdebugreadrepair
					printf ("WRITE %s (%s:%i)\n", 
						cVgossipelement->svnodeid ().c_str (), cVgossipelement->svaddress ().c_str (),
						cVgossipelement->ivport ()); 
#endif

					cVstatement = cVmessage.mutable_cvstatement ();
					cVstatement->mutable_cvmutable ()->CopyFrom (cQueryresultandrepair->mutable_cvquery ()->cvmutable ());
					cVstatement->mutable_cvkey ()->CopyFrom (cQueryresultandrepair->mutable_cvquery ()->cvkey ());
					cVstatement->mutable_cvvalue ()->CopyFrom (*cValue);
					cVmessage.mutable_cvgossipelement ()->CopyFrom (*cVgossipelement);	
					cSedamanager->pushs (cVgossipelement->svwriter (), cVmessage);
				}
			}			
		} 
		return bVreturn;
	}

