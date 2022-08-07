#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>

class SplashScreen : public QSplashScreen
{
public:
    SplashScreen(const QString &img, const QString &icon);
};

#endif // SPLASHSCREEN_H
