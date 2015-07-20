namespace java com.tomting.orion

struct ThrfLmtb {
	1: required string sVnamespace;			  		  		
	2: string sVtable;
}  

enum iEstatetype {
	UPSERT = 0,
	DELTMB = 1
}

struct ThrfLkey {
	1: required string sVmain;			  		  		
	2: string sVaccessgroup;
	3: string sVqualifier;
	4: iEstatetype iVstate;
	5: i64 iVtimestamp;
}  	

enum iEcolumntype {
	KEYTYPE___ = 0,
	STRINGTYPE = 1,
	LSTRNGTYPE = 2,
	INTEGRTYPE = 3,
	LINTGRTYPE = 4,
	DOUBLETYPE = 5,
	LDOUBLTYPE = 6,
	BOOLN_TYPE = 7
}

struct ThrfL2cv {
	1: required iEcolumntype iVtype;
	2: string sVvalue;
	3: list<string> sVlistvalue;	
	4: i32 iVvalue;	
	5: list<i32> iVlistvalue;
	6: double dVvalue;
	7: list<double> dVlistvalue;
	8: bool bVvalue;	
}

enum iEconditiontype {
	ECOND = 0,
	GCOND = 1,
	LCOND = 2,
	GECOND = 3,
	LECOND = 4,
}

struct ThrfL2cl {
	//COLUMN DEFINITION
	1: required string sVcolumn;
	2: iEcolumntype iVtype;

	//VALUE
	3: ThrfL2cv cVvalue;

	//COMPARE CONDITION
	4: iEconditiontype iVconditiontype;
}

enum iEopcodetype {
	MUTATOR = 0,
	REPAIR_ = 2,
}

struct ThrfLval {
	1: required string sVopaquevalue;
	2: required i64 iVtimestamp;
}

struct ThrfL1ks {
	1: required ThrfLkey cVkey;
	2: required bool bVresult;
	3: required ThrfLval cVvalue;	
}

struct ThrfLstm {
	1: required ThrfLmtb cVmutable;
	2: required ThrfLkey cVkey;
	3: required ThrfLval cVvalue;	
	4: iEopcodetype iVopcode = iEopcodetype.MUTATOR;
	5: string sVcallback;
}

struct ThrfL2st {
	1: required ThrfLmtb cVmutable;
	2: required ThrfLkey cVkey;
	3: list<ThrfL2cl> cVcolumns;	
	4: iEopcodetype iVopcode = iEopcodetype.MUTATOR;
}

struct ThrfL2ag {
	1: required string sVname;
	2: list<string> sVcolumns;
}

struct ThrfL2ct {
	1: required ThrfLmtb cVmutable;
	2: list<ThrfL2cl> cVcolumns;
	3: list<ThrfL2ag> cVaccessgroups;
}

enum iEquerytype {
	EXACTQUERY = 0,
	RANGEQUERY = 1,
	TOKENQUERY = 2,
	TKALLQUERY = 3,
	TKSLWQUERY = 4
}

struct ThrfL2ks {
	1: required ThrfLkey cVkey;
	2: list<ThrfL2cl> cVcolumns;	
}

enum iEordertype {
	ASC = 0,
	DESC = 1
}

struct ThrfL2or {
	1: required string sVcolumn;
	2: required iEordertype  iVordertype;
}

struct ThrfL2wh {
	1: list<ThrfL2cl> cVcondition;
	2: list<ThrfL2or> cVorderby;
}

struct ThrfL2qr {
	//SELECT
	1: list<ThrfL2cl> cVselect;

	//FROM
	2: required ThrfLmtb cVmutable;

	//WHERE

	3: iEquerytype iVquery;
	4: ThrfLkey cVkey_start;
	5: ThrfLkey cVkey_end;
	6: ThrfL2wh cVwhere;
	7: i32 iVcount = 100;
	8: required bool bVonlysecondary = false;
	9: required bool bVdisableteleport = false;
}

