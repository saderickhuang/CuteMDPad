#include "webviewcontent.h"


void WebViewContent::SetText(const QString & text)
{
    if (text == this->m_Text) return;

    this->m_Text = text;
    emit this->TextChanged(this->m_Text);
}


