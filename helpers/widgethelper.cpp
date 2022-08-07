#include "widgethelper.h"

#include <QActionGroup>
#include <QApplication>
#include <QScreen>
#include <QStyle>
#include <QToolBar>

void setWidgetToScreenCenter(QWidget* widget)
{
    auto rectScreen = QGuiApplication::primaryScreen()->availableGeometry();
    auto rectWidget = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                          widget->size(), rectScreen);
    widget->setGeometry(rectWidget);
}

void setWidgetToParentCenter(QWidget* widget)
{
    auto parent = widget->parentWidget();
    if(parent == nullptr)
    {
        qCritical() << widget << ": Parent not found";
        return;
    }
    auto prect = parent->geometry();
    auto center = prect.center();
    center.setX(center.x() - (widget->width() / 2));
    center.setY(center.y() - (widget->height() / 2));
    widget->move(center);
}

bool findPreviousWindowCopy(const QString& caption)
{
    auto widgets = QApplication::topLevelWidgets();
    for(auto w: widgets)
    {
        if(!w) continue;

        if(w->windowTitle() == caption)
        {
            w->showNormal();
            w->setWindowState(Qt::WindowActive);
            return true;
        }
    }
    return false;
}

void addCustomToolBarAction(QToolBar* bar, QAction *action, const QString& stylename)
{
    if(!bar || !action) return;
    bar->addAction(action);

    if(stylename.isEmpty()) return;
    bar->widgetForAction(action)->setObjectName(stylename);
}

void addCustomToolBarActionFirst(QToolBar *bar, QAction *action, const QString &stylename)
{
    if(!bar || !action) return;

    auto list = bar->actions();
    if(list.isEmpty()) bar->addAction(action);
    else bar->insertAction(list.at(0), action);

    if(stylename.isEmpty()) return;
    bar->widgetForAction(action)->setObjectName(stylename);
}

void addCustomToolBarWidgetFirst(QToolBar *bar, QWidget *widget)
{
    if(!bar || !widget) return;

    auto list = bar->actions();
    if(list.isEmpty()) bar->addWidget(widget);
    else bar->insertWidget(list.at(0), widget);
}

void normalizeWidgetPosSize(QWidget *widget)
{
    auto rectScreen = QGuiApplication::primaryScreen()->availableGeometry();
    auto pointWidget = widget->pos();
    auto sizeWidget = widget->size();
    if (pointWidget.x() < 0) pointWidget.setX(0);
    if (pointWidget.y() < 0) pointWidget.setY(0);
    if (sizeWidget.width() > rectScreen.width()) sizeWidget.setWidth(rectScreen.width());
    if (sizeWidget.height() > rectScreen.height()) sizeWidget.setHeight(rectScreen.height());
    widget->move(pointWidget);
    widget->resize(sizeWidget);
}

void addCustomToolBarActions(QToolBar *bar, QActionGroup *actions, const QString &stylename)
{
    if(!bar || !actions) return;
    bar->addActions(actions->actions());

    if(stylename.isEmpty()) return;

    for(auto a: actions->actions()) bar->widgetForAction(a)->setObjectName(stylename);
}
