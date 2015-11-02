#ifndef IFCEORN_
#define IFCEORN_
#include "CoreOrn_.hpp"
#include "CorePlan.hpp"
#include "CoreAutl.hpp"
#include <stdarg.h>

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

		Options (PrtoIoop& cProto) {
			log_path = cProto.svlogpath ();
			master_xml = cProto.has_svmasterxml () ? cProto.svmasterxml () : "Orion.xml";
			node_id = cProto.has_svnodeid () ? cProto.svnodeid () : CoreOrn_::sCnodeid.to_string ();
			tablet_sub_path = cProto.svtabletsubpath ();
			redolog_sub_path = cProto.svredologsubpath ();
			data_sub_path = cProto.svdatasubpath ();
			thrift_address = cProto.has_svthriftaddress () ? cProto.svthriftaddress () : CoreOrn_::sClocaladdress.to_string ();
			thrift_gossiper_address = cProto.svthriftgossiperaddress ();
			thrift_port = cProto.has_ivthriftport () ? cProto.ivthriftport () : CoreOrn_::iCthriftport;
			thrift_gossiper_port = cProto.has_ivthriftgossiperport () ? cProto.ivthriftgossiperport () : CoreOrn_::iCthriftport;
			redolog_dim = cProto.has_ivredologdim () ? cProto.ivredologdim () : CoreOrn_::iCredologdim;
			thrift_listeners = cProto.has_ivthriftlisteners () ? cProto.ivthriftlisteners () : CoreOrn_::iCthriftlisteners;
			max_compaction_level = cProto.has_ivmaxcompactionlevel () ? cProto.ivmaxcompactionlevel () : 
				CoreOrn_::iCmaxcompactionlevel;
			replication_factor = cProto.has_ivreplicationfactor () ? cProto.ivreplicationfactor () : CoreOrn_::iCreplicationfactor;
			error_if_exists = cProto.has_bverrorifexists () ? cProto.bverrorifexists () : false;
		}
	};

	struct WriteOptions {
		PrtoIwop write_options;
		PrtoL2cl* first_column_L2;
		CorePlan::PTR_TABLELOC* cVinternaltableloc;
		CoreAlog::TABLETLOC* cVinternaltabletloc;

		WriteOptions (iCdbleveltype iLevel = L2) {
			cVinternaltableloc = NULL;
			cVinternaltabletloc = NULL;
			write_options.set_ivlevel (iLevel);  
			write_options.set_bvenabletimestamp (true);
			
			switch (write_options.ivlevel ()) {
				case L1:
					write_options.mutable_cvstatementl1 ()->mutable_cvmutable ()->
						set_svtable (CoreOrn_::sCdefault.to_string ());
					write_options.mutable_cvstatementl1 ()->mutable_cvmutable ()->
						set_svnamespace (CoreOrn_::sCdefault.to_string ());
				
					write_options.mutable_cvstatementl1 ()->mutable_cvkey ()->set_ivtimestamp (0);
					write_options.mutable_cvstatementl1 ()->mutable_cvvalue ()->set_ivtimestamp (0);
					CoreAutl::zrkey (write_options.mutable_cvstatementl1 ()->mutable_cvkey ());
					break;
				case L2:
					write_options.mutable_cvstatementl2 ()->mutable_cvmutable ()->
						set_svtable (CoreOrn_::sCdefault.to_string ());
					write_options.mutable_cvstatementl2 ()->mutable_cvmutable ()->
						set_svnamespace (CoreOrn_::sCdefault.to_string ());
					write_options.mutable_cvstatementl2 ()->mutable_cvkey ()->set_ivtimestamp (0);
					first_column_L2 = write_options.mutable_cvstatementl2 ()->mutable_cvcolumns ()->Add ();
					first_column_L2->set_svcolumn (CoreOrn_::sCdefault.to_string ());	
					CoreAutl::zrkey (write_options.mutable_cvstatementl2 ()->mutable_cvkey ());
					write_options.set_bvoptimizewriteindex (true);
					write_options.set_bvupdateonlyindex ( false);
					write_options.set_bvlocalfilter ( false);
					break;
			}
		}
	};

	struct ReadOptions {
		PrtoIrop read_options;
		PrtoL2cl* first_column_L2;

		ReadOptions (iCdbleveltype iLevel = L2) {
			read_options.set_ivlevel (iLevel);
			switch (read_options.ivlevel ()) {
				case L1:
					read_options.mutable_cvqueryl1 ()->
						mutable_cvmutable ()->set_svtable (CoreOrn_::sCdefault.to_string ());
					read_options.mutable_cvqueryl1 ()->
						mutable_cvmutable ()->set_svnamespace (CoreOrn_::sCdefault.to_string ());	
					read_options.mutable_cvqueryl1 ()->set_ivquery (EXACTQUERY);
					break;
				case L2:
					read_options.mutable_cvqueryl2 ()->
						mutable_cvmutable ()->set_svtable (CoreOrn_::sCdefault.to_string ());
					read_options.mutable_cvqueryl2 ()->
						mutable_cvmutable ()->set_svnamespace (CoreOrn_::sCdefault.to_string ());
					first_column_L2 = read_options.mutable_cvqueryl2 ()->mutable_cvselect ()->Add ();
					first_column_L2->set_svcolumn (CoreOrn_::sCdefault.to_string ());
					read_options.mutable_cvqueryl2 ()->set_ivquery (EXACTQUERY);	
					break;
			}
		}
	};

	struct StorageOptions {
		PrtoIgop storage_options;

		StorageOptions (iCdbleveltype iLevel = L2) {
			storage_options.set_ivlevel (iLevel);
			switch (storage_options.ivlevel ()) {
				case L1:
					storage_options.mutable_cvdmll1 ()->set_svtable (CoreOrn_::sCdefault.to_string ());
					storage_options.mutable_cvdmll1 ()->set_svnamespace (CoreOrn_::sCdefault.to_string ());
					storage_options.mutable_cvdmll1 ()->mutable_cvstorage ()->set_bvmemorytable (true);
					storage_options.mutable_cvdmll1 ()->mutable_cvstorage ()->set_bvvolatiletable (false);					
					break;
				case L2:
					storage_options.mutable_cvdmll2 ()->mutable_cvmutable ()->set_svtable (CoreOrn_::sCdefault.to_string ());
					storage_options.mutable_cvdmll2 ()->mutable_cvmutable ()->set_svnamespace (CoreOrn_::sCdefault.to_string ());
					PrtoL2cl* cVcolumn = storage_options.mutable_cvdmll2 ()->mutable_cvcolumns ()->Add ();
					cVcolumn->set_svcolumn (CoreOrn_::sCdefault.to_string ());	
					cVcolumn->set_ivtype (STRINGTYPE);
					storage_options.mutable_cvdmll2 ()->set_ivtabletype (MEMORYTABLE__);
					storage_options.mutable_cvdmll2 ()->set_ivindextype (DISTRINDEX);
					storage_options.mutable_cvdmll2 ()->set_ivpartitiontype (NOPARTITION);
					break;
			}
		}
	};

	struct OsqlOptions {
		PrtoIqop osql_options;

		OsqlOptions () {
			osql_options.mutable_cvosqll2 ()->set_svnamespace (CoreOrn_::sCdefault.to_string ());
			osql_options.mutable_cvosqll2 ()->set_bvonlysecondary (false);
			osql_options.set_bvenabletimestamp (false);
		}
	};

	class SnapshotIterator {
		private:
			int seek;
			PrtoL2mr* snapshot_L2;

		public:
			SnapshotIterator (PrtoL2mr* iSnapshot_L2) : snapshot_L2 (iSnapshot_L2) {};

			void SeekToFirst ();
			bool Valid ();
			void Next ();
			string* key ();
			string* valueString (int column = 0);
			int valueInt (int column = 0);
			double valueDouble (int column = 0);
			bool valueBool (int column = 0);
	};

	struct Status {
		PrtoIsop status;
		
		Status (bool bResult = false) {

			status.set_bvresult (bResult);
		}

		bool ok () {
			return status.bvresult ();
		}

		SnapshotIterator* NewSnapshotIterator () {
			return new SnapshotIterator (status.mutable_cvsnapshotl2 ());
		}

	};

	class Iterator {
		private:
			bool valid;
			string current;
			ReadOptions readOptions;
			void* db;

		public:
			Iterator (ReadOptions& readOptions, void* db) : readOptions (readOptions), db (db), valid (false) {};

			inline void SeekToFirst () {Seek ("");}
			void Seek (string start);
			inline bool Valid () {return valid;}
			inline string* value () {return &current;}
			void Next ();
			string* key ();
	};


	class DB {

		PTR_POOL cVpool;
		PTR_ORION cVorion;

		public:
			PTR_PLANNER cVplanner;
			static const int VaEOF;

			DB (Options& cOptions, string sPath);
			~DB ();

			Status Put (WriteOptions& writeOptions, string key, string value); 
			Status Delete(WriteOptions& writeOptions, string key);
			Status Get(ReadOptions& readOptions, string key, string* value);
			Status Get(ReadOptions& readOptions, string* value);
			Status Create (StorageOptions& storageOptions);
			Status CreateTableConfig (StorageOptions& storageOptions, string table, ...);
			Status CreateIndexConfig (StorageOptions& storageOptions, string index, ...);
			Status Osql (OsqlOptions& osqlOptions, string osql);
			Status Osql (OsqlOptions& osqlOptions);
			Status OsqlBind (OsqlOptions& osqlOption, bool full_scan, ...);
			Iterator* NewIterator (ReadOptions& readOptions);

			static boost::shared_ptr<DB> Open (Options& options, string path);
			static Status DestroyDB(const Options& options, string sPath);
			static void Shutdown ();

			static void setVolatile (StorageOptions& storageOptions);
	};


	class AriesDB : public DB {

	public:
			AriesDB (Options& cOptions, string sPath);
			~AriesDB ();

			Status Create (StorageOptions& storageOptions);

			static boost::shared_ptr<AriesDB> Open (Options& options, string path);
	};

	
	typedef boost::shared_ptr<DB> Instance;
	typedef boost::shared_ptr<AriesDB> AriesInstance;
	
}

#endif