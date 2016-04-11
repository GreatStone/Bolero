#include <cstdio>
#include <assert.h>
#include <leveldb/db.h>
#include <vector>
#include <string>

#include "serv/server.h"

using namespace bolero;
using namespace leveldb;
int main() {
    Server server;
    assert(server.init("/home/vagrant/gra_pro/Bolero/conf/example.conf"));
    std::string fielda = "a";
    std::string fieldb = "b";
    Slice key;
    for (int i = 0; i < 1000; ++i) {
        int e = i + 100;
        std::vector<std::pair<leveldb::Slice, leveldb::Slice>> values;
        values.push_back(std::make_pair(fielda, Slice(reinterpret_cast<char*>(&i), sizeof(int))));
        values.push_back(std::make_pair(fieldb, Slice(reinterpret_cast<char*>(&e), sizeof(int))));
        assert(server.hmset(Slice(reinterpret_cast<char*>(&i), sizeof(int)), values).ok());
    }
    for (int i = 0; i < 10; ++i) {
        std::vector<Slice> fields;
        fields.push_back(fielda);
        fields.push_back(fieldb);
        std::vector<std::string> values;
        assert(server.hmget(Slice(reinterpret_cast<char*>(&i), sizeof(int)), fields, &values).ok());
        printf("key: %d a: %d b: %d\n", i, *(int*)values[0].data(), *(int*)values[1].data());
    }
    return 0;
}
