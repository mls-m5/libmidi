#include "midilib/track.h"
#include "bitops.h"
#include "verification.h"
#include <array>

namespace midilib {

Track::Track(std::istream &file) {
    constexpr auto header = std::array<char, 4>{'M', 'T', 'r', 'k'};
    if (!verifyHeader(file, header)) {
        size = 0;
        return;
    }

    readInt(file, &size);

    for (auto t = readVarInt(file);; t = readVarInt(file)) {
        auto c = file.peek();

        if (c == 0xff) {
            metaEvents.emplace_back(file, t);
            if (metaEvents.back().type() == MetaEvent::EndOfTrack) {
                break;
            }
        }
        else if (c & 0b1000'0000) {
            messages.emplace_back(file, t);
        }
        else {
            throw std::runtime_error{"unknown event type " + std::to_string(c)};
        }
    }
}

} // namespace midilib
