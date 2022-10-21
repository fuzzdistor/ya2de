#ifndef TEST_SHAPENODE_HPP
#define TEST_SHAPENODE_HPP

#include "nlohmann/json.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <scenenode.hpp>

class ShapeNode : public SceneNode
{
public:
    enum class Shapes {
        Rectangle,
        Circle,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(Shapes,
    {
        { Shapes::Rectangle, "Rectangle" },
        { Shapes::Circle, "Circle" },
    })

    ShapeNode(const Shapes& shape);

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape m_shape;
    Shapes m_shapeID;
};

#endif // TEST_SHAPENODE_HPP
