#include "midilib/metaevent.h"
#include "mls-unit-test/unittest.h"
#include <sstream>
#include <string_view>

using namespace midilib;

std::stringstream fakeFile(const char *str, size_t len) {
    auto ss = std::stringstream{};
    ss.write(str, len);
    return ss;
}

std::stringstream fakeFile(std::string_view str) {
    auto ss = std::stringstream{};
    ss.write(str.data(), str.length());
    return ss;
}

std::stringstream fakeFile(char c) {
    auto ss = std::stringstream{};
    ss.write(&c, 1);
    return ss;
}

// For segments where one character is zero
template <typename... Args>
std::stringstream fakeFile(char c, Args... args) {
    auto ss = fakeFile(args...);
    ss.write(&c, 1);
    return ss;
}

TEST_SUIT_BEGIN

TEST_CASE("sequence number") {
    auto ss = fakeFile("\x03\xff\x00\x02\x00\x08", 6);

    auto event = MetaEvent{ss};

    ASSERT_EQ(event.delta(), 3);
    ASSERT_EQ(event.type(), MetaEvent::SequenceNumber);
    ASSERT_EQ(event.sequenceNumber(), 8);
}

TEST_CASE("text events") {
    {
        auto ss = fakeFile("\x04\xff\x01\x05"
                           "hello",
                           9);

        auto event = MetaEvent{ss};

        ASSERT_EQ(event.delta(), 4);
        ASSERT_EQ(event.type(), MetaEvent::TextEvent);
        ASSERT_EQ(event.text(), "hello");
    }
}

TEST_SUIT_END
