#include "CoreOo2w.hpp"
#include "assert.h"

using namespace orion;

	template<typename T, int iVleafdim>
  __fastcall CoreOo2w<T, iVleafdim>::CoreOo2w () {  
		
		init ();
	}

	template<typename T, int iVleafdim>
  __fastcall CoreOo2w<T, iVleafdim>::~CoreOo2w () {

		cVindextree.clear ();
  }

	/*INIT*/
	template<typename T, int iVleafdim>
	void __fastcall CoreOo2w<T, iVleafdim>::init () {
		OO2LEAFLOC* cVroot;

		cVroot = cVindextree.init_ ();
		cVfirst.cVnext = cVroot;
		cVlast.cVprev = cVroot;
		cVroot->cVprev = &cVfirst;
		cVroot->cVnext = &cVlast; 
		iVcount = 0;
	}

	template<typename T, int iVleafdim>
	typename CoreOo2w<T, iVleafdim>::OO2LEAFLOC* __fastcall CoreOo2w<T, iVleafdim>::insrt (AnsiString sKey) {

		typename CoreOo2t<OO2LEAFLOC>::OO2RESULTLOC cVinsertresult;

		cVinsertresult = cVindextree.insrt (sKey, sKey);
		if (cVinsertresult.bVresult) {
			cVinsertresult.cVleft->cVnext->cVprev = cVinsertresult.cVtarget;
			cVinsertresult.cVtarget->cVnext = cVinsertresult.cVleft->cVnext;
			cVinsertresult.cVleft->cVnext = cVinsertresult.cVtarget;
			cVinsertresult.cVtarget->cVprev = cVinsertresult.cVleft;
			return cVinsertresult.cVtarget;
		} else return NULL;
	}

	/*list SaTURation*/
	template<typename T, int iVleafdim>
	void __fastcall CoreOo2w<T, iVleafdim>::satur (CoreList<T>* cList) {
		AnsiString sVkey;
		int iVpartition;	
		T* cVtemp;
		OO2LEAFLOC* cVleafloc; 

		if (cList->count () > iVleafdim) {
			iVpartition = iVleafdim / 2;
			cVtemp = cList->get__ (iVpartition);
			sVkey = cVtemp->sVindex; 
			cVleafloc = insrt (sVkey);  
			cList->split (&cVleafloc->cVlist, iVpartition);
		}
	}

	/*BiNary SET*/
	template<typename T, int iVleafdim>
	T* __fastcall CoreOo2w<T, iVleafdim>::bnset (OO2LEAFLOC* cLeaf, AnsiString* sKey) {
		T* cVreturn;
		CoreList<T>* cVlist;
 
		cVlist = &cLeaf->cVlist;
		cVreturn = cVlist->bnset (*sKey);
		satur (cVlist);
		return cVreturn;
	}

	/*BiNary SET*/
	template<typename T, int iVleafdim>
	T* __fastcall CoreOo2w<T, iVleafdim>::bnset (AnsiString sKey) {
		OO2LEAFLOC* cVleafloc;
		T* cVreturn;

		cVreturn = NULL;
		cVmutex.locks ();
		cVleafloc = cVindextree.srch_ (sKey, true);
		if (cVleafloc == NULL) cVleafloc = insrt (sKey);
		if (cVleafloc != NULL) cVreturn = bnset (cVleafloc, &sKey);
		iVcount += cVreturn != NULL ? 1 : 0;
		cVmutex.relse ();
		return cVreturn;
	}

	/*BiNary GET*/
	template<typename T, int iVleafdim>
	T* __fastcall CoreOo2w<T, iVleafdim>::bnget (AnsiString sKey) {
		OO2LEAFLOC* cVleafloc;
		T* cVreturn;

		cVreturn = NULL;
		cVmutex.locks ();
		cVleafloc = cVindextree.srch_ (sKey, true);
		if (cVleafloc != NULL) cVreturn = cVleafloc->cVlist.bnget (sKey);
		cVmutex.relse ();
		return cVreturn;
	}

	/*BiNary SeaRCh*/
	template<typename T, int iVleafdim>  
	typename CoreOo2w<T, iVleafdim>::OO2POS __fastcall CoreOo2w<T, iVleafdim>::bnsrc (AnsiString* sKey) {
		OO2POS cVreturn(-1,NULL); 

		cVmutex.locks ();  
		cVreturn.cVpointer = cVindextree.srch_ (sKey, true);
		if (cVreturn.cVpointer  != NULL) {
			cVreturn.iVposition = cVreturn.cVpointer->cVlist.bnsrc (*sKey); 
			if (cVreturn.iVposition >= cVreturn.cVpointer->cVlist.count ()) cVreturn.rpair (true);
			else if (cVreturn.iVposition < 0) cVreturn.rpair (false);
		}
		cVmutex.relse ();
		return cVreturn;
	}

	/*BiNary SeaRCh*/
	template<typename T, int iVleafdim> 
	typename CoreOo2w<T, iVleafdim>::OO2POS __fastcall CoreOo2w<T, iVleafdim>::bnsrc (AnsiString* sKey, int& iCompare) {
		OO2POS cVreturn(-1,NULL);

		cVmutex.locks ();
		cVreturn.cVpointer = cVindextree.srch_ (sKey, true); 
		if (cVreturn.cVpointer  != NULL) cVreturn.iVposition = cVreturn.cVpointer->cVlist.bnsrc (sKey, iCompare); 
		else iCompare = -1;
		cVmutex.relse ();
		return cVreturn;
	}

	/*BiNary Search for DeLete*/
	template<typename T, int iVleafdim> 
	typename CoreOo2w<T, iVleafdim>::OO2POS __fastcall CoreOo2w<T, iVleafdim>:: bnsdl (AnsiString* sKey, bool& bDelete, AnsiString& sDelete) {
		typename CoreOo2t<OO2LEAFLOC>::OO2QUERYLOC cVqueryloc;
		OO2POS cVreturn(-1,NULL);
		int iVcomparison;

		cVmutex.locks ();
		cVqueryloc = cVindextree.srch_ (sKey);	
		if (cVqueryloc.bVresult) cVreturn.cVpointer = cVqueryloc.cVresult->cVvaluepointer;
		if (cVreturn.cVpointer  != NULL) {
			cVreturn.iVposition = cVreturn.cVpointer->cVlist.bnsrc (sKey, iVcomparison);   
			
			//questo il problema
			if (iVcomparison == 0 && cVreturn.iVposition > 0) cVreturn.iVposition--;   

			//printf ("!!!!cerco %s, trovo %i\n%s\n", sKey->prntb ().c_str (), cVreturn.iVposition, cVreturn.cVpointer->cVlist.debug ().c_str ());


			bDelete = cVqueryloc.bVdeletable;
			sDelete = cVqueryloc.cVresult->sVkey;
		}
		cVmutex.relse ();
		return cVreturn;
	}

	/*GET*/
	template<typename T, int iVleafdim>
	T* __fastcall CoreOo2w<T, iVleafdim>::get__ (OO2POS cPosition) { 
		T* cVreturn;

		cVreturn = NULL;
		cVmutex.locks ();
		if (cPosition.cVpointer != NULL) 
			cVreturn = cPosition.cVpointer->cVlist.get__ (cPosition.iVposition);
		cVmutex.relse ();
		return cVreturn;
	}

	/*SET*/
	template<typename T, int iVleafdim>
	T* __fastcall CoreOo2w<T, iVleafdim>::set__ (OO2POS cPosition, bool bInsert, AnsiString* sOrderkey) { 
		T* cVreturn;
		OO2LEAFLOC* cVleaf;
		CoreList<T>* cVlist; 

		cVmutex.locks ();
		cVleaf = cPosition.iVposition == -1 ? cVlast.cVprev : cPosition.cVpointer;
		cVlist = &cVleaf->cVlist;
		cVreturn = cVlist->set__ (cPosition.iVposition, bInsert);
		iVcount += cVreturn != NULL ? 1 : 0;
		if (sOrderkey != NULL) {
			cVreturn->sVindex = *sOrderkey;
			satur (cVlist);
		} else assert (cVlist->count () < iVleafdim);
		cVmutex.relse ();
		return cVreturn;
	} 

	/*UNDERrun*/
	template<typename T, int iVleafdim>
	void __fastcall CoreOo2w<T, iVleafdim>::under (OO2LEAFLOC* cLeaf, bool bDeletable, AnsiString* sKeydeleterb) {
		typename CoreOo2t<OO2LEAFLOC>::OO2RESULTLOC cVresultloc;

		if (bDeletable && cLeaf->cVlist.count () == 0) {
			cLeaf->cVprev->cVnext = cLeaf->cVnext;
			cLeaf->cVnext->cVprev = cLeaf->cVprev;
			cVresultloc = cVindextree.delte (*sKeydeleterb, true);  
			if (cVresultloc.sVkey != NULL) {
				typename CoreOo2t<OO2LEAFLOC>::OO2QUERYLOC cVqueryloc = cVindextree.srch_ (cVresultloc.sVkey);
				if (cVqueryloc.bVresult) under (cVqueryloc.cVresult->cVvaluepointer, cVqueryloc.bVdeletable, &cVqueryloc.cVresult->sVkey);
			}
		}
	}

	/*BiNary SET*/
	template<typename T, int iVleafdim>
	bool __fastcall CoreOo2w<T, iVleafdim>::bnuns (	OO2LEAFLOC* cLeaf, AnsiString* sKey, 
																									bool bDeletable, AnsiString* sKeydeleterb) {
		bool bVreturn;
		CoreList<T>* cVlist;
 
		cVlist = &cLeaf->cVlist;
		bVreturn = cVlist->bnuns (*sKey); 
		under (cLeaf, bDeletable, sKeydeleterb); 
		return bVreturn;
	}

	/*UNSET*/
	template<typename T, int iVleafdim> 
	bool __fastcall CoreOo2w<T, iVleafdim>::unset (OO2POS cPosition, bool bDeletable, AnsiString* sKeydeleterb) {
		OO2LEAFLOC* cVleaf;
		CoreList<T>* cVlist;
		bool bVreturn;

		cVmutex.locks ();  
		cVleaf = cPosition.iVposition == -1 ? cVlast.cVprev : cPosition.cVpointer;
		cVlist = &cVleaf->cVlist;
		if (cVlist->unset (cPosition.iVposition)) {
			iVcount -= 1;
			under (cVleaf, bDeletable, sKeydeleterb); 
			bVreturn = true;
		} else bVreturn = false;
		cVmutex.relse ();
		return bVreturn;
	}

	/*BiNary UNSet*/
	template<typename T, int iVleafdim>
	bool __fastcall CoreOo2w<T, iVleafdim>::bnuns (AnsiString sKey) {
		OO2LEAFLOC* cVleafloc;
		typename CoreOo2t<OO2LEAFLOC>::OO2QUERYLOC cVqueryloc;
		bool bVreturn;

		cVmutex.locks ();
		cVqueryloc = cVindextree.srch_ (sKey);		
		bVreturn = false;
		if (cVqueryloc.bVresult) {
			cVleafloc = cVqueryloc.cVresult->cVvaluepointer;
			if (cVleafloc != NULL) bVreturn = bnuns (cVleafloc, &sKey, cVqueryloc.bVdeletable, &cVqueryloc.cVresult->sVkey);
			iVcount -= bVreturn ? 1 : 0;
		}
		cVmutex.relse ();
		return bVreturn;
	}

	/*FIRST*/
	template<typename T, int iVleafdim>
	typename CoreOo2w<T, iVleafdim>::OO2POS __fastcall CoreOo2w<T, iVleafdim>::first () { 
		OO2POS cVreturn(-1,NULL);

		cVmutex.locks ();
		cVreturn.iVposition = 0;
		cVreturn.cVpointer = cVfirst.cVnext;
		cVmutex.relse ();
		return cVreturn;
	}

	/*COUNT*/
	template<typename T, int iVleafdim>
	int __fastcall CoreOo2w<T, iVleafdim>::count () {

		return iVcount;
	}

	/*LAST*/
	template<typename T, int iVleafdim> 
	typename CoreOo2w<T, iVleafdim>::OO2POS __fastcall CoreOo2w<T, iVleafdim>::last_ () {
		OO2POS cVreturn(-1,NULL);

		cVmutex.locks ();
		cVreturn.cVpointer = cVlast.cVprev;
		cVreturn.iVposition = cVreturn.cVpointer->cVlist.count ();
		cVmutex.relse ();
		return cVreturn;
	}

	/*CLEAR*/
	template<typename T, int iVleafdim> 
	void __fastcall CoreOo2w<T, iVleafdim>::clear () {

		cVmutex.locks ();
		cVindextree.clear ();
		init ();	
		cVmutex.relse ();
	}

	/*LiNearized CoUNT*/
	template<typename T, int iVleafdim>
	int __fastcall CoreOo2w<T, iVleafdim>::lncnt () {
		OO2LEAFLOC* cVcursor;
		int iVresult;
		
		iVresult = 0;
		cVcursor = cVfirst.cVnext;
		while (cVcursor != &cVlast) {
			iVresult += cVcursor->cVlist.count ();
			cVcursor = cVcursor->cVnext;
		}
		return iVresult;
	}

	/*LiNearized 2 PoSition*/
	template<typename T, int iVleafdim> 
	typename CoreOo2w<T, iVleafdim>::OO2POS __fastcall CoreOo2w<T, iVleafdim>::ln2ps (int iLinearizedelement) {
		OO2LEAFLOC* cVcursor;
		OO2POS cVreturn(-1,NULL);
		int iVresult;
		int iVthislistdim;
		
		iVresult = 0;
		cVcursor = cVfirst.cVnext;
		while (cVcursor != &cVlast) {
			iVthislistdim = cVcursor->cVlist.count ();
			iVresult += iVthislistdim;
			if (iVresult > iLinearizedelement) {
				cVreturn.cVpointer = cVcursor;
				cVreturn.iVposition = iVthislistdim - iVresult + iLinearizedelement;
				break;
			}
			cVcursor = cVcursor->cVnext;
		}
		return cVreturn;
	}


	/*PRiNT All*/
	template<typename T, int iVleafdim> 
	AnsiString __fastcall CoreOo2w<T, iVleafdim>::prnta (bool bReadableleaves) {

		return cVindextree.prnta (bReadableleaves);
	} 

