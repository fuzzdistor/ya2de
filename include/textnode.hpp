#ifndef YA2DE_TEXTNODE_HPP
#define YA2DE_TEXTNODE_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <scenenode.hpp>


class TextNode : public SceneNode
{
public:
    TextNode();

    float getTextWidth() const;

protected:
    virtual void setLuaUsertype() override;

    sf::Text m_text;

private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Sound m_sound;
    sf::SoundBuffer m_soundbuffer;

    friend class NodeFactories;
};

#endif //YA2DE_TEXTNODE_HPP

