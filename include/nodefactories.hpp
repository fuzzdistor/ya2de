#ifndef TEST_NODEFACTORIES_HPP
#define TEST_NODEFACTORIES_HPP

#include "spritenode.hpp"
#include <cassert>
#include <nlohmann/json.hpp>

#include <scenenode.hpp>
#include <resourceidentifiers.hpp>

#include <map>
#include <functional>

using ordered_json = nlohmann::ordered_json;

class NodeFactories
{
public:
    typedef std::function<SceneNode::UniPtr(const ordered_json&)> FactoryFunction;
    typedef std::function<void(SceneNode*, const ordered_json&)> SetterFunction;

    enum class NodeID
    {
        SceneNode,
        SpriteNode,
        TileMapNode,
        ShapeNode,
        YSortNode,
        TextNode,
        SoundPlayerNode,
    };
    // nlohmann json specializations for NodeID enum
    NLOHMANN_JSON_SERIALIZE_ENUM(NodeID,
    {
        { NodeID::SceneNode, "SceneNode" },
        { NodeID::SpriteNode, "SpriteNode" },
        { NodeID::TileMapNode, "TileMapNode" },
        { NodeID::ShapeNode, "ShapeNode" },
        { NodeID::YSortNode, "YSortNode" },
        { NodeID::TextNode, "TextNode" },
        { NodeID::SoundPlayerNode, "SoundPlayerNode" },
    })

    NodeFactories(TextureCollection& textures, FontCollection& fonts, SoundBufferCollection& sounds);
    SceneNode::UniPtr createNode(const ordered_json& recipe) const;


private:
    std::map<NodeID, FactoryFunction> mNodeConstructors;
    std::map<NodeID, SetterFunction> mNodeSetters;
};

#endif // TEST_NODEFACTORIES_HPP
