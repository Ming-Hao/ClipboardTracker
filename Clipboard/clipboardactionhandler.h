#ifndef CLIPBOARDACTIONHANDLER_H
#define CLIPBOARDACTIONHANDLER_H

#include <QString>
#include <QImage>

#include "myclipboardinfo.h"

class QClipboard;

class ClipboardActionHandler
{
public:
    const QString getSaveFolder() const;
    void setSaveFolder(const QString &newSaveFolder);
    QList<ClipInfo> save(QClipboard* clipboard);
    void clear();
    void sendText(const QString& text);
    void openFile(const QString& path);
    void deleteFile(const QString& path);

private:
    void mkDirIfNotCreated();

    QString saveFile(const QString& filePath);
    QString saveImage(const QImage& image);
    QString getTodayFolder() const;
private:
    QString saveFolder;
};

#endif // CLIPBOARDACTIONHANDLER_H
