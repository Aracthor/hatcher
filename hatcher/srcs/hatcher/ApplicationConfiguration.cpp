#include "ApplicationConfiguration.hpp"

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <string>

#include "assert.hpp"
#include "span.hpp"

namespace hatcher
{

namespace
{
class CommandLineIterator
{
public:
    CommandLineIterator(int argc, char** argv)
        : m_argc(argc)
        , m_argv(argv)
        , m_index(1)
    {
        HATCHER_ASSERT(argc > 0);
    }

    std::string operator*() const
    {
        HATCHER_ASSERT(!Ended());
        return {m_argv[m_index]};
    }

    CommandLineIterator& operator++(int)
    {
        m_index++;
        return *this;
    }

    bool Ended() const { return m_index >= m_argc; }

private:
    const int m_argc;
    const char* const* const m_argv;
    int m_index;
};

class ArgumentParser
{
public:
    ArgumentParser(const std::string& flag, std::optional<std::string>& result)
        : m_flag(flag)
        , m_result(result)
    {
    }

    bool Matches(const std::string& arg) const { return arg == m_flag; }

    void Process(CommandLineIterator& iterator) const
    {
        if (iterator.Ended())
            throw std::invalid_argument("Missing name after '" + m_flag + "'");

        m_result = *iterator;
        iterator++;
    }

private:
    const std::string m_flag;
    std::optional<std::string>& m_result;
};
} // namespace

ApplicationConfiguration::ApplicationConfiguration(int argc, char** argv)
{
#ifndef __EMSCRIPTEN__
    const std::string commandLineStr = argv[0];
    const std::size_t lastSlash = commandLineStr.rfind('/');
    if (lastSlash != std::string::npos)
    {
        pathToProject = commandLineStr.substr(0, lastSlash + 1);
    }
    pathToProject += "../";
#endif

    ArgumentParser parsers[] = {
        ArgumentParser("--save", this->commandSaveFile),
        ArgumentParser("--load", this->commandLoadFile),
    };
    const span<ArgumentParser> parsersSpan = span<ArgumentParser>(parsers, std::size(parsers));
    CommandLineIterator iterator(argc, argv);
    while (!iterator.Ended())
    {
        const std::string arg = *iterator;
        iterator++;
        auto it = std::find_if(parsersSpan.begin(), parsersSpan.end(),
                               [arg](const ArgumentParser& parser) { return parser.Matches(arg); });
        if (it == parsersSpan.end())
            throw std::invalid_argument("Unknown argument '" + arg + "'");
        it->Process(iterator);
    }
}

} // namespace hatcher
