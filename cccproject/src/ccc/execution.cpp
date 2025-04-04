#include "ccc/execution.h"

void ccc::execution::process(ccc::config project_cfg) {
    this->handle(project_cfg);

    // If the output_path doesn't exist, create it.
    if (!(fs::exists(this->output_path) &&
          fs::is_directory(this->output_path))) {
        fs::create_directories(this->output_path);
    }

    std::string cmd = (
        // linker
        (this->config.linker.length() != 0 ? this->config.linker
                                           : project_cfg.linker) +
        " " +
        // Object files
        joinWithSpace(this->obj_files) + " -o " +
        // Output file
        (this->output_path.length() != 0 ? this->output_path : "./build/bin") +
        "/" + this->name + " " +
        // Linker flags from project
        joinWithSpace(project_cfg.link_flags) + " " +
        // Linker flags from execution
        joinWithSpace(this->config.link_flags));

    // Link
    std::cout << cmd << std::endl;
    std::system(cmd.c_str());
}
