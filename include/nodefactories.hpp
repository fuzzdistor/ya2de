#ifndef TEST_NODEFACTORIES_HPP
#define TEST_NODEFACTORIES_HPP

#include <nlohmann/json.hpp>
#include <LoggerCpp/LoggerCpp.h>

#include <resourceidentifiers.hpp>

#include <map>
#include <cassert>
#include <functional>
#include <memory>

using ordered_json = nlohmann::ordered_json;

class SceneNode;

class NodeFactories
{
public:
    typedef std::function<std::unique_ptr<SceneNode>(const ordered_json&)> FactoryFunction;
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

    NodeFactories(TextureCollection& textures, FontCollection& fonts, SoundBufferCollection& sounds, TileSetCollection& tilesets);
    std::unique_ptr<SceneNode> createNode(const ordered_json& recipe) const;

private:

    std::unique_ptr<SceneNode> nodeConstructor(const ordered_json& recipe) const;
    std::map<NodeID, SetterFunction> m_nodeSetters;
    Log::Logger m_logger { "NodeFactories" };
};

// nlohmann json specializations for NodeID enum
NLOHMANN_JSON_SERIALIZE_ENUM(NodeFactories::NodeID, {
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
