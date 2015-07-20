#include "CoreAnsx.hpp"
#include "CoreVutl.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

using namespace orion;

#define iCansimaxdigit 64
#define iCguard 13
#define ANSXMIN(a,b) ((a<=b) ? (a) : (b))

  CoreAnsx::CoreAnsx () {

		bVfence = '\0';
		bVguard = iCguard;
    iVlength = 0;
    iVallocatedlength = 0;
    bVcontent = 0;
  }

  int CoreAnsx::alloc (int iLength) {

		return iLength + iCansialloc - (iLength & iCansiallocmask);
  }

  void CoreAnsx::start (const char* bString, int iOffset, int iLength) {
    char* bVoldstring;
    int iVtemplength;
		int iVtemp;

		if (iLength == 0) {
			iVlength = 0;
			return;
		}
		iVtemplength = iLength;
    if (iOffset == 0) {
      iVlength = iVtemplength;
      if (iVlength > iVallocatedlength) {
				iVallocatedlength = alloc (iVlength);
				bVcontent = (char*) CoreVutl::reall (bVcontent, iVallocatedlength);
      }			
			if (bVcontent != NULL) memcpy (bVcontent, bString, iVlength);
    } else {
      iVtemp = iOffset - 1;
      iVlength = iVtemplength + iVtemp;
      if (iVlength > iVallocatedlength) {
        if (iVallocatedlength == 0) {
					iVallocatedlength = alloc (iVlength);
					bVcontent = (char*) CoreVutl::reall (bVcontent, iVallocatedlength);
        } else {					
					iVallocatedlength = alloc (iVlength);
					bVcontent = (char*) CoreVutl::reall (bVcontent, iVallocatedlength);					
        }
      };
			if (bVcontent != NULL) memcpy (&bVcontent [iVtemp], bString, iVtemplength);
    }
    if (bVcontent != NULL) bVcontent [iVlength - 1] = 0;
    bVoldstring = 0;
  }


  void CoreAnsx::start (const CoreAnsx& cString, int iOffset) {

		start (cString.c_str (), iOffset, cString.Length () + 1);
  }

  CoreAnsx::CoreAnsx (const char* bString, int iLength) {

		bVfence = '\0';
		bVguard = iCguard;
    iVlength = 0;
    iVallocatedlength = 0;
    bVcontent = 0;
		if (iLength <= 0) {
			if (bString != 0 ) start (bString, 0, strlen (bString) + 1);
			else start ("", 0, 1);
		} else start (bString, 0, iLength);
  }

  CoreAnsx::CoreAnsx (const CoreAnsx& cString)  {
		bVfence = '\0';
		bVguard = iCguard;
    iVlength = 0;
    iVallocatedlength = 0;
    bVcontent = 0;
    start (cString, 0);
  }

  CoreAnsx::CoreAnsx (std::string cString)  {
		bVfence = '\0';
		bVguard = iCguard;
    iVlength = 0;
    iVallocatedlength = 0;
    bVcontent = 0;
		start (cString.c_str (), 0, cString.length () + 1);
  }

  CoreAnsx::CoreAnsx (char bValue, bool bForcelength) {
		char bVstring [iCansimaxdigit];

		bVfence = '\0';
		bVguard = iCguard;
    iVlength = 0;
    iVallocatedlength = 0;
    bVcontent = 0;
		::sprintf (bVstring, "%c", bValue);
		start (bVstring, 0, (!bForcelength) ? strlen (bVstring) + 1 : 2);
  }

  CoreAnsx::CoreAnsx (double dValue) {
    char bVstring [iCansimaxdigit];

		bVfence = '\0';
		bVguard = iCguard;
    iVlength = 0;
    iVallocatedlength = 0;
    bVcontent = 0;
    ::sprintf (bVstring, "%G", dValue);
    start (bVstring, 0, strlen (bVstring) + 1);
	}

	CoreAnsx::CoreAnsx (long int iValue) {
		char bVstring [iCansimaxdigit];

		bVfence = '\0';
		bVguard = iCguard;
    iVlength = 0;
    iVallocatedlength = 0;
    bVcontent = 0;
		::sprintf (bVstring, "%li", iValue);
		start (bVstring, 0, strlen (bVstring) + 1);
	}

	CoreAnsx::CoreAnsx (long unsigned int iValue) {
		char bVstring [iCansimaxdigit];

		bVfence = '\0';
		bVguard = iCguard;
    iVlength = 0;
    iVallocatedlength = 0;
    bVcontent = 0;
		::sprintf (bVstring, "%lu", iValue);
		start (bVstring, 0, strlen (bVstring) + 1);
	}

	CoreAnsx::CoreAnsx (int iValue) {
		char bVstring [iCansimaxdigit];

		bVfence = '\0';
		bVguard = iCguard;
    iVlength = 0;
    iVallocatedlength = 0;
    bVcontent = 0;
		::sprintf (bVstring, "%i", iValue);
		start (bVstring, 0, strlen (bVstring) + 1);
	}

  CoreAnsx::~CoreAnsx () {
		   
		if (iVallocatedlength > 0) free (bVcontent);
		bVcontent = 0;
  }

  CoreAnsx& CoreAnsx::operator =(const CoreAnsx& cString) {

		if ((cString.bVfence == 0)  && (cString.bVguard == iCguard)) 
			start (cString, 0);
		return *this;
  }

  CoreAnsx& CoreAnsx::operator +=(const CoreAnsx& cString) {

		start (cString, iVlength);
    return *this;
  }

  CoreAnsx CoreAnsx::operator +(const CoreAnsx& cString) const {
    CoreAnsx sVstring (*this);

		sVstring.start(cString, sVstring.iVlength);
		return sVstring;
  }

  bool CoreAnsx::operator ==(const CoreAnsx& sString) const {

		return strcmp (sString) == 0;
  }

	bool CoreAnsx::operator !=(const CoreAnsx& sString) const {

		return strcmp (sString) != 0;
  }

  bool  CoreAnsx::operator >(const CoreAnsx& sString) const {

		return strcmp (sString) > 0;
  }

  bool  CoreAnsx::operator <(const CoreAnsx& sString) const {

		return strcmp (sString) < 0;
  }

  bool  CoreAnsx::operator >=(const CoreAnsx& sString) const {

		return strcmp (sString) >= 0;
  }

  bool  CoreAnsx::operator <=(const CoreAnsx& sString) const {

		return strcmp (sString) <= 0;
  }

  char* CoreAnsx::c_str () const {

    if (bVcontent != 0) return bVcontent;
    else return (char*) "";
  }

  int CoreAnsx::strcmp (const CoreAnsx& sString, int iLength) const {
		int iVlengthsource;
		int iVlengthcompare;
		int iVcmpsize;
		int iVmemcmp;

		iVlengthsource = iVlength - 1;		
		iVlengthcompare = sString.Length ();
		if (iLength > 0) {
			iVlengthsource = ANSXMIN (iVlengthsource, iLength);
			iVlengthcompare = ANSXMIN (iVlengthcompare, iLength);
		}
		iVcmpsize = ANSXMIN (iVlengthsource, iVlengthcompare);
		iVmemcmp = iVcmpsize > 0 ? memcmp (bVcontent, sString.c_str (), iVcmpsize) : 0;
		return	iVmemcmp != 0 ? iVmemcmp : 
							(iVlengthsource == iVlengthcompare ? 0 : iVlengthsource < iVlengthcompare ? -1 : 1);
	}

	std::string CoreAnsx::to_string () const {
		std::string sVreturn;

		if (iVlength == 0) return sVreturn;
		sVreturn.resize (iVlength - 1);
		std::copy (bVcontent, &bVcontent [iVlength - 1], sVreturn.begin ());
		return sVreturn;
	}

	std::string CoreAnsx::to_string (char* sData, int iData) {
		std::string sVreturn;

		sVreturn.resize (iData);
		std::copy (sData, &sData [iData - 1], sVreturn.begin ());
		return sVreturn;
	}

  int  CoreAnsx::Length() const {

		return iVlength - 1;
  }

  CoreAnsx CoreAnsx::UpperCase() const {
    CoreAnsx cVreturn;
    int iV;

    cVreturn.start (bVcontent, 0, iVlength);
		for (iV = 0; iV < iVlength; iV++) {
      cVreturn.bVcontent [iV] = toupper (bVcontent [iV]);
    }
    return cVreturn;
	}

	CoreAnsx CoreAnsx::LowerCase() const {
		CoreAnsx cVreturn;
		int iV;

		cVreturn.start (bVcontent, 0, iVlength);
		for (iV = 0; iV < iVlength; iV++) {
			cVreturn.bVcontent [iV] = tolower (bVcontent [iV]);
		}
		return cVreturn;
	}

  CoreAnsx CoreAnsx::SubString (int iIndex, int iCount) const {
		CoreAnsx cVreturn;
		int iVindex;

		if (iIndex <= 0) iVindex = 1;
		else iVindex = iIndex;
		if (iCount <= 0) cVreturn = "";
		if (iVindex < iVlength) cVreturn.start (&bVcontent [iVindex - 1], 0, iCount + 1);
    return cVreturn;
  }

  CoreAnsx CoreAnsx::Trim() const {
    CoreAnsx cVreturn;
		int iV;
    int iV2;

    iV = 0;
    while (iV < iVlength) {
      if (isspace (bVcontent [iV]) == 0) break;
      iV++;
    }
    iV2 = iVlength - 2;
    while (iV2 >= iV) {
      if (isspace (bVcontent [iV2]) == 0) break;
      iV2--;
    }
    cVreturn.start (&bVcontent [iV], 0, iV2 - iV + 2);
    return cVreturn;
  } 

  double CoreAnsx::ToDouble() const {

    return (strtod (bVcontent, NULL));
  }

  int CoreAnsx::ToInt() const {

    return (atoi (bVcontent));
  }

  int CoreAnsx::Pos(const CoreAnsx& sString) const {
    char* bVfind;

    bVfind = strstr (bVcontent, sString.bVcontent);
    if (bVfind != NULL) return int (bVfind - bVcontent) + 1;
    else return 0;
  }

  int CoreAnsx::AnsiPos(const CoreAnsx& sString) const {
    return Pos (sString);
	}

	CoreAnsx& CoreAnsx::sprintf(const char* cFormat, double d) {
		char bVstring [iCansimaxdigit];

		::sprintf (bVstring, cFormat, d);
		start (bVstring, 0, strlen (bVstring) + 1);
		return *this;
	}

  CoreAnsx CoreAnsx::_FloatToStrF (	double dValue, int iDummy,
																		int iPrecision, int iDigits) {
    CoreAnsx sVreturn;
    char bVstring [10];

    ::sprintf (bVstring, "%%.%if", iDigits);
    sVreturn.sprintf(bVstring, dValue);
    return sVreturn;
  } 

	CoreAnsx CoreAnsx::debug (bool bAscii) {
		CoreAnsx sVreturn;
		
		sVreturn = debug (bVcontent, iVlength - 1, bAscii);
		return sVreturn;
	}

	CoreAnsx CoreAnsx::debug (char* bString, int iLength, bool bAscii) {
		CoreAnsx sVreturn;
		CoreAnsx sVtemp;
		char bV [10];
		unsigned char iVchar;

		sVreturn = "";
		for (int iV = 0; iV < iLength; iV++) {
			if (bString [iV] != 0)	{
				iVchar = (unsigned char) bString [iV];
				::sprintf (bV, "%x ", iVchar);
				sVtemp = bV;
				if (sVtemp.Length () == 2) sVtemp = (CoreAnsx) "0" + sVtemp;
				if (bAscii && iVchar > 31 && iVchar < 127) 
					sVreturn += (CoreAnsx) "'" + (CoreAnsx) bString [iV] + (CoreAnsx) "'=";
				sVreturn += sVtemp.UpperCase ();

			} else sVreturn += "00 ";
		}
		return sVreturn;
	}

	CoreAnsx CoreAnsx::prntb () {
		CoreAnsx sVreturn;
		char bVtemp;

		sVreturn = "";
		for (int iV = 0; iV < iVlength; iV++) {
			if (bVcontent [iV] >= 32)	sVreturn += bVcontent [iV];
			else {
				bVtemp = 255 - bVcontent [iV];
				if (bVtemp >= 32)	sVreturn += bVtemp;
				else sVreturn += "#";
			}
		}
		return sVreturn;
	}



