#include "IfceOrn_.hpp"
#include "CoreXml_.hpp"
#include "CoreHutl.hpp"
#include "DataXml_.hpp"

using namespace orion; 
using namespace google;
using namespace google::protobuf;

	CoreSmph cVglobalmutex;
	int iVlibraryrefs = 0;

	const int DB::VaEOF = -1;

	DB::DB (Options& cOptions, string sPath) {

		cVpool = PTR_POOL (new CorePool ());
		FLAGS_log_dir = cOptions.log_path;

		cVglobalmutex.locks ();
		if (iVlibraryrefs == 0) InitGoogleLogging("");
		iVlibraryrefs++;
		cVglobalmutex.relse ();
		PTR_XML_ cVdocument = PTR_XML_ (new CoreXml_ ());
		AnsiString sVmasterxml = sPath + cOptions.master_xml;
		if (!CoreHutl::ispvl (sPath)) CoreHutl::crdir (sPath);
		if ( cVdocument->getfc (CoreButl::getff (cVpool, sVmasterxml))->getcd(DataXml_::sCtagorion_) != NULL) {
			if (cOptions.error_if_exists) {
				LOG(WARNING) << "Exists: " << cOptions.master_xml.c_str ();		
				FlushLogFiles(GLOG_WARNING);		
				return;
			}
		} else cVdocument->setrt (DataXml_::sCtagorion_);
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagnodeid, cOptions.node_id);
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagtblspt, cOptions.tablet_sub_path);
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagrlgspt, cOptions.redolog_sub_path);		
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagdtaspt, cOptions.data_sub_path);		
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagthrfad, cOptions.thrift_address);		
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagthrfpt, (AnsiString) cOptions.thrift_port);	
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagthrgad, cOptions.thrift_gossiper_address);	
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagthrgpt, (AnsiString) cOptions.thrift_gossiper_port);	
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagthrfls, (AnsiString) cOptions.thrift_listeners);	
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagrldim_, (AnsiString) cOptions.redolog_dim);	
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagmaxcol, (AnsiString) cOptions.max_compaction_level);
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagrepfac, (AnsiString) cOptions.replication_factor);
		CoreButl::settf (cVpool, sVmasterxml, cVdocument->settc (true, false).c_str ());
		cVorion = PTR_ORION (new CoreOrn_ (cVpool, sPath, cOptions.master_xml,CoreEnpo::iCrunning, false));
		cVplanner = cVorion->getpl ();	
		cVplanner->lnch_ (NULL, &CorePlan::touch);
	}

	DB::~DB () {

		cVplanner.reset ();
		cVorion.reset ();
		cVpool.reset ();

		cVglobalmutex.locks ();
		iVlibraryrefs--;
		if (iVlibraryrefs == 0) ShutdownGoogleLogging();
		cVglobalmutex.relse ();
		ShutdownProtobufLibrary();
	}

  /*Open*/
	Instance DB::Open (Options& options, string path) {

		return Instance (new DB (options, path));
	}

	/*SHUTDOWN*/
	void DB::Shutdown () {

		ShutdownProtobufLibrary();
	}

	/*PUT*/
	Status DB::Put (WriteOptions& writeOptions, string key, string value) {
		bool bVresult;
		PrtoIwop* cVwriteoptions = &writeOptions.write_options;

		switch (cVwriteoptions->ivlevel ()) {
			case L1:
				cVwriteoptions->mutable_cvstatementl1 ()->
					mutable_cvkey ()->set_svmain (key);
				cVwriteoptions->mutable_cvstatementl1 ()->mutable_cvkey ()->set_ivstate (UPSERT);
				cVwriteoptions->mutable_cvstatementl1 ()->mutable_cvvalue ()->set_svopaquevalue (value);
				if (cVwriteoptions->bvenabletimestamp ()) 
					cVwriteoptions->mutable_cvstatementl1 ()->mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstf ());
				bVresult = cVplanner->lcstm (cVwriteoptions->mutable_cvstatementl1 (), true, &writeOptions.cVinternaltabletloc);
				break;
			case L2:
				cVwriteoptions->mutable_cvstatementl2 ()->
					mutable_cvkey ()->set_svmain (key);
				cVwriteoptions->mutable_cvstatementl2 ()->mutable_cvkey ()->set_ivstate (UPSERT);
				if (cVwriteoptions->bvenabletimestamp ()) 
					cVwriteoptions->mutable_cvstatementl2 ()->mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstf ());
				writeOptions.first_column_L2->mutable_cvvalue ()->set_svvalue (value);
				bVresult = cVplanner->statm (cVwriteoptions->mutable_cvstatementl2 (), 
					cVwriteoptions->bvoptimizewriteindex (), cVwriteoptions->bvupdateonlyindex (),
					cVwriteoptions->bvlocalfilter(), writeOptions.cVinternaltableloc);
		}
		return Status (bVresult);
	}

	/*DELETE*/
	Status DB::Delete (WriteOptions& writeOptions, string key) {
		bool bVresult;
		PrtoIwop* cVwriteoptions = &writeOptions.write_options;

		switch (cVwriteoptions->ivlevel ()) {
			case L1:
				cVwriteoptions->mutable_cvstatementl1 ()->mutable_cvkey ()->set_svmain (key);
				cVwriteoptions->mutable_cvstatementl1 ()->mutable_cvkey ()->set_ivstate (DELTMB);
				if (cVwriteoptions->bvenabletimestamp ()) 
					cVwriteoptions->mutable_cvstatementl1 ()->mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstf ());
				bVresult = cVplanner->lcstm (cVwriteoptions->mutable_cvstatementl1 (), true, &writeOptions.cVinternaltabletloc);
				break;
			case L2:
				cVwriteoptions->mutable_cvstatementl2 ()->mutable_cvkey ()->set_svmain (key);
				cVwriteoptions->mutable_cvstatementl2 ()->mutable_cvkey ()->set_ivstate (DELTMB);
				if (cVwriteoptions->bvenabletimestamp ()) 
					cVwriteoptions->mutable_cvstatementl2 ()->mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstf ());
				bVresult = cVplanner->statm (cVwriteoptions->mutable_cvstatementl2 (), 
					cVwriteoptions->bvoptimizewriteindex (), cVwriteoptions->bvupdateonlyindex (),
					cVwriteoptions->bvlocalfilter(), writeOptions.cVinternaltableloc);
		}
		return Status (bVresult);
	}

	/*GET*/
	Status DB::Get(ReadOptions& readOptions, string key, string* value) {
		Status result (false);
		PrtoIsop* cVstatus = &result.status;
		PrtoIrop* cVreadoptions = &readOptions.read_options;

		switch (cVreadoptions->ivlevel ()) {
			case L1:
				cVreadoptions->mutable_cvqueryl1 ()->mutable_cvkey ()->set_svmain (key);
				if (cVplanner->lcqry (
						cVreadoptions->mutable_cvqueryl1 (), 
						cVstatus->mutable_cvsnapshotl1 ()->mutable_cvvalue (), true)) {
					*value = cVstatus->mutable_cvsnapshotl1 ()->mutable_cvvalue ()->svopaquevalue ();
					cVstatus->set_bvresult (true);
				}
				break;
			case L2:
				cVreadoptions->mutable_cvqueryl2 ()->mutable_cvkey_start ()->set_svmain (key);
				if (cVplanner->query (cVreadoptions->mutable_cvqueryl2 (), cVstatus->mutable_cvsnapshotl2 ())) {
					*value = cVstatus->mutable_cvsnapshotl2 ()->mutable_cvkeyslice (0)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ();
					cVstatus->set_bvresult (true);
				}
				break;
		}

		return result;
	}

	/*GET*/
	Status DB::Get(ReadOptions& readOptions, string* value) {
		Status result (false);
		PrtoIsop* cVstatus = &result.status;
		PrtoIrop* cVreadoptions = &readOptions.read_options;

		switch (cVreadoptions->ivlevel ()) {
			case L1:
				if (cVplanner->lcqry (
						cVreadoptions->mutable_cvqueryl1 (), 
						cVstatus->mutable_cvsnapshotl1 ()->mutable_cvvalue (), true)) {
					*value = cVstatus->mutable_cvsnapshotl1 ()->mutable_cvvalue ()->svopaquevalue ();
					cVstatus->set_bvresult (true);
				}
				break;
			case L2:
				if (cVplanner->query (cVreadoptions->mutable_cvqueryl2 (), cVstatus->mutable_cvsnapshotl2 ())) {
					*value = cVstatus->mutable_cvsnapshotl2 ()->mutable_cvkeyslice (0)->
						mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ();
					cVstatus->set_bvresult (true);
				}
				break;
		}

		return result;
	}

	/*CREATE*/
	Status DB::Create (StorageOptions& storageOptions) {
		Status result (false);
		PrtoIsop* cVstatus = &result.status;

		switch (storageOptions.storage_options.ivlevel ()) {
			case L1:
				cVstatus->set_bvresult (cVplanner->crttb (storageOptions.storage_options.mutable_cvdmll1 ()));
				break;
			case L2:
				cVstatus->set_bvresult (cVplanner->crttb (storageOptions.storage_options.mutable_cvdmll2 ()));
				break;
		}
		return result;
	}

	/*CREATE TABLE CONFIG*/
	Status DB::CreateTableConfig (StorageOptions& storageOptions, string table, ...) {
		Status result (false);
		PrtoIsop* cVstatus = &result.status;

		switch (storageOptions.storage_options.ivlevel ()) {
				case L1:
					storageOptions.storage_options.mutable_cvdmll1 ()->set_svtable (table);	
					cVstatus->set_bvresult (true);
					break;
				case L2:
					storageOptions.storage_options.mutable_cvdmll2 ()->mutable_cvmutable ()->set_svtable (table);
					storageOptions.storage_options.mutable_cvdmll2 ()->clear_cvcolumns ();
					va_list cVlist;
					va_start (cVlist, table);
					while (true) {
						int iVcoltype = va_arg (cVlist, int);
						if (iVcoltype == VaEOF) break;
						PrtoL2cl* cVcolumn = 
							storageOptions.storage_options.mutable_cvdmll2 ()->mutable_cvcolumns ()->Add ();
						string sVcolumnname = va_arg (cVlist, char*);
						cVcolumn->set_svcolumn (sVcolumnname);	
						cVcolumn->set_ivtype ((iCcolumntype) iVcoltype);
						cVstatus->set_bvresult (true);
					}
					va_end (cVlist);
					break;
		}
		return result;
	}

	/*CREATE INDEX CONFIG*/
	Status DB::CreateIndexConfig (StorageOptions& storageOptions, string index, ...) {
		Status result (false);
		PrtoIsop* cVstatus = &result.status;

		switch (storageOptions.storage_options.ivlevel ()) {
				case L2:
					PrtoL2ix* cVindex = 
						storageOptions.storage_options.mutable_cvdmll2 ()->add_cvindexes ();
					cVindex->set_svindex (index);
					va_list cVlist;
					va_start (cVlist, index);
					while (true) {
						int iVcolorder = va_arg (cVlist, int);
						if (iVcolorder == VaEOF) break;
						PrtoL2or* cVcolumn = cVindex->add_cvcolumn ();
						string sVcolumnname = va_arg (cVlist, char*);
						cVcolumn->set_svcolumn (sVcolumnname);	
						cVcolumn->set_ivordertype ((iCordertype) iVcolorder);
						cVstatus->set_bvresult (true);
					}
					va_end (cVlist);
					break;
		}
		return result;
	}

	/*OSQL*/
	Status DB::Osql (OsqlOptions& osqlOptions) {
		Status result (false);
		PrtoIsop* cVstatus = &result.status;

		if (osqlOptions.osql_options.bvenabletimestamp ())
			osqlOptions.osql_options.mutable_cvosqll2 ()->set_ivtimestamp (CoreButl::tmstf ());
		cVstatus->set_bvresult (cVplanner->osql_ (
			osqlOptions.osql_options.mutable_cvosqll2 (), cVstatus->mutable_cvsnapshotl2 ()));
		return result;
	}

	/*OSQL*/
	Status DB::Osql (OsqlOptions& osqlOptions, string osql) {
		
		osqlOptions.osql_options.mutable_cvosqll2 ()->set_svosqlstring (osql);
		return Osql (osqlOptions);
	}

	/*OSQL BIND*/
	Status DB::OsqlBind (OsqlOptions& osqlOption, bool full_scan, ...) {
		Status result (false);
		PrtoIsop* cVstatus = &result.status;
		string sVstringvalue;
		int iVintvalue;
		double dVdoublevalue;
		bool bVboolvalue;

		osqlOption.osql_options.mutable_cvosqll2 ()->set_bvfulltablescan (full_scan);
		osqlOption.osql_options.mutable_cvosqll2 ()->clear_cvinputbindings ();
		va_list cVlist;
		va_start (cVlist, full_scan);
		while (true) {
			int iVcoltype = va_arg (cVlist, int);
			if (iVcoltype == VaEOF) break;
			cVstatus->set_bvresult (true);
			PrtoL2cv* cVcolumnvalue = osqlOption.osql_options.mutable_cvosqll2 ()->add_cvinputbindings ();
			cVcolumnvalue->set_ivtype ((iCcolumntype) iVcoltype);
			switch (iVcoltype) {
				case STRINGTYPE:
					sVstringvalue = va_arg (cVlist, char*);
					cVcolumnvalue->set_svvalue (sVstringvalue);
					break;
				case INTEGRTYPE:
					iVintvalue = va_arg (cVlist, int);
					cVcolumnvalue->set_ivvalue (iVintvalue);
					break;
				case DOUBLETYPE:
					dVdoublevalue = va_arg (cVlist, double);
					cVcolumnvalue->set_dvvalue (dVdoublevalue);
					break;
				case BOOLN_TYPE:
					bVboolvalue = va_arg (cVlist, bool);
					cVcolumnvalue->set_bvvalue (bVboolvalue);
					break;
				default:
					cVstatus->set_bvresult (false);
			}
		}
		va_end (cVlist);
		return result;
	}

	/*DESTROY DB*/
	Status DB::DestroyDB(const Options& options, string sPath) {

		return Status (CoreHutl::rmdir (sPath));
	}

	/*SET VOLATILE*/
	void DB::setVolatile (StorageOptions& storageOptions) {

		switch (storageOptions.storage_options.ivlevel ()) {
			case L1: 
				storageOptions.storage_options.mutable_cvdmll1 ()->
					mutable_cvstorage ()->set_bvmemorytable (true);
				storageOptions.storage_options.mutable_cvdmll1 ()->
					mutable_cvstorage ()->set_bvvolatiletable (true);
				storageOptions.storage_options.mutable_cvdmll1 ()->
					mutable_cvstorage ()->set_bvfastkey (true);
				break;
			case L2: storageOptions.storage_options.mutable_cvdmll2 ()->
					set_ivtabletype (VOLATILETABLE); break;
				break;
		}
	}


	/*SEEK TO FIRST*/
	void SnapshotIterator::SeekToFirst () {
		
		seek = 0;
	}

	/*NEXT*/
	void SnapshotIterator::Next () {
		
		seek++;
	}

	/*VALID*/
	bool SnapshotIterator::Valid () {
		
		return snapshot_L2->cvkeyslice_size () > seek;
	}

	/*KEY*/
	string* SnapshotIterator::key () {

		return snapshot_L2->mutable_cvkeyslice (seek)->mutable_cvkey ()->mutable_svmain ();
	}

	/*VALUE STRING*/
	string* SnapshotIterator::valueString (int column) {

		return snapshot_L2->mutable_cvkeyslice (seek)->mutable_cvcolumns (column)->mutable_cvvalue ()->mutable_svvalue ();
	}

	/*VALUE INT*/
	int SnapshotIterator::valueInt (int column) {

		return snapshot_L2->mutable_cvkeyslice (seek)->mutable_cvcolumns (column)->mutable_cvvalue ()->ivvalue ();
	}

	/*VALUE DOUBLE*/
	double SnapshotIterator::valueDouble (int column) {

		return snapshot_L2->mutable_cvkeyslice (seek)->mutable_cvcolumns (column)->mutable_cvvalue ()->dvvalue ();
	}

	/*VALUE BOOLean*/
	bool SnapshotIterator::valueBool (int column) {

		return snapshot_L2->mutable_cvkeyslice (seek)->mutable_cvcolumns (column)->mutable_cvvalue ()->bvvalue ();
	}

	/*NEW Iterator*/
	Iterator* DB::NewIterator (ReadOptions& readOptions) {

		return new Iterator (readOptions, this);
	}

	/*SEEK*/
	void Iterator::Seek (string start) {

		switch (readOptions.read_options.ivlevel ()) {
			case L1:
				readOptions.read_options.mutable_cvqueryl1 ()->set_ivquery (NRNGEQUERY);
				break;
			case L2:
				readOptions.read_options.mutable_cvqueryl2 ()->set_ivquery (NRNGEQUERY);	
				readOptions.read_options.mutable_cvqueryl2 ()->set_ivcount (1);
				break;
		}
		valid = ((DB*) db)->Get(readOptions, start, &current).ok ();
	}

	/*NEXT*/
	void Iterator::Next () {

		valid = ((DB*) db)->Get(readOptions, &current).ok ();
	}

	/*KEY*/
	string* Iterator::key () {

		switch (readOptions.read_options.ivlevel ()) {
			case L1:
				return readOptions.read_options.mutable_cvqueryl1 ()->mutable_cvkey ()->mutable_svmain ();
			case L2:
				return readOptions.read_options.mutable_cvqueryl2 ()->mutable_cvkey_start ()->mutable_svmain ();
		}		
		return NULL;
	}


	AriesDB::AriesDB (Options& cOptions, string sPath) : DB (cOptions, sPath) {
	};
	
	AriesDB::~AriesDB () {
	}

  /*Open*/
	AriesInstance AriesDB::Open (Options& options, string path) {

		return AriesInstance (new AriesDB (options, path));
	}

	/*CREATE*/
	Status AriesDB::Create (StorageOptions& storageOptions) {
		Status result (false);
		
		/*
		PrtoIsop* cVstatus = &result.status;

		switch (storageOptions.storage_options.ivlevel ()) {
			case L1:
				cVstatus->set_bvresult (cVplanner->crttb (storageOptions.storage_options.mutable_cvdmll1 ()));
				break;
			case L2:
				cVstatus->set_bvresult (cVplanner->crttb (storageOptions.storage_options.mutable_cvdmll2 ()));
				break;
		}*/
		return result;
	}
