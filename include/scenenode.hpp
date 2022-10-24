#ifndef TEST_SCENENODE_HPP
#define TEST_SCENENODE_HPP

#include "nlohmann/json.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <sol/reference.hpp>
#include <sol/sol.hpp>

#include <memory>
#include <vector>
#include <set>


class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode> UniPtr;
    typedef std::shared_ptr<sol::state> ScriptPtr;
    typedef std::pair<SceneNode*, SceneNode*> Pair;

    // Mask enums are set up with this bitshift operators so that their binary
    // representation can be composed of different base masks
    enum class Mask : unsigned int
    {
        none            = 0,        //0
        Player          = 1 << 0,   //1
        CollisionObject = 1 << 1,   //2
        NonPlayable     = 1 << 2,   //4
        Enemy           = 1 << 3,   //8
        AreaSwitch      = 1 << 4,   //16
        Background      = 1 << 5,   //32
        Trigger         = 1 << 6,   //64
        CollisionShape  = 1 << 7,   //128
    };

    explicit SceneNode(Mask mask);
    virtual ~SceneNode() = default;
    
    void init();
    virtual void update(sf::Time dt);

    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;

    virtual sf::FloatRect getBoundingRect() const;

    void attachChild(UniPtr node);
    UniPtr dettachChild(const SceneNode& node);

    void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);
    void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);

    bool isMarkedForDestruction() const;
    void markForDestruction();
    void removeMarkedChildren();

    ScriptPtr getLuaState() const;
    bool loadScriptFile(const std::string& filepath);

    void setDebugInfo(bool debug);

    void setMask(Mask mask) { m_mask = mask; }
    constexpr Mask getMask() const { return m_mask; }
    constexpr bool isDebug() const { return m_debugFlag; }


private:
    void initCurrent();
    void initChildren();
    virtual void updateCurrent(sf::Time dt);
    void updateChildren(sf::Time dt);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawOrigin(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
    std::vector<UniPtr> m_children;

private:
    SceneNode* m_parent;
    Mask m_mask;
    bool m_debugFlag;
    bool m_markedForDestruction;
    const ScriptPtr m_script;
};

bool	collision(const SceneNode& lhs, const SceneNode& rhs);
float	distance(const SceneNode& lhs, const SceneNode& rhs);

NLOHMANN_JSON_SERIALIZE_ENUM(SceneNode::Mask, {
    { SceneNode::Mask::none, "none" }, 
    { SceneNode::Mask::Player, "Player" }, 
    { SceneNode::Mask::CollisionObject, "CollisionObject" }, 
    { SceneNode::Mask::NonPlayable, "NonPlayable" }, 
    { SceneNode::Mask::Enemy, "Enemy" }, 
    { SceneNode::Mask::AreaSwitch, "AreaSwitch" }, 
    { SceneNode::Mask::Background, "Background" }, 
    { SceneNode::Mask::Trigger, "Trigger" }, 
    { SceneNode::Mask::CollisionShape, "CollisionShape" }, 
})

#endif  // TEST_SCENENODE_HPP

