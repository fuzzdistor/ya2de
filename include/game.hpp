#ifndef TEST_MAIN_HPP
#define TEST_MAIN_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/Window/Event.hpp>
#include <LoggerCpp/LoggerCpp.h>

#include "resourcepack.hpp"
#include "scenedata.hpp"
#include <resourceidentifiers.hpp>
#include <scene.hpp>

#include <filesystem>


class Game
{
public:
    Game();
    ~Game();
    void run();

private:
    Game(Game&) = delete;
    void render();
    void update();
    void handleInput();
    void processStatistics();

#ifndef NDEBUG
    sf::RenderWindow m_window {{ 800, 600 }, "YA2DEngine DEBUG", sf::Style::Resize | sf::Style::Close};
#else
    sf::RenderWindow m_window {{ 800, 600 }, "YA2DEngine RELEASE", sf::Style::Resize | sf::Style::Close};
#endif
    sf::Time m_frametime {sf::Time::Zero};
    sf::Clock m_clock;
    ResourcePack m_resources;
    sf::Color m_clearColor {sf::Color::Black};
    Log::Logger m_logger {"Game"};
    SceneData m_sceneData;
    std::unique_ptr<Scene> m_scene;
    sf::Text d_statisticsText;
    unsigned int d_statisticsFPS {0};
};

#endif // TEST_MAIN_HPP
