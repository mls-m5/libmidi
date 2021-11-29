#include "bitops.h"
#include "mls-unit-test/unittest.h"
#include <sstream>

using namespace midilib;

TEST_SUIT_BEGIN

TEST_CASE("load varint") {
    auto ss = std::stringstream{"\x81\x42"};

    for (unsigned char c : ss.str()) {
        std::cout << std::hex << static_cast<int>(static_cast<unsigned char>(c))
                  << " " << std::endl;
    }

    auto loaded = readVarInt(ss);

    EXPECT_EQ(loaded, 194);
}

TEST_CASE("save varint") {
    {
        auto ss = std::stringstream{};

        saveVarInt(ss, 194);

        EXPECT_EQ(ss.str(), "\x81\x42");
    }
}

TEST_SUIT_END
