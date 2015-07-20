#ifndef CORETUTL
#define CORETUTL
#include <glog/logging.h>
#include "ThrfOrn_.h"
#include "PrtoAlog.pb.h"
#include "PrtoSeda.pb.h"
#include "CoreCofa.hpp"

using namespace com::tomting::orion;
namespace orion {


class CoreTutl {

public:
	static void cpkey (const ThrfLkey* cSource, PrtoLkey* cDestination);
	static void cpkey (const PrtoLkey* cSource, ThrfLkey* cDestination);
	static void cpmut (const ThrfLmtb* cSource, PrtoLmtb* cDestination);
	static void cpmut (const PrtoLmtb* cSource, ThrfLmtb* cDestination);
	static void cp2cv (	iEcolumntype::type iColumntype, const ThrfL2cv* cSource, PrtoL2cv* cDestination);
	static void cp2cv (	iCcolumntype iColumntype, const PrtoL2cv* cSource, ThrfL2cv* cDestination);
	static void cpval (const ThrfLval* cSource, PrtoLval* cDestination);	
	static void cpval (const PrtoLval* cSource, ThrfLval* cDestination);
	static void cpres (const ThrfL1ks* cSource, PrtoQres* cDestination);
	static void cpstm (const ThrfLstm* cSource, PrtoLstm* cDestination);
	static void cpstm (const PrtoLstm* cSource, ThrfLstm* cDestination);
	static void cpqry (const ThrfLqry* cSource, PrtoLqry* cDestination);
	static void cpqry (const PrtoLqry* cSource, ThrfLqry* cDestination);
	static void cpqry (PrtoL2qr* cSource, ThrfL2qr* cDestination);
	static void cpgoe (const ThrfGoel* cSource, PrtoGoel* cDestination);
	static void cpgoe (const PrtoGoel* cSource, ThrfGoel* cDestination);
	static void cpgos (PrtoGoss* cSource, ThrfGoss* cDestination);
	static void cptbe (const PrtoTlel* cSource, ThrfTlel* cDestination);
	static void cptbl (PrtoTabl* cSource, ThrfTabl* cDestination);
	static void cpsta (const PrtoStel* cSource, ThrfStel* cDestination);
	static void cpaen (const ThrfAenp* cSource, PrtoAenp* cDestination);
	static void cpaen (const PrtoAenp* cSource, ThrfAenp* cDestination);
	static void cptop (const PrtoTop_* cSource, ThrfTop_* cDestination);
	static void wrtrs (PrtoL2mr* cDmlresult, std::vector<ThrfL2ks>& _return);
	static void wrtrs (const std::vector<ThrfL2ks>* cSource, PrtoL2mr* cDestination);
	static bool sends (PTR_CONNECTIONFACTORY cConnectionfactory, std::vector<ThrfLstm>* cStatements);
	static bool sendl (PTR_CONNECTIONFACTORY cConnectionfactory, std::vector<ThrfLqry>* cQueries, std::vector<ThrfL1ks>& cResults);
	static bool sendq (PTR_CONNECTIONFACTORY cConnectionfactory, PrtoL2qr* cQuery, bool bRecursiveteleportdisabled, PrtoL2mr* cResult);
	static bool sendc (PTR_CONNECTIONFACTORY cConnectionfactory, const ThrfComm& cCommand, ThrfCort& cResult);
	static bool sendc (std::string sAddress, int iPort, const ThrfComm& cCommand, ThrfCort& cResult);
	static void statm (const ThrfL2st& cSource, PrtoL2st& cDestination);
	static void query (const ThrfL2qr& cSource, PrtoL2qr& cDestination);
	static void osql_ (const ThrfL2os& cSource, PrtoL2os& cDestination); 

	static AnsiString debug (const PrtoLmtb& cMutable);
	static AnsiString debug (const PrtoLkey& cKey);
	static AnsiString debug (const PrtoL2qr& cDestination);

};

};

#endif