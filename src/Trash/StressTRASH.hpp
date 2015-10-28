


//CAZZO


//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////--------------------------------------------------------------------------------
//
///*
//int _tmain(int argc, _TCHAR* argv[]) {	
//	AnsiString sVsubversion;
//	TDateTime tVstart;
//  TDateTime tVend;
//  double tVdiff;  
//
//	tVdiff = 24*3600;
//	if (sizeof (void*) == 8) sVsubversion = "x64"; 
//	else sVsubversion = "x86";
//  printf ("Stress %s\n", sVsubversion.c_str ());
// 	  
//	FLAGS_log_dir = "c:/templog";
//	google::InitGoogleLogging(argv[0]);
//
//	
//	#define iVdim 4
//	#define iVwrites 100000
//	*/
//
////velocita
//	/*
//	CoreOo2w<TESTOO2, iVdim> cVtest;    
//	CoreList<TESTOO2> cVtestcrash;
//
//	printf ("velocità %i\n", iVdim);
//	tVstart = __Time (); 
//	for (int iV = 0; iV < iVwrites; iV++) cVtest.bnset ((AnsiString) iV)->iVvalue = iV;    
//	tVend = __Time (); 
//	printf ("%i scritture forward:      %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));
//
//	tVstart = __Time ();  
//	for (int iV = 0; iV <= iVwrites; iV++)  cVtest.bnuns ((AnsiString) iV);
//	tVend = __Time (); 
//	printf ("%i cancellazioni forward:  %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));
//	 
//
//	tVstart = __Time (); 
//	for (int iV = iVwrites - 1; iV >= 0; iV--) cVtest.bnset ((AnsiString) iV)->iVvalue = iV;    
//	tVend = __Time (); 
//	printf ("%i scritture reverse:      %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));
//
//	tVstart = __Time ();  
//	for (int iV = iVwrites - 1; iV >= 0; iV--)  cVtest.bnuns ((AnsiString) iV );
//	tVend = __Time (); 
//	printf ("%i cancellazioni reverse:  %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));
//
//
//	for (int iV = 0; iV < iVwrites; iV++) cVtest.bnset (CoreButl::lpad_ ((AnsiString) iV, '0', 6))->iVvalue = iV;
//	CoreOo2w<TESTOO2,32>::OO2POS iV = cVtest.first (); 
//	int iVcycle = 0;
//	for (;;iV++) {
//		int iVtest = cVtest.get__ (iV)->iVvalue;
//		if ((iVcycle != iVtest) || (iVtest == iVwrites - 1)) break; 
//		iVcycle++;
//	}
//	if (iVcycle != iVwrites - 1) printf ("risultato atteso:          %i\n", iVcycle);
//	iV = cVtest.first (); 
//	iVcycle = 0;
//	tVstart = __Time ();  
//	for (;iV < cVtest.last_ ();iV++) {
//		int iVtest = cVtest.get__ (iV)->iVvalue;
//		if (iVtest == 99999) break;
//		iVcycle++;
//	}
//	tVend = __Time (); 
//	printf ("%i ricerche range:         %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));
//	for (int iV = 0; iV < iVwrites; iV++) cVtest.bnuns (CoreButl::lpad_ ((AnsiString) iV, '0', 6));
//
//	for (int iV = 0; iV < iVwrites; iV++) cVtest.bnset ((AnsiString) iV)->iVvalue = iV;   
//	tVstart = __Time ();  
//	for (int iV = 0; iV < iVwrites; iV++) cVtest.bnget ((AnsiString) iV);  
//	tVend = __Time (); 
//	printf ("%i ricerche random:        %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));   
//
//	for (int iV = 0; iV < iVwrites; iV++) cVtest.bnuns ((AnsiString) iV);   
//
//	PTR_POOL cVpoolsoix (new CorePool());
//	CoreSoix cVtestsoix (cVpoolsoix); 
//	CoreStib::DATALOC cVdataloc;
//	cVdataloc.iVsize = 0;
//	cVdataloc.vVdata = NULL;
//	tVstart = __Time ();  
//	for (int iV = 0; iV < iVwrites; iV++) {
//		AnsiString sVkey = (AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7); 				
//		cVtestsoix.addo2 (sVkey, 12, &cVdataloc);
//	}
//	tVend = __Time (); 
//	printf ("%i insert forward soix:    %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));  
//	cVtestsoix.clear ();
//
//	AnsiString sVkey;
//	AnsiString sVbackupkey;
//	PTR_POOL cVpoolmeot (new CorePool ());
//	CoreMeot cVtestmeot (cVpoolmeot, "io", 10000000, 100000000, 10000000);
//	PrtoLval cVvalue;
//	cVvalue.set_ivtimestamp (0);
//	tVstart = __Time ();  
//	for (int iV = 0; iV < iVwrites; iV++) {
//		AnsiString sVkey = (AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7); 				
//		cVvalue.set_svopaquevalue (((AnsiString) iV).to_string ());
//		cVtestmeot.put__ (sVkey, 12, &cVvalue);
//	}
//	tVend = __Time (); 
//	printf ("%i insert forward meot:    %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart))); 
//	cVtestmeot.clear ();
//	*/
//
//	/*
//	PTR_POOL cVpool (new CorePool ());
//	PTR_PLANNER cVplanner;
//	PrtoL2st cVstatement;
//	PrtoL2cl* cVcolumn;
//	PrtoL2qr cVquery;
//	PrtoL2mr cVdmlresult;
//
//	PTR_ORION cVorion (new CoreOrn_ (	cVpool, 
//						"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder\\", 
//						"Orion.xml",CoreEnpo::iCrunning, false));
//	cVplanner = cVorion->getpl ();
//	cVstatement.mutable_cvmutable ()->set_svtable ("AUTOGENERATE");
//	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
//	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
//	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
//	cVcolumn->set_svcolumn ("FIELD");
//	tVstart = __Time ();  
//	for (int iV = 0; iV < iVwrites; iV++) {
//		AnsiString sVkey = (AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7); 		
//
//		cVstatement.mutable_cvkey ()->set_svmain (sVkey.to_string ());
//		cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
//		cVcolumn->mutable_cvvalue ()->set_svvalue (((AnsiString) iV).to_string ());
//		if (!cVplanner->statm (&cVstatement)) printf ("ERRORE\n");
//	}
//	tVend = __Time (); 
//	printf ("%i insert table forward:   %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));   
//
//	cVplanner.reset ();
//	cVorion.reset ();
//	cVpool.reset ();*/
//
//	/*
//	PrtoTop_ cVtop;
//	cVplanner->top__ (cVtop);
//	for (int iV = 0; iV < cVtop.cvtopelement_size (); iV++) {
//		PrtoTope* cVtopelement = cVtop.mutable_cvtopelement (iV);
//		printf ("tab %s, w:%lld, r:%lld, wc:%lld, rc:%lld\n",
//			cVtopelement->svtablet ().c_str (), cVtopelement->ivwritetime (), cVtopelement->ivreadtime (),
//			cVtopelement->ivwritecountl1 (), cVtopelement->ivreadcountl1 ());
//	}
//
//	srand(time(0));
//	cVquery.mutable_cvmutable ()->set_svtable ("AUTOGENERATE");
//	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
//	cVcolumn = cVquery.mutable_cvselect ()->Add ();
//	cVcolumn->set_svcolumn ("FIELD"); 	
//	cVquery.set_ivquery (EXACTQUERY);
//
//
//	tVstart = __Time ();  
//	for (int iV = 0; iV < iVwrites; iV++) {
//		int iVextract = rand()%iVwrites;
//		AnsiString sVkey = (AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iVextract , '0', 7); 	
//		cVquery.mutable_cvkey_start ()->set_svmain (sVkey.to_string ());
//		if (!cVplanner->query (&cVquery, &cVdmlresult)) {
//			printf ("ERRORE\n");
//			break;
//		}
//	}
//	tVend = __Time (); 
//	printf ("%i read random:            %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));   
//
//	cVplanner->top__ (cVtop);
//	for (int iV = 0; iV < cVtop.cvtopelement_size (); iV++) {
//		PrtoTope* cVtopelement = cVtop.mutable_cvtopelement (iV);
//		printf ("tab %s, w:%lld, r:%lld, wc:%lld, rc:%lld\n",
//			cVtopelement->svtablet ().c_str (), cVtopelement->ivwritetime (), cVtopelement->ivreadtime (),
//			cVtopelement->ivwritecountl1 (), cVtopelement->ivreadcountl1 ());
//	}
//	*/
//
//	/*
//	tVstart = __Time (); 
//	for (int iV = 0; iV < iVwrites; iV++) {
//	
//		CoreList<CorePlan::KEYLOC> cVkeys;
//		//CoreList<CorePlan::COLUMNLOC> cVcolumns;
//		//CoreList<CorePlan::POINTERKEYLOC> cVpointerkeys; 
//	}
//	tVend = __Time (); 
//	printf ("%i insert table forward:   %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));   
//	*/
//
//
//
//	//thrift transport
//	/*
//	ThrfLmtb cVmutable;
//
//	cVmutable.sVnamespace = "test";
//	cVmutable.sVtable = "test2";
//
//  boost::shared_ptr<TMemoryBuffer> transport(new TMemoryBuffer ());
//  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
//	transport->open ();
//	cVmutable.write (protocol.get ());
//	printf ("test %i\n", transport.get ()->getBufferAsString ().length ());
//	std::string cVtry = transport.get ()->getBufferAsString ();
//	transport->close ();
//
//	ThrfLmtb cVmutable2;
//
//	tVstart = __Time ();   
//	//boost::shared_ptr<TMemoryBuffer> transport2(new TMemoryBuffer ((uint8_t*)cVtry.data(), cVtry.length(), TMemoryBuffer::COPY));
//	boost::shared_ptr<TTransport> transport2(new TMemoryBuffer ());
//	boost::shared_ptr<TProtocol> protocol2(new TBinaryProtocol(transport2));
//
//	for (int iV = 0; iV < iVwrites; iV++) {
//
//
//		transport2->write ((uint8_t*)cVtry.data(), cVtry.length());
//		transport2->flush ();
//		cVmutable2.read (protocol2.get ());
//	}
//	tVend = __Time (); 
//	printf ("%i thrift:       %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));   
//	printf ("qui %s\n", cVmutable.sVtable.c_str ());
//	*/
//
//
//
//	/*
//	CoreLssc<PTR_TESTOO2, TESTOO2, 8> cVdata; 
//
//	for (int iV = 0; iV < iVwrites; iV++) cVdata.bnset ((AnsiString) "test" + (AnsiString) iV)->iVvalue = iV;
//	srand (time (0));
//	tVstart = __Time ();   
//	for (int iV = 0; iV < iVwrites; iV++) {    
//		int iVextract = rand()%16; 
//		AnsiString sVkey = (AnsiString) "test" + (AnsiString) iVextract;  
//		//printf ("richiesto %s\n", sVkey.c_str ());
//		PTR_TESTOO2 cVresult = cVdata.bnget (sVkey); 
//		if (cVresult == NULL || cVresult->iVvalue != iVextract) { 
//			printf ("ERRORE %s\n", sVkey.c_str ());
//		}
//	}
//	tVend = __Time (); 
//	printf ("%i only get forward:       %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));   
//	printf ("%s\n", cVdata.debug ().c_str ());  
//		*/ 
//
//	/*
//	{
//		CoreSoix* cVsortedindex;
//		cVsortedindex = new CoreSoix (); 
//		bool bV1;
//		unsigned int iV1;
//	
//		tVstart = __Time ();
//		for (int iV = 0; iV < 100000; iV++)
//			cVsortedindex->addop ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 6), iV, -1, bV1, iV1); 
//
//		CoreSoix::OO2POSITION	 iVposition = cVsortedindex->bnsrc ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) 50000, '0', 6));
//		printf ("check!! %s\n", cVsortedindex->get__ (iVposition - 1).c_str ());
//
//		tVend = __Time (); 
//		printf ("elapsed: %f\n", tVdiff * (tVend - tVstart)); 
//	}
//	{
//		CoreSoix* cVsortedindex;
//		cVsortedindex = new CoreSoix (); 
//
//		tVstart = __Time ();  
//		for (int iV = 0; iV < 100000; iV++)
//			cVsortedindex->add__ ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 6), iV); 
//
//		CoreSoix::OO2POSITION	 iVposition = cVsortedindex->bnsrc ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) 50000, '0', 6));
//		printf ("check %s\n", cVsortedindex->get__ (iVposition - 1).c_str ());
//
//		tVend = __Time (); 
//		printf ("elapsed: %f\n", tVdiff * (tVend - tVstart)); 
//	}
//	{
//		CoreStix* cVsortedindex;
//		cVsortedindex = new CoreStix ();  
//		bool bV1;
//		unsigned int iV1;
//	
//		tVstart = __Time ();
//		for (int iV = 0; iV < 100000; iV++)
//			cVsortedindex->addop ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 6), iV, -1, bV1, iV1); 
//
//		int	 iVposition = cVsortedindex->bnsrc ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) 50000, '0', 6));
//		printf ("check %s\n", cVsortedindex->get__ (iVposition - 1).c_str ());
//
//		tVend = __Time (); 
//		printf ("elapsed: %f\n", tVdiff * (tVend - tVstart)); 
//	}
//	{
//		CoreStix* cVsortedindex;
//		cVsortedindex = new CoreStix ();
//
//		tVstart = __Time ();  
//		for (int iV = 0; iV < 100000; iV++)
//			cVsortedindex->add__ ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 6), iV); 
//
//		int	 iVposition = cVsortedindex->bnsrc ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) 50000, '0', 6));
//		printf ("check %s\n", cVsortedindex->get__ (iVposition - 1).c_str ());
//
//		tVend = __Time (); 
//		printf ("elapsed: %f\n", tVdiff * (tVend - tVstart)); 
//	}*/
//	
//	
//	//printf ("%s\n", cVsortedindex->debug ().c_str ());  
//
//	/*
//	CoreSoix::OO2POSITION	 iVposition = cVsortedindex->bnsrc ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) 50000, '0', 6));
//	printf ("check %s\n", cVsortedindex->get__ (iVposition - 1).c_str ());
//
//	printf ("size %i %i\n", cVsortedindex->getsz (), cVsortedindex->count ());
//	*/
//
//
//
//
//
//	/*
//	CoreClps<int> iVtest (2, NULL); 
//	CoreClps<int> iVtest2 = iVtest - 1;
//	printf ("%i %i\n", iVtest2.iVposition, iVtest2.cVpointer); 
//
//	//errore! 
//	printf ("%i %i\n", iVtest.iVposition, iVtest.cVpointer);*/
//
//	/*
//	PTR_POOL cVpool (new CorePool ());
//	CoreMetb cVmemtable (cVpool, "", 100, 10000, 100);	
//	PrtoLval cVvalue;
//	cVvalue.set_ivtimestamp (0);
//	
//	tVstart = __Time ();  
//	for (int iV = 0; iV < 100000; iV++) {
//		cVvalue.set_svopaquevalue (((AnsiString) iV).to_string ());
//		cVmemtable.put__ ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 6), -1, &cVvalue);
//	}
//	for (int iV = 0; iV < 100000; iV++) {
//		cVvalue.set_svopaquevalue (((AnsiString) iV).to_string ());
//		cVmemtable.put__ ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 6), -1, &cVvalue);
//	}
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart)); 
//	printf ("%i\n", cVmemtable.getsd ());
//	*/
//
//
//
//
//
////test o2 per la robustezza
///*
//	CoreOo2w<TESTOO2, 4> cVtest; 
//
//	tVstart = __Time ();  
//	srand(time(0));
//	int iVelements = 500000;
//	CoreList<testoo2> cVdomain;
//	for (int iV = 0; iV < iVelements; iV++) cVdomain.bnset ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7))->iVvalue = iV;
//	while (cVdomain.count () > 0) {
//		int iVextract = rand()%cVdomain.count ();
//		TESTOO2* cVextract = cVdomain.get__ (iVextract);
//		cVtest.bnset (cVextract->sVindex)->iVvalue = cVextract->iVvalue;
//		//printf ("ins %s %i\n", cVextract ->sVindex.c_str (), cVextract ->iVvalue);
//		cVdomain.unset (iVextract);
//	}
//	printf ("%i\n", cVtest.count ());
//	//printf ("%s\n%i\n", cVtest.prnta (false).c_str (), cVtest.count ());
//	for (int iV = 0; iV < iVelements; iV++) cVdomain.bnset ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7))->iVvalue = iV;
//	while (cVdomain.count () > 0) {
//		int iVextract = rand()%cVdomain.count ();
//		TESTOO2* cVextract = cVdomain.get__ (iVextract);
//		//printf ("del %s %i\n", cVextract ->sVindex.c_str (), cVextract ->iVvalue);
//		if (cVextract->iVvalue != cVtest.bnget (cVextract->sVindex)->iVvalue) {
//			printf ("ERRORE\n");
//			break;
//		}
//		cVtest.bnuns (cVextract->sVindex);
//		if (cVtest.bnget (cVextract->sVindex) != NULL && cVtest.bnget (cVextract->sVindex)->iVvalue == cVextract->iVvalue) {
//			printf ("ERRORE\n");
//		}
//		cVdomain.unset (iVextract);
//	}
//	printf ("%s\n%i\n", cVtest.prnta (false).c_str (), cVtest.count ());	
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart)); 
//*/
//
////test robustezza in puro o2
///*
//	CoreOo2w<TESTOO2, 4> cVtest; 
//
//	tVstart = __Time ();  
//	srand(time(0));
//	int iVelements = 10000000;
//	CoreOo2w<testoo2, 32> cVdomain;
//	tVstart = __Time ();  
//	for (int iV = 0; iV < iVelements; iV++) cVdomain.bnset ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7))->iVvalue = iV;
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart)); 
//
//	tVstart = __Time ();  
//	while (cVdomain.count () > 0) {
//		int iVcount = cVdomain.lncnt ();
//		if (iVcount % 10000 == 0) printf ("step %i\n", iVcount);
//		int iVextract = rand()%iVcount;
//	
//		CoreOo2w<testoo2, 32>::OO2POS cVposition =  cVdomain.ln2ps (iVextract);
//		TESTOO2* cVextract = cVdomain.get__ (cVposition);
//		cVtest.bnset (cVextract->sVindex)->iVvalue = cVextract->iVvalue;
//		//printf ("ins %s %i\n", cVextract ->sVindex.c_str (), cVextract ->iVvalue);
//		cVdomain.unset (cVposition );
//	}
//	printf ("%i\n", cVtest.count ());
//
//	//printf ("%s\n%i\n", cVtest.prnta (false).c_str (), cVtest.count ());
//	for (int iV = 0; iV < iVelements; iV++) cVdomain.bnset ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7))->iVvalue = iV;
//	while (cVdomain.count () > 0) {
//		int iVcount = cVdomain.lncnt ();
//		if (iVcount % 10000 == 0) printf ("step %i\n", iVcount);
//		int iVextract = rand()%iVcount;
//		CoreOo2w<testoo2, 32>::OO2POS cVposition =  cVdomain.ln2ps (iVextract);
//		TESTOO2* cVextract = cVdomain.get__ (cVposition);
//		//printf ("del %s %i\n", cVextract ->sVindex.c_str (), cVextract ->iVvalue);
//		if (cVextract->iVvalue != cVtest.bnget (cVextract->sVindex)->iVvalue) {
//			printf ("ERRORE\n");
//			break;
//		}
//		cVtest.bnuns (cVextract->sVindex);
//		if (cVtest.bnget (cVextract->sVindex) != NULL && cVtest.bnget (cVextract->sVindex)->iVvalue == cVextract->iVvalue) {
//			printf ("ERRORE\n");
//		}
//		cVdomain.unset (cVposition);
//	}
//	printf ("%s\n%i\n", cVtest.prnta (false).c_str (), cVtest.count ());	
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart)); 
//	*/
//
//
////test soix di inserimento e cancellazione ottimizzata
///*
//	CoreSoix cVtest; 
//	tVstart = __Time ();  
//	srand(time(0));
//	int iVelements = 500000;
//	bool bVrecycle;
//	unsigned int iVrecycle;
//	CoreList<testoo2> cVdomain;
//	for (int iV = 0; iV < iVelements; iV++) cVdomain.bnset ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7) + "xx")->iVvalue = iV;
//	while (cVdomain.count () > 0) {
//		int iVextract = rand()%cVdomain.count ();
//		TESTOO2* cVextract = cVdomain.get__ (iVextract);
//		cVtest.addop (cVextract->sVindex, cVextract->iVvalue, 12, bVrecycle, iVrecycle);
//		cVdomain.unset (iVextract);
//	}
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart));  
//
//	tVstart = __Time ();  
//	for (int iV = 0; iV < iVelements; iV++) cVdomain.bnset ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7) + "xx")->iVvalue = iV;
//	while (cVdomain.count () > 0) {
//		int iVextract = rand()%cVdomain.count ();
//		TESTOO2* cVextract = cVdomain.get__ (iVextract);
//		cVtest.delop (cVextract->sVindex, 12, iVrecycle);
//		cVdomain.unset (iVextract);
//	}
//	tVend = __Time (); 
//	printf ("ho finito\n");
//	printf ("%s\n", cVtest.debug ().c_str ());
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart)); 
//	*/
//
//
//	//3.43 0.73
///*
//	AnsiString sVkey;
//	AnsiString sVbackupkey;
//	PTR_POOL cVpool = boost::shared_ptr<CorePool> (new CorePool ());
//	CoreMeot cVtest; 
//	cVtest.init (cVpool, "io", 10000000, 100000000, 10000000);
//	PrtoLval cVvalue;
//	cVvalue.set_ivtimestamp (0);
//	tVstart = __Time ();  
//	srand(time(0));
//	int iVelements = 100000;
//	CoreList<testoo2> cVdomain;
//	for (int iV = 0; iV < iVelements; iV++) cVdomain.bnset ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7) + "xx")->iVvalue = iV;
//	while (cVdomain.count () > 0) {
//		int iVextract = rand()%cVdomain.count ();
//		TESTOO2* cVextract = cVdomain.get__ (iVextract);
//		cVvalue.set_svopaquevalue (((AnsiString) cVextract->iVvalue).to_string ());
//		cVtest.put__ (cVextract->sVindex, 12, &cVvalue);
//		cVdomain.unset (iVextract);
//	}
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart));  
//	//printf ("%s\n", cVtest.debug ().c_str ());
//
//
//	tVstart = __Time ();  
//	for (int iV = 0; iV < iVelements; iV++) cVdomain.bnset ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7) + "xx")->iVvalue = iV;
//	while (cVdomain.count () > 0) {
//		int iVextract = rand()%cVdomain.count ();
//		TESTOO2* cVextract = cVdomain.get__ (iVextract);
//		sVkey = cVextract->sVindex.SubString (1,11);
//		cVtest.get__ (sVkey , &cVvalue);
//		if (cVvalue.svopaquevalue () != ((AnsiString) cVextract->iVvalue).to_string ()) {
//			printf ("cercavo %s\n", cVextract->sVindex.SubString (1,11).c_str ());
//			printf ("ERRORE %s != %s\n", cVvalue.svopaquevalue ().c_str (), ((AnsiString) cVextract->iVvalue).c_str ());
//			break;
//		} 
//		cVtest.delte (cVextract->sVindex, 12);
//		sVbackupkey = sVkey = cVextract->sVindex.SubString (1,11);
//		if (cVtest.get__ (sVkey, &cVvalue) && sVkey == sVbackupkey ) {
//			printf ("ho trovato qualcosa (%s-%s) %s\n", 
//				sVkey.c_str (),
//				cVvalue.svopaquevalue ().c_str (), 
//				
//				((AnsiString) cVextract->iVvalue).c_str ());
//		}
//
//
//		cVdomain.unset (iVextract);
//	}
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart));  
//	printf ("%s\n", cVtest.debug ().c_str ());
//	*/
//
//
////inserimento in tabella;
///*
//	PTR_POOL cVpool (new CorePool ());
//	PTR_PLANNER cVplanner;
//	PrtoL2st cVstatement;
//	PrtoL2cl* cVcolumn;
//	PrtoL2qr cVquery;
//	PrtoL2mr cVdmlresult;
//
//	PTR_ORION cVorion (new CoreOrn_ (	cVpool, 
//						"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder\\", 
//						"Orion.xml",CoreEnpo::iCrunning, false));
//	cVplanner = cVorion->getpl ();
//		
//	cVstatement.mutable_cvmutable ()->set_svtable ("AUTOGENERATE");
//	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
//	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
//	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
//	cVcolumn->set_svcolumn ("FIELD");
//
//	tVstart = __Time ();  
//	srand(time(0));
//	int iVelements = 100000;
//	CoreList<testoo2> cVdomain;
//	for (int iV = 0; iV < iVelements; iV++) cVdomain.bnset ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7))->iVvalue = iV;
//	while (cVdomain.count () > 0) {
//		int iVextract = rand()%cVdomain.count ();
//		TESTOO2* cVextract = cVdomain.get__ (iVextract);
//		cVstatement.mutable_cvkey ()->set_svmain (cVextract->sVindex.to_string ());
//		cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
//		cVcolumn->mutable_cvvalue ()->set_svvalue (((AnsiString) cVextract->iVvalue).to_string ());
//		if (!cVplanner->statm (&cVstatement)) printf ("ERRORE\n");
//
//		cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ()+200);
//		if (!cVplanner->statm (&cVstatement)) printf ("ERRORE\n");
//		cVdomain.unset (iVextract);
//	}
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart));  
//	
//	cVquery.mutable_cvmutable ()->set_svtable ("AUTOGENERATE");
//	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
//	cVcolumn = cVquery.mutable_cvselect ()->Add ();
//	cVcolumn->set_svcolumn ("FIELD"); 	
//	cVquery.set_ivquery (EXACTQUERY);
//
//	cVstatement.mutable_cvkey ()->set_ivstate (DELTMB);
//
//	tVstart = __Time ();  
//	for (int iV = 0; iV < iVelements; iV++) cVdomain.bnset ((AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7))->iVvalue = iV;
//	while (cVdomain.count () > 0) {
//		int iVextract = rand()%cVdomain.count ();
//		TESTOO2* cVextract = cVdomain.get__ (iVextract); 
//
//		cVquery.mutable_cvkey_start ()->set_svmain (cVextract->sVindex.to_string ());
//		//printf ("CANCELLAZIONE DI %s, elapsed %i\n", cVextract->sVindex.c_str (), cVdomain.count ());
//
//		if (cVplanner->query (&cVquery, &cVdmlresult)) {
//			if ((AnsiString) cVdmlresult.mutable_cvkeyslice (0)->mutable_cvkey ()->svmain () != cVextract->sVindex) {
//				printf (">>>>>>>>>>>ERRORE %s\n", cVextract->sVindex.c_str ());	
//			} else {
//				//CANCELLAZIONE
//				cVstatement.mutable_cvkey ()->set_svmain (cVextract->sVindex.to_string ());
//				cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
//				cVcolumn->mutable_cvvalue ()->set_svvalue (((AnsiString) cVextract->iVvalue).to_string ());
//				if (!cVplanner->statm (&cVstatement)) printf (">>>>>>>>>>>ERRORE STATEMENT\n");
//
//
//				cVquery.mutable_cvkey_start ()->set_svmain (cVextract->sVindex.to_string ());
//				if (cVplanner->query (&cVquery, &cVdmlresult)) {
//					printf ("quanti %i\n", cVdmlresult.cvkeyslice_size ());
//					printf (">>>>>>>>>>>ERRORE valore %s %s\n", 
//						cVdmlresult.mutable_cvkeyslice (0)->mutable_cvkey ()->svmain ().c_str (), cVextract->sVindex.c_str ());
//					break;
//				}
//
//			}
//		}	else {
//			printf (">>>>>>>>>>>ERRORE %s\n", cVextract->sVindex.c_str ());		
//			break;
//		}
//		cVdomain.unset (iVextract);
//	}
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart));  
//*/
//
//	/*
//	PTR_POOL cVpool (new CorePool);
//	PTR_ORION cVorion (new CoreOrn_ (	cVpool, 
//						"C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\Release\\", 
//						"Orion.xml",CoreEnpo::iCrunning, false));
//	PTR_PLANNER cVplanner = cVorion->getpl ();	
//
//	cVplanner->lnch_ (NULL, &CorePlan::touch);
//	PrtoLmtb cVmutable;
//	
//	cVmutable.set_svnamespace ("DEFAULT");
//
//	tVstart = __Time ();  
//	for (int iV = 0; iV < 1000000; iV++) {
//		cVmutable.set_svtable (((AnsiString) "USERTESTNEW" + (AnsiString) (iV % 10)).to_string ());
//		cVplanner->gettl (&cVmutable);
//	}
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart));  
//	*/
//
//	/*
//	tVstart = __Time (); 
//	ThrfSrvc cVservice;
//	void* vVbuffer;
//	uint32_t iVsize;
//	PTR_POOL cVpool = boost::shared_ptr<CorePool> (new CorePool ());
//	PTR_FILESYSTEM cVfilesystem = boost::shared_ptr<CoreFsys> (new CoreFsys (cVpool));
//	ThrfSrvr cVdeserializer;
//
//	ThrfLmtb* cVmutable;
//	ThrfLkey* cVkey;
//	ThrfL2cl cVthriftcolumn;
//	ThrfL2cv* cVvalue;
//
//	cVservice.iVservicetype = iEservicetype::STATEMENT;
//	cVmutable = &cVservice.cVstatement.cVmutable;
//	cVmutable->sVnamespace = "test";
//	cVmutable->sVtable = "test2";
//	cVkey = &cVservice.cVstatement.cVkey;
//	cVkey->sVmain = "KEY";
//	cVkey->iVstate = iEstatetype::UPSERT;
//	cVkey->iVtimestamp = 0;
//	cVthriftcolumn.sVcolumn = "COLUMN";
//	cVvalue = &cVthriftcolumn.cVvalue;
//	cVvalue->sVvalue = "VALUE";
//	cVvalue->iVtype = iEcolumntype::STRINGTYPE;
//	cVservice.cVstatement.cVcolumns.push_back (cVthriftcolumn);
//
//  boost::shared_ptr<TMemoryBuffer> transport(new TMemoryBuffer ());
//  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
//	transport->open ();
//	cVservice.write (protocol.get ());
//	printf ("serialized =  %i\n", transport.get ()->getBufferAsString ().length ());
//	transport.get ()->getBuffer ((uint8_t**) &vVbuffer,&iVsize);
//	transport->close ();
//
//
//
//	AnsiString sVhostname = CoreSckt::sCpipeaddess;
//
//	CoreSckt cVsocket (CoreSckt::trsla (sVhostname));
//	bool bVsuccess = cVsocket.open_ (0);
//  if (bVsuccess) bVsuccess = cVsocket.cnnct (9000, sVhostname);
//	if (bVsuccess) { cVsocket.msend ();
//		bVsuccess = cVsocket.mrcve ();
//	}
//	cVsocket.send_ ((char*) vVbuffer, iVsize);
//	cVsocket.lflsh ();
//	iVsize = cVsocket.recve ();
//	cVfilesystem->setbf (iVsize);
//	cVsocket.recve ((char*) cVfilesystem->baddr (), iVsize);
//
//	boost::shared_ptr<TTransport> transport2(new TMemoryBuffer ());
//	boost::shared_ptr<TProtocol> protocol2(new TBinaryProtocol(transport2));
//
//	transport2->write ((uint8_t*) cVfilesystem->baddr (), iVsize);
//	transport2->flush ();
//	cVdeserializer.read (protocol2.get ());
//	printf ("risultato %i\n", cVdeserializer.bVreturn);
//	*/
//
//
//	
//
//
//	/*
//	PTR_ORIONCONNECTION cVorionconnection = cVconnectionfactory->chout();
//	cVbulkservice.iVbulkservicetype = iEbulkservicetype::SINGLETHRIFT;
//	cVbulkservice.cVsinglethrift = cVservice;
//	cVorionconnection->run__ (cVbulkserviceresult, cVbulkservice);
//	cVserviceresult = cVbulkserviceresult.cVsinglethrift;
//	printf ("risultato %i\n", cVserviceresult.bVreturn);
//	cVconnectionfactory->chin_ (cVorionconnection);
//	CoreCwrk::PTR_CLIENTJOBLOC cVjob (new CoreCwrk::CoreCjob (&cVservice));
//	*/
//
//	/*
//	cVkey->sVmain = "KEY2";
//	tVstart = __Time (); 
//	for (int iV = 0; iV < iVwrites; iV++) {
//		cVorionconnection = cVconnectionfactory->chout();
//		cVorionconnection->run__ (cVserviceresult, cVservice);
//		cVconnectionfactory->chin_ (cVorionconnection);
//	}
//	tVend = __Time (); 
//
//	printf ("risultato2 %i\n", cVserviceresult.bVreturn);
//	printf ("%i only get forward:       %f, qps %f\n", iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));   
//	*/
//	/*
//	CoreCwrk cVworker (cVconnectionfactory, 20);
//	
//	cVworker.addjb (cVjob);*/
//
//	/*
//	ThrfSrvr cVserviceresult2;
//	printf ("inizio\n");
//	CoreCaco cVfactory (cVconnectionfactory, 1, 20);
//	cVfactory.addrf ();
//	cVfactory.run__ (cVserviceresult2, cVservice);
//	printf ("risultato %i\n", cVserviceresult2.bVreturn);
//
//
//	printf ("fatto\n");
//	Sleep (5000);
//	printf ("sub\n");
//	cVfactory.subrf ();
//	*/
//
//
//
//	/*
//	tVend = __Time (); 
//	printf ("elapsed: %f\n", tVdiff * (tVend - tVstart));  
//
//	while (true) {Sleep (10);}
//	return 0;
//}*/

