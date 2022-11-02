#ifndef YA2DE_TILESET_HPP
#define YA2DE_TILESET_HPP

#include "LoggerCpp/Logger.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>

typedef sf::Rect<unsigned int> UIntRect;

class TileSet
{
public:
    struct Tile
    {
        Tile(UIntRect r):textureRect(r){}
        UIntRect textureRect;
    };

    enum class Type
    {
        Square,
        Isometric,
    };

    // TileSets must be default constructibles in order to be loaded as a resource
    TileSet();

    bool loadFromFile(const std::string& path);

    Type getType() const;
    const Tile& getTile(size_t index) const;
    constexpr const sf::Texture& getTexture() const { return m_texture; };
    unsigned int getTileHeight() const;
    unsigned int getTileWidth() const;

private:

    Type m_tileType;
    unsigned int m_tileHeight;
    unsigned int m_tileWidth;
    std::vector<Tile> m_tiles;

    sf::Texture m_texture;

    Log::Logger m_logger{ "TileSet" };
};

#endif // YA2DE_TILESET_HPP

