#include "LoggerCpp/LoggerCpp.h"
#include "areaswitchnode.hpp"
#include "nlohmann/json.hpp"
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
#include "utils.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <nodefactories.hpp>
#include <stdexcept>
#include <string_view>


NodeFactories::NodeFactories(TextureCollection& textures, FontCollection& fonts, SoundBufferCollection& sounds, TileSetCollection& tilesets)
    : m_nodeSetters()
{
    //////////////
    // SETTERS
    //////////////
    m_nodeSetters[NodeID::SpriteNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "SpriteNode Settings";
        auto recipeNode = static_cast<SpriteNode*>(node);

        Checker chk(recipe);

        if(chk.fieldType("textureid", json::value_t::string))
            recipeNode->setTexture(textures.get(recipe["textureid"].get<TextureID>()));
    };
    m_nodeSetters[NodeID::TileMapNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "TileMapNode Settings";
        auto recipeNode = static_cast<TileMapNode*>(node);

        Checker chk(recipe);

        if(chk.fieldType("datafilepath", json::value_t::string))
            recipeNode->setMapInfo(recipe["datafilepath"].get_ref<const std::string&>());

        if(chk.fieldType("tilesetid", json::value_t::string))
            recipeNode->setTileSet(tilesets.get(recipe["tilesetid"].get<TileSetID>()));

        if(chk.fieldType("tilescale", json::value_t::number_float))
            recipeNode->setTileScale(recipe["tilescale"].get<float>());

        if(chk.fieldType("tilesize", json::value_t::number_float))
            recipeNode->setTileSize(recipe["tilesize"].get<float>());
    };
    m_nodeSetters[NodeID::ShapeNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "ShapeNode Settings";
        auto recipeNode = static_cast<ShapeNode*>(node);

        Checker chk(recipe);

        if(chk.fieldType("size", json::value_t::array))
            recipeNode->setSize(recipe["size"][0].get<float>(), recipe["size"][1].get<float>());

        if(chk.fieldType("fillcolor", json::value_t::array))
            recipeNode->setFillColor(sf::Color(recipe["fillcolor"][0], recipe["fillcolor"][1], recipe["fillcolor"][2], recipe["fillcolor"][3])); 

        if(chk.fieldType("disable", json::value_t::boolean))
            recipeNode->disable(recipe["disable"].get<bool>());

        if(chk.fieldType("visible", json::value_t::boolean))
            recipeNode->setVisible(recipe["visible"].get<bool>());
    };
    m_nodeSetters[NodeID::YSortNode] = [&](SceneNode*, const ordered_json&) -> void
    {
        m_logger.debug() << "YSortNode Settings";
    };
    m_nodeSetters[NodeID::TextNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "TextNode Settings";
        auto recipeNode = static_cast<TextNode*>(node);

        Checker chk(recipe);

        if(chk.fieldType("fontid", json::value_t::string))
            recipeNode->setFont(fonts.get(recipe["fontid"].get<FontID>()));

        if(chk.fieldType("characte_rsize", json::value_t::number_unsigned))
            recipeNode->setCharacterSize(recipe["character_size"].get<unsigned int>());

        if(chk.fieldType("outlinethickness", json::value_t::number_float))
            recipeNode->setOutlineThickness(recipe["outlinethickness"].get<float>());

        if(chk.fieldType("fillcolor", json::value_t::array))
            recipeNode->setFillColor(sf::Color(recipe["fillcolor"][0], recipe["fillcolor"][1], recipe["fillcolor"][2], recipe["fillcolor"][3])); 

        if(chk.fieldType("outlinecolor", json::value_t::number_unsigned))
            recipeNode->setOutlineColor(sf::Color(recipe["outlinecolor"].get<unsigned int>()));
    };
    m_nodeSetters[NodeID::SoundPlayerNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "SoundPlayer Settings";
        auto recipeNode = static_cast<SoundPlayerNode*>(node);

        Checker chk(recipe);

        if (chk.fieldType("filepath", json::value_t::string))
            recipeNode->setSoundSource(recipe["filepath"].get_ref<const std::string&>());

        if (chk.fieldType("loop", json::value_t::boolean))
            recipeNode->setLoop(recipe["loop"].get<bool>());

        if (chk.fieldType("autoplay", json::value_t::boolean) && recipe["autoplay"].get<bool>())
            recipeNode->play();
    };
    m_nodeSetters[NodeID::AreaSwitchNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "AreaSwitchNode Settings";
        auto recipeNode = static_cast<AreaSwitchNode*>(node);

        Checker chk(recipe);

        if (chk.fieldType("destiny", json::value_t::string))
            recipeNode->setDestinyArea(recipe["destiny"].get_ref<const std::string&>());
    };
    m_nodeSetters[NodeID::TriggerNode] = [&](SceneNode*, const ordered_json&) -> void
    {
    };
    m_nodeSetters[NodeID::SceneNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "SceneNode Settings";

        Checker chk(recipe);

        if (chk.fieldType("position", json::value_t::array))
            node->setPosition(recipe["position"][0].get<float>(), recipe["position"][1].get<float>());

        if (chk.fieldType("scale", json::value_t::array))
            node->setScale(recipe["scale"][0].get<float>(), recipe["scale"][1].get<float>());

        if (chk.fieldType("rotation", json::value_t::number_float))
            node->setRotation(recipe["rotation"].get<float>());

        if (chk.fieldType("origin", json::value_t::array))
            node->setOrigin(recipe["origin"][0].get<float>(), recipe["origin"][1].get<float>());

        if (chk.fieldType("debug", json::value_t::boolean))
            node->setDebugInfo(recipe["debug"].get<bool>());

        if (chk.fieldType("mask", json::value_t::string))
            node->setMask(recipe["mask"].get<SceneNode::Mask>());

        if (chk.fieldType("script", json::value_t::string))
            node->loadScriptFile(recipe["script"].get_ref<const std::string&>());
    };
}

