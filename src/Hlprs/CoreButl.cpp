#include "CoreButl.hpp"
#include "CoreFsys.hpp"
#include "CoreHutl.hpp"
#include "re2/re2.h"

using namespace re2;
using namespace std;
using namespace orion;

	const double CoreButl::dCtmstpfactor = 1000000.0;
	const double CoreButl::dCtmstlfactor = dCtmstpfactor * 1000.0;
	const double CoreButl::dCtimestampfactor = 24.0 * 3600.0 * dCtmstpfactor;
	const double CoreButl::dCtimestamplongfactor = 24.0 * 3600.0 * dCtmstlfactor;

  /*GET From File*/
	AnsiString __fastcall CoreButl::getff (PTR_POOL cPool, AnsiString sAbsolutefilename) {
		CoreFsys cVfilesystem (cPool); 
		AnsiString sVreturn;
		
		if (cVfilesystem.read_ (sAbsolutefilename)) {
			sVreturn = AnsiString ((char*) cVfilesystem.baddr (), cVfilesystem.bsize ());
		} else sVreturn = "";
		return sVreturn;
	}

  /*SET To File*/
	bool __fastcall CoreButl::settf (PTR_POOL cPool, AnsiString sAbsolutefilename, AnsiString sContent) {
		CoreFsys cVfilesystem (cPool);
		AnsiString sVreturn;
		bool bVreturn;
		
		bVreturn = false;
		bVreturn = cVfilesystem.owrte (sAbsolutefilename, sContent.c_str (), sContent.Length ());
		return bVreturn;
	}

	/*RePLaCE*/
	AnsiString __fastcall CoreButl::rplce (AnsiString sSource, AnsiString sPattern, AnsiString sRewrite) {
		string sVsource;
		
		sVsource = sSource.to_string ();
		RE2::GlobalReplace (&sVsource, sPattern.c_str (), sRewrite.c_str ());
		return sVsource;
	}

	/*LEFT_ most right delimited only char*/
	AnsiString __fastcall CoreButl::left_ (AnsiString sSource, AnsiString sDelimiter) {
		AnsiString sVexpression;
		string sVsource;
		string sVreturn;
		
		sVexpression = (AnsiString) "(.+?)(\\" + sDelimiter + "[^" + sDelimiter + "]*$|$)";
		sVsource = sSource.to_string ();
		RE2::FullMatch(sVsource, sVexpression.to_string (), &sVreturn);
		return sVreturn;
	}

	/*RIGHT most right delimited only char*/
	AnsiString __fastcall CoreButl::right (AnsiString sSource, AnsiString sDelimiter) {
		AnsiString sVexpression;
		AnsiString sVexpression2;
		string sVsource;
		string sVreturn;
		
		sVexpression = (AnsiString) "(.+?)(\\" + sDelimiter + "[^" + sDelimiter + "]*$|$)";
		sVexpression2 = (AnsiString) "\\" + sDelimiter + "(.+?)";
		sVsource = sSource.to_string ();
		RE2::FullMatch(sVsource, sVexpression.to_string (), (void*) NULL, &sVreturn);
		RE2::FullMatch(sVreturn, sVexpression2.to_string (), &sVreturn);
		return sVreturn;
	}

	/*LEFT_ most left delimited 2*/
	AnsiString __fastcall CoreButl::left2 (AnsiString& sSource, AnsiString sDelimiter) {
		int iPos;
		int iVdelimiterlength;
		AnsiString sSourcebackup;

		sSourcebackup = sSource;
		iVdelimiterlength = sDelimiter.Length ();
		iPos = sSource.AnsiPos (sDelimiter);
		if (iPos > 0) {
			sSource = sSource.SubString (iPos + iVdelimiterlength, sSource.Length () - iPos - iVdelimiterlength + 1);
			return sSourcebackup.SubString (1, iPos - 1);
		} else {
			sSource = "";
			return sSourcebackup;
		}
	}

	/*LEFT_ most left delimited 3 case insensitive*/
	AnsiString __fastcall CoreButl::left3 (AnsiString& sSource, AnsiString sDelimiter) {
		int iPos;
		int iVdelimiterlength;
		AnsiString sSourcebackup;

		sSourcebackup = sSource;
		iVdelimiterlength = sDelimiter.Length ();
		iPos = sSource.UpperCase ().AnsiPos (sDelimiter);
		if (iPos > 0) {
			sSource = sSource.SubString (iPos + iVdelimiterlength, sSource.Length () - iPos - iVdelimiterlength + 1);
			return sSourcebackup.SubString (1, iPos - 1);
		} else {
			sSource = "";
			return sSourcebackup;
		}
	}

	/*ReVerse POSition*/
	int __fastcall CoreButl::rvpos (char* bString, char bDelimiter, int iStartpos) {
		int iVreturn;

		for (iVreturn = iStartpos; iVreturn >= 0; iVreturn--) 
			if (bString [iVreturn] == bDelimiter) break;
		return bString [iVreturn] == bDelimiter ? iVreturn : -1;
	}

	/*SPLit Path*/
	void __fastcall CoreButl::splpt (AnsiString sAddress, AnsiString& sPath, AnsiString& sFilename) {
		char* bVcontent;
		int iV;
		int iVlength;

		iVlength = sAddress.Length ();
		bVcontent = sAddress.c_str ();
		for (iV = iVlength; iV >= 0; iV--) 
			if (bVcontent [iV] == '\\' || bVcontent [iV] == '/') break;
		sPath = sAddress.SubString (0, iV + 1);
		sFilename = sAddress.SubString (iV + 2, iVlength - iV - 1);
	}

	/*TiMeSTamP*/
	int __fastcall CoreButl::tmstp (TDateTime tTime) {

		return int (tTime * dCtimestampfactor);
	}

	/*TiMeSTamp Long long*/
	int64vcl __fastcall CoreButl::tmstl () {

		return int64vcl (__Time () * dCtimestamplongfactor);
	}

	/*TiMeSTamp long long Fixed*/
	int64vcl __fastcall CoreButl::tmstf () {

		return tmstl () + CoreHutl::gettp ();
	}

	/*GET Counter Int*/
	int __fastcall CoreButl::getci (AnsiString sKey, const AnsiString* sVector, int iVectorsize) {

		for (int iV = 0; iV < iVectorsize; iV++) if (sVector [iV] == sKey) return iV;
		return -1;
	}

	/*INTeger 2 String*/
	AnsiString __fastcall CoreButl::int2s (int iLittleendian) {

		return AnsiString ((char*) &iLittleendian, sizeof (iLittleendian) + 1);
	}

	/*INTeger 2 String*/
	AnsiString __fastcall CoreButl::int2s (int iLittleendian, int iOffset) {
		char* bVpointer;

		bVpointer = (char*) &iLittleendian;
		return AnsiString (&bVpointer [iOffset], sizeof (iLittleendian) + 1 - iOffset);
	}

	/*INTeger 64 vcl 2 String*/
	AnsiString __fastcall CoreButl::i642s (int64vcl iLittleendian) {

			return AnsiString ((char*) &iLittleendian, sizeof (iLittleendian) + 1);
	}

	/*LPAD*/
	AnsiString __fastcall CoreButl::lpad_ (AnsiString sSource, AnsiString sPattern, int iLength) {
		AnsiString sVreturn;

		sVreturn = sSource;
		for (int iV = sSource.Length (); iV < iLength; iV++) sVreturn = sPattern + sVreturn;
		return sVreturn;
	}

	/*LPAD Trimmed*/
	AnsiString __fastcall CoreButl::lpadt (AnsiString sSource, AnsiString sPattern, int iLength) {
		AnsiString sVpad;

		sVpad = CoreButl::lpad_ (sSource, sPattern, iLength);
		return iLength < sVpad.Length () ? CoreButl::sbstr (sVpad, sVpad.Length () - iLength + 1) : sVpad;
	}

	/*RPAD*/
	AnsiString __fastcall CoreButl::rpad_ (AnsiString sSource, AnsiString sPattern, int iLength) {
		AnsiString sVreturn;

		sVreturn = sSource;
		for (int iV = sSource.Length (); iV < iLength; iV++) sVreturn += sPattern;
		return sVreturn;
	}

	/*Center PAD*/
	AnsiString __fastcall CoreButl::cpad_ (AnsiString sSource, AnsiString sPattern, int iDim) {

		return	CoreButl::rpad_ (CoreButl::lpad_ (sSource, sPattern, 
						int ((double) iDim / 2.0 + (double) sSource.Length () / 2.0)), sPattern, iDim);
	}

	/*Cast Int64*/
	AnsiString __fastcall CoreButl::ci64_ (int64vcl iInput, bool bLpad) {
		AnsiString sVreturn;
		char bVbuffer [50];

		sprintf (bVbuffer, "%lld", iInput);
		sVreturn = bVbuffer;
		if (bLpad) return lpad_ (sVreturn, '0', 20);
		else return sVreturn;
	}

	/*DouBLe 2 String*/
	AnsiString __fastcall CoreButl::dbl2s (double dLittleendian) {

			return AnsiString ((char*) &dLittleendian, sizeof (dLittleendian) + 1);
	}

	/*CHaR 2 String*/
	AnsiString __fastcall CoreButl::chr2s (unsigned char bInput, int iRepetitions) {
			AnsiString sVreturn;

			sVreturn = "";
			for (int iV = 0; iV < iRepetitions; iV++) sVreturn += AnsiString ((char*) &bInput, 2);			
			return sVreturn;
	}

	/*BOoLean 2 String*/
	AnsiString __fastcall CoreButl::bol2s (bool bInput) {

			return AnsiString ((char*) &bInput, sizeof (bInput) + 1);
	}

	/*DEcreSCent Ascii*/
	AnsiString __fastcall CoreButl::desca (AnsiString sAnsistring) {
		AnsiString sVreturn;
		unsigned char* bVpointer;

		sVreturn = sAnsistring;
		bVpointer = (unsigned char*) sVreturn.c_str ();
		for (int iV = 0; iV < sVreturn.Length (); iV++) 
			bVpointer [iV] = 255 - bVpointer [iV];
		return sVreturn;
	}

	/*REVersed AnsiPos*/
	int __fastcall CoreButl::revap (AnsiString sInput, char cSearchchar) {
		char* sVscan;

		sVscan = sInput.c_str ();
		for (int iV = sInput.Length (); iV >= 0; iV--) {
			if (sVscan [iV] == cSearchchar) return iV + 1;
		}
		return 0;
	}

	/*SuBSTRing*/
	AnsiString __fastcall CoreButl::sbstr (AnsiString sAnsistring, int iIndex) {
		
		return sAnsistring.SubString (iIndex, sAnsistring.Length () - iIndex + 1);
	}

  /*RePLaCE*/
  AnsiString __fastcall CoreButl::rplce (   AnsiString sString,
                                            char sTarget, char sReplace) {
    AnsiString sVreturn;
    int iV;

    sVreturn = "";
    for (iV = 0; iV < sString.Length(); iV++) {
      if (sString.c_str() [iV] != sTarget) sVreturn += sString.c_str() [iV];
      else sVreturn += sReplace;
    }
    return sVreturn;
  }

  /*Fixed FoPEN*/
  AnsiString __fastcall CoreButl::ffpen (AnsiString sFilename) {
    return rplce (sFilename, '\\', '/');
  }

  /*Fixed FoPEN*/
  FILE* __fastcall CoreButl::ffpen (AnsiString sFilename, AnsiString sMode) {
    return fopen (rplce (sFilename, '\\', '/').c_str (), sMode.c_str ());
  }

	/*GET Sub VeRsion*/
	AnsiString __fastcall CoreButl::getsvr () {
		switch (sizeof (void*)) {
			case 4: return "x86";
			case 8: return "x64";
			default: return "x?";
		}
	}

	/*GET Sub VeRsion*/
	double __fastcall CoreButl::movav (double dAccumulator, double dValue, double dAveragesize) {
		double dVnum;
		double dVden;

		dVnum = (dAccumulator * (dAveragesize - 1.0) + dValue);
		dVden = dAveragesize;
		return  dVnum == dVden ? 0 : dVnum / dVden;
	}

