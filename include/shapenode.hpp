#ifndef TEST_SHAPENODE_HPP
#define TEST_SHAPENODE_HPP

#include "nlohmann/json.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>
#include <scenenode.hpp>
#include <string_view>

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
    const sf::Color getFillColor() const;

protected:
    virtual void setLuaUsertype() override;

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingRect() const override;
    bool m_enabled{ true };
    bool m_visible{ true };

    sf::RectangleShape m_shape;
    Shapes m_shapeID;

    friend class NodeFactories;
};

NLOHMANN_JSON_SERIALIZE_ENUM(ShapeNode::Shapes, {
    { ShapeNode::Shapes::Rectangle, "Rectangle" },
    { ShapeNode::Shapes::Circle, "Circle" },
})

#endif // TEST_SHAPENODE_HPP
