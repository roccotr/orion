#include "CoreHiha.hpp"
#include "CorePart.hpp"
#include "CorePutl.hpp"
#include "TmplList.cpp"

using namespace orion;

	const AnsiString CoreHiha::sChintedhandofftable = "HINTEDHANDOFF";
	const char CoreHiha::bCdelimiter = '|';
	const int CoreHiha::iCdefaultprocess = 1000;

//#define iCdebugprocess

  __fastcall CoreHiha::CoreHiha (PTR_REDOLOG cRedolog, PTR_GOSSIPER cGossiper) {
		
		cVredolog = cRedolog;
		cVgossiper = cGossiper;
		cVtemplatemutable.set_svnamespace (CoreAlog::sCsystemnamespace.to_string ());
		cVtemplatemutable.set_svtable (sChintedhandofftable.to_string ());
	}

  __fastcall CoreHiha::~CoreHiha () {

	}

  /*PUT*/
	bool __fastcall CoreHiha::put__ (AnsiString sNode, PrtoLsts* cStatements, AnsiString sRedotimestamp) {
		bool bVreturn;
		int64vcl iVtimestamp;
		AnsiString sVkey;
		PrtoHiha cVhhopaquevalue;
		PrtoLstm cVnewstatement;
		PrtoLkey* cVkey;
		PrtoLval* cVvalue;

		cVmutex.locks ();
		iVtimestamp = cStatements->mutable_cvliststatement (
				cStatements->cvliststatement ().size () - 1)->mutable_cvkey ()->ivtimestamp ();			
		sVkey = sNode + (AnsiString) bCdelimiter + CoreButl::i642s (cVreverse.asc_l (iVtimestamp));
		cVnewstatement.set_ivopcode (MUTATOR);
		cVkey = cVnewstatement.mutable_cvkey ();
		cVkey->set_svmain (sVkey.to_string ());
		cVkey->set_svaccessgroup ("");
		cVkey->set_svqualifier ("");
		cVkey->set_ivtimestamp (iVtimestamp);
		cVkey->set_ivstate (UPSERT);
		cVnewstatement.mutable_cvmutable ()->CopyFrom (cVtemplatemutable);		
		cVvalue = cVnewstatement.mutable_cvvalue ();
		cVvalue->set_ivtimestamp (iVtimestamp);		
		cVhhopaquevalue.set_svnode (sNode.to_string ());
		cVhhopaquevalue.mutable_cvstatements ()->CopyFrom (*cStatements);					
		cVvalue->set_svopaquevalue (cVhhopaquevalue.SerializeAsString ());		
		bVreturn = cVredolog->statm (&cVnewstatement, NULL);
		cVmutex.relse ();
		return bVreturn;
	}

  /*PRoCeSS*/
	bool __fastcall CoreHiha::prcss (int iDim) {
		int iVdim;
		PrtoLkey cVkey;
		PrtoLval cVvalue;
		AnsiString sVthiskey;
		AnsiString sVlastkey;
		PrtoHiha cVhhopaquevalue;
		PrtoLstm cVnewstatement;
		CoreEnpo::GOSSIPLOC cVgossiploc;

		cVmutex.locks ();
		iVdim = iDim;
		sVlastkey = "";
		cVkey.set_svmain (sVlastkey.to_string ());
		while (cVredolog->query (&cVtemplatemutable, &cVkey, &cVvalue, CoreTble::iCtokenquery, NULL)) {
			sVthiskey = cVkey.svmain ();
			if (sVthiskey <= sVlastkey) break;	
			sVlastkey = sVthiskey;
			cVhhopaquevalue.ParseFromString (cVvalue.svopaquevalue ());
			if (!cVgossiper->getge (cVhhopaquevalue.svnode (), cVgossiploc)) continue;
			if (cVgossiploc.bValive) {
				cVnewstatement.set_ivopcode (MUTATOR);
				cVnewstatement.mutable_cvkey ()->CopyFrom (cVkey);
				cVnewstatement.mutable_cvkey ()->set_ivstate (DELTMB);
				cVnewstatement.mutable_cvmutable ()->CopyFrom (cVtemplatemutable);

#ifdef iCdebugprocess
				printf ("%s %s\n", sVthiskey.prntb ().c_str (), cVhhopaquevalue.svnode ().c_str ());
#endif			

				if (CorePutl::sends (	cVgossiploc.cVconnectionfactory,
															cVhhopaquevalue.mutable_cvstatements ())) cVredolog->statm (&cVnewstatement, NULL);
				if ((--iVdim) == 0) break;
			} 	
		}
		cVmutex.relse ();
		return true;
	}
