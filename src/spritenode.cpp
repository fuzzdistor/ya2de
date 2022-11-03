#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <LoggerCpp/Logger.h>
#include <sol/error.hpp>
#include <sol/forward.hpp>
#include <sol/inheritance.hpp>
#include <sol/overload.hpp>
#include <sol/sol.hpp>

#include <spritenode.hpp>

#include <functional>
#include <cmath>


SpriteNode::SpriteNode()
    : SpriteNode::SceneNode(Mask::none)
{
}

SpriteNode::SpriteNode(const sf::Texture& texture)
    : SpriteNode::SceneNode(Mask::none)
    , m_sprite(texture)
{
}

void SpriteNode::setLuaUsertype()
{
    auto sprite = getLuaState().new_usertype<sf::Sprite>("Sprite"
            , sol::no_constructor);

    sprite["getColor"] = &sf::Sprite::getColor;
    sprite["setColor"] = &sf::Sprite::setColor;

    auto usertype = getLuaState().new_usertype<SpriteNode>("SpriteNode"
            , sol::base_classes, sol::bases<SceneNode>());

    usertype["sprite"] = &SpriteNode::m_sprite;
    usertype["setFrame"] = &SpriteNode::setFrame;
    usertype["setOrigin"] = sol::overload(
            static_cast<void(SpriteNode::*)(sf::Vector2f const&)>(&SpriteNode::setOrigin)
            , static_cast<void(SpriteNode::*)(float, float)>(&SpriteNode::setOrigin));
    usertype["setSpriteOrigin"] = sol::overload(
            static_cast<void(SpriteNode::*)(sf::Vector2f const&)>(&SpriteNode::setSpriteOrigin)
            , static_cast<void(SpriteNode::*)(float, float)>(&SpriteNode::setSpriteOrigin));
    usertype["setAnimationInfo"] = &SpriteNode::setAnimationInfo;
    usertype["getFrameCount"] = &SpriteNode::getFrameCount;

    SceneNode::setLuaUsertype();
}

size_t SpriteNode::getFrameCount() const
{
    return m_frame.positions.size();
}

void SpriteNode::updateFrameInfo()
{
    if (m_sprite.getTexture() == nullptr)
        return;

    auto textureSize = m_sprite.getTexture()->getSize();

    // give warnings about excess pixels in the texture
    if (auto excessPixels = textureSize.x % m_frame.vframes)
        m_logger.warning() << "Texture width is not a multiple of the specified vertical frame count "
            << m_frame.vframes << " by " << excessPixels << " pixels";
    if (auto excessPixels = textureSize.y % m_frame.hframes)
        m_logger.warning() << "Texture heigth is not a multiple of the specified horizontal frame count "
            << m_frame.vframes << " by " << excessPixels << " pixels";

    m_frame.size.x = static_cast<int>(textureSize.x / m_frame.vframes);
    m_frame.size.y = static_cast<int>(textureSize.y / m_frame.hframes);

    m_frame.positions.reserve(m_frame.vframes*m_frame.hframes);
    for (int j = 0; j < static_cast<int>(m_frame.hframes); j++)
    {
        for (int i = 0; i < static_cast<int>(m_frame.vframes); i++)
        {
            m_frame.positions.emplace_back(i*m_frame.size.x, j*m_frame.size.y);
        }
    }
}

void SpriteNode::setAnimationInfo(unsigned int hframes, unsigned int vframes, size_t index)
{
    m_logger.debug() << "setting animation info with hframes = " << hframes << ", vframes = " << vframes << ", index = " << index;
    m_frame.hframes = hframes;
    m_frame.vframes = vframes;
    m_frame.index = index;

    updateFrameInfo();
}

void SpriteNode::setFrame(size_t index)
{
    if (m_frame.index == index)
        return;
    if (index > m_frame.positions.size() || index < 1)
    {
        m_logger.warning() << "Animation index underflow/overflow!: requested index: " << index << ". Available indexes: 1 to " << m_frame.positions.size();
        index = index % m_frame.positions.size() + 1;
    }

    m_frame.rect = sf::IntRect(m_frame.positions[index - 1], m_frame.size);
    m_sprite.setTextureRect(m_frame.rect);
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

void SpriteNode::setSpriteOrigin(const sf::Vector2f& origin)
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
