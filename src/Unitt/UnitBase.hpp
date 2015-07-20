#include "CoreVcl_.hpp"
#include "gtest/gtest.h"
#include "CorePool.hpp"
#include "CoreSmph.hpp"
#include "CoreStix.hpp"
#include "CoreBlm_.hpp"
#include "re2/re2.h"
#include "CoreHsrx.hpp"
#include "CoreMetb.hpp"
#include "PrtoAlog.pb.h"
#include "CoreXml_.hpp"
#include "CoreHutl.hpp"
#include "CoreEnpo.hpp"
#include "CoreOrn_.hpp"
#include "md5.h"
#include "CorePart.hpp"

#include "CoreFlwr.hpp"
#include "CoreMpfl.hpp"
#include "CoreMrfl.hpp"
#include "CoreRlsw.hpp"
#include "CoreRmwr.hpp"
#include "CoreRmrd.hpp"
#include "CoreGowr.hpp"
#include "CoreHhwr.hpp"
#include "CoreRrwr.hpp"

TEST (CaseCoreAnsx, Base) {
	AnsiString sVtestnull = NULL;
	ASSERT_TRUE (sVtestnull == NULL);
	AnsiString sVtestvoid = "";
	ASSERT_TRUE (sVtestvoid == "");
}

TEST (CaseCoreAnsx, SimpleCompare) {
	AnsiString sVstring1 = "test1";
	AnsiString sVstring2 = "test2";
	ASSERT_TRUE (sVstring1 < sVstring2); 
	AnsiString sVstring3 = sVstring2;
	AnsiString sVstring4 = sVstring2;
	sVstring3 += char (0);
	sVstring3 += char (10);
	sVstring4 += char (0);
	sVstring4 += char (1);
	ASSERT_TRUE (sVstring3 > sVstring4); 
}

TEST (CaseCoreAnsx, SimpleEdit) {
	AnsiString sVtest = "testolungo ";
	ASSERT_TRUE (sVtest.Length () == 11); 
	ASSERT_TRUE (sVtest.SubString (1,5) == "testo"); 
	ASSERT_TRUE (sVtest.Pos ("lungo") == 6); 
	ASSERT_TRUE (sVtest != "testolungo"); 
	ASSERT_TRUE (sVtest.Trim () == "testolungo"); 
	ASSERT_TRUE (sVtest.UpperCase () == "TESTOLUNGO "); 
	ASSERT_TRUE (sVtest.LowerCase () == sVtest); 
}

TEST (CaseCoreAnsx, SimpleConversion) {
	AnsiString sVinteger = "50";
	ASSERT_TRUE (sVinteger.ToInt () == 50);
	AnsiString sVdouble;
	sVdouble.sprintf ("%f", 50.2);
	ASSERT_TRUE (sVdouble.ToDouble () == 50.2);
	std::string sVstdtest = "test" + char (0) + char (10);
	AnsiString sVansistringtest = "test" + char (0) + char (10);
	ASSERT_TRUE (sVstdtest == sVansistringtest.to_string ());
	ASSERT_TRUE ((AnsiString) sVstdtest == sVansistringtest);
	AnsiString sVfloattostr = AnsiString::_FloatToStrF (15.57, 0, 2, 5);
	ASSERT_TRUE (sVfloattostr == "15.57000");
}

#define iCstresspool 100
TEST (CaseCorePool, Base) {
	CorePool cVpool;
	int iV;
	void* vVbuffer [iCstresspool];
	for (iV = 0; iV < iCstresspool; iV++) {
		vVbuffer [iV] = cVpool.mallc (CorePool::iCchar, 100, "TEST", "");
		ASSERT_TRUE (vVbuffer [iV] != NULL);
	}
	for (iV = 0; iV < iCstresspool; iV++) cVpool.free_ (vVbuffer [iV]);
}

TEST (CaseCoreSmph, Base) {
	CoreSmph cVmutex;
	cVmutex.locks ();
	ASSERT_TRUE (!cVmutex.locka ());
	cVmutex.relse ();
	ASSERT_TRUE (cVmutex.locka ());
	cVmutex.relse ();
}

