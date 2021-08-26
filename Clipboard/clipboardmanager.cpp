#include "clipboardmanager.h"

#include "clipboardmonitor.h"
#include "clipboardactionhandler.h"

#include <QCoreApplication>

ClipboardManager::ClipboardManager()
    : model(nullptr)
    , clipboardMonitor(std::make_shared<ClipboardMonitor>())
    , clipboardActionHandler(std::make_shared<ClipboardActionHandler>())
{
    clipboardActionHandler->setSaveFolder(QCoreApplication::applicationDirPath() + "/Copied");
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
