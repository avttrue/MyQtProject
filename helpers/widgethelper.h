#ifndef WIDGETHELPER_H
#define WIDGETHELPER_H

#include <QString>

class QWidget;
class QToolBar;
class QAction;
class QActionGroup;

/*!
 * поместить виджет по центру экрана
 */
void setWidgetToScreenCenter(QWidget* widget);

/*!
 * поместить виджет по центру предка
 */
void setWidgetToParentCenter(QWidget* widget);

void normalizeWidgetPosSize(QWidget* widget);

/*!
 * найти окно по заголовку, если уже есть такое, то восстановить его
 */
bool findPreviousWindowCopy(const QString& caption);

/*!
 * \brief addCustomToolBarAction - добавить Action на QToolBar с присвоением имени для стилизации через css
 */
void addCustomToolBarAction(QToolBar *bar, QAction* action, const QString& stylename = "");

void addCustomToolBarActions(QToolBar *bar, QActionGroup* actions, const QString& stylename = "");

void addCustomToolBarActionFirst(QToolBar *bar, QAction* action, const QString& stylename = "");

void addCustomToolBarWidgetFirst(QToolBar *bar, QWidget* widget);

#endif // WIDGETHELPER_H
