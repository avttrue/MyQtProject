#ifndef DIALOGTEXTEDIT_H
#define DIALOGTEXTEDIT_H

#include "dialogs/dialogbody.h"

class QPlainTextEdit;

class DialogTextEdit : public DialogBody
{
    Q_OBJECT
public:
    DialogTextEdit(QWidget *parent,
                   const QString &title,
                   const QString &icon,
                   QString* text);
    QPlainTextEdit *TextEdit() const;
    QAction *ActionAccept() const;

private:
    QAction* m_ActionAccept;
    QPlainTextEdit* m_TextEdit;

};

#endif // DIALOGTEXTEDIT_H
