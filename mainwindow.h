#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStyledItemDelegate>
#include <memory>
#include "myclipboardlistmodel.h"

class ClipboardManager;
class CustomTrayIcon;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    bool isResidentMode() const;

private:
    Ui::MainWindow *ui;
    CustomTrayIcon *trayIcon;
    MyClipboardModel* model;
    std::unique_ptr<ClipboardManager> clipboardManager;

};

class ListViewDelegate : public QStyledItemDelegate
{
protected:
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};
#endif // MAINWINDOW_H
