#include "CoreXml_.hpp"
#include "CoreLog_.hpp"
#include "CoreDbug.hpp"

using namespace orion;

	const AnsiString CoreXml_::sCxmlext = "XML";
  const AnsiString CoreXml_::sCroot = "ROOT";
  const AnsiString CoreXml_::sCopenytag = "<";
  const AnsiString CoreXml_::sCopenntag = "</";
  const AnsiString CoreXml_::sCclsextag = ">";
  const AnsiString CoreXml_::sCexcludetag [] = {"<!", "<?"};
  const int CoreXml_::iCindent = 2;

  const char CoreXml_::sCcharnl = '\0';
  const char CoreXml_::sCcharop = '<';
  const char CoreXml_::sCcharcl = '>';
  const char CoreXml_::sCcharc1 = '?';
  const char CoreXml_::sCcharc2 = '!';
  const char CoreXml_::sCcharen = '/';

  __fastcall CoreXml_::CoreXml_ () {
    cVdocument = NULL;
    bVopeneddocument = false;
  }

  __fastcall CoreXml_::~CoreXml_ () {
    close ();
  }

  /*CLOSE*/
  void __fastcall CoreXml_::close () {
    if (bVopeneddocument) {
      delete cVdocument;
      cVdocument = NULL;
      bVopeneddocument = false;
    }
  }

  /*SuBSTR*/
  AnsiString __fastcall CoreXml_::sbstr (AnsiString sString, int iPos) {
    return sString.SubString(iPos, sString.Length() - iPos + 1);
  }

  /*FiND Tag Core*/
  bool  __fastcall CoreXml_::fndtc (char* bString, int& iPointer, char* bElement,
                                    char& bType) {
    bool bVreturn;
    bool bVstart;
    int iVinsert;

    bVstart = false;
    iVinsert = 0;
    bVreturn = false;
    while (bString [iPointer] != sCcharnl) {
      if (bString [iPointer] == sCcharcl) {
        bVstart = false;
        bElement [iVinsert] = sCcharnl;
        bVreturn = true;
        break;
      }
      if (bVstart) {
        bElement [iVinsert] = bString [iPointer];
        iVinsert ++;
      }
      if (bString [iPointer] == sCcharop) {
        bVstart = true;
        bType = bString[iPointer + 1];
        if (bType == sCcharen) iPointer++;
      }
      iPointer++;
    }
    iPointer++;
    return bVreturn;
  }

  /*FiND Tag Enhanced*/
  bool  __fastcall CoreXml_::fndte (  char* bString, int& iPointer, char* bElement,
                                      bool& bOpentag) {
    char bVresponse;
    bool bVreturn;

    bVresponse = sCcharc1;
    while (bVresponse == sCcharc1 || bVresponse == sCcharc2) {
      bVreturn = fndtc (bString, iPointer, bElement, bVresponse);
      if (!bVreturn) break;
    }
    if (bVresponse == sCcharen) bOpentag = false;
    else bOpentag = true;
    return bVreturn;
  }

  /*GET Utf8*/
  AnsiString __fastcall CoreXml_::getu8 (AnsiString sInput) {
    AnsiString sVreturn;
    int iV;
    char* bVstring;
    int iVread;

    bVstring = sInput.c_str();
    if(((bVstring[0] & 0xFF) == 0xFF) && ((bVstring[1] & 0xFF) == 0xFE)) {
        sVreturn = "";
        for (iV = 2; iV < sInput.Length();iV++) {
         iVread = (int) (bVstring [iV]&0xFF);
         if (iVread>0) sVreturn += (char) iVread;
        }

    } else sVreturn = sInput;
    return sVreturn;
  }

  /*GET generic DoCument*/
  CoreNode* __fastcall CoreXml_::getdc (  AnsiString sString, bool bConvertutf8) {
    AnsiString sVxmltext;
    AnsiString sVcontent;
    int iVpointer;
    bool bVopentag;
    bool bVcontentvalid;
    bool bVtag;
    char* bVxmltext;
    char bVtagtext [256];
    CoreNode* cVreturn;
    CoreNode* cVpointernode;
    CoreNode* cVtemp;
    TList* lVpath;

    cVreturn = NULL;
    close ();
    lVpath = new TList;    
    sVxmltext = sString;		
		if (sVxmltext != NULL) {
			bVopeneddocument = true;
			cVdocument = new CoreNode (sCroot);
			cVreturn = cVdocument;
			cVpointernode = cVdocument;
			lVpath->Add(cVpointernode);
			if (bConvertutf8) sVxmltext = getu8 (sVxmltext);
			bVcontentvalid = false;
			if (sVxmltext != NULL) {
				try {
					bVxmltext = sVxmltext.c_str ();
					iVpointer = 0;
					bVtag = true;
					while (bVtag == true) {
						bVtag = fndte (bVxmltext, iVpointer, bVtagtext, bVopentag);
						if (bVtag == true) {
							if (bVopentag) {
								cVtemp = new CoreNode ((AnsiString) bVtagtext);
								lVpath->Add(cVtemp);
								cVpointernode->addcd (cVtemp);
								cVpointernode = cVtemp;
								sVcontent = "";
								bVcontentvalid = true;
								while (bVxmltext [iVpointer] != sCcharop/* && bVxmltext [iVpointer]>=32*/) {
									sVcontent += bVxmltext [iVpointer];
									iVpointer++;
								}
							} else {
								if (bVcontentvalid) {
									bVcontentvalid = false;
									cVtemp = new CoreNode (sVcontent);
									cVpointernode->addcd (cVtemp);
								}
								if (lVpath->Count <= 1) {
									throw EXCEPTION ("");
								}
								lVpath->Delete(lVpath->Count - 1);
								cVpointernode = (CoreNode*) lVpath->Items [lVpath->Count - 1];
							}
						}
					}		
				} catch (...) {
					close ();
					throw EXCEPTION ("");
				}
			}
    }
    delete lVpath;
    lVpath = NULL;
    cVpointernode = NULL;
    cVtemp = NULL;
    return cVreturn;
  }

  /*GET From Content*/
  CoreNode* __fastcall CoreXml_::getfc (AnsiString sContent) {
    CoreNode* cVreturn;

    cVreturn =  getdc (sContent, false);
    return cVreturn;
  }

	/*GET From Tag*/
	CoreNode* __fastcall CoreXml_::getft (AnsiString sTag) {

		return getfc (	CoreXml_::sCopenytag + sTag + CoreXml_::sCclsextag + 
										CoreXml_::sCopenntag + sTag + CoreXml_::sCclsextag)->getcd (sTag);
	}

  /*SET To Content*/
  AnsiString __fastcall CoreXml_::settc ( bool bIndent, int iLevel,
                                          CoreNode* cPointernode,
                                          bool& bFoundleaf,
                                          bool bFixindent) {
    AnsiString sVreturn;
    AnsiString sVindent;
    AnsiString sVtemp;
    int iV;
    int iVinner;
    bool bVfoundleaf;
    CoreNode* cVtemp;
    TList* cVchildren;

    bFoundleaf = false;
    sVreturn = "";
    sVindent = "";
    if (bIndent) {
      for (iVinner = 0; iVinner < iCindent * iLevel; iVinner++)
        sVindent += " ";
    }
    cVchildren = cPointernode->getcn();
    for (iV = 0; iV < cVchildren->Count; iV++) {
      cVtemp = (CoreNode*) cVchildren->Items [iV];
      if (cVtemp->getcn()->Count == 0) {
        sVtemp = cVtemp->gettx().Trim() ;
        if (sVtemp != "")
          sVreturn += (AnsiString) " " + cVtemp->gettx().Trim() + " ";
        bFoundleaf = true;
      } else {
        if (bIndent && iV == 0) sVreturn += bCcarriagereturn;
        sVreturn += sVindent + sCopenytag  + cVtemp->gettx() + sCclsextag;
        if (bFixindent && bIndent && iLevel == 0) sVreturn += bCcarriagereturn;
        sVreturn += settc (bIndent, iLevel + 1, cVtemp, bVfoundleaf);
        if (!bVfoundleaf) sVreturn += sVindent;
        sVreturn += sCopenntag  + cVtemp->gettx() + sCclsextag;
        if (bIndent) sVreturn += bCcarriagereturn;
      }
    }
    cVtemp = NULL;
    cVchildren = NULL;
    return sVreturn;
  }

  /*SET To Content*/
  AnsiString __fastcall CoreXml_::settc (bool bIndent, bool bFixindent) {
    bool bVdummy;

    return settc (bIndent, 0, cVdocument, bVdummy, bFixindent);
  }

  /*GET RooT*/
  CoreNode* __fastcall CoreXml_::getrt () {
    return cVdocument;
  }

  /*SET RooT*/
  void __fastcall CoreXml_::setrt (CoreNode* cContent) {
    close ();
    cVdocument = cContent;
    bVopeneddocument = true;
  }

  /*SET RooT*/
  void __fastcall CoreXml_::setrt (AnsiString sRoot) {

		setrt (new CoreNode (sRoot));
  }

  /*FREE*/
  void __fastcall CoreXml_::free_ () {
    cVdocument = NULL;
  }

  /*Path APPend Replacing*/
  void __fastcall CoreXml_::pappr ( AnsiString sPath [], int iLength,
                                    CoreNode* cAppend) {
    CoreNode* cVtemp;

    cVtemp = new CoreNode (sPath [iLength - 1]);
    cVtemp->addcd (cAppend);
    cVdocument->prplc (sPath, iLength, cVtemp);
    cVtemp = NULL;
  }


  /*Path APPenD*/
  void __fastcall CoreXml_::pappd ( AnsiString sPath [], int iLength,
                                    CoreNode* cAppend) {

    cVdocument->pappd (sPath, iLength, cAppend);
  }

  /*Path DeLeTE*/
  void __fastcall CoreXml_::pdlte (AnsiString sPath [], int iLength) {

    cVdocument->pdlte (sPath, iLength);
  }

  /*Path MOVE*/
  void __fastcall CoreXml_::pmove (
                          AnsiString sPathorig [], int iLengthorig,
                          CoreXml_* cDocumentorig,
                          AnsiString sPathdest [], int iLengthdest,
                          bool bReplacedest) {

    cVdocument->pmove ( sPathorig, iLengthorig, cDocumentorig->getrt (),
                        sPathdest, iLengthdest, bReplacedest);
  }

  /*MERGE*/
  void __fastcall CoreXml_::merge (CoreXml_* cMerger) {
    int iV;
    int iVelements;
    AnsiString sVreplacestring [] = {"", ""};
    CoreNode* cVroot;
    CoreNode* cVmain;
    CoreNode* cVtemp;
    TList* cVlist;
    TList* cVlist2;

    cVroot = cMerger->getrt();
    cVlist = cVroot->getcn ();
    if (cVlist->Count >= 1) {
      cVmain = (CoreNode*) cVlist->Items [0];
      sVreplacestring [0] = cVmain->gettx ();
      cVlist2 = cVmain->getcn ();
      iVelements = cVlist2->Count;
      for (iV = iVelements - 1; iV >= 0 ; iV--) {
        cVtemp = (CoreNode*) cVlist2->Items [iV];
        sVreplacestring [1] = cVtemp->gettx ();
        pdlte (sVreplacestring, 2);
        pmove (sVreplacestring, 2, cMerger, sVreplacestring, 2, true);
      }
    }
    cVroot = NULL;
    cVmain = NULL;
    cVtemp = NULL;
    cVlist = NULL;
    cVlist2 = NULL;
  }

	/*PATCH*/
	void __fastcall CoreXml_::patch (CoreNode* cNode, int iLevel, CoreNode* cClonednode) {
		TList* cVchildren;
		CoreNode* cVchild;

		cVchildren = cNode->getcn ();
		if (cVchildren != NULL) 
			for (int iV = 0; iV < cVchildren->Count; iV++) {
				CoreNode* cVtemp = (CoreNode*) cVchildren->Items [iV];
				if (cVtemp->leaf_ ())  cClonednode->delcn ();
				if (cClonednode->exsnc (cVtemp->gettx ()))  {
					cVchild = cClonednode->getcd (cVtemp->gettx ());					
				} else {
					cVchild = new CoreNode ((AnsiString) cVtemp->gettx ()); 
					cClonednode->addcd (cVchild);
				}
				patch (cVtemp, iLevel + 1, cVchild);	
			}	
	};

	/*SIMPle Property*/
	void __fastcall CoreXml_::simpp (AnsiString sGroup, AnsiString sProperty, AnsiString sValue) {

		AnsiString sVpath [] =  {sGroup, sProperty};
    CoreNode* cVtemp = new CoreNode (sProperty);
    cVtemp->addcd(new CoreNode (sValue));
    cVdocument->prplc(sVpath, 2, cVtemp);
	}



