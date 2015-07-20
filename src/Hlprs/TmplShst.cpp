#include "CoreShst.hpp"
#include "CoreTsrl.hpp"

using namespace orion;

	template<typename T>
	const AnsiString CoreShst<T>::sCcontrolsuffix = "_CTR"; 

	template<typename T>
  __fastcall CoreShst<T>::CoreShst (bool bServer, AnsiString sName, bool bCriticalregion) {
		
		bVserver = bServer;
		sVname = sName;
		cVshcontroller = boost::shared_ptr<CoreShme> (new CoreShme (bServer, sVname + sCcontrolsuffix, sizeof (SHAREDCONTROLLOC)));
		bVcontrollerrunning = cVshcontroller->alive ();
		if (bVcontrollerrunning) {
			if (bVserver) {
				cVcontroller = new (cVshcontroller->addrs ()) CoreShcl;
				cVmutexready = boost::shared_ptr<CoreSmph> (new CoreSmph (&cVcontroller->cVmasterready));
				cVmutexgarbage = boost::shared_ptr<CoreSmph> (new CoreSmph (&cVcontroller->cVmastergarbage));
				if (bCriticalregion) {
					cVmutexready->setsc (CoreSmph::iCspincount);
					cVmutexgarbage->setsc (CoreSmph::iCspincount);
				}
				cVmutexready->locks ();
				cVmutexgarbage->locks ();
				cVcontroller->iVdim = 0;
				cVcontroller->iVprocessid = CoreXutl::getpi ();    
			} else {
				cVcontroller = (CoreShcl*) cVshcontroller->addrs (); 
				cVmutexready = boost::shared_ptr<CoreSmph> (new CoreSmph (&cVcontroller->cVmasterready, cVcontroller->iVprocessid));
				cVmutexgarbage = boost::shared_ptr<CoreSmph> (new CoreSmph (&cVcontroller->cVmastergarbage, cVcontroller->iVprocessid)); 
			}
		}
	}

	template<typename T>
  __fastcall CoreShst<T>::~CoreShst () {   
		 
		if (bVcontrollerrunning) if (bVserver) cVcontroller->~CoreShcl ();
	}

	/*ALLOCate*/
	template<typename T>
	bool __fastcall CoreShst<T>::alloc (int64vcl iDim) {
		bool bVreturn;

		if (iDim > cVcontroller->iVdim) {  
			cVcontroller->iVdim = iDim;
			cVshobject = boost::shared_ptr<CoreShme> (new CoreShme (true, sVname, cVcontroller->iVdim));
			bVreturn = cVshobject->alive ();
			cVcontroller->bVchanged = true;
		} else {
			cVcontroller->bVchanged = false;
			bVreturn = true;
		}
		return bVreturn;
	}

  /*PUSH*/
	template<typename T>
	bool __fastcall CoreShst<T>::push_ (T& cObject) {
		T* cVobject;
		int64vcl iVsize;

		if (!bVcontrollerrunning) return false;
		iVsize = sizeof (cObject);
		if (!alloc (iVsize)) return false;
		cVobject = new (cVshobject->addrs ()) T;
		*cVobject = cObject;
		cVmutexready->relse ();
		cVmutexgarbage->locks (); 
		cVobject->~T ();
		return true;
	}

	/*POP*/
	template<typename T>
	T __fastcall CoreShst<T>::pop__ () {
		T cVreturn;

		cVmutexready->locks ();		
		if (cVcontroller->bVchanged) {
			cVshobject = boost::shared_ptr<CoreShme> (new CoreShme (sVname, cVcontroller->iVdim));
			cVcontroller->bVchanged = false;
		}
		cVreturn = * (T*) cVshobject->addrs (); 
		cVmutexgarbage->relse ();
		return cVreturn;
	}

  /*PUSH Serialized*/
	template<typename T>
	bool __fastcall CoreShst<T>::pushs (T& cObject) {
		int64vcl iVsize;
		boost::shared_ptr<CoreTsrl> cVserializer (new CoreTsrl ());

		if (!bVcontrollerrunning) return false;
		iVsize = cVserializer->srlze<T> (cObject);
		if (!alloc (iVsize)) return false;
		cVserializer->tobuf (cVshobject->addrs ()); 
		cVmutexready->relse ();
		return true;
	}

	/*POP Serialized*/
	template<typename T>
	T __fastcall CoreShst<T>::pop_s () {
		T cVreturn;
		boost::shared_ptr<CoreTsrl> cVdeserializer (new CoreTsrl ());;

		cVmutexready->locks ();		
		if (cVcontroller->bVchanged) {
			cVshobject = boost::shared_ptr<CoreShme> (new CoreShme (false, sVname, cVcontroller->iVdim));
			cVcontroller->bVchanged = false;
		}
		cVdeserializer->frbuf (cVshobject->addrs ());
		cVdeserializer->parse<T> (cVreturn);
		return cVreturn;
	}

  /*PuSH Buffer Begin*/
	template<typename T>
	void* __fastcall CoreShst<T>::pshbb (int64vcl iDim) {

		//cazzo bool invece di NULL
		if (!bVcontrollerrunning || !alloc (iDim)) return NULL;
		return cVshobject->addrs (); 
	}

	/*POP Buffer End*/
	template<typename T>
	void __fastcall CoreShst<T>::pshbe () {
		
		cVmutexready->relse ();
	}

	/*POP Buffer Begin*/
	template<typename T>
	int64vcl __fastcall CoreShst<T>::popbb () {

		cVmutexready->locks ();		
		if (cVcontroller->bVchanged) {
			cVshobject = boost::shared_ptr<CoreShme> (new CoreShme (false, sVname, cVcontroller->iVdim));
			cVcontroller->bVchanged = false;
		}
		return cVcontroller->iVdim;
	}

	/*POP Buffer End*/
	template<typename T> 
	void* __fastcall CoreShst<T>::popbe () {

		return cVshobject->addrs ();
	}

	/*ALIVE*/
	template<typename T> 
	bool __fastcall CoreShst<T>::alive () {
		
		return bVcontrollerrunning;
	}


