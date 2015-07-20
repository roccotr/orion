#include "CoreTsrl.hpp"

using namespace orion;


  __fastcall CoreTsrl::CoreTsrl () {

		cVtransport = boost::shared_ptr<TMemoryBuffer> (new TMemoryBuffer ());
		cVprotocol = boost::shared_ptr<TProtocol> (new TBinaryProtocol(cVtransport));
	}

  __fastcall CoreTsrl::~CoreTsrl () {
	}

	/*TO BUFfer*/
	void CoreTsrl::tobuf (void* cDestination) {
		void* vVbuffer;
		uint32_t iVsize;
		int* iVheader;
		
		cVtransport.get ()->getBuffer ((uint8_t**) &vVbuffer, &iVsize);
		iVheader = (int*) cDestination;
		*iVheader = iVsize;
		memcpy ((void*) &iVheader [1], vVbuffer, iVsize);
	}

	/*TO BUFfer*/
	void* CoreTsrl::tobuf () {
		void* vVbuffer;
		uint32_t iVsize;

		cVtransport.get ()->getBuffer ((uint8_t**) &vVbuffer, &iVsize);
		return vVbuffer;
	}
	int size_ ();

	/*FRom BUFfer*/
	void CoreTsrl::frbuf (void* cDestination) {
		int* iVheader;

		iVheader = (int*) cDestination;
		cVtransport->resetBuffer ((uint8_t*) &iVheader [1], iVheader [0]);
	}

	/*FRom BUffer Begin*/
	void* CoreTsrl::frbub (int iDim) {

		cVtransport->resetBuffer (iDim);
		return cVtransport->getWritePtr (iDim);
	}

	/*FRom BUffer End*/
	void CoreTsrl::frbue (int iDim) {

		cVtransport->wroteBytes (iDim);
	}