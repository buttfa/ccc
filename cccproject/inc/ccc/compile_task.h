#ifndef __COMPILE_TASK_H__
#define __COMPILE_TASK_H__

#include "ccc/config.h"
#include "ccc/description.h"
#include "util/file.hpp"
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <thread>

namespace ccc {
class compile_task : public ccc::config_manager {
  public:
    compile_task(std::string name, std::string description);

    /* The name of the task.(The path of the final product is output_path +
     * name) */
    std::string name;
    /* The output path of the task.(The path of the final product is output_path
     * + name) */
    std::string output_path = "./build/";

    /* The path for storing intermediate products. */
    std::string obj_path = "./build/obj/";

    /* The source files of the task. */
    std::vector<std::string> source_files;

    /* The intermediate products of the task.(Automatically generated by the
     * compile_task class based on the source files, or can be added by
     * oneself.) */
    std::vector<std::string> obj_files;

    /**
     * @brief Compile all source files in source_files.
     *
     * @param project_cfg The config of the father project.
     */
    virtual void compile(const ccc::config& project_cfg) final;

    /**
     * @brief The subclass needs to implement this function, and the linking
     *        operation of the subclass needs to be implemented in this
     *        function. The compile function needs to be called at the beginning
     *        of the function.
     *
     * @param project_cfg The config of the father project.
     */
    virtual void process(const ccc::config& project_cfg) = 0;

    /**
     * @brief Check whether the task is ready to be compiled.
     *
     * @param project_cfg The config of the project.
     * @return true The task is ready to be compiled.
     * @return false The task is not ready to be compiled.
     */
    virtual bool check(const ccc::config& project_cfg) = 0;

    /* The convenient utility functions provided by ccc. */

    /**
     * @brief Add file_path to the source_files variable.
     *
     * @param file_path The file_path need to be added to the source_files
     *        variable.
     */
    void add_source_file(const std::string& file_path);

    /**
     * @brief Add these file_paths to the source_files variable.
     *
     * @param file_paths The file_paths need to be added to the source_files
     *        variable.
     */
    void add_source_files(const std::initializer_list<std::string>& file_paths);

    /**
     * @brief Add all files in the dir_paths which have the suffix in suffixs to
     *        the the source_files variable.(Default recursion into subfolders.)
     *
     * @param dir_paths The dir_paths which have the files need to be added to.
     * @param suffixs The suffixs of the files need to be added to the
     *        source_files variable.
     * @param recursive Decide whether to recursively enter subfolders.
     */
    void add_source_files(const std::initializer_list<std::string>& dir_paths,
                          const std::initializer_list<std::string>& suffixs,
                          bool recursive = true);

    /**
     * @brief Add all files in the dir_paths which satisfy the judge function to
     *        the the source_files variable.(Default recursion into subfolders.)
     *
     * @param dir_paths The dir_paths which have the files need to be added to.
     * @param judge The function which decides whether to add the file.
     * @param recursive Decide whether to recursively enter subfolders.
     */
    void add_source_files(const std::initializer_list<std::string>& dir_paths,
                          auto judge(const std::string&)->bool,
                          bool recursive = true);

    /**
     * @brief Remove file_path from the source_files variable.
     *
     * @param file_path The file_path need to be removed from the source_files
     *        variable.
     */
    void remove_source_file(const std::string& file_path);

    /**
     * @brief Remove these file_paths from the source_files variable.
     *
     * @param file_paths The file_paths need to be removed from the source_files
     *        variable.
     */
    void
    remove_source_files(const std::initializer_list<std::string>& file_paths);

    /**
     * @brief Remove all files in the source_files variable which satisfy the
     *        judge function.
     *
     * @param judge The function which decides whether to remove the file.
     * @return int The number of files removed.
     */
    int remove_source_files(auto judge(const std::string&)->bool);

    /**
     * @brief Find whether the file_path is in the source_files variable.
     *
     * @param file_path The file_path need to be found in the source_files.
     * @return true The file_path is in the source_files.
     * @return false The file_path is not in the source_files.
     */
    bool find_source_file(const std::string& file_path);

  private:
    /**
     * @brief Compile a source file.
     *
     * @param project_cfg The config of the father project.
     * @param source_file The source file need to be compiled.
     */
    void compile_source_file(const ccc::config& project_cfg,
                             const std::string& source_file);
};
} // namespace ccc

#endif