#ifndef CLIPBOARDMONITOR_H
#define CLIPBOARDMONITOR_H

#include <QObject>
#include <QStandardItemModel>
#include <memory>

class ClipboardActionHandler;

class ClipboardMonitor : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardMonitor(QObject* parent = nullptr);
    void setModel(QStandardItemModel* model);
    void setActionHandler(std::shared_ptr<ClipboardActionHandler> actionHandler);

private:
    bool addToModel(const QString& text);

private:
    QStandardItemModel* model;
    std::shared_ptr<ClipboardActionHandler> clipboardActionHandler;
};

#endif // CLIPBOARDMONITOR_H
