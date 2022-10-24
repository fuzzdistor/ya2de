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

    // setting all kinds of APIs 
    luavm->new_usertype<sf::Color>("Color"
            , sol::constructors<sf::Color(sf::Uint32)>());
    luavm->set("sound", &m_sound);
    luavm->new_usertype<sf::Sound>("Sound"
            , "play"
            , &sf::Sound::play
            , "stop"
            , &sf::Sound::stop
            , "setPitch"
            , &sf::Sound::setPitch
            );
    luavm->set("text", this);
    luavm->new_usertype<TextNode>("Text"
            , "setCharacterSize"
            , &TextNode::setCharacterSize
            , "setFillColor"
            , &TextNode::setFillColor
            , "setOutlineColor"
            , &TextNode::setOutlineColor
            , "setOutlineThickness"
            , &TextNode::setOutlineThickness
            , "setString"
            , &TextNode::setString
            , "getTextWidth"
            , &TextNode::getTextWidth
            , "getString"
            , &TextNode::getString
            );

    // I've got no idea how to pass 
    // sf::Text::setString as function 
    // and call it from within lua with a 
    // const string ref and not by value
    // so that's why this ugly function goes

    // sensible text defaults
    m_soundbuffer.loadFromFile("media/sounds/m_speech.wav");
    m_sound.setBuffer(m_soundbuffer);
}

std::string TextNode::getString() const
{
    return m_text.getString();
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

