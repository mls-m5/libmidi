#include "midilib/midimessage.h"
#include "bitops.h"

namespace midilib {

MidiMessage::MidiMessage(std::istream &file, DeltaTimeT delta)
    : Event{delta} {
    _header = readInt<uint8_t>(file);
    switch (type()) {
    case ProgramChange:
    case ChannelPressure:
        _data1 = readInt<uint8_t>(file);
        break;
    default:
        _data1 = readInt<uint8_t>(file);
        _data2 = readInt<uint8_t>(file);
    }

    if ((_data1 >> 7) || (_data2 >> 7)) {
        throw std::runtime_error{"invalid values in midi message "};
    }
}

void MidiMessage::print(std::ostream &stream) {
    stream << "midi message time = " << std::dec << delta() << "   type = 0x"
           << std::hex << (_header >> 4 << 4) << "\n"
           << std::dec;
}

} // namespace midilib
