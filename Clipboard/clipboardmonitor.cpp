#include "clipboardmonitor.h"

#include "clipboardactionhandler.h"

#include <QApplication>
#include <QClipboard>
#include <QTimer>
#include <QDebug>

ClipboardMonitor::ClipboardMonitor(QObject *parent)
    : QObject(parent)
    , model(nullptr)
{
    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    updateTimer->setInterval(50);
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, [this]() {
        updateTimer->start();
    });

    connect(updateTimer, &QTimer::timeout, this, [this](){
        const auto clipInfos = clipboardActionHandler->save(QApplication::clipboard());
        if(clipInfos.isEmpty())
            return;

        for(const auto& clipInfo : clipInfos) {
            addToModel(clipInfo);
        }
    });
}

void ClipboardMonitor::setModel(MyClipboardModel *model)
{
    this->model = model;
}

void ClipboardMonitor::setActionHandler(std::shared_ptr<ClipboardActionHandler> actionHandler)
{
    clipboardActionHandler = actionHandler;
}

bool ClipboardMonitor::addToModel(const ClipInfo &clipInfoData)
{
    if(model == nullptr)
        return false;

    return model->addClipInfoData(clipInfoData);
}
