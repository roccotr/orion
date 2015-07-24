#ifndef CORETHSV
#define CORETHSV
#include <glog/logging.h>
#include "ThrfOrn_.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "CoreVcl_.hpp"
#include "CoreOrn_.hpp"




namespace orion {

	using namespace ::apache::thrift::server;

class CoreThsv : virtual public ThrfOrn_If {

	const static int iCwaitshutdownms;

	bool bVstarted;
	bool bVinprogress;
	bool bVsuspendcriticalregion;
	bool bVoffline;
	l64 iVcriticalregion;
	PTR_POOL cVpool;
	PTR_ORION cVorion;
	PTR_PLANNER cVplanner;
	CoreSmph cVshutdownstartupmutex;
	TNonblockingServer* cVserver;

	bool __fastcall strtp (CoreEnpo::iCstategossipnode iState = CoreEnpo::iCrunning, bool bResetlocal = false);
	bool __fastcall shtdw (bool bAbort = false);

	bool __fastcall entcr ();
	void __fastcall lvecr ();

public:
  __fastcall CoreThsv();
  __fastcall ~CoreThsv ();

	bool ping();
  bool create(const ThrfL2ct& cCreateTable);
	bool statement(const ThrfL2st& cStatement);
	void query(ThrfL2qb& bReturn, const ThrfL2qr& cQuery);
	void osql(std::vector<ThrfL2ks>& _return, const ThrfL2os& cQuery);
	void gossp (ThrfGoss& _return, const ThrfGoss& cGossip, const bool bAll, const bool bDetails);
  void command(ThrfCort& _return, const ThrfComm& cCommand);
	bool localstatement (const std::vector<ThrfLstm>& cMutations);
  void localquery (std::vector<ThrfL1ks> & _return, const std::vector<ThrfLqry> & cQueries);
	void protobatch (std::vector<std::string> & _return,const std::vector<std::string> & sOpaque);
	void thriftbatch (ThrfSrvr* _return, const ThrfSrvc* cService);
	void thriftbatch (std::vector<ThrfSrvr> & _return,const std::vector<ThrfSrvc> & cService);
	void protobatch (PrtoSrvr* _return, PrtoSrvc* cService, CorePlan::PTR_TABLELOC* cTablelocpointer = NULL) ;
	void protobatch (RepeatedPtrField<PrtoSrvr>*_return, RepeatedPtrField<PrtoSrvc>* cRepeatedservice) ;

	void getnd (CoreEnpo::NODEDATA& cNodedata);
	void seteb (TNonblockingServer* cServer);
	bool alive ();

};};

#endif