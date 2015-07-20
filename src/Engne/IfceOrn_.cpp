#include "IfceOrn_.hpp"
#include "CoreXml_.hpp"
#include "CoreHutl.hpp"
#include "DataXml_.hpp"


using namespace orion; 
using namespace google;
using namespace google::protobuf;




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
		cVdocument->simpp (DataXml_::sCtagorion_, DataXml_::sCtagthrfpt, (AnsiString) cOptions.thrift_listeners);	
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