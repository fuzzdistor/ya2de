#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <nlohmann/json.hpp> 
#include <fstream> 

using json = nlohmann::json;

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

#endif // TEST_UTILS_HPP
