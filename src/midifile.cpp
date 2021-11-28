
#include "midilib/midifile.h"
#include "bitops.h"
#include "verification.h"
#include <cstring>
#include <fstream>
#include <iostream>

// Midi specification
// http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html

namespace midilib {

FileHeaderChunk::FileHeaderChunk(std::istream &file) {
    constexpr auto header = std::array<char, 4>{'M', 'T', 'h', 'd'};

    if (!verifyHeader(file, header)) {
        throw std::runtime_error{"Not a midi file, no header present"};
    }

    readInt(file, &size);

    constexpr uint32_t expectedSize = 6;

    format = readInt<Format, uint16_t>(file);
    readInt(file, &ntks);
    readInt(file, &division);
    // Todo: Handle division

    file.ignore(size - expectedSize, EOF);

    std::cout << "Header size: " << size << "\n";
    std::cout << "Format: " << format << "\n";
}

Midifile::Midifile(std::filesystem::path path) {
    auto file = std::fstream{path, std::ios::in | std::ios::binary};

    if (!file) {
        throw std::runtime_error{"could not open file " + path.string()};
    }

    auto header = FileHeaderChunk{file};

    for (auto track = Track{file}; track.size; track = Track{file}) {
        tracks.push_back(std::move(track));
    }

    tracks.push_back(Track{file});
}

void Midifile::print(std::ostream &stream) {
    for (auto &track : tracks) {
        track.print(stream);
    }
}

} // namespace midilib
