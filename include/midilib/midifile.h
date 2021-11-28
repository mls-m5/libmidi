#pragma once

#include "metaevent.h"
#include "midimessage.h"
#include "track.h"
#include <filesystem>
#include <iosfwd>
#include <vector>

namespace midilib {

struct FileHeaderChunk {
    enum Format {
        SingleMultiChannelTrack,
        OneOrMoreSimultaneousTracks,
        OneOrMoreSEquentiallyIndipendent
    };

    FileHeaderChunk(std::istream &file);

    void save(std::ostream &file) const;

    void print(std::ostream &file);

    uint32_t size;
    Format format;
    uint16_t ntks;
    uint16_t division;
};

struct Midifile {
    FileHeaderChunk header;

    Midifile(std::istream &file);
    static Midifile load(std::filesystem::path path);

    void save(std::filesystem::path path) const;

    std::vector<Track> tracks;

    void print(std::ostream &stream);
};

} // namespace midilib