struct ThrfL2os {
	1: required string sVnamespace;
	2: required string sVosqlstring;
	3: list<ThrfL2cv> cVinputbindings;
	4: required bool bVonlysecondary = false;
	5: bool bVdisableteleport = false;
	6: i64 iVtimestamp;
	7: bool bVfulltablescan = false;
}

enum iEstategossipnode {
	RUNNING = 0;
	JOINING = 1;
	LEAVING = 2;
	STOPPED = 3;
	LOADING = 4;
}

struct ThrfGoel {
	1: required string sVtoken;
	2: required string sVnodeid;
	3: required double dVphiaccrual;
	4: string sVaddress;
	5: i32 iVport;
	6: required string sVdatacenterid;	
	7: iEstategossipnode iVstate;
	8: i64 iVtimestamp;
	9: i64 iVsize;
}

struct ThrfGoss {
	1: list<ThrfGoel> cVgossipelement;
}

enum iEreservedkeyword {
	STARTUP = 0,
	SHUTDOWN = 1,
	VERS = 2,
	NETWORK = 3,
	ADD = 4,
	REMOVE = 5,
	JOIN = 6,
	LEAVE = 7,
	TABLET = 8,
	STORE = 9,
	FORGET = 10,
	LOAD = 11,
	STATS = 12,
	READ = 13,
	WRITE = 14,
	INFO = 15,
	TOUCH = 16,
	KILL = 17,
	STOPTHRIFT = 18,
	STARTTHRIFT = 19,
	DISCONNECT = 20,
	RESETLOCAL = 21,
	STATSTHRIFT = 22,
	ANTIENTROPHY = 23,
	PULLALL = 24,
	PPULLMATCH = 25,
	PPULLREPAIR = 26,
	TABLE = 27,
	REBUILD = 28,
	SPLIT = 30,
	JOINFAST = 31,
	STOPSEDA = 32,
	STARTSEDA = 33,
	ADDFAST = 34,
	FREEZE = 35,
	RELOAD = 36,
	TOP = 37,
	CLEAN = 38,
	PURGE = 39,
	REPAIR = 40,
	TRUNCATE = 41,
	SHRINK = 42,
	SOFTSHRINK = 43,
	COMPACTION = 44,
	REMOVEFAST = 45,
	LEAVEFAST = 46,
	REBUILDPART = 47,
	SIZE = 48,
	CLONE = 49,
	REBOUND = 50,
	REJOIN = 51,

	NORMAL = 100,
	ABORT = 101
}

struct ThrfPtel {
	1: required string sVnodeid;
	2: required string sVbound;
}

enum iEtablettype {
	MEMORYTABLETYPE = 0,
	SSTABLETYPE = 1
}

struct ThrfTlel {
	1: required iEtablettype iVtablettype;
	2: required string sVname;
	3: required bool bVvalid;

	4: required bool bVneedcompaction;
	5: required bool bVvalidasmemorytable;
	6: required i32 iVcompactionlevel;
	7: required string sVmemorytable;
	8: required i64 iVres;
	9: required i64 iVmapped;
	10: required i64 iVsize;
	11: required i64 iVcount;
	12: required string sVredotimestamp;
	13: required i32 iVsstabletype;
}

struct ThrfTabl {
	1: required string sVxml;
	2: list<ThrfPtel> cVpartitionedelements;
	3: list<ThrfPtel> cVpartitionedindexelements;
	4: list<ThrfTlel> cVtabletelement;
	5: list<ThrfTlel> cVindextabletelement;
	6: string sVjoincustompartition;
	7: string sVjoincustompartitionindex;
	8: string sVpartitiontype;
}

struct ThrfStel {
	1: required i64 iVcount;
	2: required i32 iVmeandelay;
	3: required i32 iVworstdelay;
	4: required i64 iVsize;

	5: i64 iVelapsed;
}

