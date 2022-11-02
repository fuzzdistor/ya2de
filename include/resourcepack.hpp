#ifndef YA2DE_RESOURCEPACK_HPP
#define YA2DE_RESOURCEPACK_HPP

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

#endif // YA2DE_RESOURCEPACK_HPP

