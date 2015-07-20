#include "CorePool.hpp"
#include "CoreLog_.hpp"
#include "CoreVutl.hpp"
#include "CoreDbug.hpp"
#include <stdio.h>

using namespace google;
using namespace orion;

  const int CorePool::iCattempts = 5;

  __fastcall CorePool::CorePool () {

    iVallocatedmemory = 0;
    iVmaxallocatedmemory = 0;
    iVmaxrequestedmemory = 0;
		bValloclimit = false;		
  }

  __fastcall CorePool::~CorePool () {
    AnsiString sVtemp;
    int iV;
    POOLINFO* cVtemp;

		if (CoreLog_::aredb ())
      CoreLog_::debug ((AnsiString) "pool locations left: " +
                         (AnsiString) cVauditpool.Count, 0, 2);
    logdp ();
		for (iV = cVauditpool.Count - 1; iV >= 0; iV--) {
			cVtemp = (POOLINFO*) cVauditpool.Items [iV];
		
#ifdef iCdebugpool
			printf ("locazione: %s %i %i\n", cVtemp->bVdescription, cVtemp->iVdim, cVtemp->iVtype);
			delete [] cVtemp->bVdescription;	
#endif
			free_ (cVtemp->vVaddress);
			delete cVtemp;
		}
    cVtemp = NULL;
	}

	/*GET DiM*/
	int __fastcall CorePool::getdm (short int iType) {
    int iVreturn;

    switch (iType) {
      case iCdouble: iVreturn = sizeof (double); break;
      case iCfloat: iVreturn = sizeof (float); break;
      case iCint: iVreturn = sizeof (int); break;
      case iClongint: iVreturn = sizeof (long int); break;
      case iCbool: iVreturn = sizeof (bool); break;
      case iCchar: iVreturn = 1; break;
      case iCshortint: iVreturn = sizeof (short int); break;
			default: iVreturn = 0;
    }
		return iVreturn;
	}


#pragma warning (disable: 4312)
#pragma warning (disable: 4311)

  /*MALLoC*/
  void* __fastcall CorePool::mallc (short int iType, int iDim, const char* bComment, 
																		AnsiString sDescription) {


		LOG_IF(FATAL, iDim <= 0) << "Malloc:    " << iDim << ", type: " << iType;

#ifndef iCfastpool
		AnsiString sVtemp;
    int iVattempts;
    int iVbasedim;
    int iVbytes;
		int iVsizeofint;
		u64 iVmallocreturned;
		u64 iVmallocaligned;
		u64* iVmallocdestructor;
    POOLINFO* cVpoolinfo;


		iVmallocaligned = 0;
    iVbasedim = 1;
		iVbasedim = getdm (iType);
		iVsizeofint = sizeof (u64);
    iVbytes = iVbasedim * iDim + iCmllcalign + iVsizeofint;
		cVmutex.locks ();
    iVmallocreturned = (u64) ippsMalloc_8u (iVbytes);
    cVmutex.relse ();
		iVattempts = iCattempts;
		if (bValloclimit && iVallocatedmemory > iValloclimit) {
			iVmallocreturned = 0;
		} else {
			while (iVmallocreturned == 0 && iVattempts > 0) {
				cVmutex.locks ();
				iVmallocreturned = (u64) ippsMalloc_8u (iVbytes);
				cVmutex.relse ();
				iVattempts -= 1;
				Sleep (100);
			}
		}
    if (iVmallocreturned != 0) {
			iVmallocaligned = (u64) CoreVutl::getmp ((void*)(iVmallocreturned + iVsizeofint));
			iVmallocdestructor = (u64*) (iVmallocaligned - iVsizeofint);
			*iVmallocdestructor = iVmallocreturned;
      iVallocatedmemory += iVbytes;
      if (iVallocatedmemory > iVmaxallocatedmemory)
        iVmaxallocatedmemory = iVallocatedmemory;
      if (iVbytes > iVmaxrequestedmemory) iVmaxrequestedmemory = iVbytes;
    } else {
      sVtemp = (AnsiString) "Memory request failed " +  (AnsiString) iVbytes;
      CoreLog_::loger (sVtemp, 1);
      logst ();
			throw EXCEPTION (sVtemp);
    }
    cVpoolinfo = new POOLINFO;
    cVpoolinfo->iVtype = iType;
    cVpoolinfo->vVaddress = (void*) iVmallocaligned;
    cVpoolinfo->iVdim = iDim;
		cVpoolinfo->iVbytes = iVbytes;
    strcpy (cVpoolinfo->bVcomment, bComment); 

#ifdef iCdebugpool
		cVpoolinfo->bVdescription = new char [sDescription.Length ()];
		strcpy (cVpoolinfo->bVdescription, sDescription.c_str ());
#endif

		cVmutex.locks ();
		cVauditpool.Add(cVpoolinfo);
    cVmutex.relse ();
		if (CoreLog_::aredb ()) {
      sVtemp =  (AnsiString) "memory block #: " + bComment + 
								(AnsiString) " " + (AnsiString) int (iVmallocaligned) + 
								(AnsiString) "/" + (AnsiString) int (iVbytes) +
								(AnsiString) "/" + (AnsiString) int (iVallocatedmemory);
      CoreLog_::debug (sVtemp, 0, 6);
    }
    return (void*) iVmallocaligned;
#else
    int iVbasedim;

		iVbasedim = getdm (iType);
		return ippsMalloc_8u (iVbasedim * iDim);
#endif
  }

  /*FREE*/
  void __fastcall CorePool::free_ (void* vMemory) {

#ifndef iCfastpool
    AnsiString sVtemp;
    int iV;
    bool bVfound;
		int iVsizeofint;
		u64 iVmallocaligned;
		u64* iVmallocdestructor;
    POOLINFO* cVtemp;

		cVmutex.locks ();
    bVfound = false;
    for (iV = 0; iV < cVauditpool.Count; iV++) {
      cVtemp = (POOLINFO*) cVauditpool.Items [iV];
      if (cVtemp->vVaddress == vMemory) {
				iVallocatedmemory -= cVtemp->iVbytes;
#ifdef iCdebugpool
				delete [] cVtemp->bVdescription;	
#endif
        delete cVtemp;
        cVauditpool.Delete(iV);
        bVfound = true;
        break;
      }
    }
    if (!bVfound) {
      if ( CoreLog_::aredb ()) {
        sVtemp =  (AnsiString) "memory release error (left " +
                  (AnsiString) cVauditpool.Count + (AnsiString) ")";
        CoreLog_::loger (sVtemp, 1);
      }
			cVmutex.relse ();
			return;
    }
		iVsizeofint = sizeof (u64);
		iVmallocaligned = u64 (vMemory);
		iVmallocdestructor = (u64*) (iVmallocaligned - iVsizeofint);
		if (vMemory != NULL) {
      ippsFree ((void*) *iVmallocdestructor);
      vMemory = NULL;
    }
    cVtemp = NULL;
		cVmutex.relse ();
#else

		ippsFree (vMemory); 
#endif
  }

