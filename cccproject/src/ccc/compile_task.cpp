#include "ccc/compile_task.h"

void ccc::compile_task::handle(const ccc::config& project_cfg) {

    // Create a thread pool.
    unsigned int core_num = std::thread::hardware_concurrency();
    size_t thread_num =
        this->config.thread_num != 0
            ? this->config
                  .thread_num // Use the this.config thread num if it is not 0.

        : project_cfg.thread_num != 0
            ? project_cfg
                  .thread_num // Use the project.config thread num if it
                              // is not 0 and the this.config thread num is 0.

        : core_num != 0 ? core_num // If all the above variables are set, use
                                   // the kernel number as the parameter.

                        : 1; // If the number of cores cannot be obtained,
                             // set the parameter to 1.

    ThreadPool tp(thread_num);

    // Iteratively compile all source files.
    std::vector<std::future<void>> results;
    for (auto source_file : source_files) {
        results.push_back(tp.enqueue([this, &project_cfg, source_file]() {
            this->compile_source_file(project_cfg, source_file);
        }));
    }

    for (auto&& result : results) {
        result.wait();
    }
}

static std::mutex mtx;

void ccc::compile_task::compile_source_file(
    const ccc::config& project_cfg,
    std::string source_file) { // Get the obj file path.
    std::string obj_file_path =
        this->obj_path + "/" + changeFileExtensionToO(source_file);
    // Add the obj file to the list.
    {
        std::lock_guard<std::mutex> lock(mtx);
        obj_files.push_back(obj_file_path);
    }

    // If the obj file exists and is newer than the source file, skip it.
    if (fs::exists(obj_file_path) &&
        compareFileModificationTime(source_file, obj_file_path)) {
        return;
    }

    // Get the target folder(The storage path of the obj file.)
    std::string target_folder = extractPath(obj_file_path);
    // If the folder doesn't exist, create it.
    if (!directoryExists(target_folder)) {
        fs::create_directories(target_folder);
    }

    std::string cmd = (
        // Compiler
        (this->config.compiler.length() != 0 ? this->config.compiler
                                             : project_cfg.compiler) +
        // Only compile without linking
        " -c " +
        // Source file
        source_file + " -o " +
        // Output file
        obj_file_path + " " +
        // Compile flags from project
        joinWithSpace(project_cfg.compile_flags) + " " +
        // Compile flags from execution
        joinWithSpace(this->config.compile_flags));

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << cmd << std::endl;
    }
    std::system(cmd.c_str());
}