//#define iVwrites 1000000

//
//class TestStrs : TThread {
//	int iVtype;
//	int iVworker;
//	CoreSmph cVmutex;
//	ThrfSrvc cVservice;
//	PrtoSrvc cVprotoservice;
//	PTR_ARIESCONNECTION cVconnectionfactory;
//	PTR_ORIONCONNECTIONFACTORY cVorionconnectionfactory;
//public:
//  __fastcall TestStrs () : TThread (true) {
//		iVtype = -1;
//	}
//
//  __fastcall ~TestStrs () {
//	}
//
//	void Execute () {
//		if (iVtype == 1) {
//				printf ("inizio orion thrift simple %s\n", cVservice.cVstatement.cVmutable.sVtable.c_str ());
//				PTR_ORIONCONNECTION cVorionconnection = cVorionconnectionfactory->chout ();
//				for (int iV = 0; iV < iVwrites; iV++) {
//					ThrfBsrr cVbulkserviceresult;
//					ThrfBsrc cVbulkservice;
//
//					cVbulkservice.iVbulkservicetype = //iEbulkservicetype::PING;
//						iEbulkservicetype::SINGLETHRIFT;
//					cVservice.cVstatement.cVkey.sVmain = ((AnsiString) "KEY" + (AnsiString) iV).to_string ();
//					cVbulkservice.cVsinglethrift = cVservice;
//					cVorionconnection->run__ (&cVbulkserviceresult, &cVbulkservice);
//					if (!cVbulkserviceresult.cVsinglethrift.bVreturn) {
//							printf ("errore %i\n", iV);
//							break;
//					}
//				}
//				cVorionconnectionfactory->chin_ (cVorionconnection);
//		} else if (iVtype == 0) {
//				//SetThreadAffinityMask(GetCurrentThread(),(1 << iVworker));
//				cVconnectionfactory->addrf ();
//				PTR_ARIESPIPELINE cVariespipeline (new CoreCacp (cVconnectionfactory));
//				
//
//				for (int iV = 0; iV < iVwrites; iV++) {
//					PrtoSrvr cVserviceresult;
//					cVprotoservice.mutable_cvstatement ()->mutable_cvkey ()->set_svmain (((AnsiString) "KEY" + (AnsiString) iV).to_string ());				
//					if (!cVariespipeline->run__ (cVserviceresult, cVprotoservice, //false
//																				iV < iVwrites - 1
//																				)) {
//						printf ("errore %i\n", iV);
//						break;
//					}
//
//					/*
//					bool bV;
//					for (int iVinner = 0; iVinner < 700; iVinner++)  bV = iV != rand ();
//					*/
//				}
//				cVconnectionfactory->subrf ();
//		} else if (iVtype == 2) {
//				PrtoBsrr cVbulkserviceresult;
//				PrtoBsrc cVbulkservice;
//
//				printf ("inizio orion proto %s\n", 
//					cVprotoservice.mutable_cvstatement ()->mutable_cvmutable ()->svtable ().c_str ());
//				PTR_ORIONCONNECTION cVorionconnection = cVorionconnectionfactory->chout ();
//
//
//				for (int iV = 0; iV < iVwrites; iV++) {
//					cVbulkserviceresult.Clear ();
//					cVbulkservice.Clear ();
//
//					cVbulkservice.set_ivbulkservicetype (SINGLEPROTO);
//					cVprotoservice.mutable_cvstatement ()->mutable_cvkey ()->set_svmain (((AnsiString) "KEY" + (AnsiString) iV).to_string ());
//					cVbulkservice.mutable_cvsingleproto ()->CopyFrom (cVprotoservice);
//
//					cVorionconnection->runpb (&cVbulkserviceresult, &cVbulkservice);
//					if (!cVbulkserviceresult.mutable_cvsingleproto ()->bvreturn ()) {
//							printf ("errore %i\n", iV);
//							break;
//					}
//				}
//				cVorionconnectionfactory->chin_ (cVorionconnection);
//		}
//		cVmutex.relse ();
//	}
//
//	void prepare (PrtoSrvc& cService, PTR_ARIESCONNECTION cConnectionfactory, int iVtable, int iWorker) {
//
//		iVtype = 0;
//		iVworker = iWorker;
//		cVmutex.locks ();
//		cVprotoservice.CopyFrom (cService);
//		cVprotoservice.mutable_cvstatement ()->mutable_cvmutable ()->
//			set_svtable (((AnsiString) "USERTABLE" + (AnsiString) iVtable).to_string ());
//		cVconnectionfactory = cConnectionfactory;
//	}
//
//	void prepareOldway (ThrfSrvc cService, PTR_ORIONCONNECTIONFACTORY cConnectionfactory, int iVtable) {
//		iVtype = 1;
//		cVmutex.locks ();
//		cVservice = cService;
//		cVservice.cVstatement.cVmutable.sVtable = ((AnsiString) "USERTABLE" + (AnsiString) iVtable).to_string ();
//		cVorionconnectionfactory = cConnectionfactory;
//	}
//
//	void prepareProtobuf (PrtoSrvc& cService, PTR_ORIONCONNECTIONFACTORY cConnectionfactory, int iVtable) {
//		iVtype = 2;
//		cVmutex.locks ();
//		cVprotoservice.CopyFrom (cService);
//		cVprotoservice.mutable_cvstatement ()->mutable_cvmutable ()->
//			set_svtable (((AnsiString) "USERTABLE" + (AnsiString) iVtable).to_string ());
//		cVorionconnectionfactory = cConnectionfactory;
//	}
//
//	void start () {
//		Resume ();
//	}
//
//	void wait () {
//		cVmutex.locks ();
//	}
//
//};
//
//
//class TestStrsProto : TThread {
//	PrtoL2st cVstatement;
//	CoreSmph cVmutex;
//	PTR_POOL cVpool;
//	PTR_PLANNER cVplanner;
//	PTR_ORION cVorion;
//
//public:
//  __fastcall TestStrsProto () : TThread (true) {
//	}
//
//  __fastcall ~TestStrsProto () {
//	}
//
//
//	bool insert (std::string sSerialized) {
//		PrtoSrvc cVservice;
//
//		cVservice.ParseFromString (sSerialized);
//		return cVplanner->statm (cVservice.mutable_cvstatement ());
//	}
//
//	void Execute () {
//
//
//		printf ("inizio simple %s\n", cVstatement.mutable_cvmutable ()->svtable ().c_str ());
//
//		for (int iV = 0; iV < iVwrites; iV++) {
//			cVstatement.mutable_cvkey ()->set_svmain (((AnsiString) "KEY" + (AnsiString) iV).to_string ());
//			
//			/*
//			std::string sVserialized;
//			PrtoSrvc cVservice;
//
//			cVservice.set_ivservicetype (STATEMENT);
//			cVservice.mutable_cvstatement ()->CopyFrom (cVstatement);
//			sVserialized = cVservice.SerializeAsString ();
//			*/
//
//			if (!cVplanner->statm (&cVstatement)) {
//			//if (!insert (sVserialized)) {
//				printf ("errore %i\n", iV);
//				break;
//			}
//		}
//		printf ("fine\n");
//		cVmutex.relse ();
//	}
//
//	void prepare (PrtoL2st& cStatement, PTR_PLANNER cPlanner, int iVtable) {
//		cVmutex.locks ();
//
//		/*
//		cVpool = boost::shared_ptr<CorePool> (new CorePool ());
//		cVorion = boost::shared_ptr<CoreOrn_> (new CoreOrn_ (	cVpool, 
//						(AnsiString) "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder" + (AnsiString) iVtable + "\\", 
//						"Orion.xml",CoreEnpo::iCrunning, false));
//		cVplanner = cVorion->getpl ();*/
//
//		cVplanner = cPlanner;
//		cVstatement.CopyFrom (cStatement);
//		cVstatement.mutable_cvmutable ()->set_svtable (((AnsiString) "USERTABLE" + (AnsiString) iVtable).to_string ());
//	}
//
//	void start () {
//		Resume ();
//	}
//
//	void wait () {
//		cVmutex.locks ();
//	}
//
//};
//
//
//
////----------------------------------------------------------------------------------------------------------------------------------
//
///*
//int _tmain(int argc, _TCHAR* argv[]) {	
//	AnsiString sVsubversion;
//	TDateTime tVstart;
//  TDateTime tVend;
//  double tVdiff;  
//
//	tVdiff = 24*3600;
//	if (sizeof (void*) == 8) sVsubversion = "x64"; 
//	else sVsubversion = "x86";
//  printf ("Stress %s\n", sVsubversion.c_str ());
// 	  
//	FLAGS_log_dir = "c:/templog";
//	google::InitGoogleLogging(argv[0]);    
//
//
//	#define iVtables 10
//	#define iVworkers 8
//	*/
//
//
//
//	/*
//	printf ("cominciamo\n");
//	for (int iV = 0; iV < 1000; iV++) {
//		printf ("%i\n", iV);
//		AnsiString sVtest =(AnsiString) "memoria_" + (AnsiString) iV; 
//		CoreShst<int>* cVtest = new CoreShst<int> (true, sVtest, true); 
//		//if (!cVtest->alive ()) break;
//		cVtest->pshbb (1024*1024);
//	}
//	printf ("abbiamo desistito\n");
//	printf ("cominciamo\n");
//	for (int iV = 0; iV < 1000; iV++) {
//		printf ("%i\n", iV);
//		AnsiString sVtest =(AnsiString) "memoria_" + (AnsiString) iV; 
//		CoreShst<int>* cVtest = new CoreShst<int> (false, sVtest, true); 
//		//if (!cVtest->alive ()) break;
//		cVtest->pshbb (1024*1024);
//	}
//	printf ("abbiamo desistito\n");
//	*/
//
//	/*
//	PTR_POOL cVpool (new CorePool ());
//	PTR_PLANNER cVplanner;
//	PrtoL2st cVstatement;
//	PrtoL2cl* cVcolumn;
//	PrtoL2qr cVquery;
//	PrtoL2mr cVdmlresult;
//	CoreList<TestStrsProto> cVworkers;
//
//	PTR_ORION cVorion (new CoreOrn_ (	cVpool, 
//						"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder\\", 
//						"Orion.xml",CoreEnpo::iCrunning, false));
//	cVplanner = cVorion->getpl ();
//	cVstatement.mutable_cvmutable ()->set_svtable ("AUTOGENERATE");
//	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
//	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
//	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
//	cVcolumn->set_svcolumn ("FIELD");
//	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
//	cVcolumn->mutable_cvvalue ()->set_svvalue (((AnsiString) 1000).to_string ());
//
//
//	tVstart = __Time (); 
//	int iVtable = 0;
//	for (int iV = 0; iV < iVworkers; iV++) {
//		iVtable = (++iVtable) % iVtables;
//		cVworkers.set__ ()->prepare (cVstatement, cVplanner, iVtable);
//	}
//	for (int iV = 0; iV < iVworkers; iV++) {
//		cVworkers.get__ (iV)->start ();
//	}
//	Sleep (500);
//	for (int iV = 0; iV < iVworkers; iV++) {
//		cVworkers.get__ (iV)->wait ();
//	}
//	tVend = __Time (); 
//	printf ("%i writes:       %f, qps %f\n", iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites * iVworkers / (tVdiff * (tVend - tVstart)));   
//	for (int iV = 0; iV < iVworkers; iV++) {
//		cVworkers.unset (iV);
//	}*/
//	
//	/*
//	bool bVprotocolbuffer = true;
//	PTR_POOL cVpool = boost::shared_ptr<CorePool> (new CorePool ());
//	PTR_ORIONCONNECTIONFACTORY cVorionconnectionfactory (new CoreCocf ());
//	cVorionconnectionfactory->init_ (cVpool, CoreSckt::sCpipeaddess, 9000, bVprotocolbuffer ? 
//		CoreCocf::bCprotobuf : CoreCocf::bCthrift);*/
//	
//
//
//
//	/*
//	ThrfSrvc cVservice;
//	ThrfSrvr cVserviceresult;
//	ThrfBsrc cVbulkservice;
//	ThrfBsrr cVbulkserviceresult;
//	ThrfLmtb* cVmutable;
//	ThrfLkey* cVkey;
//	ThrfL2cl cVthriftcolumn;
//	ThrfL2cv* cVvalue;
//
//	cVservice.iVservicetype = iEservicetype::STATEMENT;
//	cVmutable = &cVservice.cVstatement.cVmutable;
//	cVmutable->sVnamespace = "DEFAULT";
//	cVmutable->sVtable = "OVERWRITE";
//	cVkey = &cVservice.cVstatement.cVkey;
//	cVkey->sVmain = "KEY";
//	cVkey->iVstate = iEstatetype::UPSERT;
//	cVkey->iVtimestamp = 0;
//	cVthriftcolumn.sVcolumn = "COLUMN";
//	cVvalue = &cVthriftcolumn.cVvalue;
//	cVvalue->sVvalue = "VALUE";
//	cVvalue->iVtype = iEcolumntype::STRINGTYPE;
//	cVservice.cVstatement.cVcolumns.push_back (cVthriftcolumn);
//	*/
//
//	/*
//	PrtoSrvc cVservice;
//	PrtoL2st* cVstatement;
//	PrtoL2cl* cVcolumn;
//	PrtoL2qr cVquery;
//	PrtoL2mr cVdmlresult;
//
//	cVservice.set_ivservicetype (STATEMENT);
//	cVstatement = cVservice.mutable_cvstatement ();
//	cVstatement->mutable_cvmutable ()->set_svtable ("AUTOGENERATE");
//	cVstatement->mutable_cvmutable ()->set_svnamespace ("DEFAULT");
//	cVstatement->mutable_cvkey ()->set_ivstate (UPSERT);
//	cVcolumn = cVstatement->mutable_cvcolumns ()->Add ();
//	cVcolumn->set_svcolumn ("COLUMN");
//	cVstatement->mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
//	cVcolumn->mutable_cvvalue ()->set_svvalue (((AnsiString) 1000).to_string ());
//	cVcolumn->mutable_cvvalue ()->set_ivtype (STRINGTYPE);
//	cVcolumn->set_ivtype (STRINGTYPE); 
//	*/
//
//
//	/*
//	tVstart = __Time (); 
//	CoreFlqu<CoreCwrk::CoreCjob, iVwrites> cVqueue2;
//	for (int iV = 0; iV < iVwrites; iV++) {
//
//		cVqueue2.push_ (new CoreCwrk::CoreCjob (&cVservice, true));
//
//	}
//	tVend = __Time (); 
//	printf ("elementi %i\n", cVqueue2.count ());
//	printf ("Problema enqueue, %i writes:       %f, qps %f\n", iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites * iVworkers / (tVdiff * (tVend - tVstart)));   
//	int iVcapacity2 = 20;
//	tVstart = __Time ();
//	do {
//			PrtoBsrc cVbulkservice;	
//			for (int iV = 0; iV < iVcapacity2; iV++) {
//				CoreCwrk::CoreCjob* cVtest;
//				if (!cVqueue2.pop__ (&cVtest)) break;
//				cVbulkservice.add_cvbulkproto ()->CopyFrom (cVtest->cVservice);
//				delete cVtest;
//		}
//		cVbulkservice.SerializeAsString ();
//	} while (cVqueue2.count () > 0);
//	tVend = __Time ();  
//	printf ("elementi %i\n", cVqueue2.count ());
//	printf ("Problema enqueue, %i writes:       %f, qps %f\n", iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites * iVworkers / (tVdiff * (tVend - tVstart)));   
//	*/
//
//
//
//
//
//
//
//	/*
//	char buffer [5000];
//	ThrfSrvc cVdeser;
//	tVstart = __Time (); 
//	for (int iV = 0; iV < iVworkers; iV++) {
//		boost::shared_ptr<CoreTsrl> cVserializer (new CoreTsrl ());
//		cVserializer->srlze<ThrfSrvc> (cVservice);
//		cVserializer->tobuf (buffer);
//		boost::shared_ptr<CoreTsrl> cVdeserializer (new CoreTsrl ());
//		cVdeserializer->frbuf (buffer);
//		cVdeserializer->parse<ThrfSrvc> (cVdeser);
//	} 
//	tVend = __Time ();   
//	printf ("%i writes:       %f, qps %f\n", iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites * iVworkers / (tVdiff * (tVend - tVstart)));   
//	*/
//
//	/*
//	double dVreference;
//	double dVperformance;
//	for (int iVouter = 1; iVouter <= iVworkers; iVouter++) {
//		PTR_ARIESCONNECTION cVariesconnection (new CoreCaco (cVorionconnectionfactory, iVouter, 50));
//		CoreList<TestStrs> cVworkers;
//
//		int iVtable = 0;
//		for (int iV = 0; iV < iVouter; iV++) { 
//			iVtable = (++iVtable) % iVtables;
//			cVworkers.set__ ()->prepare (cVservice, cVariesconnection, iVtable, iV);
//			//cVworkers.set__ ()->prepareOldway (cVservice, cVorionconnectionfactory, iVtable);
//			//cVworkers.set__ ()->prepareProtobuf (cVservice, cVorionconnectionfactory, iVtable);
//		}
//		tVstart = __Time (); 
//		for (int iV = 0; iV < iVouter; iV++) cVworkers.get__ (iV)->start ();
//		Sleep (100);
//		for (int iV = 0; iV < iVouter; iV++) cVworkers.get__ (iV)->wait ();
//		tVend = __Time (); 
//		dVperformance = (double) iVwrites * iVouter / (tVdiff * (tVend - tVstart));
//		if (iVouter == 1) dVreference = dVperformance;
//		printf ("%i %i writes:       %f, qps %f - %f\n", iVwrites, iVouter, dVperformance / dVreference, dVperformance, tVdiff * (tVend - tVstart));   
//		for (int iV = 0; iV < iVworkers; iV++) 
//			cVworkers.unset (iV);
//		Sleep (500);
//	}*/
//
//	/*
//	cVariesconnection->addrf ();
//	tVstart = __Time ();
//	for (int iV = 0; iV < iVwrites; iV++) {
//		ThrfSrvr cVserviceresult;
//
//		cVservice.cVstatement.cVkey.sVmain = ((AnsiString) "KEY" + (AnsiString) iV).to_string ();
//		cVariesconnection->run__ (cVserviceresult, cVservice);
//	}
//	tVend = __Time (); 
//	printf ("%i writes:       %f, qps %f\n", iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));   
//	cVariesconnection->subrf ();
//	*/
//
//	/*
//	PrtoSrvr cVservicereturn;
//	PTR_ARIESCONNECTION cVariesconnection (new CoreCaco (cVorionconnectionfactory, 1, 50));
//	cVariesconnection->addrf ();
//	PTR_ARIESPIPELINE cVariespipeline (new CoreCacp (cVariesconnection));
//	cVariespipeline->run__ (cVservicereturn, cVservice, true);
//	cVariespipeline->run__ (cVservicereturn, cVservice, true);
//	cVariesconnection->subrf ();
//	printf ("ok\n");
//	*/
//
//	/*
//	while (true) {Sleep (10);}
//	return 0;
//} */
//
//
//
//
///*
//#include <boost/interprocess/windows_shared_memory.hpp>
//#include <boost/interprocess/mapped_region.hpp>
//
//struct sharedloc {
//	CoreSmph cVmutex;
//	int64vcl iVdim;
//	char sVmemoryname [200];
//} SHAREDLOC;
//
//
//
//int _tmain(int argc, _TCHAR* argv[]) {	
//
//	//server
////#define iCserver  
//
//
//#ifdef iCserver
//	CoreShst<ThrfSrvc>  cVsharedstructure (true, "test"); 
//	while (true) {
//		ThrfSrvc cVservice;
//		cVservice = cVsharedstructure.popbg ();  
//		if (cVservice.iVservicetype != 2) { 
//			printf ("ERRORE\n"); 
//			break;
//		}
//		cVsharedstructure.popen ();
//	}
//#else
//	TDateTime tVstart;
//  TDateTime tVend; 
//  double tVdiff = 24*3600;
//	CoreShst<ThrfSrvc>  cVsharedstructure (false, "test"); 
//	tVstart = __Time (); 
//	for (int iV = 0; iV < iVwrites; iV++) {
//		ThrfSrvc cVservice;
//		cVservice.iVservicetype = iEservicetype::OSQL;
//		cVsharedstructure.push_ (cVservice);
//		//printf ("%i\n", iV);   
//	}
//	tVend = __Time ();  
//	printf ("%i writes:       %f, qps %f\n", iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));   
//#endif
//
//	while (true) {Sleep (10);}
//	return 0;
//}
//*/



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*
void test_queries () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoLkey cVkey;
	PrtoLval cVvalue;
	PrtoLmtb cVmutable;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;
	PrtoL2cl* cVcolumn;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
	
	cVquery.mutable_cvmutable ()->set_svtable ("TABLE2");
	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVquery.mutable_cvkey_start ()->set_svmain ("201101_CNNNPL66D45F205D_401_1");
	//cVquery.mutable_cvkey_end ()->set_svmain ("201101_CNNNPL66D45F205D_401_1");

	//cVquery.mutable_cvkey_start ()->set_svmain ("201101_CLBSLV63B48E884K_168_1");
	//cVquery.mutable_cvkey_end ()->set_svmain ("201101_CLBVSS81M56F205S_28_1");

	cVquery.set_ivquery (RANGEQUERY);
	cVquery.set_ivcount (10);
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("VALUE");

	if (cVplanner->query (&cVquery, &cVdmlresult)) {
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
			printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			printf ("%s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
		}
	}

	delete cVorion;
	delete cVpool;	
}*/


