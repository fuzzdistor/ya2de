#ifndef TEST_TEXTBOXNODE_HPP
#define TEST_TEXTBOXNODE_HPP

#include "LoggerCpp/Logger.h"
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
    
    void setDialogue(std::vector<std::string> string);
    std::vector<std::string> getDialogue() const;
    std::string getDialogueLine(size_t index) const;
    size_t getCurrentLineIndex() const;
    bool nextLine();

private:
    std::vector<std::string> m_dialogue;
    size_t m_currentLineIndex{ 0 };
    Log::Logger m_logger{ "TextboxNode" };
};

#endif //TEST_TEXTBOXNODE_HPP

