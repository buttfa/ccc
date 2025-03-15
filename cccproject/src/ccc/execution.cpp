#include "ccc/execution.h"

void ccc::execution::process(ccc::config project_cfg) {
    this->handle(project_cfg);

    // If the build_dir_path doesn't exist, create it.
    if (!(fs::exists(this->build_dir_path) &&
          fs::is_directory(this->build_dir_path))) {
        fs::create_directories(this->build_dir_path);
    }

    std::string cmd = (
        // linker
        (this->config.linker.length() != 0 ? this->config.linker
                                           : project_cfg.linker) +
        " " +
        // Linker flags
        // std::accumulate(this->config.link_flags.begin(),
        //                 this->config.link_flags.end(), std::string(" ")) +
        " " +
        // Object files
        std::accumulate(this->obj_files.begin(), this->obj_files.end(),
                        std::string(" ")) +
        " -o " +
        // Output file
        this->build_dir_path + "/" + this->name);

    // Link
    std::system(cmd.c_str());
}
