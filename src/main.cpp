

#include "midilib/midifile.h"
#include <iostream>

int main(int argc, char **argv) {
    std::cout << "hello from " << argv[0] << std::endl;

    auto midi = midifile::Midifile{"data/test1.midi"};
}
