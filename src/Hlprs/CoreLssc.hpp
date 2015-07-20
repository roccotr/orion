#ifndef CORELSSC
#define CORELSSC
#include "CoreLiss.hpp"
#include "CoreCche.hpp"

template<typename T, typename C, int iVcachecapacity>
class CoreLssc  : public CoreLiss<T, C>  {
private:
  CoreCche<T, iVcachecapacity> cVcache;

public:
  __fastcall CoreLssc ();
  __fastcall ~CoreLssc ();

  T __fastcall set__ (int iPosition = -1, bool bInsert = true);
  T __fastcall bnset (AnsiString sKey);
  T __fastcall bnget (AnsiString sKey);
  bool __fastcall bnuns (AnsiString sKey);
  void __fastcall unset (int iPosition);
  void __fastcall clear ();

  AnsiString __fastcall debug ();
};


#endif