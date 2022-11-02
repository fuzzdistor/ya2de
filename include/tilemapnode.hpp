#ifndef YA2DE_TILEMAPNODE_HPP
#define YA2DE_TILEMAPNODE_HPP
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <nlohmann/json_fwd.hpp>

#include <scenenode.hpp>
#include <tileset.hpp>
#include <resourceidentifiers.hpp>

#include <vector>

using json = nlohmann::json;


class TileMapNode : public SceneNode
{
    // forward declaration
    template<typename T>
    struct Cell;

public:
    typedef std::vector<Cell<unsigned int>> TileMapData;

    TileMapNode();
    TileMapNode(const std::string& jsonDataFilePath, const TileSet& tileSet);

    void setTileScale(float scale);
    void setTileSize(float scale);
    void setTileSet(const TileSet& tileSet);
    void setMapInfo(const std::string& mapInfoPath);

protected:
    void setLuaUsertype() override;

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void drawTileIndex(sf::RenderTarget& target, sf::RenderStates states) const;
    void buildMap();
    void buildTextures();

    template<typename T>
    struct Cell
    {
        // for some reason templates dont have a default constructor?
        Cell(T _x, T _y, size_t _id) : x(_x) , y(_y) , id(_id) {};

        T x;
        T y;
        size_t id;
    };

    TileSet::Type m_tileType{};
    unsigned int m_mapWidth{0};
    unsigned int m_mapHeight{0};
    const TileSet* m_tileSet;
    mutable sf::Sprite m_layerSprite;
    std::vector<TileMapData> m_tileMapLayers;
    std::vector<sf::Texture> m_mapTextures;
    json m_mapLayerInfo{};

    sf::Font d_font{};
    mutable sf::Text d_text{};
    mutable std::vector<Cell<float>> d_cachedPositions{};

    friend class NodeFactories;
};

#endif // YA2DE_TILEMAPNODE_HPP

