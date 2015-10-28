#include "IfceOrn_.hpp"
#include "CoreXml_.hpp"
#include "CoreHutl.hpp"
#include "DataXml_.hpp"

using namespace orion; 
using namespace google;
using namespace google::protobuf;


	const int DB::VaEOF = -1;

	DB::DB (Options& cOptions, string sPath) {

		cVpool = PTR_POOL (new CorePool ());
		FLAGS_log_dir = cOptions.log_path;
		InitGoogleLogging("");
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
		ShutdownGoogleLogging();
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

		switch (writeOptions.level) {
			case L1:
				writeOptions.statement_L1.mutable_cvkey ()->set_svmain (key);
				writeOptions.statement_L1.mutable_cvkey ()->set_ivstate (UPSERT);
				writeOptions.statement_L1.mutable_cvvalue ()->set_svopaquevalue (value);
				if (writeOptions.enable_timestamp) writeOptions.statement_L1.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstf ());
				bVresult = cVplanner->lcstm (&writeOptions.statement_L1, true, &writeOptions.cVinternaltabletloc);
				break;
			case L2:
				writeOptions.statement_L2.mutable_cvkey ()->set_svmain (key);
				writeOptions.statement_L2.mutable_cvkey ()->set_ivstate (UPSERT);
				if (writeOptions.enable_timestamp) writeOptions.statement_L2.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstf ());
				writeOptions.first_column_L2->mutable_cvvalue ()->set_svvalue (value);
				bVresult = cVplanner->statm (&writeOptions.statement_L2, 
					writeOptions.optimize_write_index, writeOptions.update_only_index, writeOptions.local_filter,
					writeOptions.cVinternaltableloc);
		}
		return Status (bVresult);
	}

	/*DELETE*/
	Status DB::Delete (WriteOptions& writeOptions, string key) {
		bool bVresult;

		switch (writeOptions.level) {
			case L1:
				writeOptions.statement_L1.mutable_cvkey ()->set_svmain (key);
				writeOptions.statement_L1.mutable_cvkey ()->set_ivstate (DELTMB);
				if (writeOptions.enable_timestamp) writeOptions.statement_L1.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstf ());
				bVresult = cVplanner->lcstm (&writeOptions.statement_L1, true, &writeOptions.cVinternaltabletloc);
				break;
			case L2:
				writeOptions.statement_L2.mutable_cvkey ()->set_svmain (key);
				writeOptions.statement_L2.mutable_cvkey ()->set_ivstate (DELTMB);
				if (writeOptions.enable_timestamp) writeOptions.statement_L2.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstf ());
				bVresult = cVplanner->statm (&writeOptions.statement_L2, 
					writeOptions.optimize_write_index, writeOptions.update_only_index, writeOptions.local_filter,
					writeOptions.cVinternaltableloc);
		}
		return Status (bVresult);
	}

	/*GET*/
	Status DB::Get(ReadOptions& readOptions, string key, string* value) {
		Status result (false);

		switch (readOptions.level) {
			case L1:
				readOptions.query_L1.mutable_cvkey ()->set_svmain (key);
				if (cVplanner->lcqry (&readOptions.query_L1, &result.snapshot_L1, true)) {
					*value = result.snapshot_L1.svopaquevalue ();
					result.result = true;
				}
				break;
			case L2:
				readOptions.query_L2.mutable_cvkey_start ()->set_svmain (key);
				if (cVplanner->query (&readOptions.query_L2, &result.snapshot_L2)) {
					*value = result.snapshot_L2.mutable_cvkeyslice (0)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ();
					result.result = true;
				}
				break;
		}

		return result;
	}

	/*GET*/
	Status DB::Get(ReadOptions& readOptions, string* value) {
		Status result (false);

		switch (readOptions.level) {
			case L1:
				if (cVplanner->lcqry (&readOptions.query_L1, &result.snapshot_L1, true)) {
					*value = result.snapshot_L1.svopaquevalue ();
					result.result = true;
				}
				break;
			case L2:
				if (cVplanner->query (&readOptions.query_L2, &result.snapshot_L2)) {
					*value = result.snapshot_L2.mutable_cvkeyslice (0)->mutable_cvcolumns (0)->mutable_cvvalue ()->svvalue ();
					result.result = true;
				}
				break;
		}

		return result;
	}

	/*CREATE*/
	Status DB::Create (StorageOptions& storageOptions) {
		Status result (false);

		switch (storageOptions.level) {
			case L1:
				result.result = cVplanner->crttb (&storageOptions.dml_L1);
				break;
			case L2:
				result.result = cVplanner->crttb (&storageOptions.dml_L2);
				break;
		}
		return result;
	}

	/*CREATE TABLE CONFIG*/
	Status DB::CreateTableConfig (StorageOptions& storageOptions, string table, ...) {
		Status result (false);

		switch (storageOptions.level) {
				case L1:
					storageOptions.dml_L1.set_svtable (table);	
					result.result = true;
					break;
				case L2:
					storageOptions.dml_L2.mutable_cvmutable ()->set_svtable (table);
					storageOptions.dml_L2.clear_cvcolumns ();
					va_list cVlist;
					va_start (cVlist, table);
					while (true) {
						int iVcoltype = va_arg (cVlist, int);
						if (iVcoltype == VaEOF) break;
						PrtoL2cl* cVcolumn = storageOptions.dml_L2.mutable_cvcolumns ()->Add ();
						string sVcolumnname = va_arg (cVlist, char*);
						cVcolumn->set_svcolumn (sVcolumnname);	
						cVcolumn->set_ivtype ((iCcolumntype) iVcoltype);
						result.result = true;
					}
					va_end (cVlist);
					break;
		}
		return result;
	}

	/*OSQL*/
	Status DB::Osql (OsqlOptions& osqlOptions) {
		Status result (false);

		result.result = cVplanner->osql_ (&osqlOptions.osql_L2, &result.snapshot_L2);
		return result;
	}

	/*OSQL*/
	Status DB::Osql (OsqlOptions& osqlOptions, string osql) {
		
		osqlOptions.osql_L2.set_svosqlstring (osql);
		return Osql (osqlOptions);
	}

	/*OSQL BIND*/
	Status DB::OsqlBind (OsqlOptions& osqlOption, bool full_scan, ...) {
		Status result (false);
		string sVstringvalue;
		int iVintvalue;
		double dVdoublevalue;
		bool bVboolvalue;

		osqlOption.osql_L2.set_bvfulltablescan (full_scan);
		osqlOption.osql_L2.clear_cvinputbindings ();
		va_list cVlist;
		va_start (cVlist, full_scan);
		while (true) {
			int iVcoltype = va_arg (cVlist, int);
			if (iVcoltype == VaEOF) break;
			result.result = true;
			PrtoL2cv* cVcolumnvalue = osqlOption.osql_L2.add_cvinputbindings ();
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
					result.result = false;
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

		switch (storageOptions.level) {
			case L1: 
				storageOptions.dml_L1.mutable_cvstorage ()->set_bvmemorytable (true);
				storageOptions.dml_L1.mutable_cvstorage ()->set_bvvolatiletable (true);
				storageOptions.dml_L1.mutable_cvstorage ()->set_bvfastkey (true);
				break;
			case L2: storageOptions.dml_L2.set_ivtabletype (VOLATILETABLE); break;
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

		switch (readOptions.level) {
			case L1:
				readOptions.query_L1.set_ivquery (RANGEQUERY);
				break;
			case L2:
				readOptions.query_L2.set_ivquery (RANGEQUERY);	
				readOptions.query_L2.set_ivcount (1);
				break;
		}
		valid = ((DB*) db)->Get(readOptions, start, &current).ok ();
	}

	const string Iterator::next = CoreTbsv::sCchar255.to_string ();

	/*NEXT*/
	void Iterator::Next () {

		switch (readOptions.level) {
			case L1:
				readOptions.query_L1.mutable_cvkey ()->set_svmain 
						(readOptions.query_L1.mutable_cvkey ()->svmain () + next);
				break;
			case L2:
				readOptions.query_L2.mutable_cvkey_start ()->set_svmain 
						(readOptions.query_L2.mutable_cvkey_start ()->svmain () + next);
				break;
		}
		valid = ((DB*) db)->Get(readOptions, &current).ok ();
	}

	/*KEY*/
	string* Iterator::key () {

		switch (readOptions.level) {
			case L1:
				return readOptions.query_L1.mutable_cvkey ()->mutable_svmain ();
			case L2:
				return readOptions.query_L2.mutable_cvkey_start ()->mutable_svmain ();
		}		
	}
