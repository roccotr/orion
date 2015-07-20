#include "CoreOo2t.hpp"

using namespace orion;

#define iCprintoo2key 4
#define iCprintoo2space 0

	template<typename T>
  CoreOo2t<T>::CoreOo2t () {

		cVroot = NULL;
  }

	template<typename T>
  CoreOo2t<T>::~CoreOo2t () {

		clear ();
  }

	/*IS RED*/
	template<typename T>
	bool CoreOo2t<T>::isred (OO2INDEXLOC* cNode ) {
		
		return cNode != NULL && cNode->bVred;
	}

	/*SiNGLe rotation*/
	template<typename T>
	typename CoreOo2t<T>::OO2INDEXLOC* CoreOo2t<T>::sngl_ (OO2INDEXLOC* cRoot, int iDir) {
		OO2INDEXLOC* cVsave = cRoot->cVlink[!iDir];
  
		cRoot->cVlink[!iDir] = cVsave->cVlink[iDir];
		cVsave->cVlink[iDir] = cRoot; 
		cRoot->bVred = true;
		cVsave->bVred = false;
		return cVsave;
	}

	/*DouBLe*/
	template<typename T>
	typename CoreOo2t<T>::OO2INDEXLOC* CoreOo2t<T>::dbl__ (OO2INDEXLOC* cRoot, int iDir) {

		cRoot->cVlink[!iDir] = sngl_ (cRoot->cVlink[!iDir], !iDir );
		return sngl_ (cRoot, iDir);
	}

	/*MAKE Node*/
	template<typename T>
	typename CoreOo2t<T>::OO2INDEXLOC* CoreOo2t<T>::maken (AnsiString sKey, OO2INDEXLOC* cInherited, AnsiString sLeafkey) {
		OO2INDEXLOC* cVreturn; 

		cVreturn = new OO2INDEXLOC (sKey);
		cVreturn->cVlink [1] = new OO2INDEXLOC (sLeafkey, false, true);
		if (cInherited != NULL) cVreturn->cVlink [0] = cInherited;
		else cVreturn->cVlink [0] = new OO2INDEXLOC (NULL, false, true);
		return cVreturn;
	}

	/*MAKE Node*/
	template<typename T>
	T* CoreOo2t<T>::init_ () {

		clear ();
		cVroot = new OO2INDEXLOC (NULL, false, true); 
		return cVroot->cVvaluepointer;
	}

	/*SeaRCH*/
	template<typename T>
	T* CoreOo2t<T>::srch_ (AnsiString* sKey, bool bComplete, OO2INDEXLOC** cNode, OO2INDEXLOC** cParent) {
		OO2INDEXLOC* cVq;
		int iVcompare;

		cVq = cVroot;
		*cNode = NULL;
		*cParent = NULL;
		while (cVq != NULL) {
			*cParent = *cNode;
			*cNode  = cVq;
			iVcompare = sKey->strcmp (cVq->sVkey);
			if (iVcompare == 0 && !bComplete) return cVq->cVvaluepointer;
			else if (iVcompare < 0) cVq = cVq->cVlink [0];
			else cVq = cVq->cVlink [1];
		}
		return *cNode  == NULL ? NULL : (*cNode)->cVvaluepointer;
	}

	/*SeaRCH*/
	template<typename T>
	T* CoreOo2t<T>::srch_ (AnsiString* sKey, bool bComplete) {
		OO2INDEXLOC* cVp;
		OO2INDEXLOC* cVq;

		return srch_ (sKey, bComplete, &cVq, &cVp);
	}

	/*SeaRCH*/
	template<typename T>
	T* CoreOo2t<T>::srch_ (AnsiString sKey, bool bComplete) {
		
		return srch_ (&sKey, bComplete);
	}

	/*SeaRCH*/
	template<typename T>
	typename CoreOo2t<T>::OO2QUERYLOC CoreOo2t<T>::srch_ (AnsiString* sKey) {
		OO2INDEXLOC* cVp;
		OO2INDEXLOC* cVq;
		OO2QUERYLOC cVreturn;

		cVreturn.bVresult = srch_ (sKey, true, &cVq, &cVp) != NULL;
		cVreturn.cVresult = cVq;
		cVreturn.bVdeletable = cVreturn.bVresult && (cVp != NULL && cVp->cVlink [1] == cVq);
		return cVreturn;
	}

	/*SeaRCH*/
	template<typename T>
	typename CoreOo2t<T>::OO2QUERYLOC CoreOo2t<T>::srch_ (AnsiString sKey) {
		
		return srch_ (&sKey);
	}

	/*INSeRT*/ 
	template<typename T>
	typename CoreOo2t<T>::OO2RESULTLOC CoreOo2t<T>::insrt (AnsiString sKey, AnsiString sLeafkey) {
		OO2RESULTLOC cVresult;
		OO2INDEXLOC* cVg;
		OO2INDEXLOC* cVt;
		OO2INDEXLOC* cVp;
		OO2INDEXLOC* cVq;
		OO2INDEXLOC cVh (NULL, NULL);
		int iVdir;
		int iVdir2;
		int iVlast;

		cVt = &cVh;
		iVdir = 0;
		cVg = cVp = NULL;
		cVq = cVt->cVlink [1] = cVroot;

		while (true) {
			if (cVq == NULL || cVq->bVleaf) {
				cVq = maken (sKey, (cVq != NULL && cVq->bVleaf) ? cVq : NULL, sLeafkey);				
				if (cVq == NULL) {
					cVresult.bVresult = false;
					return cVresult;
				}
				cVresult.cVtarget = cVq->cVlink [1]->cVvaluepointer;
				cVresult.cVleft = cVq->cVlink [0]->cVvaluepointer;
				if (cVp == NULL) {
					cVroot = cVq;
					cVroot->bVred = false;
					cVresult.bVresult = true;
					return cVresult;
				}
				cVp->cVlink[iVdir] = cVq;
			} else if (isred (cVq->cVlink [0]) && isred (cVq->cVlink [1])) {
				cVq->bVred = true;
				cVq->cVlink[0]->bVred = cVq->cVlink[1]->bVred = false;
			}
			if (cVg != NULL) {
				if (isred (cVq) && isred (cVp)) {
					iVdir2 = cVt->cVlink[1] == cVg;
					if (cVq == cVp->cVlink[iVlast]) cVt->cVlink[iVdir2] = sngl_ (cVg, !iVlast);
					else cVt->cVlink[iVdir2] = dbl__ (cVg, !iVlast);
				}
			}

			if (cVq->sVkey == sKey) break;
			iVlast = iVdir;
			iVdir = cVq->sVkey < sKey;
			if (cVg != NULL) cVt = cVg;
			cVg = cVp; 
			cVp = cVq;
			cVq = cVq->cVlink[iVdir];
		}

		cVroot = cVh.cVlink[1];	
		cVroot->bVred = false;
		cVresult.bVresult = true;
		return cVresult;
	}

	/*DELeTE*/
	template<typename T>
	typename CoreOo2t<T>::OO2RESULTLOC CoreOo2t<T>::delte (AnsiString sKey, bool bComplete) {
		OO2INDEXLOC* cVg;
		OO2INDEXLOC* cVf;
		OO2INDEXLOC* cVp;
		OO2INDEXLOC* cVq;
		OO2INDEXLOC* cVs;
		OO2RESULTLOC cVreturn;
		OO2INDEXLOC cVh (NULL, NULL);
		int iVdir;
		int iVdir2;
		int iVlast;

		if (cVroot != NULL) {

			for (int iV = 0; iV < 2; iV++) {
				cVq = &cVh;
				cVg = cVp = cVf = NULL;			
				cVq->cVlink [1] = cVroot;
				iVdir = 1;

				while (cVq->cVlink [iVdir] != NULL) { 

					iVlast = iVdir;
					cVg = cVp;
					cVp = cVq;
					cVq = cVq->cVlink [iVdir];
					iVdir = cVq->sVkey < sKey;
					if (cVq->sVkey == sKey) cVf = cVq;
					if (!isred (cVq) && !isred (cVq->cVlink[iVdir])) {
						if (isred (cVq->cVlink[!iVdir])) cVp = cVp->cVlink[iVlast] = sngl_ (cVq, iVdir);
						else if (!isred (cVq->cVlink[!iVdir] ) ) {
							cVs = cVp->cVlink [!iVlast];
							if (cVs != NULL) { 
								if (!isred (cVs->cVlink [!iVlast] ) && !isred (cVs->cVlink[iVlast])) {
									cVp->bVred = false;
									cVs->bVred = cVq->bVred = true;
								} else {
									iVdir2 = cVg->cVlink[1] == cVp;
									if (isred (cVs->cVlink [iVlast])) cVg->cVlink [iVdir2] = dbl__ (cVp, iVlast);
									else if (isred (cVs->cVlink [!iVlast])) cVg->cVlink [iVdir2] = sngl_ (cVp, iVlast);
									cVq->bVred = cVg->cVlink [iVdir2]->bVred = true;
									cVg->cVlink [iVdir2]->cVlink[0]->bVred = false;
									cVg->cVlink [iVdir2]->cVlink[1]->bVred = false;
								}
							}
						}
					}
				}

				if (cVf != NULL) { 
					
					cVf->copy_ (cVq);
					iVdir = cVq->cVlink [0] == NULL; 
				
					if (iV == 0) {
						cVreturn.sVkey = cVg->sVkey; 
						cVreturn.cVtarget = cVg->cVlink [1] != NULL ? cVg->cVlink [1]->cVvaluepointer : NULL;				
					}

					cVp->cVlink[cVp->cVlink[1] == cVq] = cVq->cVlink [iVdir];
					delete cVq;
					cVq = cVf;			 
				}

				cVroot = cVh.cVlink [1];
				if (cVroot != NULL) cVroot->bVred = false;
				if (!bComplete) break;
			}
		}
		return cVreturn;
	}

	/*LEVEL*/
	template<typename T>
	int CoreOo2t<T>::level (bool bReadableleaves, OO2INDEXLOC* cRoot, int iLevel) {
		bool bVroot;
		int iVreturn;

		bVroot = (cRoot == NULL);
		if (bVroot) 
			if (cVroot != NULL) cRoot = cVroot;
			else return iLevel;
		iLevel++;
		iVreturn = iLevel;
		for (int iV = 0; iV < 2; iV++)
			if (cRoot->cVlink [iV] != NULL && !(bReadableleaves && cRoot->cVlink [iV]->bVleaf)) 
				iVreturn = Max (iVreturn, level (bReadableleaves, cRoot->cVlink [iV], iLevel));
		return iVreturn;
	}

	/*CLEAR*/
	template<typename T>
	bool CoreOo2t<T>::clear (OO2INDEXLOC* cRoot) {
		bool bVroot;

		bVroot = (cRoot == NULL);
		if (bVroot) 
			if (cVroot != NULL) cRoot = cVroot;
			else return true;
		for (int iV = 0; iV < 2; iV++)
			if (cRoot->cVlink [iV] != NULL) clear (cRoot->cVlink [iV]);
		delete cRoot;
		if (bVroot) cVroot = NULL;
		return true;
	}

	/*DEBUG*/
	template<typename T>
	AnsiString CoreOo2t<T>::debug (int iLevel, bool bReadableleaves) {
		OO2INDEXLOC* cVq;
		int iVcount;
		int iVmask;
		int iVtemp;
		int iVdir;
		int iPrintdim;
		AnsiString sVtemp;
		AnsiString sVreturn;

		sVreturn = "";
		iPrintdim = (1 << level (bReadableleaves)) * (iCprintoo2key + iCprintoo2space * 2) / (1 << iLevel);
		iVcount = 1 << iLevel;
		iVmask = iLevel - 1;
		for (int iV = 0; iV < iVcount; iV++) {
			iVtemp = iV;
			cVq = cVroot;
			for (int iVinner = 0; iVinner < iLevel; iVinner++) {
				iVdir = (iVtemp & (1 << iVmask)) >> iVmask;
				if (cVq != NULL && !(bReadableleaves && cVq->bVleaf)) cVq = cVq->cVlink [iVdir];
				else break;
				iVtemp = iVtemp << 1;
			}
			if (cVq != NULL && !(bReadableleaves && cVq->bVleaf)) {
				if (cVq->bVleaf) {
					if (cVq->bVred) sVtemp = (AnsiString) "|" + cVq->sVkey.prntb () + (AnsiString)  "|";
					else sVtemp = (AnsiString) "{" + cVq->sVkey.prntb () + (AnsiString) "}";
				} else {
					if (cVq->bVred) sVtemp = (AnsiString) "(" + cVq->sVkey.prntb () + (AnsiString)  ")";
					else sVtemp = (AnsiString) "[" + cVq->sVkey.prntb () + (AnsiString) "]";
				}
			} else sVtemp = "";
			sVreturn += CoreButl::cpad_ (sVtemp, " ", iPrintdim);
		}
		return sVreturn;
	}

	/*PRiNT All*/
	template<typename T>
	AnsiString CoreOo2t<T>::prnta (bool bReadableleaves) {
		AnsiString sVreturn;

		sVreturn = "";
		for (int iV = 0; iV < level (bReadableleaves); iV++) sVreturn += debug (iV, bReadableleaves) + "\n";
		return sVreturn;
	}