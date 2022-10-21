#include <LoggerCpp/LoggerCpp.h>
#include <cassert>
#include <scenedata.hpp>
#include <nlohmann/json.hpp>
#include <utils.hpp>

using ordered_json = nlohmann::ordered_json;
using json = nlohmann::json;

typedef std::pair<const std::string, const std::string> ConstStringPair;


SceneData::SceneData(const std::string& gameSceneDataPath)
    : m_sceneDataMap()
{
    Log::Logger logger("SceneData::SceneData");

    const json sceneData = utils::jsonLoadFromFile(gameSceneDataPath);


    logger.debug() << sceneData["data"].get<json>();
    const json a = sceneData["data"].get<json>();

    logger.info() << "Scene register";

    for (const auto& scene : a)
    {
        logger.info() << "Registering scene: " << scene["scenename"];
        m_sceneDataMap.insert(ConstStringPair(scene["scenename"], scene["datapath"]));
        if (scene.contains("ismainscene") && scene["ismainscene"].get<bool>())
            m_mainSceneName = scene["scenename"].get<std::string>();
    }

    logger.info() << "Finished registering!";
    assert(m_mainSceneName != "invalid" && "SceneData file did not contain a main scene!");

    checkSceneDataIntegrity();
}

const std::string SceneData::getSceneDataPath(const std::string& sceneName)
{
    return m_sceneDataMap.at(sceneName);
}

const std::string SceneData::getMainSceneDataPath() const
{
    return m_sceneDataMap.at(m_mainSceneName);
}

void SceneData::checkSceneDataIntegrity()
{
    // open files, parse the contents and discard the result to
    // guarantee file and content vailidity at app lauch
    for (auto& element: m_sceneDataMap)
        utils::jsonLoadFromFile(element.second);
}
