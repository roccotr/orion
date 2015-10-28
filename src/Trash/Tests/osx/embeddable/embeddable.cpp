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

#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

std::string chars(
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890"
        "!@#$%^&*()"
        "`~-_=+[{]}\\|;:'\",<.>/? ");

void generate_random(std::string *value) {
    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);

    for (int i=0; i<10; i++) {
        value->push_back(chars[index_dist(rng)]);
    }
}

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

        string key;
        string value = "value";
        string res_value;
        db->Create (storage_options);
        orion::Status s = db->Put (write_options, "test", "peppemas");
        if (s.ok ()) {
          s = db->Get (read_options, "test", &value);
          if (s.ok ()) {
            printf ("result prima di delete: %s\n", value.c_str ());
          } else {
            printf ("[ERROR] non riesco a leggere la chiave\n");
          }
        } else {
          printf ("[ERROR] non riesco a scrivere il valore\n");
        }

        db->Delete (write_options, "test");
        if (s.ok ()) s = db->Get (read_options, "test", &value);
        if (s.ok ()) printf ("result dopo delete: %s\n", value.c_str ());

        s = db->Put (write_options, "test", "value2");
        if (s.ok ()) s = db->Get (read_options, "test", &value);
        if (s.ok ()) printf ("result prima di delete: %s\n", value.c_str ());

        orion::DB::Shutdown ();

    return 0;
}
