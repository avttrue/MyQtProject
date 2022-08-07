#include "config.h"
#include "properties.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <QSettings>
#include <QApplication>
#include <QWidget>

Config::Config(const QString& in_AppDirectory):
    m_Settings(nullptr)
{
    m_PathAppDir = in_AppDirectory;
    m_PathAppConfig = m_PathAppDir + QDir::separator() + APP_CFG_FILE_NAME;

    qInfo() << "AppConfig:" << m_PathAppConfig;

    m_Settings = new QSettings(m_PathAppConfig, QSettings::IniFormat);

    load();

    QObject::connect(this, &QObject::destroyed, this, [=]() { qInfo() << "Config destroyed"; });
    qInfo() << "Config created";
}

void Config::SaveWidgetSize(QWidget* widget)
{
    auto name = widget->objectName();
    name.remove(' ');

    if(name.isEmpty())
    {
        qCritical() << __func__ << "empty widget name";
        return;
    }

    auto size = widget->size();
    auto s_width = QString("%1/Width").arg(name);
    auto s_height = QString("%1/Height").arg(name);

    m_Settings->setValue(s_width, size.width());
    m_Settings->setValue(s_height, size.height());
}

QSize Config::GetWidgetSize(QWidget *widget, QSize def_value)
{
    auto name = widget->objectName();
    name.remove(' ');
    auto value = def_value;

    if(name.isEmpty())
    {
        qCritical() << __func__ << "empty widget name";
        return value;
    }

    auto s_width = QString("%1/Width").arg(name);
    auto s_height = QString("%1/Height").arg(name);

    if(!m_Settings->contains(s_width))
        m_Settings->setValue(s_width, value.width());

    if(!m_Settings->contains(s_height))
        m_Settings->setValue(s_height, value.height());

    value = QSize(m_Settings->value(s_width).toInt(),
                  m_Settings->value(s_height).toInt());

    return value;
}

void Config::SaveWidgetPos(QWidget *widget)
{
    auto name = widget->objectName();
    name.remove(' ');

    if(name.isEmpty())
    {
        qCritical() << __func__ << "empty widget name";
        return;
    }

    auto pos = widget->pos();
    auto s_x = QString("%1/Position_X").arg(name);
    auto s_y = QString("%1/Position_Y").arg(name);

    m_Settings->setValue(s_x, pos.x());
    m_Settings->setValue(s_y, pos.y());
}

QPoint Config::GetWidgetPos(QWidget *widget, QPoint def_value)
{
    auto name = widget->objectName();
    name.remove(' ');
    auto value = def_value;

    if(name.isEmpty())
    {
        qCritical() << __func__ << "empty widget name";
        return value;
    }

    auto s_x = QString("%1/Position_X").arg(name);
    auto s_y = QString("%1/Position_Y").arg(name);

    if(!m_Settings->contains(s_x))
        m_Settings->setValue(s_x, value.x());

    if(!m_Settings->contains(s_y))
        m_Settings->setValue(s_y, value.y());

    value = QPoint(m_Settings->value(s_x).toInt(),
                  m_Settings->value(s_y).toInt());

    return value;
}

void Config::load()
{
    if(!m_Settings->contains("LastCatalog"))
        m_Settings->setValue("LastCatalog", m_PathAppDir);
    m_LastCatalog = m_Settings->value("LastCatalog").toString();
    qInfo() << "LastCatalog:" << m_LastCatalog;

    if(!m_Settings->contains("WriteLogsToFile"))
        m_Settings->setValue("WriteLogsToFile", WRITE_LOGS_TO_FILE);
    m_WriteLogsToFile = m_Settings->value("WriteLogsToFile").toBool();
    qInfo() << "WriteLogsToFile:" << m_WriteLogsToFile;

    if(!m_Settings->contains("SIMetric"))
        m_Settings->setValue("SIMetric", SI_METRIC);
    m_SI_metric = m_Settings->value("SIMetric").toBool();
    qInfo() << "SI_metric:" << m_SI_metric;

    if(!m_Settings->contains("UseStyleSheet"))
        m_Settings->setValue("UseStyleSheet", USE_STYLE_SHEET);
    m_UseStyleSheet = m_Settings->value("UseStyleSheet").toBool();
    qInfo() << "UseStyleSheet:" << m_UseStyleSheet;

    if(!m_Settings->contains("SplashWindow/Time"))
        m_Settings->setValue("SplashWindow/Time", SPLASH_TIME);
    m_SplashTime = m_Settings->value("SplashWindow/Time").toInt();
    qInfo() << "SplashWindow/Time:" << m_SplashTime;

    if(!m_Settings->contains("SplashWindow/Size"))
        m_Settings->setValue("SplashWindow/Size", SPLASH_SIZE);
    m_SplashSize = m_Settings->value("SplashWindow/Size").toInt();
    qInfo() << "SplashWindow/Size:" << m_SplashSize;

    if(!m_Settings->contains("ShowExitQuestion"))
        m_Settings->setValue("ShowExitQuestion", SHOW_EXIT_QUESTION);
    m_ShowExitQuestion = m_Settings->value("ShowExitQuestion").toBool();
    qInfo() << "ShowExitQuestion:" << m_ShowExitQuestion;

    if(!m_Settings->contains("DateTimeFormat"))
        m_Settings->setValue("DateTimeFormat", DT_FORMAT);
    m_DateTimeFormat = m_Settings->value("DateTimeFormat").toString();
    qInfo() << "DateTimeFormat:" << m_DateTimeFormat;

    if(!m_Settings->contains("MainWindow/OnCenter"))
        m_Settings->setValue("MainWindow/OnCenter", MAIN_WINDOW_ONCENTER);
    m_MainWindowOnCenter = m_Settings->value("MainWindow/OnCenter").toBool();
    qInfo() << "MainWindow/OnCenter:" << m_MainWindowOnCenter;

    if(!m_Settings->contains("LogFileNameFormat"))
        m_Settings->setValue("LogFileNameFormat", LOG_FILE_NAME_FORMAT);
    m_LogFileNameFormat = m_Settings->value("LogFileNameFormat").toString();
    qInfo() << "LogFileNameFormat:" << m_LogFileNameFormat;

    if(!m_Settings->contains("GUISize"))
        m_Settings->setValue("GUISize", GUI_SIZE);
    m_GUISize = m_Settings->value("GUISize").toInt();
    qInfo() << "GUISize:" << m_GUISize;

    if(!m_Settings->contains("FontNameTextEditor"))
        m_Settings->setValue("FontNameTextEditor", FONT_NAME_TEXTEDITOR);
    m_FontNameTextEditor = m_Settings->value("FontNameTextEditor").toString();
    qInfo() << "FontNameTextEditor:" << m_FontNameTextEditor;

    if(!m_Settings->contains("FontPointSizeTextEditor"))
        m_Settings->setValue("FontPointSizeTextEditor", FONT_POINTSIZE_TEXTEDITOR);
    m_FontPointSizeTextEditor = m_Settings->value("FontPointSizeTextEditor").toInt();
    qInfo() << "FontPointSizeTextEditor:" << m_FontPointSizeTextEditor;
}

