#include "dialogvalueslist.h"
#include "properties.h"

#include "../interface/lineeditselectfile.h"
#include "../interface/edittextwidget.h"
#include "../helpers/helpergraphics.h"

#include <QRegularExpression>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QScrollArea>
#include <QSpinBox>
#include <QGridLayout>
#include <QToolBar>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QPushButton>
#include <QColorDialog>
#include <QApplication>

DialogValuesList::DialogValuesList(QWidget* parent,
                                   const QString& icon,
                                   const QString& caption,
                                   QMap<QString, DialogValue> *values,
                                   const QString &focusedKey) :
    DialogBody(parent, caption, icon, true, true),
    m_Values(values),
    m_FocusedKey(focusedKey)
{
    setToolBarIconSize(config->GUISize());

    auto saContent = new QScrollArea();
    saContent->setAlignment(Qt::AlignTop);
    saContent->setWidgetResizable(true);

    auto wContent = new QWidget();
    saContent->setWidget(wContent);

    glContent = new QGridLayout();
    wContent->setLayout(glContent);
    glContent->setAlignment(Qt::AlignTop);

    auto actionAccept = new QAction(QIcon(":/resources/img/yes.svg"), tr("Accept"));
    actionAccept->setAutoRepeat(false);
    QObject::connect(actionAccept, &QAction::triggered, this, [=](){ accept(); });
    addToolBarActionRight(actionAccept, CSS_STYLE_BUTTON);

    addDialogContent(saContent);

    slotLoadContent(values);

    resize(WINDOW_SIZE);

    QObject::connect(this, &QObject::destroyed, this, [=]()
    { qInfo() << "DialogValuesList" << windowTitle() << "destroyed"; });
    qInfo() << "DialogValuesList" << windowTitle() << "created";
}

void DialogValuesList::addWidgetContent(QWidget *widget, bool sub_item)
{
    if(sub_item)
    {
        auto w = new QWidget();
        auto hb = new QHBoxLayout();
        auto style = QApplication::style();
        QStyleOptionButton sob;
        auto space = style->pixelMetric(QStyle::PM_IndicatorWidth, &sob);
        hb->setContentsMargins(space * 3, 0, 0, 0);
        w->setLayout(hb);
        hb->addWidget(widget, Qt::AlignLeft);
        glContent->addWidget(w, glContent->count(), 0, 1, 1, Qt::AlignTop);
    }
    else
        glContent->addWidget(widget, glContent->count(), 0, 1, 1, Qt::AlignTop);
}

