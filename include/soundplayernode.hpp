#ifndef TEST_SOUNDPLAYERNODE_HPP
#define TEST_SOUNDPLAYERNODE_HPP

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include <scenenode.hpp>


class SoundPlayerNode : public SceneNode
{
public:
    SoundPlayerNode();
    void setSoundSource(const std::string& filepath);
    void setSoundSource(const sf::SoundBuffer& buffer);
    void play();
    void stop();
    void setLoop(bool loop);

private:
    // Utility enum to give names to type indexes in variant
    enum PlayerType {
        Music = 0,
        Sound = 1,
    };

    std::variant<sf::Music, sf::Sound> m_player;
};
 
#endif  // TEST_SOUNDPLAYERNODE_HPP
