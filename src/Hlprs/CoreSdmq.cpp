#include "CoreSdmq.hpp"

	template<typename T>
  __fastcall CoreSdmq<T>::CoreSdmq(CorePool* cPool) {
		
		cVpool = cPool;
	}

	template<typename T>
  __fastcall CoreSdmq<T>::~CoreSdmq () {

		cVpool = NULL;
	}

  /**/
