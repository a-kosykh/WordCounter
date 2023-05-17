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
            processLine(lines);
            lines.clear();
            i = 0;
        }
    }
    processLine(lines);

    m_file.close();
}

void FileLoader::processLine(const QString &line)
{
    const auto words = line.split(' ');
    for (const QString &word : words) {
        m_wordsCount[word]++;
    }

    QVariantList l;
    foreach (QString word, m_wordsCount.keys()) {
        l.append(QVariantMap({{"word", word}, {"count", m_wordsCount.value(word)}}));
    }

    std::sort(l.begin(), l.end(), [](const QVariant& lhs, const QVariant& rhs) {
        return lhs.toMap().value("count").toInt() > rhs.toMap().value("count").toInt();
    });

    // TODO: rewrite
    QVariantList l2;
    int i = 0;
    for (const auto &word : l) {
        l2.append(word);
        if (i++ >= 15) {
            break;
        }
    }

    setTopWords(l2);
}

QVariantList FileLoader::getTopWords() const
{
    return m_topWords;
}


void FileLoader::handler(const QString &filename)
{

}

void FileLoader::setTopWords(const QVariantList &topWords)
{
    if (m_topWords == topWords) {
        return;
    }
    m_topWords = topWords;
    emit topWordsChanged();
}
