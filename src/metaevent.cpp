#include "midilib/metaevent.h"
#include "bitops.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace midilib {

std::string toHex(uint32_t val) {
    auto ss = std::stringstream{};
    ss << std::hex << val;
    return ss.str();
}

void expect(char value, char expected, std::string_view message) {
    if (value != expected) {
        throw std::runtime_error{std::string{message}};
    }
}

MetaEvent::MetaEvent(std::istream &file, DeltaTimeT time)
    : Event{time} {
    {
        unsigned char c;
        file >> c;
        _type = Error;
        if (c != 0xff) {
            throw std::runtime_error{"not a meta event"};
        }
    }

    _type = readInt<Type, unsigned char>(file);

    char c;

    switch (_type) {
    case SequenceNumber:
        readInt(file, &c);
        expect(c, 0x02, "Malformed sequence number");

        _data.resize(2);
        file.read(_data.data(), 2);

        break;
    case TextEvent:
    case CopyrightNotice:
    case TrackName:
    case InstrumentName:
    case Lyric:
    case Marker:
    case CuePoint:
    case SequencerSpecificEvent: {
        auto len = readVarInt(file);
        _data.resize(len);
        file.read(_data.data(), _data.size());

    } break;

    case EndOfTrack:
        readInt(file, &c);
        expect(c, 0, "Bad formatted End of Track event");
        break;

    case SetTempo:
        readInt(file, &c);
        expect(c, 3, "Bad formatted Set tempo event");
        _data.resize(3);
        file.read(_data.data(), _data.size());
        break;

    case TimeSignature:
        readInt(file, &c);
        expect(c, 4, "Time signature event");
        _data.resize(6);
        file.read(_data.data(), _data.size());

        break;

    default:
        throw std::runtime_error{"meta event 0x" + toHex(_type) +
                                 " not implemented"};
    }
}

void MetaEvent::save(std::ostream &file) const {
    saveVarInt(file, delta());
    saveInt<char, char>(file, 0xff);
    saveInt<Type, uint8_t>(file, type());

    switch (_type) {
    case SequenceNumber:
        saveVarInt(file, 2);
        file.write(_data.data(), 2);

        break;
    case TextEvent:
    case CopyrightNotice:
    case TrackName:
    case InstrumentName:
    case Lyric:
    case Marker:
    case CuePoint:
    case SequencerSpecificEvent:
        saveVarInt(file, _data.size());
        file.write(_data.data(), _data.size());
        break;
    case EndOfTrack:
        saveVarInt(file, 0);
        break;
    case SetTempo:
        saveVarInt(file, 3);
        file.write(_data.data(), _data.size());
        break;

    case TimeSignature:
        saveVarInt(file, 4);
        file.write(_data.data(), _data.size());
        break;

    default:
        throw std::runtime_error{"meta event 0x" + toHex(_type) +
                                 " not implemented"};
    }
}

MetaEvent::Type MetaEvent::type() const {
    return _type;
}

size_t MetaEvent::size() const {
    return _data.size();
}

const char *MetaEvent::data() const {
    return _data.data();
}

void MetaEvent::print(std::ostream &stream) const {
    stream << "meta event " << std::hex << type() << std::dec;
    stream << " " << name();

    switch (_type) {
    case TextEvent:
    case CopyrightNotice:
    case TrackName:
    case InstrumentName:
    case Lyric:
    case Marker:
    case CuePoint:
        stream << " '" << text() << "'";
        break;
    default:
        break;
    }

    stream << " time = " << delta();

    stream << "\n";
}

std::string_view MetaEvent::name() const {
#define CASE(name)                                                             \
    case name:                                                                 \
        return #name;

    switch (_type) {
        CASE(SequenceNumber)
        CASE(TextEvent)
        CASE(CopyrightNotice)
        CASE(TrackName)
        CASE(InstrumentName)
        CASE(Lyric)
        CASE(Marker)
        CASE(CuePoint)

        CASE(EndOfTrack)

        CASE(SetTempo)
        CASE(TimeSignature)

        CASE(SequencerSpecificEvent)
    default:
        return "no name";
    }
}

uint16_t midilib::MetaEvent::sequenceNumber() const {
    expect(type(),
           SequenceNumber,
           "trying to get sequence number of wrong type of event");
    expect(_data.size(), 2, "wrong size of sequence number data");

    return readInt<uint16_t>(_data.data());
}

std::string_view MetaEvent::text() const {
    return {_data.data(), _data.size()};
}

} // namespace midilib
