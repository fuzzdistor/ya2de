#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <shapenode.hpp>

ShapeNode::ShapeNode()
    : ShapeNode::SceneNode(Mask::none)
    , m_shape()
    , m_shapeID(Shapes::Rectangle)
{
    m_shape.setFillColor(sf::Color(0xFFFFFFAA));
    m_shape.setSize({700, 180});
    setPosition({-350, 100});
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

void ShapeNode::disable(bool status)
{
    m_disabled = status;
}

void ShapeNode::setVisible(bool visibility)
{
    m_visibility = visibility;
}

const sf::Color ShapeNode::getFillColor() const
{
    return m_shape.getFillColor();
}

sf::FloatRect ShapeNode::getBoundingRect() const
{
    if (m_disabled)
        return sf::FloatRect{};
    auto rect = m_shape.getGlobalBounds();
    rect.left += getPosition().x;
    rect.top += getPosition().y;
    return rect;
}

void ShapeNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_visibility)
        target.draw(m_shape, states);
}

