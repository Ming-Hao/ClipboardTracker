#ifndef CLIPBOARDMONITOR_H
#define CLIPBOARDMONITOR_H

#include <QObject>
#include <memory>
#include <QTimer>

#include "myclipboardlistmodel.h"

class ClipboardActionHandler;

class ClipboardMonitor : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardMonitor(QObject* parent = nullptr);
    void setModel(MyClipboardModel *model);
    void setActionHandler(std::shared_ptr<ClipboardActionHandler> actionHandler);

private:
    bool addToModel(const ClipInfo& clipInfoData);

private:
    MyClipboardModel* model;
    std::shared_ptr<ClipboardActionHandler> clipboardActionHandler;
    QTimer* updateTimer;
};

#endif // CLIPBOARDMONITOR_H
