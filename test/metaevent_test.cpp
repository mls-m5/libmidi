
#define DO_NOT_CATCH_ERRORS

#include "bitops.h"
#include "midilib/metaevent.h"
#include "mls-unit-test/unittest.h"
#include "testutils.h"
#include <sstream>
#include <string_view>

using namespace midilib;
using namespace std::literals;

TEST_SUIT_BEGIN

TEST_CASE("sequence number") {
    auto ss = fakeFile("\xff\x00\x02\x00\x08", 6);

    auto event = MetaEvent{ss, 3};

    EXPECT_EQ(event.delta(), 3);
    EXPECT_EQ(event.type(), MetaEvent::SequenceNumber);
    EXPECT_EQ(event.sequenceNumber(), 8);
}

TEST_CASE("sequence number load/save") {
    auto in = std::string_view{"\x03\xff\x00\x02\x00\x08", 6};
    auto ss = fakeFile(in);

    const auto time = readVarInt(ss);
    const auto event = MetaEvent{ss, time};

    auto file = std::stringstream{};
    event.save(file);

    auto loadedTime = readVarInt(file);
    auto loaded = MetaEvent{file, loadedTime};

    EXPECT_EQ(file.str(), in);

    EXPECT_EQ(loadedTime, time);
    EXPECT_EQ(loaded.type(), event.type());
    EXPECT_EQ(loaded.sequenceNumber(), event.sequenceNumber());
}

TEST_CASE("text events") {
    {
        auto ss = fakeFile("\xff\x01\x05"
                           "hello",
                           9);

        auto event = MetaEvent{ss, 4};

        EXPECT_EQ(event.delta(), 4);
        EXPECT_EQ(event.type(), MetaEvent::TextEvent);
        EXPECT_EQ(event.text(), "hello");
    }
}

TEST_CASE("text events load/save") {
    {
        auto in = std::string_view{"\xff\x01\x05"
                                   "hello",
                                   9};
        auto ss = fakeFile(in);

        auto event = MetaEvent{ss, 4};

        auto file = std::stringstream{};

        event.save(file);
        auto time = readVarInt(file);
        auto loaded = MetaEvent{file, time};

        EXPECT_EQ(time, 4);
        EXPECT_EQ(loaded.type(), event.type());
        EXPECT_EQ(loaded.text(), event.text());
    }
}

TEST_CASE("sequencer specific events") {
    auto in = "\xff\x7f\x05"
              "hello"sv;
    auto ss = fakeFile(in);

    auto event = MetaEvent{ss, 5};

    EXPECT_EQ(event.type(), MetaEvent::SequencerSpecificEvent);
    EXPECT_EQ(event.text(), "hello");
}

TEST_CASE("sequencer specific events load/save") {
    auto in = "\xff\x7f\x05"
              "hello"sv;
    auto ss = fakeFile(in);

    auto event = MetaEvent{ss, 5};

    auto file = std::stringstream{};

    event.save(file);

    for (auto c : file.str()) {
        auto c2 = static_cast<unsigned char>(c);
        std::cout << std::hex << static_cast<unsigned int>(c2) << " ";
    }

    auto time = readVarInt(file);
    auto loaded = MetaEvent{file, time};

    EXPECT_EQ(time, 5);
    EXPECT_EQ(loaded.type(), event.type());
    EXPECT_EQ(loaded.text(), event.text());
}

TEST_SUIT_END
