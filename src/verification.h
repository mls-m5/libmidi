#pragma once
#include <array>
#include <iostream>

namespace midilib {

inline bool verifyHeader(std::istream &file, const std::array<char, 4> header) {
    auto data = std::array<char, 4>{};
    file.read(data.data(), 4);

    if (header != data) {
        return false;
    }
    return true;
}

} // namespace midilib
