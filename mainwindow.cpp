#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QCloseEvent>
#include <QMessageBox>
#include <QToolBar>

#include "clipboardmanager.h"
#include "customtrayicon.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , trayIcon(new CustomTrayIcon(this, this))
    , clipboardManager(std::make_unique<ClipboardManager>())
    , residentMode(false)
{
    ui->setupUi(this);

    buildToolBarActions();
    setupModelAndView();

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
    return residentMode;
}

void MainWindow::setupModelAndView()
{
    model = new MyClipboardModel();
    ui->textTypeListView->setModel(model);
    ui->textTypeListView->setItemDelegate(new ListViewDelegate);
    ui->textTypeListView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->textTypeListView->setEditTriggers(QAbstractItemView::DoubleClicked);

    clipboardManager->setModel(model);
}

void MainWindow::buildToolBarActions()
{
    auto appStyle = qApp->style();
    QAction* openAction = new QAction(appStyle->standardIcon(QStyle::SP_DialogOpenButton),
                                       "Open File", this);

    connect(openAction, &QAction::triggered, this, [this](){
        clipboardManager->openFile(ui->textTypeListView->currentIndex());
    });

    QAction* deleteAction = new QAction(appStyle->standardIcon(QStyle::SP_DialogDiscardButton),
                                        "Delete selected item", this);

    connect(deleteAction, &QAction::triggered, this, [this](){
        clipboardManager->deleteItemAndFile(ui->textTypeListView->currentIndex());
    });

    QIcon icon;
    icon.addPixmap(appStyle->standardPixmap(QStyle::SP_DialogNoButton), QIcon::Normal, QIcon::Off);
    icon.addPixmap(appStyle->standardPixmap(QStyle::SP_DialogYesButton), QIcon::Normal, QIcon::On);
    QAction* residentAction = new QAction(icon, "Resident Mode", this);
    residentAction->setCheckable(true);

    connect(residentAction, &QAction::triggered, this, [this](bool checked){
        residentMode = checked;
        if(checked) {
            statusBar()->showMessage("Enable resident mode");
        }
        else {
            statusBar()->showMessage("Disable resident mode");
        }
    });

    QToolBar *toolBar = new QToolBar(this);
    toolBar->addAction(residentAction);
    toolBar->addSeparator();
    toolBar->addAction(openAction);
    toolBar->addAction(deleteAction);

    addToolBar(toolBar);
}

QSize ListViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(24);
    return result;
}
