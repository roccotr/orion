#include "CorePutl.hpp"
#include "CoreTutl.hpp"

using namespace orion;

	/*SEND Statements*/
	bool CorePutl::sends (PTR_CONNECTIONFACTORY cConnectionfactory, PrtoLsts* cStatements) {
		std::vector<ThrfLstm> cVmutations;
		ThrfLstm cVstatement;

		for (int iV = 0; iV < cStatements->cvliststatement_size (); iV++) {
			CoreTutl::cpstm (&cStatements->cvliststatement (iV), &cVstatement);		
			cVmutations.push_back (cVstatement);					
		}
		return CoreTutl::sends (cConnectionfactory, &cVmutations);
	}

	/*PULL*/
	bool CorePutl::pull_ (PTR_CONNECTIONFACTORY cConnectionfactory, std::string sThisnodeid) {
		ThrfComm cVcommand;
		ThrfCort cVcommandresult;

		cVcommand.iVcommand = iEreservedkeyword::ANTIENTROPHY;
		cVcommand.iVsubcommand = iEreservedkeyword::PULLALL;
		cVcommand.sVnodeid = sThisnodeid;
		CoreTutl::sendc (cConnectionfactory, cVcommand, cVcommandresult);		
		return cVcommandresult.bVreturn;
	}

	/*FReeZE*/
	bool CorePutl::frze_ (PTR_CONNECTIONFACTORY cConnectionfactory) {
		ThrfComm cVcommand;
		ThrfCort cVcommandresult;

		cVcommand.iVcommand = iEreservedkeyword::TABLE;
		cVcommand.iVsubcommand = iEreservedkeyword::FREEZE;
		CoreTutl::sendc (cConnectionfactory, cVcommand, cVcommandresult);		
		return cVcommandresult.bVreturn;
	}

	/*ReBuild PARtition*/
	bool CorePutl::rbpar (PTR_CONNECTIONFACTORY cConnectionfactory) {
		ThrfComm cVcommand;
		ThrfCort cVcommandresult;

		cVcommand.iVcommand = iEreservedkeyword::TABLE;
		cVcommand.iVsubcommand = iEreservedkeyword::REBUILDPART;
		CoreTutl::sendc (cConnectionfactory, cVcommand, cVcommandresult);		
		return cVcommandresult.bVreturn;
	}

	/*PURGE*/
	bool CorePutl::purge (PTR_CONNECTIONFACTORY cConnectionfactory) {
		ThrfComm cVcommand;
		ThrfCort cVcommandresult;

		cVcommand.iVcommand = iEreservedkeyword::TABLE;
		cVcommand.iVsubcommand = iEreservedkeyword::PURGE;
		CoreTutl::sendc (cConnectionfactory, cVcommand, cVcommandresult);		
		return cVcommandresult.bVreturn;
	}

	/*working SIZE*/
	int64vcl CorePutl::size_ (PTR_CONNECTIONFACTORY cConnectionfactory) {
		ThrfComm cVcommand;
		ThrfCort cVcommandresult;

		cVcommand.iVcommand = iEreservedkeyword::SIZE;
		if (CoreTutl::sendc (cConnectionfactory, cVcommand, cVcommandresult)) return cVcommandresult.cVreturnstats.iVsize;	
		else return 0;
	}
