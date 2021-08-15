#include "customtrayicon.h"

#include <QApplication>
#include <QStyle>

CustomTrayIcon::CustomTrayIcon(QWidget* window, QObject *parent)
    : QSystemTrayIcon(parent)
    , window(window)
    , trayIconMenu(new QMenu())
    , minimizeAction(new QAction())
    , restoreAction(new QAction())
    , quitAction(new QAction())
{
    createActions();
    addActions2Menu();
    setContextMenu(trayIconMenu);

    setIcon(qApp->style()->standardIcon(QStyle::QStyle::SP_FileDialogDetailedView));
    setToolTip(window->windowTitle());

    setTrayIconConnections();
}

void CustomTrayIcon::createActions()
{
    minimizeAction->setText(tr("Mi&nimize"));
    connect(minimizeAction, &QAction::triggered, window, &QWidget::hide);

    restoreAction->setText(tr("&Restore"));
    connect(restoreAction, &QAction::triggered, window, &QWidget::showNormal);

    quitAction->setText(tr("&Quit"));
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void CustomTrayIcon::addActions2Menu()
{
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
}

void CustomTrayIcon::setTrayIconConnections()
{
    connect(this, &QSystemTrayIcon::messageClicked, this, [](){
        ;
    });

    connect(this, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason){
        switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
        case QSystemTrayIcon::MiddleClick:
            restoreAction->trigger();
            break;
        default:
            ;
        }
    });
}
