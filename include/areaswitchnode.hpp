#ifndef TEST_AREASWITCHERNODE_HPP
#define TEST_AREASWITCHERNODE_HPP

#include <LoggerCpp/LoggerCpp.h>
#include <scenenode.hpp>

class AreaSwitchNode : public SceneNode
{
public:
    AreaSwitchNode();

    void setDestinyArea(const std::string& destiny);
    std::string_view getDestinyArea() const;

private:
    sf::FloatRect getBoundingRect() const override;

    mutable SceneNode* m_shapenode;
    std::string m_destinyArea{};
    Log::Logger m_logger{ "AreaSwitchNode" };
};

#endif // TEST_AREASWITCHERNODE_HPP
