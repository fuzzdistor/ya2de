#include <SFML/System/Thread.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <game.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <cmath>
#include <string>
#include <string_view>


Game::Game()
    : m_clock()
    , m_resources()
    , m_sceneData("data/scenes.json")
    , m_scene(std::make_unique<Scene>(m_window, m_resources, m_sceneData.getMainSceneDataPath()))
{
    Log::Logger logger("Game::Game");
    d_statisticsText.setFont(m_resources.fonts.get(FontID::Main));
    d_statisticsText.setCharacterSize(8U);
    d_statisticsText.setPosition(10.f, 10.f);
    d_statisticsText.setFillColor(sf::Color::White);

    sf::Image icon;
    icon.loadFromFile("media/icon.png");

    if(icon.getSize().x > 64 && icon.getSize().y > 64)
        logger.warning() << "Icon is big! Size of icon is " << icon.getSize().x << "x" << icon.getSize().y << "px. Max size is 32x32px";

    m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Game::run()
{
    while(m_window.isOpen())
    {
        handleInput();
        if(!m_window.isOpen()) break;
        update();
        render();
    }
}

void Game::handleInput()
{
    sf::Event event;

    while(m_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                m_logger.info() << "Closed via window X";
                m_window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        m_logger.info() << "Closed via Esc key";
                        m_window.close();
                        break;
                    case sf::Keyboard::F2:
                        m_captureThread.launch();
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void Game::update()
{
    m_frametime = m_clock.restart();
    processStatistics();
    m_scene->update(m_frametime);
    if (m_scene->requestsSceneChange())
        m_scene = std::make_unique<Scene>(m_window, m_resources, m_sceneData.getSceneDataPath(m_scene->getRequestedScene()));

}

void Game::render()
{
    m_window.clear();
    m_scene->draw();
    m_window.setView(m_window.getDefaultView());
    m_window.draw(d_statisticsText);
    m_window.display();
}

void Game::processStatistics()
{
    static float frameTimeCounter = 0.f;
    frameTimeCounter += m_frametime.asSeconds();
    d_statisticsFPS++;

    if(frameTimeCounter >= 1.f)
    {
        while(frameTimeCounter >= 1.f)
            frameTimeCounter -= 1.f;

        const auto averageFrameTime = 
            d_statisticsFPS < 500 && d_statisticsFPS != 0 ?
                std::to_string(static_cast<int>(1000/d_statisticsFPS)) + " ms" :
                std::to_string(static_cast<int>(1000000/d_statisticsFPS)) + " us";
        d_statisticsText.setString(
                std::to_string(d_statisticsFPS) + " FPS\n" 
                + averageFrameTime);
        d_statisticsFPS = 0;
    }
}

Game& Game::getInstance()
{
    static Game game;
    return game;
}
