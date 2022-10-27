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

protected:
    virtual void setLuaUsertype() override;

private:
    sf::FloatRect getBoundingRect() const override;

    mutable SceneNode* m_shapenode;
    CallbackFn m_callback;

    Log::Logger m_logger{ "TriggerNode" };

    friend class NodeFactories;
};

#endif // TEST_TRIGGERNODE_HPP
