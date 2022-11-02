#ifndef TEST_NODEFACTORIES_HPP
#define TEST_NODEFACTORIES_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>
#include <LoggerCpp/LoggerCpp.h>

#include <map>
#include <cassert>
#include <functional>
#include <memory>
#include <sol/forward.hpp>
#include <sol/raii.hpp>
#include <sol/sol.hpp>
#include <string_view>


using ordered_json = nlohmann::ordered_json;

class SceneNode;
struct ResourcePack;

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
    template<typename T>
    std::unique_ptr<T> makeUniqueNode() const;

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

inline sf::Keyboard::Key getKeyBind(const std::string_view action)
{
    if (action == "move_up")
        return sf::Keyboard::W;
    if (action == "move_down")
        return sf::Keyboard::S;
    if (action == "move_left")
        return sf::Keyboard::A;
    if (action == "move_right")
        return sf::Keyboard::D;
    if (action == "action_a")
        return sf::Keyboard::J;
    if (action == "action_b")
        return sf::Keyboard::K;
    return sf::Keyboard::KeyCount;
}

inline bool checkAction(const std::string& action)
{
    return sf::Keyboard::isKeyPressed(getKeyBind(action));
}

template<typename T>
std::unique_ptr<T> NodeFactories::makeUniqueNode() const
{
    auto node = std::make_unique<T>();

    sol::state_view luavm = node->getLuaState();

    luavm.open_libraries(
            sol::lib::base,
            sol::lib::math,
            sol::lib::package,
            sol::lib::table,
            sol::lib::coroutine,
            sol::lib::string);

    node->setLuaUsertype();

    // setting utility types
    auto color = luavm.template new_usertype<sf::Color>("Color"
            , sol::no_constructor);

    // color constructor
    color["new"] = [](float r, float g, float b, float a) -> sf::Color
    { return sf::Color(
            static_cast<sf::Uint8>(r)
            , static_cast<sf::Uint8>(g)
            , static_cast<sf::Uint8>(b)
            , static_cast<sf::Uint8>(a)
            );
    };

    // color channels as propierties
    color["r"] = &sf::Color::r;
    color["g"] = &sf::Color::g;
    color["b"] = &sf::Color::b;
    color["a"] = &sf::Color::a;

    // vector2 constructor
    auto vec2 = luavm.template new_usertype<sf::Vector2f>("Vector2"
            , sol::constructors<sf::Vector2f(float, float)>());

    // propierties + normalize function
    vec2.set("x", &sf::Vector2f::x);
    vec2.set("y", &sf::Vector2f::y);
    vec2.set_function("normalize", [](sf::Vector2f& v)
            { //utility normalize function for Vector2
                float m = v.x*v.x + v.y*v.y;
                if (m != 0)
                    v /= sqrtf(m);
            });

    luavm.set_function("checkAction"
            , &checkAction);

    luavm.set("node", node.get());

    return node;
}

#endif // TEST_NODEFACTORIES_HPP
