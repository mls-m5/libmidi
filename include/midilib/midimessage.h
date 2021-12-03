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

    MidiMessage(std::istream &file, DeltaTimeT, uint8_t lastHeader = 0);

    void save(std::ostream &file) const;

    auto header() const -> uint8_t;
    auto type() const -> Type;
    auto channel() const -> uint8_t;
    auto key() const -> uint8_t;
    auto controlNumber() const -> uint8_t;
    auto controlValue() const -> uint8_t;
    auto velocity() const -> uint8_t;
    auto preassure() const -> uint8_t;
    auto pitchWheelValue() const -> uint16_t;
    void print(std::ostream &stream) const;
    auto name() const -> std::string_view;

private:
    uint8_t _header = 0;
    uint8_t _data1 = 0;
    uint8_t _data2 = 0;
};

} // namespace midilib
