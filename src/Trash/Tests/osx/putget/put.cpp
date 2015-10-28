#include "IfceOrn_.hpp"
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

int main(int argc, char **argv) {
    orion::Instance db;
    orion::Options options;

    options.log_path = "./templog";
    options.error_if_exists = false;
    db = orion::DB::Open (options, "./orionfoldernew");

    auto storage_options = orion::StorageOptions (L1);
    auto write_options = orion::WriteOptions (L1);
    auto read_options = orion::ReadOptions(L1);
    auto read_options2 = orion::ReadOptions(L1);

    string key = "test";
    string value = "value";
    string res_value;
    db->Create (storage_options);
    orion::Status s;

    s = db->Put (write_options, key, value);
    if (s.ok ()) {
        printf("put successfully executed\n");
    } else {
        printf("cannot put key\n");
    }

    orion::DB::Shutdown ();

    return 0;
}
