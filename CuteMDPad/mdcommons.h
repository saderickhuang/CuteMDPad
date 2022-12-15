#ifndef MDCOMMONS_H
#define MDCOMMONS_H
#include <QString>
#include <QList>
#include <QMap>
enum class MDElement {Heading1,Heading2,Heading3,Heading4,Heading5,
                     Bold,Italic,Blockquote,OrderedList,UnorderedList,
                     CodeBlock,HorizontalRule,Link,Image};
class MDSyntax
{
public :
    MDSyntax(uint lineOffset,uint cursorOffset, QString syntaxText);
    /**
     * @brief m_CursorOffset
     * indicate how cursor move to position of markdown syntax content
     * eg.: in bold (**bold text**) would be 2.
     */
    uint m_CursorOffset;
    uint m_LineOffset;
    QString m_SyntaxText;
};
/**
 * @brief The MDCommons class
 *
 */
class MDCommons
{
public:
    QMap<QString,MDSyntax> m_MDSyntaxDict;
    MDCommons();
};
#endif // MDCOMMONS_H