#pragma warning (default: 4312)
#pragma warning (default: 4311)

  /*GET Memory Allocated*/
  long int __fastcall CorePool::getma () {

    return iVallocatedmemory;
  }

  /*GET maX memory Allocated*/
  long int __fastcall CorePool::getxa () {

    return iVmaxallocatedmemory;
  }

  /*GET maX Requested memory*/
  long int __fastcall CorePool::getxr () {

    return iVmaxrequestedmemory;
  }

  /*LOG STatus*/
  void __fastcall CorePool::logst () {

		CoreLog_::debug ((AnsiString) "max mem alloc: " + (AnsiString) getxa (), 0, 2);
		CoreLog_::debug ((AnsiString) "max mem reqst: " + (AnsiString) getxr (), 0, 2);
    CoreLog_::debug ((AnsiString) "memory alloct: " + (AnsiString) getma (), 0, 2);
  }

#pragma warning (disable: 4312)
#pragma warning (disable: 4311)

  /*LOG DumP*/
  void __fastcall CorePool::logdp () {

#ifdef iCdebugpool    
		AnsiString sVtemp;
    int iV;
    POOLINFO* cVtemp;

		cVmutex->locks ();
    for (iV = 0; iV < cVauditpool->Count; iV++) {
      cVtemp = (POOLINFO*) cVauditpool->Items [iV];
      if (CoreLog_::aredb ()) {
        sVtemp =  (AnsiString) "left memory block: " +
                  (AnsiString) cVtemp->iVtype + (AnsiString) " " +
                  (AnsiString) cVtemp->bVcomment + (AnsiString) " " +
                  (AnsiString) cVtemp->iVdim + (AnsiString) " " +
                  (AnsiString) int (cVtemp->vVaddress);
        CoreLog_::debug (sVtemp, 0, 2);
      }
    }
		cVmutex->relse ();
#endif

  }

#pragma warning (default: 4312)
#pragma warning (default: 4311)


	/*SET AllocLimit*/
	void __fastcall CorePool::setal (long int iAlloclimit) {
		bValloclimit = true;
		iValloclimit = iAlloclimit;
	}