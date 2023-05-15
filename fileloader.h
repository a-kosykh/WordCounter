#ifndef FILELOADER_H
#define FILELOADER_H

#include <QFileSystemWatcher>
#include <QObject>

class FileLoader : public QObject
{
    Q_OBJECT
public:
    explicit FileLoader(QObject *parent = nullptr);

signals:
    void contentChanged(const QByteArray& fileContent);

private:
    QFileSystemWatcher m_fileSystemWatcher;
    QByteArray m_content;

private:
    void handler(const QString& filename);

};

#endif // FILELOADER_H
