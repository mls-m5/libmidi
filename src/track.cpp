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
            events.emplace_back(MetaEvent{file, t});
            if (get<MetaEvent>(events.back()).type() == MetaEvent::EndOfTrack) {
                break;
            }
        }
        else if (c & 0b1000'0000) {
            events.emplace_back(MidiMessage{file, t});
        }
        else {
            throw std::runtime_error{"unknown event type " + std::to_string(c)};
        }
    }
}

void Track::print(std::ostream &stream) {
    stream << "\nTrack\n";
    for (auto &event : events) {
        std::visit([&stream](auto &event) { event.print(stream); }, event);
    }
}

} // namespace midilib
