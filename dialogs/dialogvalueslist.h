#ifndef DIALOGVALUESLIST_H
#define DIALOGVALUESLIST_H

#include "dialogbody.h"

#include <QVariant>

class QGridLayout;
class QToolBar;
class QLineEdit;

/*!
 * \brief DialogValueMode - режим отображения значений
 */
enum DialogValueMode
{
    Default = 0,    // поумолчанию, для StringList - перечисление через запятую
    Disabled,       // просто текстовое отображение значения
    Caption,        // Заголовок
    OneFromList,    // для StringList - один из списка
    ManyFromList,   // для StringList - несколько из списка
    Color,          // для String - цвет
    File,           // для String - выбор файла
    Catalog,        // для String - выбор каталога
    MultiLineText,  // для String - многострочный текст
};

/*!
 * \brief DialogValue - передаваемая для редактирования структура;
 * type: тип результата;
 * value: результат и значение поумолчанию
 * minValue: минимальное значение, если доступно, для String вида File - фильтр;
 * maxValue: максимальнеа значение, для StringList вида OneFromList, ManyFromList - все варианты выбора;
 * mode: способ отображения значений
 */
struct DialogValue
{
    QMetaType::Type type = QMetaType::UnknownType;
    QVariant value = QVariant();
    QVariant minValue = QVariant();
    QVariant maxValue = QVariant();
    DialogValueMode mode = DialogValueMode::Default;
};

/*!
 * \brief DialogValuesList - универсальный диалог
 */
class DialogValuesList : public DialogBody
{
    Q_OBJECT
public:
    /*!
     * \brief DialogValuesList
     * \param icon - иконка окна;
     * \param caption - заголовок окна;
     * \param values - значения для отображения, для сортировки использовать нумерацию вида '00#_' (RE_NUM_MARK);
     * \param focusedKey - контрол для фокуса по ключу values
     */
    DialogValuesList(QWidget* parent,
                     const QString &icon,
                     const QString &caption,
                     QMap<QString, DialogValue>* values,
                     const QString &focusedKey = "");

protected:
    void addWidgetContent(QWidget* widget, bool sub_item = false);

private:
    QGridLayout* glContent;
    QMap<QString, DialogValue>* m_Values;
    QString m_FocusedKey;
    bool m_DialogMode;
    void setMapValue(const QString& key, const QVariant& value);

    // CONST
    const QString FIRST_LAST_SPACES_REGEXP =    "^\\s+|\\s+$"; // регексп на наличие стартовых и финальных пробелов
    const QString NUM_MARK_REGEXP =             "(^.*)(#_)"; // регексп удаления символов до строки '#_' включительно
    const QString SUBITEM_MARK =                "_"; // метка в начале текста о том, что вставлять виджет с отступом
    const QString BUTTON_COLOR_STYLE =  "border: 1px solid %1; border-radius: 9px; "
                                        "background-color: %2; font: bold monospace; color: %3";
    const QString TITLE_STYLE =         "QLabel{ background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                                        "stop: 0 %1, stop: 0.5 %2, stop: 0.5 %2, stop: 1 %1); "
                                        "border: 1px solid %3; border-radius: 5px; "
                                        "padding: 0px 1px 0px 1px; "
                                        "color: %4; font-family: monospace; }";
    const int DOUBLE_SPINBOX_DECIMALS = 4;
    const QSize WINDOW_SIZE = QSize(400, 500);

public Q_SLOTS:
    void slotLoadContent(QMap<QString, DialogValue> *values);

private Q_SLOTS:
    void slotStringValueChanged(const QString& value);
    void slotStringListValueChanged();
    void slotOneOfStringListValueChanged();
    void slotManyOfStringListValueChanged();
    void slotBoolValueChanged(bool value);
    void slotIntValueChanged(int value);
    void slotDoubleValueChanged(double value);
    void slotSelectColor(const QString &value, QPushButton *btn);
    void slotSelectFile(const QString &value, QObject *object);
    void slotMultiLineTextChanged(const QString &value, QObject *object);

};

#endif // DIALOGVALUESLIST_H
