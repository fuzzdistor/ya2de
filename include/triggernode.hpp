#ifndef TEST_TRIGGERNODE_HPP
#define TEST_TRIGGERNODE_HPP

#include "scenenode.hpp"
#include <functional>

class TriggerNode : public SceneNode
{
public:
    typedef std::function<void(SceneNode*)> Callback;
    TriggerNode();

    void applyCallbackTo(SceneNode* node);

private:
    sf::FloatRect getBoundingRect() const override;

    mutable SceneNode* m_shapenode;
    Callback m_callback;

};

#endif // TEST_TRIGGERNODE_HPP
