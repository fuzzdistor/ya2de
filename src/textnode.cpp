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
    luavm->set("text", &m_text);
    luavm->set("sound", &m_sound);
    luavm->new_usertype<sf::Sound>("Sound"
            , "play"
            , &sf::Sound::play
            , "stop"
            , &sf::Sound::stop
            , "setPitch"
            , &sf::Sound::setPitch
            );
    luavm->new_usertype<sf::Text>("Text"
            , "setCharacterSize"
            , &sf::Text::setCharacterSize
            , "setFillColor"
            , &sf::Text::setFillColor
            , "setOutlineColor"
            , &sf::Text::setOutlineColor
            , "setOutlineThickness"
            , &sf::Text::setOutlineThickness
            , "setString"
            , &sf::Text::setString
            , "getString"
            , &sf::Text::getString
            );

    // I've got no idea how to pass 
    // sf::Text::setString as function 
    // and call it from within lua with a 
    // const string ref and not by value
    // so that's why this ugly function goes
    luavm->set_function("setString"
            , &TextNode::setString
            , this
            );

    luavm->set_function("getTextWidth"
            , &TextNode::getTextWidth
            , this
            );

    // sensible text defaults
    m_text.setCharacterSize(20);
    m_text.setOutlineThickness(5);
    m_text.setFillColor(sf::Color(0xffffffff));
    m_text.setOutlineColor(sf::Color(0x000000ff));
    m_text.setString("DEFAULT");

    m_soundbuffer.loadFromFile("media/sounds/otsumillie.wav");
    m_sound.setBuffer(m_soundbuffer);
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

