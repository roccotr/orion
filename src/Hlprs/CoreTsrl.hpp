#ifndef CORETSRL 
#define CORETSRL
#include <glog/logging.h>
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
using namespace ::apache::thrift::server;
using boost::shared_ptr;

class CoreTsrl {

	boost::shared_ptr<TMemoryBuffer> cVtransport;
	boost::shared_ptr<TProtocol> cVprotocol;

public:
  __fastcall CoreTsrl	();
  __fastcall ~CoreTsrl ();

	template<class T>
	int __fastcall srlze (T& cThrift, bool bHeader = true);

	template<class T>
	int __fastcall parse (T& cThrift);

	void tobuf (void* cDestination);
	void* tobuf ();

	void frbuf (void* cDestination);
	void* frbub (int iDim);
	void frbue (int iDim);
};

};


#endif