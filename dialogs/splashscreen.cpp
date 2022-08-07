#include "splashscreen.h"
#include "properties.h"

#include <QIcon>
#include <QLabel>

SplashScreen::SplashScreen(const QString& img, const QString& icon)
    : QSplashScreen()
{
    setPixmap(QPixmap(img).scaled(config->SplashSize(), config->SplashSize(),
                                  Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    setWindowIcon(QIcon(icon));
    setWindowModality(Qt::WindowModal);
    setWindowState(Qt::WindowActive);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    auto label = new QLabel(QString("%1\n%2").arg(APP_NAME, APP_VERSION), this);
    label->setMargin(5);
    label->setFrameStyle(QFrame::Box | QFrame::Raised);
    label->setFont(QFont("monospace", 20, QFont::Bold, true));
    label->setAutoFillBackground(true);
    label->move(0, 0);
}
