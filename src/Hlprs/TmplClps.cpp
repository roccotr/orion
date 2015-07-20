#include "CoreClps.hpp"

using namespace orion;

	template<typename T>
  CoreClps<T>::CoreClps (int iPosition, CHAINEDLISTLOC* cPointer) {
			cVpointer = cPointer;
			iVposition = iPosition;				
  }

	template<typename T>
  CoreClps<T>::CoreClps () {
			cVpointer = NULL;
			iVposition = -1;				
  }

	template<typename T>
	CoreClps<T>::CoreClps (const CoreClps& cCopy) {

		cVpointer = cCopy.cVpointer;
		iVposition = cCopy.iVposition;
	}

	template<typename T>
	CoreClps<T>& CoreClps<T>::operator= (const CoreClps& cCopy) {

		cVpointer = cCopy.cVpointer;
		iVposition = cCopy.iVposition;
		return *this;
	}

	/*RePAIR*/
	template<typename T>
	void CoreClps<T>::rpair (bool bUpdirection) {

		while (iVposition < 0 || iVposition >= cVpointer->cVlist.count ()) {
			if (bUpdirection && cVpointer->cVnext != NULL && cVpointer->cVnext->cVnext != NULL) {
				cVpointer = cVpointer->cVnext;
				iVposition = 0;
			} else if (!bUpdirection && cVpointer->cVprev != NULL && cVpointer->cVprev->cVprev != NULL) {
				cVpointer = cVpointer->cVprev;
				iVposition = cVpointer->cVlist.count () - 1;
			} else break;
		}
	}

	template<typename T>
	void CoreClps<T>::operator+= (int iPosition) {

		iVposition += iPosition;
		rpair (iPosition > 0);

		/*
		while (	cVpointer != NULL && 
						cVpointer->cVprev != NULL && 
						cVpointer->cVnext != NULL && 
						cVpointer->cVnext->cVnext != NULL) { 
			if (iVposition >= cVpointer->cVlist.count ()) {
				iVposition = 0;
				cVpointer = cVpointer->cVnext;
			} else if (iVposition < 0) {
				cVpointer = cVpointer->cVprev;
				iVposition = cVpointer->cVlist.count () - 1;
			} else break;
		}*/
	}

	template<typename T>
	CoreClps<T> CoreClps<T>::operator++ (int iDummy) { 

		*this += 1;
		return *this;
	}

	template<typename T>
	CoreClps<T> CoreClps<T>::operator-- (int iDummy) { 

		*this += -1;
		return *this;
	}

	template<typename T>
	CoreClps<T> CoreClps<T>::operator+(const int iV) {
		CoreClps cVreturn;

		cVreturn = *this;
		cVreturn += iV;
		return cVreturn;
	}

	template<typename T>
	CoreClps<T> CoreClps<T>::operator-(const int iV) {
		CoreClps cVreturn(-1,NULL);

		cVreturn = *this;
		cVreturn += -iV;
		return cVreturn;
	}

	template<typename T>
	bool CoreClps<T>::operator< (const CoreClps& cSource) {

		return (cVpointer == cSource.cVpointer && iVposition >= cSource.iVposition) ? false : true;
	}

	template<typename T>
	bool CoreClps<T>::operator<= (const CoreClps& cSource) {

		return (cVpointer == cSource.cVpointer && iVposition > cSource.iVposition) ? false : true;
	}

	template<typename T>
	bool CoreClps<T>::operator>= (const CoreClps& cSource) {

		return ((cVpointer == cSource.cVpointer && iVposition < cSource.iVposition) || iVposition < 0) ? false : true;
	}

	template<typename T>
	bool CoreClps<T>::operator> (int iPosition) {

		return (iVposition > iPosition) ? true : false;
	}

	template<typename T>
	bool CoreClps<T>::operator>= (int iPosition) {

		return (iVposition >= iPosition) ? true : false;
	}
