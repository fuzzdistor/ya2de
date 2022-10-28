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

float TextNode::getTextWidth() const
{
    return m_text.getLocalBounds().width;
}

