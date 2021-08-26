#include "myclipboardlistmodel.h"

#include <QTextDocument>
#include <QStandardItem>
#include <QImageReader>
#include <QFile>
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

    QString getTooltipFormText(const QString& rawText)
    {
        if(QFile::exists(rawText) == false)
            return QString();

        QImageReader reader(rawText);
        if(!reader.canRead())
            return rawText;

        QString htmlForm = QString("<img src=\"%1\"/>").arg(rawText);
        return htmlForm;
    }
}

MyClipboardModel::MyClipboardModel(QObject *parent)
    : QStandardItemModel(parent)
{
}

bool MyClipboardModel::addClipInfoData(const ClipInfo &clipInfo)
{
    if(clipInfo.first.isEmpty())
        return false;

    QString processedText = TextProcessor::getFragmentText(clipInfo.first);
    auto textHash = qHash(processedText);
    for(int i = rowCount() - 1; i >= 0; i--) {
        if(item(i)->data(DataRole::TextHashValue).toUInt() != textHash)
            continue;

        if(i == 0) {
            return false;
        }
        removeRow(i);
        break;
    }
    QStandardItem* newItem = new QStandardItem(processedText);
    newItem->setData(textHash, DataRole::TextHashValue);
    if(clipInfo.second.isEmpty() == false){
        newItem->setData(clipInfo.second, DataRole::FileSavedPath);
    }

    insertRow(0, newItem);
    return true;
}

Qt::ItemFlags MyClipboardModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::ItemIsDropEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
}

QVariant MyClipboardModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::ToolTipRole){
        auto savedPath = index.data(DataRole::FileSavedPath).toString();
        if(savedPath.isEmpty())
            return QVariant();
        return TextProcessor::getTooltipFormText(savedPath);
    }
    return QStandardItemModel::data(index, role);
}
