#ifndef WEBVIEWCONTENT_H
#define WEBVIEWCONTENT_H

#include <QObject>

class WebViewContent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER m_Text NOTIFY TextChanged FINAL)
public:
    explicit WebViewContent(QObject *parent = nullptr) : QObject(parent) {}

    void SetText(const QString & text);
signals:
    void TextChanged(const QString & text);
private:
    QString m_Text;
};

#endif // WEBVIEWCONTENT_H
