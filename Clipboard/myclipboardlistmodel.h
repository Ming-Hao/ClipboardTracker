#ifndef MYCLIPBOARDLISTMODEL_H
#define MYCLIPBOARDLISTMODEL_H

#include <QStandardItemModel>

#include "myclipboardinfo.h"

class MyClipboardModel : public QStandardItemModel
{
public:
    explicit MyClipboardModel(QObject *parent = nullptr);
    bool addClipInfoData(const ClipInfo &clipInfo);
};


#endif // MYCLIPBOARDLISTMODEL_H
