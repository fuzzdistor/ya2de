#ifndef YA2DE_GAME_STATE_HPP
#define YA2DE_GAME_STATE_HPP

#include "state_frame.hpp"

class GameState : public StateFrame
{
public:
    GameState(StateStack& stack, Context context);
    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;
};

#endif //YA2DE_GAME_STATE_HPP
