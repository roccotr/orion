#include "DataXml_.hpp"

using namespace orion;

  const AnsiString DataXml_::sCtagorion_ = "ORION";
  const AnsiString DataXml_::sCtagnodeid = "NODEID";
	const AnsiString DataXml_::sCtagrepfac = "REPLICATIONFACTOR";
	const AnsiString DataXml_::sCtagdtacid = "DATACENTERID";	
	const AnsiString DataXml_::sCtagwcolev = "WRITECONSISTENCYLEVEL";	
	const AnsiString DataXml_::sCtagrcolev = "READCONSISTENCYLEVEL";	
	const AnsiString DataXml_::sCtagdtalev = "DATACENTERLEVEL";	
	const AnsiString DataXml_::sCtagpath__ = "PATH";
	const AnsiString DataXml_::sCtagtblspt = "TABLETSUBPATH";
	const AnsiString DataXml_::sCtagdtaspt = "DATASUBPATH";
	const AnsiString DataXml_::sCtagthrfad = "THRIFTADDRESS";
	const AnsiString DataXml_::sCtagthrfpt = "THRIFTPORT";
	const AnsiString DataXml_::sCtagthrtpt = "THRIFTTHREADEDPORT";
	const AnsiString DataXml_::sCtagthrgad = "THRIFTGOSSIPERADDRESS";
	const AnsiString DataXml_::sCtagthrgpt = "THRIFTGOSSIPERPORT";
	const AnsiString DataXml_::sCtagthrfls = "THRIFTLISTENERS";
	const AnsiString DataXml_::sCtagordcad = "ORDBCADDRESS";
	const AnsiString DataXml_::sCtagordcpt = "ORDBCPORT";
	const AnsiString DataXml_::sCtagrlgspt = "REDOLOGSUBPATH";
	const AnsiString DataXml_::sCtagrldim_ = "REDOLOGDIM";
	const AnsiString DataXml_::sCtagmaxcol = "MAXCOMPACTIONLEVEL";
	const AnsiString DataXml_::sCtagconwrt = "CONCURRENTWRITES";
	const AnsiString DataXml_::sCtagconrea = "CONCURRENTWRITES";
	const AnsiString DataXml_::sCtaggracet = "GRACETIME";
	const AnsiString DataXml_::sCtagmemtab = "MEMORYTABLE";
	const AnsiString DataXml_::sCtagtable_ = "TABLE";
  const AnsiString DataXml_::sCtagcolumn = "COLUMN";
  const AnsiString DataXml_::sCtagname__ = "NAME";
  const AnsiString DataXml_::sCtagcoltyp = "TYPE";	
  const AnsiString DataXml_::sCtagaccsgr = "ACCESSGROUP";
  const AnsiString DataXml_::sCtagindex_ = "INDEX";
  const AnsiString DataXml_::sCtagorder_ = "ORDER";
	const AnsiString DataXml_::sCtagstorge = "STORAGE";
	const AnsiString DataXml_::sCtagmemdim = "MEMTABLEDIM";
	const AnsiString DataXml_::sCtagmemsze = "MEMTABLESIZE";
	const AnsiString DataXml_::sCtagmemchk = "MEMTABLECHUNK";
	const AnsiString DataXml_::sCtagtbshfm = "SHARDINGFROM";
	const AnsiString DataXml_::sCtagtbshto = "SHARDINGTO";
	const AnsiString DataXml_::sCtagtbpaty = "PARTITIONTYPE";
	const AnsiString DataXml_::sCtagtitype = "INDEXTYPE";
	const AnsiString DataXml_::sCtagtishfm = "INDEXSHARDINGFROM";
	const AnsiString DataXml_::sCtagtishto = "INDEXSHARDINGTO";
	const AnsiString DataXml_::sCtagtbtype = "TABLETYPE"; 

  const AnsiString DataXml_::sCvalascend = "ASC";
	const AnsiString DataXml_::sCvalcoltps [] = {	"KEY", 
																								"STRING", "LISTSTRING", 
																								"INTEGER", "LISTINTEGER", 
																								"DOUBLE", "LISTDOUBLE", 
																								"BOOLEAN"};
	const int DataXml_::iCvalcoltps = 8;
  
  