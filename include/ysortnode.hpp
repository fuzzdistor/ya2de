#ifndef YA2DE_YSORTNODE_HPP
#define YA2DE_YSORTNODE_HPP

#include <SFML/System/Time.hpp>
#include <scenenode.hpp>

#include <algorithm>
#include <vector>

class YSortNode : public SceneNode
{
public:
    YSortNode();
    virtual void update(sf::Time dt) override;

protected:
    virtual void setLuaUsertype() override;

    friend class NodeFactories;
};

#endif // YA2DE_YSORTNODE_HPP

