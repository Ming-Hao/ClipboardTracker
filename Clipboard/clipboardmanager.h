#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <memory>

#include "myclipboardlistmodel.h"

class ClipboardMonitor;
class ClipboardActionHandler;

class ClipboardManager
{
public:
    ClipboardManager();
    ~ClipboardManager();
    void setModel(MyClipboardModel *model);
    void sendItemText(const QModelIndex& itemIndex);

private:
    MyClipboardModel* model;
    std::shared_ptr<ClipboardMonitor> clipboardMonitor;
    std::shared_ptr<ClipboardActionHandler> clipboardActionHandler;
};

#endif // CLIPBOARDMANAGER_H
