#ifndef TEST_TILEMAPNODE_HPP
#define TEST_TILEMAPNODE_HPP
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <nlohmann/json_fwd.hpp>

#include <scenenode.hpp>
#include <tileset.hpp>

#include <vector>

using json = nlohmann::json;


class TileMapNode : public SceneNode
{
public:
    TileMapNode(const std::string& dataFilePath, const sf::Texture& tileMapTexture);

    void setTileScale(float scale);
    void setTileSize(float scale);

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawTileIndex(sf::RenderTarget& target, sf::RenderStates states) const;
    void buildMap(const json& layers);
    void buildTextures();

    template<typename T>
    struct Cell 
    {
        T x;
        T y;
        int id;
    };

    bool m_isIsometric {false};
    unsigned int m_width {0};
    unsigned int m_height {0};
    TileSet m_tileSet;
    mutable sf::Sprite m_layerSprite;
    std::vector<std::vector<Cell<unsigned int>>> m_tileMaps;
    std::vector<sf::Texture> m_mapTextures;

    sf::Font d_font {};
    mutable sf::Text d_text {};
    mutable std::vector<Cell<float>> d_cachedPositions {};
};

#endif // TEST_TILEMAPNODE_HPP
