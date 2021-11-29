
#include "midilib/midifile.h"
#include "bitops.h"
#include "verification.h"
#include <cstring>
#include <fstream>
#include <iostream>

// Midi specification
// http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html

namespace {
constexpr auto headerStr = std::array<char, 4>{'M', 'T', 'h', 'd'};
}

namespace midilib {

FileHeaderChunk::FileHeaderChunk(std::istream &file) {

    if (!verifyHeader(file, headerStr)) {
        throw std::runtime_error{"Not a midi file, no header present"};
    }

    readInt(file, &size);

    constexpr uint32_t expectedSize = 6;

    format = readInt<Format, uint16_t>(file);
    readInt(file, &ntks);
    readInt(file, &division);
    // Todo: Handle division

    file.ignore(size - expectedSize, EOF);
}

void FileHeaderChunk::save(std::ostream &file) const {
    file.write(headerStr.data(), headerStr.size());
    saveInt<uint32_t>(file, 6); // Header size

    saveInt<Format, uint16_t>(file, format);
    saveInt(file, ntks);
    saveInt(file, division);
}

void FileHeaderChunk::print(std::ostream &stream) {
    stream << "Header\n";
    std::cout << "Header size: " << size << "\n";
    std::cout << "Format: " << format << "\n";
}

Midifile::Midifile(std::istream &file)
    : header{file} {

    for (auto track = Track{file}; !track.events.empty(); track = Track{file}) {
        tracks.push_back(std::move(track));
    }
}

Midifile Midifile::load(std::filesystem::path path) {
    auto file = std::fstream{path, std::ios::in | std::ios::binary};

    if (!file) {
        throw std::runtime_error{"could not open file " + path.string()};
    }

    return Midifile{file};
}

void Midifile::save(std::filesystem::path path) const {
    auto file = std::fstream{path, std::ios::out | std::ios::binary};
    if (!file) {
        throw std::runtime_error{"could not open output file " + path.string()};
    }

    header.save(file);

    for (auto &track : tracks) {
        track.save(file);
    }
}

void Midifile::print(std::ostream &stream) {
    for (auto &track : tracks) {
        track.print(stream);
    }
}

} // namespace midilib
