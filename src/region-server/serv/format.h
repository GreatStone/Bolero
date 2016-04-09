#ifndef BOLERO_FORMAT_H
#define BOLERO_FORMAT_H

#include <string>
#include <leveldb/db.h>

namespace bolero {
    class DataType {
    public:
        const static char DATA_HASH = 'h';
        //key value type just for debug/internal.
        const static char DATA_KV = 'k';
        const static char DATA_LINK = 'l';
        const static char META_RAW = 'm';
    };

    extern void encode_hash_key(leveldb::Slice user_key, leveldb::Slice field, std::string* hash_key);

    extern bool decode_hash_key(const std::string& hash_key, std::string* user_key, std::string* field);

}//namespace bolero

#endif//BOLERO_FORMAT_H
