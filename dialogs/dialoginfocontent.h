#ifndef DIALOGINFOCONTENT_H
#define DIALOGINFOCONTENT_H

#include "dialogbody.h"

class QTextBrowser;

class DialogInfoContent : public DialogBody
{
    Q_OBJECT
public:
    DialogInfoContent(QWidget *parent, const QString &title, const QString &icon = "");
    void setHtmlContent(const QString& content);
    void setMarkdownSource(const QString& source);
    void setMarkdownContent(const QString& content);
    void scrollUp();

private:
    QTextBrowser*   m_Content;
    QAction*        m_ActionBackward;
    QAction*        m_ActionForward;

    // const
    const QSize WINDOW_SIZE = QSize(500, 500);

private Q_SLOTS:
    void slotAnchorClicked(const QUrl &link);

};

#endif // DIALOGINFOCONTENT_H