TEST (CaseCoreStix, Base) {
	PTR_POOL cVpool (new CorePool ());
	CoreStix cVsortedindex (cVpool);
	cVsortedindex.add__ ("PROVA", 0, 5);
	cVsortedindex.add__ ("PROVAB", 0, 5);
	cVsortedindex.add__ ("PR", 0, 2);
	cVsortedindex.add__ ("PR", 0, 2);
	cVsortedindex.add__ ("PRO", 0, 2);
	cVsortedindex.add__ ("P", 0, 1);
	ASSERT_TRUE (cVsortedindex.get__ (0) == "P");
	ASSERT_TRUE (cVsortedindex.get__ (1) == "PR");
	ASSERT_TRUE (cVsortedindex.get__ (2) == "PR");
	ASSERT_TRUE (cVsortedindex.get__ (3) == "PRO");
	ASSERT_TRUE (cVsortedindex.get__ (4) == "PROVA");
	ASSERT_TRUE (cVsortedindex.get__ (5) == "PROVAB");
}

TEST (CaseCoreBlm_, Base) {
	CorePool cVpool;
	CoreBlm_ cVbloom (&cVpool, 1024*1024, 67);
	cVbloom.add__ ("test mio");
	cVbloom.add__ ("test tuo");
	ASSERT_TRUE (cVbloom.exst_ ("test mio"));
	ASSERT_TRUE (cVbloom.exst_ ("test tuo"));
}

TEST (CaseRe2, Base) {
		ASSERT_TRUE (RE2::FullMatch("hello", "h.*o"));
}


TEST (CaseCoreHsrx, Base) {
	PTR_POOL cVpool (new CorePool ());
	PTR_CUSTOMSORTEDINDEX cVindex (new BASE_CUSTOMSORTEDINDEX (cVpool));
	CoreHsrx cVhashradix (cVpool);
	cVindex->add__ ("ancora");
	cVindex->add__ ("ancorato");
	cVindex->add__ ("astio");
	cVindex->add__ ("asti");
	cVindex->add__ ("astino");
	cVindex->add__ ("asolfo");
	cVindex->add__ ("astroterrestre");
	cVindex->add__ ("astucci");
	cVindex->add__ ("astribia");
	cVindex->add__ ("vercelli");
	cVindex->add__ ("vercellino");
	cVindex->add__ ("asioso");
	cVindex->add__ ("vucumprà");
	cVindex->add__ ("vento");	
	cVindex->add__ ("vernicello");
	cVhashradix.build (cVindex);
	ASSERT_TRUE (cVhashradix.query ("ancora") == 0);
	ASSERT_TRUE (cVhashradix.query ("astina") == 4);
	ASSERT_TRUE (cVhashradix.query ("astribolo") == 7);
	ASSERT_TRUE (cVhashradix.query ("vucumprà") == 14);
}

TEST (CaseCoreMetb, Base) {
	PTR_POOL cVpool (new CorePool ());
	CoreMetb cVmemtable (cVpool, "", 100, 10000, 100);	
	PrtoLval cVvalue;
	cVvalue.set_ivtimestamp (0);
	cVvalue.set_svopaquevalue ("1");
	cVmemtable.put__ ("amico\n", -1, &cVvalue);
	cVvalue.set_svopaquevalue ("2");
	cVmemtable.put__ ("amico\n", -1, &cVvalue);
	cVvalue.set_svopaquevalue ("3");
	cVmemtable.put__ ("test2\n", 4, &cVvalue);
	cVvalue.set_svopaquevalue ("4");
	cVmemtable.put__ ("testone\n", 6, &cVvalue);
	cVvalue.set_svopaquevalue ("5");
	cVmemtable.put__ ("amica\n", -1, &cVvalue);
	cVvalue.set_svopaquevalue ("6");
	cVmemtable.put__ ("test3\n", 4, &cVvalue);
	cVvalue.set_svopaquevalue ("7");
	cVmemtable.put__ ("test1\n", 4, &cVvalue);
	AnsiString sVtest = "test";
	cVmemtable.get__ (sVtest, &cVvalue);
	ASSERT_TRUE (cVmemtable.getsd () == 20);
	ASSERT_TRUE (cVvalue.svopaquevalue () == "7");
}

