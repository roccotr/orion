#include "CoreAutl.hpp"

using namespace orion;

  /*ZeRo KEY*/
	void CoreAutl::zrkey (PrtoLkey* cSource) {

		cSource->set_svmain ("");
		cSource->set_svaccessgroup ("");
		cSource->set_svqualifier ("");
		cSource->set_ivstate (UPSERT);
		cSource->set_ivtimestamp (0);
	}