/*
void test_query_stem () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoL2ct cVcreatetable;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoL2cv* cVcolumnvalue;
	PrtoL2wh* cVwhere;
	PrtoL2cl* cVcondition;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
	
	cVquery.mutable_cvmutable ()->set_svtable ("PIMTINGCHANNELSTEM");
	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVquery.set_ivcount (5);
	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("TIMESTAMP"); 	
	
	cVwhere = cVquery.mutable_cvwhere ();
	
	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("CHANNELKEY");
	cVcondition->set_ivconditiontype (ECOND);	
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("_it_chilometrolanciato");

	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("GEOHASHBARREL");
	cVcondition->set_ivconditiontype (ECOND);	
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (LSTRNGTYPE);
	cVcolumnvalue->add_svlistvalue ("e014cg");
	cVcolumnvalue->add_svlistvalue ("e014e");

	if (cVplanner->query (&cVquery, &cVdmlresult)) {
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
			printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
		}
		printf ("%i\n", cVdmlresult.cvkeyslice_size ());
	}	


	delete cVorion;
	delete cVpool;

}*/

/*
void test_query2_stem () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoL2ct cVcreatetable;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoL2cv* cVcolumnvalue;
	PrtoL2wh* cVwhere;
	PrtoL2cl* cVcondition;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
	
	cVquery.mutable_cvmutable ()->set_svtable ("PIMTINGCHANNELSTEM");
	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("TIMESTAMP"); 	
	
	cVwhere = cVquery.mutable_cvwhere ();
	
	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("CHANNELKEY");
	cVcondition->set_ivconditiontype (ECOND);	
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("_it_corsaconicanidaslitta");

	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("GEOHASHBARREL");
	cVcondition->set_ivconditiontype (ECOND);	
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (LSTRNGTYPE);
	cVcolumnvalue->add_svlistvalue ("e014cg");
	cVcolumnvalue->add_svlistvalue ("e014e");

	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("TIMESTAMP");
	cVcondition->set_ivconditiontype (GECOND);	
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("20120219171205779");
	

	
	if (cVplanner->query (&cVquery, &cVdmlresult)) {
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
			printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
		}
		printf ("%i\n", cVdmlresult.cvkeyslice_size ());
	}	

	delete cVorion;
	delete cVpool;

}*/

