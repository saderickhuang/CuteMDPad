#ifndef SETTINGS_H
#define SETTINGS_H

#include <qfont.h>
#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings();
private:
    QFont m_Font;
    void ReadFromConfig();
};

#endif // SETTINGS_H
