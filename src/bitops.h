#pragma once

#include <bit>
#include <cstring>
#include <iostream>

namespace midilib {

//! Convert some bytes (in midi disk format) to a int in ram
template <typename ResultType, typename LoadType = ResultType>
ResultType readInt(const char *data, ResultType *retPtr = 0) {
    constexpr size_t size = sizeof(LoadType);

    auto ret = LoadType{};

    if constexpr (std::endian::native == std::endian::little) {
        for (size_t i = 0; i < size; ++i) {
            char c = data[i];
            ret <<= 8;
            ret += c;
        }
    }
    else {
        std::memcpy(&ret, data, size);
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
    constexpr size_t size = sizeof(LoadType);

    auto ret = LoadType{};

    if constexpr (std::endian::native == std::endian::little) {
        for (size_t i = 0; i < size; ++i) {
            char c;
            stream.read(&c, 1);
            ret <<= 8;
            ret += c;
        }
    }
    else {
        stream.read(&ret, size);
    }

    if (retPtr) {
        return *retPtr = static_cast<ResultType>(ret);
    }

    return static_cast<ResultType>(ret);
}

} // namespace midilib
