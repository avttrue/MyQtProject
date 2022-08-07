#ifndef HELPERGRAPHICS_H
#define HELPERGRAPHICS_H

#include <QPixmap>

/*!
 * \brief getBase64Image - получить изображение в формате Base64
 * \param path - путь до ресурса
 * \param size - размер изображения
 * \param imgtype - формат изображения
 * \param html - добавить префикс для использования в html документе
 */
QString getBase64Image(const QString& path, QSize size, const QString& imgtype, bool html = false);
QString getBase64Image(QPixmap* pixmap, const QString& imgtype, bool html = false);
QString getBase64Image(QByteArray* bytearray, const QString& imgtype, bool html = false);

/*!
 * \brief getPixmapFromBase64 - получить Pixmap из Base64
 * \param img - строка Base64
 * \param ok - наличие ошибок
 * \param defsize - размер картинки-заглушки в случае ошибки
 */
QPixmap getPixmapFromBase64(const QString& img, bool *ok = nullptr, int defsize = 64);

/*!
 * \brief GetContrastColor - возвращает контрастный (инвертированный) цвет
 */
QColor GetContrastColor(const QColor& color);

#endif // HELPERGRAPHICS_H
