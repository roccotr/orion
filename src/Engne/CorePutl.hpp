#ifndef COREPUTL
#define COREPUTL
#include "CoreCofa.hpp"
#include "PrtoAlog.pb.h"

namespace orion {
	using namespace com::tomting::orion;

class CorePutl {

public:
	static bool sends (PTR_CONNECTIONFACTORY cConnectionfactory, PrtoLsts* cStatements);
	static bool pull_ (PTR_CONNECTIONFACTORY cConnectionfactory, std::string sThisnodeid);
	static bool frze_ (PTR_CONNECTIONFACTORY cConnectionfactory);
	static bool rbpar (PTR_CONNECTIONFACTORY cConnectionfactory);
	static bool purge (PTR_CONNECTIONFACTORY cConnectionfactory);
	static int64vcl size_ (PTR_CONNECTIONFACTORY cConnectionfactory);
};

};


#endif