#include "clipboardmonitor.h"

#include "clipboardactionhandler.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>

ClipboardMonitor::ClipboardMonitor(QObject *parent)
    : QObject(parent)
    , model(nullptr)
{
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, [this]() {
        QString modelDisplayText = clipboardActionHandler->save(QApplication::clipboard());
        addToModel(modelDisplayText);
    });
}

void ClipboardMonitor::setModel(QStandardItemModel *model)
{
    this->model = model;
}

void ClipboardMonitor::setActionHandler(std::shared_ptr<ClipboardActionHandler> actionHandler)
{
    clipboardActionHandler = actionHandler;
}

bool ClipboardMonitor::addToModel(const QString &text)
{
    if(model == nullptr)
        return false;

    if(text.isEmpty())
        return false;

    for(int i = model->rowCount() - 1; i >= 0; i--) {
        QModelIndex idx = (model->index(i, 0));
        if(model->data(idx).toString() == text) {
            model->removeRow(i);
            break;
        }
    }
    model->insertRow(0, new QStandardItem(text));
    return true;
}
