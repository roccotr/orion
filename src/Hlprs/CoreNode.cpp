#include "CoreNode.hpp"

using namespace orion;

  __fastcall CoreNode::CoreNode (AnsiString sText) {
    sVtext = sText;
    lVchildren = new TList ();
  }

  __fastcall CoreNode::CoreNode (double dValue) {
    sVtext = (AnsiString) dValue;
    lVchildren = new TList ();
  }

  __fastcall CoreNode::CoreNode (int iValue) {
    sVtext = (AnsiString) iValue;
    lVchildren = new TList ();
  }

  __fastcall CoreNode::CoreNode (bool bValue) {
    sVtext = (AnsiString) (int) bValue;
    lVchildren = new TList ();
  }

  __fastcall CoreNode::~CoreNode () {
    CoreNode* cVtemp;

    if (lVchildren != NULL) {
      for (int iV = 0; iV < lVchildren->Count; iV++) {
        cVtemp = (CoreNode*) lVchildren->Items [iV];
        if (cVtemp != NULL) delete cVtemp;
        cVtemp = NULL;
      }
      delete lVchildren;
      lVchildren = NULL;
    }
    cVtemp = NULL;
  }

  TList* __fastcall CoreNode::getcn () {
    return lVchildren;
  }

  /*SET TeXt*/
  void __fastcall CoreNode::settx (AnsiString sText) {
    sVtext = sText;
  }

  /*GET TeXt*/
  AnsiString __fastcall CoreNode::gettx () {
    return sVtext;
  }

  /*ADD ChilD*/
  void __fastcall CoreNode::addcd (CoreNode* cChild) {
    
		if (lVchildren == NULL) lVchildren = new TList ();
		lVchildren->Add (cChild);
  }

	void __fastcall addcd (AnsiString sSingle, CoreNode* cChild);

  /*GET ChilD*/
  CoreNode* __fastcall CoreNode::getcd (AnsiString sSinglesearch) {
    CoreNode* cVreturn;
    CoreNode* cViterator;

    cVreturn = NULL;
		if (lVchildren == NULL) return cVreturn;
    for (int iVinner = 0; iVinner < lVchildren->Count; iVinner++) {
      cViterator = (CoreNode*) lVchildren->Items [iVinner];
      if (cViterator->gettx().UpperCase().Trim() ==
          sSinglesearch.UpperCase().Trim())
          cVreturn = cViterator;
    }
    cViterator = NULL;
    return cVreturn;
  }

  /*GET ChilD*/
  CoreNode* __fastcall CoreNode::getcd () {
    CoreNode* cVreturn;

    cVreturn = NULL;
		if (lVchildren == NULL) return cVreturn;		
		if (lVchildren->Count >= 1) {
      cVreturn = (CoreNode*) lVchildren->Items [0];
    }
    return cVreturn;
  }

  /*GET Leaf Content*/
  AnsiString __fastcall CoreNode::getlc (AnsiString sSinglesearch) {
    AnsiString sVreturn;
    CoreNode* cVtemp;

    sVreturn = NULL;
    cVtemp = getcd (sSinglesearch);
    if (cVtemp != NULL) {
      cVtemp = (CoreNode*) cVtemp->getcn ()->Items [0];
      if (cVtemp != NULL) {
        sVreturn = cVtemp->gettx ();
      }
    }
    cVtemp = NULL;
    return sVreturn;
  }

  /*EXiSts Leaf Content*/
  bool __fastcall CoreNode::exslc (AnsiString sSinglesearch) {
    bool bVreturn;

    if (getlc (sSinglesearch)!= NULL) bVreturn = true;
      else bVreturn = false;
    return bVreturn;
  }

  /*EXiSts Node Content*/
  bool __fastcall CoreNode::exsnc (AnsiString sSinglesearch) {

    return (getcn () != NULL && getcd (sSinglesearch) != NULL);
  }

	/*is LEAF*/
	bool __fastcall CoreNode::leaf_ () {
		
		return lVchildren == NULL || lVchildren->Count == 0;
	}

  /*GET Leaf content String*/
  AnsiString __fastcall CoreNode::getls (AnsiString sSinglesearch, bool bDefault, AnsiString sDefault) {
    AnsiString sVtemp;

		if (bDefault && !exslc (sSinglesearch)) return sDefault;
    sVtemp =  getlc (sSinglesearch);
    if (sVtemp == NULL) return "";
    return sVtemp.Trim();
  }

  /*ChilD IS*/
  bool __fastcall CoreNode::cdis_ (AnsiString sSinglesearch) {
    return (gettx ().UpperCase() == sSinglesearch);
  }

  /*GET Leaf content Double*/
  double __fastcall CoreNode::getld (AnsiString sSinglesearch) {
    AnsiString sVtemp;

    sVtemp =  getlc (sSinglesearch);
    if (sVtemp == "0" || sVtemp == "") return 0;
    return sVtemp.Trim ().ToDouble();
  }

  /*GET Leaf content Int*/
  int __fastcall CoreNode::getli (AnsiString sSinglesearch, bool bDefault, int iDefaultvalue) {
    AnsiString sVtemp;

		if (bDefault && !exslc (sSinglesearch)) return iDefaultvalue;
    sVtemp =  getlc (sSinglesearch);
    if (sVtemp == "0" || sVtemp == "") return 0;
		return sVtemp.Trim ().ToInt ();
  }

  /*GET Leaf content Bool*/
  bool __fastcall CoreNode::getlb (AnsiString sSinglesearch, bool bDefault, bool bDefaultvalue) {

		if (bDefault && !exslc (sSinglesearch)) return bDefaultvalue;
		return (getli (sSinglesearch) != 0);
  }

  /*DELete ChildreN*/
  void __fastcall CoreNode::delcn (AnsiString sText) {
    CoreNode* cViterator;

		//new
		if (lVchildren == NULL) return;
		//new

    for (int iVinner = lVchildren->Count - 1; iVinner >= 0; iVinner--) {
      cViterator = (CoreNode*) lVchildren->Items [iVinner];
      if (cViterator->gettx().UpperCase().Trim() == sText.UpperCase().Trim()) {
          delete cViterator;
          cViterator = NULL;
          lVchildren->Delete(iVinner);
      }
    }
		if (lVchildren->Count == 0) {
			delete lVchildren;
			lVchildren = NULL;
		}
		cViterator = NULL;
  }

  /*DELete ChildreN*/
  void __fastcall CoreNode::delcn () {
    CoreNode* cViterator;

		if (lVchildren == NULL) return;
    for (int iVinner = lVchildren->Count - 1; iVinner >= 0; iVinner--) {
      cViterator = (CoreNode*) lVchildren->Items [iVinner];
			delete cViterator;
    }
		lVchildren->Clear ();
  }

  /*GET Children Count*/
  int __fastcall CoreNode::getcc (AnsiString sSinglesearch) {
    int iV;
    int iVreturn;
    CoreNode* cVtemp;

    iVreturn = 0;
		//new
		if (lVchildren == NULL) return iVreturn;
		//new

    for (iV = 0; iV < lVchildren->Count; iV++) {
      cVtemp = (CoreNode*) lVchildren->Items [iV];
      if (cVtemp->gettx().Trim().UpperCase() == sSinglesearch)
        iVreturn++;
    }
    cVtemp = NULL;
    return iVreturn;
  }

  /*GET Leaf String*/
  AnsiString __fastcall CoreNode::getls (AnsiString sSinglesearch, int iElement) {
    AnsiString sVreturn;
    AnsiString sVtemp;
    int iV;
    int iVtemp;
    CoreNode* cVtemp;

    sVreturn = "";
		//new
		if (lVchildren == NULL) return sVreturn;
		//new

    iVtemp = 0;
    for (iV = 0; iV < lVchildren->Count; iV++) {
      cVtemp = (CoreNode*) lVchildren->Items [iV];
      sVtemp = cVtemp->gettx().Trim().UpperCase();
      if (sVtemp == sSinglesearch) {
        if (iVtemp == iElement) {
          sVreturn = ((CoreNode*) cVtemp->getcn ()->Items [0])->gettx();
          sVreturn = sVreturn.Trim().UpperCase();
          break;
        }
        iVtemp++;
      }
    }
    cVtemp = NULL;
    return sVreturn;
  }

  /*Path CORE*/
  CoreNode* __fastcall CoreNode::pcore (
                          AnsiString sPath [], int iLength,
                          bool bDelete, bool bSplit) {
    int iV;
    int iVinner;
    bool bVfound;
    CoreNode* cVpointernode;
    CoreNode* cVtemp;
    CoreNode* cVreturn;
    TList* lVtemp;

    cVreturn = NULL;
    cVpointernode = this;
    for (iV = 0; iV < iLength; iV++) {
      lVtemp = cVpointernode->getcn();
      if (lVtemp == NULL) break;
      bVfound = false;
      for (iVinner = 0; iVinner < lVtemp->Count; iVinner++) {
        cVtemp = (CoreNode*) lVtemp->Items [iVinner];
        if (cVtemp->gettx().UpperCase().Trim() ==
            sPath [iV].UpperCase().Trim()) {
          cVpointernode = cVtemp;
          bVfound = true;
          break;
        }
      }
      if (!bVfound && iV != iLength - 1) {
        cVtemp = new CoreNode (sPath [iV]);
        lVtemp->Add(cVtemp);
        cVpointernode = cVtemp;
      }
    }
    if (bVfound) {
      if (!bDelete) {
        cVreturn = cVtemp;
      }  else {
        if (bSplit) {
          cVreturn = cVtemp;
        } else {
          delete cVtemp;
          cVtemp = NULL;
        }
        lVtemp->Delete(iVinner);
      }
    }
    cVpointernode = NULL;
    cVtemp = NULL;
    lVtemp = NULL;
    return cVreturn;
  }

  /*Path DeLeTE*/
  void __fastcall CoreNode::pdlte ( AnsiString sPath [], int iLength) {

    pcore (sPath, iLength, true, false);
  }

  /*Path FIND*/
  CoreNode* __fastcall CoreNode::pfind ( AnsiString sPath [], int iLength) {

    return pcore (sPath, iLength, false, false);
  }

  /*Path SPLiT*/
  CoreNode* __fastcall CoreNode::psplt ( AnsiString sPath [], int iLength) {

    return pcore (sPath, iLength, true, true);
  }

  /*Path APPenD*/
  void __fastcall CoreNode::pappd ( AnsiString sPath [], int iLength,
                                    CoreNode* cAppend) {
    CoreNode* cVtemp;

    cVtemp = pfind (sPath, iLength);
    cVtemp->addcd (cAppend);
    cVtemp = NULL;
  }

  /*Path RePLaCe*/
  void __fastcall CoreNode::prplc ( AnsiString sPath [], int iLength,
                                    CoreNode* cAppend) {
    CoreNode* cVtemp;

    pdlte (sPath, iLength);
    if (iLength > 1) cVtemp = pfind (sPath, iLength - 1);
    else cVtemp = this;
    cVtemp->addcd(cAppend);
    cVtemp = NULL;
  }

  /*Path MOVE*/
  void __fastcall CoreNode::pmove (
                          AnsiString sPathsrce [], int iLengthsrce,
                          CoreNode* cNodesrce,
                          AnsiString sPathdest [], int iLengthdest,
                          bool bReplacedest) {
    CoreNode* cVtemp;

    cVtemp = cNodesrce->psplt(sPathsrce, iLengthsrce);
    if (!bReplacedest) pappd (sPathdest, iLengthdest, cVtemp);
    else prplc (sPathdest, iLengthdest, cVtemp);
    cVtemp = NULL;
  }

  /*TEMPLate*/
  void __fastcall CoreNode::templ (CoreNode* cA, CoreNode* cB) {
    int iV;
    int iVinner;
    AnsiString sVtemp;
    AnsiString sVtempdelete;
    TList* cVchildren;
    TList* cVtempdeletelist;
    bool bVleaf;
    CoreNode* cVnewone;
    CoreNode* cVtemp;
    CoreNode* cVtempdeletenode;
    CoreNode* cVpointera;
    CoreNode* cVpointerb;

    cVchildren = cA->getcn();
    for (iV = 0; iV < cVchildren->Count; iV++) {
      cVpointera = cVtemp = (CoreNode*) cVchildren->Items [iV];
      sVtemp = cVtemp->gettx();
      if (cVtemp->getcn()->Count == 0) bVleaf = true;
      else bVleaf = false;
      if (!cB->exslc(sVtemp)) {
        if (bVleaf) {
          cVtempdeletelist = cB->getcn ();
          for (iVinner =  cVtempdeletelist->Count - 1; iVinner >= 0 ; iVinner--) {
            cVtempdeletenode = (CoreNode*) cVtempdeletelist->Items [iV];
            delete cVtempdeletenode;
            cVtempdeletelist->Delete(iVinner);
          }
        }
        cVpointerb = cVnewone = new CoreNode (sVtemp);
        cB->addcd(cVnewone);
        cVnewone = NULL;
      } else cVpointerb = cB->getcd(sVtemp);
      templ (cVpointera , cVpointerb);
    }
    cVchildren = NULL;
  }

  /*UPGrade LeaF*/
  void __fastcall CoreNode::upglf ( AnsiString sLeaf, CoreNode* cA) {
    CoreNode* cVtempinsert;

    cVtempinsert = new CoreNode (sLeaf);
    cVtempinsert->addcd(cA);
    delcn(sLeaf);
    addcd(cVtempinsert);
    cVtempinsert = NULL;
  }

  /*GeT Nvl Leaf String*/
  AnsiString __fastcall CoreNode::gtnls (  CoreNode* cNode, AnsiString sLeaf,
                                        AnsiString sElse) {
    if (cNode->exslc (sLeaf)) return cNode->getls (sLeaf);
    else return sElse;
  }

  /*GeT Nvl Leaf Integer*/
  int __fastcall CoreNode::gtnli (  CoreNode* cNode, AnsiString sLeaf, int iElse) {
    if (cNode->exslc (sLeaf)) return int (cNode->getld (sLeaf));
    else return iElse;
  }

  /*GeT Nvl Leaf Double*/
  double __fastcall CoreNode::gtnld (  CoreNode* cNode, AnsiString sLeaf, double dElse) {
    if (cNode->exslc (sLeaf)) return cNode->getld (sLeaf);
    else return dElse;
  }

  /*GeT Nvl Leaf Boolean*/
  bool __fastcall CoreNode::gtnlb (  CoreNode* cNode, AnsiString sLeaf, bool bElse) {
    if (cNode->exslc (sLeaf)) return (int (cNode->getld (sLeaf)) != 0);
    else return bElse;
  }

	/*DEBUG*/
	AnsiString __fastcall CoreNode::debug (int iLevel, CoreNode* cVchild) {
		int iV;
		int iVinner;
		CoreNode* cVpointer;
		TList* cVlistofchildren;
		CoreNode* cVtemp;
		AnsiString sVreturn;
			
		if (cVchild == NULL) cVpointer = this;
		else cVpointer = cVchild;
		for (iVinner = 0; iVinner < iLevel + 1; iVinner++) sVreturn += ">";
		sVreturn += " ";
		sVreturn += cVpointer->gettx () + "\n";
		cVlistofchildren = cVpointer->getcn ();
		for (iV = 0; iV < cVlistofchildren->Count; iV++) {
			cVtemp = (CoreNode*) cVlistofchildren->Items [iV];			
			sVreturn += debug (iLevel + 1, cVtemp);
		}
		return sVreturn;
	}

	/*NEW NoDe*/
	CoreNode* __fastcall CoreNode::newnd (CoreNode* cLeaf, AnsiString sNodename) {
		CoreNode* cVreturn;

		cVreturn = new CoreNode (sNodename);
		cVreturn->addcd (cLeaf);
		return cVreturn;
	}
