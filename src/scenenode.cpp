#include "scene.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
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

#include <scenenode.hpp>
#include <input.hpp>

#include <algorithm>
#include <memory>
#include <cassert>
#include <cmath>
#include <sol/wrapper.hpp>

sf::Keyboard::Key getKeyBind(const std::string_view action)
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

bool checkAction(const std::string& action)
{
    return sf::Keyboard::isKeyPressed(getKeyBind(action));
}

void normalize(sf::Vector2f& v)
{
    float m = v.x*v.x + v.y*v.y;
    if (m != 0)
        v /= sqrtf(m);
}

void my_panic(sol::optional<std::string> maybe_msg) {
	std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
	if (maybe_msg) {
		const std::string& msg = maybe_msg.value();
		std::cerr << "\terror message: " << msg << std::endl;
	}
	// When this function exits, Lua will exhibit default behavior and abort()
}

SceneNode::SceneNode(SceneNode::Mask mask)
    : m_children()
    , m_parent(nullptr)
    , m_mask(mask)
    , m_debugFlag(false)
    , m_markedForDestruction(false)
    , m_script(std::make_shared<sol::state>(sol::c_call<decltype(&my_panic), &my_panic>))
{
    m_script->open_libraries(
            sol::lib::base, 
            sol::lib::math, 
            sol::lib::package,
            sol::lib::table,
            sol::lib::string);

    m_script->set_function("checkAction"
            , &checkAction
            );

    // Setting Vector2f
    auto vector2f = m_script->new_usertype<sf::Vector2f>("Vector2f"
            , sol::constructors<sf::Vector2f(float, float)>());
    vector2f.set("x", &sf::Vector2f::x);
    vector2f.set("y", &sf::Vector2f::y);
    vector2f.set_function("normalize", &normalize);

    // Setting API for Transformable object
    m_script->set("this", this);
    m_script->new_usertype<SceneNode>("SceneNode"
            , "move"
            , static_cast<void(SceneNode::*)(float, float)>(&SceneNode::move)
            , "setPosition"
            , sol::overload(
                static_cast<void(SceneNode::*)(sf::Vector2f const&)>(&SceneNode::setPosition)
                , static_cast<void(SceneNode::*)(float, float)>(&SceneNode::setPosition)
                )
            , "setScale"
            , static_cast<void(SceneNode::*)(float, float)>(&SceneNode::setScale)
            , "rotate"
            , &SceneNode::rotate
            , "setRotation"
            , &SceneNode::setRotation
            , "getRotation"
            , &SceneNode::getRotation
            , "getScale"
            , &SceneNode::getScale
            , "getPosition"
            , &SceneNode::getPosition
            );
}

bool SceneNode::loadScriptFile(const std::string& filepath)
{
    auto err =  m_script->script_file(filepath);
    if(err.valid())
    {
        Log::Logger log("SceneNode::loadScriptFile");
        log.info() << "Loaded lua script: \""<< filepath << '"';
        return true;
    }
    throw (&err);
};

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for(auto& child: sceneGraph.m_children)
		checkSceneCollision(*child, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node) && !isMarkedForDestruction() && !node.isMarkedForDestruction())
		collisionPairs.insert(std::minmax(this, &node));

	for(auto& child: m_children)
		child->checkNodeCollision(node, collisionPairs);
}

bool SceneNode::isMarkedForDestruction() const
{
    return m_markedForDestruction;
}

SceneNode::ScriptPtr SceneNode::getLuaState() const
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
    drawCurrent(target, states);
    drawChildren(target, states);

    if(m_debugFlag) 
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
    initCurrent();
    initChildren();
}

void SceneNode::update(sf::Time dt)
{
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::initCurrent() 
{
    sol::protected_function linit(m_script->get<sol::function>("init"));
    auto presult = linit();
    //if (!presult.valid())
}

void SceneNode::initChildren()
{
    for(auto& child: m_children)
        child->init();
}

void SceneNode::updateCurrent(sf::Time dt) 
{
    sol::protected_function lupdate(m_script->get<sol::function>("update"));
    auto presult = lupdate(dt.asSeconds());
    //if (!presult.valid())
}

void SceneNode::updateChildren(sf::Time dt)
{
    for(auto& child: m_children)
        child->update(dt);
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
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
