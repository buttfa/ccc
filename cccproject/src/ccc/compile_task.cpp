#include "ccc/compile_task.h"
ccc::compile_task::compile_task(std::string name, std::string description) {
    this->name = name;
    // Add description
    ccc::descs[name] = description;
}
void ccc::compile_task::compile(const ccc::config& project_cfg) {

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

    // Create a mutex and condition variable for thread synchronization.
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    std::atomic<size_t> active_threads(0);

    // Iteratively compile all source files.
    for (const auto& source_file : source_files) {
        // Wait until there is an available thread slot.
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return active_threads < thread_num; });

        // Increment the active thread count.
        active_threads++;

        // Launch a new thread to compile the source file.
        threads.emplace_back(
            [this, &project_cfg, source_file, &active_threads, &cv]() {
                compile_source_file(project_cfg, source_file);

                // Decrement the active thread count and notify the waiting
                // threads.
                active_threads--;
                cv.notify_one();
            });
    }

    // Wait for all threads to finish.
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

static std::mutex compile_mtx;
void ccc::compile_task::compile_source_file(const ccc::config& project_cfg,
                                            const std::string& source_file) {

    // Get the obj file path.
    std::string obj_file_path =
        this->obj_path + "/" + changeFileExtensionToO(source_file);
    // Add the obj file to the list.
    {
        std::lock_guard<std::mutex> lock(compile_mtx);
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
        std::lock_guard<std::mutex> lock(compile_mtx);
        std::cout << cmd << std::endl;
    }
    if (std::system(cmd.c_str()) != 0)
        return;
}

void ccc::compile_task::add_source_file(const std::string& file_path) {
    this->source_files.push_back(file_path);
    return;
}

void ccc::compile_task::add_source_files(
    const std::initializer_list<std::string>& file_paths) {
    for (auto file_path : file_paths) {
        this->source_files.push_back(file_path);
    }
    return;
}

void ccc::compile_task::add_source_files(
    const std::initializer_list<std::string>& dir_paths,
    const std::initializer_list<std::string>& suffixs, bool recursive) {
    namespace fs = std::filesystem;

    // Convert suffix initializer_list to vector for easier processing
    std::vector<std::string> suffix_vector(suffixs);

    // Process each directory in the input list
    for (const auto& dir : dir_paths) {
        const fs::path dir_path(dir);

        // Skip invalid directories
        if (!fs::exists(dir_path))
            continue; // Path doesn't exist
        if (!fs::is_directory(dir_path))
            continue; // Path isn't a directory

        std::vector<fs::directory_entry> entries;

        // Collect directory entries (recursive or non-recursive)
        if (recursive) {
            for (const auto& entry :
                 fs::recursive_directory_iterator(dir_path)) {
                entries.emplace_back(entry);
            }
        } else {
            for (const auto& entry : fs::directory_iterator(dir_path)) {
                entries.emplace_back(entry);
            }
        }

        // Process collected directory entries
        for (const auto& entry : entries) {
            // Skip non-files and symlinks
            if (!entry.is_regular_file() || entry.is_symlink())
                continue;

            // Extract file extension (includes the dot, e.g., ".cpp")
            const std::string ext = entry.path().extension().string();

            // Skip files without extensions
            if (!ext.empty()) {
                // Check if extension matches any of the target suffixes
                for (const auto& suffix : suffix_vector) {
                    if (ext == suffix) {

                        // Add normalized path to source files list
                        source_files.emplace_back(
                            entry.path().lexically_normal().string());
                        break; // No need to check other suffixes
                    }
                }
            }
        }
    }
}

void ccc::compile_task::add_source_files(
    const std::initializer_list<std::string>& dir_paths,
    auto judge(const std::string&)->bool, bool recursive) {
    return;
}

void ccc::compile_task::remove_source_file(const std::string& file_path) {
    size_t index = 0;
    for (size_t i = 0; i < source_files.size(); ++i) {
        if (source_files[i] != file_path) {
            // Move non-matching elements to front
            if (index != i) {
                source_files[index] = std::move(source_files[i]);
            }
            ++index;
        }
    }
    // Truncate vector to new size
    source_files.resize(index);
}

void ccc::compile_task::remove_source_files(
    const std::initializer_list<std::string>& file_paths) {
    // Iterate through paths to remove
    for (const auto& path : file_paths) {
        // Reimplement single removal logic for each path
        size_t index = 0;
        for (size_t i = 0; i < source_files.size(); ++i) {
            if (source_files[i] != path) {
                // Compact array by moving kept elements
                if (index != i) {
                    source_files[index] = std::move(source_files[i]);
                }
                ++index;
            }
        }
        source_files.resize(index);
    }
}

int ccc::compile_task::remove_source_files(bool (*judge)(const std::string&)) {
    int removed_count = 0;
    size_t index = 0;

    // Process all elements
    for (size_t i = 0; i < source_files.size(); ++i) {
        if (!judge(
                source_files[i])) { // Keep elements that don't match condition
            // Shift elements to maintain order
            if (index != i) {
                source_files[index] = std::move(source_files[i]);
            }
            ++index;
        } else { // Count removed elements
            ++removed_count;
        }
    }
    // Resize container after removal
    source_files.resize(index);
    return removed_count;
}

bool ccc::compile_task::find_source_file(const std::string& file_path) {
    // Implement linear search manually
    for (const auto& path : source_files) {
        if (path == file_path) {
            return true; // Early return when found
        }
    }
    return false; // Return false if not found
}