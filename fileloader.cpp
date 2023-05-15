#include "fileloader.h"

#include <QFile>
#include <QDebug>

FileLoader::FileLoader(QObject *parent)
    : QObject{parent}
{
    m_fileSystemWatcher.addPath("/home/user/test/test.txt");
    connect(&m_fileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &FileLoader::handler);
}

void FileLoader::handler(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    qDebug() << "readAll started()";
    QByteArray fileContent = f.readAll();
    if (fileContent == m_content) {
        return;
    }
    qDebug() << "readAll ended()";
    m_content = fileContent;

    emit contentChanged(m_content);
}
