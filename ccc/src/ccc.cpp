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
        cmd = "default_project " + cmd;
        if (std::system(cmd.c_str()) != 0)
            return -1;
        return 0;
    }

    /* Set the default project binary file. */
#ifdef _WIN32
    string project_bin_file = "project.exe";
#else
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
    if (!fs::exists(project_bin_file) ||
        !compareFileModificationTime(project_config_file, project_bin_file) ||
        !compareFileModificationTime(string(CCC_LIBRARY_PATH) + "/cccmain.a",
                                     project_bin_file) ||
        !compareFileModificationTime(string(CCC_LIBRARY_PATH) + "/cccproject.a",
                                     project_bin_file)) {

#ifdef _WIN32
        string compile_cmd = CCC_COMPILER + string(" ") +
                             // project.cpp
                             project_config_file + " " +
                             // cccproject
                             CCC_LIBRARY_PATH + "/cccproject.a " +
                             // cccmain
                             CCC_LIBRARY_PATH + "/cccmain.a " +
                             // project bin
                             " -o " + project_bin_file + " -I " +
                             CCC_INCLUDE_PATH;
#else
        string compile_cmd = CCC_COMPILER + string(" ") +
                             // cccmain
                             CCC_LIBRARY_PATH + "/cccmain.a" + " " +
                             // project.cpp
                             project_config_file + " " +
                             // cccproject
                             CCC_LIBRARY_PATH + "/cccproject.a " +
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