#ifndef CORERVRS
#define CORERVRS
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreButl.hpp"

namespace orion {

class CoreRvrs
{

public:
  __fastcall CoreRvrs ();
  __fastcall ~CoreRvrs ();

	int __fastcall asc_i (int iLocal);
	int __fastcall desci (int iLocal);
	double __fastcall asc_d (double dLocal);
	double __fastcall descd (double dLocal);
	int64vcl __fastcall asc_l (int64vcl iLocal);
	int64vcl __fastcall descl (int64vcl iLocal);
	int64vcl __fastcall rdesl (int64vcl iLocal);

};

typedef boost::shared_ptr<CoreRvrs> PTR_REVERSE;

};

#endif