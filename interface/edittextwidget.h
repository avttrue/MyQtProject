#ifndef EDITTEXTWIDGET_H
#define EDITTEXTWIDGET_H

#include <QFrame>

class QTextEdit;
class QPushButton;

class EditTextWidget : public QFrame
{
    Q_OBJECT
public:
    explicit EditTextWidget(QWidget *parent,
                                bool asCatalog,
                                const QString& text = "");
    QTextEdit* TextEdit() { return m_te; }
    QString getText();
    void setFileFilter(const QString& text) {m_FileFilter = text; };

private:
    QSize TEXT_EDIT_SIZE = QSize(500, 300);

    QTextEdit*      m_te;
    QPushButton*    m_pb;
    QString         m_FileFilter;
    bool            m_AsCatalog;

private Q_SLOTS:
    void slotTextEdit();

Q_SIGNALS:
    void signalTextEdited(const QString& text);

};

#endif // EDITTEXTWIDGET_H
