#include "CoreTutl.hpp"
#include "CoreRmrd.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"

using namespace orion;

//#define iCdebugrecvthrift

  __fastcall CoreRmrd::CoreRmrd() : CoreSdwk () {}

  __fastcall CoreRmrd::~CoreRmrd () {}

	/*INIT*/
	void __fastcall CoreRmrd::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}

	/*RECeive Thrift*/
	bool CoreRmrd::recvt (void* cGossipelement, void* cQueries) {
		PrtoGoel* cVgossipelement;
		PrtoQres* cVqueryresult;
		std::vector<ThrfLqry>* cVqueries;
		std::vector<ThrfL1ks> cVresults;
		PTR_SEDAMANAGER cVsedamanager;
		PTR_GOSSIPER cVgossiper;
		CoreEnpo::GOSSIPLOC cVgossiploc;

		cVgossipelement = (PrtoGoel*) cGossipelement;
		cVqueries = (std::vector<ThrfLqry>*) cQueries;
		cVsedamanager = cVplanner->getsm ();
		cVgossiper = cVplanner->getpr ()->getgo ();

#ifdef iCdebugrecvthrift
		printf ("READ  %s (%s:%i) <- %i\n", 
			cVgossipelement->svnodeid ().c_str (), cVgossipelement->svaddress ().c_str (),
			cVgossipelement->ivport (), cVqueries->size ()); 
#endif

		if (!cVgossiper->getge (cVgossipelement->svnodeid (), cVgossiploc)) return false;
		if (CoreTutl::sendl (cVgossiploc.cVconnectionfactory, cVqueries, cVresults) &&
			cVresults.size () == cVqueries->size ()) {
			for (unsigned int iV = 0; iV < cVresults.size (); iV++) {
				PrtoSeda cVmessageresult;

				cVqueryresult = cVmessageresult.add_cvlistqueryresult ();
				cVqueryresult->set_svnodeid (cVgossipelement->svnodeid ());
				cVqueryresult->set_bvdigest ((*cVqueries) [iV].bVdigest);
				CoreTutl::cpres (&cVresults [iV], cVqueryresult);
				cVsedamanager->pshqu ((*cVqueries) [iV].sVcallback, cVmessageresult);	
			}
		} 
		cVqueries->clear ();
		return true;
	}

  /*WORK*/
	void CoreRmrd::work_ () {
		bool bVlastendt;
		PrtoSeda cVmessage;
		PrtoGoel cVpreviousgossipelement;
		PrtoGoel* cVcurrentgossipelement;
		AnsiString sVpreviousnode;
		AnsiString sVcurrentnode;
		std::vector<ThrfLqry> cVqueries;		
		ThrfLqry cVquery;

		sVcurrentnode = sVpreviousnode = NULL;
		bVlastendt = false;
		while (cVmessagequeue->pop__ (cVmessage)) {
			bVlastendt = true;
			cVcurrentgossipelement = cVmessage.mutable_cvgossipelement ();
			sVcurrentnode = cVcurrentgossipelement->svnodeid ();
			if (sVpreviousnode != NULL && sVpreviousnode != sVcurrentnode) 
				recvt (&cVpreviousgossipelement, &cVqueries);
			if (sVpreviousnode != sVcurrentnode) cVpreviousgossipelement = *cVcurrentgossipelement;
			sVpreviousnode = sVcurrentnode;							
			CoreTutl::cpqry (cVmessage.mutable_cvquery (), &cVquery);
			cVquery.sVcallback = cVmessage.svcallback ();
			cVqueries.push_back (cVquery);
		}		
		if (bVlastendt) recvt (&cVpreviousgossipelement, &cVqueries);
	}
