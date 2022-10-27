#ifndef TEST_INIT_HPP
#define TEST_INIT_HPP

#define SOL_ALL_SAFETIES_ON 1

#include <LoggerCpp/LoggerCpp.h>
#include <sol/sol.hpp>
#include <cassert>

namespace Init
{
    static void loggerInit()
    {
#ifndef NDEBUG
        Log::Manager::setDefaultLevel(Log::Log::eDebug);
#else
        Log::Manager::setDefaultLevel(Log::Log::eNotice);
#endif
        Log::Config::Vector configList;
        Log::Config::addOutput(configList, "OutputConsole");
        Log::Config::addOutput(configList, "OutputFile");
        Log::Config::setOption(configList, "filename",          "log.txt");
        Log::Config::setOption(configList, "filename_old",      "log.old.txt");
        Log::Config::setOption(configList, "max_startup_size",  "0");
        Log::Config::setOption(configList, "max_size",          "100000");
        try
        {
            Log::Manager::configure(configList);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what();
        }
    }
}
 
#endif // TEST_INIT_HPP
