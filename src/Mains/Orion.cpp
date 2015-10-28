#include "CoreThsv.hpp"
#include "CoreHutl.hpp"
#include "CoreOrsv.hpp"

#ifdef VSTUDIO
	#include <SDKDDKVer.h>
	#include <stdio.h>
	#include <tchar.h>
#else
	#define _TCHAR char
#endif

#include <server/TNonblockingServer.h>
#include <server/TThreadedServer.h>
#include <server/TThreadPoolServer.h>
#include <concurrency/PlatformThreadFactory.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::concurrency;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using boost::shared_ptr;
using namespace orion;

#include <glog/logging.h>
#include <gflags/gflags.h>

  DEFINE_string(log_folder, "", "Logging folder");
  DEFINE_string(make, "", "Make file");
  DEFINE_string(chkdsk, "", "Input file");
  DEFINE_string(output, "", "Output file");
  DEFINE_string(work_folder, "", "Work folder");

static void dummyo (const char* message) {

#ifdef iCdebugthrift
	printf ("%s\n", message);
#endif

};


class CoreTths : TThread {
	TThreadedServer* cVtthreadedserver;

public:
  __fastcall CoreTths (TThreadedServer* cTthreadedserver) : TThread (true) {
		cVtthreadedserver = cTthreadedserver;
	}

  __fastcall ~CoreTths () {}

	void Execute () {

		cVtthreadedserver->serve ();
	}

	void start () {
		Resume ();
	}

};


int _tmain(int argc, _TCHAR* argv[]) {	
	CoreEnpo::NODEDATA cVnodedata;
	CoreThsv* cVthriftserver;
	CoreTths* cVthriftthreadedmanager;
	CoreOrsv* cVordbcserver;
	AnsiString sVpath;
	AnsiString sVdummy;

#ifdef VSTUDIO
	WSADATA cVwsa_data;
	int iVresult = WSAStartup(MAKEWORD(2, 2), &cVwsa_data);
#else
	int iVresult = 0;
#endif

	google::ParseCommandLineFlags(&argc, &argv, true);	
	if (FLAGS_log_folder == "") {
		CoreButl::splpt (CoreButl::left_ (CoreHutl::appnm (GetModuleHandle (NULL)), "."), sVpath, sVdummy);
		FLAGS_log_dir = sVpath.to_string ();
	} else FLAGS_log_dir = FLAGS_log_folder;
#ifdef sClogpath
	FLAGS_log_dir = sClogpath;
#endif
	printf ("Logging: %s\n",  FLAGS_log_dir.c_str ());
	google::InitGoogleLogging(argv[0]);

	if (FLAGS_make != "")  {
		if (CoreSstb::make_ (FLAGS_work_folder, FLAGS_make, FLAGS_output)) return 0;
		else return -1;
	}
	if (FLAGS_chkdsk != "")  {
		if (CoreSstb::chkdk (FLAGS_work_folder, FLAGS_chkdsk)) return 0;
		else return -1;
	}

	GlobalOutput.setOutputFunction (&dummyo);

	if (iVresult == 0) {
		cVthriftserver = new CoreThsv ();
		cVthriftserver->getnd (cVnodedata);
		boost::shared_ptr<CoreThsv> cVhandler(cVthriftserver);
		boost::shared_ptr<TProcessor> cVprocessor(new ThrfOrn_Processor(cVhandler));
		boost::shared_ptr<TProtocolFactory> cVprotocolfactory(new TBinaryProtocolFactory());

		boost::shared_ptr<ThreadManager> cVthreadmanager = ThreadManager::newSimpleThreadManager (cVnodedata.iVthriftlisteners);
		boost::shared_ptr<PlatformThreadFactory> cVthreadfactory = boost::shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory());
		cVthreadmanager->threadFactory (cVthreadfactory);
		cVthreadmanager->start ();
		printf ("NIO on localhost:%i, listeners = %i\n", cVnodedata.iVthriftport, cVnodedata.iVthriftlisteners);

		boost::shared_ptr<TServerTransport> cVservertransport (new TServerSocket(cVnodedata.iVthriftthreadedport));
		boost::shared_ptr<TTransportFactory> cVtransportfactory(new TBufferedTransportFactory());
		TThreadedServer cVthreadedserver (cVprocessor, cVservertransport, cVtransportfactory, cVprotocolfactory);
		if (cVnodedata.iVthriftthreadedport > 0) {
			printf ("I/O on localhost:%i\n", cVnodedata.iVthriftthreadedport);
			cVthriftthreadedmanager = new CoreTths (&cVthreadedserver);
			cVthriftthreadedmanager->start ();
		} else cVthriftthreadedmanager = NULL;
		if (cVnodedata.iVordbcport > 0) {
			printf ("DBC on %s:%i\n", cVnodedata.sVordbcaddress.c_str (), cVnodedata.iVordbcport);
			cVordbcserver = new CoreOrsv (cVthriftserver, cVnodedata.sVordbcaddress, cVnodedata.iVordbcport, 
																		cVnodedata.iVthriftlisteners, cVnodedata.sVnodeid);
		}

		do {
			TNonblockingServer cVserver (cVprocessor, cVprotocolfactory, cVnodedata.iVthriftport, cVthreadmanager);
			cVthriftserver->seteb (&cVserver);
			cVserver.serve();
			Sleep (100);
		} while (cVthriftserver->alive ());

		delete cVthriftserver;
		if (cVthriftthreadedmanager != NULL) delete cVthriftthreadedmanager;
		return 0;
	}
}
