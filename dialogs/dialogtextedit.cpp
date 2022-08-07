#include "dialogtextedit.h"
#include "properties.h"

#include <QDebug>
#include <QApplication>
#include <QIcon>
#include <QToolBar>
#include <QPlainTextEdit>

DialogTextEdit::DialogTextEdit(QWidget *parent,
                               const QString& title,
                               const QString& icon,
                               QString *text)
    : DialogBody(parent, title, icon)
{
    setToolBarIconSize(config->GUISize());

    m_TextEdit = new QPlainTextEdit(this);
    m_TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_TextEdit->setReadOnly(false);
    m_TextEdit->setUndoRedoEnabled(false);
    m_TextEdit->setPlainText(*text);
    QFont font;
    font.setFamily(config->FontNameTextEditor());
    font.setFixedPitch(true);
    font.setPointSize(config->FontPointSizeTextEditor());
    font.setBold(true);
    m_TextEdit->setFont(font);
    QObject::connect(m_TextEdit, &QPlainTextEdit::textChanged, this, [=](){ m_ActionAccept->setEnabled(true); });

    m_ActionAccept = new QAction(QIcon(":/resources/img/yes.svg"), tr("Accept"));
    m_ActionAccept->setAutoRepeat(false);
    m_ActionAccept->setDisabled(true);
    QObject::connect(m_ActionAccept, &QAction::triggered, this, [=]()
    {
        *text = m_TextEdit->toPlainText();
        accept();
    });

    addToolBarActionRight(m_ActionAccept, CSS_STYLE_BUTTON);

    addDialogContent(m_TextEdit);

    QObject::connect(this, &QObject::destroyed, this, [=]()
    { qInfo() << "DialogTextEdit" << windowTitle() << "destroyed"; });
    qInfo() << "DialogTextEdit" << windowTitle() << "created";
}

QPlainTextEdit *DialogTextEdit::TextEdit() const
{
    return m_TextEdit;
}

QAction *DialogTextEdit::ActionAccept() const
{
    return m_ActionAccept;
}

