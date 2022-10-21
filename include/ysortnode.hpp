#ifndef TEST_YSORTNODE_HPP
#define TEST_YSORTNODE_HPP

#include <SFML/System/Time.hpp>
#include <scenenode.hpp>

#include <algorithm>
#include <vector>

class YSortNode : public SceneNode 
{
public:
    YSortNode();
    virtual void update(sf::Time dt) override;
};

#endif // TEST_YSORTNODE_HPP