TEST (CaseCoreXml_, Base) {
	CoreXml_ cVdocument;
	cVdocument.getfc ("  <LIST>   <A>    1    </A>   <B>    2   </B>   </LIST>  ");
	ASSERT_TRUE (cVdocument.settc (false) == "<LIST><A> 1 </A><B> 2 </B></LIST>");
}

TEST (CaseCoreHutl, Base) {
	PTR_POOL cVpool (new CorePool ());
	CoreList<AnsiString> cVfilenames;
	CoreStix cVsortedfilenames (cVpool);		
	CoreHutl::readr (CoreButl::ffpen ((AnsiString) sCunittestpath + (AnsiString) "\\Test"), "file*.txt", cVfilenames);
	for (int iV = 0; iV < cVfilenames.count (); iV++) cVsortedfilenames.add__ (*cVfilenames.get__ (iV));
	ASSERT_TRUE (cVsortedfilenames.count () == 4);
	ASSERT_TRUE (cVsortedfilenames.get__ (0) == "file1.txt");
	ASSERT_TRUE (cVsortedfilenames.get__ (1) == "file2.txt");
	ASSERT_TRUE (cVsortedfilenames.get__ (2) == "file3.txt");
	ASSERT_TRUE (cVsortedfilenames.get__ (3) == "file5.txt");
}

TEST (CaseCorePrto, Base) {
	PrtoLval cVvalue;
	AnsiString sVtest = "";
	AnsiString sVtestcompare;
	std::string sVencoding;
	for (int iV = 0; iV < 100000; iV++) sVtest += char (iV);
	cVvalue.set_ivtimestamp (0);
	cVvalue.set_svopaquevalue (sVtest.to_string ());
	sVencoding = cVvalue.SerializeAsString ();
	cVvalue.Clear ();
	cVvalue.ParseFromString (sVencoding);
	sVtestcompare = cVvalue.svopaquevalue ();
	ASSERT_TRUE (sVtest == sVtestcompare); 
}

TEST (CaseCoreMd5, Base) {
	ASSERT_TRUE (md5("grape") == "b781cbb29054db12f88f08c6e161c199");
}

TEST (CaseCorePlan, Base) {
	PTR_POOL cVpool (new CorePool ());
	PTR_PLANNER cVplanner;	
	CoreOrn_* cVorion;

	for (int iV = 0; iV < 10; iV++) {
		cVorion = new CoreOrn_ (cVpool, 
			CoreButl::ffpen ((AnsiString) sCunittestpath + (AnsiString) "\\"), "ORION.XML", CoreEnpo::iCrunning);
		cVplanner = cVorion->getpl ();
		delete cVorion;
	}
}

TEST (CaseCorePlan, SedaManager) {
	AnsiString sVtemp;
	PTR_POOL cVpool (new CorePool ());
	CoreOrn_* cVorion;
	CoreSdmg* cVsedamanager;

	cVorion = new CoreOrn_ (cVpool, 
			CoreButl::ffpen ((AnsiString) sCunittestpath + (AnsiString) "\\"), "ORION.XML", CoreEnpo::iCrunning);
	for (int iV = 0; iV < 10000; iV++) {
		cVsedamanager = new CoreSdmg ();
		cVsedamanager->init_ (cVorion->getpl ());
		cVsedamanager->init_<CoreFlwr> (CorePlan::sCflushwriter, 1000, 1);
		cVsedamanager->init_<CoreMpfl> (CorePlan::sCmemtablepostflusher, 1000, 1);
		cVsedamanager->init_<CoreMrfl> (CorePlan::sCmemtablepartial, 1000, 1);
		cVsedamanager->init_<CoreRlsw> (CorePlan::sCredologswitcher, 1000, 1);
		cVsedamanager->init_<CoreGowr> (CorePlan::sCgossipworker, 1000, 1, true);
		cVsedamanager->init_<CoreHhwr> (CorePlan::sChhworker, 1000, 1, true);
		cVsedamanager->init_<CoreRrwr> (CorePeer::sCrrworker, 100, 1);
		for (int iVinner = 0; iV < 30; iV++) {
			sVtemp = (AnsiString) "A" + (AnsiString) iVinner;
			cVsedamanager->init_<CoreRmwr> (sVtemp, 1000, 1, true);
		}
		for (int iVinner = 0; iV < 30; iV++) {
			sVtemp = (AnsiString) "B" + (AnsiString) iVinner;
			cVsedamanager->init_<CoreRmrd> (sVtemp, 1000, 1, true);
		}
		cVsedamanager->stop_ ();
		delete cVsedamanager;
	}
	delete cVorion;
}

