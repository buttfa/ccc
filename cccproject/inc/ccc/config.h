#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdint>
#include <string>
#include <vector>

namespace ccc {
class config {
  public:
    /* The compiler */
    std::string compiler = "g++";
    /* The compile flags */
    std::vector<std::string> compile_flags;
    /* The linker */
    std::string linker = "g++";
    /* The link flags */
    std::vector<std::string> link_flags;

    /* The header folder paths */
    std::vector<std::string> header_folder_paths;

    /* The macros */
    std::vector<std::string> macros;

    uint32_t thread_num = 0;

    config() = default;

    config(const config& other)
        : compiler(other.compiler), compile_flags(other.compile_flags),
          linker(other.linker), link_flags(other.link_flags),
          header_folder_paths(other.header_folder_paths), macros(other.macros),
          thread_num(other.thread_num) {}
};

class config_manager {
  public:
    /* The configuration. */
    ccc::config config;

    config_manager() = default;

    config_manager(const ccc::config& other) : config(other) {}

    /**
     * @brief Set the config object from another config object.
     *
     * @param cfg The configuration need to be copy.
     */
    virtual void set_config(const ccc::config& cfg) final;

    /**
     * @brief Set the compiler object
     *
     * @param compiler The compiler is used to compile.
     */
    virtual void set_compiler(const std::string& compiler) final;

    /**
     * @brief Set the compile flags object
     *
     * @param compile_flags The compile_flags is used to compile.
     */
    virtual void set_compile_flags(
        const std::initializer_list<std::string>& compile_flags) final;

    /**
     * @brief Add the compile flags
     *
     * @param compile_flags The compile_flags is used to compile.
     */
    virtual void add_compile_flags(
        const std::initializer_list<std::string>& compile_flags) final;

    /**
     * @brief Set the linker object
     *
     * @param linker The linker is used to link.
     */
    virtual void set_linker(const std::string& linker) final;

    /**
     * @brief Set the link flags object
     *
     * @param link_flags The link_flags is used to link.
     */
    virtual void
    set_link_flags(const std::initializer_list<std::string>& link_flags) final;

    /**
     * @brief Add the link flags
     *
     * @param link_flags The link_flags is used to link.
     */
    virtual void
    add_link_flags(const std::initializer_list<std::string>& link_flags) final;

    /**
     * @brief Set the thread num object
     *
     * @param num The thread num when compile.
     */
    virtual void set_thread_num(uint32_t num) final;

    /**
     * @brief Set the header folder paths object.
     *
     * @param header_folder_paths The header folder paths need to be set.
     */
    virtual void set_header_folder_paths(
        const std::initializer_list<std::string>& header_folder_paths) final;

    /**
     * @brief Add the header folder paths.
     *
     * @param header_folder_paths The header folder paths need to be add.
     */
    virtual void add_header_folder_paths(
        const std::initializer_list<std::string>& header_folder_paths) final;

    /**
     * @brief Set the marcos object
     *
     * @param macros The marcos need to be set.
     */
    virtual void
    set_marcos(const std::initializer_list<std::string>& macros) final;

    /**
     * @brief Add the marcos.
     *
     * @param macros The marcos need to be add.
     */
    virtual void
    add_marcos(const std::initializer_list<std::string>& macros) final;
};

} // namespace ccc

#endif