#ifndef IFCEORN_
#define IFCEORN_
#include "CoreOrn_.hpp"
#include "CorePlan.hpp"
#include "CoreAutl.hpp"

using namespace std;

namespace orion {

	struct Options {
		string log_path;
		string master_xml;
		string node_id;
		string tablet_sub_path;
		string redolog_sub_path;
		string data_sub_path;
		string thrift_address;
		string thrift_gossiper_address;
		int thrift_port;
		int thrift_gossiper_port;
		int redolog_dim;
		short int thrift_listeners;
		short int max_compaction_level;
		short int replication_factor;
		bool error_if_exists;

		Options () {
			master_xml = "Orion.xml";
			error_if_exists = false;
			node_id = CoreOrn_::sCnodeid.to_string ();
			thrift_address = CoreOrn_::sClocaladdress.to_string ();
			thrift_port = CoreOrn_::iCthriftport;
			thrift_gossiper_port = CoreOrn_::iCthriftport;
			thrift_listeners = CoreOrn_::iCthriftlisteners;
			redolog_dim = CoreOrn_::iCredologdim;
			max_compaction_level = CoreOrn_::iCmaxcompactionlevel;
			replication_factor = CoreOrn_::iCreplicationfactor;
		}
	};

	struct WriteOptions {
		bool optimize_write_index;
		bool update_only_index;
		bool local_filter;
		bool enable_timestamp;
		iCdbleveltype level;
		PrtoLstm statement_L1; 
		PrtoL2st statement_L2;
		PrtoL2cl* first_column_L2;
		CorePlan::PTR_TABLELOC* cVinternaltableloc;
		CoreAlog::TABLETLOC* cVinternaltabletloc;

		WriteOptions (iCdbleveltype iLevel = L2) {
			cVinternaltableloc = NULL;
			cVinternaltabletloc = NULL;
			level = iLevel;
			enable_timestamp = true;
			
			switch (level) {
				case L1:
					statement_L1.mutable_cvmutable ()->set_svtable (CoreOrn_::sCdefault.to_string ());
					statement_L1.mutable_cvmutable ()->set_svnamespace (CoreOrn_::sCdefault.to_string ());
					statement_L1.mutable_cvkey ()->set_ivtimestamp (0);
					statement_L1.mutable_cvvalue ()->set_ivtimestamp (0);
					CoreAutl::zrkey (statement_L1.mutable_cvkey ());
					break;
				case L2:
					statement_L2.mutable_cvmutable ()->set_svtable (CoreOrn_::sCdefault.to_string ());
					statement_L2.mutable_cvmutable ()->set_svnamespace (CoreOrn_::sCdefault.to_string ());
					statement_L2.mutable_cvkey ()->set_ivtimestamp (0);
					first_column_L2 = statement_L2.mutable_cvcolumns ()->Add ();
					first_column_L2->set_svcolumn (CoreOrn_::sCdefault.to_string ());	
					CoreAutl::zrkey (statement_L2.mutable_cvkey ());
					optimize_write_index = true;
					update_only_index = false;
					local_filter = false;
					break;
			}
		}
	};

	struct ReadOptions {
		iCdbleveltype level;
		PrtoLqry query_L1;
		PrtoL2qr query_L2;
		PrtoL2cl* first_column_L2;

		ReadOptions (iCdbleveltype iLevel = L2) {
			level = iLevel;
			switch (level) {
				case L1:
					query_L1.mutable_cvmutable ()->set_svtable (CoreOrn_::sCdefault.to_string ());
					query_L1.mutable_cvmutable ()->set_svnamespace (CoreOrn_::sCdefault.to_string ());	
					query_L1.set_ivquery (EXACTQUERY);
					break;
				case L2:
					query_L2.mutable_cvmutable ()->set_svtable (CoreOrn_::sCdefault.to_string ());
					query_L2.mutable_cvmutable ()->set_svnamespace (CoreOrn_::sCdefault.to_string ());
					first_column_L2 = query_L2.mutable_cvselect ()->Add ();
					first_column_L2->set_svcolumn (CoreOrn_::sCdefault.to_string ());
					query_L2.set_ivquery (EXACTQUERY);	
					break;
			}
		}
	};

	struct StorageOptions {
		PrtoLmtb dml_L1;
		PrtoL2ct dml_L2;
		iCdbleveltype level;

		StorageOptions (iCdbleveltype iLevel = L2) {
			level = iLevel;
			switch (level) {
				case L1:
					dml_L1.set_svtable (CoreOrn_::sCdefault.to_string ());
					dml_L1.set_svnamespace (CoreOrn_::sCdefault.to_string ());
					dml_L1.mutable_cvstorage ()->set_bvmemorytable (true);
					dml_L1.mutable_cvstorage ()->set_bvvolatiletable (false);					
					break;
				case L2:
					dml_L2.mutable_cvmutable ()->set_svtable (CoreOrn_::sCdefault.to_string ());
					dml_L2.mutable_cvmutable ()->set_svnamespace (CoreOrn_::sCdefault.to_string ());
					PrtoL2cl* cVcolumn = dml_L2.mutable_cvcolumns ()->Add ();
					cVcolumn->set_svcolumn (CoreOrn_::sCdefault.to_string ());	
					cVcolumn->set_ivtype (STRINGTYPE);
					dml_L2.set_ivtabletype (MEMORYTABLE__);
					dml_L2.set_ivindextype (DISTRINDEX);
					dml_L2.set_ivpartitiontype (NOPARTITION);
					break;
			}
		}
	};

	struct Status {
		bool result;
		PrtoL2mr snapshot_L2;
		PrtoLval snapshot_L1;
		
		Status (bool bResult = false) {

			result = bResult;
		}

		bool ok () {
			return result;
		}

	};



	class DB {

		PTR_POOL cVpool;
		PTR_PLANNER cVplanner;
		PTR_ORION cVorion;

		public:

			DB (Options& cOptions, string sPath);
			~DB ();

			Status Put (WriteOptions& writeOptions, string key, string value); 
			Status Delete(WriteOptions& writeOptions, string key);
			Status Get(ReadOptions& readOptions, string key, string* value);
			Status Create (StorageOptions& storageOptions);

			static boost::shared_ptr<DB> Open (Options& options, string path);
			static Status DestroyDB(const Options& options, string sPath);
			static void Shutdown ();

			static void setVolatile (StorageOptions& storageOptions);
	};

	
	typedef boost::shared_ptr<DB> Instance;
	
}

#endif