TEST (CaseCorePlan, SimpleLifeCycle) {
	PTR_PLANNER cVplanner;
	PrtoL2ct cVcreatetable;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoL2cv* cVcolumnvalue;
	PrtoL2wh* cVwhere;
	PrtoL2cl* cVcondition;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;
	PTR_POOL cVpool (new CorePool ());
	CoreOrn_ cVorion (cVpool,CoreButl::ffpen ((AnsiString) sCunittestpath + (AnsiString) "\\"), "ORION.XML", CoreEnpo::iCrunning);
	cVplanner = cVorion.getpl ();			
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLE");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("TEST");
	cVstatement.mutable_cvkey ()->set_svmain ("CHIAVE");
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("ENTE_KEY");
	cVcolumn->mutable_cvvalue ()->add_svlistvalue ("301");
	cVcolumn->mutable_cvvalue ()->add_svlistvalue ("31");
	cVcolumn->mutable_cvvalue ()->add_svlistvalue ("302");
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("CODICE_FISCALE");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("BRDSDR51H11L736O");
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("PACKED");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("PACKED AND RANDOM TEXT");
	cVplanner->statm (&cVstatement);
	cVquery.mutable_cvmutable ()->set_svtable ("TABLE");
	cVquery.mutable_cvmutable ()->set_svnamespace ("TEST");
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("CODICE_FISCALE"); 		
	cVwhere = cVquery.mutable_cvwhere ();
	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("CODICE_FISCALE");
	cVcondition->set_ivconditiontype (ECOND);
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (LSTRNGTYPE);
	cVcolumnvalue->add_svlistvalue ("BRDSDR51H11L736O");
	cVcolumnvalue->add_svlistvalue ("BRDSDR51H11L73");
	cVcolumnvalue->add_svlistvalue ("BRDSDR51H11L734");
	ASSERT_TRUE (cVplanner->query (&cVquery, &cVdmlresult));
	ASSERT_TRUE (cVdmlresult.cvkeyslice_size () == 1);
	ASSERT_TRUE (cVdmlresult.mutable_cvkeyslice (0)->mutable_cvkey ()->svmain () == "CHIAVE");	
}


TEST (CaseCoreParc, Base) {
	AnsiString sVleft = "benedettaparodinonsacucinare";
	AnsiString sVright = "fortunatamentehafinito";
	AnsiString sVhex;
	ASSERT_TRUE (CorePart::gethx (sVleft) ==	"62656e6564657474617061726f64696e6f6e7361637563696e617265");
	ASSERT_TRUE (CorePart::gethx (sVright) == "666f7274756e6174616d656e7465686166696e69746f0000");
	CorePart::split (sVleft, sVright, 3, 8, sVhex);
	ASSERT_TRUE (sVhex == "63e92feb0ac8cd54616f42f0f144c9098c0c918469d2fe21e4fce77f");
}

void delpt (AnsiString sPath) {
	CoreList<AnsiString> cVfilenames;
	CoreList<AnsiString> cVfilenames2;
	AnsiString sVpath;

	sVpath = CoreButl::ffpen (sPath);
	CoreHutl::readr (sVpath, "*", cVfilenames);
	for (int iV = 0; iV < cVfilenames.count (); iV++) 
		CoreXutl::delfl((CoreButl::ffpen ((AnsiString) sVpath + (AnsiString) "\\") + *cVfilenames.get__ (iV)).c_str ());			
}
 
