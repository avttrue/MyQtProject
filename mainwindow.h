#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {};

protected:
    bool eventFilter(QObject* object, QEvent* event);
    void atApplicationClosed();
    void loadGui();
    void applySizes();

private:
    QToolBar*           m_ToolBarMain;

private Q_SLOTS:
    void slotAbout();
    void slotSetup();
};
#endif // MAINWINDOW_H
