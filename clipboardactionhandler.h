#ifndef CLIPBOARDACTIONHANDLER_H
#define CLIPBOARDACTIONHANDLER_H

#include <QString>
#include <QImage>

class QClipboard;

class ClipboardActionHandler
{
public:
    const QString getSaveFolder() const;
    void setSaveFolder(const QString &newSaveFolder);
    QString save(QClipboard* clipboard);
    void clear();
    void sendText(const QString& text);

private:
    QString getText(QClipboard* clipboard);
    void mkDirIfNotCreated();

    bool saveFile(const QString& filePath);
    bool saveImage(const QImage& image);
private:
    QString saveFolder;
};

#endif // CLIPBOARDACTIONHANDLER_H
