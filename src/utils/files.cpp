//
// Created by xflajs00 on 22.10.2021.
//

#include "files.h"

std::filesystem::path pf::getExeFolder() {
  const auto currentPath = std::filesystem::current_path();
  return absolute(currentPath);
}
