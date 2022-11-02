#include <sol/property.hpp>
#include <sol/types.hpp>
#include <textboxnode.hpp>

TextboxNode::TextboxNode()
{
}

bool TextboxNode::nextLine()
{
    // is the current line index not the last dialogue line?
    if (m_currentLineIndex != m_dialogueLines.size() - 1)
    {
        m_currentLineIndex++;
        return true;
    }

    return false;
}

void TextboxNode::setLuaUsertype()
{
    auto usertype = getLuaState().new_usertype<TextboxNode>("TextboxNode"
            , sol::base_classes, sol::bases<SceneNode, TextNode>());

    usertype["dialogue"] = &TextboxNode::m_dialogueLines;
    usertype["box_width"] = &TextboxNode::m_textboxWidth;
    usertype["nextLine"] = &TextboxNode::nextLine;
    usertype["getCurrentLineIndex"] = [](const TextboxNode& node)
        { return node.m_currentLineIndex + 1; }; // get a 1-based index result
    usertype["getDialogueLine"] = [](const TextboxNode& node, size_t index)
        { return node.getDialogueLine(index-1); }; // get a 1-based index result

    TextNode::setLuaUsertype();
}

std::string TextboxNode::getDialogueLine(size_t index) const
{
    if (index >= m_dialogueLines.size())
    {
        m_logger.warning() << "Requested dialogue index out bounds! Max index of entries in m_dialogue: " << m_dialogueLines.size() - 1 << ". Requested index: " << index;
        return m_dialogueLines.back();
    }
    return m_dialogueLines.at(index);
}

