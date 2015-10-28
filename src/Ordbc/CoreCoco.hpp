#ifndef CORECOCO
#define CORECOCO
#include <glog/logging.h>
#include "ThrfOrn_.h"
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

#include "CoreVcl_.hpp"
#include "CorePool.hpp"
#include "CoreSckt.hpp"
#include "CoreFsys.hpp"
#include "CoreShst.hpp"

#include "PrtoAlog.pb.h"

using namespace orion;
using namespace com::tomting::orion; 

class CoreCoco {
	typedef boost::shared_ptr<CoreSckt> PTR_SOCKET;

	PTR_POOL cVpool;
	bool bVconnection;
	bool bVsharedmemory;
	PTR_FILESYSTEM cVfilesystem;
	PTR_SOCKET cVsocket;
	boost::shared_ptr<CoreShst<ThrfBsrc> > cVsharedsend;
	boost::shared_ptr<CoreShst<ThrfBsrr> > cVsharedreceive;

public:
  __fastcall CoreCoco	();
  __fastcall ~CoreCoco ();

	bool __fastcall open_ (PTR_POOL cPool, AnsiString sAddress, int iPort, AnsiString sProtocol);
	bool __fastcall clse_ ();
	void __fastcall run__ (ThrfBsrr* cReturn, ThrfBsrc* cService);
	bool __fastcall valid ();


	void __fastcall runpb (PrtoBsrr* cReturn, PrtoBsrc* cService);
};

typedef boost::shared_ptr<CoreCoco> PTR_ORIONCONNECTION;

#endif