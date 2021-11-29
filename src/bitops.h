#pragma once

#include <bit>
#include <cstring>
#include <iostream>

namespace midilib {

//! Convert some bytes (in midi disk format) to a int in ram
template <typename ResultType, typename LoadType = ResultType>
ResultType readInt(const char *data, ResultType *retPtr = 0) {
    constexpr auto size = sizeof(LoadType);

    auto ret = LoadType{};

    for (size_t i = 0; i < size; ++i) {
        char c = data[i];
        ret <<= 8;
        ret += c;
    }

    if (retPtr) {
        return *retPtr = static_cast<ResultType>(ret);
    }

    return static_cast<ResultType>(ret);
}

//! Read single int.
//! If retPtr is specified it will be set to the return value
//! The function always returns the value anyway
//! If reading to a enum for example the LoadType might differ
//! From the result type. The load type is what is read from the file
//! Usages:
//!        readInt(file, &size); // version 1
//!        size = readInt<uint32_t>(file); // version 2
template <typename ResultType, typename LoadType = ResultType>
ResultType readInt(std::istream &stream, ResultType *retPtr = 0) {
    constexpr auto size = sizeof(LoadType);

    auto ret = LoadType{};

    for (size_t i = 0; i < size; ++i) {
        unsigned char c;
        stream >> c;
        ret <<= 8;
        ret |= c;
    }

    if (retPtr) {
        return *retPtr = static_cast<ResultType>(ret);
    }

    return static_cast<ResultType>(ret);
}

// Read variable sized integer according to midi specification
uint32_t readVarInt(std::istream &stream);

// Save integer in midi format
template <typename Type, typename IntType = Type>
void saveInt(std::ostream &stream, Type in) {
    constexpr auto size = sizeof(IntType);
    auto value = static_cast<IntType>(in);

    for (size_t i = 0; i < size; ++i) {
        auto c = static_cast<char>((value >> 8 * (size - 1 - i)) & 0xff);
        stream.write(&c, 1);
    }
}

void saveVarInt(std::ostream &stream, uint32_t);

} // namespace midilib
