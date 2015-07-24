#ifndef CORESTIB
#define CORESTIB
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"

namespace orion {

class CoreStib
{
public:

	typedef struct CoreDtlc {
		unsigned int iVsize;
		void* vVdata;

		/*INITIALIZE*/
		CoreDtlc (unsigned int iSize = 0, void* vData = NULL) { 
			iVsize = iSize;
			vVdata = vData;
		}

	} DATALOC;

	typedef struct CoreStil {
		AnsiString sVindex;
		int iVmainindexlength;
		int64vcl iVindexed;

		DATALOC cVdataloc;

		/*INITIALIZE*/
		CoreStil () {
			sVindex = NULL;
			iVmainindexlength = -1;
			iVindexed = 0;
		}

	} SORTEDINDEXLOC;


	void __fastcall set__ (	SORTEDINDEXLOC* cSortedindexloc, AnsiString sKey, 
													int64vcl iIndexed, short int iMainindexlength);
	void __fastcall set__ (	SORTEDINDEXLOC* cSortedindexloc, int64vcl iIndexed, short int iMainindexlength);

  __fastcall CoreStib ();
  __fastcall ~CoreStib ();


	static AnsiString __fastcall getmn (SORTEDINDEXLOC* cSortedindexloc);



};

};

#endif