#include "CoreGowr.hpp"
#include "TmplList.cpp"
#include "TmplSdmq.cpp"
#include "CoreTutl.hpp"

using namespace orion;

//#define iCdebuggossiper

	const double CoreGowr::dVtimeconst = 24*3600;

  __fastcall CoreGowr::CoreGowr() : CoreSdwk () {
		
	}

  __fastcall CoreGowr::~CoreGowr () {

	}

	/*INIT*/
	void __fastcall CoreGowr::init (PTR_VOID cPlanner) {

		cVplanner = boost::static_pointer_cast<CorePlan> (cPlanner);
	}

  /*WORK*/
	void CoreGowr::work_ () {

		CoreEnpo::GOSSIPLOC cVgossiploc;
		PTR_PEER cVpeer;
		PTR_GOSSIPER cVgossiper;

		cVpeer = cVplanner->getpr ();
		cVgossiper = cVpeer->getgo ();
		if (cVgossiper->getrn (cVgossiploc)) {
#ifdef iCdebuggossiper
			printf ("-> %s:%i\n", cVgossiploc.sVaddress.c_str (), cVgossiploc.iVport);	
#endif
			gossr (cVgossiper, &cVgossiploc);		
		}
#ifdef iCdebuggossiper
		printf ("%s\n", cVpeer->getgo ()->debug ().c_str ());
#endif
	}

	/*GOSSip Routine*/
	bool CoreGowr::gossr (PTR_GOSSIPER cGossiper, CoreEnpo::GOSSIPLOC* cGossiploc) {
		ThrfGoss cVgossipresult;
		PrtoGoss cVprotogossip;
		ThrfGoss cVthriftgossip;
		ThrfGoel cVgossipresultelement;
		PrtoGoel cVprotogossipelement;
		ThrfGoel cVgossipvoidelement;
		TDateTime tVstart;
		TDateTime tVend;
		double dVelapsed;
		PTR_CONNECTIONFACTORY cVconnectionfactory;
		PTR_CONNECTION cVconnection;
		ThrfOrn_Client* cVclient;

		try {
			tVstart = __Time ();
			cVconnectionfactory = CoreEnpo::getcf (cGossiploc);
			cVconnection = cVconnectionfactory->begin ();
			if (!cVconnection->valid ()) return false;
			cVclient = cVconnection->get__ ();
			cVclient->gossp (cVgossipresult, cVthriftgossip, false, false);	
			tVend = __Time ();
			dVelapsed = (tVend - tVstart) * dVtimeconst;			
			if (cVgossipresult.cVgossipelement.size () == 0) throw EXCEPTION ();
			cVgossipresultelement = cVgossipresult.cVgossipelement [0];
			CoreTutl::cpgoe (&cVgossipresultelement, &cVprotogossipelement);
			cGossiper->setrn (&cVprotogossipelement, 1, dVelapsed);
			cGossiper->getgp (&cVprotogossip);
			CoreTutl::cpgos (&cVprotogossip, &cVthriftgossip);
			cVclient->gossp (cVgossipresult, cVthriftgossip, false, false);
			cVconnectionfactory->end__ (cVconnection);
			return true;
		} catch (...) {
			cVconnection->close ();
			cVconnectionfactory->end__ (cVconnection);
			if (cGossiploc->sVindex != NULL) cGossiper->setrn (*cGossiploc, 0, 0);
			return false;
		}
	}




