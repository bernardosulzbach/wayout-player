#pragma once

#include <filesystem>
#include <string>

namespace WayoutPlayer {
std::string readFile(const std::filesystem::path &path);
}
