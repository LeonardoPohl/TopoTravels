#pragma once

#include <string>
#include <filesystem>

class Tile {
public:
  Tile(std::string filename) {
    this->filename /= filename;
  }

  std::filesystem::path filename{"resources/raw"};
};