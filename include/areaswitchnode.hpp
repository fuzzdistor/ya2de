#ifndef YA2DE_AREASWITCHERNODE_HPP
#define YA2DE_AREASWITCHERNODE_HPP

#include <LoggerCpp/LoggerCpp.h>
#include <scenenode.hpp>

class AreaSwitchNode : public SceneNode
{
public:
    AreaSwitchNode();

protected:
    void setLuaUsertype() override;

private:
    sf::FloatRect getBoundingRect() const override;

    mutable SceneNode* m_shapenode;
    std::string m_destinyArea{};
    Log::Logger m_logger{ "AreaSwitchNode" };


    friend class NodeFactories;
};


#endif // YA2DE_AREASWITCHERNODE_HPP

