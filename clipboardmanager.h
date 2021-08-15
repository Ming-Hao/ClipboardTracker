#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <memory>

#include <QStandardItemModel>

class ClipboardMonitor;
class ClipboardActionHandler;

class ClipboardManager
{
public:
    ClipboardManager();
    ~ClipboardManager();
    void setModel(QStandardItemModel* model);
    void sendItemText(const QModelIndex& itemIndex);

private:
    QStandardItemModel* model;
    std::shared_ptr<ClipboardMonitor> clipboardMonitor;
    std::shared_ptr<ClipboardActionHandler> clipboardActionHandler;
};

#endif // CLIPBOARDMANAGER_H
