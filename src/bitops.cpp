#include "bitops.h"

uint32_t midilib::readVarInt(std::istream &stream) {
    constexpr auto maxSize = sizeof(uint32_t);
    auto ret = uint32_t{0};

    char c;

    for (size_t i = 0; i < maxSize; ++i) {
        stream >> c;
        ret += c;
        if (!(c & 0b1000'0000)) {
            break;
        }

        ret <<= 7;
    }

    return ret;
}
