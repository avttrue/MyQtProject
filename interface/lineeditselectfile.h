#ifndef LINEEDITSELECTFILE_H
#define LINEEDITSELECTFILE_H

#include <QFrame>

class QLineEdit;
class QPushButton;

class LineEditSelectFile : public QFrame
{
    Q_OBJECT
public:
    explicit LineEditSelectFile(QWidget *parent,
                                bool asCatalog,
                                const QString& text = "",
                                bool fileExist = true);
    QLineEdit* LineEdit() { return m_le; }
    QString getText();
    void setFileFilter(const QString& text) {m_FileFilter = text; };

private:
    QLineEdit*      m_le;
    QPushButton*    m_pb;
    QString         m_FileFilter;
    bool            m_AsCatalog;
    bool            m_FileExist;

private Q_SLOTS:
    void slotOpenFile();

Q_SIGNALS:
    void signalFileSelected(const QString& text);

};

#endif // LINEEDITSELECTFILE_H
