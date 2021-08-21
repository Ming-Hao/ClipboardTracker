#ifndef CLIPBOARDACTIONHANDLER_H
#define CLIPBOARDACTIONHANDLER_H

#include <QString>
#include <QImage>
#include <utility>

using ClipInfo = std::pair<QString, QString>;

class QClipboard;

class ClipboardActionHandler
{
public:
    const QString getSaveFolder() const;
    void setSaveFolder(const QString &newSaveFolder);
    QList<ClipInfo> save(QClipboard* clipboard);
    void clear();
    void sendText(const QString& text);

private:
    QStringList getTextOrUrls(QClipboard* clipboard);
    void mkDirIfNotCreated();

    QString saveFile(const QString& filePath);
    QString saveImage(const QImage& image);
private:
    QString saveFolder;
};

#endif // CLIPBOARDACTIONHANDLER_H
