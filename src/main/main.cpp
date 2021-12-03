

#include "midilib/midifile.h"
#include <iostream>

int main(int argc, char **argv) {
    //    auto midi = midilib::Midifile::load("data/test1.midi");
    auto midi = midilib::Midifile::load("data/rosegarden.midi");

    midi.print(std::cout);

    std::cout.flush();

    midi.save("testsave.midi");

    auto midi2 = midilib::Midifile::load("testsave.midi");
}
