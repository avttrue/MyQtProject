#include "lineeditselectfile.h"
#include "properties.h"

#include "../helpers/helper_common.h"

#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

LineEditSelectFile::LineEditSelectFile(QWidget *parent,
                                       bool asCatalog,
                                       const QString &text,
                                       bool any)
    : QFrame(parent),
      m_AsCatalog(asCatalog),
      m_Any(any)
{
    setFrameStyle(QFrame::Plain);
    setMidLineWidth(0);

    auto hbl = new QHBoxLayout();
    hbl->setContentsMargins(1, 1, 1, 1);
    hbl->setSpacing(1);
    hbl->setAlignment(Qt::AlignLeft);
    setLayout(hbl);

    m_pb = new QPushButton;
    m_pb->setObjectName(CSS_STYLE_BUTTON);
    m_pb->setIconSize(QSize(config->GUISize(), config->GUISize()));
    m_pb->setIcon(QIcon(":/resources/img/folder_open.svg"));
    m_pb->setFocusPolicy(Qt::NoFocus);
    QObject::connect(m_pb, &QPushButton::clicked, this, &LineEditSelectFile::slotOpenFile);

    m_le = new QLineEdit(text);
    m_le->setReadOnly(true);
    m_le->setAlignment(Qt::AlignLeft);
    m_le->setCursorPosition(0);

    hbl->addWidget(m_le, -1, Qt::AlignVCenter);
    hbl->addWidget(m_pb, 0, Qt::AlignRight);
}

QString LineEditSelectFile::getText() { return m_le->text(); }

void LineEditSelectFile::slotOpenFile()
{
    QString filename;
    if(m_AsCatalog)
    {
        filename = QFileDialog::getExistingDirectory(this, tr("Выбрать каталог"),
                                                     config->LastCatalog(),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    }
    else
    {
        if(m_Any)
        {
            filename = QFileDialog::getSaveFileName(this, tr("Выбрать файл"),
                                                    config->LastCatalog(),
                                                    m_FileFilter);
        }
        else
        {
            filename = QFileDialog::getOpenFileName(this, tr("Выбрать файл"),
                                                    config->LastCatalog(),
                                                    m_FileFilter);
        }
    }

    if(filename.isNull() || filename.isEmpty()) return;

    m_le->setText(filename);

    Q_EMIT signalFileSelected(filename);
}