/*
void test_query3_stem () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoL2ct cVcreatetable;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoL2cv* cVcolumnvalue;
	PrtoL2wh* cVwhere;
	PrtoL2cl* cVcondition;
	PrtoL2or* cVorder;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
	
	cVquery.mutable_cvmutable ()->set_svtable ("PIMTINGCHANNELSTEM");
	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVquery.set_ivcount (10);
	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("TIMESTAMP"); 	
	
	cVwhere = cVquery.mutable_cvwhere ();
	
	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("CHANNELKEY");
	cVcondition->set_ivconditiontype (ECOND);	
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("_it_surf");

	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("GEOHASHBARREL");
	cVcondition->set_ivconditiontype (ECOND);	
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (LSTRNGTYPE);
	cVcolumnvalue->add_svlistvalue ("e014c");
	cVcolumnvalue->add_svlistvalue ("e014e");

	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("TIMESTAMP");
	cVcondition->set_ivconditiontype (LECOND);	
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	//cVcolumnvalue->set_svvalue ("20120207111817001");
	cVcolumnvalue->set_svvalue ("20120219171002574");

	cVorder = cVwhere->add_cvorderby ();
	cVorder->set_svcolumn ("TIMESTAMP");
	cVorder->set_ivordertype (DESC);
		
	if (cVplanner->query (&cVquery, &cVdmlresult)) {
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
			//printf ("%s\n", cVquery.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
		}
		printf ("%i\n", cVdmlresult.cvkeyslice_size ());
	}	

	delete cVorion;
	delete cVpool;

}*/

/*
class ThrfOrn_Handler : virtual public ThrfOrn_If {
 public:
  ThrfOrn_Handler() {
    // Your initialization goes here
  }

  void ping() {
    // Your implementation goes here
    printf("ping\n");
  }

  bool create(const ThrfL2ct& cCreateTable) {
    // Your implementation goes here
    printf("create\n");
		return true;
	}

  bool statement(const ThrfL2st& cStatement) {
    // Your implementation goes here
    printf("statement\n");
		return true;
  }

  void query(std::vector<ThrfL2ks> & _return, const ThrfL2qr& cQuery) {
    // Your implementation goes here
    printf("query\n");
  }

};*/


/*
int _tmain(int argc, _TCHAR* argv[]) {	
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

	if (0 != result) {
		printf ("error\n");
	} else {
		int port = 9090;
		boost::shared_ptr<ThrfOrn_Handler> handler(new ThrfOrn_Handler());
		boost::shared_ptr<TProcessor> processor(new ThrfOrn_Processor(handler));
		boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
		boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
		boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());


		TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
		server.serve();
		return 0;
	}
}*/



/*
int _tmain(int argc, _TCHAR* argv[]) {	
	ThrfL2ct cVcreatetable;
	ThrfL2cl cVcolumn;
	ThrfLmtb cVmutable;
	ThrfL2qr cVquery;
	vector<ThrfL2ks> cVreturn;

  boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  ThrfOrn_Client client(protocol);
  transport->open();
	*/

	/*
	cVcolumn.iVtype = iEcolumntype::INTEGRTYPE;
	cVcolumn.sVcolumn = "ENTE_KEY";	
	cVcreatetable.cVcolumns.push_back (cVcolumn);
	cVcolumn.iVtype = iEcolumntype::STRINGTYPE;
	cVcolumn.sVcolumn = "CODICE_FISCALE";	
	cVcreatetable.cVcolumns.push_back (cVcolumn);
	cVcolumn.iVtype = iEcolumntype::STRINGTYPE;
	cVcolumn.sVcolumn = "PACKED";	
	cVcreatetable.cVcolumns.push_back (cVcolumn);

	cVcreatetable.cVmutable.sVtable = "TABLE3";
	cVcreatetable.cVmutable.sVnamespace = "NAMESPACE";

	client.create (cVcreatetable);
	*/

	/*
	ThrfL2st cVstatement;

	cVstatement.cVmutable.sVtable = "AUTOCREATE";
	cVstatement.cVmutable.sVnamespace = "NAMESPACE";
	cVstatement.cVkey.sVmain = "CHIAVE";
	cVstatement.cVkey.iVstate = iEstatetype::UPSERT;
	cVstatement.cVkey.iVtimestamp = CoreButl::tmstp ();

	cVcolumn.sVcolumn = "ENTE_KEY";	
	cVcolumn.cVvalue.iVtype = iEcolumntype::LSTRNGTYPE;
	cVcolumn.cVvalue.sVlistvalue.push_back ("301");
	cVcolumn.cVvalue.sVlistvalue.push_back ("302");
	cVstatement.cVcolumns.push_back (cVcolumn);

	cVcolumn.sVcolumn = "CODICE_FISCALE";	
	cVcolumn.cVvalue.iVtype = iEcolumntype::STRINGTYPE;
	cVcolumn.cVvalue.sVvalue = "BRDSDR51H11L736O";
	cVstatement.cVcolumns.push_back (cVcolumn);

	cVcolumn.sVcolumn = "TEST_INTEGER";	
	cVcolumn.cVvalue.iVtype = iEcolumntype::INTEGRTYPE;
	cVcolumn.cVvalue.iVvalue = 2;
	cVstatement.cVcolumns.push_back (cVcolumn);


	cVcolumn.sVcolumn = "PACKED";	
	cVcolumn.cVvalue.iVtype = iEcolumntype::STRINGTYPE;
	cVcolumn.cVvalue.sVvalue = "una montagna di cazzate";
	cVstatement.cVcolumns.push_back (cVcolumn);

	client.statement (cVstatement);
	*/

	/*
	cVquery.cVmutable.sVtable = "TABLE3";
	cVquery.cVmutable.sVnamespace = "NAMESPACE";
	cVquery.cVkey_start.sVmain = "CHIAVE";
	cVquery.iVquery = iEquerytype::RANGEQUERY;
	cVquery.iVcount = 1;
	cVcolumn.sVcolumn = "PACKED";	
	cVquery.cVselect.push_back (cVcolumn);
	
	client.query (cVreturn, cVquery);

	for (unsigned int iV = 0; iV < cVreturn.size (); iV++) {
		printf ("valore %s\n", cVreturn [iV].cVcolumns [0].cVvalue.sVvalue.c_str ());
	}*/


/*
  transport->close();
  return 0;
}
*/




/*
void test_2level2 () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;
	PrtoL2cv* cVcolumnvalue;
	PrtoL2wh* cVwhere;
	PrtoL2cl* cVcondition;


	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
		
	//insert	
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLEDELETE");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVstatement.mutable_cvkey ()->set_svmain ("CHIAVE");
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstp ());
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("ENTE_KEY");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("301");

	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("CODICE_FISCALE");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("BRDSDR51H11L736O");
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("PACKED");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("una montagna di cazzate6");
	cVplanner->statm (&cVstatement);
	
	
	//query
	cVquery.mutable_cvmutable ()->set_svtable ("TABLEDELETE");
	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("CODICE_FISCALE"); 	
	
	
	cVquery.mutable_cvkey_start ()->set_svmain ("CHIAVE");
	cVquery.set_ivquery (EXACTQUERY);

	if (cVplanner->query (&cVquery, &cVdmlresult)) {
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
			printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
		}
	}	

	//query su indice per controllare non regressione
	cVquery.Clear ();
	cVquery.mutable_cvmutable ()->set_svtable ("TABLEDELETE");
	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("PACKED"); 	
	cVwhere = cVquery.mutable_cvwhere ();
	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("CODICE_FISCALE");
	cVcondition->set_ivconditiontype (ECOND);
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("BRDSDR51H11L736O");

	if (cVplanner->query (&cVquery, &cVdmlresult)) {
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
			printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
		}
	}
	//statement di cancellazione anche degli indici
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLEDELETE");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVstatement.mutable_cvkey ()->set_svmain ("CHIAVE");
	cVstatement.mutable_cvkey ()->set_ivstate (DELTMB);

	//cVplanner->statm (&cVstatement);
	printf ("cancellazione\n");

	cVquery.set_ivquery (EXACTQUERY);

	if (cVplanner->query (&cVquery, &cVdmlresult)) {
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
			printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
		}
	}	




	delete cVorion;
	delete cVpool;

}*/


