#include "midilib/track.h"
#include "bitops.h"
#include "verification.h"
#include <array>
#include <sstream>

namespace {
constexpr auto header = std::array<char, 4>{'M', 'T', 'r', 'k'};
}

namespace midilib {

Track::Track(std::istream &file) {
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

void Track::save(std::ostream &stream) const {
    stream.write(header.data(), header.size());

    auto ss = std::stringstream{};

    for (auto &event : events) {
        std::visit([&ss](auto &event) { event.save(ss); }, event);
    }

    auto size = ss.str().size();
    saveInt<uint32_t>(stream, size);
    stream.write(ss.str().data(), size);
}

void Track::print(std::ostream &stream) const {
    stream << "\nTrack\n";
    for (auto &event : events) {
        std::visit([&stream](auto &event) { event.print(stream); }, event);
    }
}

} // namespace midilib
