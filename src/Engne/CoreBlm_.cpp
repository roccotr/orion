#include "CoreMmh3.hpp"
#include "CoreBlm_.hpp" 
#include <stdio.h>

using namespace orion;

  __fastcall CoreBlm_::CoreBlm_ (CorePool* cPool, int iM, int iK) { 
		
		iVdim = iM >> 3;
		iVk = iK;
		iVm = iM;
		cVpool = cPool;
		bVbuffer = (char*) cPool->mallc (CorePool::iCchar, iVdim, "BLM_");
		memset (bVbuffer, 0, iVdim);
  }

  __fastcall CoreBlm_::~CoreBlm_ () {

		cVpool->free_ (bVbuffer);
  }

	/*ADD*/
	__fastcall void CoreBlm_::add__ (AnsiString sKey) {
		int iV;
		unsigned int iVhash1;
		unsigned int iVhash2;

		MurmurHash3_x86_32 ( (const void *) sKey.c_str (), sKey.Length (), 0, &iVhash1);
		MurmurHash3_x86_32 ( (const void *) sKey.c_str (), sKey.Length (), iVhash1, &iVhash2);
		for (iV = 0; iV < iVk; iV++) write ((iVhash1 + iV * iVhash2) % iVm);
		
	}

	/*WRITE*/
	__fastcall void CoreBlm_::write (unsigned int iHash) {

		bVbuffer [iHash >> 3] |= 1 << (iHash % 8);
	}

	/*EXiSTing*/
	__fastcall bool CoreBlm_::exst_ (AnsiString sKey) {
		int iV;
		unsigned int iVhash1;
		unsigned int iVhash2;
		bool bVreturn;

		bVreturn = true;
		MurmurHash3_x86_32 ( (const void *) sKey.c_str (), sKey.Length (), 0, &iVhash1);
		MurmurHash3_x86_32 ( (const void *) sKey.c_str (), sKey.Length (), iVhash1, &iVhash2);
		for (iV = 0; iV < iVk; iV++) bVreturn &= read_ ((iVhash1 + iV * iVhash2) % iVm);
		return bVreturn;		
	}

	/*READ*/
	__fastcall bool CoreBlm_::read_ (unsigned int iHash) {

		return ((bVbuffer [iHash >> 3] & (1 << (iHash % 8))) != 0);
	}

	/*CLEAR*/
	__fastcall void CoreBlm_::clear () {

		memset (bVbuffer, 0, iVdim);
	}

	/*GET Default M*/
	/*John Rose @ Oracle*/
	__fastcall int CoreBlm_::getdm (int iMaxindex) {
		
		return int (iMaxindex * iCbloomdefaultk * 1.44);
	}