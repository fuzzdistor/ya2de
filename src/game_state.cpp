#include "game_state.hpp"

GameState::GameState(StateStack& stack, Context context) : StateFrame(stack, context)
{
}

void GameState::draw()
{
}

bool GameState::update(sf::Time dt)
{
    return false;
}

bool GameState::handleEvent(const sf::Event& event)
{
    return false;
}
