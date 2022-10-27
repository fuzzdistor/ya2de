#ifndef TEST_TEXTBOXNODE_HPP
#define TEST_TEXTBOXNODE_HPP

#include "LoggerCpp/Logger.h"
#include "nodefactories.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <textnode.hpp>
#include <scenenode.hpp>


class TextboxNode : public TextNode
{
public:
    TextboxNode();
    
    std::string getDialogueLine(size_t index) const;
    bool nextLine();

protected:
    void setLuaUsertype() override;

private:
    float m_textboxWidth{ 0 };
    std::vector<std::string> m_dialogueLines {};
    size_t m_currentLineIndex{ 0 };
    Log::Logger m_logger{ "TextboxNode" };

    friend class NodeFactories;
};

#endif //TEST_TEXTBOXNODE_HPP

