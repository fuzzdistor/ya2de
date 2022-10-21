#ifndef TEST_TILESET_HPP
#define TEST_TILESET_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>


class TileSet
{
public:
    struct Tile
    {
        sf::IntRect textureRect;
    };

    explicit TileSet(const sf::Texture& texture);
    TileSet(unsigned int tileSize, const sf::Texture& texture, bool isIsometric);

    void setTileSize(unsigned int size);
    void setIsometric(bool isIsometric);

    const Tile& getTile(size_t index) const;
    const sf::Texture& getTexture() const;
    unsigned int getUnitPixelSize() const;

private:
    void buildSet();

    bool m_isIsometric;
    unsigned int m_tilePixelSize;
    const sf::Texture& m_texture;
    std::vector<Tile> m_tiles;
};

#endif // TEST_TILESET_HPP

