#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>

/*!
 * \brief Config - настройки программы с загрузкой из файла конфига и сохранением в файл.
 * Интерфейс над QSettings.
 */

class QSettings;

class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config(const QString& in_AppCatalog);
    void SaveWidgetSize(QWidget* widget);
    QSize GetWidgetSize(QWidget* widget, QSize def_value);
    void SaveWidgetPos(QWidget* widget);
    QPoint GetWidgetPos(QWidget* widget, QPoint def_value);

    int SplashTime() const;
    void setSplashTime(int value);
    int SplashSize() const;
    const QString &PathAppCatalog() const;
    const QString &PathAppConfig() const;
    const QString &PathLogsCatalog() const;
    const QString &LastCatalog() const;
    void setLastCatalog(const QString &value);
    void setPathLogsCatalog(const QString &value);
    int GUISize() const;
    void setGUISize(int value);
    const QString &DateTimeFormat() const;
    void setDateTimeFormat(const QString &value);
    bool SIMetric() const;
    void setSIMetric(bool value);
    bool ShowExitQuestion() const;
    void setShowExitQuestion(bool value);
    bool UseStyleSheet() const;
    void setUseStyleSheet(bool value);
    bool MainWindowOnCenter() const;
    void setMainWindowOnCenter(bool value);
    bool WriteLogsToFile() const;
    void setWriteLogsToFile(bool value);
    const QString &LogFileNameFormat() const;
    void setLogFileNameFormat(const QString &value);
    const QString &FontNameTextEditor() const;
    void setFontNameTextEditor(const QString &value);
    int FontPointSizeTextEditor() const;
    void setFontPointSizeTextEditor(int value);

protected:
    void load();

private:
    QSettings* m_Settings;

    // сохраняемые параметры
    QString m_DateTimeFormat;
    QString m_PathAppConfig;
    QString m_PathAppCatalog;
    QString m_PathLogsCatalog;
    QString m_LogFileNameFormat;
    QString m_LastCatalog;          // путь до последнего каталога
    QString m_FontNameTextEditor;
    int m_SplashTime;
    int m_SplashSize;
    int m_GUISize;
    bool m_SI_metric;
    bool m_ShowExitQuestion;
    bool m_UseStyleSheet;
    bool m_MainWindowOnCenter;
    bool m_WriteLogsToFile;
    int m_FontPointSizeTextEditor;
};

#endif // CONFIG_H
