#include "LoggerCpp/Logger.h"
#include "scenenode.hpp"
#include <algorithm>
#include <triggernode.hpp>

TriggerNode::TriggerNode()
    : SceneNode(SceneNode::Mask::Trigger)
    , m_shapenode(nullptr)
    , m_callback()
{
    m_callback = [&](SceneNode* otherNode)
    {
        // simple test callback that rotates other node and then gets removed
        otherNode->setRotation(180);
        this->markForDestruction();
    };
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

void TriggerNode::applyCallbackTo(SceneNode* node)
{
    Log::Logger logger("TriggerNode::applyCallbackTo");
    logger.info() << "Applying callback to colliding node";
    m_callback(node);
}


