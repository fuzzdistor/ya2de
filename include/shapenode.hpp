#ifndef TEST_SHAPENODE_HPP
#define TEST_SHAPENODE_HPP

#include "nlohmann/json.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>
#include <scenenode.hpp>

using uint = unsigned int;

class ShapeNode : public SceneNode
{
public:
    enum class Shapes {
        Rectangle,
        Circle,
    };

    ShapeNode();

    void setSize(float x, float y);
    void setSize(const sf::Vector2f& size);
    void setFillColor(const sf::Color color);
    void disable(bool status);
    void setVisible(bool visibility);

    const sf::Color getFillColor() const;
    constexpr bool isDisabled() const { return m_disabled; };

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingRect() const override;
    bool m_disabled{ false };
    bool m_visibility{ true };

    sf::RectangleShape m_shape;
    Shapes m_shapeID;
};

NLOHMANN_JSON_SERIALIZE_ENUM(ShapeNode::Shapes, {
    { ShapeNode::Shapes::Rectangle, "Rectangle" },
    { ShapeNode::Shapes::Circle, "Circle" },
})

#endif // TEST_SHAPENODE_HPP
