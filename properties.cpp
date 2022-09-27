#include "properties.h"

#include <QSize>

Config* config = nullptr;

const QString APP_NAME =                     QStringLiteral("Project");
const QString APP_VERSION =                  QStringLiteral("0.1");
const QString APP_URL =                      QStringLiteral("https://disk.yandex.ru/d/aFImbZ4soYDKUw");
const QString DT_FORMAT =                    QStringLiteral("yyyy.MM.dd hh:mm:ss");
const QString LOG_FILE_NAME_FORMAT =         QStringLiteral("yyyyMMdd_hhmmss");
const QString TEXT_CODEC =                   QStringLiteral("UTF-8");
const QString APP_CFG_FILE_NAME =            QStringLiteral("settings.cfg");
const QString LOG_DIRECTORY =                QStringLiteral("logs");
const QString LOG_FILE_EXTENSION =           QStringLiteral("log");
const QString CSS_MAIN =                     QStringLiteral(":/resources/css/main.css");
const QString CSS_STYLE_BUTTON =             QStringLiteral("CssButton");
const QString FONT_NAME_TEXTEDITOR =         QStringLiteral("Courier");
const QSize MAINWINDOW_SIZE =                QSize(800, 700);
const QSize ABOUTWINDOW_SIZE =               QSize(500, 500);
const QSize CONFIG_DIALOG_SIZE =             QSize(400, 600);

const QString REGEXP_EXTERN_URL =            QStringLiteral("https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]"
                                                            "{1,256}\\.[a-zA-Z0-9()]{1,6}\\b"
                                                            "([-a-zA-Z0-9()@:%_\\+.~#?&//=]*)");

const QString HTML_LABEL_CAPTION_PATTERN =   QStringLiteral("<center><b><font size='+1'> %1 </font></b></center>");
const QString HTML_LABEL_CAPTION_PATTERN_H = QStringLiteral("<center><b><font size='+2'> %1 </font></b></center>");
