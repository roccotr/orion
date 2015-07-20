#ifndef CORESTIX
#define CORESTIX
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreStib.hpp"
#include "CoreList.hpp"
#include "CoreFsys.hpp"
#include "PrtoSstb.pb.h"

namespace orion {

using namespace com::tomting::orion; 

class CoreStix : CoreStib {

	CoreList<SORTEDINDEXLOC> cVlist;

public:

  __fastcall CoreStix (PTR_POOL cPool);
  __fastcall ~CoreStix ();

	bool __fastcall add__ (AnsiString sKey, int64vcl iIndexed = 0, short int iMainindexlength = -1);
	bool __fastcall addop (AnsiString sKey, int64vcl iIndexed, short int iMainindexlength, 
												bool& bRecycle, unsigned int& iRecycle);
	bool __fastcall delop (AnsiString sKey, short int iMainindexlength, unsigned int& iRecycle);
	bool __fastcall addns (AnsiString sKey, int64vcl iIndexed = 0, short int iMainindexlength = -1);
	int __fastcall bnsrc (AnsiString sKey);
	AnsiString __fastcall get__ (int iPosition, bool bOnlymain = false);
	int64vcl __fastcall getix (int iPosition);
	SORTEDINDEXLOC* __fastcall getil (int iPosition);
	void __fastcall setil (SORTEDINDEXLOC* cSortedindexloc, int iPosition = -1);
	void __fastcall setix (int iPosition, int64vcl iIndexed);
	int __fastcall count ();
	unsigned int __fastcall getsz ();
	AnsiString __fastcall debug ();
	void __fastcall clear ();
	void __fastcall deldp ();

	void __fastcall tocnt (PrtoIndx* cVproto);
	void __fastcall fmcnt (PrtoIndx* cVproto);

	int first ();
	int last_ ();


	bool __fastcall addo2 (	AnsiString sKey, short int iMainindexlength, DATALOC* cDataloc) {return false;}
	bool __fastcall delo2 (AnsiString sKey, short int iMainindexlength, bool& bVsearchallkey) {return false;}
	unsigned int __fastcall getsd () {return 0;}
};

typedef boost::shared_ptr<CoreStix> PTR_SORTEDINDEX;

};

#endif