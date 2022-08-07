#include "mainwindow.h"
#include "properties.h"

#include "helpers/helper_common.h"
#include "helpers/widgethelper.h"
#include "interface/separators.h"
#include "dialogs/dialoginfocontent.h"

#include <QEvent>
#include <QMessageBox>
#include <QDebug>
#include <QToolBar>
#include <QGridLayout>
#include <QApplication>

#include <dialogs/dialogvalueslist.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setObjectName("MainWindow");
    setWindowIcon(QIcon(":/resources/img/mainicon.svg"));
    setWindowTitle(QString("%1 %2").arg(APP_NAME, APP_VERSION));
    loadGui();
    installEventFilter(this);

    resize(config->GetWidgetSize(this, MAINWINDOW_SIZE));
    if(config->MainWindowOnCenter()) setWidgetToScreenCenter(this);
    else move(config->GetWidgetPos(this, QPoint(0, 0)));
    normalizeWidgetPosSize(this);

    QObject::connect(this, &MainWindow::destroyed, this, [=]() { qInfo() << "MainWindow destroyed"; });
    qInfo() << "MainWindow created";
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Close:
    {
        event->ignore();
        if(config->ShowExitQuestion())
        {

            auto reply = QMessageBox::question(this, tr("Confirm"),
                                               QString("%1 '%2'?").
                                               arg(tr("Really stop working with the"), APP_NAME),
                                               QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::No)
            {
                return true;
            }
        }

        qInfo() << "Exit...";
        atApplicationClosed();
        config->deleteLater();
        return QMainWindow::eventFilter(object, event);
    }

    default: { return QMainWindow::eventFilter(object, event); }
    }
}

void MainWindow::atApplicationClosed()
{
    config->SaveWidgetPos(this);
    config->SaveWidgetSize(this);
}

void MainWindow::loadGui()
{
    // действия
    auto actionQt = new QAction(QIcon(":/resources/img/qt_logo.svg"), tr("About Qt"), this);
    QObject::connect(actionQt, &QAction::triggered, qApp, QApplication::aboutQt);
    actionQt->setAutoRepeat(false);

    auto actionExit = new QAction(QIcon(":/resources/img/exit.svg"), tr("Exit"), this);
    QObject::connect(actionExit, &QAction::triggered, this, &MainWindow::close);
    actionExit->setAutoRepeat(false);

    auto actionAbout = new QAction(QIcon(":/resources/img/info.svg"), tr("About"), this);
    QObject::connect(actionAbout, &QAction::triggered, this, &MainWindow::slotAbout);
    actionAbout->setAutoRepeat(false);

    auto actionSetup = new QAction(QIcon(":/resources/img/setup.svg"), tr("Settings"), this);
    actionSetup->setAutoRepeat(false);
    QObject::connect(actionSetup, &QAction::triggered, this, &MainWindow::slotSetup);


    // тулбар основной
    m_ToolBarMain = new QToolBar(this);
    m_ToolBarMain->setMovable(false);
    m_ToolBarMain->setOrientation(Qt::Horizontal);

    // основной тулбар actions
    m_ToolBarMain->addWidget(new WidgetSpacer(this));
    addCustomToolBarAction(m_ToolBarMain, actionSetup, CSS_STYLE_BUTTON);
    m_ToolBarMain->addSeparator();
    addCustomToolBarAction(m_ToolBarMain, actionAbout, CSS_STYLE_BUTTON);
    addCustomToolBarAction(m_ToolBarMain, actionQt, CSS_STYLE_BUTTON);
    addCustomToolBarAction(m_ToolBarMain, actionExit, CSS_STYLE_BUTTON);

    // основной контейнер
    auto glMainContainer = new QGridLayout();
    glMainContainer->setAlignment(Qt::AlignTop);
    glMainContainer->setContentsMargins(2, 2, 2, 2);
    glMainContainer->setSpacing(0);
    glMainContainer->addWidget(m_ToolBarMain, 0, 0, 1, 1);

    // основной Widget
    auto mainWidget = new QWidget(this);
    mainWidget->setLayout(glMainContainer);

    setCentralWidget(mainWidget);

    applySizes();
}

void MainWindow::applySizes()
{
    m_ToolBarMain->setIconSize(QSize(config->GUISize(), config->GUISize()));
}

void MainWindow::slotAbout()
{
    auto content = getTextFromRes(":/resources/html/about_body.html").
            arg(APP_NAME, APP_VERSION, getSystemInfo(), QT_VERSION_STR, BuildDateTime(), APP_URL);

    auto title = tr("About '%1'").arg(APP_NAME);
    if(findPreviousWindowCopy(title)) return;

    auto dic = new DialogInfoContent(this, title);
    dic->resize(ABOUTWINDOW_SIZE);
    dic->setHtmlContent(content);
    dic->hideToolBar();
    dic->show();
}

void MainWindow::slotSetup()
{
    const QVector<QString> keys =
    {QString("00#_%1").arg(tr("Общие настройки")),
     QString("01#_%1").arg(tr("Подтвеждение завершения работы")),
     QString("02#_%1").arg(tr("Окно приложения по центру экрана")),
     QString("03#_%1").arg(tr("Время заставки (ms)")),
     QString("04#_%1").arg(tr("Писать логи в файл (после перезапуска)")),
     QString("05#_%1").arg(tr("Размеры интерфейса")),
     QString("06#_%1").arg(tr("Стилизовать интерфейс")),

    };

    QMap<QString, DialogValue>
            map =
    {
        {keys.at(0), {}},
        {keys.at(1), {QMetaType::Bool, config->ShowExitQuestion()}},
        {keys.at(2), {QMetaType::Bool, config->MainWindowOnCenter()}},
        {keys.at(3), {QMetaType::Int, config->SplashTime(), 0, 10000}},
        {keys.at(4), {QMetaType::Bool, config->WriteLogsToFile()}},
        {keys.at(5), {QMetaType::Int, config->GUISize(), 16, 128}},
        {keys.at(6), {QMetaType::Bool, config->UseStyleSheet()}},

    };

    auto dvl = new DialogValuesList(this, ":/resources/img/setup.svg", tr("Настройки"), &map);
    dvl->setObjectName("DialogSettings");
    dvl->resize(config->GetWidgetSize(dvl, CONFIG_DIALOG_SIZE));
    QObject::connect(dvl, &DialogBody::signalSizeChanged, this, [=]()
    { config->SaveWidgetSize(dvl); });
    if(!dvl->exec()) return;

    qInfo() << __func__ << ": Accepted";

    // 0
    config->setShowExitQuestion(map.value(keys.at(1)).value.toBool());
    config->setMainWindowOnCenter(map.value(keys.at(2)).value.toBool());
    config->setSplashTime(map.value(keys.at(3)).value.toInt());
    config->setWriteLogsToFile(map.value(keys.at(4)).value.toBool());
    config->setGUISize(map.value(keys.at(5)).value.toInt());
    config->setUseStyleSheet(map.value(keys.at(6)).value.toBool());

    // применение возможных опций
    qApp->setStyleSheet(config->UseStyleSheet() ? getTextFromRes(CSS_MAIN) : "");
    applySizes();
}