SceneNode::UniPtr NodeFactories::createNode(const ordered_json& recipe) const
{
    m_logger.debug() << "Creating Node";
    m_logger.debug() << "Looking for constructor of type " << recipe["NodeType"];
    SceneNode::UniPtr node = nodeConstructor(recipe);

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

SceneNode::UniPtr NodeFactories::nodeConstructor(const ordered_json& recipe) const
{
    NodeID id = recipe["NodeType"];

    m_logger.debug() << "NodeConstructor";

    m_logger.debug() << "Creating Node";

    SceneNode::UniPtr node;
    switch (id)
    {
        case NodeID::SpriteNode:
            node = std::make_unique<SpriteNode>();
            break;
        case NodeID::TileMapNode:
            node = std::make_unique<TileMapNode>();
            break;
        case NodeID::ShapeNode:
            node = std::make_unique<ShapeNode>();
            break;
        case NodeID::YSortNode:
            node = std::make_unique<YSortNode>();
            break;
        case NodeID::TextNode:
            node = std::make_unique<TextNode>();
            break;
        case NodeID::SoundPlayerNode:
            node = std::make_unique<SoundPlayerNode>();
            break;
        case NodeID::TriggerNode:
            node = std::make_unique<TriggerNode>();
            break;
        case NodeID::AreaSwitchNode:
            node = std::make_unique<AreaSwitchNode>();
            break;
        case NodeID::SceneNode:
            m_logger.error() << "SceneNode is not a valid node!";
            throw std::logic_error("SceneNode is not a valid node!");
            break;
        case NodeID::Invalid:
            m_logger.error() << "Invalid node name passed!";
            throw std::logic_error("Node id is not a valid node!");
            break;
    }

    m_logger.debug() << "Configuring SceneNode";
    m_nodeSetters.at(NodeID::SceneNode)(node.get(), recipe);

    m_logger.debug() << "Configuring " << recipe["NodeType"].get<std::string_view>();
    m_nodeSetters.at(id)(node.get(), recipe);

    return node;
}

