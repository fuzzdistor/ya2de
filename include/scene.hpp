#ifndef TEST_SCENE_HPP
#define TEST_SCENE_HPP

#include <LoggerCpp/Logger.h>
#include <resourcepack.hpp>
#include <nodefactories.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>

#include <resourcecollection.hpp>
#include <resourceidentifiers.hpp>
#include <scenenode.hpp>


class Scene : private sf::NonCopyable
{
public:
    Scene(sf::RenderTarget& outputTarget, ResourcePack& resources, const std::string& recipePath);
    void update(sf::Time dt);
    void draw();

    sf::FloatRect getViewBounds() const;
    //CommandQueue& getCommandQueue();
    void setCurrentBattleFieldPosition(float lineY);
    void setWorldHeight(float height);
    bool requestsSceneChange() const;
    const std::string& getRequestedScene() const;

private:
    bool matchesMask(SceneNode::Pair& colliders, SceneNode::Mask mask1, SceneNode::Mask mask2);
    void loadResources();
    void handleCollisions(const sf::Time& dt);
    void updateSounds();

    void buildScene(const ordered_json& recipe);
    void destroyEntitiesOutsideView();

    sf::RenderTarget& m_target;
    sf::RenderTexture m_sceneTexture;
    sf::View m_worldView;
    ResourcePack& m_resources;
    std::string m_toArea{};
    //SoundPlayer& m_sounds;

    SceneNode::UniPtr m_sceneGraph;
    sf::Vector2f m_viewStartPosition;
    //CommandQueue m_commandQueue;

    Log::Logger m_logger{ "Scene" };
};

#endif // TEST_SCENE_HPP
