#ifndef HELPER_COMMON_H
#define HELPER_COMMON_H

#include <QUrl>

class QObject;

/*!
 * \brief getTextFromRes - получить текстовый файл из ресурсов приложения
 */
QString getTextFromRes(const QString& path);

/*!
 * \brief fileToText - считать файл как текст.
 */
QString FileToText(const QString& path, QString *error = nullptr);

/*!
 * \brief DataToFile - записать данные в файл
 */
bool DataToFile(QByteArray* data,
                const QString& path,
                QString* error);

bool TextToUtf8File(const QString& text,
                    const QString& path,
                    QString* error);


/*!
 * \brief longTimeToString - перевести время в формате long (MSecsSinceEpoch) в строку в формате format.
 */
QString longTimeToString(qlonglong datetime, const QString& format);

/*!
 * \brief humanReadableByteCount - перевести количество байт в удобный формат.
 * \param bytes - количество байт
 * \param si - SI или BINARY формат
 * \return количество байт в виде строки
 */
QString humanReadableByteCount(long long bytes, bool si = true);

QString getSystemInfo();

bool CreateDir(const QString& path);

bool OpenUrl(const QString& path);

bool OpenUrl(const QUrl& url);

QString BuildDateTime();

#endif // HELPER_COMMON_H
