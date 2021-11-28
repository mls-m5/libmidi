#pragma once

#include "event.h"

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

    Type type() const {
        return static_cast<Type>(_header >> 4);
    }

    uint8_t channel() const {
        return _header & 0b1111;
    }

    uint8_t key() const {
        return _data1;
    }

    uint8_t controlNumber() const {
        return key();
    }

    uint8_t velocity() const {
        return _data2;
    }

    uint8_t preassure() const {
        return velocity();
    }

    uint16_t pitchWheelValue() const { // Note that not all bits are used
        return ((static_cast<uint16_t>(_data1) | 0b0111'1111) << 7) +
               (_data2 | 0b0111'1111);
    }

    uint8_t _header = 0;
    uint8_t _data1 = 0;
    uint8_t _data2 = 0;
};

} // namespace midilib
