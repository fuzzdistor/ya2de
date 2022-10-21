#ifndef TEST_NODEFACTORIES_HPP
#define TEST_NODEFACTORIES_HPP

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
        TriggerNode,
    };

    NodeFactories(TextureCollection& textures, FontCollection& fonts, SoundBufferCollection& sounds);
    SceneNode::UniPtr createNode(const ordered_json& recipe) const;


private:
    std::map<NodeID, FactoryFunction> mNodeConstructors;
    std::map<NodeID, SetterFunction> mNodeSetters;
};
    // nlohmann json specializations for NodeID enum
    NLOHMANN_JSON_SERIALIZE_ENUM(NodeFactories::NodeID,
    {
        { NodeFactories::NodeID::SceneNode, "SceneNode" },
        { NodeFactories::NodeID::SpriteNode, "SpriteNode" },
        { NodeFactories::NodeID::TileMapNode, "TileMapNode" },
        { NodeFactories::NodeID::ShapeNode, "ShapeNode" },
        { NodeFactories::NodeID::YSortNode, "YSortNode" },
        { NodeFactories::NodeID::TextNode, "TextNode" },
        { NodeFactories::NodeID::SoundPlayerNode, "SoundPlayerNode" },
        { NodeFactories::NodeID::TriggerNode, "TriggerNode" },
    })

#endif // TEST_NODEFACTORIES_HPP
