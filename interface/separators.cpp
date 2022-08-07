#include "separators.h"

WidgetSpacer::WidgetSpacer(QWidget *parent): QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

SeparatorH::SeparatorH(QWidget *parent, bool visible): QFrame(parent)
{
    if(visible)
    {
        setFrameStyle(QFrame::Raised | QFrame::HLine);
        setMidLineWidth(1);
        setLineWidth(0);
    }
    setFixedHeight(SEPARATOR_WIDTH);
    setFocusPolicy(Qt::NoFocus);
}

SeparatorV::SeparatorV(QWidget *parent, bool visible): QFrame(parent)
{
    if(visible)
    {
        setFrameStyle(QFrame::Raised | QFrame::VLine);
        setMidLineWidth(1);
        setLineWidth(0);
    }
    setFixedWidth(SEPARATOR_WIDTH);
    setFocusPolicy(Qt::NoFocus);
}


