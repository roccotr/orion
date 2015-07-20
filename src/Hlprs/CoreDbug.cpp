#include "CoreVcl_.hpp"
#ifdef VSTUDIODEBUG			
	#include "CoreSmph.hpp"
	#include <stdio.h>
		
	typedef struct {
		bool bVvirtualdelete;
		void*	cVaddress;
		DWORD	iVsize;
		char	bVfile [128];
		DWORD	iVline;
  } TRACEINFO;
	TList cVnewlist;   
	CoreSmph cVnewmutex;
	TList cVmalloclist;   

	/*ADD TRace*/
	void addtr (TList* cTlist, CoreSmph* cMutex,
							void* vAddr,  DWORD iAsize,  const char* bFname, DWORD iLnum) {
		TRACEINFO* cVinfo;	      
		 
		cVinfo = new (TRACEINFO);
		cVinfo->bVvirtualdelete = false;
		cVinfo->cVaddress = vAddr;
		strcpy(cVinfo->bVfile, bFname);
		cVinfo->iVline = iLnum;
		cVinfo->iVsize = iAsize;
		if (cMutex) cMutex->locks ();
		cTlist->Add(cVinfo);
		if (cMutex) cMutex->relse ();
  };

	/*ReMoVe TRace*/
	void rmvtr (TList* cTlist, CoreSmph* cMutex, void* vAddr) {
		int iV;
		TRACEINFO* cVinfo;	      
		#ifdef iCvirtualdelete
			bool bVfound = false;
			char bVbuf [1024];
			sprintf(bVbuf, "Location not found %i\n", int (vAddr));
		#endif

		if (cMutex) cMutex->locks ();
		for(iV = 0; iV < cTlist->Count; iV++) {
			cVinfo = (TRACEINFO*) cTlist->Items [iV];					
			if(cVinfo->cVaddress == vAddr) {
				#ifdef iCvirtualdelete
					if (cVinfo->bVvirtualdelete) {
						sprintf(bVbuf, "%s(%d): %d deleted again %i\n",
							cVinfo->bVfile, cVinfo->iVline, cVinfo->iVsize, int (vAddr));			
					} else {
						bVfound = true;
						cVinfo->bVvirtualdelete = true;	
					}						
				#else
					cTlist->Delete (iV);
			  break;
				#endif
		  }
	  }
		if (cMutex) cMutex->relse ();
		#ifdef iCvirtualdelete
		if (!bVfound) {
			OutputDebugString(bVbuf);
		}
		#endif
  };

	/*DUMP Unfreed*/
	void dumpu () {
		#ifndef iCvirtualdelete
			int iV;
			char bVbuf [1024];
			TRACEINFO* cVinfo;	      

			for(iV = 0; iV < cVnewlist.Count; iV++) {
				cVinfo = (TRACEINFO*) cVnewlist.Items [iV];	
				
				sprintf(bVbuf, "%s(%d): %d unfreed [new]\n",
					cVinfo->bVfile, cVinfo->iVline, cVinfo->iVsize);
				OutputDebugString(bVbuf);
			}			
			for(iV = 0; iV < cVmalloclist.Count; iV++) {
				cVinfo = (TRACEINFO*) cVmalloclist.Items [iV];	
				
				sprintf(bVbuf, "%s(%d): %d unfreed [factorymalloc] %s\n",
					cVinfo->bVfile, cVinfo->iVline, cVinfo->iVsize, 
					cVinfo->cVaddress);
				OutputDebugString(bVbuf);
			}		
		#endif
	};

#else
	void dumpu () {}
#endif