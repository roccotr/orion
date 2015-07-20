#ifndef COREAUTL
#define COREAUTL
#include "PrtoAlog.pb.h"
#include "PrtoSeda.pb.h"
#include "PrtoSstb.pb.h"
#include "PrtoSqll.pb.h"

namespace orion{

using namespace com::tomting::orion; 

class CoreAutl {

public:
	static void zrkey (PrtoLkey* cSource);
};

};

#endif