#ifndef CORENEWV
#define CORENEWV
	#ifndef VSTUDIODEBUG		
	#include "CoreVcl_.hpp"


		/*NEW*/
		inline void* __cdecl operator new (size_t iSize) {

			return malloc (iSize);
		};


		/*DELETE*/

		inline void __cdecl operator delete (void* vP) {

			free (vP);
		};



	#endif
#endif