#include "helper_common.h"

#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QtMath>
#include <QMetaProperty>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>
#include <QSaveFile>

QString getTextFromRes(const QString& path)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        QString text(file.readAll());
        return text;
    }
    qCritical() << __func__ << ": Resurce not found:" << path;
    return "";
}

QString longTimeToString(qlonglong datetime, const QString& format)
{
    auto dt = QDateTime::fromMSecsSinceEpoch(datetime);
    return dt.toString(format);
}

QString humanReadableByteCount(long long bytes, bool si)
{
    int unit = si ? 1000 : 1024;
    if (bytes < unit)
        return QString("%1 B").arg(QString::number(bytes));

    int exp = static_cast<int>(qLn(bytes) / qLn(unit));
    QString pre = si ? "kMGTPE" : "KMGTPE";

    return QString("%1 %2%3B").
            arg(bytes / qPow(unit, exp), 0, 'f', 1, '0').
            arg(pre[exp - 1]).
            arg(si ? "" : "i");
}

QString FileToText(const QString& path, QString *error)
{
    QString text;
    QFile file(path);

    if (!file.exists())
    {
        *error = QObject::tr("File not found: '%1'").arg(path);
        return "";
    }

    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        *error = QObject::tr("File not opened for reading: '%1'").arg(path);
        return "";
    }

    QTextStream stream(&file);
    text = stream.readAll();
    file.close();
    return text;
}

QString getSystemInfo()
{
    QString result;

    result.append(QSysInfo::currentCpuArchitecture()).append(" ").
            append(QSysInfo::kernelType()).append(" ").
            append(QSysInfo::kernelVersion()).append(" ").
            append(QSysInfo::productType()).append(" ").
            append(QSysInfo::productVersion()).append(" ").
            append(QSysInfo::prettyProductName());

    return result;
}

bool CreateDir(const QString &path)
{
    if(!QDir().exists(path) && !QDir().mkpath(path))
    {
        qCritical() << __func__ << ": Catalog not exist and cannot be created:" << path;
        return false;
    }

    auto p = QFile(path).permissions();
    if(!QFile::setPermissions(path, p |
                              QFileDevice::ReadOwner |
                              QFileDevice::WriteOwner))
    {
        qCritical() << __func__ << ": Cannot set permissions to catalog:" << path;
        return false;
    }

    qInfo() << "Catalog" << path << "ready";
    return true;
}

bool OpenUrl(const QString &path)
{
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(path)))
    {
        qCritical() << __func__ << ": Error at QDesktopServices::openUrl:" << path;
        return false;
    }
    return true;
}

bool OpenUrl(const QUrl &url)
{
    if (!QDesktopServices::openUrl(url))
    {
        qCritical() << __func__ << ": Error at QDesktopServices::openUrl:" << url.toString();
        return false;
    }
    return true;
}

bool DataToFile(QByteArray *data,
                const QString &path,
                QString *error)
{
    QSaveFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        *error = QString("%1 '%2'").arg(QObject::tr("File cannot be opened for writing:"), path);
        return false;
    }

    if(file.write(*data) < 0)
    {
        *error = QString("%1 '%2'").arg(QObject::tr("Error at file writing:"), path);
        return false;
    }

    if(!file.commit())
    {
        *error = QString("%1 '%2'").arg(QObject::tr("Error at file commiting:"), path);
        return false;
    }
    return true;
}

bool TextToUtf8File(const QString &text, const QString &path, QString *error)
{
    QSaveFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        *error = QString("%1 '%2'").arg(QObject::tr("File cannot be opened for writing:"), path);
        return false;
    }

    if(file.write(text.toUtf8()) < 0)
    {
        *error = QString("%1 '%2'").arg(QObject::tr("Error at file writing:"), path);
        return false;
    }

    if(!file.commit())
    {
        *error = QString("%1 '%2'").arg(QObject::tr("Error at file commiting:"), path);
        return false;
    }
    return true;
}

QString BuildDateTime()
{
    QString ds = __DATE__;
    QString ts = __TIME__;

    return QString("%1 %2").arg(ds.simplified(), ts.simplified());
}