void DialogValuesList::slotLoadContent(QMap<QString, DialogValue>* values)
{
    QLayoutItem *child;
    while ((child = glContent->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

    if(!values) { qCritical() << __func__ << ": Values is empty"; return; }

    auto keys = values->keys();
    for (const auto &key: keys)
    {
        QMetaType::Type t = values->value(key).type;
        QVariant v = values->value(key).value;
        QVariant minv = values->value(key).minValue;
        QVariant maxv = values->value(key).maxValue;
        QString text = key; text.remove(QRegularExpression(NUM_MARK_REGEXP));
        bool sub_item = false;
        if(text.startsWith(SUBITEM_MARK))
        {
            text.remove(0, SUBITEM_MARK.length());
            sub_item = true;
        }

        if(t == QMetaType::UnknownType ||
                values->value(key).mode == DialogValueMode::Caption)
        {
            auto label = new QLabel();
            label->setStyleSheet(TITLE_STYLE.
                                 arg(QPalette().color(QPalette::Dark).name(QColor::HexRgb),
                                     QPalette().color(QPalette::Light).name(QColor::HexRgb),
                                     QPalette().color(QPalette::Shadow).name(QColor::HexRgb),
                                     QPalette().color(QPalette::ButtonText).name(QColor::HexRgb)));
            label->setText(HTML_LABEL_CAPTION_PATTERN.arg(text));
            label->setWordWrap(true);
            addWidgetContent(label);
            continue;
        }

        if(values->value(key).mode == DialogValueMode::Disabled)
        {
            auto widget = new QFrame();
            widget->setFrameStyle(QFrame::Box | QFrame::Sunken);
            widget->setMidLineWidth(0);
            auto bl = new QVBoxLayout();
            bl->setContentsMargins(1, 1, 1, 1);
            bl->setSpacing(1);
            auto label = new QLabel(widget);
            label->setText(QString("<b>%1</b>").arg(text));
            label->setWordWrap(true);
            bl->addWidget(label, 0);
            auto le = new QLineEdit(v.toString(), widget);
            le->setReadOnly(true);
            le->setCursorPosition(0);
            QPalette pal;
            pal.setColor(QPalette::Base, palette().color(QPalette::Button));
            pal.setColor(QPalette::Text, palette().color(QPalette::ButtonText));
            le->setPalette(pal);
            bl->addWidget(le, 1);
            widget->setLayout(bl);
            addWidgetContent(widget, sub_item);
            continue;
        }

        if(values->value(key).mode == DialogValueMode::Color)
        {
            auto widget = new QFrame();
            widget->setFrameStyle(QFrame::Box | QFrame::Sunken);
            widget->setMidLineWidth(0);
            auto bl = new QVBoxLayout();
            bl->setContentsMargins(1, 1, 1, 1);
            bl->setSpacing(1);
            auto label = new QLabel(widget);
            label->setText(QString("<b>%1</b>").arg(text));
            label->setWordWrap(true);
            bl->addWidget(label, 0);

            auto btn = new QPushButton(v.toString(), widget);
            btn->setFixedHeight(config->GUISize());
            btn->setAutoFillBackground(true);
            btn->setProperty("ValueName", key);
            btn->setStyleSheet(BUTTON_COLOR_STYLE.
                               arg(QPalette().color(QPalette::Shadow).name(QColor::HexRgb),
                                   v.toString(), GetContrastColor(QColor(v.toString())).name()));
            QObject::connect(btn, &QPushButton::pressed, this, [=]() { slotSelectColor(btn->text(), btn); });
            bl->addWidget(btn, 1);

            widget->setLayout(bl);
            addWidgetContent(widget, sub_item);
            continue;
        }

        if(values->value(key).mode == DialogValueMode::File)
        {
            auto widget = new QFrame();
            widget->setFrameStyle(QFrame::Box | QFrame::Sunken);
            widget->setMidLineWidth(0);
            auto bl = new QVBoxLayout();
            bl->setContentsMargins(1, 1, 1, 1);
            bl->setSpacing(1);
            auto label = new QLabel(widget);
            label->setText(QString("<b>%1</b>").arg(text));
            label->setWordWrap(true);
            bl->addWidget(label, 0);

            auto lesf = new LineEditSelectFile(widget, false, v.toString());
            lesf->setFileFilter(values->value(key).minValue.toString());
            lesf->setProperty("ValueName", key);
            QObject::connect(lesf, &LineEditSelectFile::signalFileSelected, this, [=]()
            { slotSelectFile(lesf->getText(), lesf); });
            bl->addWidget(lesf, 1);

            widget->setLayout(bl);
            addWidgetContent(widget, sub_item);
            continue;
        }

        if(values->value(key).mode == DialogValueMode::Catalog)
        {
            auto widget = new QFrame();
            widget->setFrameStyle(QFrame::Box | QFrame::Sunken);
            widget->setMidLineWidth(0);
            auto bl = new QVBoxLayout();
            bl->setContentsMargins(1, 1, 1, 1);
            bl->setSpacing(1);
            auto label = new QLabel(widget);
            label->setText(QString("<b>%1</b>").arg(text));
            label->setWordWrap(true);
            bl->addWidget(label, 0);

            auto lesf = new LineEditSelectFile(widget, true, v.toString());
            lesf->setFileFilter(values->value(key).minValue.toString());
            lesf->setProperty("ValueName", key);
            QObject::connect(lesf, &LineEditSelectFile::signalFileSelected, this, [=]()
            { slotSelectFile(lesf->getText(), lesf); });
            bl->addWidget(lesf, 1);

            widget->setLayout(bl);
            addWidgetContent(widget, sub_item);
            continue;
        }

        if(values->value(key).mode == DialogValueMode::MultiLineText)
        {
            auto widget = new QFrame();
            widget->setFrameStyle(QFrame::Box | QFrame::Sunken);
            widget->setMidLineWidth(0);
            auto bl = new QVBoxLayout();
            bl->setContentsMargins(1, 1, 1, 1);
            bl->setSpacing(1);
            auto label = new QLabel(widget);
            label->setText(QString("<b>%1</b>").arg(text));
            label->setWordWrap(true);
            bl->addWidget(label, 0);

            auto etw = new EditTextWidget(widget, true, v.toString());
            etw->setProperty("ValueName", key);
            QObject::connect(etw, &EditTextWidget::signalTextEdited, this, [=]()
                             { slotMultiLineTextChanged(etw->getText(), etw); });
            bl->addWidget(etw, 1);

            widget->setLayout(bl);
            addWidgetContent(widget, sub_item);
            continue;
        }

        if(t == QMetaType::QString)
        {
            auto widget = new QFrame();
            widget->setFrameStyle(QFrame::Box | QFrame::Sunken);
            widget->setMidLineWidth(0);
            auto bl = new QVBoxLayout();
            bl->setContentsMargins(1, 1, 1, 1);
            bl->setSpacing(1);
            auto label = new QLabel(widget);
            label->setWordWrap(true);
            label->setText(QString("<b>%1</b>").arg(text));
            bl->addWidget(label, 0);
            auto le = new QLineEdit(v.toString(), widget);
            le->setCursorPosition(0);
            le->setProperty("ValueName", key);
            QObject::connect(le, &QLineEdit::textEdited, this, &DialogValuesList::slotStringValueChanged);
            bl->addWidget(le, 1);
            widget->setLayout(bl);

            addWidgetContent(widget, sub_item);
            if(key == m_FocusedKey) le->setFocus();
            continue;
        }

        if(t == QMetaType::Bool)
        {
            auto cbox = new QCheckBox(text);
            cbox->setChecked(v.toBool());
            cbox->setProperty("ValueName", key);
            QObject::connect(cbox, &QCheckBox::stateChanged,
                             this, &DialogValuesList::slotBoolValueChanged);
            addWidgetContent(cbox, sub_item);
            if(key == m_FocusedKey) cbox->setFocus();
            continue;
        }

        if(t == QMetaType::Int)
        {
            auto spinbox = new QSpinBox();
            spinbox->setPrefix(QString("%1: ").arg(text));
            spinbox->setRange(minv.toInt(),
                              maxv.toInt() == minv.toInt()
                              ? std::numeric_limits<int>::max()
                              : maxv.toInt());
            spinbox->setSingleStep(1);
            spinbox->setValue(v.toInt());
            spinbox->installEventFilter(this);
            spinbox->setProperty("ValueName", key);
            QObject::connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged),
                             this, &DialogValuesList::slotIntValueChanged);
            addWidgetContent(spinbox, sub_item);
            if(key == m_FocusedKey) spinbox->setFocus();
            continue;
        }
        if(t == QMetaType::Double)
        {
            auto spinbox = new QDoubleSpinBox();
            spinbox->setPrefix(QString("%1: ").arg(text));
            spinbox->setRange(minv.toDouble(),
                              maxv.toDouble() - minv.toDouble() == 0.0
                              ? std::numeric_limits<double>::max()
                              : maxv.toDouble());
            spinbox->setDecimals(DOUBLE_SPINBOX_DECIMALS);
            spinbox->setSingleStep(0.1);
            spinbox->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
            spinbox->setValue(v.toDouble());
            spinbox->installEventFilter(this);
            spinbox->setProperty("ValueName", key);
            QObject::connect(spinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                             this, &DialogValuesList::slotDoubleValueChanged);
            addWidgetContent(spinbox, sub_item);
            if(key == m_FocusedKey) spinbox->setFocus();
            continue;
        }
        if(t == QMetaType::QStringList)
        {
            auto widget = new QFrame();
            widget->setFrameStyle(QFrame::Box | QFrame::Sunken);
            widget->setMidLineWidth(0);
            auto bl = new QVBoxLayout();
            bl->setContentsMargins(1, 1, 1, 1);
            auto label = new QLabel();
            label->setText(QString("<b>%1</b>").arg(text));
            label->setWordWrap(true);
            bl->setSpacing(2);
            bl->addWidget(label, 0);
            if(values->value(key).mode == DialogValueMode::Default)
            {
                auto le = new QLineEdit(v.toStringList().join(','), widget);
                le->setCursorPosition(0);
                le->setProperty("ValueName", key);
                le->setToolTip("Enter");
                // значение присваивается по нажатию Enter
                QObject::connect(le, &QLineEdit::returnPressed, this, &DialogValuesList::slotStringListValueChanged);
                bl->addWidget(le, 1);
                widget->setLayout(bl);
                if(key == m_FocusedKey) le->setFocus();
            }
            else if(values->value(key).mode == DialogValueMode::OneFromList)
            {
                auto cb = new QComboBox(widget);
                cb->installEventFilter(this);
                auto list = maxv.toStringList();
                if(!list.isEmpty())
                {
                    //list.sort(Qt::CaseInsensitive);
                    cb->addItems(list);
                    cb->setProperty("ValueName", key);
                    auto index = list.indexOf(v.toString());
                    if(index != -1) cb->setCurrentIndex(index);
                    QObject::connect(cb, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                     this, &DialogValuesList::slotOneOfStringListValueChanged);
                }

                else cb->setDisabled(true);
                bl->addWidget(cb, 1);
                widget->setLayout(bl);
                if(key == m_FocusedKey) cb->setFocus();
            }
            else if(values->value(key).mode == DialogValueMode::ManyFromList)
            {
                auto lv = new QListView(widget);
                lv->setSelectionMode(QAbstractItemView::NoSelection);
                lv->setEditTriggers(QAbstractItemView::NoEditTriggers);
                lv->setSelectionRectVisible(false);
                QFontMetrics fm(lv->font());
                lv->setFixedHeight(fm.height() * 5); // количество строк
                auto model = new QStandardItemModel(lv);
                model->setProperty("ValueName", key);
                auto list = maxv.toStringList();
                if(!list.isEmpty())
                {
                    list.sort(Qt::CaseInsensitive);
                    for(const QString &s: list)
                    {
                        auto i = new QStandardItem(s);
                        i->setCheckable(true);
                        i->setCheckState(v.toStringList().contains(s) ? Qt::Checked : Qt::Unchecked);
                        model->appendRow(i);
                    }

                    lv->setModel(model);
                    connect(model, &QStandardItemModel::itemChanged,
                            this, &DialogValuesList::slotManyOfStringListValueChanged);
                }
                else lv->setDisabled(true);
                bl->addWidget(lv, 1);
                widget->setLayout(bl);
                if(key == m_FocusedKey) lv->setFocus();
            }
            addWidgetContent(widget, sub_item);
            continue;
        }

        qCritical() << __func__ << ": Value" << key << "Unsupported type" << t;
    }
}

