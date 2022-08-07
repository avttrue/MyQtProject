#ifndef SEPARATORS_H
#define SEPARATORS_H

#include <QFrame>

const int SEPARATOR_WIDTH = 9;

class WidgetSpacer: public QWidget
{    
public:
    explicit WidgetSpacer(QWidget* parent = nullptr);
};

class SeparatorH:  public QFrame
{
public:
    explicit SeparatorH(QWidget* parent = nullptr, bool visible = true);
};

class SeparatorV:  public QFrame
{
public:
    explicit SeparatorV(QWidget* parent = nullptr, bool visible = true);
};

#endif // SEPARATORS_H