/*
void test_2delete () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
		
	//insert	
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLENEWDELETE");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVstatement.mutable_cvkey ()->set_svmain ("CHIAVE");
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstp ());
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("ENTE_KEY");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("301");
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("CODICE_FISCALE");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("BRDSDR51H11L736O");
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("PACKED");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("una montagna di cazzate6");
	cVplanner->statm (&cVstatement);
	
	
	//statement di cancellazione anche degli indici
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLENEWDELETE");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVstatement.mutable_cvkey ()->set_svmain ("CHIAVE");
	cVstatement.mutable_cvkey ()->set_ivstate (DELTMB);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstp ());
	cVplanner->statm (&cVstatement);

	//query
	cVquery.mutable_cvmutable ()->set_svtable ("TABLENEWDELETE");
	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("CODICE_FISCALE"); 			
	cVquery.mutable_cvkey_start ()->set_svmain ("CHIAVE");
	cVquery.set_ivquery (EXACTQUERY);

	if (cVplanner->query (&cVquery, &cVdmlresult)) {
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
			printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
		}
	}	


	delete cVorion;
	delete cVpool;

}*/

/*
void test_2collections () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();

	//insert
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLECOLLECTIONS");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("TEST");
	cVstatement.mutable_cvkey ()->set_svmain ("CHIAVE");
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstp ());
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("ENTE_KEY");
	cVcolumn->set_ivtype (LSTRNGTYPE);
	cVcolumn->mutable_cvvalue ()->set_ivtype (LSTRNGTYPE);
	cVcolumn->mutable_cvvalue ()->add_svlistvalue ("301");
	cVcolumn->mutable_cvvalue ()->add_svlistvalue ("302");
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("CODICE_FISCALE");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("BRDSDR51H11L736O");
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("PACKED");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("una montagna di cazzate6");
	cVplanner->statm (&cVstatement);


	

	//query
	cVquery.mutable_cvmutable ()->set_svtable ("TABLECOLLECTIONS");
	cVquery.mutable_cvmutable ()->set_svnamespace ("TEST");
	cVquery.mutable_cvkey_start ()->set_svmain ("CHIAVE");
	cVquery.add_cvselect ()->set_svcolumn  ("CODICE_FISCALE");
	cVquery.add_cvselect ()->set_svcolumn  ("ENTE_KEY");
	cVquery.set_ivquery (EXACTQUERY);
	
	if (cVplanner->query (&cVquery, &cVdmlresult)) {
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
			printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());

			printf ("%i\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (1)->mutable_cvvalue ()->svlistvalue_size ());
			printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (1)->mutable_cvvalue ()->mutable_svlistvalue (0)->c_str ());

		}
	}


	delete cVorion;
	delete cVpool;

}*/

/*
void test_thrift () {	
	ThrfL2ct cVcreatetable;
	ThrfL2cl cVcolumn;
	ThrfLmtb cVmutable;
	ThrfL2qr cVquery;
	vector<ThrfL2ks> cVreturn;

  boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  ThrfOrn_Client client(protocol);
  transport->open();
	
	
	ThrfL2st cVstatement;

	cVstatement.cVmutable.sVtable = "TESTCOLLECTIONS";
	cVstatement.cVmutable.sVnamespace = "NAMESPACE";
	cVstatement.cVkey.sVmain = "CHIAVE";
	cVstatement.cVkey.iVstate = iEstatetype::UPSERT;
	cVstatement.cVkey.iVtimestamp = CoreButl::tmstp ();

	cVcolumn.sVcolumn = "ENTE_KEY";	
	cVcolumn.cVvalue.iVtype = iEcolumntype::LSTRNGTYPE;
	cVcolumn.cVvalue.sVlistvalue.push_back ("301");
	cVcolumn.cVvalue.sVlistvalue.push_back ("302");
	cVstatement.cVcolumns.push_back (cVcolumn);

	cVcolumn.sVcolumn = "CODICE_FISCALE";	
	cVcolumn.cVvalue.iVtype = iEcolumntype::STRINGTYPE;
	cVcolumn.cVvalue.sVvalue = "BRDSDR51H11L736O";
	cVstatement.cVcolumns.push_back (cVcolumn);

	cVcolumn.sVcolumn = "TEST_INTEGER";	
	cVcolumn.cVvalue.iVtype = iEcolumntype::INTEGRTYPE;
	cVcolumn.cVvalue.iVvalue = 56;
	cVstatement.cVcolumns.push_back (cVcolumn);


	cVcolumn.sVcolumn = "PACKED";	
	cVcolumn.cVvalue.iVtype = iEcolumntype::STRINGTYPE;
	cVcolumn.cVvalue.sVvalue = "una montagna di cazzate";
	cVstatement.cVcolumns.push_back (cVcolumn);

	//client.statement (cVstatement);
	
	cVquery.cVmutable.sVtable = "TESTCOLLECTIONS";
	cVquery.cVmutable.sVnamespace = "NAMESPACE";
	cVquery.cVkey_start.sVmain = "CHIAVE";
	cVquery.iVquery = iEquerytype::EXACTQUERY;
	cVquery.iVcount = 1;
	cVcolumn.sVcolumn = "TEST_INTEGER";	
	cVquery.cVselect.push_back (cVcolumn);
	
	client.query (cVreturn, cVquery);

	for (unsigned int iV = 0; iV < cVreturn.size (); iV++) {
		printf ("valore %i\n", cVreturn [iV].cVcolumns [0].cVvalue.sVlistvalue.size ());	
		printf ("valore %i\n", cVreturn [iV].cVcolumns [0].cVvalue.iVtype);		
		printf ("valore %i\n", cVreturn [iV].cVcolumns [0].iVtype);	
		printf ("valore %i\n", cVreturn [iV].cVcolumns [0].cVvalue.iVvalue);
		printf ("valore %s\n", cVreturn [iV].cVcolumns [0].cVvalue.sVvalue.c_str ());
	}
  transport->close();
}

void test_thrift_nonblocking () {	

  boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9001));
  boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  ThrfOrn_Client client(protocol);
  transport->open();	
	client.ping ();	
  transport->close();
}

void test_thrift_nonblockingread () {	
	ThrfL2qr cVquery;
	vector<ThrfL2ks> cVreturn;
	ThrfL2cl cVcolumn;

  boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9001));
  boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  ThrfOrn_Client client(protocol);
  transport->open();	
	
	cVquery.cVmutable.sVtable = "PITINGGEO";
	cVquery.cVmutable.sVnamespace = "DEFAULT";
	cVquery.cVkey_start.sVmain = "_peppemas";
	cVquery.iVquery = iEquerytype::EXACTQUERY;
	cVquery.iVcount = 1;
	cVcolumn.sVcolumn = "KEY";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "GEOHASHBARRELDELTATEMP";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "GEOHASHSTEMBARREL";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "LASTGEOHASHBARREL";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "LAT";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "LNG";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "PARENTKEY";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "RESOLUTION";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "RESOLUTIONDELTA";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "RESOLUTIONSTEM";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "UPDATEDSTEMTEMP";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "ZONEDELTALAT";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "ZONEDELTALNG";	cVquery.cVselect.push_back (cVcolumn);
	cVcolumn.sVcolumn = "ZONERADIUSBARREL";	cVquery.cVselect.push_back (cVcolumn);

	for (int iVouter = 0; iVouter < 1000000; iVouter++) {
		client.query (cVreturn, cVquery);
		if (iVouter % 10000 == 0) printf ("%i %i\n", iVouter, cVreturn.size ());
	}	

  transport->close();
}*/



/*
void test_osql2 () {
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff = 24*3600;
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	PrtoL2qr cVquery;
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;
	PrtoL2st cVstatement;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();

	//osql
	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);

	PrtoL2cv* cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (LSTRNGTYPE);
	cVcolumnvalue->add_svlistvalue ("_user1");
	cVcolumnvalue->add_svlistvalue ("_user0");

	cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("_xx_wall");

	cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("201205050906300000540");

	cVosql.set_svosqlstring ("SELECT FROM PBOX WHERE tingKeyBarrel = :0   AND  channelKey = :1  AND  timestamp < :2 ORDER BY timestamp desc  LIMIT 1");

  tVstart = __Time ();
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s ", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
		//printf ("valore %s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
	}

  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("%i - Elapsed: %f\n", cVdmlresult.cvkeyslice_size (),tVdiff);

	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

}*/

/*
void test_osql3 () {
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff = 24*3600;
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	PrtoL2qr cVquery;
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;
	PrtoL2st cVstatement;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();

	//osql
	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);
	cVosql.set_svosqlstring ("SELECT timestamp FROM PIMTINGCHANNELGEOSTEM");

  tVstart = __Time ();
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
		printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
	}
  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("%i - Elapsed: %f\n", cVdmlresult.cvkeyslice_size (),tVdiff);

	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

}*/


/*
void test_2update () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoL2mr cVdmlresult;
	PrtoL2os cVosql;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
		
	//insert	
	
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLEUPDATE");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVstatement.mutable_cvkey ()->set_svmain ("CHIAVE");
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstp ());
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("FIELD");
	cVcolumn->mutable_cvvalue ()->set_svvalue ("INDEXKEY29");
	//cVplanner->statm (&cVstatement);


	//query
	cVosql.set_svnamespace ("NAMESPACE");
	cVosql.set_bvonlysecondary (false);
	cVosql.set_svosqlstring ("SELECT FROM TABLEUPDATE WHERE FIELD = 'INDEXKEY29'");
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
		printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
	}
	
	delete cVorion;
	delete cVpool;

}*/

/*
void test_compaction () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoL2st cVstatement;
	PrtoL2mr cVdmlresult;
	PrtoL2os cVosql;
	PrtoLmtb cVmutable;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
	
	cVmutable.set_svtable ("PIMTINGCHANNELGEOSTEM");
	cVmutable.set_svnamespace ("DEFAULT");
	cVplanner->compa (&cVmutable, CoreTbsv::iCevery, 0);
			
	cVmutable.set_svtable ("LOCAL");
	cVmutable.set_svnamespace ("SYSTEM");
	cVplanner->compa (&cVmutable, CoreTbsv::iCmerge, 0);
	
	delete cVorion;
	delete cVpool;
}*/

/*
void test_count (CorePlan* cPlanner) {
	PrtoL2qr cVquery;
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;

	cVosql.set_svnamespace ("NAMESPACE");
	cVosql.set_bvonlysecondary (true);

	cVosql.set_svosqlstring ("SELECT FROM TABLEUPDATE LIMIT 10000000");
	cPlanner->osql_ (&cVosql, &cVdmlresult);
	printf ("count %i\n", cVdmlresult.cvkeyslice_size ());
}*/


/*
void test_speedread (CorePlan* cPlanner, int iV) {
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;

	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);

	PrtoL2cv* cVcolumnvalue = cVosql.add_cvinputbindings ();

	cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue (((AnsiString) iV).to_string ());

	cVosql.set_svosqlstring ("SELECT FROM POWNER WHERE email = :0");
	cPlanner->osql_ (&cVosql, &cVdmlresult);
}*/


/*
void test_2insert (CorePlan* cPlanner, int iKey, int iV) {
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	PrtoL2mr cVdmlresult;
	PrtoL2os cVosql;
	AnsiString sVkey = (AnsiString) iV;

	if (iV % 1000 == 0) printf ("%i\n", iV);
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLEUPDATE");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVstatement.mutable_cvkey ()->set_svmain (((AnsiString) iKey).to_string ());
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstp ());
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("FIELD");
	cVcolumn->mutable_cvvalue ()->set_svvalue (sVkey.to_string ());
	cPlanner->statm (&cVstatement);	
}*/

/*
void test_2read (CorePlan* cPlanner, int iV) {
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;
	AnsiString sVkey = (AnsiString) iV;

	cVosql.set_svnamespace ("NAMESPACE");
	cVosql.set_bvonlysecondary (true);

	PrtoL2cv* cVcolumnvalue;

	
	cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue (sVkey.to_string ());

	cVosql.set_svosqlstring ("SELECT FROM TABLEUPDATE WHERE FIELD = :0");
	cPlanner->osql_ (&cVosql, &cVdmlresult);
	//printf ("risultati %i\n", cVdmlresult.cvkeyslice_size ());
}*/

/*
void test_delete () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoL2st cVstatement;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
		
	//statement di cancellazione anche degli indici
	cVstatement.mutable_cvmutable ()->set_svtable ("PBOX");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("DEFAULT");
	cVstatement.mutable_cvkey ()->set_svmain ("PBOX_4d853cd0-a55b-11e1-8dca-080027e99426");
	cVstatement.mutable_cvkey ()->set_ivstate (DELTMB);

	cVplanner->statm (&cVstatement);


	delete cVorion;
	delete cVpool;

}*/


/*
void test_crashread () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;
	PrtoL2cl* cVcolumn;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
		
	cVquery.mutable_cvmutable ()->set_svtable ("PITINGGEO");
	cVquery.mutable_cvmutable ()->set_svnamespace ("DEFAULT");
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("KEY"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("GEOHASHBARRELDELTATEMP"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("GEOHASHSTEMBARREL"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("LASTGEOHASHBARREL"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("LAT"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("LNG"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("PARENTKEY"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("RESOLUTION"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("RESOLUTIONDELTA"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("RESOLUTIONSTEM"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("UPDATEDSTEMTEMP"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("ZONEDELTALAT"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("ZONEDELTALNG"); 	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("ZONERADIUSBARREL"); 	

	cVquery.mutable_cvkey_start ()->set_svmain ("_peppemas");
	cVquery.set_ivquery (EXACTQUERY);

	for (int iVouter = 0; iVouter < 1000000; iVouter++) {
		if (cVplanner->query (&cVquery, &cVdmlresult)) {
			for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
				if (iVouter % 10000 == 0) 
					printf ("%i %s\n", iVouter, cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			}
		}	
	}



	delete cVorion;
	delete cVpool;
}*/

/*
void test_repair_index () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	PrtoL2st cVstatement;
	PrtoLmtb* cVmutable;
	PrtoLkey* cVkey;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();

	cVmutable = cVstatement.mutable_cvmutable ();
	cVmutable->set_svtable ("PBOX");
	cVmutable->set_svnamespace ("DEFAULT");
	cVkey = cVstatement.mutable_cvkey ();
	cVkey->set_svmain ("PBOX_c971f250-abd4-11e1-870b-080027e99426");
	cVstatement.set_ivopcode (REPAIR_);

	cVplanner->statm (&cVstatement);


	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

} */

/*
void test_stress () {
	
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
	test_count (cVplanner);
	for (int iV = 2000; iV < 2500; iV++) test_2insert (cVplanner, iV); 
	test_count (cVplanner);

	delete cVorion;
	delete cVpool;*/


	/*
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;



	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();
	
	for (int iV = 0; iV < 100; iV++) {
		test_2insert (cVplanner, iV, iV+60000); 
		test_2read (cVplanner, iV);
	}
	test_count (cVplanner);

	delete cVorion;

	printf ("valore %i\n", cVpool->getma ());
	delete cVpool;*/
	

	/*
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;


	while (true) {
	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	delete cVorion;
	delete cVpool;
	}
}*/

/*
void test_state_index () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	PrtoLmtb cVmutable;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();

	cVmutable.set_svtable ("PIMTINGCHANNELGEOSTEM");
	cVmutable.set_svnamespace ("DEFAULT");
	//cVplanner->debgi (&cVmutable, "4", "_peppemas_it_peppemastestchanneugil", CoreTble::iCtokenquery);
	//cVplanner->debgi (&cVmutable, "4", "_peppemas_it_peppemastestchannel", CoreTble::iCtkallquery);	

	cVplanner->debgi (&cVmutable, "50");
	//cVmutable.set_svtable ("TABLEUPDATE");
	//cVmutable.set_svnamespace ("NAMESPACE");
	//cVplanner->debgi (&cVmutable, "FIELD");

	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

} */

/*
void test_crash () {
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	PrtoL2qr cVquery;
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;
	PrtoL2st cVstatement;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrà\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();

	//osql
	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);

	PrtoL2cv* cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (LSTRNGTYPE);
	cVcolumnvalue->add_svlistvalue ("e014ca9");
	cVcolumnvalue->add_svlistvalue ("e014ca8");
	cVcolumnvalue->add_svlistvalue ("e014ca3");
	cVcolumnvalue->add_svlistvalue ("e014ca2");

	cVosql.set_svosqlstring ("SELECT FROM PIMTINGCHANNELGEOSTEM WHERE geoHashBarrel = :0  AND   channelKey = '_it_programmazione'  AND   isTingOwned = false  AND   timestamp < '201206071242140000247' ORDER BY timestamp desc  LIMIT 8");
	
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s  - %s\n", 
			cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str (),
			cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str());
	}
	

	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

}*/

