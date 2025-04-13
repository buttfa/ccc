#include "ccc/project.h"
#include <string>
#include <vector>
using namespace ccc;
using namespace std;

/* In the init_project function, we can describe and add the libraries and
 * applications that make up the project. */
void init_project(project* self, string cmd, vector<string> args) {
    execution exe("hello_world", "Say hello world!"); // Create an application
    exe.add_source_files({"./src"}, {".cpp"});        // Add source files
    self->add_exe(exe); // Add the application to the project
}

/* We did not request any resources that need to be manually processed in
 * init_project, so we did nothing in exit_project. */
void exit_project(project* self, string cmd, vector<string> args) {}

/* Register a project and specify its initialization and exit functions. */
project hello_world("hello_world", init_project, exit_project,
                    "Say hello world!");