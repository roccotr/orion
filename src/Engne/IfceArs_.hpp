#ifndef IFCEARS_
#define IFCEARS_
#include "CoreOrn_.hpp"
#include "CoreTutl.hpp"
#include "CoreCacp.hpp"

namespace orion {

	class IfceArs_ {

		PTR_POOL cVpool;
		PTR_ORIONCONNECTIONFACTORY cVorionconnectionfactory;
		PTR_ARIESCONNECTION cVariesconnectionfactory;
		PTR_ARIESPIPELINE cVariespipeline;

		public:

			IfceArs_ (AnsiString sAddress, int iPort, int iWorkers, int iBulksize, bool& bValid);
			~IfceArs_ ();

			bool run__ (ThrfSrvc cVservice, ThrfSrvc& cVserviceresult, bool bOptimistic);

	};

	
}

#endif