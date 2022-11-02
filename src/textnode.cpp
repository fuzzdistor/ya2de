#include "LoggerCpp/Logger.h"
#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include <scenenode.hpp>
#include <textnode.hpp>

TextNode::TextNode()
    : TextNode::SceneNode(Mask::none)
    , m_text()
    , m_sound()
    , m_soundbuffer()
{
    auto luavm = getLuaState();

    m_soundbuffer.loadFromFile("media/sounds/m_speech.wav");
    m_sound.setBuffer(m_soundbuffer);
    std::vector<int> v;
}

void TextNode::setLuaUsertype()
{
    getLuaState().new_usertype<sf::Sound>("Sound"
            , "play"
            , &sf::Sound::play
            , "stop"
            , &sf::Sound::stop
            , "setPitch"
            , &sf::Sound::setPitch
            );


    auto texttype = getLuaState().new_usertype<sf::Text>("Text");

    texttype["setCharacterSize"] = &sf::Text::setCharacterSize;
    texttype["setFillColor"] = &sf::Text::setFillColor;
    texttype["setOutlineColor"] = &sf::Text::setOutlineColor;
    texttype["setOutlineThickness"] = &sf::Text::setOutlineThickness;
    texttype["getString"] = [](const sf::Text& text)
                { return std::string(text.getString()); };
    texttype["setString"] = [](sf::Text& text, const std::string& string)
                { text.setString(string); };
    texttype["getBounds"] = &sf::Text::getLocalBounds;


    auto usertype = getLuaState().new_usertype<TextNode>("TextNode"
            , sol::base_classes, sol::bases<SceneNode>());

    usertype["sound"] = &TextNode::m_sound;
    usertype["text"] = &TextNode::m_text;
    usertype["getTextWidth"] = &TextNode::getTextWidth;

    SceneNode::setLuaUsertype();
}

void TextNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_text, states);
}

float TextNode::getTextWidth() const
{
    return m_text.getLocalBounds().width;
}

