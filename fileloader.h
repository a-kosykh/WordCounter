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
    explicit FileLoader(QObject *parent = nullptr);
    void openFile();
    QVariantList getTopWords() const;

signals:
    void partParsed(const QString& line, quint64 processedBytes, quint64 totalBytes);

private:
    QFile m_file;

};

#endif // FILELOADER_H
