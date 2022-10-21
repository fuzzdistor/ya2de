#ifndef TEST_SPRITENODE_HPP
#define TEST_SPRITENODE_HPP

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <scenenode.hpp>


class SpriteNode : public SceneNode
{
public:
    SpriteNode();
    SpriteNode(const sf::Texture&);
    void setTexture(const sf::Texture& texture);
    void setOrigin(const sf::Vector2f origin);
    void setOrigin(const float x, const float y);

    // void setTextureRect(sf::FloatRect textureRect);

private:
    //virtual void updateCurrent(sf::Time dt) override;
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual sf::FloatRect getBoundingRect() const override;

    sf::Sprite m_sprite;
};

#endif // TEST_SPRITENODE_HPP
