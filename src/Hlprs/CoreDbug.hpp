#ifndef COREDBUG
	#define COREDBUG

	#include "CoreVcl_.hpp"
	#ifdef VSTUDIODEBUG			
		#include "CoreSmph.hpp"
		#include <stdio.h>

		extern TList cVnewlist;
		extern CoreSmph cVnewmutex;
		extern TList cVmalloclist;
		void addtr (TList* cTlist, CoreSmph* cMutex,
								void* vAddr,  DWORD iAsize,  const char* bFname, DWORD iLnum);
		void rmvtr (TList* cTlist, CoreSmph* cMutex, void* vAddr);
    void dumpu ();

		/*NEW*/
		inline void* __cdecl operator new (	unsigned int iSize,
                                        const char* bFile, int iLine) {
			void* vVptr;

			vVptr = (void *) malloc(iSize);
			addtr (&cVnewlist, &cVnewmutex, vVptr, iSize, bFile, iLine);
			return	(vVptr);
		};

		/*DELETE*/
		inline void __cdecl operator delete (void* vP) {

			rmvtr (&cVnewlist, &cVnewmutex, vP);
			free (vP);
		};

		/*DeBug Factory MAlloc*/
		inline void* dbfma (	unsigned int iSize,
													const char* bFile = "dbfma", int iLine = 0) {
			void* vVptr;
			
			vVptr = CoreVutl::mllc8 (iSize);
			addtr (&cVmalloclist, NULL, vVptr, iSize, bFile, iLine);
			return	(vVptr);
		};

		/*DeBug Factory FRee*/
		inline void dbffr (void* vP) {

			rmvtr (&cVmalloclist, NULL, vP);
			CoreVutl::free8 (vP);
			
		};

		#define DEBUGNEW new (__FILE__, __LINE__)
		#define new DEBUGNEW

		#ifdef iCdebugfactorymalloc
			#undef factorymalloc
			#define factorymalloc(X) dbfma (X, __FILE__, __LINE__) 
			#undef factoryfree
			#define factoryfree dbffr
		#endif
	#else
		void dumpu ();
	#endif


#endif