void DialogValuesList::setMapValue(const QString &key, const QVariant &value)
{
    DialogValue dv = m_Values->value(key);
    dv.value = value;
    m_Values->insert(key, dv);
}

void DialogValuesList::slotStringValueChanged(const QString &value)
{
    auto ledit = qobject_cast<QLineEdit*>(sender());
    if(!ledit) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = ledit->property("ValueName").toString();
    setMapValue(key, value);
}

void DialogValuesList::slotStringListValueChanged()
{
    auto ledit = qobject_cast<QLineEdit*>(sender());
    if(!ledit) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = ledit->property("ValueName").toString();
    setMapValue(key, ledit->text().split(',', Qt::SkipEmptyParts).
                replaceInStrings(QRegularExpression(FIRST_LAST_SPACES_REGEXP), ""));
}

void DialogValuesList::slotOneOfStringListValueChanged()
{
    auto cb = qobject_cast<QComboBox*>(sender());
    if(!cb) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = cb->property("ValueName").toString();
    setMapValue(key, cb->currentText());
}

void DialogValuesList::slotManyOfStringListValueChanged()
{
    auto sim = qobject_cast<QStandardItemModel*>(sender());
    if(!sim) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = sim->property("ValueName").toString();
    auto sl = QStringList();
    for(int i = 0; i < sim->rowCount(); i++)
        if(sim->item(i)->checkState() == Qt::Checked) sl.append(sim->item(i)->text());
    setMapValue(key, sl);
}

