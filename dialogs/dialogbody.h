#ifndef DIALOGBODY_H
#define DIALOGBODY_H

#include <QDialog>
#include <QLabel>

class QGridLayout;
class QToolBar;

/*!
 * \brief DialogCaption class - заголовок окна с возможностью перетаскивания
 */
class DialogCaption : public QLabel
{
    Q_OBJECT

public:
    DialogCaption(const QString &text, QWidget* parent = nullptr);

public Q_SLOTS:
    void setCaptionText(const QString& text);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    int m_MouseClick_X;
    int m_MouseClick_Y;

    // const
private:
    const int BORDER_WIDTH = 4; // DialogBody
    const QString CAPTION_STYLE = "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                                  "stop: 0   %1, stop: 0.5 %2, "
                                  "stop: 0.5 %2, stop: 1   %1); "
                                  "border: 1px solid %3; "
                                  "padding: 0px 1px 0px 1px; border-radius: 5px; "
                                  "color: %4; font-family: monospace; font: bold; ";
};

/////////////////////////////////////////////////////

class DialogBody : public QDialog
{
    Q_OBJECT

public:
    DialogBody(QWidget* parent,
               const QString& text,
               const QString& icon,
               bool closable = true,
               bool modal = false,
               bool ontop = false);

    void addDialogContent(QWidget* widget);
    void setClosable(bool value);
    void addToolBarSeparatorLeft(bool visible = true);
    void setToolBarIconSize(int intsize);
    void hideToolBar();
    void addToolBarSpacerRight();
    void addToolBarActionRight(QAction *action, const QString& stylename = "");
    void addToolBarActionLeft(QAction *action, const QString &stylename = "");
    void addToolBarWidgetLeft(QWidget *widget);

private:
    QPushButton*    m_BtnCaptionClose;
    QGridLayout*    m_ContentGridLayout;
    DialogCaption*  m_Caption;
    QToolBar*       m_ToolBar;

    // const
private:
    const int BORDER_WIDTH = 4; // DialogCaption
    const QString WINDOW_STYLE =         "QDialog { border: 3px ridge %1; }";
    const QString CAPTION_ICON_STYLE =   "border: 1px solid %1; "
                                         "background: qradialgradient(cx:0, cy:0, radius: 1, fx:0.5, fy:0.5, stop:0 white, stop:1 gray); "
                                         "border-radius: 5px; "
                                         "padding: 1px 1px 1px 1px; "
                                         "margin: 1px 1px 1px 1px; ";
    const QString CAPTION_BUTTON_STYLE = "border: 0px; "
                                         "background: %1; "
                                         "border-radius: 5px; "
                                         "padding: 1px 1px 1px 1px; "
                                         "margin: 1px 1px 1px 1px; ";
    const QString TOOLBAR_STYLE =        "QToolBar{ border-radius: 5px; margin: 1px; padding: 2px 1px 2px 1px; }";


protected:
    bool eventFilter(QObject *object, QEvent *event);

Q_SIGNALS:
    void signalSizeChanged(QSize value);

};

#endif // DIALOGBODY_H
