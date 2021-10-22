//
// Created by xflajs00 on 22.10.2021.
//

#ifndef OPENGL_TEMPLATE_SRC_UTILS_FILES_H
#define OPENGL_TEMPLATE_SRC_UTILS_FILES_H

#include <string>
#include <filesystem>

namespace pf {

/**
 * @return absolute path to folder of current executable
 */
[[nodiscard]] std::filesystem::path getExeFolder();

}

#endif//OPENGL_TEMPLATE_SRC_UTILS_FILES_H
