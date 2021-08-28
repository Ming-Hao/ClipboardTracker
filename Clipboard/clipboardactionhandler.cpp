#include "clipboardactionhandler.h"

#include <QClipboard>
#include <QMimeData>
#include <QDateTime>
#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QProcess>
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

    const QString text = clipboard->text();
    if(text.isEmpty()) {
        return {};
    }

    const auto urls = clipboard->mimeData()->urls();
    if(urls.isEmpty()) {
        return { { text, "" } };
    }

    QList<ClipInfo> results;
    for(const auto& url : urls) {
        if(url.isLocalFile()) {
            results.push_back(std::make_pair(url.toString(), saveFile(url.toLocalFile())));
        }
        else {
            results.push_back(std::make_pair(url.toString(), ""));
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

void ClipboardActionHandler::openFile(const QString &path)
{
    if(path.isEmpty())
        return;

    if(QFile::exists(path) == false)
        return;

    qDebug() << "openFile path: " << path;
    QStringList cmdArgs = {"/select", ",", QDir::toNativeSeparators(path) };

    QProcess process;
    process.setProgram("explorer.exe");
    process.setArguments(cmdArgs);
    qint64 pid;
    process.startDetached(&pid);

}

void ClipboardActionHandler::deleteFile(const QString &path)
{
    if(path.isEmpty())
        return;

    qDebug() << "deleteFile path: " << path;
    QFile::remove(path);
}

void ClipboardActionHandler::mkDirIfNotCreated()
{
    if(saveFolder.isEmpty())
        return;


    QDir dir(getTodayFolder());
    if (dir.exists())
        return;

    dir.mkpath(".");
}

QString ClipboardActionHandler::saveFile(const QString &filePath)
{
    if(QFile::exists(filePath) == false)
        return QString();

    QFileInfo fileInfo(filePath);
    const QString savedPath = getTodayFolder() + "/" + fileInfo.baseName() + "." + fileInfo.completeSuffix();

    if(filePath == savedPath) {
        return savedPath;
    }

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
    const QString savedPath = getTodayFolder() + "/" + currentTime.toString("HHmmss") + ".png";
    if(image.save(savedPath)) {
        return savedPath;
    }
    else {
        return QString();
    }
}

QString ClipboardActionHandler::getTodayFolder() const
{
    const QString todayFolder = saveFolder + "/" +
                                QDateTime::currentDateTime().toString("yyyyMMdd");
    return todayFolder;
}
