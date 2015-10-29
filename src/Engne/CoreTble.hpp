#ifndef CORETBLE
#define CORETBLE
#include "CoreVcl_.hpp"
#include "PrtoAlog.pb.h"

namespace orion {

using namespace com::tomting::orion; 

class CoreTble {

public:

  enum iCquerytype {
		iCexactquery = 0,
		iCrangequery = 1,
		iCtokenquery = 2,
		iCtkslwquery = 4,
		iCnrngequery = 5
  };

	typedef boost::shared_ptr<CoreTble> PTR;

	virtual bool __fastcall get__ (AnsiString& sKey, PrtoLval* cValue) = 0;
	virtual AnsiString __fastcall getid () = 0;

};

};

#endif