/*
void test_osql4 () {
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff = 24*3600;
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	PrtoL2qr cVquery;
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;
	PrtoL2st cVstatement;

	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder\\", 
																	"Orion.xml");
	cVplanner = cVorion->getpl ();

	//osql
	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);

	PrtoL2cv* cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (LSTRNGTYPE);
	cVcolumnvalue->add_svlistvalue ("e014ca9");
	cVcolumnvalue->add_svlistvalue ("e014ca8");
	cVcolumnvalue->add_svlistvalue ("e014ca3");
	cVcolumnvalue->add_svlistvalue ("e014ca2");

	cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (LSTRNGTYPE);
	cVcolumnvalue->add_svlistvalue ("_it_tests3nonpostarequi");
	cVcolumnvalue->add_svlistvalue ("_it_bovisa");

	cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("201207101752210000454");

	cVosql.set_svosqlstring ("SELECT FROM PIMTINGCHANNELGEOSTEM WHERE geoHashBarrel = :0   AND  channelKey = :1  AND   timestamp < :2 ORDER BY timestamp desc  LIMIT 2");

  tVstart = __Time ();
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s ", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
		//printf ("valore %s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
	}

  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("%i - Elapsed: %f\n", cVdmlresult.cvkeyslice_size (),tVdiff);

	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

}
*/

/*
int _tmain(int argc, _TCHAR* argv[]) {	

	google::ParseCommandLineFlags(&argc, &argv, true);
	FLAGS_log_dir = "c:/templog";
	google::InitGoogleLogging(argv[0]);
	::testing::InitGoogleTest (&argc, argv);
	return RUN_ALL_TESTS();
}
*/


/*
int _tmain(int argc, _TCHAR* argv[]) {	
	AnsiString sVsubversion;
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff;

  tVstart = __Time ();
	tVdiff = 24*3600;
	if (sizeof (void*) == 8) sVsubversion = "x64";
	else sVsubversion = "x86";
  printf ("Stress %s\n", sVsubversion.c_str ());





	CorePool cVpool;
	CoreOrn_* cVorion;
	CorePlan* cVplan;
	CorePeer* cVpeer;
	PrtoGoel* cVgossipelement;
	PrtoGosr cVgossipresult;
	PrtoLmtb cVmutable;
	PrtoLkey cVkey;
	PrtoLval cVvalue;

	
	cVorion = new CoreOrn_ (&cVpool, "C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder\\",  
		"Orion.xml", CoreEnpo::iCrunning);
	cVplan = cVorion->getpl ();	
	cVpeer = cVplan->getpr ();

	PrtoGoss cGossip;
	cVgossipelement = cGossip.add_cvgossipelement ();
	cVgossipelement->set_svdatacenterid ("DEFAULTDC");
	cVgossipelement->set_svnodeid ("ALTRONODE2");
	cVgossipelement->set_svaddress ("127.0.0.1");
	cVgossipelement->set_ivport (9002);
	cVgossipelement->set_dvphiaccrual (1);
	cVpeer->gossp (&cGossip, &cVgossipresult);
	printf ("%s\n", cVpeer->debug ().c_str ());	

	//qui faccio il mutator di tipo paro
	//il token di node1 e di node2
	//costruisco due mutazioni

	CoreBint cVa;
	CoreBint cVb;

	cVa = (AnsiString) "d0b96bcbaec9f3120c0477f9ebc05c1a";
	cVb = (AnsiString) "bb6a547e026e0df97c09bed524b9c581";

	printf ("%s\n", cVa.bin2a ().debug ().c_str ());
	printf ("%s\n", cVb.bin2a ().debug ().c_str ());


	cVkey.set_svmain ("CHIAVE");
	cVkey.set_ivtimestamp (CoreButl::tmstl ());
	cVkey.set_ivstate (UPSERT);
	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("TEST");
	cVpeer->muttr (MUTATOR, &cVmutable, &cVkey, &cVvalue, CorePart::iCbyteorderpartition);


	//basta fare una query di tipo token


	delete cVorion;



  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("Elapsed: %f\n",tVdiff);
	while (true) {Sleep (10);}
	return 0;
}*/


    


/*
int _tmain(int argc, _TCHAR* argv[]) {	
	AnsiString sVsubversion;
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff;

  tVstart = __Time ();
	tVdiff = 24*3600;
	if (sizeof (void*) == 8) sVsubversion = "x64";
	else sVsubversion = "x86";
  printf ("Stress %s\n", sVsubversion.c_str ());
	*/

	/*
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	PrtoL2qr cVquery;
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;
	PrtoL2st cVstatement;
	CoreMktr cVmerkeltree;

	FLAGS_log_dir = "c:/templog";
	google::InitGoogleLogging(argv [0]);
	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (	cVpool, "C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\Release\\", 
														"Orion.xml",CoreEnpo::iCrunning, false);
	cVplanner = cVorion->getpl ();
	*/

	/*
	PrtoTabl cVtablet;
	PrtoLmtb cVmutable;
	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("PBOX");
	cVplanner->touch (&cVmutable);*/
	/*
	cVmerkeltree.build (cVplanner->gettb (&cVmutable), "", "z");*/

	
	//cVplanner->iftbl ("DEFAULT_PBOX", &cVtablet);

	//cVplanner->setab (true);	

	/*
	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;*/
	

	/*
	CoreRvrs cVreverse;
	int iVtestcounter = 25678;
	AnsiString sIndex = CoreAlog::getix (&cVreverse, iVtestcounter);

	printf ("%s\n", sIndex.debug (false).c_str ());
	printf ("%f\n", __Time () * tVdiff);

	//trovare il numero finale
	//trovare il numero di secondi

	printf ("data %s\n", CoreAlog::logrx (&cVreverse, sIndex).c_str ());

	int iVcounter;
	int64vcl iVtimestamp;
	CoreAlog::getrx (&cVreverse, sIndex, iVtimestamp, iVcounter);
	printf ("counter %i\n", iVcounter);
	printf ("timestamp %lld %lld \n", iVtimestamp, CoreButl::tmstl ());


	time_t cVnowtime;
	struct tm *cVnowtm;
	char bVtmbuf[64];
	double dVseconds = ((double) iVtimestamp) / (1000000.0 * 1000.0);

	cVnowtime = floor (dVseconds);
	cVnowtm = localtime(&cVnowtime);
	strftime(bVtmbuf, sizeof bVtmbuf, "%Y-%m-%d %H:%M:%S", cVnowtm);
	printf ("%s.%06d\n", bVtmbuf, int ((dVseconds - (double) cVnowtime) * 1000000.0) );
	*/

	/*
	CoreRvrs cVreverse;
	int64vcl iVtimestampa;
	int64vcl iVtimestampb;
	int64vcl iVa = 1348600583810285;
	int64vcl iVb = 1348753406718497024; //risultato atteso, cioè numero più alto, la coretbsv cerca la più bassa

	iVtimestampa = cVreverse.des2l (iVa);
	iVtimestampb = cVreverse.descl (iVb);
	printf ("valore basso %lld %s\n", iVa, CoreButl::i642s (iVtimestampa).debug (false).c_str ());
	printf ("valore alto  %lld %s\n", iVb, CoreButl::i642s (iVtimestampb).debug (false).c_str ());
	printf ("basso > alto %i\n", CoreButl::i642s (iVtimestampa) > CoreButl::i642s (iVtimestampb));

	iVtimestampa = cVreverse.asc_l (iVa);
	iVtimestampb = cVreverse.asc_l (iVb);
	printf ("valore basso %lld %s\n", iVa, CoreButl::i642s (iVtimestampa).debug (false).c_str ());
	printf ("valore alto  %lld %s\n", iVb, CoreButl::i642s (iVtimestampb).debug (false).c_str ());
	printf ("basso > alto %i\n", CoreButl::i642s (iVtimestampa) > CoreButl::i642s (iVtimestampb));
	*/

	/*
  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("Elapsed: %f\n",tVdiff);
	while (true) {Sleep (10);}
	return 0;
}*/


/*
int _tmain(int argc, _TCHAR* argv[]) {	

	google::ParseCommandLineFlags(&argc, &argv, true);
	FLAGS_log_dir = "c:/templog";
	google::InitGoogleLogging(argv[0]);
	LOG(INFO) << "a";
	LOG(WARNING) << "b";
	LOG(INFO) << "c";
}*/


//#define iCcheck
/*
int _tmain(int argc, _TCHAR* argv[]) {	
	AnsiString sVsubversion;
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff;

  tVstart = __Time ();
	tVdiff = 24*3600;
	if (sizeof (void*) == 8) sVsubversion = "x64";
	else sVsubversion = "x86";
  printf ("Stress %s\n", sVsubversion.c_str ());



	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	CorePeer* cVpeer;
	PrtoL2qr cVquery;
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;
	PrtoL2st cVstatement;	
	PrtoTabl cVtablet;
	PrtoLmtb cVmutable;
	PrtoMktr cVprotomk;
	PrtoMktr cVprotomkout;
	PrtoGoel* cVgossipelement;
	PrtoGosr cVgossipresult;
	CoreAenp* cVantientrophy;

	FLAGS_log_dir = "c:/templog/templog";
	google::InitGoogleLogging(argv [0]);
	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (	cVpool, 

#ifndef iCcheck
						"C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\Release\\", 
						//"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder2\\",
#else
						"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder2\\",
#endif
														"Orion.xml",CoreEnpo::iCrunning, false);
	cVplanner = cVorion->getpl ();	
	cVpeer = cVplanner->getpr ();
	

	PrtoGoss cGossip;
	cVgossipelement = cGossip.add_cvgossipelement ();
	cVgossipelement->set_svdatacenterid ("DEFAULTDC");
	cVgossipelement->set_svnodeid ("ALTRONODE2");
	cVgossipelement->set_svaddress ("127.0.0.1");
	cVgossipelement->set_ivport (9002);
	cVgossipelement->set_dvphiaccrual (1);
	cVplanner->getpr ()->gossp (&cGossip, &cVgossipresult);
	printf ("%s\n", cVpeer->debug ().c_str ());	

	printf ("partenza\n");
	//trovare index iniziale
	CoreMktr cVmerkeltree (cVpeer->getal (), cVpeer->getpt (), cVpeer->getep (), "NODE1");
	cVplanner->touch ();

#ifndef iCcheck

	Sleep (5000);
	cVantientrophy = new CoreAenp (cVplanner);
	//cVantientrophy->rpair ("ALTRONODE2",  false, true, "DEFAULT/PIMTINGCHANNELGEOSTEM");
	cVantientrophy->rpair ("ALTRONODE2",  true, true);
	delete cVantientrophy;
		
#else	
	cVmerkeltree.build ("ALTRONODE2", &cVmutable, "", NULL, 100000);
	printf ("dimensione %i\n", cVmerkeltree.getdm ());
	//return 0;
#endif

	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("Elapsed: %f\n",tVdiff);
	while (true) {Sleep (10);}
	return 0;
}*/


/*
int _tmain(int argc, _TCHAR* argv[]) {	
	AnsiString sVsubversion;
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff;

  tVstart = __Time ();
	tVdiff = 24*3600;
	if (sizeof (void*) == 8) sVsubversion = "x64";
	else sVsubversion = "x86";
  printf ("Stress %s\n", sVsubversion.c_str ());

	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	CorePeer* cVpeer;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;
	AnsiString sVvalue;

	FLAGS_log_dir = "c:/templog/templog";
	google::InitGoogleLogging(argv [0]);
	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (	cVpool, 
						"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder2\\",
														"Orion.xml",CoreEnpo::iCrunning, false);
	cVplanner = cVorion->getpl ();	
	cVpeer = cVplanner->getpr ();
	
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLEUPDATE");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVstatement.mutable_cvkey ()->set_svmain ("CHIAVE");
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("FIELD");


	for (int iV = 0; iV < 1000000; iV++) {
		sVvalue = md5 (((AnsiString) iV).to_string ());
		cVcolumn->mutable_cvvalue ()->set_svvalue (sVvalue.to_string ());
		cVplanner->statm (&cVstatement);
		if (iV %50000 == 0) printf ("ciclo %i\n", iV);
	}

	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("Elapsed: %f\n",tVdiff);
	while (true) {Sleep (10);}
	return 0;
}*/


/*
int _tmain(int argc, _TCHAR* argv[]) {	
	AnsiString sVsubversion;
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff;

  tVstart = __Time ();
	tVdiff = 24*3600;
	if (sizeof (void*) == 8) sVsubversion = "x64";
	else sVsubversion = "x86";
  printf ("Stress %s\n", sVsubversion.c_str ());



	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	CorePeer* cVpeer;
	PrtoL2qr cVquery;
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;
	PrtoL2st cVstatement;	
	PrtoTabl cVtablet;
	PrtoLmtb cVmutable;
	PrtoMktr cVprotomk;
	PrtoMktr cVprotomkout;
	CoreAenp* cVantientrophy;

	FLAGS_log_dir = "c:/templog/templog";
	google::InitGoogleLogging(argv [0]);
	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (	cVpool, 
						"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orioncluster\\node1\\", 
														"Orion.xml",CoreEnpo::iCrunning, false);
	cVplanner = cVorion->getpl ();	
	cVpeer = cVplanner->getpr ();
	*/
	
	/*
	PrtoGoss cGossip;
	PrtoGoel* cVgossipelement;
	PrtoGosr cVgossipresult;
	bool bVdummy;
	cVgossipelement = cGossip.add_cvgossipelement ();
	cVgossipelement->set_svdatacenterid ("DEFAULTDC");
	cVgossipelement->set_svnodeid ("ALTRONODE2");
	cVgossipelement->set_svaddress ("127.0.0.1");
	cVgossipelement->set_ivport (9002);
	cVgossipelement->set_dvphiaccrual (1);
	cVplanner->getpr ()->getgo ()->gossp (&cGossip, &cVgossipresult, bVdummy);
	printf ("%s\n", cVpeer->getgo ()-> debug ().c_str ());	
	*/
	
	/*
	cVantientrophy = new CoreAenp (cVplanner);
	cVplanner->lnch_ (NULL, &CorePlan::touch);
	//cVplanner->pulls ();
	cVantientrophy->rpair ("CLUSTERNODE2", true, true, "DEFAULT/PTOKEN");
	//cVantientrophy->rpair ("ALTRONODE2", true, true, NULL);

	delete cVantientrophy;


	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("Elapsed: %f\n",tVdiff);
	while (true) {Sleep (10);}
	return 0;
}*/


