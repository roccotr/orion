#include "CoreBint.hpp"
#include "CoreButl.hpp"
#include "CoreRvrs.hpp"
#include <math.h>
#include <stdio.h>
#include <string.h>

using namespace orion;

const int CoreBint::iCbaseorder = 4;
const int64vcl CoreBint::iCbase = int64vcl (pow ((double) 256, CoreBint::iCbaseorder));


  CoreBint::CoreBint() {
		bVoverflow = false;
	}

  CoreBint::~CoreBint () {
	}

  CoreBint::CoreBint (AnsiString &sV) {

		read_(sV);
  }

  void CoreBint::operator=(const CoreBint &cA) {

		cVa = cA.cVa;
		bVoverflow = cA.bVoverflow;
  }

  void CoreBint::operator=(const AnsiString& sA) {

		read_ (sA);
	}

  CoreBint CoreBint::operator+(const CoreBint &cA) const {
		int64vcl iVaccumulator;
		int iVopers;
		int iVelement;

		CoreBint cVres = cA;
		iVopers = Max(cVa.size(), cVres.cVa.size());
		for (int iV = cVres.cVa.size (); iV < iVopers; iV++) cVres.cVa.insert (cVres.cVa.begin (), 0);
    for (int iV = iVopers  - 1, iVcarry = 0; iV >= 0 || iVcarry; iV--) {
			if (iV < 0 && iVcarry) cVres.cVa.insert (cVres.cVa.begin (), 0);
			iVelement = Max (0, iV);
			iVaccumulator = cVres.cVa[iVelement ] + iVcarry + (iV >= 0 ? cVa[iV] : 0);
			iVcarry = iVaccumulator >= iCbase;
			cVres.cVa[iVelement] = iVcarry ? iVaccumulator - iCbase : iVaccumulator;
    }
    return cVres;
	}

  void CoreBint::operator+=(const CoreBint &cA) {
  
		*this = *this + cA;
  }

  CoreBint CoreBint::operator-(const CoreBint &cA) const {
		int64vcl iVaccumulator;
		int iVopers;
		int iVelement;

		CoreBint cVres = cA;
		iVopers = Max(cVa.size(), cVres.cVa.size());
		for (int iV = cVres.cVa.size (); iV < iVopers; iV++) cVres.cVa.insert (cVres.cVa.begin (), 0);
    for (int iV = iVopers  - 1, iVcarry = 0; iV >= 0 || iVcarry; iV--) {
			if (iV < 0 && iVcarry) cVres.cVa.insert (cVres.cVa.begin (), 0);
			if (cVres.cVa.size () > cVa.size ()) {
				cVres.bVoverflow = true;
				break;
			}
			iVelement = Max (0, iV);
			iVaccumulator = - cVres.cVa[iVelement ] - iVcarry + (iV >= 0 ? cVa[iV] : 0);
			iVcarry = iVaccumulator < 0;
			cVres.cVa[iVelement] = iVcarry ? iVaccumulator + iCbase : iVaccumulator;
    }
    return cVres;
	}

  CoreBint CoreBint::operator/(int64vcl cA) const {
		int64vcl iVrem;

		iVrem = 0;
		CoreBint cVres = *this;
		for (int iV = 0; iV < (int) cVa.size(); iV++) {
			int64vcl iVcur = cVa[iV] + iVrem * (int64vcl) iCbase;
			cVres.cVa[iV] = iVcur / cA;			
			iVrem = iVcur % cA;

    }
		cVres.trim_();
		return cVres;
  }

  void CoreBint::operator/=(int64vcl cA) {

		*this = *this / cA;
  }

  CoreBint CoreBint::operator*(int64vcl cA) const {
		int64vcl iVcarry;
		int iVelement;

		iVcarry = 0;
		CoreBint cVres = *this;
		for (int iV = cVres.cVa.size ()  - 1; iV >= 0 || iVcarry; iV--) {
			if (iV < 0 && iVcarry) cVres.cVa.insert (cVres.cVa.begin (), 0);
			iVelement = Max (0, iV);
			int64vcl iVcur = cA * cVres.cVa[iVelement] + iVcarry;
			cVres.cVa[iVelement] = iVcur % iCbase;		
			iVcarry = iVcur / iCbase;		
    }
		return cVres;
  }

  void CoreBint::operator*=(int64vcl cA) {

		*this = *this * cA;
  }

	/*TRIM*/
  void CoreBint::trim_ () {
		while (!cVa.empty() && !cVa.front ()) cVa.erase (cVa.begin ());
	}

	/*ANsiString 2 Bint*/
	void  CoreBint::ans2b (AnsiString sAnsistring) {
		int iVnewlength;	
		int iVx;
		char* bVpointer;
		CoreRvrs cVendian;

		bVoverflow = false;
		cVa.clear();
		iVnewlength = sAnsistring.Length ();
		bVpointer = &((char*) &iVx) [sizeof (int) - iCbaseorder];
		for (int iV = 0; iV < iVnewlength; iV += iCbaseorder) {
			iVx = 0;
			memcpy (bVpointer, &sAnsistring.c_str () [iV], Min (iCbaseorder, iVnewlength - iV));
			cVa.push_back(cVendian.asc_i (iVx));
		}
	}

	/*BINt 2 Ansistsring*/
	AnsiString CoreBint::bin2a () {
		AnsiString sVreturn;
		int iVx;
		CoreRvrs cVendian;
		char* bVpointer;

		sVreturn = "";
		bVpointer = &((char*) &iVx) [sizeof (int) - iCbaseorder];
    for (int iV = 0; iV < (int) cVa.size(); iV++) {
			iVx = cVendian.asc_i (int (cVa [iV]));
			sVreturn += AnsiString (bVpointer, iCbaseorder + 1);
		}
		return sVreturn;
	}

	/*WRITE*/
	AnsiString CoreBint::write () {
		AnsiString sVreturn;
		AnsiString sVtemp;
		char bVbuffer [3];
		int iVoffset;
		CoreRvrs cVendian;

    sVreturn = "";
		iVoffset = sizeof (int) - iCbaseorder;
    for (int iV = 0; iV < (int) cVa.size(); iV++) {
			sVtemp = CoreButl::int2s (cVendian.asc_i (int (cVa [iV])));
			for (int iVinner = sizeof (int) - iCbaseorder; iVinner < sizeof (int); iVinner++) {
				itoa ((unsigned char) sVtemp.c_str () [iVinner], bVbuffer, 16);
				sVreturn += CoreButl::lpad_ (bVbuffer, '0', 2);
			}
		}
		return sVreturn;
	}

	/*READ*/
	void CoreBint::read_(AnsiString sAnsistring) {
		AnsiString sVtemp;
		AnsiString sVansistring;
		AnsiString sVaccumulator;
		CoreRvrs cVendian;

		bVoverflow = false;
		sVaccumulator = "";
		sVansistring = sAnsistring;
		for (int iV = 0; iV < sVansistring.Length (); iV += 2) {
			sVtemp = sVansistring.SubString (iV + 1, 2);
			sVaccumulator = sVaccumulator + sVtemp;
			if (((iV + 2) % (iCbaseorder * 2)) == 0) {
				cVa.push_back ((unsigned int) strtoul (sVaccumulator.c_str (), NULL, 16));
				sVaccumulator = "";
			}
		}
		trim_ ();
	}

	/*DEBUG*/
	AnsiString CoreBint::debug () {
		AnsiString sVreturn;
		AnsiString sVtemp;
		int iVtemp;
		CoreRvrs cVendian;

		sVreturn = "";
		for (unsigned int iV = 0; iV < cVa.size (); iV++) {
			iVtemp = int (cVa [iV]);
			sVtemp = CoreButl::int2s (cVendian.asc_i (iVtemp));
			sVreturn += (AnsiString) iVtemp  + (AnsiString) " -> " + sVtemp.debug () + (AnsiString) " | ";
		}
		return sVreturn;
	}

	/*IS OVerFlow*/
	bool CoreBint::isovf () {
		bool bVreturn;

		bVreturn = bVoverflow;
		return bVreturn;
	}

