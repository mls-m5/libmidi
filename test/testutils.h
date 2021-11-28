#pragma once

#include <sstream>

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
