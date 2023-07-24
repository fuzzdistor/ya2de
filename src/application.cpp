#include "application.hpp"
#include "game_state.hpp"

#include "config.h"

#include <SFML/Window/Event.hpp>

#ifndef NDEBUG
#include <imgui-SFML.h>
#include <imgui.h>
#endif


Application::Application() : m_window({1280, 720}, "YA2DE", sf::Style::None)
{
    m_window.setVerticalSyncEnabled(true);
#ifndef NDEBUG
    ImGui::SFML::Init(m_window);
    ImGui::GetIO().IniFilename = nullptr; // disable saving imgui.ini file
#endif                                    // NDEBUG

    m_stateStack.registerState<GameState>(0);
    m_stateStack.pushState(0);
}

void Application::run()
{
    while (m_window.isOpen())
    {
        m_frameTime = m_frameClock.restart();
        m_appTime += m_frameTime;

        handleEvents();
        update();
        if (m_stateStack.isEmpty())
        {
            m_window.close();
        }

        draw();
    }
}

void Application::handleEvents()
{
    sf::Event event{};
    while (m_window.pollEvent(event))
    {
#ifndef NDEBUG
        ImGui::SFML::ProcessEvent(m_window, event);
#endif // NDEBUG

        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        if (event.type == sf::Event::Resized)
        {
            auto win_size = sf::Vector2f(m_window.getSize());
            m_window.setView({win_size / 2.f, win_size});
        }

        m_stateStack.handleEvent(event);
    }
}

void Application::update()
{
#ifndef NDEBUG
    imGuiUpdateBegin();
#endif
    m_stateStack.update(m_frameTime);
#ifndef NDEBUG
    imGuiUpdateEnd();
#endif
}

void Application::draw()
{
    m_window.clear();
    m_stateStack.draw();
#ifndef NDEBUG
    ImGui::SFML::Render(m_window);
#endif // NDEBUG
    m_window.display();
}

Application::~Application()
{
#ifndef NDEBUG
    ImGui::SFML::Shutdown(m_window);
#endif // NDEBUG
}

#ifndef NDEBUG
void Application::imGuiUpdateBegin()
{
    ImGui::SFML::Update(m_window, m_frameTime);
    ImGui::Begin("Debug");
}

void Application::imGuiUpdateEnd()
{
    if (ImGui::Button("Close"))
    {
        m_window.close();
    }
    ImGui::Text("frame time: %dms", m_frameTime.asMilliseconds());
    ImGui::Text("%s ver %s", PROJECT_NAME, PROJECT_VER);
    ImGui::End();
}
#endif // NDEBUG
