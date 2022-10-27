#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <shapenode.hpp>

ShapeNode::ShapeNode()
    : ShapeNode::SceneNode(Mask::none)
    , m_shape()
    , m_shapeID(Shapes::Rectangle)
{
}

void ShapeNode::setLuaUsertype()
{
    auto usertype = getLuaState()->new_usertype<ShapeNode>("ShapeNode"
            , sol::base_classes, sol::bases<SceneNode>());

    usertype["enabled"] = &ShapeNode::m_enabled;
    usertype["visible"] = &ShapeNode::m_visible;
    usertype["setFillColor"] = &ShapeNode::setFillColor;
    usertype["setSize"] = sol::overload(
            static_cast<void(ShapeNode::*)(sf::Vector2f const&)>(&ShapeNode::setSize)
            , static_cast<void(ShapeNode::*)(float, float)>(&ShapeNode::setSize));
    usertype["getFillColor"] = &ShapeNode::getFillColor;

    SceneNode::setLuaUsertype();
}

void ShapeNode::setSize(float x, float y)
{
    setSize({x, y});
}

void ShapeNode::setSize(const sf::Vector2f& size)
{
    m_shape.setSize(size);
}

void ShapeNode::setFillColor(const sf::Color color)
{
    m_shape.setFillColor(color);
}

const sf::Color ShapeNode::getFillColor() const
{
    return m_shape.getFillColor();
}

sf::FloatRect ShapeNode::getBoundingRect() const
{
    if (!m_enabled)
        return sf::FloatRect{};

    sf::FloatRect rect = m_shape.getLocalBounds();

    return getWorldTransform().transformRect(rect);

}

void ShapeNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_visible)
        target.draw(m_shape, states);
}

