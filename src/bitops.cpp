#include "bitops.h"
#include <array>

uint32_t midilib::readVarInt(std::istream &stream) {
    constexpr auto maxSize = sizeof(uint32_t);
    auto ret = uint32_t{0};

    char c;

    for (size_t i = 0; i < maxSize; ++i) {
        stream >> c;
        ret += (c & 0b0111'1111);
        if (!(c & 0b1000'0000)) {
            break;
        }

        ret <<= 7;
    }

    return ret;
}

void midilib::saveVarInt(std::ostream &stream, uint32_t value) {
    constexpr auto maxSize = sizeof(uint32_t);

    auto data = std::array<char, maxSize>{};

    size_t num = 0;

    for (size_t i = 0; i < maxSize && value; ++i) {
        char c = (value & 0b0111'1111) | (0b1000'0000 * (value > 127));
        data.at(maxSize - 1 - i) = c;
        value >>= 7;
        num = i;
    }

    stream.write(data.data() + 3 - num, num + 1);
}
