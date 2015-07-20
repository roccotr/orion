#include "CoreLssc.hpp"

	template<typename T, typename C, int iVcachecapacity>
  __fastcall CoreLssc<T, C, iVcachecapacity>::CoreLssc () : CoreLiss<T, C> () {
		
  }

	template<typename T, typename C, int iVcachecapacity> 
  __fastcall CoreLssc<T, C, iVcachecapacity>::~CoreLssc () {
	}
  
	/*BiNary SET*/
	template<typename T, typename C, int iVcachecapacity>
	T __fastcall CoreLssc<T, C, iVcachecapacity>::set__ (int iPosition, bool bInsert) {

		cVcache.clear ();
		return CoreLiss<T, C>::set__ (iPosition, bInsert);
	}

	/*BiNary SET*/
	template<typename T, typename C, int iVcachecapacity>
	T __fastcall CoreLssc<T, C, iVcachecapacity>::bnset (AnsiString sKey) {

		cVcache.clear ();
		return CoreLiss<T, C>::bnset (sKey); 
	}

	/*BiNary GET*/
	template<typename T, typename C, int iVcachecapacity>
	T __fastcall CoreLssc<T, C, iVcachecapacity>::bnget (AnsiString sKey) {
		T cVreturn;

		if (cVcache.get__ (&sKey, cVreturn)) return cVreturn;
		cVreturn = CoreLiss<T, C>::bnget (sKey);
		if (cVreturn != NULL) cVcache.set__ (cVreturn); 
		return cVreturn;
	}

	/*BiNary UNSet*/
	template<typename T, typename C, int iVcachecapacity>
	bool __fastcall CoreLssc<T, C, iVcachecapacity>::bnuns (AnsiString sKey) {

		cVcache.clear ();
		return CoreLiss<T, C>::bnuns (sKey);
	}

	/*UNSET*/
	template<typename T, typename C, int iVcachecapacity>
	void __fastcall CoreLssc<T, C, iVcachecapacity>::unset (int iPosition) {

		cVcache.clear ();
		CoreLiss<T, C>::unset (iPosition);
	}

	/*CLEAR*/
	template<typename T, typename C, int iVcachecapacity>
	void __fastcall CoreLssc<T, C, iVcachecapacity>::clear () {

		cVcache.clear ();
		CoreLiss<T, C>::clear ();
	}

	/*DEBUG*/
	template<typename T, typename C, int iVcachecapacity>
	AnsiString __fastcall CoreLssc<T, C, iVcachecapacity>::debug () {

		return cVcache.debug ();
	}
