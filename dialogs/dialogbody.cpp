#include "dialogbody.h"
#include "properties.h"
#include "../interface/separators.h"
#include "../helpers/widgethelper.h"

#include <QCoreApplication>
#include <QGridLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QSizeGrip>
#include <QToolBar>

DialogBody::DialogBody(QWidget* parent,
                       const QString& text,
                       const QString& icon,
                       bool closable,
                       bool modal,
                       bool ontop)
    : QDialog(parent)
{
    Qt::WindowFlags flags;
    flags = Qt::Dialog | Qt::FramelessWindowHint;
    if(ontop) flags |= Qt::WindowStaysOnTopHint;
    setWindowFlags(flags);

    setAttribute(Qt::WA_DeleteOnClose, true);
    setModal(modal);
    setStyleSheet(WINDOW_STYLE.
                  arg(QPalette().color(QPalette::Shadow).name(QColor::HexRgb)));

    // основной контейнер
    auto glMainContainer = new QGridLayout(this);
    glMainContainer->setAlignment(Qt::AlignAbsolute);
    glMainContainer->setContentsMargins(2, 2, 2, 2);
    glMainContainer->setSpacing(0);

    // заголовок
    auto frameCaption = new QFrame();
    frameCaption->setAutoFillBackground(true);
    frameCaption->setFrameStyle(QFrame::Raised | QFrame::Panel);
    frameCaption->setLineWidth(1);
    auto layoutCaption = new QHBoxLayout();
    layoutCaption->setContentsMargins(1, 1, 1, 1);
    layoutCaption->setSpacing(2);

    m_Caption = new DialogCaption(text);

    auto font_height = QFontMetrics(m_Caption->font()).height();
    QSize caption_size = QSize(font_height + font_height / 2,
                               font_height + font_height / 2);
    m_Caption->setFixedHeight(caption_size.height());

    setWindowTitle(text);

    if(!icon.isEmpty())
    {
        auto labelIcon = new QLabel();
        labelIcon->setStyleSheet(CAPTION_ICON_STYLE.
                                 arg(QPalette().color(QPalette::Shadow).name(QColor::HexRgb)));
        labelIcon->setPixmap(QPixmap(icon).scaled(caption_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        layoutCaption->addWidget(labelIcon);
        setWindowIcon(QIcon(icon));
    }

    layoutCaption->addWidget(m_Caption, 1);

    m_BtnCaptionClose = new QPushButton();
    m_BtnCaptionClose->setStyleSheet(CAPTION_BUTTON_STYLE.
                                 arg(QPalette().color(QPalette::Light).name(QColor::HexRgb)));
    m_BtnCaptionClose->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_BtnCaptionClose->setIconSize(caption_size);
    m_BtnCaptionClose->setFixedSize(caption_size + QSize(4, 4));
    m_BtnCaptionClose->setIcon(QIcon(":/resources/img/exit.svg"));
    m_BtnCaptionClose->setToolTip(tr("Close"));
    m_BtnCaptionClose->setFlat(true);
    QObject::connect(m_BtnCaptionClose, &QPushButton::released, this, &QDialog::close);
    layoutCaption->addWidget(m_BtnCaptionClose);
    m_BtnCaptionClose->setVisible(closable);

    frameCaption->setLayout(layoutCaption);

    // содержание
    auto frameContent = new QFrame();
    frameContent->setAutoFillBackground(true);
    frameContent->setFrameStyle(QFrame::Raised | QFrame::Panel);

    m_ContentGridLayout = new QGridLayout(frameContent);
    m_ContentGridLayout->setAlignment(Qt::AlignAbsolute);
    m_ContentGridLayout->setContentsMargins(0, 0, 0, 0);
    m_ContentGridLayout->setSpacing(0);
    frameContent->setLayout(m_ContentGridLayout);

    // панель кнопок
    m_ToolBar = new QToolBar();
    m_ToolBar->setMovable(false);
    m_ToolBar->setOrientation(Qt::Horizontal);
    m_ToolBar->setStyleSheet(TOOLBAR_STYLE);

    glMainContainer->addWidget(frameCaption,        0, 0, 1, -1, Qt::AlignTop);
    glMainContainer->addWidget(frameContent,        1, 0, 1, -1);
    glMainContainer->addWidget(m_ToolBar,           2, 0, 1, 1);
    glMainContainer->addWidget(new QSizeGrip(this), 2, 1, 1, -1, Qt::AlignBottom | Qt::AlignRight);
    setLayout(glMainContainer);

    layout()->setContentsMargins(BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH);

    addToolBarSpacerRight();
    installEventFilter(this);
}

bool DialogBody::eventFilter(QObject* object, QEvent *event)
{
    auto o = qobject_cast<DialogBody*>(object);
    if(o)
    {
        switch (event->type())
        {
        case QEvent::WindowStateChange:
        {
            if(windowState() == Qt::WindowMinimized ||
                    windowState() == Qt::WindowMaximized)
            {
                setWindowState(static_cast<QWindowStateChangeEvent *>(event)->oldState());
                return true;
            }
            return QDialog::eventFilter(object, event);
        }
        case QEvent::Hide:
        case QEvent::Close:
        {
            if(object != this || isMinimized() || isMaximized())
                return QDialog::eventFilter(object, event);

            Q_EMIT signalSizeChanged(size());
            return true;
        }
        default: { return QDialog::eventFilter(object, event); }
        }
    }
    else
    {
        switch (event->type())
        {
        case QEvent::Wheel:
        { return true; }
        default: { return QDialog::eventFilter(object, event); }
        }
    }
}

void DialogBody::addDialogContent(QWidget *widget)
{
    m_ContentGridLayout->addWidget(widget);
}

void DialogBody::setClosable(bool value)
{
    m_BtnCaptionClose->setVisible(value);
}

void DialogBody::setToolBarIconSize(int intsize)
{
    m_ToolBar->setIconSize(QSize(intsize, intsize));
}

void DialogBody::hideToolBar()
{
    auto actions = m_ToolBar->actions();
    for(auto a: actions) m_ToolBar->removeAction(a);

    m_ToolBar->addWidget(new WidgetSpacer(this));
    m_ToolBar->setMaximumHeight(1);
}

void DialogBody::addToolBarSeparatorLeft(bool visible)
{
    addCustomToolBarWidgetFirst(m_ToolBar, new SeparatorV(this, visible));
}

void DialogBody::addToolBarSpacerRight()
{
    m_ToolBar->addWidget(new WidgetSpacer(this));
}

void DialogBody::addToolBarActionRight(QAction *action, const QString& stylename)
{
    addCustomToolBarAction(m_ToolBar, action, stylename);
}

void DialogBody::addToolBarActionLeft(QAction *action, const QString &stylename)
{
    addCustomToolBarActionFirst(m_ToolBar, action, stylename);
}

void DialogBody::addToolBarWidgetLeft(QWidget *widget)
{
    addCustomToolBarWidgetFirst(m_ToolBar, widget);
}

///////////////////////////////////////////////

DialogCaption::DialogCaption(const QString &text, QWidget* parent)
    : QLabel(text, parent)
{
    setStyleSheet(CAPTION_STYLE.
                  arg(QPalette().color(QPalette::Highlight).name(QColor::HexRgb),
                      QPalette().color(QPalette::Light).name(QColor::HexRgb),
                      QPalette().color(QPalette::Shadow).name(QColor::HexRgb),
                      QPalette().color(QPalette::ButtonText).name(QColor::HexRgb)));
    setCaptionText(text);
}

void DialogCaption::setCaptionText(const QString& text)
{
    setText(HTML_LABEL_CAPTION_PATTERN_H.arg(text));
}

void DialogCaption::mousePressEvent(QMouseEvent* event)
{
    m_MouseClick_X = event->position().x() + BORDER_WIDTH;
    m_MouseClick_Y = event->position().y() + BORDER_WIDTH;
}

void DialogCaption::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton)) return;

    window()->move(event->globalPosition().x() - m_MouseClick_X - pos().x(),
                   event->globalPosition().y() - m_MouseClick_Y - pos().y());
}

