#ifndef COREBUTL
#define COREBUTL
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"

namespace orion {

class CoreButl {

public:
	const double static dCtmstpfactor;
	const double static dCtmstlfactor;
	const double static dCtimestampfactor; 
	const double static dCtimestamplongfactor; 

	static AnsiString __fastcall getff (PTR_POOL cPool, AnsiString sAbsolutefilename);
	static bool __fastcall settf (PTR_POOL cPool, AnsiString sAbsolutefilename, AnsiString sContent);
	static AnsiString __fastcall rplce (AnsiString sSource, AnsiString sPattern, AnsiString sRewrite);
	static AnsiString __fastcall left_ (AnsiString sSource, AnsiString sDelimiter);
	static AnsiString __fastcall right (AnsiString sSource, AnsiString sDelimiter);
	static AnsiString __fastcall left2 (AnsiString& sSource, AnsiString sDelimiter);
	static AnsiString __fastcall left3 (AnsiString& sSource, AnsiString sDelimiter);
	static int __fastcall rvpos (char* bString, char bDelimiter, int iStartpos);
	static void __fastcall splpt (AnsiString sAddress, AnsiString& sPath, AnsiString& sFilename);
	static int __fastcall tmstp (TDateTime tTime);
	static int64vcl __fastcall tmstl ();
	static int64vcl __fastcall tmstf ();
	static int __fastcall getci (AnsiString sKey, const AnsiString* sVector, int iVectorsize);
	static AnsiString __fastcall int2s (int iLittleendian);
	static AnsiString __fastcall int2s (int iLittleendian, int iOffset);
	static AnsiString __fastcall i642s (int64vcl iLittleendian);
	static AnsiString __fastcall lpad_ (AnsiString sSource, AnsiString sPattern, int iLength);
	static AnsiString __fastcall lpadt (AnsiString sSource, AnsiString sPattern, int iLength);
	static AnsiString __fastcall rpad_ (AnsiString sSource, AnsiString sPattern, int iLength);
	static AnsiString __fastcall cpad_ (AnsiString sSource, AnsiString sPattern, int iDim);
	static AnsiString __fastcall ci64_ (int64vcl iInput, bool bLpad = true);
	static AnsiString __fastcall dbl2s (double dLittleendian);
	static AnsiString __fastcall bol2s (bool bInput);
	static AnsiString __fastcall chr2s (char bInput, int iRepetitions = 1);
	static AnsiString __fastcall desca (AnsiString sAnsistring);
	static int __fastcall revap (AnsiString sInput, char cSearchchar);
	static AnsiString __fastcall sbstr (AnsiString sAnsistring, int iIndex);
  static AnsiString __fastcall rplce (AnsiString sString, char sTarget, char sReplace);
  static AnsiString __fastcall ffpen (AnsiString sFilename);
  static FILE* __fastcall ffpen (AnsiString sFilename, AnsiString sMode);
	static AnsiString __fastcall getsvr ();
	static double __fastcall movav (double dAccumulator, double dValue, double dAveragesize);

};

};

#endif