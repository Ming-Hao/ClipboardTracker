#include "clipboardactionhandler.h"

#include <QClipboard>
#include <QMimeData>
#include <QDateTime>
#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include <Windows.h>

const QString ClipboardActionHandler::getSaveFolder() const
{
    return saveFolder;
}

void ClipboardActionHandler::setSaveFolder(const QString &newSaveFolder)
{
    saveFolder = newSaveFolder;
}

QString ClipboardActionHandler::save(QClipboard *clipboard)
{
    mkDirIfNotCreated();
    auto modelDisplayText = getText(clipboard);
    if(clipboard->mimeData()->hasImage()) {
        if(saveImage(clipboard->image())) {

        }
    }
    else if(clipboard->mimeData()->formats().contains("text/uri-list")) {
        if(saveFile(QUrl(clipboard->text()).toLocalFile())) {

        }
    }
    return modelDisplayText;
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

QString ClipboardActionHandler::getText(QClipboard *clipboard)
{
    const QString clippedText = clipboard->text();
    if(clippedText.isEmpty() == false)
        return clippedText;

    if(clipboard->mimeData()->hasHtml()) {
        const QString htmlText = clipboard->mimeData()->html();
        if(htmlText.isEmpty() == false)
            return htmlText;
    }

    return QString();
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

bool ClipboardActionHandler::saveFile(const QString &filePath)
{
    if(QFile::exists(filePath) == false)
        return false;

    QFileInfo fileInfo(filePath);
    return QFile::copy(filePath,
                       saveFolder + "/" + fileInfo.baseName() + "." + fileInfo.completeSuffix());
}

bool ClipboardActionHandler::saveImage(const QImage &image)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    return image.save(saveFolder + "/" + currentTime.toString("ddhhmmss") + ".png");
}
