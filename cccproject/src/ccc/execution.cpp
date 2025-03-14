#include "ccc/execution.h"

void ccc::execution::process(ccc::config project_cfg) {
    this->handle(project_cfg);

    std::string cmd = (
        // linker
        (this->cfg.linker.length() != 0 ? this->cfg.linker
                                        : project_cfg.linker) +
        " " +
        // Linker flags
        std::accumulate(this->cfg.link_flags.begin(),
                        this->cfg.link_flags.end(), std::string(" ")) +
        " " +
        // Object files
        std::accumulate(this->obj_files.begin(), this->obj_files.end(),
                        std::string(" ")) +
        " -o " +
        // Output file
        this->build_dir_path + "/" + this->name);
    std::system(cmd.c_str());
}
