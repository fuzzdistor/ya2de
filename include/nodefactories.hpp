#ifndef TEST_NODEFACTORIES_HPP
#define TEST_NODEFACTORIES_HPP

#include <nlohmann/json.hpp>
#include <LoggerCpp/LoggerCpp.h>

#include <map>
#include <cassert>
#include <functional>
#include <memory>

using ordered_json = nlohmann::ordered_json;

class SceneNode;
class ResourcePack;

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
        AreaSwitchNode,
        TextboxNode,
        Invalid = -1,
    };

    NodeFactories(const ResourcePack& resources);
    std::unique_ptr<SceneNode> createSceneGraph(const ordered_json& recipe) const;

private:

    std::unique_ptr<SceneNode> createNode(const ordered_json& recipe) const;
    std::unique_ptr<SceneNode> nodeConstructor(const ordered_json& recipe) const;
    std::map<NodeID, SetterFunction> m_nodeSetters;
    Log::Logger m_logger { "NodeFactories" };
    std::map<NodeID, std::vector<NodeID>> m_nodeComposition;
};

// nlohmann json specializations for NodeID enum
NLOHMANN_JSON_SERIALIZE_ENUM(NodeFactories::NodeID, {
    { NodeFactories::NodeID::Invalid, "" },
    { NodeFactories::NodeID::SceneNode, "SceneNode" },
    { NodeFactories::NodeID::SpriteNode, "SpriteNode" },
    { NodeFactories::NodeID::TileMapNode, "TileMapNode" },
    { NodeFactories::NodeID::ShapeNode, "ShapeNode" },
    { NodeFactories::NodeID::YSortNode, "YSortNode" },
    { NodeFactories::NodeID::TextNode, "TextNode" },
    { NodeFactories::NodeID::SoundPlayerNode, "SoundPlayerNode" },
    { NodeFactories::NodeID::TriggerNode, "TriggerNode" },
    { NodeFactories::NodeID::AreaSwitchNode, "AreaSwitchNode" },
    { NodeFactories::NodeID::TextboxNode, "TextboxNode" },
})

#endif // TEST_NODEFACTORIES_HPP
