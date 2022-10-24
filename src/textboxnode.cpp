#include <textboxnode.hpp>

TextboxNode::TextboxNode()
{
    setString("");

    auto luavm = getLuaState();

    // setting all kinds of APIs 
    luavm->set("textbox", this);
    luavm->new_usertype<TextboxNode>("Textbox"
            , "setDialogue"
            , &TextboxNode::setDialogue
            , "getDialogue"
            , &TextboxNode::getDialogue
            , "nextLine"
            , &TextboxNode::nextLine
            , "getDialogueLine"
            , &TextboxNode::getDialogueLine
            , "getCurrentLineIndex"
            , &TextboxNode::getCurrentLineIndex
            );
}

void TextboxNode::setDialogue(std::vector<std::string> string)
{
    m_dialogue = std::move(string);
    setString(getDialogueLine(m_currentLineIndex));
}

size_t TextboxNode::getCurrentLineIndex() const
{
    return m_currentLineIndex;
}

bool TextboxNode::nextLine()
{
    if (m_currentLineIndex + 1 >= m_dialogue.size())
    {
        return false;
    }
    else 
    {
        m_currentLineIndex++;
        return true;
    }
}

std::vector<std::string> TextboxNode::getDialogue() const
{
    return m_dialogue;
}

std::string TextboxNode::getDialogueLine(size_t index) const
{
    if (index >= m_dialogue.size())
    {
        m_logger.warning() << "Requested dialogue index out bounds! Max index of entries in m_dialogue: " << m_dialogue.size() - 1 << ". Requested index: " << index;
        return m_dialogue.back();
    }
    return m_dialogue.at(index);
}

