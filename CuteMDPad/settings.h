#ifndef SETTINGS_H
#define SETTINGS_H

#include <qfont.h>

class Settings
{
public:
    Settings();
private:
    QFont m_Font;
    void ReadFromConfig();
};

#endif // SETTINGS_H
