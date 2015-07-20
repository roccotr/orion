#include "CoreShme.hpp"

#ifndef iCdisablesharedmemory
#include <boost/interprocess/windows_shared_memory.hpp> 
#endif

using namespace orion;

  __fastcall CoreShme::CoreShme (bool bServer, AnsiString sName, int64vcl iSize) { 

		//printf ("alloco %s %i\n", sName.c_str (), iSize);

		iVsize = iSize;
		cVsharedmemory = NULL;

#ifndef iCdisablesharedmemory
		try {
			if (bServer) cVsharedmemory = new windows_shared_memory (open_or_create, TEXT (sName.c_str ()), read_write, iVsize);
			else cVsharedmemory = new windows_shared_memory (open_only, TEXT (sName.c_str ()), read_write);
			cVregion = boost::shared_ptr<mapped_region> (new mapped_region (*(windows_shared_memory*)cVsharedmemory, read_write));
			bVrunning = true;
		} catch (...) {
			bVrunning = false;
			//printf ("CoreShme %lld\n", iSize);
		}
#else
			bVrunning = false;
#endif


	}

  __fastcall CoreShme::~CoreShme () { 

#ifndef iCdisablesharedmemory
		//printf ("dealloco %lld\n", iVsize);
		if (cVsharedmemory != NULL) delete (windows_shared_memory*) cVsharedmemory;
#endif

	}

  /*ADDReSs*/
	void* __fastcall CoreShme::addrs () {

		return cVregion->get_address ();
	}

	/*GET SiZe*/
	int64vcl __fastcall CoreShme::getsz () {

		return iVsize;
	}

	/*ALIVE*/
	bool __fastcall CoreShme::alive () {

		return bVrunning;
	}