#include "helpergraphics.h"

#include <QBuffer>
#include <QIcon>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QSvgRenderer>
#include <QBitmap>
#include <QWidget>

QString getBase64Image(const QString& path, QSize size, const QString& imgtype, bool html)
{
    QIcon icon(path);
    auto p = icon.pixmap(size);
    return getBase64Image(&p, imgtype, html);
}

QString getBase64Image(QPixmap* pixmap, const QString& imgtype, bool html)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap->save(&buffer, imgtype.toUpper().toLatin1());
    return getBase64Image(&byteArray, imgtype, html);
}

QString getBase64Image(QByteArray* bytearray, const QString& imgtype, bool html)
{
    auto type = imgtype.toLower();
    type = type == "svg" ? type + "+xml" : type;
    type = type == "ico" ? "x-icon"      : type;
    QString stringBase64 = html ? QString("data:image/%1;base64,").arg(type) : "";
    stringBase64.append(bytearray->toBase64(QByteArray::Base64Encoding));
    return stringBase64;
}

QPixmap getPixmapFromBase64(const QString& img, bool* ok, int defsize)
{
    QPixmap pixmap;
    bool result = true;
    if(!pixmap.loadFromData(QByteArray::fromBase64(img.toLatin1())))
    {
        pixmap = QIcon(":/resources/img/error.svg").pixmap(defsize, defsize);
        result = false;
        qCritical() << __func__ << ": Error loading from Base64";
    }
    if(ok) *ok = result;
    return pixmap;
}

QColor GetContrastColor(const QColor &color)
{
    const int SATURATION_THRESHOLD = 51;
    int h = color. hslHue();
    int s = color.hslSaturation();
    int l = color.lightness();

    h = h + 180;
    if (h >= 360) h -= 360;
    if (s < SATURATION_THRESHOLD) l = l < 128 ? 255 : 0;

    return QColor::fromHsl(h, 255, l, color.alpha());
}
