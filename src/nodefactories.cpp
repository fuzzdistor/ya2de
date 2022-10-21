#include "LoggerCpp/Logger.h"
#include "resourceidentifiers.hpp"
#include "resourcecollection.hpp"
#include "scenenode.hpp"
#include "soundplayernode.hpp"
#include "spritenode.hpp"
#include "shapenode.hpp"
#include "textnode.hpp"
#include "tilemapnode.hpp"
#include "triggernode.hpp"
#include "ysortnode.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <nodefactories.hpp>

NodeFactories::NodeFactories(TextureCollection& textures, FontCollection& fonts, SoundBufferCollection& sounds)
    : mNodeConstructors()
    , mNodeSetters()
{
    ////////////////
    // CONSTRUCTORS
    ////////////////
    mNodeConstructors[NodeID::SpriteNode] = [&](const ordered_json& recipe) -> SceneNode::UniPtr
    {
        Log::Logger logger("Factory::SpriteNodeConstructor");

        logger.info() << "Creating SpriteNode";
        auto sn = recipe.contains("textureid")?
            std::make_unique<SpriteNode>(textures.get(recipe["textureid"].get<TextureID>())) 
            : std::make_unique<SpriteNode>();

        logger.info() << "Configuring SceneNode";
        mNodeSetters[NodeID::SceneNode](sn.get(), recipe);

        logger.info() << "Configuring SpriteNode";
        mNodeSetters[NodeID::SpriteNode](sn.get(), recipe);

        return sn;
    };

    mNodeConstructors[NodeID::TriggerNode] = [&](const ordered_json& recipe) -> SceneNode::UniPtr
    {
        Log::Logger logger("Factory::TriggerNodeConstructor");

        logger.info() << "Creating TriggerNode";
        auto recipeNode = std::make_unique<TriggerNode>();

        logger.info() << "Configuring SceneNode";
        mNodeSetters[NodeID::SceneNode](recipeNode.get(), recipe);

        logger.info() << "Configuring TriggerNode";
        mNodeSetters[NodeID::TriggerNode](recipeNode.get(), recipe);

        return recipeNode;
    };

    mNodeConstructors[NodeID::TileMapNode] = [&](const ordered_json& recipe) -> SceneNode::UniPtr
    {
        Log::Logger logger("Factory::TileMapNodeConstructor");

        logger.info() << "Creating TileMapNode";
        auto tmn = std::make_unique<TileMapNode>(
                recipe["datafilepath"].get_ref<const std::string&>()
                , textures.get(recipe["textureid"].get<TextureID>()));

        logger.info() << "Configuring SceneNode";
        mNodeSetters[NodeID::SceneNode](tmn.get(), recipe);

        logger.info() << "Configuring TileMapNode";
        mNodeSetters[NodeID::TileMapNode](tmn.get(), recipe);

        return tmn;
    };

    mNodeConstructors[NodeID::ShapeNode] = [&](const ordered_json& recipe) -> SceneNode::UniPtr
    {
        Log::Logger logger("Factory::ShapeNodeConstructor");

        logger.info() << "Creating ShapeNode";
        auto sn = std::make_unique<ShapeNode>(
                recipe["shape"].get<ShapeNode::Shapes>());

        logger.info() << "Configuring SceneNode";
        mNodeSetters[NodeID::SceneNode](sn.get(), recipe);

        logger.info() << "Configuring ShapeNode";
        mNodeSetters[NodeID::ShapeNode](sn.get(), recipe);

        return sn;
    };
    mNodeConstructors[NodeID::YSortNode] = [&](const ordered_json& recipe) -> SceneNode::UniPtr
    {
        Log::Logger logger("Factory::YSortNode");

        logger.info() << "Creating YSortNode";
        auto ysn = std::make_unique<YSortNode>();

        logger.info() << "Configuring SceneNode";
        mNodeSetters[NodeID::SceneNode](ysn.get(), recipe);

        logger.info() << "Configuring YSortNode";
        mNodeSetters[NodeID::YSortNode](ysn.get(), recipe);

        return ysn;
    };
    mNodeConstructors[NodeID::TextNode] = [&](const ordered_json& recipe) -> SceneNode::UniPtr
    {
        Log::Logger logger("Factory::TextNode");

        logger.info() << "Creating TextNode";
        auto tn = std::make_unique<TextNode>();

        logger.info() << "Configuring SceneNode";
        mNodeSetters[NodeID::SceneNode](tn.get(), recipe);

        logger.info() << "Configuring TextNode";
        mNodeSetters[NodeID::TextNode](tn.get(), recipe);

        return tn;
    };
    mNodeConstructors[NodeID::SoundPlayerNode] = [&](const ordered_json& recipe) -> SceneNode::UniPtr
    {
        Log::Logger logger("Factory::SoundPlayerNode");

        logger.info() << "Creating SoundPlayerNode";
        auto spn = std::make_unique<SoundPlayerNode>();

        logger.info() << "Configuring SceneNode";
        mNodeSetters[NodeID::SceneNode](spn.get(), recipe);

        logger.info() << "Configuring SoundPlayerNode";
        mNodeSetters[NodeID::SoundPlayerNode](spn.get(), recipe);

        return spn;
    };

    //////////////
    // SETTERS
    //////////////
    mNodeSetters[NodeID::SpriteNode] = [&](SceneNode*, const ordered_json&) -> void
    {
    };
    mNodeSetters[NodeID::TileMapNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        Log::Logger logger("Factory::TileMapSetter");
        auto tmn = dynamic_cast<TileMapNode*>(node);

        // check if the dynamic_cast succeded
        assert(tmn != nullptr);

        logger.info() << "Checking tilescale";
        if(recipe.contains("tilescale"))
            tmn->setTileScale(recipe["tilescale"].get<float>());

        logger.info() << "Checking tilesize";
        if(recipe.contains("tilesize"))
            tmn->setTileSize(recipe["tilesize"].get<float>());
    };
    mNodeSetters[NodeID::ShapeNode] = [&](SceneNode*, const ordered_json&) -> void
    {
    };
    mNodeSetters[NodeID::YSortNode] = [&](SceneNode*, const ordered_json&) -> void
    {
    };
    mNodeSetters[NodeID::TextNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        Log::Logger logger("Factory::TextNodeSetter");
        auto tmn = dynamic_cast<TextNode*>(node);

        // check if the dynamic_cast succeded
        assert(tmn != nullptr);

        logger.info() << "Checking fontid";
        if(recipe.contains("fontid"))
            tmn->setFont(fonts.get(recipe["fontid"].get<FontID>()));

        logger.info() << "Checking charactersize";
        if(recipe.contains("charactersize"))
            tmn->setCharacterSize(recipe["charactersize"].get<unsigned int>());

        logger.info() << "Checking outlinethickness";
        if(recipe.contains("outlinethickness"))
            tmn->setOutlineThickness(recipe["outlinethickness"].get<float>());

        logger.info() << "Checking fillcolor";
        if(recipe.contains("fillcolor"))
            tmn->setFillColor(sf::Color(recipe["fillcolor"].get<unsigned int>()));

        logger.info() << "Checking outlinecolor";
        if(recipe.contains("outlinecolor"))
            tmn->setOutlineColor(sf::Color(recipe["outlinecolor"].get<unsigned int>()));
    };
    mNodeSetters[NodeID::SoundPlayerNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        Log::Logger logger("Factory::SoundPlayerSetter");
        auto spc = dynamic_cast<SoundPlayerNode*>(node);

        // check if the dynamic_cast succeded
        assert(spc != nullptr);

        logger.info() << "Checking filepath";
        if (recipe.contains("filepath"))
            spc->setSoundSource(recipe["filepath"].get_ref<const std::string&>());

        logger.info() << "Checking loop";
        if (recipe.contains("loop"))
            spc->setLoop(recipe["loop"].get<bool>());

        logger.info() << "Checking autoplay";
        if (recipe.contains("autoplay") && recipe["autoplay"].get<bool>())
            spc->play();
    };
    mNodeSetters[NodeID::TriggerNode] = [&](SceneNode*, const ordered_json&) -> void
    {
    };
    mNodeSetters[NodeID::SceneNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        Log::Logger logger("Factory::SceneNodeSetter");
        logger.info() << "Checking position";
        if (recipe.contains("position"))
        {
            logger.info() << "Setting position to " << recipe["position"][0].get<float>() << " " << recipe["position"][1].get<float>();
            node->setPosition(recipe["position"][0].get<float>(), recipe["position"][1].get<float>());
        }

        logger.info() << "Checking scale";
        if (recipe.contains("scale"))
            node->setScale(recipe["scale"][0].get<float>(), recipe["scale"][1].get<float>());

        logger.info() << "Checking rotation";
        if (recipe.contains("rotation"))
            node->setRotation(recipe["rotation"].get<float>());

        logger.info() << "Checking origin";
        if (recipe.contains("origin"))
            node->setOrigin(recipe["origin"][0].get<float>(), recipe["origin"][1].get<float>());

        logger.info() << "Checking debug";
        if (recipe.contains("debug"))
            node->setDebugInfo(recipe["debug"].get<bool>());

        logger.info() << "Checking script";
        if (recipe.contains("script"))
            node->loadScriptFile(recipe["script"].get_ref<const std::string&>());
    };
}

SceneNode::UniPtr NodeFactories::createNode(const ordered_json& recipe) const
{
    Log::Logger logger("NodeFactories::createNode");
    logger.info() << "Looking for constructor of type " << recipe["NodeType"];
    auto factoryIt = mNodeConstructors.find(recipe["NodeType"].get<NodeID>());
    assert(factoryIt != mNodeConstructors.end());

    logger.info() << "Creating Node of type " << recipe["NodeType"];
    SceneNode::UniPtr node = factoryIt->second(recipe);

    // if it has children create them recursively and attach them
    if (recipe.contains("children") && recipe["children"].is_array())
    {
        for (const auto& child : recipe["children"])
        {
            node->attachChild(createNode(child));
        }
    }

    // not using std::move for local object preserves copy-elision
    return node;
}
