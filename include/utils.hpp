#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <nlohmann/json.hpp>
#include <LoggerCpp/LoggerCpp.h>
#include <fstream>

#ifndef LOGGERLEVEL
#define LOGGERLEVEL eDebug
#endif

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

namespace utils
{
    inline const json jsonLoadFromFile(const std::string& jsonDataFilePath)
    {
        std::ifstream mapDataFile(jsonDataFilePath);
        if (!mapDataFile.is_open())
            throw std::runtime_error("Could not open file \"" + jsonDataFilePath + "\"");

        return json::parse(mapDataFile);
    }
}

class Checker
{
public:
    Checker(const ordered_json& _json)
        : json(_json)
    {
        logger.setLevel(Log::Log::LOGGERLEVEL);
    }

    bool fieldType(std::string_view field, json::value_t value)
    {
        logger.debug() << "checking field " << field;
        bool chk;
        if ((chk = json.contains(field)))
        {
            logger.debug() << "value: " << json[field].dump();
            if (json[field].type() == value)
                logger.debug() << "OK!";
            else
            {
                logger.error() << "Value is NOT the expected type!";
                throw std::logic_error("json is illformed");
            }
        }
        else
            logger.debug() << "field not found";
        return chk;
    }

    private:
        const ordered_json& json;
        Log::Logger logger{"JsonFieldChecker"};
};
#endif // TEST_UTILS_HPP
