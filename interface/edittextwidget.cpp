#include "edittextwidget.h"
#include "properties.h"

#include "../dialogs/dialogtextedit.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>

EditTextWidget::EditTextWidget(QWidget *parent,
                                       bool asCatalog,
                                       const QString &text)
    : QFrame(parent),
      m_AsCatalog(asCatalog)
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
    m_pb->setIcon(QIcon(":/resources/img/edit.svg"));
    m_pb->setFocusPolicy(Qt::NoFocus);
    QObject::connect(m_pb, &QPushButton::clicked, this, &EditTextWidget::slotTextEdit);

    m_te = new QTextEdit();
    m_te->setPlainText(text);
    m_te->setReadOnly(true);
    m_te->setFixedHeight(config->GUISize());
    m_te->setAlignment(Qt::AlignLeft);
    m_te->setUndoRedoEnabled(false);
    m_te->setWordWrapMode(QTextOption::NoWrap);
    m_te->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_te->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    hbl->addWidget(m_te, -1, Qt::AlignVCenter);
    hbl->addWidget(m_pb, 0, Qt::AlignRight);
}

QString EditTextWidget::getText() { return m_te->toPlainText(); }

void EditTextWidget::slotTextEdit()
{
    auto text = m_te->toPlainText();
    auto dte = new DialogTextEdit(this, tr("Edit text"), ":/resources/img/edit.svg",  &text);
    dte->setObjectName("ConfigDialogTextEdit");
    dte->resize(config->GetWidgetSize(dte, TEXT_EDIT_SIZE));
    QObject::connect(dte, &DialogBody::signalSizeChanged, this, [=]() { config->SaveWidgetSize(dte); });
    if(!dte->exec()) return;

    qInfo() << __func__ << ": Accepted";
    m_te->setPlainText(text);

    emit signalTextEdited(text);
}

