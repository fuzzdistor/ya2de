#ifndef TEST_MAIN_HPP
#define TEST_MAIN_HPP

#include "scenedata.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/Window/Event.hpp>
#include <LoggerCpp/LoggerCpp.h>

#include <resourceidentifiers.hpp>
#include <scene.hpp>

#include <filesystem>


class Game
{
public:
    Game(Game&) = delete;
    static Game& getInstance();
    void run();

private:
    Game();
    void render();
    void update();
    void handleInput();
    void processStatistics();

    static void saveCapture(const Game* game)
    {
        game->m_logger.info() << "Screenshot functionality is disabled. Check bug report";
        return; // disabling capture until finding a thread safe way of doing it.
        // TODO fix application crashing with exit code 134 when screencapping too fast
        sf::Texture tex;
        tex.create(game->m_window.getSize().x, game->m_window.getSize().y);
        tex.update(game->m_window);
        auto img = tex.copyToImage();
        int i = 0;
        while (std::filesystem::exists("media/capture" + std::to_string(i) + ".png"))
            i++;
        img.saveToFile("media/capture" + std::to_string(i) + ".png");
        game->m_logger.info() << "Screenshot taken \"capture" << i << ".png\"";
    }

    sf::RenderWindow m_window {{ 800, 600 }, "YA2DEngine", sf::Style::Resize | sf::Style::Close};
    sf::Time m_frametime {sf::Time::Zero};
    sf::Clock m_clock; 
    TextureCollection m_textures;
    FontCollection m_fonts;
    sf::Color m_clearColor {sf::Color::Black};
    Log::Logger m_logger {"Game"};
    SceneData m_sceneData;
    Scene m_scene;
    sf::Thread m_captureThread {&Game::saveCapture, this};
    sf::Text d_statisticsText;
    unsigned int d_statisticsFPS {0};
};

#endif // TEST_MAIN_HPP
