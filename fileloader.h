#ifndef FILELOADER_H
#define FILELOADER_H

#include <QVariantList>
#include <QHash>
#include <QFile>
#include <QObject>

class FileLoader : public QObject
{
    Q_OBJECT
public:
    explicit FileLoader(int linesPerIteration, QObject *parent = nullptr);
    void openFile(QString fileUrl);
    QVariantList getTopWords() const;
    static QString getFilePathFromUrl(const QString &urlString);

signals:
    void partParsed(const QString& line, quint64 processedBytes, quint64 totalBytes);

private:
    QFile m_file;
    int m_linesPerIteration;
};

#endif // FILELOADER_H
