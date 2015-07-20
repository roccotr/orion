#include "CoreLog_.hpp"
#include <stdio.h>
#include "CoreDbug.hpp"

using namespace orion;

	const double CoreLog_::dVsecondsperday = 24*3600;
	const AnsiString CoreLog_::sCsimplelogpath = "C:\\test.log";
	const AnsiString CoreLog_::sCerrortag = "Error in tag";
	const AnsiString CoreLog_::sCerrorfle = "Error in file";
	const AnsiString CoreLog_::sCerrorprm = "***ERROR***>";

	AnsiString CoreLog_::sVpath = "";
	AnsiString CoreLog_::sVfilename = "";
	short int CoreLog_::iVdebuglevel = 0;
	short int CoreLog_::iVtracelevel = 0;
	bool CoreLog_::bVdebugtimestamp = false;
	TDateTime CoreLog_::tVdebugtimestamp [iCdebugtimestampsdim];

  /*SiMple LOG*/
	void CoreLog_::smlog (AnsiString sLog) {
		FILE* cVfile;

		cVfile = fopen ((sCsimplelogpath).c_str(), "a");
		fprintf ( cVfile, "%s\n", sLog.c_str());
		fclose (cVfile);
		cVfile = NULL;  
	}

	/*SET PaTh*/
	void CoreLog_::setpt (AnsiString sPath) {
		sVpath = sPath;
	}

  /*GET PaTh*/
  AnsiString CoreLog_::getpt () {
    return sVpath;
  }

	/*SET FileName*/
	void CoreLog_::setfn (AnsiString sFilename) {
		sVfilename = sFilename;
	}

  /*GET FileName*/
  AnsiString CoreLog_::getfn () {
    return sVfilename;
  }

	/*SET DeBug*/
	void CoreLog_::setdb (short int iDebuglevel, bool bDebugtimestamp) {
		short int iV;

		iVdebuglevel = iDebuglevel;
		bVdebugtimestamp = bDebugtimestamp;
		for (iV = 0; iV < iCdebugtimestampsdim; iV++) tVdebugtimestamp [iV] = __Time();
	}

  /*GET DeBug level*/
  short int CoreLog_::getdb () {

    return iVdebuglevel;
  }

  /*ARE DeBugging*/
  bool CoreLog_::aredb () {

    return iVdebuglevel > 0;
  }

	/*LOG AnsiString*/
	void CoreLog_::logas (AnsiString sLog) {
		FILE* cVfile;
		int iVcurrentprocessid;
		unsigned long int iVcurrentthreadid;

    iVcurrentprocessid = GetCurrentProcessId ();
    iVcurrentthreadid = GetCurrentThreadId();
    if (sVpath != "" && sVfilename != "") {
		  cVfile = fopen ((sVpath + sVfilename).c_str(), "a");
		  fprintf ( cVfile, "%i-%lu %s\n",
							  iVcurrentprocessid,
							  iVcurrentthreadid, sLog.c_str());
		  fclose (cVfile);
    }
		cVfile = NULL;

#ifdef iClogtrace
		printf ("%i-%lu %s\n", iVcurrentprocessid, iVcurrentthreadid, sLog.c_str());
#endif

	}

	/*LOG INtegeer*/
	void CoreLog_::login (AnsiString sLog, int iLog) {
		logas (sLog + " " + (AnsiString) iLog);
	}

	/*LOG ERror*/
	void CoreLog_::loger (AnsiString sError, short int iVdebuglevelreq) {
		if (iVdebuglevelreq <= iVdebuglevel) logas (sCerrorprm + " " + sError);
	}

	/*LOG TaG error*/
	void CoreLog_::logtg (AnsiString sTag, short int iVdebuglevelreq) {
		if (iVdebuglevelreq <= iVdebuglevel)
			logas (sCerrorprm + sCerrortag + " " + sTag);
	}

	/*LOG FiLe error*/
	void CoreLog_::logfl (AnsiString sTag, short int iVdebuglevelreq) {

		if (iVdebuglevelreq <= iVdebuglevel)
			logas (sCerrorprm + sCerrorfle + " " + sTag);
	}

	/*DEBUG*/
	void CoreLog_::debug (AnsiString sLog, int iIndent,
												short int iVdebuglevelreq) {
		short int iV;
		AnsiString sVprint;
		TDateTime tVnow;
		double dVseconds;

		if (iVdebuglevelreq <= iVdebuglevel) {
			if (bVdebugtimestamp && iIndent < iCdebugtimestampsdim) {
				tVnow = __Time ();
				dVseconds = dVsecondsperday * (tVnow - tVdebugtimestamp [iIndent]);
				if (dVseconds < 0) dVseconds = 0;
				sVprint.sprintf ("%.1f >", dVseconds);
				for (iV = 0; iV < 8 - sVprint.Length(); iV++) sVprint = (AnsiString) "0" + sVprint;
				tVdebugtimestamp [iIndent]= tVnow;
			} else sVprint = ">";
			for (int iV = 0; iV < iIndent; iV++) sVprint += "-";
			logas (sVprint + sLog + (AnsiString) "<");
		} 
	}

	/*Debug TIME*/
	void CoreLog_::dtime (AnsiString sLog) {
		AnsiString sVprint;
		TDateTime tVnow;
		double dVseconds;

		tVnow = __Time ();
		dVseconds = dVsecondsperday * (tVnow - tVdebugtimestamp [iCdebugtimestampsdim - 1]);
		sVprint.sprintf ("%.3f >", dVseconds);
		tVdebugtimestamp [iCdebugtimestampsdim - 1] = tVnow;
		logas (sVprint + sLog);
	}

	/*DEBuG String*/
	void CoreLog_::debgs (AnsiString sLog, AnsiString sValue, int iIndent,
												short int iVdebuglevelreq) {

		debug (sLog + " = " + sValue, iIndent, iVdebuglevelreq);
	}

	/*DEBuG Integer*/
	void CoreLog_::debgi (AnsiString sLog, int iValue, int iIndent,
												short int iVdebuglevelreq) {

		debgs (sLog, (AnsiString) iValue, iIndent, iVdebuglevelreq);
	}

	/*DEBuG Double*/
	void CoreLog_::debgd (AnsiString sLog, double dValue, int iIndent,
												short int iVdebuglevelreq) {

		debgs (sLog, (AnsiString) dValue, iIndent, iVdebuglevelreq);
	}

	/*DEBuG Boolean*/
	void CoreLog_::debgb (AnsiString sLog, bool bValue, int iIndent,
												short int iVdebuglevelreq) {

		AnsiString sVtemp;

		if (bValue) sVtemp = "True";
		else sVtemp = "False";
		debgs (sLog, sVtemp, iIndent, iVdebuglevelreq);
	}

	/*TRACE*/
	void CoreLog_::trace (AnsiString sTrace, bool bStart) {

		if (iVtracelevel > 0) {
			if (bStart) logas ((AnsiString) "<<<" + sTrace);
			else logas ((AnsiString) ">>>" + sTrace);
		} 
	}




