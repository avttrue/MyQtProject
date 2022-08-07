#include "mainwindow.h"
#include "properties.h"

#include "dialogs/splashscreen.h"
#include "helpers/helper_common.h"

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include <QTimer>

// подготовка обработчика консоли
static QScopedPointer<QFile> m_logFile;
void consoleOut(QtMsgType type, const QMessageLogContext &context, const QString &msg);

// создание каталогов
void createDirectories()
{
    // каталог логов
    config->setPathLogsDir(config->PathAppDir() + QDir::separator() + LOG_DIRECTORY);
    if(!CreateDir(config->PathLogsDir()))
        config->setPathLogsDir(config->PathAppDir());
}

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));
    config = new Config(application.applicationDirPath());
    createDirectories();

    if(config->WriteLogsToFile())
    {
        auto name = QDateTime::currentDateTime().toString(config->LogFileNameFormat()).append('.');
        auto file = config->PathLogsDir() + QDir::separator() + name + LOG_FILE_EXTENSION.toLower();
        m_logFile.reset(new QFile(file));
        m_logFile.data()->open(QFile::Append | QFile::Text);
        qInstallMessageHandler(consoleOut);
    }

    qInfo() << "App version:" << APP_VERSION;
    qInfo() << "Build date:" << __DATE__ << __TIME__;
    qInfo() << "System info:" << getSystemInfo();
    qInfo() << "Qt version:" << QT_VERSION_STR;

    qInfo() << "PathAppDirectory:" << config->PathAppDir();
    qInfo() << "PathAppConfig:" << config->PathAppConfig();
    qInfo() << "PathLogsDirectory:" << config->PathLogsDir();

    if(config->UseStyleSheet())
        application.setStyleSheet(getTextFromRes(CSS_MAIN));

    SplashScreen splash(":/resources/img/logo.svg", ":/resources/img/mainicon.svg");
    splash.show();

    MainWindow window;

    QEventLoop loop;
    QTimer::singleShot(config->SplashTime(), &application, [&application, &window, &splash, &loop]()
    {
        splash.finish(&window);
        window.show();
        application.setActiveWindow(&window);
        loop.quit();
    });
    loop.exec(QEventLoop::ExcludeUserInputEvents);


    QObject::connect(&application, &QApplication::destroyed, [=](){ qInfo() << "Good bye!"; });
    return application.exec();
}

// обработчик консоли
void consoleOut(QtMsgType msgtype, const QMessageLogContext &context, const QString &msg)
{
    QTextStream log_out(m_logFile.data());
    log_out.setEncoding(QStringConverter::Utf8);

    auto dtformat = config->DateTimeFormat();
    auto time = QDateTime::currentDateTime().toString(dtformat);
    auto ctgr = QString(context.category);
    QString finf("");
    QString type("");

    if(ctgr == "EVENT")
    {
        type = "[MSG]";
        finf = "EVENT";
    }
    else
    {
        finf = QFileInfo(context.file).fileName() + ":" + QString::number(context.line);
        switch (msgtype)
        {
        case QtDebugMsg:
        { type = "[DBG]"; break; }
        case QtInfoMsg:
        { type = "[INF]"; break; }
        case QtWarningMsg:
        { type = "[WRN]"; break; }
        case QtCriticalMsg:
        { type = "[CRT]"; break; }
        case QtFatalMsg:
        { type = "[FTL]"; break; }
        }
    }

    log_out << time << "\t" << type << "\t" << finf << "\t" << msg << Qt::endl;
    log_out.flush();
}
