#include <IfceOrn_.hpp>
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



int main(int argc, char **argv)
{
#define iVwrites 1000000
    //#define iVwrites 10


    AnsiString sVkey;
    AnsiString sVbackupkey;
    PTR_POOL cVpoolmeot (new CorePool ());
    CoreMeot cVtestmeot (cVpoolmeot, "io", 10000000, 100000000, 10000000);
    PrtoLval cVvalue;
    cVvalue.set_ivtimestamp (0);
    auto tVstart = __Time ();
    auto tVdiff = 1;
    for (int iV = 0; iV < iVwrites; iV++) {
        AnsiString sVkey = (AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7);
        cVvalue.set_svopaquevalue (((AnsiString) iV).to_string ());
        cVtestmeot.put__ (sVkey, 12, &cVvalue);
    }
    auto tVend = __Time ();
    printf ("%i insert forward meot:    %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));
    cVtestmeot.clear ();

    using namespace orion;
    Instance db;
    Options options;
    options.log_path = "/Users/sommoyogurt/Desktop/orion_test/log";
    DB::DestroyDB (options, "/Users/sommoyogurt/Desktop/orion_test/orionfoldernew/test/");
    db = DB::Open (options, "/Users/sommoyogurt/Desktop/orion_test/orionfoldernew/test/");

    struct StorageOptions storageOptions (L1);
    DB::setVolatile (storageOptions);
    db->Create (storageOptions);
    struct WriteOptions writeOptions (L1);

    tVstart = __Time ();
    for (int iV = 0; iV < iVwrites; iV++) {
        AnsiString sVkey = (AnsiString) "test" + CoreButl::lpad_ ((AnsiString) iV, '0', 7);
        if (!db->Put (writeOptions, sVkey.to_string (), ((AnsiString) iV).to_string ()).ok ()) throw new Exception ();
    }
    tVend = __Time ();
    printf ("%i insert forward L1:    %f, qps %f\n",iVwrites, tVdiff * (tVend - tVstart), (double) iVwrites / (tVdiff * (tVend - tVstart)));
    orion::DB::Shutdown ();

}