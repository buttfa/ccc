#ifndef __COMPILE_H__
#define __COMPILE_H__

#include "ccc/config.h"
#include "ccc/util.h"
#include <filesystem>
#include <iostream>
#include <numeric>
#include <string>

namespace fs = std::filesystem;

namespace ccc {
class compile {
  public:
    std::string name;
    std::string description;
    std::string output_path;
    std::string obj_path;
    std::vector<std::string> source_files;
    std::vector<std::string> obj_files;

    ccc::config config;

    void handle(ccc::config project_cfg);
};
} // namespace ccc

#endif