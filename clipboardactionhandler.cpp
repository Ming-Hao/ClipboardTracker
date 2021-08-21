#include "clipboardactionhandler.h"

#include <QClipboard>
#include <QMimeData>
#include <QDateTime>
#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <Windows.h>

const QString ClipboardActionHandler::getSaveFolder() const
{
    return saveFolder;
}

void ClipboardActionHandler::setSaveFolder(const QString &newSaveFolder)
{
    saveFolder = newSaveFolder;
}

QList<ClipInfo> ClipboardActionHandler::save(QClipboard *clipboard)
{
    mkDirIfNotCreated();

    if(clipboard->mimeData()->hasHtml() && clipboard->mimeData()->hasImage()) {
        QString savedPath = saveImage(clipboard->image());
        return { { clipboard->mimeData()->html(), savedPath } };
    }

    const auto textOrUrls = getTextOrUrls(clipboard);

    if(textOrUrls.isEmpty()) {
        return {};
    }

    QList<ClipInfo> results;

    for(int i = 0; i < textOrUrls.size(); i++) {
        QUrl url(textOrUrls[i]);
        if(url.isLocalFile()) {
            results.push_back(std::make_pair(textOrUrls[i], saveFile(url.toLocalFile())));
        }
        else {
            results.push_back(std::make_pair(textOrUrls[i], ""));
        }
    }

    return results;
}

void ClipboardActionHandler::clear()
{
    if(saveFolder.isEmpty())
        return;

    QDir dir(saveFolder);
    dir.removeRecursively();
}

void ClipboardActionHandler::sendText(const QString &text)
{
    INPUT ip;
    // Pause for 5 seconds.
    Sleep(2000);
    for (int i=0; i < text.length(); i++)
    {
        // Set up a generic keyboard event.
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0; // hardware scan code for key
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;

        // Press the key
        byte x = VkKeyScan(text.at(i).unicode());
        ip.ki.wVk = x; // virtual-key code for the "a" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        // Release the key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
}

QStringList ClipboardActionHandler::getTextOrUrls(QClipboard *clipboard)
{
    const QString text = clipboard->text();
    if(text.isEmpty()) {
        return {};
    }

    if(clipboard->mimeData()->hasUrls() == false) {
        return { text };
    }

    QStringList results;
    const auto urls = clipboard->mimeData()->urls();
    for(const auto& url : urls) {
        results.push_back(url.toString());
    }

    return results;
}

void ClipboardActionHandler::mkDirIfNotCreated()
{
    if(saveFolder.isEmpty())
        return;

    QDir dir(saveFolder);
    if (dir.exists())
        return;

    dir.mkdir(".");
}

QString ClipboardActionHandler::saveFile(const QString &filePath)
{
    if(QFile::exists(filePath) == false)
        return QString();

    QFileInfo fileInfo(filePath);
    const QString savedPath = saveFolder + "/" + fileInfo.baseName() + "." + fileInfo.completeSuffix();
    if(QFile::copy(filePath, savedPath)){
        return savedPath;
    }
    else {
        return QString();
    }
}

QString ClipboardActionHandler::saveImage(const QImage &image)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString savedPath = saveFolder + "/" + currentTime.toString("ddhhmmss") + ".png";
    if(image.save(savedPath)) {
        return savedPath;
    }
    else {
        return QString();
    }
}
