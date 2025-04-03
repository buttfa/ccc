#include "ccc/project.h"

namespace ccc {
std::vector<ccc::project*> projects;
}

ccc::project::project(
    auto (*init_func)(project*, std::string, std::vector<std::string>)->void,
    auto (*exit_func)(project*, std::string, std::vector<std::string>)->void)
    : init_func(init_func), exit_func(exit_func) {
    // Default config
    this->config.compiler = "g++";
    this->config.linker = "g++";
    // Add project
    ccc::projects.push_back(this);
}

ccc::project::project(
    std::string name,
    auto (*init_func)(project*, std::string, std::vector<std::string>)->void,
    auto (*exit_func)(project*, std::string, std::vector<std::string>)->void,
    std::string description)
    : init_func(init_func), exit_func(exit_func) {
    // Default config
    this->config.compiler = "g++";
    this->config.linker = "g++";
    // Add project
    ccc::projects.push_back(this);
    // Add description
    ccc::descs[name] = description;
}
void ccc::project::process() {
    for (auto& lib : libs)
        if (lib.check(this->config))
            lib.process(this->config);

    for (auto& exe : exes)
        if (exe.check(this->config))
            exe.process(this->config);
}

void ccc::project::set_config(std::string compiler,
                              std::vector<std::string> compile_flags,
                              std::string linker,
                              std::vector<std::string> link_flags) {
    return;
}

void ccc::project::add_exe(ccc::execution exe) { return; }

void ccc::project::add_lib(ccc::library lib) { return; }