#include "CoreAenp.hpp"
#include "CoreMktr.hpp"
#include "CoreAewk.hpp"

using namespace google;

//#define iCdebugrepair

	const int CoreAenp::iCchunkdim = 10000;
	const int CoreAenp::iCchunksize = 5 * 1024 * 1024;

  __fastcall CoreAenp::CoreAenp(CorePlan* cPlan) {
		
		cVplanner = cPlan;
		cVpeer = cVplanner->getpr ();
		cVredolog = cVpeer->getal ();
		cVgossiper = cVpeer->getgo ();
	}

  __fastcall CoreAenp::~CoreAenp () {

		cVgossiper.reset ();
		cVredolog.reset ();
		cVpeer.reset ();
	}

  /*RePAIR*/
	bool __fastcall CoreAenp::rpair (AnsiString sNodeid, bool bTable, bool bIndex, AnsiString sIndextable, CoreEnpo::GOSSIPLOC* cGossipelement) {
		AnsiString sVindex;
		AnsiString sVkeystart;
		AnsiString sVkeyend;
		AnsiString sVthisnodeid = cVpeer->getni ();
		int iVlimitsize;
		PrtoLmtb cVmutable;
		PrtoMktr cVprtomerkeltreesource;
		PrtoMktr cVprtomerkeltreedestination;
		CoreMktr cVmerkeltree (	cVredolog, cVpeer->getpt (), cVpeer->getep (), sVthisnodeid);
		CorePlan::PTR_TABLELOC cVtableloc;
		CoreEnpo::GOSSIPLOC cVgossipelementstatic;
		CoreEnpo::GOSSIPLOC* cVgossipelement;
		CorePart::PARTITIONLOC* cVpartitionloc;

		sVindex = "";
		if (cGossipelement == NULL) {
			if (!cVgossiper->getge (sNodeid, cVgossipelementstatic)) return false;
			cVgossipelement = &cVgossipelementstatic;
		} else {
			cVgossipelement = cGossipelement;
			sNodeid = cVgossipelement->sVindex;
		}
		while (true) {
			cVtableloc = cVplanner->gettl (sVindex);
			if (cVtableloc == NULL) break;
			cVpartitionloc = &cVtableloc->cVpartitionloc;
			sVindex = cVtableloc->sVindex;
			if (sIndextable != NULL && sIndextable != sVindex) continue;
			CoreAlog::settk (sVindex, &cVmutable);

#ifdef iCdebugrepair
			printf ("table %s\n", cVmutable.svtable ().c_str ());
#endif

			for (int iV = 0; iV < 2; iV++) {				
				sVkeyend = "";
				if ((iV == 0 && bTable) || (iV == 1 && bIndex))
					while (cVmerkeltree.build (true, sNodeid, &cVmutable, cVpartitionloc, sVkeyend, NULL, iCchunkdim, iCchunksize)) {

						LOG(INFO) <<	"Repair:    " << sNodeid.c_str () << ", " << cVmutable.svtable ().c_str ();
						FlushLogFiles(GLOG_INFO);

						cVprtomerkeltreesource.Clear ();
						cVprtomerkeltreedestination.Clear ();
						sVkeystart = sVkeyend;
						sVkeyend = cVmerkeltree.getlm ();
						iVlimitsize = cVmerkeltree.getdm ();
						cVmerkeltree.pushs (&cVprtomerkeltreesource);
				
#ifdef iCdebugrepair
						printf ("I1 %s %i\n", cVprtomerkeltreesource.svhash ().c_str (), cVprtomerkeltreesource.ByteSize ());	
#endif

						LOG(INFO) <<	"Rep send:  " << cVprtomerkeltreesource.svhash ().c_str () << 
													", " << cVprtomerkeltreesource.ByteSize ();
						FlushLogFiles(GLOG_INFO);

						if (!CoreAewk::ppllm (	cVgossipelement, 
																		&cVprtomerkeltreesource, &cVprtomerkeltreedestination, 
																		sVthisnodeid, &cVmutable, sVkeystart, iVlimitsize)) return false;
				
#ifdef iCdebugrepair
						printf ("R1 %s %i\n", cVprtomerkeltreedestination.svhash ().c_str (), cVprtomerkeltreedestination.ByteSize ());
#endif
	
						LOG(INFO) <<	"Rep recv:  " << cVprtomerkeltreedestination.svhash ().c_str () << 
													", " << cVprtomerkeltreedestination.ByteSize ();
						FlushLogFiles(GLOG_INFO);

						if (!cVmerkeltree.pulls (&cVprtomerkeltreedestination, &cVprtomerkeltreesource)) continue;

#ifdef iCdebugrepair
						printf ("I2 %s %i\n", cVprtomerkeltreesource.svhash ().c_str (), cVprtomerkeltreesource.ByteSize ());
#endif				

						LOG(INFO) <<	"Rep fix:   " << cVprtomerkeltreesource.svhash ().c_str () << 
													", " << cVprtomerkeltreesource.ByteSize ();
						FlushLogFiles(GLOG_INFO);

						if (!CoreAewk::ppllr (	cVgossipelement, &cVprtomerkeltreesource, &cVmutable)) return false;
					}
				CoreAlog::tb2ix (&cVmutable);
			}
		}
		return true;
	}

	/*Push Pull MatCH*/
	bool __fastcall CoreAenp::ppmch (PrtoAenp* cAntientrophy) {
		PrtoLmtb cVmutable;
		PrtoMktr cVprtomerkeltreesource;
		PrtoMktr cVprtomerkeltreedestination;
		CoreMktr cVmerkeltree (	cVredolog, cVpeer->getpt (), cVpeer->getep (), cAntientrophy->svnodeid ()); 
		CorePlan::PTR_TABLELOC cVtableloc;

		cVmutable.CopyFrom (cAntientrophy->cvmutable ());
		CoreAlog::ix2tb (&cVmutable);
		cVtableloc = cVplanner->gettl (&cVmutable);
		if (cVtableloc == NULL) return false;
		if (cVmerkeltree.build (false, cVpeer->getni (), 
														cAntientrophy->mutable_cvmutable (), 
														&cVtableloc->cVpartitionloc,
														cAntientrophy->svkeystart (), NULL, cAntientrophy->ivlimitsize ())) {
			cVprtomerkeltreesource.ParseFromString (*cAntientrophy->mutable_svopaquedata ());
			cVmerkeltree.pushd (&cVprtomerkeltreesource, &cVprtomerkeltreedestination);

			LOG(INFO) <<	
					"Ppull mtc: " << cVprtomerkeltreedestination.svhash ().c_str () << ", " << 
					cVmerkeltree.getdm () << ", " << cAntientrophy->ivlimitsize ();
			FlushLogFiles(GLOG_INFO);

		} else {
			cVprtomerkeltreedestination.set_svhash ("");	
			cVprtomerkeltreedestination.set_bvleaf (true);
		}
		cAntientrophy->set_svopaquedata (cVprtomerkeltreedestination.SerializeAsString ());
		return true;
	}

	/*Push Pull REPair*/
	bool __fastcall CoreAenp::pprep (PrtoAenp* cAntientrophy) {
		PrtoMktr cVprtomerkeltree;
		PrtoLmtb cVmutable;
		CoreMktr cVmerkeltree (	cVredolog, cVpeer->getpt (), cVpeer->getep (), cAntientrophy->svnodeid ()); 			
		CorePlan::PTR_TABLELOC cVtableloc;

		cVmutable.CopyFrom (cAntientrophy->cvmutable ());
		CoreAlog::ix2tb (&cVmutable);
		cVtableloc = cVplanner->gettl (&cVmutable);
		if (cVtableloc == NULL) return false;
		cVmerkeltree.build (cAntientrophy->mutable_cvmutable (), &cVtableloc->cVpartitionloc);
		cVprtomerkeltree.ParseFromString (*cAntientrophy->mutable_svopaquedata ());
		
		LOG(INFO) <<	"Ppull rep: " << cVprtomerkeltree.svhash ().c_str ();
		FlushLogFiles(GLOG_INFO);

		return cVmerkeltree.pulld (&cVprtomerkeltree, NULL);
	}

	/*PURGE*/
	bool __fastcall CoreAenp::purge (PrtoLmtb* cMutable, CorePart::PARTITIONLOC* cPartitionloc) {
		bool bVreturn;
		PrtoLmtb cVmutable;
		CoreMktr cVmerkeltree (	cVredolog, cVpeer->getpt (), cVpeer->getep (), cVpeer->getep ()->getni ()); 	

		bVreturn = true;
		cVmutable.CopyFrom (*cMutable);
		bVreturn &= cVmerkeltree.purge (&cVmutable, cPartitionloc);
		cVmutable.CopyFrom (*cMutable);
		CoreAlog::tb2ix (&cVmutable);
		bVreturn &= cVmerkeltree.purge (&cVmutable, cPartitionloc);
		return bVreturn;
	}

