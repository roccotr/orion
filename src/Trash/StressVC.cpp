#ifdef iCdebugvld_
	#include <vld.h> 


	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif


#include "stdafx.h"
#include "IfceOrn_.hpp"


typedef struct testoo2 {
	AnsiString sVindex;
	int iVvalue;
} TESTOO2;


typedef boost::shared_ptr<TESTOO2> PTR_TESTOO2;



int _tmain(int argc, _TCHAR* argv[]) {	


#ifdef _DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF; 
	_CrtSetDbgFlag(flag);
	//_CrtSetBreakAlloc(2580);
#endif
	


	
	AnsiString sVsubversion;
	TDateTime tVstart;
  TDateTime tVend;
  double tVdiff;

	tVdiff = 24*3600;
	if (sizeof (void*) == 8) sVsubversion = "x64";
	else sVsubversion = "x86";
	printf ("Stress %s\n", sVsubversion.c_str ());

	tVstart = __Time ();
	
	//IFACE
	/*
	{

		using namespace orion;
		
		Instance db;
		Options options;

		options.log_path = "c:\\templog\\templog";
		options.error_if_exists = true;
		DB::DestroyDB (options, "E:\\Orion\\orionfoldernew\\test\\");
		db = DB::Open (options, "E:\\Orion\\orionfoldernew\\test\\");

		struct StorageOptions storageOptions (L1);
		DB::setVolatile (storageOptions);
		db->Create (storageOptions);		


		Status s = db->Put (WriteOptions (L1), "test", "value");
		string value;
		if (s.ok ()) s = db->Get (ReadOptions (L1), "test", &value);
		if (s.ok ()) printf ("result prima di delete: %s\n", value.c_str ());
		db->Delete (WriteOptions (L1), "test");
		if (s.ok ()) s = db->Get (ReadOptions (L1), "test", &value);
		if (s.ok ()) printf ("result dopo delete: %s\n", value.c_str ());
		
		

		orion::DB::Shutdown ();
		
	}*/


	//BUG MASTRANGELO L1
	/*
	{

		using namespace orion;
		
		Instance db;
		Options options;

		options.log_path = "c:\\templog\\templog";
		options.error_if_exists = true;
		DB::DestroyDB (options, "E:\\Orion\\orionfoldernew\\test\\");
		db = DB::Open (options, "E:\\Orion\\orionfoldernew\\test\\");

		struct StorageOptions storageOptions (L1);
		//DB::setVolatile (storageOptions);
		db->Create (storageOptions);		
		Status s;
		string value;

		for (int iV = 0; iV < 20; iV++) {
			printf ("test %i\n", iV);
			s = db->Put (WriteOptions (L1), "test", "porca puttana");
			if (s.ok ()) s = db->Get (ReadOptions (L1), "test", &value);
			if (s.ok ()) printf ("lettura 1: %s\n", value.c_str ());
			db->Delete (WriteOptions (L1), "test");
			if (s.ok ()) s = db->Get (ReadOptions (L1), "test", &value);
			if (s.ok ()) printf ("lettura (sbagliata 1): %s\n", value.c_str ());		

			s = db->Put (WriteOptions (L1), "test", "puttana eva");
			if (s.ok ()) s = db->Get (ReadOptions (L1), "test", &value);
			if (s.ok ()) printf ("lettura 2: %s\n", value.c_str ());		
			db->Delete (WriteOptions (L1), "test");
			if (s.ok ()) s = db->Get (ReadOptions (L1), "test", &value);
			if (s.ok ()) printf ("lettura (sbagliata 2): %s\n", value.c_str ());	
		}

		orion::DB::Shutdown ();
		
	}
	*/



	//LEAKS
	/*
	{
		FLAGS_log_dir = "c:\\templog\\templog";
		google::InitGoogleLogging(argv [0]);
		PTR_POOL cVpool (new CorePool);
		PTR_ORION cVorion (new CoreOrn_ (	cVpool, 
							"E:\\Orion\\orionfolder2\\", 
							//"C:\\Program Files (x86)\\Borland\\CBuilder6\\Projects\\orion\\trunk\\Ide10\\Orion\\x64\\Release\\",
							//"C:\\Users\\giancarlo\\Desktop\\old desktop\\mastrangelo\\orion\\orioncluster\\node1\\",
							"Orion.xml",CoreEnpo::iCrunning, false));
		
		PTR_PLANNER cVplanner = cVorion->getpl ();	

		cVplanner->lnch_ (NULL, &CorePlan::touch);
		printf ("valore %i\n", cVplanner->getsz ());

		google::ShutdownGoogleLogging();
		google::protobuf::ShutdownProtobufLibrary();
	}*/


	//BENCHMARK L1
	
	{
		//#define iVwrites 10000
		//#define iVwrites 50000
		//#define iVwrites 1
		#define iVwrites 1000000


		AnsiString sVkey;
		AnsiString sVbackupkey;
		PTR_POOL cVpoolmeot (new CorePool ());
		CoreMeot cVtestmeot (cVpoolmeot, "io", 10000000, 100000000, 10000000);
		PrtoLval cVvalue;
		cVvalue.set_ivtimestamp (0);
		tVstart = __Time ();  
		for (int iV = 0; iV < iVwrites; iV++) {
			AnsiString sVkey = (AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7); 				
			cVvalue.set_svopaquevalue (((AnsiString) iV).to_string ());
			cVtestmeot.put__ (sVkey, 12, &cVvalue);
		}
		tVend = __Time (); 
		double dVqpsmeot = (double) iVwrites / (tVdiff * (tVend - tVstart));
		printf ("%i insert forward meot:    %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), dVqpsmeot); 
		cVtestmeot.clear ();


		using namespace orion;
		Instance db;
		Options options;
		options.log_path = "c:\\templog\\templog";
		DB::DestroyDB (options, "E:\\Orion\\orionfoldernew\\test\\");
		db = DB::Open (options, "E:\\Orion\\orionfoldernew\\test\\");

		struct StorageOptions storageOptions (L1);
		DB::setVolatile (storageOptions);
		db->Create (storageOptions);		
		struct WriteOptions writeOptions (L1);

		tVstart = __Time ();  
		for (int iV = 0; iV < iVwrites; iV++) {
			AnsiString sVkey = (AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7); 		
			db->Put (writeOptions, sVkey.to_string (), ((AnsiString) iV).to_string ());
		}
		tVend = __Time (); 
		double dVqpsl1 = (double) iVwrites / (tVdiff * (tVend - tVstart));
		printf ("%i insert forward L1:    %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart))); 
		printf ("rendimento %f\n", dVqpsl1 / dVqpsmeot * 100);
	}


	//BENCHMARK
	/*
	{
		#define iVdim 16
		#define iVwrites 10000000
		CoreOo2w<TESTOO2, iVdim> cVtest;    
		CoreList<TESTOO2> cVtestcrash;

		printf ("velocita' %i\n", iVdim);
		tVstart = __Time (); 
		for (int iV = 0; iV < iVwrites; iV++) cVtest.bnset ((AnsiString) iV)->iVvalue = iV;    
		tVend = __Time (); 
		printf ("%i scritture forward:      %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));

		tVstart = __Time ();  
		for (int iV = 0; iV <= iVwrites; iV++)  cVtest.bnuns ((AnsiString) iV);
		tVend = __Time (); 
		printf ("%i cancellazioni forward:  %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));
	 

		tVstart = __Time (); 
		for (int iV = iVwrites - 1; iV >= 0; iV--) cVtest.bnset ((AnsiString) iV)->iVvalue = iV;    
		tVend = __Time (); 
		printf ("%i scritture reverse:      %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));

		tVstart = __Time ();  
		for (int iV = iVwrites - 1; iV >= 0; iV--)  cVtest.bnuns ((AnsiString) iV );
		tVend = __Time (); 
		printf ("%i cancellazioni reverse:  %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));


		for (int iV = 0; iV < iVwrites; iV++) cVtest.bnset (CoreButl::lpad_ ((AnsiString) iV, '0', 7))->iVvalue = iV;
		CoreOo2w<TESTOO2,32>::OO2POS iV = cVtest.first (); 
		int iVcycle = 0;
		for (;;iV++) {
			int iVtest = cVtest.get__ (iV)->iVvalue;
			if ((iVcycle != iVtest) || (iVtest == iVwrites - 1)) break; 
			iVcycle++;
		}
		if (iVcycle != iVwrites - 1) printf ("risultato atteso:          %i\n", iVcycle);
		iV = cVtest.first (); 
		iVcycle = 0;
		tVstart = __Time ();  
		for (;iV < cVtest.last_ ();iV++) {
			int iVtest = cVtest.get__ (iV)->iVvalue;
			if (iVtest == 99999) break;
			iVcycle++;
		}
		tVend = __Time (); 
		printf ("%i ricerche range:         %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));
		for (int iV = 0; iV < iVwrites; iV++) cVtest.bnuns (CoreButl::lpad_ ((AnsiString) iV, '0', 7));

		for (int iV = 0; iV < iVwrites; iV++) cVtest.bnset ((AnsiString) iV)->iVvalue = iV;   
		tVstart = __Time ();  
		for (int iV = 0; iV < iVwrites; iV++) cVtest.bnget ((AnsiString) iV);  
		tVend = __Time (); 
		printf ("%i ricerche random:        %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));   

		for (int iV = 0; iV < iVwrites; iV++) cVtest.bnuns ((AnsiString) iV);   

		PTR_POOL cVpoolsoix (new CorePool());
		CoreSoix cVtestsoix (cVpoolsoix); 
		CoreStib::DATALOC cVdataloc;
		cVdataloc.iVsize = 0;
		cVdataloc.vVdata = NULL;
		tVstart = __Time ();  
		for (int iV = 0; iV < iVwrites; iV++) {
			AnsiString sVkey = (AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7); 				
			cVtestsoix.addo2 (sVkey, 12, &cVdataloc);
		}
		tVend = __Time (); 
		printf ("%i insert forward soix:    %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));  
		cVtestsoix.clear ();

		AnsiString sVkey;
		AnsiString sVbackupkey;
		PTR_POOL cVpoolmeot (new CorePool ());
		CoreMeot cVtestmeot (cVpoolmeot, "io", 10000000, 100000000, 10000000);
		PrtoLval cVvalue;
		cVvalue.set_ivtimestamp (0);
		tVstart = __Time ();  
		for (int iV = 0; iV < iVwrites; iV++) {
			AnsiString sVkey = (AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7); 				
			cVvalue.set_svopaquevalue (((AnsiString) iV).to_string ());
			cVtestmeot.put__ (sVkey, 12, &cVvalue);
		}
		tVend = __Time (); 
		printf ("%i insert forward meot:    %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart))); 
		cVtestmeot.clear ();
	}
	*/










  tVend = __Time ();
  tVdiff = tVdiff * (tVend - tVstart);
  printf ("Elapsed: %f\n",tVdiff);


#ifndef iCdebugvld
	while (true) {Sleep (10);}
#else



#endif



	return 0;
}
