#ifndef YA2DE_APPLICATION_HPP
#define YA2DE_APPLICATION_HPP

#include "state_stack.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


class Application
{
public:
    Application();
    ~Application();

    void run();

private:
    void update();
    void handleEvents();
    void draw();

#ifndef NDEBUG
    void imGuiUpdateBegin();
    void imGuiUpdateEnd();
#endif

    sf::RenderWindow m_window{};
    ResourcePack     m_resourcePack{};
    StateStack       m_stateStack{{m_window, m_resourcePack}};
    sf::Time         m_frameTime{};
    sf::Time         m_appTime{sf::Time::Zero};
    sf::Clock        m_frameClock{};
};


#endif //YA2DE_APPLICATION_HPP
