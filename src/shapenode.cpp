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

ShapeNode::ShapeNode(const Shapes& shape)
    : ShapeNode::SceneNode(Mask::none)
    , m_shape()
    , m_shapeID(shape)
{
    m_shape.setFillColor(sf::Color(0xFFFFFFAA));
    m_shape.setSize({700, 180});
    setPosition({-350, 100});
}

sf::FloatRect ShapeNode::getBoundingRect() const
{
    auto rect = m_shape.getGlobalBounds();
    rect.left += getPosition().x;
    rect.top += getPosition().y;
    return rect;
}

void ShapeNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
}