void DialogValuesList::slotBoolValueChanged(bool value)
{
    auto cbox = qobject_cast<QCheckBox*>(sender());
    if(!cbox) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = cbox->property("ValueName").toString();
    setMapValue(key, value);
}

void DialogValuesList::slotIntValueChanged(int value)
{
    auto spinbox = qobject_cast<QSpinBox*>(sender());
    if(!spinbox) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = spinbox->property("ValueName").toString();
    setMapValue(key, value);
}

void DialogValuesList::slotDoubleValueChanged(double value)
{
    auto spinbox = qobject_cast<QDoubleSpinBox*>(sender());
    if(!spinbox) { qCritical() << __func__ << ": Signal sender not found."; return; }
    auto key = spinbox->property("ValueName").toString();
    setMapValue(key, value);
}

void DialogValuesList::slotSelectColor(const QString &value, QPushButton* btn)
{
    const auto color = QColorDialog::getColor(QColor(value), this, tr("Select color"));
    if(color.isValid())
    {
        auto colortxt = color.name().toUpper();
        btn->setText(colortxt);
        btn->setStyleSheet(BUTTON_COLOR_STYLE.
                           arg(QPalette().color(QPalette::Shadow).name(QColor::HexRgb),
                               colortxt, GetContrastColor(QColor(colortxt)).name()));
        auto key = btn->property("ValueName").toString();
        setMapValue(key, colortxt);
    }
}

void DialogValuesList::slotSelectFile(const QString &value, QObject *object)
{
    auto key = object->property("ValueName").toString();
    setMapValue(key, value);
}

void DialogValuesList::slotMultiLineTextChanged(const QString &value, QObject *object)
{
    auto key = object->property("ValueName").toString();
    setMapValue(key, value);
}
