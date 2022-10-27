#include <SFML/Audio/SoundBuffer.hpp>

#include <soundplayernode.hpp>
#include <scenenode.hpp>

SoundPlayerNode::SoundPlayerNode()
    : SceneNode(SceneNode::Mask::none)
    , m_player(sf::Sound())
{
}

void SoundPlayerNode::setSoundSource(const sf::SoundBuffer& buffer)
{
    if(m_player.index() != PlayerType::Sound)   // if m_player does not hold a Sound type...
        m_player.emplace<sf::Sound>(buffer);    // ...overwrite it
    else
        std::get<sf::Sound>(m_player).setBuffer(buffer);
}

void SoundPlayerNode::setLuaUsertype()
{
    auto usertype = getLuaState()->new_usertype<SoundPlayerNode>("SoundPlayerNode"
            , sol::base_classes, sol::bases<SceneNode>());

    usertype["play"] = &SoundPlayerNode::play;
    usertype["stop"] = &SoundPlayerNode::stop;

    SceneNode::setLuaUsertype();
}
void SoundPlayerNode::setSoundSource(const std::string& filepath)
{
    if(m_player.index() != PlayerType::Music)   // if m_player does not hold a Music type...
        m_player.emplace<sf::Music>();          // ...overwrite it

    std::get<sf::Music>(m_player).openFromFile(filepath);
}

void SoundPlayerNode::play()
{
    if(m_player.index() == PlayerType::Music)
        std::get<sf::Music>(m_player).play();
    else if(m_player.index() == PlayerType::Sound)
        std::get<sf::Sound>(m_player).play();
}

void SoundPlayerNode::stop()
{
    if(m_player.index() == PlayerType::Music)
        std::get<sf::Music>(m_player).stop();
    else if(m_player.index() == PlayerType::Sound)
        std::get<sf::Sound>(m_player).stop();
}

void SoundPlayerNode::setLoop(bool loop)
{
    if(m_player.index() == PlayerType::Music)
        std::get<sf::Music>(m_player).setLoop(loop);
    else if(m_player.index() == PlayerType::Sound)
        std::get<sf::Sound>(m_player).setLoop(loop);
}
