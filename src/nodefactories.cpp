#include "textboxnode.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <LoggerCpp/LoggerCpp.h>
#include <algorithm>
#include <nlohmann/json.hpp>

#include <resourcepack.hpp>
#include <nodefactories.hpp>
#include <resourceidentifiers.hpp>
#include <resourcecollection.hpp>
#include <utils.hpp>
#include <areaswitchnode.hpp>
#include <scenenode.hpp>
#include <soundplayernode.hpp>
#include <spritenode.hpp>
#include <shapenode.hpp>
#include <textnode.hpp>
#include <tilemapnode.hpp>
#include <triggernode.hpp>
#include <ysortnode.hpp>

#include <memory>
#include <stdexcept>
#include <string_view>


NodeFactories::NodeFactories(const ResourcePack& resources)
    : m_nodeSetters()
    , m_nodeComposition()
{
    m_nodeComposition[NodeID::AreaSwitchNode] = { NodeID::AreaSwitchNode, NodeID::SceneNode };
    m_nodeComposition[NodeID::ShapeNode] = { NodeID::ShapeNode, NodeID::SceneNode };
    m_nodeComposition[NodeID::SpriteNode] = { NodeID::SpriteNode, NodeID::SceneNode };
    m_nodeComposition[NodeID::TextNode] = { NodeID::TextNode, NodeID::SceneNode };
    m_nodeComposition[NodeID::SoundPlayerNode] = { NodeID::SoundPlayerNode, NodeID::SceneNode };
    m_nodeComposition[NodeID::TriggerNode] = { NodeID::TriggerNode, NodeID::SceneNode };
    m_nodeComposition[NodeID::TileMapNode] = { NodeID::TileMapNode, NodeID::SceneNode };
    m_nodeComposition[NodeID::YSortNode] = { NodeID::YSortNode, NodeID::SceneNode };
    m_nodeComposition[NodeID::TextboxNode] = { NodeID::TextNode, NodeID::TextboxNode, NodeID::SceneNode };
    m_nodeComposition[NodeID::SceneNode] = { NodeID::SceneNode };

    //////////////
    // SETTERS
    //////////////
    m_nodeSetters[NodeID::SpriteNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "SpriteNode Settings";
        auto recipeNode = static_cast<SpriteNode*>(node);

        Checker chk(recipe);

        if(chk.fieldType("textureid", json::value_t::string))
            recipeNode->setTexture(resources.textures.get(recipe["textureid"].get<TextureID>()));
    };
    m_nodeSetters[NodeID::TileMapNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "TileMapNode Settings";
        auto recipeNode = static_cast<TileMapNode*>(node);

        Checker chk(recipe);

        if(chk.fieldType("data_filepath", json::value_t::string))
            recipeNode->setMapInfo(recipe["data_filepath"].get_ref<const std::string&>());

        if(chk.fieldType("tilesetid", json::value_t::string))
            recipeNode->setTileSet(resources.tilesets.get(recipe["tilesetid"].get<TileSetID>()));

        if(chk.fieldType("tile_scale", json::value_t::number_float))
            recipeNode->setTileScale(recipe["tile_scale"].get<float>());

        if(chk.fieldType("tile_size", json::value_t::number_float))
            recipeNode->setTileSize(recipe["tile_size"].get<float>());
    };
    m_nodeSetters[NodeID::ShapeNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "ShapeNode Settings";
        auto recipeNode = static_cast<ShapeNode*>(node);

        Checker chk(recipe);

        if(chk.fieldType("size", json::value_t::array))
            recipeNode->setSize(recipe["size"][0].get<float>(), recipe["size"][1].get<float>());

        if(chk.fieldType("fill_color", json::value_t::array))
            recipeNode->setFillColor(sf::Color(recipe["fill_color"][0], recipe["fill_color"][1], recipe["fill_color"][2], recipe["fill_color"][3]));

        if(chk.fieldType("enabled", json::value_t::boolean))
            recipeNode->m_enabled = recipe["enabled"].get<bool>();
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
            recipeNode->m_text.setFont(resources.fonts.get(recipe["fontid"].get<FontID>()));

        if(chk.fieldType("character_size", json::value_t::number_unsigned))
            recipeNode->m_text.setCharacterSize(recipe["character_size"].get<unsigned int>());

        if(chk.fieldType("outline_thickness", json::value_t::number_float))
            recipeNode->m_text.setOutlineThickness(recipe["outline_thickness"].get<float>());

        if(chk.fieldType("string", json::value_t::string))
            recipeNode->m_text.setString(recipe["string"].get_ref<const std::string&>());

        if(chk.fieldType("fill_color", json::value_t::array))
            recipeNode->m_text.setFillColor(sf::Color(recipe["fill_color"][0], recipe["fill_color"][1], recipe["fill_color"][2], recipe["fill_color"][3]));

        if(chk.fieldType("outline_color", json::value_t::array))
            recipeNode->m_text.setOutlineColor(sf::Color(recipe["outline_color"][0], recipe["outline_color"][1], recipe["outline_color"][2], recipe["outline_color"][3]));
    };
    m_nodeSetters[NodeID::TextboxNode] = [&](SceneNode* node, const ordered_json& recipe) -> void
    {
        m_logger.debug() << "TextboxNode Settings";
        auto recipeNode = static_cast<TextboxNode*>(node);

        Checker chk(recipe);

        if (chk.fieldType("box_width", json::value_t::number_float))
            recipeNode->m_textboxWidth = recipe["box_width"].get<float>();

        if (chk.fieldType("dialogue", json::value_t::array))
            recipeNode->m_dialogueLines = recipe["dialogue"].get<std::vector<std::string>>();

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
            recipeNode->m_destinyArea = recipe["destiny"].get_ref<const std::string&>();
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
            node->m_debugFlag = recipe["debug"];

        if (chk.fieldType("mask", json::value_t::string))
            node->m_mask = recipe["mask"];

        if(chk.fieldType("visible", json::value_t::boolean))
            node->m_visible = recipe["visible"].get<bool>();

        if (chk.fieldType("script", json::value_t::string))
            node->loadScriptFile(recipe["script"].get_ref<const std::string&>());

        if (chk.fieldType("logger_name", json::value_t::string))
            node->m_logger = Log::Logger{recipe["logger_name"].get_ref<const std::string&>().c_str()};
    };
}

