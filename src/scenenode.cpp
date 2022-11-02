#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <LoggerCpp/Logger.h>

#include <ios>
#include <scenenode.hpp>
#include <input.hpp>

#include <algorithm>
#include <memory>
#include <cassert>
#include <cmath>
#include <sol/forward.hpp>
#include <sol/raii.hpp>
#include <sol/variadic_args.hpp>
#include <sol/wrapper.hpp>
#include <string_view>


void my_panic(sol::optional<std::string> maybe_msg) {
    Log::Logger logger("Lua panic handler");
	logger.critic() << "Lua is in a panic state and will now abort() the application";
	if (maybe_msg) {
		const std::string& msg = maybe_msg.value();
		logger.critic() << "error message: " << msg;
	}
	// When this function exits, Lua will exhibit default behavior and abort()
}

SceneNode::SceneNode(const char* logChannelName)
    : m_logger(logChannelName)
    , m_script(sol::c_call<decltype(&my_panic), &my_panic>)
{
}

SceneNode::SceneNode()
    : m_script(sol::c_call<decltype(&my_panic), &my_panic>)
{
}

SceneNode::SceneNode(SceneNode::Mask mask)
    : m_children()
    , m_mask(mask)
    , m_script(sol::c_call<decltype(&my_panic), &my_panic>)
{
}

SceneNode::~SceneNode()
{
    sol::protected_function lend(m_script.get<sol::function>("end"));

    // if there is an init function then call it and check for
    // errors. Throw if there are any errors.
    if (lend != sol::nil)
    {
        auto presult = lend();
        if (!presult.valid())
            m_logger.critic() << "end error encountered! message: " << sol::error(presult).what();
    }
}

void SceneNode::setLuaUsertype()
{
    auto usertype = getLuaState().new_usertype<SceneNode>("SceneNode"
            , sol::factories(std::make_unique<SceneNode>));
    usertype["move"] = static_cast<void(SceneNode::*)(float, float)>(&SceneNode::move);
    usertype["setPosition"] = sol::overload(
                static_cast<void(SceneNode::*)(sf::Vector2f const&)>(&SceneNode::setPosition)
                , static_cast<void(SceneNode::*)(float, float)>(&SceneNode::setPosition));
    usertype["setScale"] = static_cast<void(SceneNode::*)(float, float)>(&SceneNode::setScale);
    usertype["rotate"] = &SceneNode::rotate;
    usertype["visible"] = &SceneNode::m_visible;
    usertype["setRotation"] = &SceneNode::setRotation;
    usertype["getRotation"] = &SceneNode::getRotation;
    usertype["getScale"] = &SceneNode::getScale;
    usertype["getPosition"] = &SceneNode::getPosition;
    usertype["markForDestruction"] = &SceneNode::markForDestruction;
    usertype["invoke"] = [](SceneNode& node, const char* function, sol::variadic_args args)
        { node.getLuaState()[function](args); };
    usertype["attachChild"] = [](SceneNode& node, UniPtr& uniptr)
        {
            std::cout << "node children size: " << node.m_children.size() << '\n';
            node.attachChild(std::move(uniptr));
            std::cout << "node children size: " << node.m_children.size() << std::endl;
        };
    usertype["dettachChild"] = &SceneNode::dettachChild;
}

void SceneNode::loadScriptFile(const std::string& filepath)
{
    auto err =  m_script.script_file(filepath);
    if(!err.valid())
        throw (&err);

    Log::Logger log("SceneNode::loadScriptFile");
    log.info() << "Loaded lua script: \""<< filepath << '"';

    l_update = m_script.get<sol::function>("update");
};

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for(auto& child: sceneGraph.m_children)
		checkSceneCollision(*child, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
    if (node.m_mask != Mask::Player)
        return;
    if (this != &node && collision(*this, node) && !isMarkedForDestruction() && !node.isMarkedForDestruction())
		collisionPairs.insert(std::minmax(this, &node));

	for(auto& child: m_children)
		child->checkNodeCollision(node, collisionPairs);
}

bool SceneNode::isMarkedForDestruction() const
{
    return m_markedForDestruction;
}

