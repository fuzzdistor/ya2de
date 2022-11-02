#ifndef YA2DE_SCENEDATA_HPP
#define YA2DE_SCENEDATA_HPP

#include <string>
#include <map>

class SceneData
{
public:
    SceneData(const std::string& gameSceneDataPath);

    const std::string getSceneDataPath(const std::string& sceneName);
    const std::string getMainSceneDataPath() const;

private:
    void checkSceneDataIntegrity();

    std::map<const std::string, const std::string> m_sceneDataMap;
    std::string m_mainSceneName {"invalid"};
};

#endif // YA2DE_SCENEDATA_HPP

