#include "fileloader.h"

#include <QUrl>
#include <QDir>
#include <QThread>
#include <QDebug>

FileLoader::FileLoader(int linesPerIteration, QObject *parent)
    : QObject{parent}
{
    if (linesPerIteration == 0) {
        m_linesPerIteration = 1000;
    } else {
        m_linesPerIteration = linesPerIteration;
    }

}

void FileLoader::openFile(QString fileUrl)
{
    if (fileUrl.isEmpty()) {
        return;
    }

    if (m_file.isOpen()) {
        m_file.close();
    }

    m_file.setFileName(getFilePathFromUrl(fileUrl));
    if (!m_file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    quint64 totalSize = m_file.size();

    QTextStream in(&m_file);

    int i = 0;
    QString lines;
    while (!in.atEnd())
    {
        QString line;
        line += in.readLine();
        if (line.isEmpty() || line == "\n") {
            continue;
        }
        lines.append(line);
        lines.append(QChar::Space);
        i++;
        if (i == m_linesPerIteration) {
            if (QThread::currentThread()->isInterruptionRequested()) {
                break;
            }
            emit partParsed(lines, in.pos(), totalSize);
            lines.clear();
            i = 0;
        }
    }
    emit partParsed(lines, in.pos(), totalSize);

    m_file.close();
}

QString FileLoader::getFilePathFromUrl(const QString &urlString) {
    const QUrl url(urlString);
    if (!url.isLocalFile()) {
        return QString();
    }
    return QDir::toNativeSeparators(url.toLocalFile());
}
