#ifndef CORESHST
#define CORESHST
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreShme.hpp"
#include "CoreVcl_.hpp"
#include "CoreSmph.hpp"



#define iCsharedmemorynamedim 200

namespace orion {

template<typename T>
class CoreShst {

	const static AnsiString sCcontrolsuffix;

	struct CoreShcl {
		CoreSmph cVmasterready;
		CoreSmph cVmastergarbage;
		int64vcl iVdim;
		bool bVchanged;
		unsigned int iVprocessid;
	} SHAREDCONTROLLOC;

	AnsiString sVname;
	bool bVserver;
	bool bVcontrollerrunning;
	PTR_SHAREDMEMORY cVshcontroller;
	PTR_SHAREDMEMORY cVshobject;
	CoreShcl* cVcontroller;

	boost::shared_ptr<CoreSmph> cVmutexready;
	boost::shared_ptr<CoreSmph> cVmutexgarbage;

	bool __fastcall alloc (int64vcl iDim);

public:
  __fastcall CoreShst	(bool bServer, AnsiString sName, bool bCriticalregion);
  __fastcall ~CoreShst ();

	bool __fastcall push_ (T& cObject);
	T __fastcall pop__ ();

	bool __fastcall pushs (T& cObject);
	T __fastcall pop_s ();

	void* __fastcall pshbb (int64vcl iDim);
	void __fastcall pshbe ();

	int64vcl __fastcall popbb ();
	void* __fastcall popbe ();

	bool __fastcall alive ();
};

};

#endif