std::unique_ptr<SceneNode> NodeFactories::createSceneGraph(const ordered_json& sceneRecipe) const
{
    auto sceneGraph = std::make_unique<SceneNode>("SceneGraph");
    for (auto& nodeRecipe : sceneRecipe)
    {
        m_logger.debug() << nodeRecipe.dump();
        sceneGraph->attachChild(createNode(nodeRecipe));
    }

    return sceneGraph;
}


SceneNode::UniPtr NodeFactories::createNode(const ordered_json& recipe) const
{
    m_logger.debug() << "Looking for constructor of type " << recipe["NodeType"];
    SceneNode::UniPtr node = nodeConstructor(recipe);

    // if it has children create them recursively and attach them
    if (recipe.contains("children") && recipe["children"].is_array())
    {
        for (const auto& childRecipe : recipe["children"])
        {
            node->attachChild(createNode(childRecipe));
        }
    }

    return node;
}

SceneNode::UniPtr NodeFactories::nodeConstructor(const ordered_json& recipe) const
{
    NodeID id = recipe["NodeType"];

    m_logger.debug() << "Constructing Node";

    auto node = [&]() -> SceneNode::UniPtr
    {
        switch (id)
        {
            case NodeID::SpriteNode:
                return makeUniqueNode<SpriteNode>();
                break;
            case NodeID::TileMapNode:
                return makeUniqueNode<TileMapNode>();
                break;
            case NodeID::ShapeNode:
                return makeUniqueNode<ShapeNode>();
                break;
            case NodeID::YSortNode:
                return makeUniqueNode<YSortNode>();
                break;
            case NodeID::TextNode:
                return makeUniqueNode<TextNode>();
                break;
            case NodeID::SoundPlayerNode:
                return makeUniqueNode<SoundPlayerNode>();
                break;
            case NodeID::TriggerNode:
                return makeUniqueNode<TriggerNode>();
                break;
            case NodeID::AreaSwitchNode:
                return makeUniqueNode<AreaSwitchNode>();
                break;
            case NodeID::TextboxNode:
                return makeUniqueNode<TextboxNode>();
                break;
            case NodeID::SceneNode:
                return makeUniqueNode<SceneNode>();
                break;
                break;
            case NodeID::Invalid:
                m_logger.error() << "Invalid node name passed!";
                throw std::logic_error("Node id is not a valid node!");
                break;
        }
        throw std::logic_error("How did you even get here?");
        return nullptr;
    }();

    for (auto component : m_nodeComposition.at(id))
    {
        m_logger.debug() << "Configuring " << recipe["NodeType"].get<std::string_view>();
        m_nodeSetters.at(component)(node.get(), recipe);
    }

    // setting the variable name for the lua usertype

    return node;
}

