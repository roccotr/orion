#ifndef CORELOG_
#define CORELOG_
#include "CoreVcl_.hpp"

namespace orion {

class CoreLog_
{

  enum iCdebugtimestamps
  {
    iCdebugtimestampsdim = 10
  };

	const static AnsiString sCsimplelogpath;
  const static AnsiString sCerrortag;
  const static AnsiString sCerrorfle;
  const static AnsiString sCerrorprm;

	static AnsiString sVpath;
  static AnsiString sVfilename;

  static short int iVdebuglevel;
  static bool bVdebugtimestamp;
  static TDateTime tVdebugtimestamp [iCdebugtimestampsdim];
  static short int iVtracelevel;
 
public:

	const static double dVsecondsperday;

  static void smlog (AnsiString sLog);
  static void setpt (AnsiString sPath);
  static AnsiString getpt ();
  static void setfn (AnsiString sFilename);
  static AnsiString getfn ();

  static void logas (AnsiString sLog);
  static void login (AnsiString sLog, int iLog);
  static void setdb (short int iDebuglevel, bool bDebugtimestamp = false);
  static short int getdb ();
  static bool aredb ();
  static void loger (AnsiString sError, short int iVdebuglevelreq = 3);
  static void logtg (AnsiString sTag, short int iVdebuglevelreq);
  static void logfl (AnsiString sTag, short int iVdebuglevelreq);
  static void debug ( AnsiString sLog, int iIndent,
                      short int iVdebuglevelreq = 3);
	static void dtime (AnsiString sLog);
  static void debgs ( AnsiString sLog, AnsiString sValue, int iIndent,
                      short int iVdebuglevelreq = 3);
  static void debgd ( AnsiString sLog, double dValue, int iIndent,
                      short int iVdebuglevelreq = 3);
  static void debgi ( AnsiString sLog, int iValue, int iIndent,
                      short int iVdebuglevelreq = 3);
  static void debgb ( AnsiString sLog, bool bValue, int iIndent,
                      short int iVdebuglevelreq = 3);
  static void trace (AnsiString sTrace, bool bStart = true);



};

};

#endif