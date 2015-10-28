#include "IfceOrn_.hpp"
#include "Orion.cpp"
//#include "Unitt/UnitBase.hpp"
#include "TmplSdmg.cpp"
#include "TmplFsys.cpp"
#include "TmplList.cpp"
#include "TmplSdel.cpp"
#include "TmplSdmq.cpp"
#include "TmplOo2w.cpp"
#include "TmplOo2t.cpp"
#include "TmplClps.cpp"
#include "CoreSmph.cpp"


#include "TmplShst.cpp"

/*
int _tmaintest(int argc, _TCHAR** argv){

//#define iCserver

//#ifdef iCserver

if (argc == 1){
    CoreShst<ThrfSrvc>  cVsharedstructure (true, "test");
    while (true) {
        ThrfSrvc cVservice;
        cVservice = cVsharedstructure.popbg ();
        if (cVservice.iVservicetype != 2) {
            printf ("ERRORE\n");
            break;
        }
        cVsharedstructure.popen ();
    }
//#else
} else {
	int iVwrites = 1000000;
  TDateTime tVstart;
  TDateTime tVend;
  double tVdiff = 24*3600;
    CoreShst<ThrfSrvc>  cVsharedstructure (false, "test");
    tVstart = __Time ();
    for (int iV = 0; iV < iVwrites; iV++) {
        ThrfSrvc cVservice;
        cVservice.iVservicetype = iEservicetype::OSQL;
        cVsharedstructure.push_ (cVservice);
        printf ("%i\n", iV);
    }
    tVend = __Time ();
    printf ("%i writes:       %f, qps %f\n", iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));
//#endif
}
return 0;

}
*/
int main(int argc, char **argv) {
    //::testing::InitGoogleTest(&argc,argv);
    //int i = RUN_ALL_TESTS();
    //printf("%d test",i);
    return _tmain(argc, (_TCHAR **) argv);
}