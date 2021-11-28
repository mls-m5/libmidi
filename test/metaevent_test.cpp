#include "midilib/metaevent.h"
#include "mls-unit-test/unittest.h"
#include "testutils.h"
#include <sstream>
#include <string_view>

using namespace midilib;

TEST_SUIT_BEGIN

TEST_CASE("sequence number") {
    auto ss = fakeFile("\xff\x00\x02\x00\x08", 6);

    auto event = MetaEvent{ss, 3};

    EXPECT_EQ(event.delta(), 3);
    EXPECT_EQ(event.type(), MetaEvent::SequenceNumber);
    EXPECT_EQ(event.sequenceNumber(), 8);
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

TEST_CASE("sequencer specific events") {
    auto ss = fakeFile("\xff\x7f\x05"
                       "hello");

    auto event = MetaEvent{ss, 5};

    EXPECT_EQ(event.type(), MetaEvent::SequencerSpecificEvent);
    EXPECT_EQ(event.text(), "hello");
}

TEST_SUIT_END
