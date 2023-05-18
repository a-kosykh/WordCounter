#include "contentprocessor.h"

#include <QMutexLocker>
#include <QThread>
#include <QTextStream>
#include <QDebug>

ContentProcessor::ContentProcessor(QObject *parent)
    : QObject{parent}
{

}

void ContentProcessor::process(const QString& line, quint64 processedBytes, quint64 totalBytes)
{
    if (QThread::currentThread()->isInterruptionRequested()) {
        return;
    }

    const auto words = line.split(' ');
    for (const QString &word : words) {
        if ( word.trimmed() != QString() ) {
            if (m_wordsCount.contains(word)) {
                m_wordsCount[word].count++;
            } else {
                WordProperties wp{1, QColor(qrand()%255, qrand()%255, qrand()%255)};
                m_wordsCount[word] = wp;
            }
        }
    }

    QVariantList l;
    foreach (QString word, m_wordsCount.keys()) {
        l.append(QVariantMap({{"word", word},
                              {"count", m_wordsCount.value(word).count},
                              {"color", m_wordsCount.value(word).color}}));
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
    setProgress(static_cast<double>(processedBytes) / static_cast<double>(totalBytes));
}

QVariantList ContentProcessor::getTopWords()
{
    QMutexLocker locker(&m_mutex);
    return m_topWords;
}

double ContentProcessor::getProgress()
{
    QMutexLocker locker(&m_mutex);
    return m_progress;
}

void ContentProcessor::setTopWords(const QVariantList &topWords)
{
    QMutexLocker locker(&m_mutex);
    if (m_topWords == topWords) {
        return;
    }
    m_topWords = topWords;
    emit topWordsChanged();
}

void ContentProcessor::setProgress(double progress)
{
    QMutexLocker locker(&m_mutex);
    if (m_progress == progress) {
        return;
    }
    m_progress = progress;
    emit progressChanged();
}
