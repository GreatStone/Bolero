#ifndef BOLERO_CODING_H
#define BOLERO_CODING_H

#include <stdint.h>
#include <string>

namespace bolero {
    template <typename T>
    static inline char* encode_varuint(char* buf, T value) {
        auto e = static_cast<unsigned>(value);
        unsigned char* ret = reinterpret_cast<unsigned char*>(buf);
        while (e > 127) {
            *ret = e | 128;
            ++ret;
            e >>= 7;
        }
        *(ret++) = e;
        return reinterpret_cast<char*>(ret);
    }
    template <typename T>
    static inline const char* decode_varuint(const char* buf, const char* limit, T* value) {
        *value = buf[0] & 127;
        if (!(buf[0] & 128)) {
            return buf + 1;
        }
        const char* cur = buf + 1;
        int shift = 7;
        for (; cur < limit; ++cur, shift += 7) {
            *value |= (*cur & 127) << shift;
            if (!(cur[0] & 128)) {
                break;
            }
        }
        if (cur == limit) {
            return nullptr;
        }
        return cur;
    }
    template <typename T>
    static inline void put_varuint(std::string* dst, T value) {
        char buf[sizeof(T) * 2];
        int length = encode_varuint(buf, value) - buf;
        dst->append(buf, length);
    }
    template <typename T>
    static inline char* encode_fixuint(char* buf, T value) {
        *(reinterpret_cast<T*>(buf)) = value;
        return buf + sizeof(T);
    }
    template <typename T>
    static inline const char* decode_fixuint(const char* buf, T* value) {
        *value = *(reinterpret_cast<const T*>(buf));
        return buf + sizeof(T);
    }
    template <typename T>
    static inline void put_fixuint(std::string* dst, T value) {
        char buf[sizeof(T) + 1];
        encode_fixuint(buf, value);
        dst->append(buf, sizeof(T));
    }
}//namespace bolero

#endif //BOLERO_CODING_H
