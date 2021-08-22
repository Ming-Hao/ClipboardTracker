#include "myclipboardlistmodel.h"

#include <QTextDocument>
#include <QDebug>

namespace TextProcessor
{
    QString getFragmentText(const QString& rawText)
    {
        if(Qt::mightBeRichText(rawText) == false)
            return rawText;

        const QString startHint = "<!--StartFragment-->";
        const QString endHint = "<!--EndFragment-->";

        if(rawText.contains(startHint) == false)
            return rawText;
        if(rawText.contains(endHint) == false)
            return rawText;

        int index_begin = rawText.indexOf(startHint);
        QString fragmentText = rawText.mid(index_begin + startHint.size());
        int index_end = fragmentText.indexOf(endHint);
        fragmentText.truncate(index_end);
        return fragmentText;
    }
}

MyClipboardModel::MyClipboardModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int MyClipboardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return clipText.size();
}

int MyClipboardModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant MyClipboardModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int rowIndex = index.row();
    if(rowIndex >= clipText.size())
        return QVariant();

    if(role == Qt::DisplayRole) {
        return clipText[rowIndex];
    }
    if(role == Qt::ToolTip) {
        QString tooltip = clipTooltip[clipText[rowIndex]];
        if(tooltip.isEmpty())
            return QVariant();
        return tooltip;
    }
    return QVariant();
}

Qt::ItemFlags MyClipboardModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
}

bool MyClipboardModel::addClipInfoData(const ClipInfo &clipInfo)
{
    if(clipInfo.first.isEmpty())
        return false;

    QString processedText = TextProcessor::getFragmentText(clipInfo.first);
    int index = clipText.indexOf(processedText);
    if(index == 0)
        return false;

    if(index < 0) {
        clipText.push_front(processedText);
        clipTooltip.insert(processedText, clipInfo.second);
    }
    else {
        clipText.move(index, 0);
    }
    auto start_index = createIndex(0, 0);
    auto end_index = createIndex((clipText.size() - 1), 1);
    emit dataChanged(start_index, end_index, { Qt::DisplayRole });
    return true;
}
