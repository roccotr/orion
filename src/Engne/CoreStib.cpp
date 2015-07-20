#include "CoreStib.hpp"

using namespace orion;

  __fastcall CoreStib::CoreStib () {
		
  }

  __fastcall CoreStib::~CoreStib () {

  }

		/*SET*/
	void __fastcall CoreStib::set__ (	SORTEDINDEXLOC* cSortedindexloc, AnsiString sKey, 
																		int64vcl iIndexed, short int iMainindexlength) {
			
			cSortedindexloc->sVindex = sKey;
			cSortedindexloc->iVindexed = iIndexed;
			cSortedindexloc->iVmainindexlength = iMainindexlength;
	}

		/*SET*/
	void __fastcall CoreStib::set__ (	SORTEDINDEXLOC* cSortedindexloc, int64vcl iIndexed, short int iMainindexlength) {
			
			cSortedindexloc->iVindexed = iIndexed;
			cSortedindexloc->iVmainindexlength = iMainindexlength;
	}

	/*GET MaiN*/
	AnsiString __fastcall CoreStib::getmn (SORTEDINDEXLOC* cSortedindexloc) {
		
		if (cSortedindexloc->iVmainindexlength > 0) 
			return cSortedindexloc->sVindex.SubString (0, 
				Min (cSortedindexloc->sVindex.Length (), cSortedindexloc->iVmainindexlength));
		else return cSortedindexloc->sVindex;
	}