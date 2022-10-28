#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <LoggerCpp/LoggerCpp.h>
#include <sol/sol.hpp>

#include <resourceidentifiers.hpp>
#include <scene.hpp>
#include <shapenode.hpp>
#include <soundplayernode.hpp>
#include <spritenode.hpp>
#include <textnode.hpp>
#include <tilemapnode.hpp>
#include <resourcepack.hpp>
#include <areaswitchnode.hpp>
#include <nodefactories.hpp>
#include <scenenode.hpp>
#include <triggernode.hpp>
#include <ysortnode.hpp>
#include <utils.hpp>

#include <algorithm>
#include <cassert>
#include <memory>


Scene::Scene(sf::RenderTarget& outputTarget, ResourcePack& resources, const std::string& recipePath)
    : m_target(outputTarget)
    , m_sceneTexture()
    , m_worldView(outputTarget.getDefaultView())
    , m_resources(resources)
    , m_sceneGraph(std::make_unique<SceneNode>(SceneNode::Mask::none))
    , m_viewStartPosition()
{
    m_logger.debug() << "Constructing Scene Object";
    m_sceneTexture.create(m_target.getSize().x, m_target.getSize().y);

    m_logger.debug() << "Going for the parse";
    ordered_json sceneRecipe = utils::jsonLoadFromFile(recipePath);
    m_logger.debug() << "Parse completed!";

    loadResources();
    buildScene(sceneRecipe);
    
    // call the init of every lua script in the scene
    m_sceneGraph->init();

    // Prepare the view
    m_worldView.setCenter(m_viewStartPosition);
}

void Scene::update(sf::Time dt)
{
    // Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
    /* while (!mCommandQueue.isEmpty()) */
    /*     mSceneGraph.onCommand(mCommandQueue.pop(), dt); */

    m_sceneGraph->update(dt);

    handleCollisions(dt);
    m_sceneGraph->removeMarkedChildren();
}

void Scene::draw()
{
    m_target.setView(m_worldView);
    m_target.draw(*m_sceneGraph.get());
}

bool Scene::requestsSceneChange() const
{
    return !m_toArea.empty();
}

const std::string& Scene::getRequestedScene() const
{
    return m_toArea;
}

void Scene::loadResources()
{
    m_resources.fonts.load(FontID::Main, "media/fonts/MesloLGS NF Regular.ttf");
    m_resources.textures.load(TextureID::Player, "media/textures/test.png");
    m_resources.tilesets.load(TileSetID::Room, "media/tilemaps/room/room.txt");
    m_resources.tilesets.load(TileSetID::Iso, "media/tilemaps/isomap/isomap.txt");
}

void Scene::handleCollisions(const sf::Time& dt)
{
    std::set<SceneNode::Pair> collisionPairs;
    m_sceneGraph->checkSceneCollision(*m_sceneGraph.get(), collisionPairs);

    for (auto pair : collisionPairs)
    {
        if (matchesMask(pair, SceneNode::Mask::Player, SceneNode::Mask::Trigger))
        {
            auto player = static_cast<SpriteNode*>(pair.first);
            auto trigger = static_cast<TriggerNode*>(pair.second);
            trigger->applyCallbackTo(player, dt);
        }
        if (matchesMask(pair, SceneNode::Mask::Player, SceneNode::Mask::AreaSwitch))
        {
            auto areaswitchernode = pair.second;
            m_toArea = (*areaswitchernode->getLuaState())["node"]["destiny_area"];
        }
    }
}

bool Scene::matchesMask(SceneNode::Pair& colliders, SceneNode::Mask mask1, SceneNode::Mask mask2)
{
    auto nodeMask1 = static_cast<unsigned int>(colliders.first->getMask());
    auto nodeMask2 = static_cast<unsigned int>(colliders.second->getMask());

    auto checkMask1 = static_cast<unsigned int>(mask1);
    auto checkMask2 = static_cast<unsigned int>(mask2);

    // Make sure first pair entry has category type1 and second has type2
    if (checkMask1 & nodeMask1 && checkMask2 & nodeMask2)
    {
        return true;
    }

    if (checkMask1 & nodeMask2 && checkMask2 & nodeMask1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }

    return false;
}

void Scene::buildScene(const ordered_json& recipe)
{
    m_logger.info() << "Building scene start";

    // Clock for timing scene building.
    // NOTE: Scene building is *SEVERELY* impacted by the console writing
    // of debug messages. Different terminal emulators might yield different
    // build times and should not be compared between one another.
    // Ex: running on Windows git bash reported scene build times 10x faster
    // than cmd, which took up to 10 frames to finish the build. Maybe an
    // asynchronous log system might solve this, but "asynchronous log"
    // seems like a bad idea.
    sf::Clock sceneBuildTimer;
    sceneBuildTimer.restart();

    NodeFactories nf(m_resources);
    m_sceneGraph = nf.createSceneGraph(recipe);

    m_logger.info() << "Finished building scene in " << sceneBuildTimer.restart().asMicroseconds() << "us";
}

sf::FloatRect Scene::getViewBounds() const
{
    return sf::FloatRect(m_worldView.getCenter() - m_worldView.getSize() / 2.f, m_worldView.getSize());
}

