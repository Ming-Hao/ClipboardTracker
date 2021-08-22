#ifndef MYCLIPBOARDLISTMODEL_H
#define MYCLIPBOARDLISTMODEL_H

#include <QAbstractListModel>

#include "myclipboardinfo.h"

class MyClipboardModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MyClipboardModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool addClipInfoData(const ClipInfo& clipInfo);
private:
    QHash<QString, QString> clipTooltip;
    QStringList clipText;
};

#endif // MYCLIPBOARDLISTMODEL_H
