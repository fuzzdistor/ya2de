#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <LoggerCpp/Logger.h>
#include <sol/error.hpp>
#include <sol/forward.hpp>
#include <sol/inheritance.hpp>
#include <sol/sol.hpp>

#include <spritenode.hpp>

#include <functional>
#include <cmath>


SpriteNode::SpriteNode()
    : SpriteNode::SceneNode(Mask::none)
    , m_sprite()
{
}

SpriteNode::SpriteNode(const sf::Texture& texture)
    : SpriteNode::SceneNode(Mask::none)
    , m_sprite()
{
    m_sprite.setTexture(texture);
}

void SpriteNode::setLuaUsertype()
{
    auto usertype = getLuaState()->new_usertype<SpriteNode>("SpriteNode"
            , sol::base_classes, sol::bases<SceneNode>());

    SceneNode::setLuaUsertype();
}

sf::FloatRect SpriteNode::getBoundingRect() const
{
    sf::FloatRect rect(m_sprite.getTextureRect());
    rect.top = -m_sprite.getOrigin().y;
    rect.left = -m_sprite.getOrigin().x;

    return getWorldTransform().transformRect(rect);
}

void SpriteNode::setTexture(const sf::Texture& texture)
{
    m_sprite.setTexture(texture);
}

void SpriteNode::setSpriteOrigin(const sf::Vector2f origin)
{
    setSpriteOrigin(origin.x, origin.y);
}

void SpriteNode::setSpriteOrigin(const float x, const float y)
{
    m_sprite.setOrigin(x, y);
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}
