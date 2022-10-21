#include "nodefactories.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <LoggerCpp/LoggerCpp.h>
#include <cassert>
#include <sol/sol.hpp>

#include <resourceidentifiers.hpp>
#include <scene.hpp>
#include <shapenode.hpp>
#include <soundplayernode.hpp>
#include <spritenode.hpp>
#include <textnode.hpp>
#include <tilemapnode.hpp>
#include <ysortnode.hpp>
#include <utils.hpp>

#include <algorithm>
#include <memory>


Scene::Scene(sf::RenderTarget& outputTarget, FontCollection& fonts, TextureCollection& textures, const std::string& recipePath)
    : m_target(outputTarget)
    , m_sceneTexture()
    , m_worldView(outputTarget.getDefaultView())
    , m_textures(textures) 
    , m_fonts(fonts)
    , m_sounds()
    , m_text()
    , m_sceneGraph(SceneNode::Mask::none)
      , m_viewStartPosition()
{
    Log::Logger logger("Scene");
    m_sceneTexture.create(m_target.getSize().x, m_target.getSize().y);

    ordered_json sceneRecipe = utils::jsonLoadFromFile(recipePath);

    loadResources();
    buildScene(sceneRecipe);

    // Prepare the view
    m_worldView.setCenter(m_viewStartPosition);
}

void Scene::update(sf::Time dt)
{
    // Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
    /* while (!mCommandQueue.isEmpty()) */
    /*     mSceneGraph.onCommand(mCommandQueue.pop(), dt); */

    handleCollisions();

    m_sceneGraph.update(dt);
}

void Scene::draw()
{
    m_target.setView(m_worldView);
    m_target.draw(m_sceneGraph);
    m_target.draw(m_text);
}

bool Scene::hasReachedWinCondition() const
{
    return false;
}

void Scene::loadResources()
{
    m_fonts.load(FontID::Main, "media/fonts/MesloLGS NF Regular.ttf");
    m_textures.load(TextureID::Player, "media/textures/test.png");
    m_textures.load(TextureID::Background, "media/tilemaps/test2map/test2map.png");
    m_textures.load(TextureID::BackgroundRoom, "media/tilemaps/room/room.png");
    m_textures.load(TextureID::BackgroundIso, "media/tilemaps/isomap/isomap.png");
}

bool matchesMask(SceneNode::Pair& colliders, SceneNode::Mask mask1, SceneNode::Mask mask2)
{
    auto nodeMask1 = static_cast<unsigned int>(colliders.first->getMask());
    auto nodeMask2 = static_cast<unsigned int>(colliders.second->getMask());

    auto checkMask1 = static_cast<unsigned int>(mask1);
    auto checkMask2 = static_cast<unsigned int>(mask2);

    // Make sure first pair entry has category type1 and second has type2
    if (checkMask1 & nodeMask1 && checkMask2 & nodeMask2)
        return true;

    if (checkMask1 & nodeMask2 && checkMask2 & nodeMask1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }

    return false;
}

void Scene::handleCollisions()
{
}

void Scene::buildScene(const ordered_json& recipe)
{
    Log::Logger logger("Scene::buildScene");
    NodeFactories nf(m_textures, m_fonts, m_sounds);

    ordered_json scene = utils::jsonLoadFromFile("data/scenes/scene1.json");
    logger.info() << "Building scene start";

    for (auto& nodeRecipe : recipe)
    {
        logger.debug() << nodeRecipe.dump();
        m_sceneGraph.attachChild(nf.createNode(nodeRecipe));
    }

    logger.info() << "Finished building scene!";
}

sf::FloatRect Scene::getViewBounds() const
{
    return sf::FloatRect(m_worldView.getCenter() - m_worldView.getSize() / 2.f, m_worldView.getSize());
}

