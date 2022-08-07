#include "dialoginfocontent.h"
#include "properties.h"

#include "helpers/helper_common.h"

#include <QDebug>
#include <QApplication>
#include <QIcon>
#include <QToolBar>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QScrollBar>
#include <QRegularExpression>

DialogInfoContent::DialogInfoContent(QWidget *parent, const QString& title, const QString &icon)
    : DialogBody(parent, title, icon.isEmpty() ? ":/resources/img/info.svg" : icon)
{    
    setToolBarIconSize(config->GUISize());

    m_Content = new QTextBrowser(this);
    m_Content->setOpenLinks(false);
    QObject::connect(m_Content, &QTextBrowser::forwardAvailable, this, [=](bool value) { m_ActionForward->setEnabled(value); });
    QObject::connect(m_Content, &QTextBrowser::backwardAvailable, this, [=](bool value) { m_ActionBackward->setEnabled(value); });
    QObject::connect(m_Content, &QTextBrowser::anchorClicked, this, &DialogInfoContent::slotAnchorClicked);
    
    m_ActionBackward = new QAction(QIcon(":/resources/img/left_arrow.svg"), tr("Back"));
    QObject::connect(m_ActionBackward, &QAction::triggered, this, [=](){ m_Content->backward(); });
    m_ActionBackward->setAutoRepeat(false);
    m_ActionBackward->setEnabled(false);   
    
    m_ActionForward = new QAction(QIcon(":/resources/img/right_arrow.svg"), tr("Forward"));
    QObject::connect(m_ActionForward, &QAction::triggered, this, [=](){ m_Content->forward(); });
    m_ActionForward->setAutoRepeat(false);
    m_ActionForward->setEnabled(false);   
    
    auto m_ActionHome = new QAction(QIcon(":/resources/img/up_arrow.svg"), tr("Main page"));
    QObject::connect(m_ActionHome, &QAction::triggered, this, [=](){ m_Content->home(); });
    m_ActionHome->setAutoRepeat(false);

    addToolBarActionLeft(m_ActionForward, CSS_STYLE_BUTTON);
    addToolBarActionLeft(m_ActionBackward, CSS_STYLE_BUTTON);
    addToolBarActionLeft(m_ActionHome, CSS_STYLE_BUTTON);
    
    addDialogContent(m_Content);

    resize(WINDOW_SIZE);
    
    QObject::connect(this, &QObject::destroyed, [=]()
    { qDebug() << "DialogInfoContent" << windowTitle() << "destroyed"; });
    qDebug() << "DialogInfoContent" << windowTitle() << "created";
}

void DialogInfoContent::slotAnchorClicked(const QUrl &link)
{
    qDebug() << __func__;
    
    if(!link.isValid()) return;
    
    if(!link.toString().contains(QRegularExpression(REGEXP_EXTERN_URL)))
    {
        auto source_page = m_Content->source().toString();
        auto target_page = link.toString();
        
        qInfo() << "Source page:" << source_page;
        qInfo() << "Target page:"  << target_page;
        
        //if(source_page.endsWith(HELP_PAGE_TRIGGER)) config->setHelpPage(target_page);

        auto res_type = m_Content->sourceType();
        m_Content->setSource(link, res_type);
        return;
    }
    
    OpenUrl(link);
}

void DialogInfoContent::setHtmlContent(const QString& content)
{
    m_Content->setHtml(content);
}

void DialogInfoContent::setMarkdownSource(const QString &source)
{
    m_Content->setSource(QUrl(source), QTextDocument::MarkdownResource);
}

void DialogInfoContent::setMarkdownContent(const QString &content)
{
    m_Content->document()->setMarkdown(content, QTextDocument::MarkdownDialectCommonMark);
}

void DialogInfoContent::scrollUp()
{
  auto vScrollBar = m_Content->verticalScrollBar();
  vScrollBar->triggerAction(QScrollBar::SliderToMinimum);
}

