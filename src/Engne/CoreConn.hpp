#ifndef CORECONN
#define CORECONN
#include <glog/logging.h>
#include "ThrfOrn_.h"
#pragma warning(disable:4244)	
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"

namespace orion {

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using boost::shared_ptr;



class CoreConn {

	bool bVconnection;
  boost::shared_ptr<TSocket> cVsocket;
  boost::shared_ptr<TTransport> cVtransport;
  boost::shared_ptr<TProtocol> cVprotocol;
	ThrfOrn_Client* cVclient;

public:
  __fastcall CoreConn();
  __fastcall ~CoreConn ();

	bool __fastcall open_ (AnsiString sAddress, int iPort);
	bool __fastcall close ();
	ThrfOrn_Client* __fastcall get__ ();

	bool __fastcall valid ();
};

typedef boost::shared_ptr<CoreConn> PTR_CONNECTION;

};

#endif
