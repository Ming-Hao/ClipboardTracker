#include "clipboardmanager.h"

#include "clipboardmonitor.h"
#include "clipboardactionhandler.h"

#include <QCoreApplication>
#include <QStandardPaths>

ClipboardManager::ClipboardManager()
    : model(nullptr)
    , clipboardMonitor(std::make_shared<ClipboardMonitor>())
    , clipboardActionHandler(std::make_shared<ClipboardActionHandler>())
{
#if defined(Q_OS_WIN)
    clipboardActionHandler->setSaveFolder(QCoreApplication::applicationDirPath() + "/Copied");
#else
    clipboardActionHandler->setSaveFolder(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Copied");
#endif
    clipboardMonitor->setActionHandler(clipboardActionHandler);
}

ClipboardManager::~ClipboardManager()
{
    clipboardActionHandler->clear();
}

void ClipboardManager::setModel(MyClipboardModel *model)
{
    this->model = model;
    clipboardMonitor->setModel(this->model);
}

void ClipboardManager::sendItemText(const QModelIndex &itemIndex)
{
    if(model == nullptr)
        return;

    if(!itemIndex.isValid())
        return;

    clipboardActionHandler->sendText(model->data(itemIndex).toString());
}

void ClipboardManager::openFile(const QModelIndex &itemIndex)
{
    if(model == nullptr)
        return;

    if(!itemIndex.isValid())
        return;

    auto savedPath = model->data(itemIndex, MyClipboardModel::FileSavedPath).toString();
    clipboardActionHandler->openFile(savedPath);
}

void ClipboardManager::deleteItemAndFile(const QModelIndex &itemIndex)
{
    if(model == nullptr)
        return;

    if(!itemIndex.isValid())
        return;

    auto savedPath = model->data(itemIndex, MyClipboardModel::FileSavedPath).toString();
    clipboardActionHandler->deleteFile(savedPath);
    model->removeRow(itemIndex.row());
}
