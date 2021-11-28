#include "midilib/midimessage.h"
#include "mls-unit-test/unittest.h"
#include "testutils.h"

using namespace midilib;

TEST_SUIT_BEGIN

TEST_CASE("note off") {
    auto ss = fakeFile("\x90\x40\x50\x00\x08", 6);

    auto event = MidiMessage{ss, 0x06};

    EXPECT_EQ(event.delta(), 0x06);
    EXPECT_EQ(event.key(), 0x40);
    EXPECT_EQ(event.velocity(), 0x50);
}

TEST_SUIT_END
