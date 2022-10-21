#include <scenenode.hpp>
#include <ysortnode.hpp>

YSortNode::YSortNode()
    : SceneNode(SceneNode::Mask::none)
{
}

void YSortNode::update(sf::Time dt)
{
    // Update normally
    SceneNode::update(dt);

    // And then sort acording to y value of updated children
    std::sort(m_children.begin(), m_children.end(), 
            [&](const auto& lhs, const auto& rhs)
            { return lhs->getPosition().y < rhs->getPosition().y; });
}