TEST (CaseCorePlan, StressShutdownCycle) {
	AnsiString sVkey;	
	PTR_POOL cVpool (new CorePool ());
	PTR_PLANNER cVplanner;	
	CoreOrn_* cVorion;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoTabl cVtablet;
	int iVtemp;
	int iVouterdim = 5;
	int iVdim = 30;

	delpt ((AnsiString) sCunittestpath + (AnsiString) "\\Redologs");
	delpt ((AnsiString) sCunittestpath + (AnsiString) "\\Data\\SYSTEM");
	delpt ((AnsiString) sCunittestpath + (AnsiString) "\\Data\\TEST");
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLEMEMORY");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("TEST");
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("KEY");
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	for (int iVouter = 0; iVouter < iVouterdim; iVouter++) {
		cVorion = new CoreOrn_ (cVpool, 
				CoreButl::ffpen ((AnsiString) sCunittestpath + (AnsiString) "\\"), "ORION.XML", CoreEnpo::iCrunning);
		cVplanner = cVorion->getpl ();
		for (int iV = 0; iV < iVdim; iV++) {
			iVtemp = iVouter * iVdim + iV;
			sVkey = (AnsiString) iVtemp;
			cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
			cVstatement.mutable_cvkey ()->set_svmain (sVkey.to_string ());
			cVcolumn->mutable_cvvalue ()->set_svvalue (sVkey.to_string ());
			cVplanner->statm (&cVstatement);		
		}
		cVplanner->iftbl (cVstatement.mutable_cvmutable (), &cVtablet);
		ASSERT_TRUE (int (cVtablet.mutable_cvtabletelement (0)->ivcount ()) == (iVtemp + 1));
		delete cVorion;
	}
}

TEST (CaseCorePlan, StressShutdownAbortCycle) {
	AnsiString sVkey;	
	PTR_POOL cVpool (new CorePool ());
	PTR_PLANNER cVplanner;	
	CoreOrn_* cVorion;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoTabl cVtablet;
	int iVtemp;
	int iVouterdim = 5;
	int iVdim = 300;
	int iVaborts = 2;

	delpt ((AnsiString) sCunittestpath + (AnsiString) "\\Redologs");
	delpt ((AnsiString) sCunittestpath + (AnsiString) "\\Data\\SYSTEM");
	delpt ((AnsiString) sCunittestpath + (AnsiString) "\\Data\\TEST");
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLEMEMORY");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("TEST");
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("KEY");
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	for (int iVouter = 0; iVouter < iVouterdim; iVouter++) {
		cVorion = new CoreOrn_ (cVpool, 
		CoreButl::ffpen ((AnsiString) sCunittestpath + (AnsiString) "\\"), "ORION.XML", CoreEnpo::iCrunning);
		cVplanner = cVorion->getpl ();
		for (int iV = 0; iV < iVdim; iV++) {
			iVtemp = iVouter * iVdim + iV;
			sVkey = (AnsiString) iVtemp;
			cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
			cVstatement.mutable_cvkey ()->set_svmain (sVkey.to_string ());
			cVcolumn->mutable_cvvalue ()->set_svvalue (sVkey.to_string ());
			cVplanner->statm (&cVstatement);		
		}
		cVplanner->iftbl (cVstatement.mutable_cvmutable (), &cVtablet);
		ASSERT_TRUE (int (cVtablet.mutable_cvtabletelement (0)->ivcount ()) == (iVtemp + 1));
		if (iVouter % iVaborts == 0) cVplanner->setab (true);
		delete cVorion;
	}
}

TEST (CaseCorePlan,RawInsert) {
	CoreOrn_* cVorion;
	PTR_POOL cVpool (new CorePool ());
	PTR_PLANNER cVplanner;	
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	AnsiString sVvalue;

	delpt ((AnsiString) sCunittestpath + (AnsiString) "\\Redologs");
	delpt ((AnsiString) sCunittestpath + (AnsiString) "\\Data\\NAMESPACE");
	delpt ((AnsiString) sCunittestpath + (AnsiString) "\\Data\\SYSTEM");
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLEUPDATE");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVstatement.mutable_cvkey ()->set_svmain ("CHIAVE");
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("FIELD");
	cVorion = new CoreOrn_ (cVpool, 
		CoreButl::ffpen ((AnsiString) sCunittestpath + (AnsiString) "\\"), "ORION.XML", CoreEnpo::iCrunning);
	cVplanner = cVorion->getpl ();

	for (int iV = 0; iV < 1000000; iV++) {
		sVvalue = md5 (((AnsiString) iV).to_string ());
		cVcolumn->mutable_cvvalue ()->set_svvalue (sVvalue.to_string ());
		cVplanner->statm (&cVstatement);
		if (iV %50000 == 0) printf ("ciclo %i\n", iV);
	}

	delete cVorion;
}

