#ifdef iCdebugvld
	//#include <vld.h> 

	
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "PrtoAlog.pb.h"
/*
#include "ThrfOrn_.h"
#pragma warning(disable:4244)	
#pragma warning(disable:4018)	
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h> 
#include <protocol/TBinaryProtocol.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using boost::shared_ptr;
*/

/*
#include "CoreConn.hpp" 

#ifndef iCdisabletest
	#include "UnitBase.hpp"
#endif

#include "CoreFsys.hpp"
#include "CoreSstb.hpp"
*/


#include "stdafx.h"

/*
#include "CoreTbsv.hpp"
#include "CoreSqll.hpp"
#include "CorePlan.hpp"
#include "CoreAlog.hpp"
#include "CoreOrn_.hpp"
#include "CoreButl.hpp"
#include "TmplList.cpp"
#include "TmplLiss.cpp"
#include "CorePart.hpp"
#include "CoreHiha.hpp"
#include "CoreBint.hpp"
#include "CoreAenp.hpp"
#include "CoreMktr.hpp"
#include "CoreTutl.hpp"
#include "CoreCofa.hpp"
#include "CoreLiss.hpp" 
#include "CoreSoix.hpp"
#include "CoreMeot.hpp"
#include "CoreOrsv.hpp"
#include "CoreCoco.hpp"
#include "CoreCocf.hpp"
#include "CoreCwrk.hpp"
#include "CoreCaco.hpp"
#include "CoreCacp.hpp"
#include "TmplTsrl.cpp"

#include "TmplSdmg.cpp"
#include "TmplObpl.cpp"
#include "TmplOo2t.cpp"
#include "TmplOo2w.cpp"
#include "TmplClps.cpp"
#include "TmplCche.cpp"
#include "TmplLstc.cpp"
#include "TmplLssc.cpp"
#include "TmplFsys.cpp"
#include "TmplShst.cpp"
#include "md5.h"
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "TmplFlqu.cpp"
*/




//CAZZO
int _tmain(int argc, _TCHAR* argv[]) {	
	

#ifdef _DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF; 
	_CrtSetDbgFlag(flag);
	//_CrtSetBreakAlloc(2580);
#endif



	/*
	AnsiString sVsubversion;
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff;

	tVdiff = 24*3600;
	if (sizeof (void*) == 8) sVsubversion = "x64";
	else sVsubversion = "x86";
	printf ("Stress %s\n", sVsubversion.c_str ());

	tVstart = __Time ();
	*/














	//LEAKS
	{

		/*
		FLAGS_log_dir = "c:/templog/templog";
		google::InitGoogleLogging(argv [0]);*/

		/*
		PTR_POOL cVpool (new CorePool);
		PTR_ORION cVorion (new CoreOrn_ (	cVpool, 
							//"C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\Release\\", 
							"C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\x64\\Release\\",
							//"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orioncluster\\node1\\",
							"Orion.xml",CoreEnpo::iCrunning, false));
		PTR_PLANNER cVplanner = cVorion->getpl ();	

		cVplanner->lnch_ (NULL, &CorePlan::touch);
		printf ("valore %i\n", cVplanner->getsz ());
		*/
	}
	







	/*
  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("Elapsed: %f\n",tVdiff);

#ifndef iCdebugvld
	while (true) {Sleep (10);}
#endif
	*/

	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}
