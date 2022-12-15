#include "mdcommons.h"

MDCommons::MDCommons()
{
    m_MDSyntaxDict = *new QMap<QString,MDSyntax>();
    m_MDSyntaxDict.insert("Heading 1",*new MDSyntax(0,2,"# \r\n"));
    m_MDSyntaxDict.insert("Heading 2",*new MDSyntax(0,3,"## \r\n"));
    m_MDSyntaxDict.insert("Heading 3",*new MDSyntax(0,4,"### \r\n"));
    m_MDSyntaxDict.insert("Heading 4",*new MDSyntax(0,5,"#### \r\n"));
    m_MDSyntaxDict.insert("Heading 5",*new MDSyntax(0,6,"##### \r\n"));
    m_MDSyntaxDict.insert("Bold",*new MDSyntax(0,2,"****\r\n"));
    m_MDSyntaxDict.insert("Italic",*new MDSyntax(0,1,"**\r\n"));
    m_MDSyntaxDict.insert("Block quote",*new MDSyntax(0,2,"> \r\n"));
    m_MDSyntaxDict.insert("Ordered list",*new MDSyntax(0,3,"1. \r\n"));
    m_MDSyntaxDict.insert("Unordered list",*new MDSyntax(0,2,"- \r\n"));
    m_MDSyntaxDict.insert("Code block",*new MDSyntax(1,0,"```\r\n\r\n```\r\n\r\n"));
    m_MDSyntaxDict.insert("Horizontal rule",*new MDSyntax(0,3,"---\r\n"));
    m_MDSyntaxDict.insert("Link",*new MDSyntax(0,0,"[title](https://www.example.com)\r\n"));
    m_MDSyntaxDict.insert("Image",*new MDSyntax(0,0,"![alt text](image.jpg)\r\n"));
}

MDSyntax::MDSyntax(uint lineOffset,uint cursorOffset,QString syntaxText):m_CursorOffset(cursorOffset), m_LineOffset(lineOffset),m_SyntaxText(syntaxText)
{

}
