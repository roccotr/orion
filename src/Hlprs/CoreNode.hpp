#ifndef CORENODE
#define CORENODE
#include "CoreVcl_.hpp"

namespace orion {

class CoreNode
{
private:
  AnsiString sVtext;
  TList* lVchildren;

  AnsiString __fastcall getlc (AnsiString sSinglesearch);
  CoreNode* __fastcall pcore (
                          AnsiString sPath [], int iLength,
                          bool bDelete, bool bSplit);

public:

  __fastcall CoreNode (AnsiString sText);
  __fastcall CoreNode (double dValue);
  __fastcall CoreNode (int iValue);
	__fastcall CoreNode (bool bValue);
  __fastcall ~CoreNode ();
  AnsiString __fastcall gettx ();
  void __fastcall settx (AnsiString sText);
  TList* __fastcall getcn ();
  void __fastcall addcd (CoreNode* cChild);
  CoreNode* __fastcall getcd (AnsiString sSinglesearch);
  CoreNode* __fastcall getcd ();
  bool __fastcall exslc (AnsiString sSinglesearch);
	bool __fastcall exsnc (AnsiString sSinglesearch);
	bool __fastcall leaf_ ();
  AnsiString __fastcall getls (AnsiString sSinglesearch, bool bDefault = false, AnsiString sDefault = NULL);
  double __fastcall getld (AnsiString sSinglesearch);
  int __fastcall getli (AnsiString sSinglesearch, bool bDefault = false, int iDefaultvalue = 0);
  bool __fastcall getlb (AnsiString sSinglesearch, bool bDefault = false, bool bDefaultvalue = false);
	void __fastcall delcn (AnsiString sText);
  void __fastcall delcn ();
  int __fastcall getcc (AnsiString sSinglesearch);
  AnsiString __fastcall getls (AnsiString sSinglesearch, int iElement);
  bool __fastcall cdis_ (AnsiString sSinglesearch);

  void __fastcall pdlte ( AnsiString sPath [], int iLength);
  CoreNode* __fastcall pfind ( AnsiString sPath [], int iLength);
  CoreNode* __fastcall psplt ( AnsiString sPath [], int iLength);
  void __fastcall pappd ( AnsiString sPath [], int iLength,
                          CoreNode* cAppend);
  void __fastcall prplc ( AnsiString sPath [], int iLength,
                          CoreNode* cAppend);
  void __fastcall pmove (  AnsiString sPathsrce [], int iLengthsrce,
                                  CoreNode* cNodesrce,
                                  AnsiString sPathdest [], int iLengthdest,
                                  bool bReplacedest = false);
  static void __fastcall templ (CoreNode* cA, CoreNode* cB);
  void __fastcall upglf ( AnsiString sLeaf, CoreNode* cA);
	AnsiString __fastcall debug  (int iLevel = 0, CoreNode* cVchild = NULL);
  static AnsiString __fastcall gtnls (  CoreNode* cNode, AnsiString sLeaf,
                                        AnsiString sElse);
  static int __fastcall gtnli (  CoreNode* cNode, AnsiString sLeaf, int iElse);
  static double __fastcall gtnld (  CoreNode* cNode, AnsiString sLeaf, double dElse);
  static bool __fastcall gtnlb (  CoreNode* cNode, AnsiString sLeaf, bool bElse);

	static CoreNode* __fastcall newnd (CoreNode* cLeaf, AnsiString sNodename);
};

};

#endif
