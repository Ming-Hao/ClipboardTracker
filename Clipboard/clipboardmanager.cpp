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

    clipboardActionHandler->sendText(model->data(itemIndex).toString());
}
