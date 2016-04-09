#include "format.h"

#include "util/coding.h"

namespace bolero {
    /*
     * Hash key format:
     * +---------------------------------------+
     * |  Type  |  Field-Length | Field |  Key |
     * +---------------------------------------+
     * |  1byte |    varuint    |       |      |
     * +---------------------------------------+
     */
    void encode_hash_key(leveldb::Slice user_key, leveldb::Slice field, std::string* hash_key) {
        hash_key->assign(1, DataType::DATA_HASH);
        put_varuint(hash_key, field.size());
        hash_key->append(field.data(), field.size());
        hash_key->append(user_key.data(), user_key.size());
    }

    bool decode_hash_key(const std::string& hash_key, std::string* user_key, std::string* field) {
        if (hash_key.empty() || hash_key.data()[0] != DataType::DATA_HASH) {
            return false;
        }
        uint64_t field_size;
        const char* pos = decode_varuint(hash_key.data() + 1, hash_key.data() + hash_key.length(), &field_size);
        uint64_t remain = hash_key.length() - (pos - hash_key.data());
        if (pos == nullptr || field_size > remain) {
            return false;
        }
        field->assign(pos, field_size);
        remain -= field_size;
        pos += field_size;
        if (remain < 0) {
            return false;
        }
        user_key->assign(pos, remain);
    }
}//namespace bolero
