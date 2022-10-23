#include "LoggerCpp/Logger.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <tileset.hpp>
#include <utils.hpp>

#include <vector>
#include <cassert>

TileSet::TileSet()
    : m_tileType(Type::Square)
    , m_tileHeight(0)
    , m_tileWidth(0)
    , m_tiles()
    , m_texture()
{
}

bool TileSet::loadFromFile(const std::string& path)
{
    const json tileSetData = utils::jsonLoadFromFile(path);


    Log::Logger logger("TilesetLoadFromFile");

    // TODO optimize strings
    std::string texturePath;
    // TileSetter keeps its tileset textures in the same path as the .txt with
    // all the json data. If for some reason I later feel like keeping them 
    // apart in a separate path I want to encode the texture path in the json
    // with the texture key.
    if (tileSetData.contains("texture"))
    {
        texturePath = tileSetData["texture"];
        logger.critic() << tileSetData["texture"];
    }
    else 
    {
        // TODO rewrite in order to recognize and cut just the extension.
        texturePath = path.substr(0, path.size() - 4) + ".png";
        logger.critic() << path.substr(0, path.size() - 4) + ".png";
    }

    if(!m_texture.loadFromFile(texturePath))
        return false;

    if (tileSetData.contains("tile_size"))
    {
        m_tileType = Type::Square;
        m_tileWidth = m_tileHeight = tileSetData["tile_size"];
    }
    else 
    {
        m_tileType = Type::Isometric;
        m_tileWidth = tileSetData["tile_width"];
        m_tileHeight = tileSetData["tile_height"];
    }


    // the tileset is read from top left to the right, row by row
    // this is the way TileSetter packs the tilemap tiles
    for(unsigned int y = 0; y < m_texture.getSize().y ; y+= m_tileHeight)
        for(unsigned int x = 0; x < m_texture.getSize().x ; x+= m_tileWidth)
        {
            m_tiles.emplace_back(UIntRect(x,y, m_tileWidth, m_tileHeight));
        }

    return true;
}

TileSet::Type TileSet::getType() const
{
    return m_tileType;
}

unsigned int TileSet::getTileHeight() const
{
    return m_tileHeight;
}

unsigned int TileSet::getTileWidth() const
{
    return m_tileHeight;
}

const TileSet::Tile& TileSet::getTile(size_t index) const
{
    return m_tiles[index];
}

