#include "state_frame.hpp"
#include "state_stack.hpp"

StateFrame::StateFrame(StateStack& stack, StateFrame::Context context) : m_stack(stack), m_context(context)
{
}

void StateFrame::requestStackPush(unsigned int id)
{
    m_stack.pushState(id);
}

void StateFrame::requestStackPop()
{
    m_stack.popState();
}

void StateFrame::requestStackClear()
{
    m_stack.clearStates();
}

StateFrame::Context StateFrame::getContext() const
{
    return m_context;
}
