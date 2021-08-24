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

    model = new MyClipboardModel();
    ui->textTypeListView->setModel(model);
    ui->textTypeListView->setItemDelegate(new ListViewDelegate);
    ui->textTypeListView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->textTypeListView->setEditTriggers(QAbstractItemView::DoubleClicked);

    clipboardManager->setModel(model);

    ui->btnOpenFile->setIcon(qApp->style()->standardIcon(QStyle::QStyle::SP_DialogOpenButton));
    ui->btnOpenFile->setToolTip("Open File");
    connect(ui->btnOpenFile, &QAbstractButton::clicked, this, [this](){
        clipboardManager->openFile(ui->textTypeListView->currentIndex());
    });

    ui->btnDelete->setIcon(qApp->style()->standardIcon(QStyle::QStyle::SP_DialogCloseButton));
    ui->btnDelete->setToolTip("Delete selected item");
    connect(ui->btnDelete, &QAbstractButton::clicked, this, [this](){
        clipboardManager->deleteItemAndFile(ui->textTypeListView->currentIndex());
    });

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

QSize ListViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(24);
    return result;
}

