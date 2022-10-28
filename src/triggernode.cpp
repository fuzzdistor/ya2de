#include "LoggerCpp/Logger.h"
#include "scenenode.hpp"
#include <SFML/System/Time.hpp>
#include <algorithm>
#include <triggernode.hpp>


TriggerNode::TriggerNode()
    : SceneNode(SceneNode::Mask::Trigger)
    , m_shapenode(nullptr)
    , m_callback(nullptr)
{
    m_callback = [&](SceneNode* otherNode, float)
    {
        // simple test callback that rotates other node and then gets removed
        otherNode->getLuaState()->set("triggercallback", true);
        this->markForDestruction();
    };
}

void TriggerNode::setLuaUsertype()
{
    auto usertype = getLuaState()->new_usertype<TriggerNode>("TriggerNode"
            , sol::base_classes, sol::bases<SceneNode>());

    usertype["callback"] = &TriggerNode::m_callback;

    SceneNode::setLuaUsertype();
}

sf::FloatRect TriggerNode::getBoundingRect() const
{
    // search once for the first collision shape node in children
    if (m_shapenode == nullptr)
    {
        auto collisionshape = std::find_if(m_children.begin(), m_children.end()
                , [](const SceneNode::UniPtr& node)
                { return node->getMask() == SceneNode::Mask::CollisionShape; });
        assert(collisionshape != m_children.end());

        m_shapenode = collisionshape->get();
    }

    return m_shapenode->getBoundingRect();
}

void TriggerNode::applyCallbackTo(SceneNode* node, const sf::Time& dt)
{
    Log::Logger logger("TriggerNode::applyCallbackTo");
    logger.setLevel(Log::Log::eInfo);
    logger.debug() << "Applying callback to colliding node";
    m_callback(node, dt.asSeconds());
}

