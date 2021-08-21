#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QCloseEvent>
#include <QMessageBox>

#include "clipboardmanager.h"
#include "customtrayicon.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , trayIcon(new CustomTrayIcon(this, this))
    , clipboardManager(std::make_unique<ClipboardManager>())
{
    ui->setupUi(this);


    model = new QStandardItemModel();
    ui->textTypeListView->setModel(model);
    ui->textTypeListView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->textTypeListView->setEditTriggers(QAbstractItemView::DoubleClicked);

    clipboardManager->setModel(model);

//    connect(ui->textTypeListView, &QAbstractItemView::clicked, this, [this](const QModelIndex& index){
//       clipboardManager->sendItemText(index);
//    });

    setWindowTitle("ClipboardTracker");
    setWindowIcon(trayIcon->icon());
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isResidentMode() == false) {
        qApp->quit();
        return;
    }

    if (trayIcon->isVisible() == false) {
        QMainWindow::closeEvent(event);
        return;
    }

    QMessageBox::information(this, tr("Systray"),
                             tr("The program will keep running in the "
                                "system tray. To terminate the program, "
                                "choose <b>Quit</b> in the context menu "
                                "of the system tray entry."));
    hide();
    event->ignore();
}

bool MainWindow::isResidentMode() const
{
    return ui->chkboxResident->isChecked();
}
