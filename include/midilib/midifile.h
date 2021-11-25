#pragma once

#include <filesystem>
#include <iosfwd>
#include <vector>

namespace midifile {

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

// struct Event {
//     Event(std::istream &file);
// };

struct TrackChunk {
    TrackChunk(std::istream &file);

    uint32_t size;
};

struct Midifile {
    Midifile(std::filesystem::path path);

    std::vector<TrackChunk> tracks;
};

} // namespace midifile
