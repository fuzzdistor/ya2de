#ifndef TEST_SCENE_HPP
#define TEST_SCENE_HPP

#include "nodefactories.hpp"
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
    Scene(sf::RenderTarget& outputTarget, FontCollection& fonts, TextureCollection& textures, const std::string& recipePath);
    void update(sf::Time dt);
    void draw();

    sf::FloatRect getViewBounds() const;		
    //CommandQueue& getCommandQueue();
    void setCurrentBattleFieldPosition(float lineY);
    void setWorldHeight(float height);

    bool hasReachedWinCondition() const;

private:
    void loadResources();
    void handleCollisions();
    void updateSounds();

    void buildScene(const ordered_json& recipe);
    void destroyEntitiesOutsideView();

    sf::RenderTarget& m_target;
    sf::RenderTexture m_sceneTexture;
    sf::View m_worldView;
    TextureCollection& m_textures;
    FontCollection& m_fonts;
    SoundBufferCollection m_sounds;
    sf::Text m_text;
    //SoundPlayer& m_sounds;

    SceneNode m_sceneGraph;
    sf::Vector2f m_viewStartPosition;
    //CommandQueue m_commandQueue;
};

#endif // TEST_SCENE_HPP
