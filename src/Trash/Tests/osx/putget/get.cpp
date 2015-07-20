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
    auto read_options = orion::ReadOptions(L1);

    string key = "test";
    string value = "value";
    string res_value;
    db->Create (storage_options);
    orion::Status s;

    s = db->Get (read_options, key, &res_value);
    if (s.ok ()) {
        printf("get successfully executed. value is '%s'\n", res_value.c_str());
    } else {
        printf("cannot get key '%s'\n",key.c_str());
    }

    orion::DB::Shutdown ();

    return 0;
}
