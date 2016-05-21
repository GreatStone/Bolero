#ifndef BOLERO_REGIONS_H
#define BOLERO_REGIONS_H

#include "leveldb/slice.h"

#include <set>
#include <memory>
#include <stdint.h>
#include <string.h>

using std::shared_ptr;
using std::weak_ptr;

namespace bolero {
    typedef uint32_t (* dispatch_hash_handle_t) (::leveldb::Slice key);

    extern uint32_t default_hash_handle(::leveldb::Slice key);
    extern shared_ptr<struct Region> new_region(const std::string& rname,
                                                const std::string& raddr,
                                                unsigned rreadable,
                                                unsigned rwritable);

    struct Region {
        std::string name;
        std::string addr;
        unsigned readable:1;
        unsigned writable:1;
        uint32_t hash_pos;
    };

    inline shared_ptr<struct Region> nullregion() {
        static shared_ptr<struct Region> _nullregion(nullptr);
        return _nullregion;
    }

    /*
     * HashPool is a tool to dispatch key to regions.
     * This class is not thread-safe.
     * The template argument k_pool_size is the maximal number of Regions.
     * Every function use this class should use reput after insert all of the regions.
     */
    template <int k_pool_size>
    class HashPool {
    public:
        HashPool() : _positions(), _hash_handle(default_hash_handle), _regions() {
            for (int i = 0; i < k_pool_size; ++i) {
                _positions[i] = nullregion();
            }
        }
        shared_ptr<struct Region> hash_dispatch(::leveldb::Slice key);
        bool insert_region(const shared_ptr<struct Region>& region);
        void remove_region(const shared_ptr<struct Region>& region);
        void reput();
        int pool_size() const {return k_pool_size;}
        void set_hash_handle(dispatch_hash_handle_t handle) { _hash_handle = handle; }
    private:
        uint32_t hash_index(const std::string& region_name);
        bool insert_callback(const shared_ptr<struct Region>& region);

        class _RegionCompare {
        public:
            bool operator()(shared_ptr<struct Region> a, shared_ptr<struct Region> b) {
                return a->name < b->name;
            }
        };

        weak_ptr<struct Region> _positions[k_pool_size];
        dispatch_hash_handle_t _hash_handle;
        std::set<shared_ptr<struct Region>, _RegionCompare> _regions;

        static const int _max_rehash = 10;

        HashPool(const HashPool&);
        HashPool& operator=(const HashPool&);
    };

    template <int k_pool_size>
    bool HashPool<k_pool_size>::insert_callback(const shared_ptr<struct Region>& region) {
        uint32_t hash_pos = _hash_handle({region->name.data(), region->name.length()}) % k_pool_size;
        uint32_t pos = hash_pos % k_pool_size;
        if (_positions[pos].lock() != nullregion() && _positions[pos].lock()->hash_pos == pos) {
            int cnt = 0;
            while (_positions[pos].lock()->hash_pos == pos && cnt < _max_rehash) {
                hash_pos = _hash_handle({reinterpret_cast<char*>(&hash_pos), sizeof(uint32_t)});
                pos = hash_pos % k_pool_size;
                ++cnt;
            }
        }
        if (_positions[pos].lock() != nullregion() && _positions[pos].lock()->hash_pos == pos) {
            return false;
        }
        region->hash_pos = pos;
        while (_positions[pos].lock() == nullregion() || _positions[pos].lock()->hash_pos != pos) {
            _positions[pos++] = region;
            if (pos >= k_pool_size) {
                pos %= k_pool_size;
            }
        }
        return true;
    }

    template <int k_pool_size>
    bool HashPool<k_pool_size>::insert_region(const shared_ptr<struct Region>& region) {
        _regions.insert(region);
        return insert_callback(region);
    }

    template <int k_pool_size>
    void HashPool<k_pool_size>::remove_region(const shared_ptr<struct Region>& region) {
        auto iter = _regions.find(region);
        if (iter == _regions.end()) {
            return;
        }
        _regions.erase(iter);
        uint32_t pos = region->hash_pos;
        weak_ptr<struct Region> replace = _positions[(pos + k_pool_size - 1) % k_pool_size];
        if (replace.lock() == region) {
            replace = nullregion();
        }
        while (_positions[pos] == region) {
            _positions[pos++] = replace;
            if (pos >= k_pool_size) {
                pos %= k_pool_size;
            }
        }
    }

    template <int k_pool_size>
    shared_ptr<struct Region> HashPool<k_pool_size>::hash_dispatch(::leveldb::Slice key) {
        uint32_t pos = _hash_handle(key);
        if (pos >= k_pool_size) {
            pos %= k_pool_size;
        }
        return _positions[pos].lock();
    }

    template <int k_pool_size>
    void HashPool<k_pool_size>::reput() {
        for (int i = 0; i < k_pool_size; ++i) {
            _positions[i] = nullregion();
        }
        for (shared_ptr<struct Region> e : _regions) {
            insert_callback(e);
        }
    }
}//namespace bolero

#endif//BOLERO_REGIONS_H
