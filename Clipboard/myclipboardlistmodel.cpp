#include "myclipboardlistmodel.h"

#include <QTextDocument>
#include <QStandardItem>
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
    : QStandardItemModel(parent)
{
}

bool MyClipboardModel::addClipInfoData(const ClipInfo &clipInfo)
{
    if(clipInfo.first.isEmpty())
        return false;

    QString processedText = TextProcessor::getFragmentText(clipInfo.first);

    for(int i = rowCount() - 1; i >= 0; i--) {
        if(item(i)->text() != processedText)
            continue;

        if(i == 0) {
            return false;
        }
        removeRow(i);
        break;
    }
    QStandardItem* newItem = new QStandardItem(processedText);

    if(clipInfo.second.isEmpty() == false){
        newItem->setData(clipInfo.second, Qt::ToolTipRole);
    }

    insertRow(0, newItem);
    return true;
}
