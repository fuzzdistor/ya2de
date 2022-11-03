#ifndef YA2DE_SPRITENODE_HPP
#define YA2DE_SPRITENODE_HPP

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
    void setAnimationInfo(unsigned int h_frames, unsigned int v_frames, size_t frameIndex = 0);
    void setFrame(size_t index);
    void setSpriteOrigin(const sf::Vector2f& origin);
    void setSpriteOrigin(const float x, const float y);
    size_t getFrameCount() const;

protected:
    void setLuaUsertype() override;

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual sf::FloatRect getBoundingRect() const override;
    void updateFrameInfo();

    struct FrameInfo
    {
        size_t index {0};
        unsigned int hframes {1};
        unsigned int vframes {1};
        sf::Vector2i size {};
        std::vector<sf::Vector2i> positions {};
        sf::IntRect rect {};
    };

    sf::Sprite m_sprite {};
    FrameInfo m_frame {};

    friend class NodeFactories;
};

#endif // YA2DE_SPRITENODE_HPP

