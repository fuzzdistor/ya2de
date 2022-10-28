#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <LoggerCpp/Logger.h>
#include <nlohmann/json.hpp>

#include <sol/inheritance.hpp>
#include <tilemapnode.hpp>
#include <resourceidentifiers.hpp>
#include <tileset.hpp>
#include <utils.hpp>

#include <string>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <cmath>

TileMapNode::TileMapNode()
    : SceneNode(Mask::Background)
    , m_tileSet()
    , m_layerSprite()
    , m_tileMapLayers()
    , m_mapTextures()
{
    d_font.loadFromFile("media/fonts/Metropolis-SemiBold.otf");
    d_text.setFont(d_font);
    d_text.setCharacterSize(10U);
    d_text.setFillColor(sf::Color::White);
    d_text.setOutlineThickness(1U);
    d_text.setOutlineColor(sf::Color::Black);
}

TileMapNode::TileMapNode(const std::string& jsonDataFilePath, const TileSet& tileSet)
    : SceneNode(Mask::Background)
    , m_tileSet(&tileSet)
    , m_layerSprite()
    , m_tileMapLayers()
    , m_mapTextures()
{
    const json mapData = utils::jsonLoadFromFile(jsonDataFilePath);

    // TileSetter has different fields for square and isometric tilesets
    // if it is an isometric tileset then it has tile_width as a field
    m_tileType = tileSet.getType();

    m_mapWidth = mapData["map_width"];
    m_mapHeight = mapData["map_height"];

    m_mapLayerInfo = mapData["layers"];

    // builds maps AND the textures. I'm planning to eventually
    // separate the functions lauer with a texture update function
    // that refreshes the textures after a runtime change by an event
    // or player interaction
    buildMap();

    d_font.loadFromFile("media/fonts/Metropolis-SemiBold.otf");
    d_text.setFont(d_font);
    d_text.setCharacterSize(10U);
    d_text.setFillColor(sf::Color::White);
    d_text.setOutlineThickness(1U);
    d_text.setOutlineColor(sf::Color::Black);
}

void TileMapNode::setLuaUsertype()
{
    auto cell = getLuaState()->new_usertype<Cell<unsigned int>>("Cell");
    cell["x"] = &Cell<unsigned int>::x;
    cell["y"] = &Cell<unsigned int>::y;
    cell["id"] = &Cell<unsigned int>::id;

    auto usertype = getLuaState()->new_usertype<TileMapNode>("TileMapNode"
            , sol::base_classes, sol::bases<SceneNode>());

    // TODO find a way to add this to the lua state
    //usertype["tilemap_layers"] = &m_tileMapLayers;

    SceneNode::setLuaUsertype();
}

void TileMapNode::setTileSize(float size)
{
    float scale = size / static_cast<float>(m_tileSet->getTileHeight());
    m_layerSprite.setScale(scale, scale);
}

void TileMapNode::setTileScale(float scale)
{
    m_layerSprite.setScale(scale, scale);
}

void TileMapNode::setMapInfo(const std::string& mapInfoPath)
{
    const json mapData = utils::jsonLoadFromFile(mapInfoPath);

    m_mapWidth = mapData["map_width"];
    m_mapHeight = mapData["map_height"];

    m_mapLayerInfo = mapData["layers"];

    if(m_tileSet != nullptr)
        buildMap();
}

void TileMapNode::setTileSet(const TileSet& tileSet)
{
    m_tileSet = &tileSet;
    m_tileType = tileSet.getType();
    if (!m_mapLayerInfo.empty())
        buildMap();
}

void TileMapNode::buildMap()
{
    for(auto& layer: m_mapLayerInfo)
    {
        auto& tileMapLayer = m_tileMapLayers.emplace_back();
        for(auto& tile: layer["positions"])
        {
            tileMapLayer.emplace_back(Cell<unsigned int>(
                        tile["x"]
                        , tile["y"]
                        , tile["id"]));
        }
    }

    buildTextures();
}

void TileMapNode::buildTextures()
{
    const sf::Image tileset = m_tileSet->getTexture().copyToImage();

    // tilePixelUnit is the base size in pixels for tiles
    // Square tiles are 1 Unit x 1 Unit in size.
    // Isometric tiles are 2 Units x 1 Unit in size.
    unsigned int tilePixelUnit = m_tileSet->getTileHeight();

    for(const auto& layer: m_tileMapLayers)
    {
        sf::Image layerImage;
        unsigned int pixelWidth;
        unsigned int pixelHeight;
        if(m_tileType == TileSet::Type::Isometric)
        {
            pixelWidth  = (m_mapWidth + m_mapHeight) * tilePixelUnit;
            pixelHeight = (m_mapHeight + m_mapWidth) * tilePixelUnit/2;
        }
        if(m_tileType == TileSet::Type::Square)
        {
            pixelWidth  = m_mapWidth * tilePixelUnit;
            pixelHeight = m_mapHeight * tilePixelUnit;
        }

        layerImage.create(pixelWidth, pixelHeight, sf::Color::Transparent);

        for(const auto& tile: layer)
        {
            // the build order for rectangular tilemaps is top to bottom
            // in columns from left to right. For isometric tilemaps it's from top to left
            // in tilted columns from top to right. An example here for square tilemaps can be found here:
            // https://cdn.discordapp.com/attachments/968305996491530244/1035165772529549402/TileOrder.gif
            sf::IntRect rect = sf::IntRect(m_tileSet->getTile(tile.id).textureRect);
            sf::Vector2u destination;

            if(m_tileType == TileSet::Type::Isometric)
            {
                destination.x = (m_mapHeight - tile.y + tile.x) * tilePixelUnit;
                destination.y = (tile.x + tile.y) * tilePixelUnit/2;
            }
            if(m_tileType == TileSet::Type::Square)
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

    if(d_cachedPositions.size() != m_tileMapLayers.back().size())
    {
        float tileUnitSize = static_cast<float>(m_tileSet->getTileHeight());

        // Get the indexes for just the top-most layer
        for(auto& tile: m_tileMapLayers.back())
        {
            float x = 0.f;
            float y = 0.f;
            if(m_tileType == TileSet::Type::Isometric)
            {
                x = static_cast<float>(m_mapHeight - tile.y + tile.x) * tileUnitSize + tileUnitSize / 2.f;
                y = static_cast<float>(tile.x + tile.y) * tileUnitSize/2.f;
            }
            if(m_tileType == TileSet::Type::Square)
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