void Config::setLogFileNameFormat(const QString &value)
{
    if(m_LogFileNameFormat == value) return;

    m_LogFileNameFormat = value;
    m_Settings->setValue("LogFileNameFormat", m_LogFileNameFormat);
}

void Config::setWriteLogsToFile(bool value)
{
    if(m_WriteLogsToFile == value) return;

    m_WriteLogsToFile = value;
    m_Settings->setValue("WriteLogsToFile", m_WriteLogsToFile);
}

void Config::setPathLogsDir(const QString &value)
{
    if(m_PathLogsDir == value) return;

    m_PathLogsDir = value;
    m_Settings->setValue("PathLogsDir", m_PathLogsDir);
}

void Config::setMainWindowOnCenter(bool value)
{
    if(m_MainWindowOnCenter == value) return;

    m_MainWindowOnCenter = value;
    m_Settings->setValue("MainWindow/OnCenter", m_MainWindowOnCenter);
}

void Config::setSplashTime(int value)
{
    if(m_SplashTime == value) return;

    m_SplashTime = value;
    m_Settings->setValue("SplashWindow/Time", m_SplashTime);
}

void Config::setUseStyleSheet(bool value)
{
    if(m_UseStyleSheet == value) return;

    m_UseStyleSheet = value;
    m_Settings->setValue("UseStyleSheet", m_UseStyleSheet);
}

void Config::setShowExitQuestion(bool value)
{
    if(m_ShowExitQuestion == value) return;

    m_ShowExitQuestion = value;
    m_Settings->setValue("ShowExitQuestion", m_ShowExitQuestion);
}

void Config::setSIMetric(bool value)
{
    if(m_SI_metric == value) return;

    m_SI_metric = value;
    m_Settings->setValue("SIMetric", m_SI_metric);
}

void Config::setDateTimeFormat(const QString &value)
{
    if(m_DateTimeFormat == value) return;

    m_DateTimeFormat = value;
    m_Settings->setValue("DateTimeFormat", m_DateTimeFormat);
}

void Config::setGUISize(int value)
{
    if(m_GUISize == value) return;

    m_GUISize = value;
    m_Settings->setValue("GUISize", m_GUISize);
}

void Config::setLastCatalog(const QString &value)
{
    if(m_LastCatalog == value) return;

    m_LastCatalog = value;
    m_Settings->setValue("LastCatalog", m_LastCatalog);
}

void Config::setFontNameTextEditor(const QString &value)
{
    if(m_FontNameTextEditor == value) return;

    m_FontNameTextEditor = value;
    m_Settings->setValue("FontNameTextEditor", m_FontNameTextEditor);
}

void Config::setFontPointSizeTextEditor(int value)
{
    if(m_FontPointSizeTextEditor == value) return;

    m_FontPointSizeTextEditor = value;
    m_Settings->setValue("FontPointSizeTextEditor", m_FontPointSizeTextEditor);
}

bool Config::SIMetric() const { return m_SI_metric; }
const QString &Config::PathLogsDir() const { return m_PathLogsDir; }
const QString &Config::DateTimeFormat() const { return m_DateTimeFormat; }
int Config::GUISize() const { return m_GUISize; }
int Config::SplashTime() const { return m_SplashTime; }
int Config::SplashSize() const { return m_SplashSize; }
const QString &Config::PathAppDir() const { return m_PathAppDir; }
const QString &Config::PathAppConfig() const { return m_PathAppConfig; }
bool Config::ShowExitQuestion() const { return m_ShowExitQuestion; }
bool Config::UseStyleSheet() const { return m_UseStyleSheet; }
bool Config::MainWindowOnCenter() const { return m_MainWindowOnCenter; }
bool Config::WriteLogsToFile() const { return m_WriteLogsToFile; }
const QString &Config::LogFileNameFormat() const { return m_LogFileNameFormat; }
const QString &Config::LastCatalog() const { return m_LastCatalog; }
int Config::FontPointSizeTextEditor() const { return m_FontPointSizeTextEditor; }
const QString &Config::FontNameTextEditor() const { return m_FontNameTextEditor; }
