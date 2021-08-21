#include "clipboardmonitor.h"

#include "clipboardactionhandler.h"

#include <QApplication>
#include <QClipboard>
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

ClipboardMonitor::ClipboardMonitor(QObject *parent)
    : QObject(parent)
    , model(nullptr)
{
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, [this]() {
        const auto clipInfos = clipboardActionHandler->save(QApplication::clipboard());
        if(clipInfos.isEmpty())
            return;

        for(const auto& clipInfo : clipInfos) {
            if(addToModel(clipInfo.first)) {
                if(clipInfo.second.isEmpty() == false) {
                    model->setData(model->item(0)->index(), clipInfo.second ,Qt::ToolTipRole);
                }
            }
        }
    });
}

void ClipboardMonitor::setModel(QStandardItemModel *model)
{
    this->model = model;
}

void ClipboardMonitor::setActionHandler(std::shared_ptr<ClipboardActionHandler> actionHandler)
{
    clipboardActionHandler = actionHandler;
}

bool ClipboardMonitor::addToModel(const QString &text)
{
    if(model == nullptr)
        return false;

    if(text.isEmpty())
        return false;

    QString processedText = TextProcessor::getFragmentText(text);

    for(int i = model->rowCount() - 1; i >= 0; i--) {
        QModelIndex idx = (model->index(i, 0));
        if(model->data(idx).toString() == processedText) {
            if(i == 0) {
                return false;
            }
            model->removeRow(i);
            break;
        }
    }

    QStandardItem* item = new QStandardItem(processedText);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
    model->insertRow(0, item);

    return true;
}
