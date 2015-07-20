#ifndef COREXML_
#define COREXML_
#include <boost/shared_ptr.hpp>
#include "CoreVcl_.hpp"
#include "CoreNode.hpp"

namespace orion {

class CoreXml_
{
private:
  const static AnsiString sCopenytag;
  const static AnsiString sCopenntag;
  const static AnsiString sCclsextag;
  const static AnsiString sCexcludetag [];
  const static int iCindent;

  const static char sCcharnl;
  const static char sCcharop;
  const static char sCcharcl;
  const static char sCcharc1;
  const static char sCcharc2;
  const static char sCcharen;

  CoreNode* cVdocument;
  bool bVopeneddocument;

  AnsiString __fastcall sbstr (AnsiString sString, int iPos);
  void __fastcall close ();
  AnsiString __fastcall settc ( bool bIndent, int iLevel,
                                CoreNode* cPointernode, bool& bFoundleaf,
                                bool bFixindent = false);

public:

	const static AnsiString sCxmlext;

  bool static __fastcall fndtc (char* bString, int& iPointer, char* bElement,
                                char& bType);
  bool static __fastcall fndte (char* bString, int& iPointer, char* bElement,
                                bool& bOpentag);


  const static AnsiString sCroot;

  __fastcall CoreXml_ ();
  __fastcall ~CoreXml_ ();

  AnsiString __fastcall getu8 (AnsiString sInput);
  CoreNode* __fastcall getdc (AnsiString sString, bool bConvertutf8 = false);
  CoreNode* __fastcall getfc (AnsiString sContent);
  AnsiString __fastcall settc (bool bIndent, bool bFixindent = false);
	CoreNode* __fastcall getft (AnsiString sTag);
  CoreNode* __fastcall getrt ();
  void __fastcall setrt (CoreNode* cContent);
  void __fastcall setrt (AnsiString sRoot);
  void __fastcall free_ ();

  void __fastcall pappd ( AnsiString sPath [], int iLength, CoreNode* cAppend);
  void __fastcall pappr ( AnsiString sPath [], int iLength, CoreNode* cAppend);  
  void __fastcall pdlte (AnsiString sPath [], int iLength);
  void __fastcall pmove ( AnsiString sPathorig [], int iLengthorig,
                          CoreXml_* cDocumentorig,
                          AnsiString sPathdest [], int iLengthdest,
                          bool bReplacedest = false);
  void __fastcall merge (CoreXml_* cMerger);     
	void __fastcall simpp (AnsiString sGroup, AnsiString sProperty, AnsiString sValue);

	static void __fastcall patch (CoreNode* cNode, int iLevel, CoreNode* cClonednode);
};

typedef boost::shared_ptr<CoreXml_> PTR_XML_;

};

#endif