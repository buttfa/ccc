#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
#include <vector>

namespace ccc {
class command {
  public:
    /* Name of the command  */
    std::vector<std::string> names;

    /* Description of the command  */
    std::string description;

    /* Run the command  */
    void (*run)(std::vector<std::string> args);

    command(std::string name, auto (*run)(std::vector<std::string> args)->void,
            std::string description);

    command(std::vector<std::string> names,
            auto (*run)(std::vector<std::string> args)->void,
            std::string description);
};

extern std::vector<ccc::command*> cmds;

} // namespace ccc

#endif