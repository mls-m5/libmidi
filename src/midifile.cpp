
#include "midilib/midifile.h"

#include <bit>
#include <cstring>
#include <fstream>
#include <iostream>

// Midi specification
// http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html

namespace midifile {

//! Read single int.
//! If retPtr is specified it will be set to the return value
//! The function always returns the value anyway
//! If reading to a enum for example the LoadType might differ
//! From the result type. The load type is what is read from the file
//! Usages:
//!        readInt(file, &size); // version 1
//!        size = readInt<uint32_t>(file); // version 2
template <typename ResultType, typename LoadType = ResultType>
ResultType readInt(std::istream &stream, ResultType *retPtr = 0) {
    constexpr size_t size = sizeof(LoadType);

    auto ret = LoadType{};

    if constexpr (std::endian::native == std::endian::little) {
        for (size_t i = 0; i < size; ++i) {
            char c;
            stream.read(&c, 1);
            ret <<= 8;
            ret += c;
        }
    }
    else {
        stream.read(&ret, size);
    }

    if (retPtr) {
        return *retPtr = static_cast<ResultType>(ret);
    }

    return static_cast<ResultType>(ret);
}

bool verifyHeader(std::istream &file, const std::array<char, 4> header) {
    auto data = std::array<char, 4>{};
    file.read(data.data(), 4);

    if (header != data) {
        return false;
    }
    return true;
}

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

    for (auto track = TrackChunk{file}; track.size; track = TrackChunk{file}) {
        tracks.push_back(std::move(track));
    }
}

TrackChunk::TrackChunk(std::istream &file) {
    constexpr auto header = std::array<char, 4>{'M', 'T', 'r', 'k'};
    if (!verifyHeader(file, header)) {
        size = 0;
        return;
    }

    readInt(file, &size);
}

} // namespace midifile
