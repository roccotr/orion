#include "CoreTutl.hpp"
#include "CoreRmwr.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"

using namespace orion;

//#define iCdebugsendthrift

  __fastcall CoreRmwr::CoreRmwr() : CoreSdwk () {}

  __fastcall CoreRmwr::~CoreRmwr () {}

	/*INIT*/
	void __fastcall CoreRmwr::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}

	/*SEND Thrift*/
	bool CoreRmwr::sendt (void* cGossipelement, void* cMutations) {
		AnsiString sVcallback;
		std::vector<ThrfLstm>* cVmutations;
		PrtoSeda cVmessageresult;
		PrtoGoel* cVgossipelement;
		PTR_PEER cVpeer;
		PrtoLsts cVstatements;
		PTR_SEDAMANAGER cVsedamanager;
		PTR_GOSSIPER cVgossiper;
		CoreEnpo::GOSSIPLOC cVgossiploc;

		cVmutations = (std::vector<ThrfLstm>*) cMutations;
		cVgossipelement = (PrtoGoel*) cGossipelement;
		cVsedamanager = cVplanner->getsm ();
		cVgossiper = cVplanner->getpr ()->getgo ();
		

#ifdef iCdebugsendthrift
		printf ("WRITE %s (%s:%i) <- %i\n", 
			cVgossipelement->svnodeid ().c_str (), cVgossipelement->svaddress ().c_str (),
			cVgossipelement->ivport (), cVmutations->size ()); 
#endif

		if (!cVgossiper->getge (cVgossipelement->svnodeid (), cVgossiploc)) return false;		
		if (!CoreTutl::sends (cVgossiploc.cVconnectionfactory, cVmutations)) {
			cVpeer = cVplanner->getpr ();
			for (unsigned int iV = 0; iV < cVmutations->size (); iV++) 
				CoreTutl::cpstm (&(*cVmutations) [iV], cVstatements.add_cvliststatement ());
			cVpeer->puthh (cVgossipelement->svnodeid (), &cVstatements);
		} else {
			for (unsigned int iV = 0; iV < cVmutations->size (); iV++) {
				sVcallback = (*cVmutations) [iV].sVcallback;
				if (sVcallback != "") cVsedamanager->pshqu (sVcallback, cVmessageresult);	
			}
		}
		cVmutations->clear ();
		return true;
	}

  /*WORK*/
	void CoreRmwr::work_ () {
		bool bVlastendt;
		PrtoSeda cVmessage;
		PrtoGoel cVpreviousgossipelement;
		PrtoGoel* cVcurrentgossipelement;
		AnsiString sVpreviousnode;
		AnsiString sVcurrentnode;
		std::vector<ThrfLstm> cVmutations;
		ThrfLstm cVstatement;

		sVcurrentnode = sVpreviousnode = NULL;
		bVlastendt = false;
		while (cVmessagequeue->pop__ (cVmessage)) {
			bVlastendt = true;
			cVcurrentgossipelement = cVmessage.mutable_cvgossipelement ();
			sVcurrentnode = cVcurrentgossipelement->svnodeid ();
			if (sVpreviousnode != NULL && sVpreviousnode != sVcurrentnode) 
				sendt (&cVpreviousgossipelement, &cVmutations);
			if (sVpreviousnode != sVcurrentnode) cVpreviousgossipelement = *cVcurrentgossipelement;
			sVpreviousnode = sVcurrentnode;				
			CoreTutl::cpstm (cVmessage.mutable_cvstatement (), &cVstatement);
			cVstatement.sVcallback = cVmessage.svcallback ();
			cVmutations.push_back (cVstatement);
		}		
		if (bVlastendt) sendt (&cVpreviousgossipelement, &cVmutations);
	}