#ifndef YA2DE_RESOURCEIDENTIFIERS_HPP
#define YA2DE_RESOURCEIDENTIFIERS_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <nlohmann/json.hpp>

#include <tileset.hpp>


#define TEXTURE_IDS \
X(none, "TextureID::none") \
X(Background, "TextureID::Background") \
X(BackgroundIso, "TextureID::BackgroundIso") \
X(BackgroundRoom, "TextureID::BackgroundRoom") \
X(Test, "TextureID::Test") \
X(Player, "TextureID::Player") \

#define SHADER_IDS \
X(none,"ShaderID::none") \
X(BrightnessPass,"ShaderID::BrightnessPass") \
X(DownSamplePass,"ShaderID::DownSamplePass") \
X(GaussianBlurPass,"ShaderID::GaussianBlurPass") \
X(AddPass,"ShaderID::AddPass") \

#define FONTS_IDS \
X(none,"FontID::none") \
X(Main,"FontID::Main") \
X(MesloLGS_NF_Regular,"FontID::MesloLGS_NF_Regular") \
X(Metropolis_SemiBold,"FontID::Metropolis_SemiBold") \

#define SOUNDEFFECT_IDS \
X(none,"SoundEffectID::none") \
X(MillieVoice,"SoundEffectID::MillieVoice") \

#define MUSIC_IDS \
X(none,"MusicID::none") \
X(MenuTheme,"MusicID::MenuTheme") \
X(FoolishVilain,"MusicID::FoolishVilain") \

#define TILESET_IDS \
X(none,"TileSetID::none") \
X(Room,"TileSetID::Room") \
X(Iso,"TileSetID::Iso") \

#define X(texture, name) texture,
enum class TextureID
{
    TEXTURE_IDS
};
#undef X

#define X(texture, name) name,
[[gnu::unused]] static const char* texture_names[] =
{
    TEXTURE_IDS
};
#undef X

#define X(font, name) font,
enum class FontID
{
    FONTS_IDS
};
#undef X

#define X(font, name) name,
[[gnu::unused]] static const char* font_names[] =
{
    FONTS_IDS
};
#undef X

#define X(shader, name) shader,
enum class ShaderID
{
    SHADER_IDS
};
#undef X

#define X(shader, name) name,
[[gnu::unused]] static const char* shader_names[] =
{
    SHADER_IDS
};
#undef X

#define X(soundeffect, name) soundeffect,
enum class SoundEffectID
{
    SOUNDEFFECT_IDS
};
#undef X

#define X(soundeffect, name) name,
[[gnu::unused]] static const char* soundeffect_names[] =
{
    SOUNDEFFECT_IDS
};
#undef X

#define X(tileset, name) tileset,
enum class TileSetID
{
    TILESET_IDS
};
#undef X

#define X(tileset, name) name,
[[gnu::unused]] static const char* tileset_names[] =
{
    TILESET_IDS
};
#undef X


NLOHMANN_JSON_SERIALIZE_ENUM(TextureID,
{
    { TextureID::Player, "Player" },
    { TextureID::Test, "Test" },
    { TextureID::Background, "Background" },
    { TextureID::BackgroundIso, "BackgroundIso" },
    { TextureID::BackgroundRoom, "BackgroundRoom" },
})

NLOHMANN_JSON_SERIALIZE_ENUM(FontID,
{
    { FontID::Main, "Main" },
    { FontID::MesloLGS_NF_Regular, "MesloLGS_NF_Regular" },
    { FontID::Metropolis_SemiBold, "Metropolis_SemiBold" },
})

NLOHMANN_JSON_SERIALIZE_ENUM(TileSetID,
{
    { TileSetID::Room, "Room" },
    { TileSetID::Iso, "Iso" },
})

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class GenericResourceCollection;

typedef GenericResourceCollection<sf::Texture, TextureID>			TextureCollection;
typedef GenericResourceCollection<sf::Font, FontID>					FontCollection;
typedef GenericResourceCollection<sf::Shader, ShaderID>				ShaderCollection;
typedef GenericResourceCollection<sf::SoundBuffer, SoundEffectID>	SoundBufferCollection;
typedef GenericResourceCollection<TileSet, TileSetID>	            TileSetCollection;

#endif // YA2DE_RESOURCEIDENTIFIERS_HPP

