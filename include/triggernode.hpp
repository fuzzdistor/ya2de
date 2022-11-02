#ifndef YA2DE_TRIGGERNODE_HPP
#define YA2DE_TRIGGERNODE_HPP

#include <LoggerCpp/Logger.h>
#include <SFML/System/Time.hpp>
#include <scenenode.hpp>
#include <functional>

class TriggerNode : public SceneNode
{
public:
    typedef std::function<void(SceneNode*, float)> CallbackFn;
    TriggerNode();

    void applyCallbackTo(SceneNode* node, const sf::Time& time);

protected:
    virtual void setLuaUsertype() override;

private:
    sf::FloatRect getBoundingRect() const override;

    mutable SceneNode* m_shapenode;
    mutable bool m_needsUpdate { true };
    CallbackFn m_callback;

    friend class NodeFactories;
};

#endif // YA2DE_TRIGGERNODE_HPP

