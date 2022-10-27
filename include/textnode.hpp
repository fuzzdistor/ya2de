#ifndef TEST_TEXTNODE_HPP
#define TEST_TEXTNODE_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <scenenode.hpp>


class TextNode : public SceneNode
{
public:
    TextNode();

    void setFont(const sf::Font& font);
    float getTextWidth() const;
    void setCharacterSize(unsigned int size);
    void setOutlineThickness(float thickness);
    void setFillColor(const sf::Color color);
    void setOutlineColor(const sf::Color color);
    void setString(std::string string);
    std::string getString() const;

protected:
    virtual void setLuaUsertype() override;

private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Text m_text;
    sf::Sound m_sound;
    sf::SoundBuffer m_soundbuffer;

    friend class NodeFactories;
};

#endif //TEST_TEXTNODE_HPP

