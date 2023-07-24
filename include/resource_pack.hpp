#ifndef YA2DE_RESOURCE_PACK_HPP
#define YA2DE_RESOURCE_PACK_HPP

#include "resource_holder.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>


struct ResourcePack
{
    ResourceHolder<sf::Texture>     textures;
    ResourceHolder<sf::Font>        fonts;
    ResourceHolder<sf::Music>       music;
    ResourceHolder<sf::SoundBuffer> sounds;
    ResourceHolder<sf::Shader>      shaders;
};

#endif //YA2DE_RESOURCE_PACK_HPP