sol::state_view SceneNode::getLuaState() const
{
    return m_script;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    for(auto node = this; node != nullptr; node = node->m_parent)
        transform = node->getTransform() * transform;

    return transform;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    if (m_visible)
        drawCurrent(target, states);

    drawChildren(target, states);

    if (m_debugFlag)
    {
        drawBoundingRect(target, states);
        drawOrigin(target, states);
    }
}

void SceneNode::setDebugInfo(bool debug)
{
    m_debugFlag = debug;
}

sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states [[gnu::unused]]) const
{
    sf::FloatRect rect = getBoundingRect();

    sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

void SceneNode::drawOrigin(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::CircleShape originCircle;
	originCircle.setRadius(5.f);
    auto origin = states.transform.transformPoint(getOrigin());
	originCircle.setPosition(origin - sf::Vector2f(originCircle.getRadius(), originCircle.getRadius()));
	originCircle.setFillColor(sf::Color(0xFF888888));
	originCircle.setOutlineColor(sf::Color::Red);
	originCircle.setOutlineThickness(1.f);

    sf::VertexArray va(sf::Lines, 4);
    sf::Vertex v[4];
    v[0].color = sf::Color::Red;
    v[1].color = sf::Color::Red;
    v[2].color = sf::Color::Red;
    v[3].color = sf::Color::Red;
    v[0].position = origin - sf::Vector2f(originCircle.getRadius(), 0.f);
    v[1].position = origin + sf::Vector2f(originCircle.getRadius(), 0.f);
    v[2].position = origin - sf::Vector2f(0.f, originCircle.getRadius());
    v[3].position = origin + sf::Vector2f(0.f, originCircle.getRadius());

    for(int i = 0; i < 4; i++)
        va.append(v[i]);

    target.draw(originCircle);
    target.draw(va);
}

void SceneNode::drawCurrent(sf::RenderTarget& target [[gnu::unused]], sf::RenderStates states [[gnu::unused]]) const
{
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(auto& child: m_children)
    {
        child->draw(target, states);
    }
}

void SceneNode::attachChild(SceneNode::UniPtr node)
{
    node->m_parent = this;
    m_children.push_back(std::move(node));
}

SceneNode::UniPtr SceneNode::dettachChild(const SceneNode &node)
{
    auto found = std::find_if(m_children.begin(), m_children.end(),
            [&](UniPtr& p) { return p.get() == &node; });

    assert(found != m_children.end());

    UniPtr result = std::move(*found);
    result->m_parent = nullptr;
    m_children.erase(found);
    return result;
}

void SceneNode::init()
{
    sol::protected_function linit(m_script.get<sol::function>("init"));

    // if there is an init function then call it and check for
    // errors. Throw if there are any errors.
    if (linit != sol::nil)
    {
        auto presult = linit();
        if (!presult.valid())
            throw sol::error(presult);
    }

    for(auto& child: m_children)
        child->init();
}

void SceneNode::update(sf::Time dt)
{
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time dt)
{
    // if "update" is not present do nothing
    if (l_update == sol::nil)
        return;

    // check whether the update executed successfully or there were
    // errors. Throw if there were any.
    auto presult = l_update(dt.asSeconds());
    if(!presult.valid())
    {
        Log::Logger logger("Update Error");
        logger.critic() << "There was an error in the Lua update call";
        throw sol::error(presult);
    }
}

void SceneNode::updateChildren(sf::Time dt)
{
    for(auto& child: m_children)
        child->update(dt);
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
    auto lhs_rect = lhs.getBoundingRect();
    auto rhs_rect = rhs.getBoundingRect();

    if (lhs_rect.height == 0 or lhs_rect.width == 0 or rhs_rect.height == 0 or rhs_rect.width == 0)
        return false;

    return lhs_rect.intersects(rhs_rect);
}

float length(sf::Vector2f vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

float distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}

void SceneNode::markForDestruction()
{
    m_markedForDestruction = true;
}
void SceneNode::removeMarkedChildren()
{
	// Remove all children which request so
	auto firstMarked = std::remove_if(m_children.begin(), m_children.end(), std::mem_fn(&SceneNode::isMarkedForDestruction));


    m_children.erase(firstMarked, m_children.end());

	// Call function recursively for all remaining children
	std::for_each(m_children.begin(), m_children.end(), std::mem_fn(&SceneNode::removeMarkedChildren));
}
