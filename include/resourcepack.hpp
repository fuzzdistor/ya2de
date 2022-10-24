#ifndef TEST_RESOURCEPACK_HPP
#define TEST_RESOURCEPACK_HPP

#include <SFML/System/NonCopyable.hpp>
#include <resourceidentifiers.hpp>
#include <resourcecollection.hpp>

struct ResourcePack : private sf::NonCopyable
{
    TextureCollection textures;
    SoundBufferCollection sounds;
    FontCollection fonts;
    TileSetCollection tilesets;
    ShaderCollection shaders;
};

#endif // TEST_RESOURCEPACK_HPP