struct ThrfStth {
	1: required i32 iVprocessors;
	2: required i32 iVconnections;
	3: required i32 iVidleconnections;
	4: required i32 iVmaxprocessors;
	5: required i32 iVmaxconnections;
}

struct ThrfAenp {
	1: required string sVopaquedata;
	2: string sVnodeid;
	3: ThrfLmtb cVmutable;
	4: string sVkeystart;
	5: i32 iVlimitsize;
}

struct ThrfTope {
	1: required string sVtablet;
	2: required i64 iVreadtime;
	3: required i64 iVwritetime;
	4: required i64 iVreadcountl2;
	5: required i64 iVwritecountl2;
	6: required i64 iVreadcountl1;
	7: required i64 iVwritecountl1;
}

struct ThrfTop_ {
	1: required i64 iVtimestamp;
	2: list<ThrfTope> cVtopelement;
}

struct ThrfComm {
	1: iEreservedkeyword iVcommand;
	2: iEreservedkeyword iVsubcommand;
	3: string sVreturnmessage;
	4: string sVaddress;
	5: i32 iVport; 
	6: string sVtablet;
	7: ThrfLmtb cVmutable;
	8: ThrfAenp cVantientrophy;
	9: string sVnodeid;
	10: string sVnewtoken = "";
	11: ThrfLmtb cVmutabledestination;
	12: i64 iVtimestamp;
}

struct ThrfCort {
	1: bool bVreturn;
	2: string sVreturnmessage;
	3: ThrfTabl cVreturntable;
	4: ThrfStel cVreturnstats;
	5: ThrfStth cVreturnthriftstats;
	6: ThrfAenp cVreturnantientrophy;
	7: ThrfTop_ cVreturntop;
	8: i64 iVreturn;
}

struct ThrfLqry {
	1: required iEquerytype iVquery;
	2: required ThrfLmtb cVmutable;
	3: required ThrfLkey cVkey;
	4: required bool bVdigest = false;
	5: string sVcallback;
}

struct ThrfL2qb {
	1: list<ThrfL2ks> cKeyslices;
	2: bool bVreturn;
}

enum iEservicetype {
	STATEMENT = 0,
	QUERY = 1,
	OSQL = 2
}

struct ThrfSrvc {
	1: required iEservicetype iVservicetype;
	2: ThrfL2qr cVquery;
	3: ThrfL2st cVstatement;
	4: ThrfL2os cVosql;
}

struct ThrfSrvr {
	1: bool bVreturn;
	2: list<ThrfL2ks> cVdmlresult;
	3: ThrfL2qb cVqueryresult;
}

enum iEbulkservicetype {
	BULKTHRIFT = 0,
	BULKSTRING = 1,
	SINGLETHRIFT = 2,
	PING = 3
}

struct ThrfBsrc {
	1: required  iEbulkservicetype iVbulkservicetype;
	2: list<ThrfSrvc> cVbulkthrift;
	3: list<string> cVbulkstring;
	4: ThrfSrvc cVsinglethrift;
}

struct ThrfBsrr {
	1: list<ThrfSrvr> cVbulkthrift;
	2: list<string> cVbulkstring;
	3: ThrfSrvr cVsinglethrift;
}

service ThrfOrn_ {
	bool ping ();
	
	bool create (1: ThrfL2ct cCreateTable);
	bool statement (1: ThrfL2st cStatement);	
	ThrfL2qb query (1: ThrfL2qr cQuery);
	list<ThrfL2ks> osql (1: ThrfL2os cQuery);		
	ThrfGoss gossp (1: ThrfGoss cGossip, 2: bool bAll = false, 3: bool bDetails = false);
	ThrfCort command (1: ThrfComm cCommand);
	bool localstatement (1: list<ThrfLstm> cMutations);
	list<ThrfL1ks> localquery (1: list<ThrfLqry> cQueries);
	list<string> protobatch (1: list<string> sOpaquevalue);
	list<ThrfSrvr> thriftbatch (1: list<ThrfSrvc> cService);
}

