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
                WordProperties wp{1, QColor(qrand() % 255, qrand() % 255, qrand() % 255)};
                m_wordsCount[word] = wp;
            }
        }
    }

    QVector<QVariant> v;
    foreach (QString word, m_wordsCount.keys()) {
        v.append(QVariantMap({{wordProp, word},
                              {countProp, m_wordsCount.value(word).count},
                              {colorProp, m_wordsCount.value(word).color}}));
    }

    std::sort(v.begin(), v.end(), [this](const QVariant& lhs, const QVariant& rhs) {
        return lhs.toMap().value(countProp).toInt() > rhs.toMap().value(countProp).toInt();
    });

    v.resize(qMin(v.size(), topCount));

    setTopWords(v.toList());
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

int ContentProcessor::getMaxCount()
{
    QMutexLocker locker(&m_mutex);
    if (m_topWords.isEmpty()) {
        return 0;
    }
    return m_topWords.at(0).toMap().value(countProp).toInt();
}

void ContentProcessor::clearContents()
{
    m_wordsCount.clear();
    setTopWords({});
    setProgress(0.0);
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
