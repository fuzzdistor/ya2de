#ifndef YA2DE_STATE_STACK_HPP
#define YA2DE_STATE_STACK_HPP

#include "state_frame.hpp"

#include <functional>
#include <vector>


class StateStack : sf::NonCopyable
{
public:
    enum class Action
    {
        Push,
        Pop,
        Clear,
    };

    explicit StateStack(StateFrame::Context context);

    template <typename StateType>
    void registerState(unsigned int id);

    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event& event);

    void pushState(unsigned int id);
    void popState();
    void clearStates();

    [[nodiscard]] bool isEmpty() const;

private:
    StateFrame::UPtr createStateFrame(unsigned int id);
    void             applyPendingChanges();

    struct PendingChange
    {
        explicit PendingChange(Action action, unsigned int id = 0) : action(action), id(id){};
        Action       action;
        unsigned int id;
    };

    std::map<unsigned int, std::function<StateFrame::UPtr()>> m_stateFactory;
    std::vector<StateFrame::UPtr>                             m_stack;
    std::vector<PendingChange>                                m_pendingList;
    StateFrame::Context                                       m_context;
};

template <typename StateType>
void StateStack::registerState(unsigned int id)
{
    m_stateFactory[id] = [this] { return std::make_unique<StateType>(*this, m_context); };
}

#endif //YA2DE_STATE_STACK_HPP
