#ifndef MYCLIPBOARDLISTMODEL_H
#define MYCLIPBOARDLISTMODEL_H

#include <QStandardItemModel>

#include "myclipboardinfo.h"

class MyClipboardModel : public QStandardItemModel
{
public:
    enum DataRole {
        TextHashValue = Qt::UserRole + 100,
        FileSavedPath = Qt::UserRole + 200
    };
    explicit MyClipboardModel(QObject *parent = nullptr);
    bool addClipInfoData(const ClipInfo &clipInfo);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

};


#endif // MYCLIPBOARDLISTMODEL_H
