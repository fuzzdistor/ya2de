#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <LoggerCpp/Logger.h>
#include <nlohmann/json.hpp>

#include <tilemapnode.hpp>
#include <tileset.hpp>
#include <utils.hpp>

#include <string>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <cmath>


TileMapNode::TileMapNode(const std::string& jsonDataFilePath, const sf::Texture& tileMapTexture)
    : SceneNode(Mask::Background)
    , m_tileSet(tileMapTexture)
    , m_layerSprite()
    , m_tileMaps()
    , m_mapTextures()
{

    const json mapData = utils::jsonLoadFromFile(jsonDataFilePath);

    // TileSetter has different fields for square and isometric tilesets
    // if it is an isometric tileset then it has tile_width as a field
    m_isIsometric = mapData.contains("tile_width");

    if(m_isIsometric)
        m_tileSet.setTileSize(mapData["tile_height"]);
    else
        m_tileSet.setTileSize(mapData["tile_size"]);

    m_tileSet.setIsometric(m_isIsometric);

    m_width = mapData["map_width"];
    m_height = mapData["map_height"];

    const json layers = mapData["layers"];

    // builds maps AND the textures. I'm planning to eventually
    // separate the functions lauer with a texture update function 
    // that refreshes the textures after a runtime change by an event
    // or player interaction
    buildMap(layers);

    d_font.loadFromFile("media/fonts/Metropolis-SemiBold.otf");
    d_text.setFont(d_font);
    d_text.setCharacterSize(10U);
    d_text.setFillColor(sf::Color::White);
    d_text.setOutlineThickness(1U);
    d_text.setOutlineColor(sf::Color::Black);
}

void TileMapNode::setTileSize(float size)
{
    float scale = size / static_cast<float>(m_tileSet.getUnitPixelSize());
    m_layerSprite.setScale(scale, scale);
}

void TileMapNode::setTileScale(float scale)
{
    m_layerSprite.setScale(scale, scale);
}

void TileMapNode::buildMap(const json& layers)
{
    for(auto& layer: layers)
    {
        auto& tileMap = m_tileMaps.emplace_back();
        for(auto& tile: layer["positions"])
        {
            tileMap.push_back({
                    tile["x"].get<unsigned int>()
                    , tile["y"].get<unsigned int>()
                    , tile["id"].get<int>()});
        }
    }

    buildTextures();
}

void TileMapNode::buildTextures()
{
    const sf::Image tileset = m_tileSet.getTexture().copyToImage();

    // tilePixelUnit is the base size in pixels for tiles
    // Square tiles are 1 Unit x 1 Unit in size.
    // Isometric tiles are 2 Units x 1 Unit in size.
    unsigned int tilePixelUnit = m_tileSet.getUnitPixelSize();

    for(const auto& layer: m_tileMaps)
    {
        sf::Image layerImage;
        unsigned int pixelWidth;
        unsigned int pixelHeight;
        if(m_isIsometric)
        {
            pixelWidth  = (m_width + m_height) * tilePixelUnit;
            pixelHeight = (m_height + m_width) * tilePixelUnit/2;
        }
        else 
        {
            pixelWidth  = m_width * tilePixelUnit;
            pixelHeight = m_height * tilePixelUnit;
        }
        
        layerImage.create(pixelWidth, pixelHeight, sf::Color::Transparent);

        for(const auto& tile: layer)
        {
            // the build order for normal tilemaps is top to bottom 
            // in columns from left to right
            sf::IntRect rect = m_tileSet.getTile(static_cast<size_t>(tile.id)).textureRect;
            sf::Vector2u destination;

            if(m_isIsometric)
            {
                destination.x = (m_height - tile.y + tile.x) * tilePixelUnit;
                destination.y = (tile.x + tile.y) * tilePixelUnit/2;
            }
            else
            {
                destination.x = tile.x * tilePixelUnit;
                destination.y = tile.y * tilePixelUnit;
            }

            layerImage.copy(tileset, destination.x, destination.y, rect, true);
        }
        auto& layerTexture = m_mapTextures.emplace_back();
        layerTexture.loadFromImage(layerImage);
    }
}

void TileMapNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for(const auto& layer: m_mapTextures)
    {
        m_layerSprite.setTexture(layer);
        target.draw(m_layerSprite, states);
    }

    if(isDebug())
        drawTileIndex(target, states);
}

void TileMapNode::drawTileIndex(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Show tile on top right corner
    /* static int FPS = 0; */
    /* static size_t index = 0; */

    /* FPS++; */
    /* if (FPS > 2000) */
    /* { */
    /*     while (FPS > 2000) */
    /*         FPS -= 2000; */

    /*     index++; */
    /* } */
    /* sf::Sprite sp; */
    /* sp.setTexture(m_tileSet.getTexture()); */
    /* sp.setTextureRect(m_tileSet.getTile(index).textureRect); */
    /* target.draw(sp, states); */

    if(d_cachedPositions.size() != m_tileMaps.back().size())
    {
        float tileUnitSize = static_cast<float>(m_tileSet.getUnitPixelSize());
        for(auto& tile: m_tileMaps.back())
        {
            float x = 0.f;
            float y = 0.f;
            if(m_isIsometric)
            {
                x = static_cast<float>(m_height - tile.y + tile.x) * tileUnitSize + tileUnitSize / 2.f;
                y = static_cast<float>(tile.x + tile.y) * tileUnitSize/2.f;
            } 
            else 
            {
                x = static_cast<float>(tile.x) * tileUnitSize;
                y = static_cast<float>(tile.y) * tileUnitSize;
            }
            d_cachedPositions.push_back({ x, y, tile.id });
        }
    }

    for(auto& info : d_cachedPositions)
    {
        d_text.setString(std::to_string(info.id)); 
        d_text.setPosition(std::floor(info.x * m_layerSprite.getScale().x), std::floor(info.y * m_layerSprite.getScale().x));
        target.draw(d_text, states);
    }
}

