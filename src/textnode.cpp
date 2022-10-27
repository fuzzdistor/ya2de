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
    
    luavm->open_libraries(sol::lib::coroutine);

    m_soundbuffer.loadFromFile("media/sounds/m_speech.wav");
    m_sound.setBuffer(m_soundbuffer);
    std::vector<int> v;
}

std::string TextNode::getString() const
{
    return m_text.getString();
}


void TextNode::setLuaUsertype()
{
    getLuaState()->new_usertype<sf::Sound>("Sound"
            , "play"
            , &sf::Sound::play
            , "stop"
            , &sf::Sound::stop
            , "setPitch"
            , &sf::Sound::setPitch
            );

    auto usertype = getLuaState()->new_usertype<TextNode>("TextNode"
            , sol::base_classes, sol::bases<SceneNode>());

    usertype["sound"] = &TextNode::m_sound;
    usertype["setCharacterSize"] = &TextNode::setCharacterSize;
    usertype["setFillColor"] = &TextNode::setFillColor;
    usertype["setOutlineColor"] = &TextNode::setOutlineColor;
    usertype["setOutlineThickness"] = &TextNode::setOutlineThickness;
    usertype["setString"] = &TextNode::setString;
    usertype["getTextWidth"] = &TextNode::getTextWidth;
    usertype["getString"] = &TextNode::getString;

    SceneNode::setLuaUsertype();
}

void TextNode::setCharacterSize(unsigned int size)
{
    m_text.setCharacterSize(size);
}

void TextNode::setOutlineThickness(float thickness)
{
    m_text.setOutlineThickness(thickness);
}

void TextNode::setFillColor(const sf::Color color)
{
    m_text.setFillColor(color);
}

void TextNode::setOutlineColor(const sf::Color color)
{
    m_text.setOutlineColor(color);
}

float TextNode::getTextWidth() const
{
    return m_text.getLocalBounds().width;
}

void TextNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_text, states);
}

void TextNode::setString(std::string string)
{
    m_text.setString(string);
}

void TextNode::setFont(const sf::Font &font)
{
    m_text.setFont(font);
}

