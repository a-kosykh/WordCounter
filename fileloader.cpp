#include "fileloader.h"

#include <QThread>
#include <QDebug>

FileLoader::FileLoader(QObject *parent)
    : QObject{parent}
{

}

void FileLoader::openFile()
{
    if (m_file.isOpen()) {
        m_file.close();
    }

    m_file.setFileName("/home/user/test/test.txt");

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
        if (line.isEmpty() || line.contains(QChar::CarriageReturn)) {
            continue;
        }
        lines.append(line);
        i++;
        if (i == 4096) {
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
