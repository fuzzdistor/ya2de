#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <tileset.hpp>

#include <vector>
#include <cassert>

TileSet::TileSet(const sf::Texture& tileSetTexture)
    : m_isIsometric(false)
    , m_tilePixelSize()
    , m_texture(tileSetTexture)
    , m_tiles()
{
}

TileSet::TileSet(unsigned int tileSize, const sf::Texture& texture, bool isIsometric)
    : m_isIsometric(isIsometric)
    , m_tilePixelSize(tileSize)
    , m_texture(texture)
    , m_tiles()
{
    buildSet();
}

void TileSet::buildSet()
{
    // the tileset is read from top left to the right, row by row
    // this is the way TileSetter packs the tilemap tiles

    m_tiles = {};
    unsigned int tileWidht = m_isIsometric? m_tilePixelSize * 2 : m_tilePixelSize;

    for(unsigned int y = 0; y < m_texture.getSize().y ; y+= m_tilePixelSize)
        for(unsigned int x = 0; x < m_texture.getSize().x ; x+= tileWidht)
        {
            sf::IntRect rect(sf::Rect<unsigned int>(x,y, tileWidht, m_tilePixelSize));
            m_tiles.push_back({rect});
        }
}

unsigned int TileSet::getUnitPixelSize() const
{
    return m_tilePixelSize;
}

void TileSet::setTileSize(unsigned int size)
{
    assert(m_tilePixelSize != size && "smells like unnecessary operations");
    m_tilePixelSize = size;
    buildSet();
}

void TileSet::setIsometric(bool isIsometric)
{
    if(isIsometric == m_isIsometric)
        return;

    m_isIsometric = isIsometric;
    buildSet();
}

const sf::Texture& TileSet::getTexture() const
{
    return m_texture;
}

const TileSet::Tile& TileSet::getTile(size_t index) const
{
    return m_tiles[index];
}

