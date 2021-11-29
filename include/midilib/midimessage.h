#pragma once

#include "event.h"
#include <string_view>

namespace midilib {

struct MidiMessage : Event {
    enum Type {
        NoteOff = 0b1000,
        NoteOn = 0b1001,
        PolyphohnicKeyPressure = 0b1010, // Aftertouch
        ControlChange = 0b1011,
        ProgramChange = 0b1100,
        ChannelPressure = 0b1101, // Aftertouch
        PitchWheelChange = 0b1110,
    };

    MidiMessage(std::istream &file, DeltaTimeT);

    void save(std::ostream &file) const;

    Type type() const;

    uint8_t channel() const;

    uint8_t key() const;

    uint8_t controlNumber() const;

    uint8_t controlValue() const;

    uint8_t velocity() const;

    uint8_t preassure() const;

    uint16_t pitchWheelValue() const;

    void print(std::ostream &stream) const;

    std::string_view name() const;

    uint8_t _header = 0;
    uint8_t _data1 = 0;
    uint8_t _data2 = 0;
};

} // namespace midilib
