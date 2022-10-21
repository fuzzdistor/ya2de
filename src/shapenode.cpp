#include <SFML/Graphics/RenderTarget.hpp>

#include <shapenode.hpp>

ShapeNode::ShapeNode(const Shapes& shape)
    : ShapeNode::SceneNode(Mask::none)
    , m_shape()
    , m_shapeID(shape)
{
    m_shape.setFillColor(sf::Color(0xFFFFFFAA));
    m_shape.setSize({700, 180});
    m_shape.setPosition({-350, 100});
}

void ShapeNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
}
