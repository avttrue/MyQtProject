#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "config.h"

#include <QString>

extern Config* config;                              // интерфейс над QSettings

// дефолтные параметры
extern const QString APP_NAME;                      // название приложения
extern const QString APP_VERSION;                   // версия приложения
extern const QString APP_URL;                       // url приложения
extern const QString DT_FORMAT;                     // формат отображения даты и времени
extern const QString LOG_DIRECTORY;                 // имя каталога логов
extern const QString LOG_FILE_EXTENSION;            // расширение имени файлов логов
extern const QString LOG_FILE_NAME_FORMAT;          // формат имени файлов логов
extern const QString APP_CFG_FILE_NAME;             // путь до конфига приложения
extern const QString TEXT_CODEC;                    // общая кодировка текста
extern const QString CSS_MAIN;                      // название основной таблицы стилей
extern const QString CSS_STYLE_BUTTON;              // название стиля кнопок в таблице стилей
extern const QSize MAINWINDOW_SIZE;                 // размеры основного окна приложения
extern const QSize ABOUTWINDOW_SIZE;                // размеры окна диалога About
extern const QSize CONFIG_DIALOG_SIZE;              // размеры окна диалога настроек
extern const QString REGEXP_EXTERN_URL;             // регексп url
extern const QString HTML_LABEL_CAPTION_PATTERN;    // html паттерн для label заголовка
extern const QString HTML_LABEL_CAPTION_PATTERN_H;  // html паттерн для label заголовка крупного
extern const QString FONT_NAME_TEXTEDITOR;          // семейство шрифта редакторов текста

const int SPLASH_TIME =                    500;     // время отображения сплеш-заставки
const int SPLASH_SIZE =                    512;     // размер сплеш-заставки (квадрат)
const int GUI_SIZE =                       24;      // базовые размеры кнопок и элементов интерфейса
const int FONT_POINTSIZE_TEXTEDITOR =      12;      // размер шрифта редакторов текста
const bool SI_METRIC =                     false;   // использовать систему СИ единиц измерений
const bool SHOW_EXIT_QUESTION =            true;    // отображать перед завершением работы подтверждение
const bool USE_STYLE_SHEET =               true;    // использовать таблицу стилей
const bool MAIN_WINDOW_ONCENTER =          true;    // располагать основное окно по центру экрана
const bool WRITE_LOGS_TO_FILE =            false;   // писать логи в файл

#endif // PROPERTIES_H


