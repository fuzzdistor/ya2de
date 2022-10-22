#ifndef TEST_TRIGGERNODE_HPP
#define TEST_TRIGGERNODE_HPP

#include <LoggerCpp/Logger.h>
#include <scenenode.hpp>
#include <functional>

class TriggerNode : public SceneNode
{
public:
    typedef std::function<void(SceneNode*)> CallbackFn;
    TriggerNode();

    void applyCallbackTo(SceneNode* node);

private:
    sf::FloatRect getBoundingRect() const override;

    mutable SceneNode* m_shapenode;
    CallbackFn m_callback;

    Log::Logger m_logger{ "TriggerNode" };

};

#endif // TEST_TRIGGERNODE_HPP
