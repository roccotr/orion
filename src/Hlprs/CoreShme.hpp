#ifndef CORESHME
#define CORESHME
#pragma warning(disable:4018)	
#include <boost/interprocess/mapped_region.hpp>
#pragma warning(default:4018)	
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"


using namespace boost::interprocess;
namespace orion{

class CoreShme {
	typedef boost::shared_ptr<void> PTR_VOID;

	void* cVsharedmemory;
	boost::shared_ptr<mapped_region> cVregion;
	int64vcl iVsize;
	bool bVrunning;

public:
  __fastcall CoreShme	(bool bServer, AnsiString sName, int64vcl iSize = 1024);
  __fastcall ~CoreShme();
	void* __fastcall addrs ();
	int64vcl __fastcall getsz ();
	bool __fastcall alive ();

};

typedef boost::shared_ptr<CoreShme> PTR_SHAREDMEMORY;

};

#endif