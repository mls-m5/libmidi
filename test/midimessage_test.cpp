#include "bitops.h"
#include "midilib/midimessage.h"
#include "mls-unit-test/unittest.h"
#include "testutils.h"

using namespace midilib;

TEST_SUIT_BEGIN

TEST_CASE("note off") {
    auto in = std::string_view{"\x04\x90\x40\x50\x00\x08", 7};
    auto ss = fakeFile(in);

    const auto time = readVarInt(ss);
    auto event = MidiMessage{ss, 0x06};

    EXPECT_EQ(event.delta(), 0x06);
    EXPECT_EQ(event.key(), 0x40);
    EXPECT_EQ(event.velocity(), 0x50);
}

TEST_CASE("note off load/save") {
    auto in = std::string_view{"\x04\x90\x40\x50\x00\x08", 7};
    auto ss = fakeFile(in);

    const auto time = readVarInt(ss);
    auto event = MidiMessage{ss, time};

    auto file = std::stringstream{};
    event.save(file);

    const auto loadedTime = readVarInt(file);
    auto loaded = MidiMessage{file, loadedTime};

    EXPECT_EQ(loadedTime, time);
    EXPECT_EQ(loaded.type(), event.type());
    EXPECT_EQ(loaded.key(), event.key());
    EXPECT_EQ(loaded.velocity(), event.velocity());
}

TEST_SUIT_END