/*
int _tmain(int argc, _TCHAR* argv[]) {	
	AnsiString sVsubversion;
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff;

  tVstart = __Time ();
	tVdiff = 24*3600;
	if (sizeof (void*) == 8) sVsubversion = "x64";
	else sVsubversion = "x86";
  printf ("Stress %s\n", sVsubversion.c_str ());



	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	

	FLAGS_log_dir = "c:/templog/templog";
	google::InitGoogleLogging(argv [0]);
	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (	cVpool, 
						//"C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\Release\\", 
						"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orionfolder2\\",
														"Orion.xml",CoreEnpo::iCrunning, false);
	cVplanner = cVorion->getpl ();	



	PrtoL2st cVstatement;

	printf ("replication factor %i\n", cVplanner->getpr ()->getep ()->getnd ()->iVreplicationfactor); 	
	cVplanner->getpr ()->getep ()->getnd ()->iVreplicationfactor = 1;		
	cVstatement.mutable_cvmutable ()->set_svtable ("TABLEPARTITION");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVplanner->touch (cVstatement.mutable_cvmutable ());
	printf ("res %i\n", cVplanner->purge (cVstatement.mutable_cvmutable ()));
	*/

	/*
	//JOIN
	PrtoGoss cGossip;
	AnsiString sVkey;
	PrtoGoel* cVgossipelement;
	PrtoGosr cVgossipresult;
	cVgossipelement = cGossip.add_cvgossipelement ();
	cVgossipelement->set_svdatacenterid ("DEFAULTDC");
	cVgossipelement->set_svnodeid ("NODE1");
	cVgossipelement->set_svaddress ("127.0.0.1");
	cVgossipelement->set_ivport (9001);
	cVgossipelement->set_dvphiaccrual (1);
	bool bVdummy;
	cVplanner->getpr ()->getgo ()->gossp (&cGossip, &cVgossipresult, bVdummy);


	//SPLIT
	cVplanner->setpt (cVstatement.mutable_cvmutable (), "M", "M");
	cVplanner->frzsp (cVstatement.mutable_cvmutable ());

	//SCRITTURA
	PrtoTabl cVtable;
	PrtoL2cl* cVcolumn;
	cVplanner->iftbl (cVstatement.mutable_cvmutable (), &cVtable);
	for (int iV = 0; iV < cVtable.cvpartitionedelements_size (); iV++) {
		printf ("%i %s %s\n", iV, 
			cVtable.mutable_cvpartitionedelements (iV)->svnodeid ().c_str (),
			((AnsiString) cVtable.mutable_cvpartitionedelements (iV)->svbound ()).prntb ().c_str ());
	}

	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("FIELD");
	
	sVkey = "A";
	cVstatement.mutable_cvkey ()->set_svmain (sVkey.to_string ());
	cVcolumn->mutable_cvvalue ()->set_svvalue (sVkey.to_string ());
	cVplanner->statm (&cVstatement);
	sVkey = "Q";
	cVstatement.mutable_cvkey ()->set_svmain (sVkey.to_string ());
	cVcolumn->mutable_cvvalue ()->set_svvalue (sVkey.to_string ());
	cVplanner->statm (&cVstatement);
	*/


	/*
	//LETTURA
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;
	PrtoL2cl* cVcolumnread;

	cVquery.mutable_cvmutable ()->set_svtable ("TABLEPARTITION");
	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVcolumnread = cVquery.mutable_cvselect ()->Add ();
	cVcolumnread->set_svcolumn ("FIELD");
	cVquery.mutable_cvkey_start ()->set_svmain ("");
	cVquery.set_ivquery (RANGEQUERY);
	cVquery.set_ivcount (3);

	if (cVplanner->query (&cVquery, &cVdmlresult))
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++)
			printf ("*%s* - *%s*\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str (),
			cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->cvvalue ().svvalue ().c_str ()
			);*/


	/*
	//CHECK
	PrtoL2os cVosql;	
	PrtoL2qr cVquery2;
	PrtoL2mr cVdmlresult2;

	cVosql.set_svnamespace ("NAMESPACE");
	cVosql.set_bvonlysecondary (false);
	cVosql.set_svosqlstring ("LOCAL SELECT FROM TABLEPARTITION");

	printf ("\n");
	if (cVplanner->osql_ (&cVosql, &cVdmlresult2))
		for (int iV = 0; iV < cVdmlresult2.cvkeyslice_size (); iV++)
			printf ("*%s* - *%s*\n", cVdmlresult2.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str (),
			cVdmlresult2.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->cvvalue ().svvalue ().c_str ()
			);
	*/

	/*
	//LOCAL
	PrtoLqry cVquery;
	PrtoLval cVvalue;

	cVquery.mutable_cvmutable ()->set_svtable ("TABLEPARTITION");
	cVquery.mutable_cvmutable ()->set_svnamespace ("NAMESPACE");
	cVquery.mutable_cvkey ()->set_svmain ("");		
	cVquery.set_ivquery (TKSLWQUERY);

	if (cVplanner->lcqry (&cVquery, &cVvalue)) printf ("risultato *%s*\n", cVquery.mutable_cvkey ()->svmain ().c_str ());
	printf ("Ris *%s*\n", cVquery.mutable_cvkey ()->svaccessgroup ().c_str ()); 

	if (cVplanner->lcqry (&cVquery, &cVvalue)) printf ("risultato *%s*\n", cVquery.mutable_cvkey ()->svmain ().c_str ());
	cVquery.mutable_cvkey ()->set_svmain ("B");	
	if (cVplanner->lcqry (&cVquery, &cVvalue)) printf ("risultato *%s*\n", cVquery.mutable_cvkey ()->svmain ().c_str ());
	*/

	/*
	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("Elapsed: %f\n",tVdiff);
	while (true) {Sleep (10);}
	return 0;
}*/


/*
int _tmain(int argc, _TCHAR* argv[]) {	
	AnsiString sVsubversion;
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff;

  tVstart = __Time ();
	tVdiff = 24*3600;
	if (sizeof (void*) == 8) sVsubversion = "x64";
	else sVsubversion = "x86";
  printf ("Stress %s\n", sVsubversion.c_str ());

	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	PrtoLmtb cVmutable;

	FLAGS_log_dir = "c:/templog/templog";
	google::InitGoogleLogging(argv [0]);
	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (	cVpool, 
						"C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\Release\\", 
						"Orion.xml",CoreEnpo::iCrunning, false);
	cVplanner = cVorion->getpl ();	
	*/
	

	/*
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;

	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);
	cVosql.set_svosqlstring ("SELECT timestamp FROM PIMTINGCHANNELGEOSTEM LIMIT 2");

  tVstart = __Time ();
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
		printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
	}*/
















	/*
	PrtoGoss cGossip;
	PrtoGoel* cVgossipelement;
	PrtoGosr cVgossipresult;

	cVgossipelement = cGossip.add_cvgossipelement ();
	cVgossipelement->set_svdatacenterid ("DEFAULTDC");
	cVgossipelement->set_svnodeid ("ALTRONODE2");
	cVgossipelement->set_svaddress ("127.0.0.1");
	cVgossipelement->set_ivport (9002);
	cVgossipelement->set_dvphiaccrual (1);
	cVplanner->getpr ()->getgo ()->gossp (&cGossip, &cVgossipresult);
	printf ("%s\n", cVplanner->getpr ()->getgo ()->debug ().c_str ());	
	printf ("%s\n", cVplanner->getpr ()->getep ()->debug ().c_str ());


	CoreEnpo::GOSSIPLOC cVgossipelementprev;
	cVplanner->getpr ()->getgo ()->getge (NULL, cVgossipelementprev, -1);
	printf ("sto cercando *%s*\n", cVgossipelementprev.sVindex.c_str ());
	*/

	/*
	cVplanner->getpr ()->getgo ()->newtk ("a");
	printf ("%s\n", cVplanner->getpr ()->getgo ()->debug ().c_str ());	
	printf ("%s\n", cVplanner->getpr ()->getep ()->debug ().c_str ());

	//comando che fa lo split
	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("PTOKEN");
	cVplanner->split (&cVmutable);

	//comando che esegue il freeze della struttura di split
	cVplanner->frzsp (&cVmutable);*/


	/*
	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("PTOKEN");

	PrtoTabl cVtable;
	printf ("esecuzione %i\n", cVplanner->iftbl (&cVmutable, &cVtable, false));
	printf ("partizioni %i\n", cVtable.cvpartitionedelements_size ());*/

	/*
	Sleep (1000);
	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("PBOX");
	cVplanner->lnch_ (NULL, &CorePlan::touch);
	PrtoTabl cVprototablet;
	printf ("esecuzione %i\n", cVplanner->iftbl ("DEFAULT_PBOX", &cVprototablet));*/

	//cVplanner->touch (&cVmutable);
	//cVplanner->rbldi (&cVmutable);


	//cVplanner->split ("DEFAULT/PBOX");
	//cVplanner->split ();


	/*
	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("PIMTINGCHANNELGEOSTEM");
	cVplanner->touch (&cVmutable);
	cVplanner->rbldi (&cVmutable);*/


	/*
	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("PBOX");
	cVplanner->lnch_ (&cVmutable, &CorePlan::touch);
	cVplanner->lnch_ (&cVmutable, &CorePlan::rbldi);*/

	/*
	cVplanner->lnch_ (NULL, &CorePlan::touch);
	cVplanner->lnch_ (NULL, &CorePlan::rbldi);*/

	/*
	cVplanner->touch ();

	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("PBOX");

	AnsiString sVpartition = "";
	cVplanner->getpt (&cVmutable, false, sVpartition);
	printf ("%s\n", sVpartition.c_str ());
	printf ("controprova %f\n", cVplanner->getpt (&cVmutable, false, sVpartition));	*/



	/*
	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("Elapsed: %f\n",tVdiff);
	while (true) {Sleep (10);}
	return 0;
}*/


/*
int _tmain(int argc, _TCHAR* argv[]) {	
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	PrtoL2qr cVquery;
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;
	PrtoL2st cVstatement;

	FLAGS_log_dir = "c:/templog/templog";
	google::InitGoogleLogging(argv [0]);
	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (	cVpool, 
						"C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\Release\\", 
														"Orion.xml",CoreEnpo::iCrunning, false);

	cVplanner = cVorion->getpl ();
	*/
	

	/*
	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);

	cVosql.set_svosqlstring ("INSERT INTO POWNERR2 (TINGKEY,BIRTHDAY,EMAIL,KEY,LANGUAGE,LASTUPDATE,PASSWORD,USERNAME) SELECT TINGKEY,BIRTHDAY,EMAIL,KEY,LANGUAGE,LASTUPDATE,PASSWORD,USERNAME FROM POWNER LIMIT 1000000");
	printf ("success %i\n", cVplanner->osql_ (&cVosql, &cVdmlresult));
	*/

	/*
	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);

	cVosql.set_svosqlstring ("INSERT INTO POWNERR2 (KEY, TINGKEY) VALUES ('TEST', 'TEST2')");
	printf ("success %i\n", cVplanner->osql_ (&cVosql, &cVdmlresult));
	
	cVosql.Clear ();
	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);
	cVosql.set_svosqlstring ("SELECT FROM POWNERR2 LIMIT 500");	
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s  - %s\n", 
			cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str (),
			cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str());
	}
	*/

	//osql
	/*
	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);

	PrtoL2cv* cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("_giancarlo");
	cVcolumnvalue = cVosql.add_cvinputbindings ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("201301021558510000223");

	cVosql.set_svosqlstring ("SELECT FROM PCHATR2 WHERE tingKey = :0  AND  timestamp < :1 ORDER BY timestamp desc  LIMIT 5");
	
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s  - %s\n", 
			cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str (),
			cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str());
	}*/


	//SELECT FROM PCHATR2 WHERE tingKey = '_giancarlo'  AND  timestamp < '201301021558510000223' ORDER BY timestamp desc  LIMIT 5

	/*
	PrtoL2cl* cVcolumn;
	PrtoL2cv* cVcolumnvalue;
	PrtoL2wh* cVwhere;
	PrtoL2cl* cVcondition;

	cVquery.mutable_cvmutable ()->set_svtable ("PCHATR2");
	cVquery.mutable_cvmutable ()->set_svnamespace ("DEFAULT");
	cVquery.set_ivcount (5);
	
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("KEY"); 	
	
	cVwhere = cVquery.mutable_cvwhere ();
	
	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("TINGKEY");
	cVcondition->set_ivconditiontype (ECOND);	
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("_giancarlo");

	cVcondition = cVwhere->add_cvcondition ();
	cVcondition->set_svcolumn ("TIMESTAMP");
	cVcondition->set_ivconditiontype (LECOND);	
	cVcolumnvalue = cVcondition->mutable_cvvalue ();
	cVcolumnvalue->set_ivtype (STRINGTYPE);
	cVcolumnvalue->set_svvalue ("201301021558510000223");


	

	//if (cVplanner->query (&cVquery, &cVdmlresult)) {
	if (CoreTutl::sendq ("127.0.0.1", 9002, &cVquery, &cVdmlresult)) {
		for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
			printf ("%s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
			printf ("valore %s\n", 
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str ());
		}
		printf ("%i\n", cVdmlresult.cvkeyslice_size ());
	}	



	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;

}*/

/*
int _tmain(int argc, _TCHAR* argv[]) {	
	CoreOrn_* cVorion;
	CorePool* cVpool;
	CorePlan* cVplanner;	
	PrtoL2qr cVquery;
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;
	PrtoL2st cVstatement;

	FLAGS_log_dir = "c:/templog/templog";
	google::InitGoogleLogging(argv [0]);
	cVpool = new CorePool ();
	cVorion = new CoreOrn_ (	cVpool, 
						"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orioncluster\\node1\\", 
														"Orion.xml",CoreEnpo::iCrunning, false);

	cVplanner = cVorion->getpl ();
	*/

	/*
	PrtoGoss cGossip;
	PrtoGoel* cVgossipelement;
	PrtoGosr cVgossipresult;
	CoreEnpo::GOSSIPLOC cVgossipelementprev;
	bool bVdummy;

	cVgossipelement = cGossip.add_cvgossipelement ();
	cVgossipelement->set_svdatacenterid ("DEFAULTDC");
	cVgossipelement->set_svnodeid ("CLUSTERNODE2");
	cVgossipelement->set_svaddress ("127.0.0.1");
	cVgossipelement->set_ivport (9052);
	cVgossipelement->set_dvphiaccrual (1);
	cVplanner->getpr ()->getgo ()->gossp (&cGossip, &cVgossipresult, bVdummy);	*/

	/*
	cVplanner->lnch_ (NULL, &CorePlan::trunc);

	delete cVorion;
	printf ("valore pool %i\n", cVpool->getma ());
	delete cVpool;
}*/





