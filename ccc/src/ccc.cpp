#include "util/file.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char** argv) {

    /* Parse the command line arguments. */
    string cmd;
    for (int i = 1; i < argc; i++) {
        cmd += " " + string(argv[i]);
    }

    /* Set the default project configuration file. */
    string project_config_file = "project.cpp";

    /* If there is no project.cpp file in the current directory, run the default
     * cccpoject. */
    if (!fs::exists(project_config_file)) {
#ifdef _WIN32
        cmd = "default_project " + cmd;
#endif
#ifdef __linux__
        cmd = "bash -c 'default_project " + cmd + "'";
#endif
        if (std::system(cmd.c_str()) != 0)
            return -1;
        return 0;
    }

    /* Set the default project binary file. */
#ifdef _WIN32
    string project_bin_file = "project.exe";
#endif
#ifdef __linux__
    string project_bin_file = "./project";
#endif

    // Check if the required environment variables are set.
    const char* CCC_COMPILER = std::getenv("CCC_COMPILER");
    const char* CCC_INCLUDE_PATH = std::getenv("CCC_INCLUDE_PATH");
    const char* CCC_LIBRARY_PATH = std::getenv("CCC_LIBRARY_PATH");
    if (CCC_COMPILER == nullptr || CCC_INCLUDE_PATH == nullptr ||
        CCC_LIBRARY_PATH == nullptr) {
        cerr << "The required environment variables("
             << (CCC_COMPILER == nullptr ? "CCC_COMPILER " : "")
             << (CCC_INCLUDE_PATH == nullptr ? "CCC_INCLUDE_PATH " : "")
             << (CCC_LIBRARY_PATH == nullptr ? "CCC_LIBRARY_PATH" : "")
             << ") for ccc are missing. "
                "Please try reinstalling ccc."
             << endl;
        return -1;
    }

    /* Check if the project binary file is out of date. */
    std::string cccmain_path = string(CCC_LIBRARY_PATH) +
#ifdef _WIN32
                               "/libcccmain.lib";
#endif
#ifdef __linux__
    "/libcccmain.a";
#endif
    std::string cccproject_path = string(CCC_LIBRARY_PATH) +
#ifdef _WIN32
                                  "/libcccproject.lib";
#endif
#ifdef __linux__
    "/libcccproject.a";
#endif

    if (!fs::exists(project_bin_file) ||
        !compareFileModificationTime(project_config_file, project_bin_file) ||
        !compareFileModificationTime(cccmain_path, project_bin_file) ||
        !compareFileModificationTime(cccproject_path, project_bin_file)) {

#ifdef _WIN32
        string compile_cmd = CCC_COMPILER + string(" ") +
                             // project.cpp
                             project_config_file + " " +
                             // cccmain
                             cccmain_path + " " +
                             // cccproject
                             cccproject_path + " " +
                             // project bin
                             " -o " + project_bin_file + " -I " +
                             CCC_INCLUDE_PATH;
#endif
#ifdef __linux__
        string compile_cmd = CCC_COMPILER + string(" ") +
                             // cccmain
                             cccmain_path + " " +
                             // project.cpp
                             project_config_file + " " +
                             // cccproject
                             cccproject_path + " " +
                             // project bin
                             " -o " + project_bin_file + " -I " +
                             CCC_INCLUDE_PATH;
#endif
        /* Compile the project binary file. */
        if (std::system(compile_cmd.c_str()) != 0)
            return -1;
    }

    /* Run the project binary file. */
    cmd = project_bin_file + " " + cmd;
    if (std::system(cmd.c_str()) != 0)
        return -1;

    return 0;
}