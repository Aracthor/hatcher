#include "ApplicationConfiguration.hpp"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

#include "assert.hpp"
#include "span.hpp"
#include "unique_ptr.hpp"

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

class IArgumentParser
{
public:
    virtual ~IArgumentParser() = default;

    virtual bool Matches(const std::string& arg) const = 0;
    virtual void Process(CommandLineIterator& iterator) const = 0;
};

class ArgumentParserOptionalInteger64 : public IArgumentParser
{
public:
    ArgumentParserOptionalInteger64(const char* flag, std::optional<int64_t>& result)
        : m_flag(flag)
        , m_result(result)
    {
    }

    bool Matches(const std::string& arg) const override { return arg == m_flag; }

    void Process(CommandLineIterator& iterator) const override
    {
        if (iterator.Ended())
            throw std::invalid_argument("Missing number after '" + m_flag + "'");

        std::istringstream iss(*iterator);
        m_result = 0;
        iss >> *m_result;
        iterator++;
    }

private:
    const std::string m_flag;
    std::optional<int64_t>& m_result;
};

class ArgumentParserOptionalString : public IArgumentParser
{
public:
    ArgumentParserOptionalString(const char* flag, std::optional<std::string>& result)
        : m_flag(flag)
        , m_result(result)
    {
    }

    bool Matches(const std::string& arg) const override { return arg == m_flag; }

    void Process(CommandLineIterator& iterator) const override
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

    std::optional<int64_t> seed;

    unique_ptr<IArgumentParser> parsers[] = {
        hatcher::make_unique<ArgumentParserOptionalInteger64>("--seed", seed),
        hatcher::make_unique<ArgumentParserOptionalString>("--save", this->commandSaveFile),
        hatcher::make_unique<ArgumentParserOptionalString>("--load", this->commandLoadFile),
    };
    const span<unique_ptr<IArgumentParser>> parsersSpan =
        span<unique_ptr<IArgumentParser>>(parsers, std::size(parsers));
    CommandLineIterator iterator(argc, argv);
    while (!iterator.Ended())
    {
        const std::string arg = *iterator;
        iterator++;
        auto it = std::find_if(parsersSpan.begin(), parsersSpan.end(),
                               [arg](const unique_ptr<IArgumentParser>& parser) { return parser->Matches(arg); });
        if (it == parsersSpan.end())
            throw std::invalid_argument("Unknown argument '" + arg + "'");
        (*it)->Process(iterator);
    }

    if (!seed)
    {
        seed = std::time(nullptr);
        std::cout << "Running with seed: " << *seed << std::endl;
    }
    this->seed = *seed;
}

} // namespace hatcher
