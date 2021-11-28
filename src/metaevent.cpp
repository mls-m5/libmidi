#include "midilib/metaevent.h"
#include "bitops.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace midilib {

void expect(char value, char expected, std::string_view message) {
    if (value != expected) {
        throw std::runtime_error{std::string{message}};
    }
}

MetaEvent::MetaEvent(std::istream &file) {
    {
        unsigned char c;
        file >> c;
        if (c != 0xff) {
            throw std::runtime_error{"not a meta event"};
        }
    }

    char c, c2;

    readInt(file, &c);

    switch (c) {
    case SequenceNumber:
        readInt(file, &c2);
        expect(c2, 0x02, "Malformed sequence number");

        _data.resize(2);
        file.read(_data.data(), 2);

        break;
    }
}

uint16_t midilib::MetaEvent::sequenceNumber() {
    expect(type(),
           SequenceNumber,
           "trying to get sequence number of wrong type of event");
    expect(_data.size(), 2, "wrong size of sequence number data");

    return readInt<uint16_t>(_data.data());
}

} // namespace midilib
