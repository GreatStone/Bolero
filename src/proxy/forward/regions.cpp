#include "regions.h"

namespace bolero {
    shared_ptr<struct Region> new_region(const std::string& rname, const std::string& raddr,
                                         unsigned rreadable, unsigned rwritable) {
        shared_ptr<struct Region> ret(new struct Region);
        if (ret == nullptr) {
            return nullptr;
        }
        ret->name = rname;
        ret->addr = raddr;
        ret->readable = rreadable;
        ret->writable = rwritable;
        return ret;
    }
    /*
     * Hash function - murmur
     */
    uint32_t default_hash_handle(::leveldb::Slice key) {
        uint32_t length = key.size();
        const unsigned int m = 0x5bd1e995;
        const uint32_t seed = (0xdeadbeef * length);
        const int r = 24;

        uint32_t h = seed ^ (uint32_t)length;

        const unsigned char * data = reinterpret_cast<const unsigned char *>(key.data());

        while (length >= 4) {
            unsigned int k = *(unsigned int *)data;
            k *= m;
            k ^= k >> r;
            k *= m;
            h *= m;
            h ^= k;
            data += 4;
            length -= 4;
        }

        switch(length) {
        case 3:
            h ^= ((uint32_t)data[2]) << 16;
        case 2:
            h ^= ((uint32_t)data[1]) << 8;
        case 1:
            h ^= data[0];
            h *= m;
        default:
            break;
        };

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }
}//namespace bolero
