#ifndef COREOO2T
#define COREOO2T
#include <glog/logging.h>
#include "CoreButl.hpp"
#include "CoreVcl_.hpp"
#include "CoreList.hpp"
#include "CoreSmph.hpp"

namespace orion {

template<typename T>
class CoreOo2t {

public:
	typedef struct CoreOo2l {
		AnsiString sVkey;
		T* cVvaluepointer;
		bool bVred;
		bool bVleaf;
		CoreOo2l* cVlink [2];

		/*INITIALIZE*/
		CoreOo2l (AnsiString sKey, bool bRed = true, bool bLeaf = false) {

			sVkey = sKey;
			bVred = bRed;
			bVleaf = bLeaf;
			cVlink [0] = cVlink [1] = NULL;
			cVvaluepointer = bLeaf ? new T : NULL;
		}

		/*DESTRUCTOR*/
		~CoreOo2l () {

			if (cVvaluepointer != NULL) delete cVvaluepointer;
		}

		void copy_ (CoreOo2l* cSource) {

			sVkey = cSource->sVkey;
			bVleaf = cSource->bVleaf;
			if (cVvaluepointer != NULL) delete cVvaluepointer;
			cVvaluepointer = cSource->cVvaluepointer;
			cSource->cVvaluepointer = NULL;
		}

	} OO2INDEXLOC;

	typedef struct CoreOo2r {
		AnsiString sVkey;
		bool bVresult;
		T* cVtarget;
		T* cVleft;
	} OO2RESULTLOC;

	typedef struct CoreOo2q {
		bool bVresult;
		bool bVdeletable;
		OO2INDEXLOC* cVresult;
	} OO2QUERYLOC;

private:
	CoreSmph cVmutex;
	OO2INDEXLOC* cVroot;

	bool isred (OO2INDEXLOC* cNode);
	OO2INDEXLOC* sngl_ (OO2INDEXLOC* cRoot, int iDir); 
	OO2INDEXLOC* dbl__ (OO2INDEXLOC* cRoot, int iDir); 
	OO2INDEXLOC* maken (AnsiString sKey, OO2INDEXLOC* cInherited = NULL, AnsiString sLeafkey = NULL);
	static AnsiString pad__ (AnsiString sString, int iDim);
	T* srch_ (AnsiString* sKey, bool bComplete, OO2INDEXLOC** cNode, OO2INDEXLOC** cParent);

public:

  CoreOo2t ();
  ~CoreOo2t (); 

	T* srch_ (AnsiString* sKey, bool bComplete);
	T* srch_ (AnsiString sKey, bool bComplete);
	OO2QUERYLOC srch_ (AnsiString* sKey);
	OO2QUERYLOC srch_ (AnsiString sKey);
	OO2RESULTLOC insrt (AnsiString sKey, AnsiString sLeafkey = NULL);
	OO2RESULTLOC delte (AnsiString sKey, bool bComplete);
	T* init_ ();

	int level (bool bReadableleaves = true, OO2INDEXLOC* cRoot = NULL, int iLevel = 0);
	bool clear (OO2INDEXLOC* cRoot = NULL);

	AnsiString debug (int iLevel, bool bReadableleaves = true);
	AnsiString prnta (bool bReadableleaves = true);

};

};


#endif