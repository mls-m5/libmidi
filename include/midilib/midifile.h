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

    uint32_t size;
    Format format;
    uint16_t ntks;
    uint16_t division;
};

struct Midifile {
    Midifile(std::filesystem::path path);

    std::vector<Track> tracks;
};

} // namespace midilib
