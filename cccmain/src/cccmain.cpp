#include "ccc/project.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::string target_cmd = "";

    // Determine the target command
    if (argc > 1)
        target_cmd = argv[1];

    // Get the arguments
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    // Run the target command
    if (ccc::cmds.find(target_cmd) != ccc::cmds.end()) {
        try {
            ccc::cmds[target_cmd]->run(args);
            return 0;                       // Normal execution returns 0
        } catch (const std::exception& e) { // Catch standard exceptions
            std::cerr << "Command execution failed: " << e.what() << std::endl;
            return 1;
        } catch (...) { // Catch other unknown anomalies
            std::cerr << "Command execution failed due to unknown exception"
                      << std::endl;
            return 1;
        }
    }

    std::cout << "Unknown command: " << target_cmd << std::endl;

    return 0;
}