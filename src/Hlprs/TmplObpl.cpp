#include "CoreObpl.hpp"

//#define iCdebugconnection

using namespace orion;

	template<typename T>
	const double CoreObpl<T>::dCtimestampfactor = 24.0 * 3600.0 * 1000.0;

	template<typename T>
  __fastcall CoreObpl<T>::CoreObpl(int iExpirationtimems) {

		dVexpirationtime = (double) iExpirationtimems / dCtimestampfactor;
	}

	template<typename T>
  __fastcall CoreObpl<T>::~CoreObpl () {
	}

	template<typename T>
	void __fastcall CoreObpl<T>::close () {
		for (int iV = 0; iV < cVunlockedpool.count (); iV++) unset (cVunlockedpool.get__ (iV)->cVobject);
		for (int iV = 0; iV < cVlockedpool.count (); iV++) unset (cVlockedpool.get__ (iV)->cVobject);
	}

	template<typename T>
	AnsiString __fastcall CoreObpl<T>::key__ (typename CoreObpl::PTR_T cObject) {
		int64vcl iVpointer;

		iVpointer = (int64vcl) cObject.get ();
		return AnsiString ((char*) &iVpointer , sizeof (&iVpointer) + 1);
	}

  /*CHeck OUT*/
	template<typename T>
	typename CoreObpl<T>::PTR_T  __fastcall CoreObpl<T>::chout() {
		PTR_T cVreturn;
		OBJECTLOC* cVobjectloc;
		OBJECTLOC* cVnewobjectloc;
		TDateTime dVnow;

		cVmutex.locks ();
		if (cVunlockedpool.count () > 0) {
			dVnow = __Time ();
			for (int iV = cVunlockedpool.count () - 1; iV >= 0 ; iV--) {
				cVobjectloc = cVunlockedpool.get__ (iV);
				if ((dVnow - cVobjectloc->dVstarttime) > dVexpirationtime) {
					unset (cVobjectloc->cVobject);
					cVunlockedpool.unset (iV);
				} else {
					if (valid (cVobjectloc->cVobject)) {
						cVnewobjectloc = cVlockedpool.bnset (key__ (cVobjectloc->cVobject));
						cVnewobjectloc->cVobject = cVobjectloc->cVobject;
						cVnewobjectloc->dVstarttime = __Time ();
						cVunlockedpool.unset (iV);
						cVreturn = cVobjectloc->cVobject;

#ifdef iCdebugconnection
						printf ("Rus connection %i/%i\n", cVlockedpool.count (), cVlockedpool.count () + cVunlockedpool.count ());
#endif

						break;
					} else {
						unset (cVobjectloc->cVobject);
						cVunlockedpool.unset (iV);
						cVreturn.reset ();

#ifdef iCdebugconnection
						printf ("Inv connection %i/%i\n", cVlockedpool.count (), cVlockedpool.count () + cVunlockedpool.count ());
#endif

					}
				}
			}
		}
		if (cVreturn.use_count () == 0) {
			cVreturn = set__ ();
			cVobjectloc = cVlockedpool.bnset (key__ (cVreturn));
			cVobjectloc->cVobject = cVreturn;			
			cVobjectloc->dVstarttime = __Time ();

#ifdef iCdebugconnection
			printf ("New connection %i/%i\n", cVlockedpool.count (), cVlockedpool.count () + cVunlockedpool.count ());
#endif

			
		}
		cVmutex.relse ();
		return cVreturn;
	}

  /*CHeck IN*/
	template<typename T>
	void __fastcall CoreObpl<T>::chin_ (typename CoreObpl::PTR_T cObject) {
		int iVposition;
		AnsiString sVkey;
		OBJECTLOC* cVobjectloc;

		cVmutex.locks ();
		sVkey = key__ (cObject);
		iVposition = cVlockedpool.bnsrc (sVkey) - 1;
		if (iVposition >= 0 && iVposition < cVlockedpool.count ()) cVlockedpool.unset (iVposition);
		cVobjectloc = cVunlockedpool.bnset (sVkey);
		cVobjectloc->cVobject = cObject;
		cVobjectloc->dVstarttime = __Time ();
		cVmutex.relse ();
	}