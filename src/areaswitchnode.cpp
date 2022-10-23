#include "scenenode.hpp"
#include <areaswitchnode.hpp>

AreaSwitchNode::AreaSwitchNode()
    : SceneNode(SceneNode::Mask::AreaSwitch)
    , m_shapenode(nullptr)
    , m_destinyArea()
{
}

sf::FloatRect AreaSwitchNode::getBoundingRect() const
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

void AreaSwitchNode::setDestinyArea(const std::string& destiny)
{
    m_destinyArea = destiny;
}

std::string_view AreaSwitchNode::getDestinyArea() const
{
    return m_destinyArea;
}
