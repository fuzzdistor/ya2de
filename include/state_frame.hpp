#ifndef YA2DE_STATE_FRAME_HPP
#define YA2DE_STATE_FRAME_HPP

#include "resource_pack.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class StateStack;

class StateFrame
{
public:
    struct Context
    {
        Context(sf::RenderWindow& window, ResourcePack& resourcePack) : window(window), resourcePack(resourcePack){};

        sf::RenderWindow& window;
        ResourcePack&     resourcePack;
    };

    typedef std::unique_ptr<StateFrame> UPtr;

    StateFrame(StateStack& stack, Context context);
    virtual ~StateFrame() = default;

    virtual void draw()                              = 0;
    virtual bool update(sf::Time dt)                 = 0;
    virtual bool handleEvent(const sf::Event& event) = 0;

protected:
    void                  requestStackPush(unsigned int id);
    void                  requestStackPop();
    void                  requestStackClear();
    [[nodiscard]] Context getContext() const;

private:
    StateStack& m_stack;
    Context     m_context;
};

#endif //YA2DE_STATE_FRAME_HPP
