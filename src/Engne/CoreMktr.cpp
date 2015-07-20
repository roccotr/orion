#include "CoreMktr.hpp"
#include "CorePars.hpp"
#include "TmplList.cpp"
#include "md5.h"

using namespace google;

//#define iCdebugapply
//#define iCdebugpullmh
//#define iCdebugsend

  __fastcall CoreMktr::CoreMktr (	PTR_REDOLOG cRedolog, 
																	PTR_PARTITIONER cPartitioner, PTR_ENDPOINT cEndpoint,
																	AnsiString sSourcenode, int iLeafdim) {
		
		cVredolog = cRedolog;
		cVpartitioner = cPartitioner;
		cVendpoint = cEndpoint;
		sVsourcenode = sSourcenode;
		iVleafdim = iLeafdim;
		bVmaxsizecheck = false;
		cVpartitionloc = NULL;
		cVreverse = boost::shared_ptr<CoreRvrs> (new CoreRvrs ());
	}


  __fastcall CoreMktr::~CoreMktr () {

	}

	  /*BUILD*/
	void __fastcall CoreMktr::bmkin (NODEHASHLOC* cNode, PrtoMktr* cPrtonode, int iLevel) {
		
		cPrtonode->set_svhash (cNode->sVhash.to_string ());
		cPrtonode->set_bvleaf (cNode->cVchildrenpool.count () == 0);
		cPrtonode->set_ivmerkeltreedataposition (cNode->iVmerkeltreedataposition);
		for (int iV = 0; iV < cNode->cVchildrenpool.count (); iV++) {
			bmkin (*cNode->cVchildrenpool.get__ (iV), cPrtonode->add_cvchildren (), iLevel + 1);		
		}
	}

	/*Build MerKel INternal*/
	bool __fastcall CoreMktr::bmkin (int iLeafdim) {
		int iVleafcount;
		AnsiString sVtemp;
		AnsiString sVhashkey;
		AnsiString sVhashvalue;
		NODEDATALOC* cVnodedataloc;
		LEVELHASHLOC* cVlevelhashloc;
		LEVELHASHLOC* cVfatherlevelhashloc;
		NODEHASHLOC* cVnodehashloc;
		NODEHASHLOC* cVfathernodehashloc;
		CoreList<LEVELHASHLOC> cVtemptree;

		cVlevelhashloc = cVtemptree.set__ ();
		cVfatherlevelhashloc = cVtemptree.set__ ();
		iVleafcount = 0;
		for (int iV = 0; iV < cVmerkeltreedata.count (); iV++) {			
			cVnodedataloc = cVmerkeltreedata.get__ (iV);			
			sVhashkey = md5 (cVnodedataloc->cVkey.SerializeAsString ());
			sVhashvalue = md5 (cVnodedataloc->cVvalue.SerializeAsString ());
			cVnodehashloc = cVlevelhashloc->cVlevel.set__ ();
			cVnodehashloc->bVleaf = true;
			cVnodehashloc->iVmerkeltreedataposition = iV;
			cVnodehashloc->sVhash = md5 ((sVhashkey + sVhashvalue).to_string ());					
			if (iVleafcount == 0) cVfathernodehashloc = cVfatherlevelhashloc->cVlevel.set__ ();
			cVnodehashloc->cVfather = cVfathernodehashloc;
			*cVfathernodehashloc->cVchildrenpool.set__ () = cVnodehashloc;
			iVleafcount = (iVleafcount + 1) % iLeafdim;
		}
		while (true) {
			cVlevelhashloc = cVfatherlevelhashloc;
			cVfatherlevelhashloc = cVtemptree.set__ ();
			iVleafcount = 0;
			for (int iV = 0; iV < cVlevelhashloc->cVlevel.count (); iV++) {
				cVnodehashloc = cVlevelhashloc->cVlevel.get__ (iV);
				sVtemp = "";
				for (int iVinner = 0; iVinner < cVnodehashloc->cVchildrenpool.count (); iVinner++) 
					sVtemp += (*cVnodehashloc->cVchildrenpool.get__ (iVinner))->sVhash;
				cVnodehashloc->sVhash = md5 (sVtemp.to_string ());
				if (iVleafcount == 0) cVfathernodehashloc = cVfatherlevelhashloc->cVlevel.set__ ();
				cVnodehashloc->cVfather = cVfathernodehashloc;
				*cVfathernodehashloc->cVchildrenpool.set__ () = cVnodehashloc;
				iVleafcount = (iVleafcount + 1) % 2;
			}
			if (cVlevelhashloc->cVlevel.count () == 1) break;
		}
		cVtemptree.unset (cVtemptree.count () - 1);
		bmkin (cVtemptree.get__ (cVtemptree.count () - 1)->cVlevel.get__ (0), &cVmerkeltreehash);
		return true;
	}

	/*ReBuiLD*/
	bool __fastcall CoreMktr::rbld_ () {
		AnsiString sVtempmain;
		AnsiString sVbackupmain;
		NODEDATALOC* cVnodedataloc;
		CoreEnpo::REPLICATIONDATA cVreplicationdata;
		CoreEnpo::POINTERGOSSIPLOC* cVpointergossiploc;
		CoreEnpo::GOSSIPLOC* cVgossiploc;
		PrtoLkey* cVtempkey;
		int iVsize;
		bool bVdummy;
		bool bVindex;
		bool bVsource;
		bool bVdestination;
		bool bVreturn;

		cVmerkeltreedata.clear ();
		cVmerkeltreehash.Clear ();
		cVnodedataloc = cVmerkeltreedata.set__ ();
		cVnodedataloc->cVkey.set_svmain (sVkeystart.to_string ());
		iVsize = 0;
		sVbackupmain = sVtempmain = NULL;
		bVindex = CoreAlog::index (&cVmutable);
		while (	
			cVredolog->query (&cVmutable, &cVnodedataloc->cVkey, &cVnodedataloc->cVvalue, CoreTble::iCtkslwquery, NULL) && 
																		(sVkeyend == NULL || cVnodedataloc->cVkey.svmain () <= sVkeyend.to_string ())) {
			iVsize += cVnodedataloc->cVvalue.ByteSize ();			
			cVtempkey = &cVnodedataloc->cVkey;
			sVbackupmain = sVtempmain;
			sVtempmain = cVtempkey->svmain ();
			if (sVbackupmain != sVtempmain &&
					((bVmaxsizecheck && iVsize > iVmaxsize) || (cVmerkeltreedata.count () > iVmaxdim))) break;
			cVendpoint->getrp (CoreEnpo::iCwrite, 
				cVpartitioner->desow (
					bVindex ? cVpartitionloc->cVpartitionindex.iVpartitiontype : cVpartitionloc->cVpartition.iVpartitiontype,
					sVtempmain, bVindex, cVpartitionloc), 
					cVreplicationdata, bVdummy, 0, cVpartitionloc->iVcustomreplicationfactor);	
			bVsource = bVdestination = false;
			for (int iV = 0; iV < cVreplicationdata.cVpointergossippool.count (); iV++) {
				cVpointergossiploc = cVreplicationdata.cVpointergossippool.get__ (iV);
				cVgossiploc = cVendpoint->getrg (cVpointergossiploc->iVelement);
				if (cVgossiploc != NULL) {
					if (cVgossiploc->sVindex == sVsourcenode) bVsource = true;
					else if (cVgossiploc->sVindex == sVdestinationnode) bVdestination = true;
				}
			}
			if (bVdestination && (bVsource || bVcheckonlydestination)) {

#ifdef iCdebugsend
				printf ("MKTR %s (%s)\n", cVmutable.svtable ().c_str (), 
					((AnsiString) cVnodedataloc->cVkey.svmain ()).prntb ().c_str ());
#endif

				cVnodedataloc = cVmerkeltreedata.set__ ();
			}
			cVnodedataloc->cVkey = *cVtempkey;
		}	
		cVmerkeltreedata.unset (cVmerkeltreedata.count () - 1);	
		bVreturn = cVmerkeltreedata.count () > 0 ? bmkin (iVleafdim) : false;
		return bVreturn;
	}

  /*BUILD*/
	void __fastcall CoreMktr::build (	PrtoLmtb* cMutable, CorePart::PARTITIONLOC* cPartitionloc) {

		sVdestinationnode = "";
		cVmutable.CopyFrom (*cMutable);
		cVpartitionloc = cPartitionloc;
		sVkeystart = sVkeyend = "";
		iVmaxdim = iVmaxsize = 0;
		bVmaxsizecheck = false;
		bVcheckonlydestination = false;
	}

  /*BUILD*/
	bool __fastcall CoreMktr::build (	bool bCheckonlydestination, AnsiString sDestinationnode, 
																		PrtoLmtb* cMutable, CorePart::PARTITIONLOC* cPartitionloc,
																		AnsiString sKeystart, AnsiString sKeyend, int iMaxdim, int iMaxsize) {

		bVcheckonlydestination = bCheckonlydestination;
		sVdestinationnode = sDestinationnode;
		cVmutable.CopyFrom (*cMutable);
		cVpartitionloc = cPartitionloc;
		sVkeystart = sKeystart;
		sVkeyend = sKeyend;
		iVmaxdim = iMaxdim;
		iVmaxsize = iMaxsize;
		bVmaxsizecheck = iVmaxsize > 0;
		return rbld_ ();
	}

	/*GET DiM*/
	int __fastcall CoreMktr::getdm () {

		return cVmerkeltreedata.count ();
	}

	/*GET Last Main*/
	AnsiString __fastcall CoreMktr::getlm () {
		int iVdummy;

		if (cVmerkeltreedata.count () > 0) 
			//return cVmerkeltreedata.get__ (cVmerkeltreedata.count () - 1)->cVkey.svmain ();
			return CoreTbsv::key__ (cVreverse, &cVmerkeltreedata.get__ (cVmerkeltreedata.count () - 1)->cVkey, iVdummy);
    else return NULL;
	}

	/*GET Merkel tree Hash*/
	PrtoMktr* __fastcall CoreMktr::getmh () {
		return &cVmerkeltreehash;
	}

	/*PuLL Merkel tree Hash*/
	bool __fastcall CoreMktr::pllmh (PrtoMktr* cInput, PrtoMktr* cOutput, PrtoMktr* cCurrent, bool bFix) {
		bool bVfix;
		bool bVmatch;
		bool bVreturn;

		bVreturn = false;
		if (cCurrent == NULL) cCurrent = &cVmerkeltreehash;
		bVmatch = cInput != NULL && cInput->svhash () == cCurrent->svhash ();
		bVfix = bFix || (cInput == NULL);
		cOutput->set_svhash (cCurrent->svhash ());	
		cOutput->set_bvleaf (cCurrent->bvleaf ());
		if (bVmatch) return false;
		if (bVfix && cCurrent->bvleaf ()) {
			cOutput->mutable_cvkey ()->CopyFrom (cVmerkeltreedata.get__ (cCurrent->ivmerkeltreedataposition ())->cVkey);
			cOutput->mutable_cvvalue ()->CopyFrom (cVmerkeltreedata.get__ (cCurrent->ivmerkeltreedataposition ())->cVvalue);
			bVreturn = true;

#ifdef iCdebugpullmh
			int iV;
			printf ("%s\n", CoreTbsv::key__ (new CoreRvrs (), 
				cOutput->mutable_cvkey (), iV).prntb ().c_str ());
#endif

		}
		for (int iV = 0; iV < cCurrent->cvchildren_size (); iV++) {
			bVreturn |= pllmh (	cInput == NULL || cInput->cvchildren_size () <= iV ? NULL : cInput->mutable_cvchildren (iV),
									cOutput->add_cvchildren (), cCurrent->mutable_cvchildren (iV), bVfix);
		}
		return bVreturn;
	}

	/*APPLY*/
	int __fastcall CoreMktr::apply (PrtoMktr* cInput) {
		PrtoLstm cVstatement;
		int iVreturn;

		iVreturn = 0;
		if (cInput->has_cvkey ()) {
			cVstatement.set_ivopcode (MUTATOR);
			cVstatement.mutable_cvmutable ()->CopyFrom (cVmutable);
			cVstatement.mutable_cvkey ()->CopyFrom (*cInput->mutable_cvkey ());
			cVstatement.mutable_cvvalue ()->CopyFrom (*cInput->mutable_cvvalue ());

#ifdef iCdebugapply
			int iV;
			printf ("%s\n", CoreTbsv::key__ (new CoreRvrs (), cVstatement.mutable_cvkey (), iV).prntb ().c_str ());
			printf ("%s\n", CoreTbsv::key__ (new CoreRvrs (), cVstatement.mutable_cvkey (), iV).debug ().c_str ());
#endif

			cVredolog->statm (&cVstatement, NULL);
			iVreturn++;
		}
		for (int iV = 0; iV < cInput->cvchildren_size (); iV++) iVreturn += apply (cInput->mutable_cvchildren (iV));		
		return iVreturn;
	}

	/*PUSH Source*/
	bool __fastcall CoreMktr::pushs (PrtoMktr* cOutput) {

		cOutput->CopyFrom (*getmh ());
		return true;
	}

	/*PUSH Destination*/
	bool __fastcall CoreMktr::pushd (PrtoMktr* cInput, PrtoMktr* cOutput) {

		return pllmh (cInput, cOutput);
	}

	/*PULL Source*/
	bool __fastcall CoreMktr::pulls (PrtoMktr* cInput, PrtoMktr* cOutput) {
		int iVapply;

		iVapply = 0;
		if (cInput != NULL && cInput->cvchildren_size () != 0) {
			iVapply = apply (cInput);
			rbld_ ();
		}

		LOG(INFO) <<	"Push apply:" << cVmutable.svtable ().c_str () << ", " << iVapply;
		FlushLogFiles(GLOG_INFO);

		return pllmh (cInput, cOutput);
	}

	/*PULL Destination*/
	bool __fastcall CoreMktr::pulld (PrtoMktr* cInput, PrtoMktr* cOutput) {
		int iVapply;

		iVapply = 0;
		if (cInput->cvchildren_size () != 0) iVapply = apply (cInput);
		if (cOutput != NULL) {
			rbld_ ();
			return pllmh (cInput, cOutput);
		}

		LOG(INFO) <<	"Pull apply:" << cVmutable.svtable ().c_str () << ", " << iVapply;
		FlushLogFiles(GLOG_INFO);

		return true;
	}

	/*DEBUG*/
	AnsiString __fastcall CoreMktr::debug (PrtoMktr* cInput, int iLevel) {
		AnsiString sVreturn;
		AnsiString sVtemp;
		int iVdummy;
		PTR_REVERSE cVreverse (new CoreRvrs ());

		sVtemp = cInput->bvleaf () ? "L) - " : (AnsiString) iLevel + (AnsiString) ") - ";
		sVreturn += CoreButl::lpad_ (sVtemp, " ", 5 + iLevel * 2) + cInput->svhash () + 
								(AnsiString) (cInput->bvleaf () && cInput->has_cvkey () ? 
								(AnsiString) " - " + CoreTbsv::key__ (cVreverse, cInput->mutable_cvkey (), iVdummy).prntb () : "") + 
								(AnsiString) "\n";
		for (int iV = 0; iV < cInput->cvchildren_size (); iV++) 	
			sVreturn += debug (cInput->mutable_cvchildren (iV), iLevel + 1);		
		return sVreturn;
	}

	/*PURGE*/
	bool __fastcall CoreMktr::purge (PrtoLmtb* cMutable, CorePart::PARTITIONLOC* cPartitionloc) {
		AnsiString sVtempmain;
		CoreEnpo::REPLICATIONDATA cVreplicationdata;
		CoreEnpo::POINTERGOSSIPLOC* cVpointergossiploc;
		CoreEnpo::GOSSIPLOC* cVgossiploc;
		PrtoLstm cVstatement;
		PrtoLkey* cVtempkey;
		int iVfound;
		bool bVdummy;
		PrtoLkey cVkey;
		PrtoLval cVvalue;
		bool bVindex;
		bool bVreturn;		

		cVkey.set_svmain (CorePars::sCminansistring.to_string ());
		sVtempmain = NULL;
		bVindex = CoreAlog::index (cMutable);
		cVstatement.set_ivopcode (MUTATOR);
		(cVstatement.mutable_cvmutable ())->CopyFrom (*cMutable);
		bVreturn = true;
		while (	
			cVredolog->query (cMutable, &cVkey, &cVvalue, CoreTble::iCtkslwquery, NULL) &&
			cVkey.svmain () != CorePars::sCminansistring.to_string ()) {	
			cVtempkey = &cVkey;
			if (cVkey.ivstate () == DELTMB) continue;
			sVtempmain = cVtempkey->svmain ();
			cVendpoint->getrp (CoreEnpo::iCwrite, 
				cVpartitioner->desow (
				bVindex ? cPartitionloc->cVpartitionindex.iVpartitiontype : cPartitionloc->cVpartition.iVpartitiontype, 
				sVtempmain, bVindex, cPartitionloc), 
				cVreplicationdata, bVdummy, 0, cPartitionloc->iVcustomreplicationfactor);	
			iVfound = 0;
			for (int iV = 0; iV < cVreplicationdata.cVpointergossippool.count (); iV++) {
				cVpointergossiploc = cVreplicationdata.cVpointergossippool.get__ (iV);
				cVgossiploc = cVendpoint->getrg (cVpointergossiploc->iVelement);
				if (cVgossiploc != NULL) iVfound += cVgossiploc->sVindex == sVsourcenode ? 1 : 0;
			}
			if (iVfound == 0) {
				cVkey.set_ivstate (DELTMB);		
				(cVstatement.mutable_cvkey ())->CopyFrom (cVkey);
				(cVstatement.mutable_cvvalue ())->CopyFrom (cVvalue);
				bVreturn &= cVredolog->statm (&cVstatement, NULL); 
			}		
		}	
		return bVreturn;
	}