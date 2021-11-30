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

void MidiMessage::save(std::ostream &file) const {
    saveVarInt(file, delta());
    saveInt(file, _header);
    switch (type()) {
    case ProgramChange:
    case ChannelPressure:
        saveInt(file, _data1);
        break;
    default:
        saveInt(file, _data1);
        saveInt(file, _data2);
    }
}

MidiMessage::Type MidiMessage::type() const {
    return static_cast<Type>(_header >> 4);
}

uint8_t MidiMessage::channel() const {
    return _header & 0b1111;
}

uint8_t MidiMessage::key() const {
    return _data1;
}

uint8_t MidiMessage::controlNumber() const {
    return _data1;
}

uint8_t MidiMessage::controlValue() const {
    return _data2;
}

uint8_t MidiMessage::velocity() const {
    return _data2;
}

uint8_t MidiMessage::preassure() const {
    return velocity();
}

uint16_t MidiMessage::pitchWheelValue()
    const { // Note that not all bits are used
    return ((static_cast<uint16_t>(_data1) | 0b0111'1111) << 7) +
           (_data2 | 0b0111'1111);
}

void MidiMessage::print(std::ostream &stream) const {
    stream << "midi message " << std::hex << (_header >> 4 << 4) << " "
           << name() << " time = " << std::dec << delta() << "\n";
}

std::string_view MidiMessage::name() const {
    switch (type()) {
    case NoteOff:
        return "NoteOff";
    case NoteOn:
        return "NoteOn";
    case PolyphohnicKeyPressure:
        return "PolyphohnicKeyPressure";
    case ControlChange:
        return "ControlChange";
    case ProgramChange:
        return "ProgramChange";
    case ChannelPressure:
        return "ChannelPressure";
    case PitchWheelChange:
        return "PitchWheelChange";
    }
    return {};
}

} // namespace midilib