//TERZA SEZIONE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





















	/*
	PTR_POOL cVpool (new CorePool ());
	PTR_CUSTOMSORTEDINDEX cVindex (new BASE_CUSTOMSORTEDINDEX (cVpool));
	PTR_FILESYSTEM cVfsys (new CoreFsys (cVpool));
	CoreHsrx cVhashradix (cVpool);*/


	/*
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
	printf ("test %s\n", cVhashradix.debug ().c_str ());*/
	

	/*
	cVhashradix.tocnt (cVfsys, CoreHsrx::iCsstable, "");
	cVfsys->owrte ("E:\\Tomting\\test\\file.idx");*/

	//cVhashradix.tocnt ("E:\\Tomting\\test\\file2.idx", CoreHsrx::iCsstable, "");

	/*
	cVfsys->read_ ("E:\\Tomting\\test\\file.idx");
	cVfsys->setpo (0);
	CoreHsrx::iCsstabletypes iVtabletype;
	AnsiString sVredolog;
	cVhashradix.fmcnt (cVfsys, iVtabletype, sVredolog);
	printf ("test %s\n", cVhashradix.debug ().c_str ());
	*/


	/*
	CoreHsrx::iCsstabletypes iVtabletype;
	AnsiString sVredolog;
	unsigned int iVsize;

	cVhashradix.fmcnt ("E:\\Tomting\\test\\file2.idx", iVtabletype, sVredolog, iVsize);
	printf ("test %s\n", cVhashradix.debug ().c_str ());
	*/

	/*
	AnsiString sVtest = "ORNI";
	printf ("%i\n", sVtest.Length ());*/
	
	/*
	CoreHsrx::iCsstabletypes iVtabletype;
	AnsiString sVredolog;
	unsigned int iVsize;

	for (int iV = 0; iV < 1000; iV++) {
		CoreHsrx cVhashradix (cVpool);
		cVhashradix.fmcnt ("E:\\Orion\\orionfolder\\Data\\DEFAULT\\PIMBOXATOM_676.IDX", iVtabletype, sVredolog, iVsize);
		printf ("test %i\n", cVhashradix.getsz ());
	}*/
	

	//lettura folder, tutti i files

	/*
	AnsiString sVwhere = "E:\\Orion\\orionfolder\\Data\\DEFAULT\\";
	AnsiString sVtempfilename;
	CoreList<AnsiString> cVfilenames;
	CoreHsrx::iCsstabletypes iVtabletype;
	AnsiString sVredolog;
	int64vcl iVsize;
	PTR_POOL cVpool (new CorePool ());

	CoreHutl::readr (sVwhere, (AnsiString) "*.IDX", cVfilenames);
	for (int iV = 0; iV < cVfilenames.count (); iV++) {		
		sVtempfilename = *cVfilenames.get__ (iV);
		CoreHsrx cVhashradix (cVpool);
		cVhashradix.fmcnt (sVwhere + sVtempfilename, iVtabletype, sVredolog, iVsize);
		printf ("%s %i...", sVtempfilename.c_str (), cVhashradix.getsz ());
		cVhashradix.tocnt (sVwhere + sVtempfilename, iVtabletype, sVredolog, iVsize, 1);
		printf ("%i\n", iVsize);
	}*/


	/*
	FLAGS_log_dir = "c:/templog/templog";
	google::InitGoogleLogging(argv [0]);

	PTR_POOL cVpool (new CorePool);


	PTR_ORION cVorion (new CoreOrn_ (	cVpool, 
						"C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\x64\\Release\\", 
						//"E:\\Orion\\oriondistributed\\node3\\",
						"Orion.xml",CoreEnpo::iCrunning, false));
	PTR_PLANNER cVplanner = cVorion->getpl ();	


	//CoreSstb::debug ("E:\\Orion\\orionfolder\\Data\\DEFAULT\\", "TESTSSTABLE_341");
	//CoreSstb::dbgix(cVpool, "E:\\Orion\\orionfolder\\Data\\DEFAULT\\", "TESTSSTABLE_341");


	cVplanner->lnch_ (NULL, &CorePlan::touch);
	printf ("valore %i\n", cVplanner->getsz ());
	//cVplanner->lnch_ (NULL, &CorePlan::rbldi);
	*/
	
	
	/*
	PrtoL2cl* cVcolumn;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;

		
	cVquery.mutable_cvmutable ()->set_svtable ("TESTSSTABLE");
	cVquery.mutable_cvmutable ()->set_svnamespace ("DEFAULT");
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("FIELD"); 	
	

	for (int iVouter = 25989; iVouter <= 25990; iVouter++) {
	//for (int iVouter = 30000; iVouter <= 30000; iVouter++) {
		AnsiString sVkey = (AnsiString) "key_" + (AnsiString) iVouter;
		cVquery.mutable_cvkey_start ()->set_svmain (sVkey.to_string ());
		cVquery.set_ivquery (EXACTQUERY);
		cVquery.set_bvfulltablescan (false);
		cVquery.set_bvonlysecondary (false);
		cVquery.set_ivcount (1);

	
		if (cVplanner->query (&cVquery, &cVdmlresult)) {
			for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
				printf ("%i *%s* - *%s*\n", 
				iVouter,
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str (),
				cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->cvvalue ().svvalue ().c_str ()
				);
				if (cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->cvvalue ().svvalue () != "value_2") {
					printf ("grave\n");
					while (true) Sleep (1000);
				}
			}
		}

	}	*/
	

	//cazzo
	/*
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;




	//osql
	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);
	cVosql.set_bvdisableteleport (false);
	cVosql.set_bvfulltablescan (true);
	cVosql.set_svosqlstring ("SELECT FROM MEDUSA LIMIT 10");
	//cVosql.set_svosqlstring ("SELECT FROM PCOMMENT WHERE parentKey = 'PBOX_232ddb40-3ed9-11e3-b41f-000c293e9cf9'");

	Sleep (1000);

  tVstart = __Time ();
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
	}


	printf ("sleep\n");
	while (true) Sleep (1000);
	*/





	//inserire in hashradix i valori
	/*
	PTR_POOL cVpool (new CorePool ());
	PTR_CUSTOMSORTEDINDEX cVindex (new BASE_CUSTOMSORTEDINDEX (cVpool));
	PTR_FILESYSTEM cVfsys (new CoreFsys (cVpool));
	CoreHsrx cVhashradix (cVpool);
	int64vcl iVsize;

	for (int iV = 0; iV < 100000; iV++) {
		AnsiString sVtemp = (AnsiString) "key_" + (AnsiString) iV;
		cVindex->add__ (sVtemp, iV, sVtemp.Length ());
	}
	cVhashradix.build (cVindex);
	printf ("test %s\n", cVhashradix.debug ().c_str ());

	//interrogare un valore
	printf ("%lld\n", cVhashradix.fndrx ("key_99735"));


	cVhashradix.tocnt ("E:\\Tomting\\test\\file2.idx", CoreHsrx::iCsstable, "", iVsize);
	CoreHsrx cVhashradixread (cVpool);
	CoreHsrx::iCsstabletypes iVsstabletype;
	AnsiString sVredotimestamp;
	cVhashradixread.fmcnt ("E:\\Tomting\\test\\file2.idx", iVsstabletype, sVredotimestamp, iVsize);
	printf ("%lld\n", cVhashradixread.fndrx ("key_99735"));
	//key_99735# 99707 9
	*/

	//memtable
	/*
	FLAGS_log_dir = "c:/templog/templog";
	google::InitGoogleLogging(argv [0]);*/


	/*
	PTR_POOL cVpool (new CorePool ());
	CoreSstb::PTR cVsstable [2];
	int iVcount = 0;
	PrtoLval cVvalue;
	AnsiString sVredotimestamp;
	CoreHsrx::iCsstabletypes iVtype;
	cVsstable [0] = CoreSstb::PTR (new CoreSstb ());
	cVsstable [1] = CoreSstb::PTR (new CoreSstb ());

	for (int iVtable = 0; iVtable < 2; iVtable++) {
		CoreMeot::PTR cVmemtable (new CoreMeot (cVpool, "", 100, 10000, 100));
		cVvalue.set_ivtimestamp (0);
	
		for (int iV = 0; iV < 1000; iV++) {
			AnsiString sVkey = (AnsiString) "key_" + (AnsiString) iVcount;
			cVvalue.set_svopaquevalue (((AnsiString) iVcount).to_string ());
			cVmemtable->put__ (sVkey, sVkey.Length (), &cVvalue);
			iVcount++;
		}
		printf ("elem %i\n", cVmemtable->getsc ());
	
		//sstable
		cVsstable [iVtable]->init (cVpool);
		cVsstable [iVtable]->setmd ("E:\\Tomting\\test\\", (AnsiString) "testsstable_" + (AnsiString) iVtable);
		cVsstable [iVtable]->saved (cVmemtable, CoreHsrx::iCsstable, "");
		cVsstable [iVtable]->loadd (iVtype, sVredotimestamp);
	}

	//compaction
	CoreSstb cVsstablecomp (cVpool);
	cVsstablecomp.setmd ("E:\\Tomting\\test\\", "compacted");
	cVsstablecomp.majcp (cVsstable [0], cVsstable [1], "");

	//read
	CoreSstb cVsstableread;
	cVsstableread.init (cVpool);
	cVsstableread.setmd ("E:\\Tomting\\test\\", "compacted");
	cVsstableread.loadd (iVtype, sVredotimestamp);

	printf ("letti da compacted %i\n", cVsstableread.getsc ());
	iVcount = 0;
	for (int iVtable = 0; iVtable < 2; iVtable++) {
		for (int iV = 0; iV < 1000; iV++) {
			AnsiString sVkey = (AnsiString) "key_" + (AnsiString) iVcount;
			if (!cVsstableread.get__ (sVkey, &cVvalue)) {
				printf ("GRAVE\n");
				break;
			} else printf ("trovato %s\n", cVvalue.svopaquevalue ().c_str ());
			

			iVcount++;		
		}
	}

	printf ("end\n");
	*/




	//prendere la sstable
	/*
	PTR_POOL cVpool (new CorePool ());
	CoreSstb cVsstableread;
	AnsiString sVredotimestamp;
	CoreHsrx::iCsstabletypes iVtype;
	cVsstableread.init (cVpool);
	//cVsstableread.setmd ("E:\\Tomting\\test\\", "compacted");
	cVsstableread.setmd ("E:\\Orion\\orionfolder\\Data\\DEFAULT\\", "TESTSSTABLE_341");
	
	
	cVsstableread.loadd (iVtype, sVredotimestamp);

	printf ("letti da compacted %i\n", cVsstableread.getsc ());

	printf ("%i\n", cVsstableread.hshok ());*/













	//printf ("comando leave %i\n", cVplanner->leave (true));
	/*
	PrtoLmtb cVmutable;
	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("CLUS");

	printf ("comando rebuildpart %i\n", cVplanner->rbpar (&cVmutable));
	Sleep (1000);

	printf ("uscito\n");*/



	/*
	PrtoLmtb cVmutablesource;
	PrtoLmtb cVmutabledestination;
	int64vcl iVcount;

	cVmutablesource.set_svnamespace ("DEFAULT");
	cVmutablesource.set_svtable ("PMEDIA");
	
	cVmutabledestination.set_svnamespace ("DEFAULT");
	cVmutabledestination.set_svtable ("PMEDIA2");

	cVplanner->clone (&cVmutablesource, &cVmutabledestination, 0, iVcount);*/

	


	/*
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;

	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);
	cVosql.set_ivtimestamp (CoreButl::tmstl ());
	cVosql.set_bvfulltablescan (true);



	//cVosql.set_svosqlstring ("DELETE FROM POWNERR2 WHERE tingkey = '_peppemas' LIMIT 1");
	//printf ("result %i\n", cVplanner->osql_ (&cVosql, &cVdmlresult));
	

	//cVosql.set_svosqlstring ("select from POWNERR2 WHERE tingkey = '_peppemas' LIMIT 1");
	//cVosql.set_svosqlstring ("select from POWNERR2");
	cVosql.set_svosqlstring ("select from clus");


	
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());

		//printf ("colonne %i\n", cVdmlresult.mutable_cvkeyslice (iV)->cvcolumns_size ());
		//printf ("timestamp: %lld\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->ivtimestamp ());
		//printf ("state: %i\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->ivstate ());
	}*/

	/*
	cVplanner.reset ();
	cVorion.reset ();
	cVpool.reset ();*/

	/*
	PrtoL2cl* cVcolumn;
	PrtoL2qr cVquery;
	PrtoL2mr cVdmlresult;

	cVquery.mutable_cvmutable ()->set_svtable ("POWNERR2");
	cVquery.mutable_cvmutable ()->set_svnamespace ("DEFAULT");
	cVquery.mutable_cvkey_start ()->set_svmain ("_peppemas");

	cVquery.set_ivquery (RANGEQUERY);
	cVquery.set_ivcount (1);
	cVcolumn = cVquery.mutable_cvselect ()->Add ();
	cVcolumn->set_svcolumn ("KEY");

	cVplanner->query (&cVquery, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
		printf ("colonne %i\n", cVdmlresult.mutable_cvkeyslice (iV)->cvcolumns_size ());
		printf ("timestamp: %lld\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->ivtimestamp ());
		printf ("state: %i\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->ivstate ());
	}*/

	/*
	PrtoL2os cVosql;
	PrtoL2mr cVdmlresult;

	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);
	cVosql.set_ivtimestamp (CoreButl::tmstl ());
	cVosql.set_bvfulltablescan (true);

	cVosql.set_svosqlstring ("select from pimtingchannelgeostem where key = '_peppemas_it_test'");

	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
		printf ("colonne %i\n", cVdmlresult.mutable_cvkeyslice (iV)->cvcolumns_size ());
		printf ("timestamp: %lld\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->ivtimestamp ());
		printf ("state: %i\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->ivstate ());
	}

	cVosql.set_svosqlstring ("select from pimtingchannelgeostem where boxkey = 'PBOX_6807fce0-adc6-11e2-b907-50465d4d2220'");

	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
		printf ("colonne %i\n", cVdmlresult.mutable_cvkeyslice (iV)->cvcolumns_size ());
		printf ("timestamp: %lld\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->ivtimestamp ());
		printf ("state: %i\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->ivstate ());
	}

	cVosql.set_svosqlstring ("select from pimtingchannelgeostem where boxkey = 'PBOX_4dbb58d0-496e-11e2-9dac-001c14014395'");

	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str ());
		printf ("colonne %i\n", cVdmlresult.mutable_cvkeyslice (iV)->cvcolumns_size ());
		printf ("timestamp: %lld\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->ivtimestamp ());
		printf ("state: %i\n", cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->ivstate ());
	}*/


	


	/*
	cVosql.set_svosqlstring ("UPDATE PBOX SET CHANNELKEY = '_it_test' WHERE key = 'PBOX_02cd6770-beb5-11e2-8687-50465d4d2220' LIMIT 1");
	printf ("result %i\n", cVplanner->osql_ (&cVosql, &cVdmlresult));*/


	//cVosql.set_svosqlstring ("INSERT INTO PBOXR3 (address,channelKey,countComments,isTingOwner,key,lat,lng,ownedTingKey,ownedTingName,previewMediaContent,previewMediaLink,previewMediaType,ratings,reservedChannel,timestamp,tingCommenterBarrel,tingDislikeBarrel,tingKey,tingKeyBarrel,tingLikeBarrel,tingName,tingNameBarrel) SELECT address,channelKey,countComments,isTingOwner,key,lat,lng,ownedTingKey,ownedTingName,previewMediaContent,previewMediaLink,previewMediaType,ratings,reservedChannel,timestamp,tingCommenterBarrel,tingDislikeBarrel,tingKey,tingKeyBarrel,tingLikeBarrel,tingName,tingNameBarrel FROM PBOXR2 LIMIT  1");
	/*
	cVosql.set_svosqlstring ("INSERT INTO PBOXR2 (key,lat,lng,address) SELECT KEY,LAT,LNG,ADDRESS FROM PBOX LIMIT  1");
	printf ("success %i\n", cVplanner->osql_ (&cVosql, &cVdmlresult));
	*/

	/*
	cVosql.Clear ();
	cVosql.set_svnamespace ("DEFAULT");
	cVosql.set_bvonlysecondary (false);
	cVosql.set_svosqlstring ("SELECT FROM POWNERR2 LIMIT 500");	
	cVplanner->osql_ (&cVosql, &cVdmlresult);
	for (int iV = 0; iV < cVdmlresult.cvkeyslice_size (); iV++) {
		printf ("chiave (osql): %s  - %s\n", 
			cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvkey ()->svmain ().c_str (),
			cVdmlresult.mutable_cvkeyslice (iV)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ().c_str());
	}*/

	/*
	PrtoLmtb cVmutable;
	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("PBOX");
	//cVmutable.set_svtable ("PCHANNELR2");
	//cVplanner->rpair (&cVmutable);
	cVplanner->touch (&cVmutable);
	//cVplanner->rbldi (&cVmutable);
	//printf ("risultato %i\n", cVplanner->soshr (&cVmutable));

	cVplanner->fcomp (&cVmutable);
	printf ("fin\n");
	//cVplanner->lnch_ (NULL, &CorePlan::touch);
	//cVplanner->lnch_ (NULL, &CorePlan::shrnk);
	*/


	/*
	PrtoLmtb cVmutable;
	cVmutable.set_svnamespace ("DEFAULT");
	cVmutable.set_svtable ("%MEDUSA");
	cVplanner->lnch_ (&cVmutable, &CorePlan::touch);

	int iVcounter;
	PTR_REVERSE cVreverse (new CoreRvrs ());
	PrtoL2mr cVdmlresult;
	PrtoL2st cVstatement;
	PrtoL2cl* cVcolumn;

	cVstatement.mutable_cvmutable ()->set_svtable ("MEDUSA");
	cVstatement.mutable_cvmutable ()->set_svnamespace ("DEFAULT");
	cVstatement.mutable_cvkey ()->set_ivstate (UPSERT);
	cVstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("VALUE");
	cVcolumn = cVstatement.mutable_cvcolumns ()->Add ();
	cVcolumn->set_svcolumn ("TIMESTAMP");
	
  tVstart = __Time ();
	for (int iV = 0; iV < 3; iV++) {	
		AnsiString sVtemp = (AnsiString) "KEY" + (AnsiString) iV;
		cVstatement.mutable_cvkey ()->set_svmain (sVtemp.to_string ());
		cVstatement.mutable_cvcolumns (0)->mutable_cvvalue ()->set_svvalue (sVtemp.to_string ());
		cVstatement.mutable_cvcolumns (1)->mutable_cvvalue ()->set_svvalue (CoreAlog::logrs (cVreverse , CoreAlog::getix (cVreverse, iVcounter)).c_str ());
		cVplanner->statm (&cVstatement);
	}
  tVend = __Time ();



	printf ("%s\n", cVplanner->gettb (&cVmutable)->debug ().c_str ());

	//cancellazione
	cVstatement.mutable_cvkey ()->set_ivstate (DELTMB);
	cVstatement.mutable_cvkey ()->set_svmain ("KEY1");
	cVplanner->statm (&cVstatement);*/


	//printf ("%s\n", cVplanner->gettb (&cVmutable)->debug ().c_str ());


	/*
	cVplanner.reset ();
	cVorion.reset ();

	cVorion = boost::shared_ptr<CoreOrn_> (new CoreOrn_ (	cVpool, 
						"C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\Release\\", 
						"Orion.xml",CoreEnpo::iCrunning, false));
	cVorion.reset ();
	cVpool.reset ();
	*/


//void stress_thrift () {	
//
//  boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9051));
//  boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
//  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
//
//  ThrfOrn_Client client(protocol);
//
//  transport->open();	
//	client.ping ();	
//  transport->close();
//}
//
//
//	typedef struct testList {
//		AnsiString sVindex;
//
//		int iVvalue;
//
//		/*INITIALIZE*/
//		testList () {
//			printf ("creazione\n");
//		}
//
//		~testList () {
//			printf ("distruzione\n");
//		}
//	} TESTLIST;
//
//typedef boost::shared_ptr<TESTLIST> PTR_TESTLIST;
//
//void testdistruttori () {
//	PTR_POOL cVpool (new CorePool);
//	PTR_ORION cVorion (new CoreOrn_ (	cVpool, 
//						"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orioncluster\\node1\\", 
//						"Orion.xml",CoreEnpo::iCrunning, false));
//	PTR_PLANNER cVplanner = cVorion->getpl ();	
//	
//	Sleep (1000);
//	cVplanner.reset ();
//	cVorion.reset ();
//	cVpool.reset ();
//}

