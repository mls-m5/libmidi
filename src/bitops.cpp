#include "bitops.h"
#include <array>

uint32_t midilib::readVarInt(std::istream &stream) {
    constexpr auto maxSize = sizeof(uint32_t);
    auto ret = uint32_t{0};

    char c;

    for (size_t i = 0; i < maxSize; ++i) {
        //        stream >> c;
        stream.read(&c, 1);
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

    for (size_t i = 0; i < 4; ++i) {
        unsigned char shifted = (value >> 7 * (maxSize - 1 - i)) & 0xff;
        unsigned char c = shifted & 0b0111'1111;
        if (shifted && i != 3) {
            c |= 0b1000'0000;
            ++num;
        }
        data.at(i) = c;
    }

    stream.write(data.data() + 3 - num, num + 1);
}
