#include "CoreVutl.hpp"
#include "CoreVcl_.hpp"
#ifdef iCtcmalloc
	#include "gperftools/tcmalloc.h"
#endif

using namespace google;
using namespace orion;

  /*Integer DuMMY*/
  int CoreVutl::idmmy ()  {return 0;}

  /*Double DuMMY*/
  double CoreVutl::ddmmy () {return 0;}

	/*Pointer DuMMY*/
	void CoreVutl::pdmmy (void*) {}

#pragma warning (disable: 4312)
#pragma warning (disable: 4311)


	/*GET Malloc Pointer*/
	void* CoreVutl::getmp (void* iDim) {
		u64 iVdim;

		iVdim = u64 (iDim);
		return (void*) (iVdim + iCmllcalign - (iVdim & iCmllcalignmask));
	}

  /*MaLLoC 8*/
  void* CoreVutl::mllc8 (int iDim)  {

    int iVsizeofint;
    u64 iVpointer;
    u64 iVpointerreturn;
    u64* iVpointerstart;
    void* vVpointer;

		LOG_IF(FATAL, iDim <= 0) << "Malloc:    " << iDim;

    iVsizeofint = sizeof (u64);

#ifndef iCtcmalloc		
		vVpointer = malloc (iDim + iCmllcalign + iVsizeofint);  
#else
		vVpointer = tc_malloc (iDim + iCmllcalign + iVsizeofint);  
#endif

		if (vVpointer != 0) {
			iVpointer =  (u64) vVpointer;
			iVpointerreturn = iVpointer + iVsizeofint;
			iVpointerreturn = u64 (CoreVutl::getmp ((void*) iVpointerreturn));
			iVpointerstart = (u64*) (iVpointerreturn - iVsizeofint);
			*iVpointerstart = iVpointer;	
			return (void*) iVpointerreturn;
		} else return 0;  
  }

  /*FREE 8*/
  void CoreVutl::free8 (void* cPointer)  {
		u64* iVpointerstart;
	  void* vVpointer;

    iVpointerstart = (u64*) ((u64) cPointer - sizeof (u64));
		vVpointer = (void*) (*iVpointerstart);

#ifndef iCtcmalloc
    free (vVpointer);
#else
    tc_free (vVpointer);
#endif
    cPointer = 0;
  }

#pragma warning (default: 4312)
#pragma warning (default: 4311)


  /*REALLoc*/
  void* CoreVutl::reall (void* cPointer, int iDim)  {
		void* cVreturn;
		int iVmallocattempts;

		if (iDim != 0) {
			iVmallocattempts = 0;
			do {
				cVreturn = realloc (cPointer, iDim);
				if (cVreturn == 0) iVmallocattempts++;
			} while (cVreturn == 0 && iVmallocattempts < iCmallocattempts);
		} else {
			if (cPointer != 0) free (cPointer);
			cVreturn = 0;
		} 
		return cVreturn;
	}

  /*GET Volume Information*/
  void CoreVutl::getvi ( char*, int, int,
                      unsigned long* iVreturn,
                      int, int, int, int) {
    *iVreturn = 0;
  }

  /*FNSPLit*/
  void CoreVutl::fnspl (char* bString, char* bDrive, char* bDir, char* bFile, char* bExt) {
    int iVtemp;
    int iVdrive;
    int iVext;
    int iVfile;
    char bVcurrent;
    bool bVfound;

    iVdrive = 0;
    bVfound = false;
    do {
      bVcurrent = bString [iVdrive];
      iVdrive++;
      if (bVcurrent == ':') {
        bVfound = true;
        break;
      }
    }  while (bVcurrent != 0);
    if (bVfound) strncpy (bDrive, bString, iVdrive);
    else iVdrive = 0;
    bDrive [iVdrive] = 0;
    iVfile = iVext = int (strlen (bString));
    bVfound = false;
    do {
      bVcurrent = bString [iVext];
      if (bVcurrent == '.') {
        bVfound = true;
        break;
      }
      iVext--;
    }  while (iVext > iVdrive);
    if (bVfound) strcpy (bExt, &bString [iVext]);
    else {
      bExt [0] = 0;
      iVext = iVfile;
    }
    iVfile = iVext;
    do {
      bVcurrent = bString [iVfile];
      if (bVcurrent == '\\' || bVcurrent == '/') {
        iVfile++;
        break;
      }
      iVfile--;
    }  while (iVfile > iVdrive);
    if (iVfile < 0) iVfile = 0;
    iVtemp = iVext - iVfile;
    if (iVtemp > 0) strncpy (bFile, &bString [iVfile], iVtemp);
    bFile [iVtemp] = 0;
    iVtemp = iVfile - iVdrive;
    if (iVtemp) strncpy (bDir, &bString [iVdrive], iVtemp);
    bDir [iVtemp] = 0;  
	}






