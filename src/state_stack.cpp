#include "state_stack.hpp"

StateStack::StateStack(StateFrame::Context context) : m_stack(), m_pendingList(), m_context(context), m_stateFactory()
{
}

void StateStack::update(sf::Time dt)
{
    for (auto& frame : m_stack)
    {
        if (!frame->update(dt))
        {
            break;
        }
    }

    applyPendingChanges();
}

void StateStack::draw()
{
    for (auto& frame: m_stack)
    {
        frame->draw();
    }
}

void StateStack::handleEvent(const sf::Event& event)
{
    for (auto& frame: m_stack)
    {
        if (!frame->handleEvent(event))
        {
            break;
        }
    }

    applyPendingChanges();
}

void StateStack::pushState(unsigned int id)
{
    m_pendingList.emplace_back(Action::Push, id);
}

void StateStack::popState()
{
    m_pendingList.emplace_back(Action::Pop);
}

void StateStack::clearStates()
{
    m_pendingList.emplace_back(Action::Clear);
}

bool StateStack::isEmpty() const
{
    return m_stack.empty();
}

StateFrame::UPtr StateStack::createStateFrame(unsigned int id){
    auto found = m_stateFactory.find(id);
    assert(found != m_stateFactory.end());

    return found->second();
}

void StateStack::applyPendingChanges()
{
    for (PendingChange change: m_pendingList)
    {
        switch (change.action)
        {
            case Action::Push:
                m_stack.push_back(createStateFrame(change.id));
                break;
            case Action::Pop:
                m_stack.pop_back();
                break;
            case Action::Clear:
                m_stack.clear();
                break;
        }
    }

    m_pendingList.clear();
}

